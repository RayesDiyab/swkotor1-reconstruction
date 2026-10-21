// lane-prepare: WinMain/Shell/ReInit/Message dispatch (Bindings::invoke,
// read_word, write_word) over the existing M40 window bridges plus the
// lane-routes route table (kotor_dispatch_routes.h). Consumer contracts:
// winmain.cpp call()/read() (Call -> Tick), window_shell.cpp call() (Result),
// reinit.cpp call()/virt()/api() and message.cpp call()/read()/write().
// No default result exists: a Call without a bridge or table row terminates.
#include "2ff1299369-game_prepare_producers.h"
#include "dfce5ef33e-kotor_dispatch_routes.h"
#include "2a0f55241f-bridges.h"
#include <windows.h>
#include <intrin.h>
#include <cstdio>

namespace {
using kotor_dispatch::Word;
// EXPLICIT UNREACHABLE-ROUTE TRAP (exit 97). It is not a success fixture and
// produces no value: it fires only for a Call with no bridge and no
// lane-routes row (or a null game address). Requirement: once lane-routes
// delivers route_table, no first-frame route (ROUTES.json required_pre_frame1 /
// conditional_pre_frame1 on the run configuration) may reach it. Mirrors
// release_contract.cpp's non-unwinding termination (no CRT exit path).
[[noreturn]] void unbound(const char* consumer, Word site, Word target) noexcept {
    char text[128];
    const int n = std::snprintf(text, sizeof(text),
        "KOTOR source host: unbound %s route site=%08X target=%08X; terminating.\r\n",
        consumer, site, target);
    DWORD written = 0;
    if (n > 0) WriteFile(GetStdHandle(STD_ERROR_HANDLE), text, static_cast<DWORD>(n), &written, nullptr);
    TerminateProcess(GetCurrentProcess(), 97u);
    __fastfail(7);
}
template<class R, class C>
const R* find(const R* rows, unsigned n, const C& c, Word kind, Word slot) noexcept {
    for (unsigned i = 0; i < n; ++i) {
        const R& r = rows[i];
        if (r.site != c.site || r.count != c.count || r.kind != kind || r.slot != slot || !r.bind) continue;
        if (kind != kotor_dispatch::kind_virtual_slot && r.target != c.target) continue;
        return &r;
    }
    return nullptr;
}
}

namespace kotor_game_prepare {
// winmain.cpp:5-8 call(); WinMain bridge first (00403F20 at 004042DF,
// 00403800 at 00404663), then the route table.
kotor_winmain::Tick __cdecl kotor_winmain_invoke(void* context, const kotor_winmain::Call& c) {
    if (!context) unbound("winmain(context)", c.site, c.target);
    kotor_winmain::Tick result = 0;
    if (kotor_menu_window::winmain_window_route(c, &result)) return result;
    const auto& t = kotor_dispatch::route_table;
    if (const auto* r = find(t.winmain, t.winmain_count, c, kotor_dispatch::kind_direct, 0)) return r->bind(c);
    unbound("winmain", c.site, c.target);
}
// Native DWORD load of a game address (e.g. 'mov ecx,[eax+4]' at 00404449).
// A null base faults in the original; here it terminates without a value.
kotor_winmain::Word __cdecl kotor_winmain_read_word(void* context, kotor_winmain::Word address) {
    if (!context || !address) unbound("read_word", 0, address);
    return *reinterpret_cast<const volatile kotor_winmain::Word*>(static_cast<std::uintptr_t>(address));
}
void __cdecl kotor_window_write_word(void* context, kotor_message::Word address, kotor_message::Word value) {
    if (!context || !address) unbound("write_word", 0, address);
    *reinterpret_cast<volatile kotor_message::Word*>(static_cast<std::uintptr_t>(address)) = value;
}
// window_shell.cpp call(); existing shell_import (six user32/gdi imports) first.
kotor_window_shell::Result __cdecl kotor_shell_invoke(void* context, const kotor_window_shell::Call& c) {
    if (!context) unbound("shell(context)", c.site, c.target);
    kotor_window_shell::Result result = 0;
    if (kotor_menu_window::shell_import(c, &result)) return result;
    const auto& t = kotor_dispatch::route_table;
    if (const auto* r = find(t.shell, t.shell_count, c, kotor_dispatch::kind_direct, 0)) return r->bind(c);
    unbound("shell", c.site, c.target);
}
// reinit.cpp call()/virt()/api(): Kind selects direct, IAT slot or vtable slot.
kotor_reinit::Result __cdecl kotor_reinit_invoke(void* context, const kotor_reinit::Call& c) {
    if (!context) unbound("reinit(context)", c.site, c.target);
    const auto& t = kotor_dispatch::route_table;
    if (const auto* r = find(t.reinit, t.reinit_count, c, static_cast<Word>(c.kind), c.slot)) return r->bind(c);
    unbound("reinit", c.site, c.target);
}
// message.cpp call(); existing message_import first, then the table.
kotor_message::Result __cdecl kotor_message_invoke(void* context, const kotor_message::Call& c) {
    if (!context) unbound("message(context)", c.site, c.target);
    kotor_message::Result result = 0;
    if (kotor_menu_window::message_import(c, &result)) return result;
    const auto& t = kotor_dispatch::route_table;
    if (const auto* r = find(t.message, t.message_count, c, static_cast<Word>(c.kind), c.slot)) return r->bind(c);
    unbound("message", c.site, c.target);
}
}
