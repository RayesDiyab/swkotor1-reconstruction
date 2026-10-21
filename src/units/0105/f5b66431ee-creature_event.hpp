#pragma once
#include "93d73ae698-w1_ports.hpp"
// G04: 004FECE0 CSWSCreature::EventHandler(code, caller id, data, unused, unused), creature vtable +0x78, thiscall ret 0x14.
namespace w1::creature_event {
// `frame` = 0x124 bytes: 0x54 own (frame+N == original [esp+N] after the prologue; +0x40/+0x44/+0x48 are the code,
// caller and data argument slots, reused as locals) + 0xD0 SpawnInHeartbeatPerception frame at frame+0x54.
void event_handler_004fece0(U creature, U code, U caller, U data, U frame, Ports& p);
}
