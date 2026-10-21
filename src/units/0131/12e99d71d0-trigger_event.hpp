#pragma once
#include "93d73ae698-w1_ports.hpp"
// G05: 0058F140 CSWSTrigger::EventHandler(code, caller, data, unused, unused), vtable +0x78, thiscall ret 0x14.
namespace w1::trigger_event {
// `frame` = 0x40 bytes (frame+N == original [esp+N] after `push edi`; +0x34/+0x38/+0x3C are the argument slots).
void event_handler_0058f140(U trigger, U code, U caller, U data, U frame, Ports& p);
}
