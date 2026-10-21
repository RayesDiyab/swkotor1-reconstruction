// lane-window (D37): exported cdecl adapter for SetVideoMode 005F1830's ReInitAurora calls
// (005F196F..005F19B2: push check_movies, fullscreen, bits, h, w; call 00403800; add esp,14h).
// Binds lane-config video_mode_settings.h Bindings::reinit_aurora_00403800
// (void (__cdecl*)(U32,U32,U32,U32,U32)) to the existing M41 ReInitAurora owner
// kotor_reinit::reinit_00403800 (window-reinit.obj), entering/leaving the reinit context
// through the same task-6 publication hooks as window-bridges.obj's own reinit()
// (bridges.cpp: enter_reinit -> bound_context scope -> reinit_00403800 -> leave_reinit).
// Integration adapter only: no original body, no default value, invalid publication -> exit 86.
// lane-m50-diag NON-ORIGINAL HOST DIAGNOSTIC REPLACEMENT COPY of M49 unit aurora-window-reinit-adapter
// (aurora-window/reinit_adapter.cpp sha 3fe46672.. obj). Only change: invalid_binding() takes a stable check
// id and reports it via kotor_host_diag (D94 proposal) before the unchanged ExitProcess(86).
#include "92652a06f2-bridges.h"

namespace {
using Word = std::uint32_t;
[[noreturn]] void invalid_binding(const char*) { ; ExitProcess(86); }   // same policy as window-bridges
struct ReinitScope {
    kotor_reinit::Context* prior; kotor_reinit::Context* current;
    void (__cdecl* leave)(kotor_reinit::Context*) noexcept;
    ReinitScope(kotor_reinit::Context* c, void (__cdecl* done)(kotor_reinit::Context*) noexcept)
        : prior(kotor_reinit::bound_context), current(c), leave(done) { kotor_reinit::bound_context = c; }
    ~ReinitScope() { kotor_reinit::bound_context = prior; leave(current); }
    ReinitScope(const ReinitScope&) = delete; ReinitScope& operator=(const ReinitScope&) = delete;
};
}

extern "C" void __cdecl kotor_window_reinit_aurora_00403800(Word width, Word height, Word depth, Word fullscreen, Word check_movies) {
    const auto* p = kotor_menu_window_publication;
    if (!p) invalid_binding("reinit_adapter.publication_null");
    if (!p->enter_reinit) invalid_binding("reinit_adapter.enter_reinit_null");
    if (!p->leave_reinit) invalid_binding("reinit_adapter.leave_reinit_null");
    auto* c = p->enter_reinit();
    if (!c) invalid_binding("reinit_adapter.context_null");
    if (!c->globals) invalid_binding("reinit_adapter.globals_null");
    if (!c->constants) invalid_binding("reinit_adapter.constants_null");
    if (!c->frame) invalid_binding("reinit_adapter.frame_null");
    if (!c->dependencies.invoke) invalid_binding("reinit_adapter.invoke_null");
    if (!c->dependencies.read_word) invalid_binding("reinit_adapter.read_word_null");
    ReinitScope scope(c, p->leave_reinit);
    kotor_reinit::reinit_00403800(width, height, depth, fullscreen, check_movies);
}
