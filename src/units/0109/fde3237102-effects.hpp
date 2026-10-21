#pragma once
#include "93d73ae698-w1_ports.hpp"
// G03 effect list update (004D1730) and effect removal leaves.
namespace w1::effects {
// 004D1730 CSWSObject::UpdateEffectList(day, ms), thiscall ret 8. `frame` = 0x54 bytes (frame+N == [esp+N] after the
// prologue; +0x4C/+0x50 are the argument slots, +0x50 doubles as the "restart scan" flag).
void update_effect_list_004d1730(U object, U day, U ms, U frame, Ports& p);
// 004D06C0 CSWSObject::RemoveEffectById(id_lo, id_hi), thiscall ret 8. Returns 1 when an effect was removed.
U remove_effect_by_id_004d06c0(U object, U id_lo, U id_hi, Ports& p);
// 00503590 CGameEffect::~CGameEffect, thiscall ret.
void effect_destroy_00503590(U effect, Ports& p);
// 004CF3F0 CSWSObject::RemoveEffectTarget(owner id), thiscall ret 4.
void remove_effect_target_004cf3f0(U creator, U owner_id, Ports& p);
// 0066DCC0 CExoArrayList<int>::remove(value), thiscall ret 4.
void int_list_remove_0066dcc0(U list, U value, Ports& p);
// 005036C0 CGameEffect::GetFloat(i) (ST0 = float), 005036D0 SetFloat(i, f), 00503770 GetExpiryTime(&day, &ms).
U effect_float_005036c0(U effect, U index, Ports& p);
void effect_set_float_005036d0(U effect, U index, U value, Ports& p);
void effect_expiry_00503770(U effect, U day_out, U ms_out, Ports& p);
void remove_effect_004d05c0(U object, U effect, Ports& p);              // CSWSObject::RemoveEffect, ret 4
void remove_effect_by_creator_004d0820(U object, U creator_id, Ports& p);  // CSWSObject::RemoveEffectByCreator, ret 4
}
