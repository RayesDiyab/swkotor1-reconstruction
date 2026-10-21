#pragma once
#include "93d73ae698-w1_ports.hpp"
// Tier 9 (server boot / combat): SetCombatState and the CSWSRules constructor family.
namespace w1::combat_state {
// CSWSCreature::SetCombatState(int state, BYTE flag), thiscall ret 8. Frame 0x30 (+0x20/+0x28 debug CExoStrings).
void set_combat_state_004f2610(U creature, U state, U flag, U frame, Ports& p);
void combat_round_remove_all_actions_004d3770(U round, Ports& p);   // CSWSCombatRound::RemoveAllActions, ret
U rules_server_construct_0057a1f0(U self, Ports& p);               // CSWSRules::CSWSRules, ret (returns this)
void rules_server_destroy_0057a210(U self, Ports& p);              // CSWSRules::~CSWSRules
U rules_server_scalar_destroy_0057a220(U self, U flags, Ports& p); // scalar deleting destructor, ret 4 (vtable slot 0)
}
