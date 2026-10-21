#pragma once
#include "93d73ae698-w1_ports.hpp"
// G05: 0058B850 CSWSDoor::EventHandler(code, caller, data, day, ms), vtable +0x78, thiscall ret 0x14.
namespace w1::door_event {
// 005890D0 CSWSDoor::GetIsLinked: 1 when byte [+0x384] is 1 or 2. Leaves ECX untouched (callers rely on it).
U door_is_linked_005890d0(U door, Ports& p);
// `frame` = 0x68 bytes (frame+N == original [esp+N] after `push ebp`; +0x54..+0x64 are the argument slots, reused as locals).
void event_handler_0058b850(U door, U code, U caller, U data, U day, U ms, U frame, Ports& p);
}
