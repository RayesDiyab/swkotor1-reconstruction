#pragma once
// ONE agreed header between lane-prepare (dispatch owner) and lane-routes
// (route-binding table producer). lane-prepare defines the four dispatchers and
// the window-source Providers; lane-routes defines kotor_dispatch::route_table
// (a stage-ready, sorted-by-nothing const table, one row per original call
// site). A row is the ONLY way a consumer Call reaches a body; there is no
// default/success row. An unmatched Call is a fail-closed terminal.
//
// Matching key (all fields must be equal to the consumer Call):
//   site   - consumer's original call-instruction address (Call::site)
//   target - Call::target (direct callee VA, IAT slot VA for imports, or the
//            loaded vtable entry for virtual_slot calls - see kind/slot)
//   count  - Call::count (exact stack-word count the consumer supplied)
//   kind   - 0 direct, 1 import_slot, 2 virtual_slot (reinit/message Calls;
//            WinMain/Shell Calls have no kind and match kind==0)
//   slot   - vtable byte offset for virtual_slot rows, else 0
//            (virtual rows match on site+kind+slot, target is not compared
//             because it is a runtime vtable load)
// bind receives the consumer's full Call (self, args, x87 operand) and returns
// the native EAX (or EDX:EAX for 64-bit results) as the consumer expects.
#include "cda5e2e155-winmain.h"
#include "1b70edc205-window_shell.h"
#include "8c5bd7d913-reinit.h"
#include "10f537aa03-message.h"
#include <cstdint>

namespace kotor_dispatch {
using Word = std::uint32_t;
enum : Word { kind_direct = 0, kind_import_slot = 1, kind_virtual_slot = 2 };
template<class CallT, class ResultT> struct Route {
    Word site, target, count, kind, slot;
    ResultT (__cdecl* bind)(const CallT&);
};
using WinMainRoute = Route<kotor_winmain::Call, kotor_winmain::Tick>;
using ShellRoute = Route<kotor_window_shell::Call, kotor_window_shell::Result>;
using ReinitRoute = Route<kotor_reinit::Call, kotor_reinit::Result>;
using MessageRoute = Route<kotor_message::Call, kotor_message::Result>;
struct RouteTable {
    const WinMainRoute* winmain; unsigned winmain_count;
    const ShellRoute* shell; unsigned shell_count;
    const ReinitRoute* reinit; unsigned reinit_count;
    const MessageRoute* message; unsigned message_count;
};
// [lane-routes] stage-ready definition.
extern const RouteTable route_table;
}
