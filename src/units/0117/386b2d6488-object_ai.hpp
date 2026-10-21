#pragma once
#include "93d73ae698-w1_ports.hpp"
// G05: placeable / door / trigger AIUpdate (vtable +0x70), thiscall plain ret.
// `frame` = 0xB0 bytes: 0x1C own (frame+N == original [esp+N] after the prologue: +0xC ms, +0x10 day, +0x14 elapsed ms,
// +0x18 elapsed days) + 0x54 UpdateEffectList frame at +0x1C + 0x40 RunActions frame at +0x70.
namespace w1::object_ai {
void placeable_ai_update_005849d0(U placeable, U frame, Ports& p);
void door_ai_update_005889c0(U door, U frame, Ports& p);
void trigger_ai_update_0058d760(U trigger, U frame, Ports& p);
}
