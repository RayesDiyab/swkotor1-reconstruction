#pragma once
#include "93d73ae698-w1_ports.hpp"
// Object state queries reached by AIUpdate through the creature vtable.
namespace w1::object_state {
// 004CB810 CSWSObject::GetDead, thiscall ret: current HP (vtable +0x9C, arg 0) <= -10 for a creature in the death-anim
// state (+0x9D4), else <= 0.
U object_get_dead_004cb810(U object, Ports& p);
// 004EF820 CSWSCreature::GetDead (vtable +0x94), thiscall ret: 0 for a client party member, else 004CB810.
U creature_get_dead_004ef820(U creature, Ports& p);
// 004CAEC0 CSWSObject::GetCurrentHitPoints(int base_only) (vtable +0x9C), thiscall ret 4. base_only == 1 -> AX = [+0xDC]
// (the upper EAX half is the caller's, returned here as 0); else [+0xE4] + [+0xDC].
U object_current_hp_004caec0(U object, U base_only, Ports& p);
// 0050D340 CSWSArea CGameObject-subobject AsSWSArea thunk (vtable 00747B88 +0x28): this - 0x11C.
inline U area_as_sws_area_0050d340(U game_object) { return game_object - 0x11c; }
// 004C6BB0 CSWSModule CGameObject-subobject AsSWSModule thunk (vtable 007458F0 +0x20): this - 0x1C.
inline U module_as_sws_module_004c6bb0(U game_object) { return game_object - 0x1c; }
}
