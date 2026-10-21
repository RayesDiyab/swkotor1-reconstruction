#pragma once
#include "93d73ae698-w1_ports.hpp"
// 004FAF20 CSWSCreature::UpdateCombatRoundTimer and its combat-round timer leaves.
namespace w1::combat_timer {
// 004D2B50 CSWSCombatRound::GetAttack(index), thiscall ret 4.
U round_attack_004d2b50(U round, U index);
// 004D4C10 CSWSCombatRound::IncrementTimer(dt), thiscall ret 4. `scratch` = 0x10 bytes (two debug CExoStrings).
void increment_timer_004d4c10(U round, U dt, U scratch, Ports& p);
// 004FAF20 CSWSCreature::UpdateCombatRoundTimer, thiscall ret. `scratch` = 0x10 bytes.
void update_combat_round_timer_004faf20(U creature, U scratch, Ports& p);
}
