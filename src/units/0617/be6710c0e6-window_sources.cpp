// lane-prepare: window-source owner = kotor_live_publication::Providers.
// ReInitAurora 00403800 and WinMessageHandler 00402800 run on per-invocation
// native stack frames; each acquisition here supplies a distinct heap block
// (Invocation node + Context + Frame) from the game Win32 heap family
// (006FA7E6/006FA390 = live_operator_new/live_free), recorded in the owner
// list before it is returned. Nested (re-entrant) invocations get distinct
// blocks, so frames never alias active or retiring nodes.
#include "2ff1299369-game_prepare_producers.h"
#include "68794e31bc-prepare_image_data.h"   // D13 generated .rdata strings
#include "bc1db71b7f-functional_runtime.h"
#include "ed8f77ad6d-allocator_live.h"
#include <windows.h>
#include <new>

namespace kotor_game_prepare {
namespace {
using Word = std::uint32_t;
template<class C, class F> struct Block {
    kotor_live_publication::Invocation<C> node;
    C context;
    // D2: the original frame is uninitialized native stack (ReInitAurora 00403800
    // 0040381B sub esp,58h / WinMessageHandler 00402800 locals); the modern frame is
    // value-initialized (zero) instead of reading indeterminate bytes.
    F frame;
    Block* next = nullptr;
    Block* prev = nullptr;
};
using ReinitBlock = Block<kotor_reinit::Context, kotor_reinit::Frame>;
using MessageBlock = Block<kotor_message::Context, kotor_message::Frame>;
inline Word word_of(const void* p) noexcept { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }

class WindowSources {
public:
    void* const heap;
    const DWORD ui_thread;                // thread that ran prepare == WinMain thread
    kotor_menu_live_views::Views* const views;
    // ReInitAurora .rdata literals / thread entry (pushes at 00403942, 00403955,
    // 0040396A, 00403A89, 0040389F, 00403876):
    kotor_reinit::Constants reinit_constants;
    ReinitBlock* reinit_blocks = nullptr;
    MessageBlock* message_blocks = nullptr;
    WindowSources(void* h, kotor_menu_live_views::Views* v) noexcept
        : heap(h), ui_thread(GetCurrentThreadId()), views(v),
          reinit_constants{word_of(kotor_rdata_str_0073d648),                     // 0073D648
                           word_of(kotor_rdata_str_0073d6ac),                   // 0073D6AC
                           word_of(kotor_rdata_str_0073d6c0),      // 0073D6C0
                           word_of(kotor_rdata_str_0073d694),             // 0073D694
                           word_of(kotor_rdata_str_0073d6e0),                      // 0073D6E0
                           static_cast<Word>(reinterpret_cast<std::uintptr_t>(&kotor_stdcall_00401a30))} {} // 00401A30
    bool identity() const noexcept {
        return kotor_functional::source_heap_identity() && live_globals.heap && *live_globals.heap == heap;
    }
};
template<class B> void link(B*& head, B* b) noexcept { b->next = head; if (head) head->prev = b; head = b; }
template<class B> void unlink(B*& head, B* b) noexcept {
    if (b->prev) b->prev->next = b->next; else head = b->next;
    if (b->next) b->next->prev = b->prev;
}
template<class B> void destroy(B*& head, B* b) noexcept { unlink(head, b); b->~B(); live_free(b); }

bool __cdecl ready(void* owner) noexcept {
    auto* s = static_cast<WindowSources*>(owner);
    return s && s->identity() && GetCurrentThreadId() == s->ui_thread;
}
kotor_live_publication::ReinitInvocation* __cdecl acquire_reinit(void* owner) {
    auto* s = static_cast<WindowSources*>(owner);
    if (!ready(owner)) return nullptr;
    void* storage = live_operator_new(sizeof(ReinitBlock));
    if (!storage) return nullptr;
    // D2 value-initialization: ReInitAurora writes only the low byte of the mouse_mode
    // local at 0040382A (sub esp,58h at 0040381B) and reads the full DWORD at 00403BCE.
    auto* b = ::new(storage) ReinitBlock{};
    link(s->reinit_blocks, b);          // recorded before return/any callback
    b->context.globals = &s->views->reinit;
    b->context.constants = &s->reinit_constants;
    b->context.frame = &b->frame;
    b->context.dependencies = {s, &kotor_reinit_invoke, &kotor_winmain_read_word};
    b->node.context = &b->context;
    return &b->node;
}
void __cdecl release_reinit(void* owner, kotor_live_publication::ReinitInvocation* p) noexcept {
    auto* s = static_cast<WindowSources*>(owner);
    for (ReinitBlock* b = s ? s->reinit_blocks : nullptr; b; b = b->next)
        if (&b->node == p && !p->active) { destroy(s->reinit_blocks, b); return; }
    // Unknown/active node: retained (quiesce refuses while it remains).
}
kotor_live_publication::MessageInvocation* __cdecl acquire_message(void* owner, std::uint32_t, std::uint32_t,
                                                                   std::uint32_t, std::uint32_t) {
    auto* s = static_cast<WindowSources*>(owner);
    if (!ready(owner)) return nullptr;
    void* storage = live_operator_new(sizeof(MessageBlock));
    if (!storage) return nullptr;
    // D2 value-initialization of the WinMessageHandler 00402800 frame.
    auto* b = ::new(storage) MessageBlock{};
    link(s->message_blocks, b);
    b->context.globals = &s->views->message;
    b->context.frame = &b->frame;
    b->context.dependencies = {s, &kotor_message_invoke, &kotor_winmain_read_word, &kotor_window_write_word};
    b->node.context = &b->context;
    return &b->node;
}
void __cdecl release_message(void* owner, kotor_live_publication::MessageInvocation* p) noexcept {
    auto* s = static_cast<WindowSources*>(owner);
    for (MessageBlock* b = s ? s->message_blocks : nullptr; b; b = b->next)
        if (&b->node == p && !p->active) { destroy(s->message_blocks, b); return; }
}
// Called by live_publication::detach while views/callbacks stay published.
// ReInitAurora joins its own backdrop worker (00403C8A GetExitCodeThread loop,
// 00403CA5/00403CA8 CloseHandle) before returning, so no worker outlives an
// invocation. The original creates no other window worker and relies on
// process exit (ExitProcess 0073D160) for the top-level window; no extra
// DestroyWindow is invented here. Quiescence = no live invocation remains;
// any retained (rejected) block is reclaimed.
bool __cdecl quiesce(void* owner) noexcept {
    auto* s = static_cast<WindowSources*>(owner);
    if (!s || !s->identity() || GetCurrentThreadId() != s->ui_thread) return false;
    for (ReinitBlock* b = s->reinit_blocks; b; b = b->next) if (b->node.active) return false;
    for (MessageBlock* b = s->message_blocks; b; b = b->next) if (b->node.active) return false;
    while (s->reinit_blocks) destroy(s->reinit_blocks, s->reinit_blocks);
    while (s->message_blocks) destroy(s->message_blocks, s->message_blocks);
    return true;
}
}

bool __cdecl kotor_window_sources_acquire(void* aggregate, kotor_menu_live_views::Views* views,
    kotor_winmain::Context* winmain, kotor_window_shell::Context* shell, kotor_live_publication::Providers* out) {
    if (!aggregate || !views || !winmain || !shell || !out || out->owner ||
        !kotor_functional::source_heap_identity() || !live_globals.heap || !*live_globals.heap) return false;
    void* storage = live_operator_new(sizeof(WindowSources));
    if (!storage) return false;
    auto* s = ::new(storage) WindowSources(*live_globals.heap, views);
    *out = {s, &ready, &acquire_reinit, &release_reinit, &acquire_message, &release_message, &quiesce};
    return true;
}
// GameDrain step (after detach): the owner may end only with no block left.
bool __cdecl kotor_window_sources_drain(void* owner) noexcept {
    auto* s = static_cast<WindowSources*>(owner);
    if (!s) return true;                       // nothing was acquired
    if (!s->identity() || s->reinit_blocks || s->message_blocks) return false;
    s->~WindowSources();
    live_free(s);
    return true;
}
}
