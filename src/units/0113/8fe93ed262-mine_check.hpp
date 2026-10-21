#pragma once
#include "93d73ae698-w1_ports.hpp"
// 004FA390 CSWSCreature::UpdateMineCheck and its trigger geometry leaf.
namespace w1::mine_check {
// 00550B60 CSWRules::RollDice(BYTE count, BYTE sides), thiscall ret 8 (rules object unused). Returns AX.
U roll_dice_00550b60(U count, U sides, Ports& p);
// 0058C8A0 CSWSTrigger::CalculateNearestPoint(const Vector* in, Vector* out), thiscall ret 8. `frame` = 0x64 bytes.
void nearest_point_0058c8a0(U trigger, U in, U out, U frame, Ports& p);
// 004FA390 CSWSCreature::UpdateMineCheck, thiscall ret. `frame` = 0xC4 bytes (+0x60: CalculateNearestPoint frame).
void update_mine_check_004fa390(U creature, U frame, Ports& p);
}
