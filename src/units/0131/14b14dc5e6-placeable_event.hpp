#pragma once
#include "93d73ae698-w1_ports.hpp"
// G05: 00587BA0 CSWSPlaceable::EventHandler(code, caller, data, day, ms), vtable +0x78, thiscall ret 0x14.
namespace w1::placeable_event {
// `frame` = 0x4C bytes (frame+N == original [esp+N] after `push ebp`; +0x38..+0x48 are the argument slots, reused as locals).
void event_handler_00587ba0(U placeable, U code, U caller, U data, U day, U ms, U frame, Ports& p);
}
