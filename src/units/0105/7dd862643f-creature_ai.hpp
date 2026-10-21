#pragma once
#include "93d73ae698-w1_ports.hpp"
// G03: CSWSCreature::AIUpdate (creature vtable slot +0x70) and its small W1-owned leaves.
namespace w1::creature_ai {
// 004FE210 CSWSCreature::AIUpdate: thiscall (ECX = creature), plain ret.
// `frame` = 0x27C-byte scratch (0x44 own + 0xD0 SpawnInHeartbeatPerception + 0x10 UpdateCombatRoundTimer at frame+0x114 + 0x54 UpdateEffectList at frame+0x124 + 0x40 RunActions at frame+0x178 + 0xC4 UpdateMineCheck at frame+0x1B8 at frame+0x44) standing for the original ESP-relative locals (frame+N == original [esp+N] after prologue).
void ai_update_004fe210(U creature, U frame, Ports& p);
// 004EB6E0 CSWSCreature::SpawnInHeartbeatPerception(int), thiscall ret 4. `frame` = 0xD0-byte scratch: 0x4c own locals
// + 0x34 for CleanInvitationLists at frame+0x4c + 0x50 for UpdateVisibleList at frame+0x80
// (frame+N == original [esp+N] after the last prologue push; frame+0x48 is the argument slot, reused as a byte local).
void spawn_heartbeat_perception_004eb6e0(U creature, U frame, Ports& p);
// 004AE830 CServerExoApp::GetActiveTimer (thunk) + 004B6C40 CServerExoAppInternal::GetActiveTimer, ret 4.
U active_timer_004ae830(U server, U id, Ports& p);
// 004ED110 CSWSCreature::UpdateExcitedStateTimer, ret.
void excited_timer_004ed110(U creature, Ports& p);
// 004F1460 CSWSCreature::ComputeAIState, ret. `scratch` = 4-byte list position slot.
void compute_ai_state_004f1460(U creature, U scratch, Ports& p);
// 004EBB70 CSWSCreature::ComputeAIStateOnAction(type, target), thiscall ret 8.
void compute_ai_state_on_action_004ebb70(U creature, U type, U target, Ports& p);
// 005A5C70 / 005A5CC0 CSWSCreatureStats::Adjust{HP,FP}AccumulatorBy(float), ret 4. Returns EAX (__ftol2 low dword; callers sign-extend AX).
U adjust_hp_accumulator_005a5c70(U stats, U amount_bits, Ports& p);
U adjust_fp_accumulator_005a5cc0(U stats, U amount_bits, Ports& p);
// 00506AA0 CSWSArea::SetCurrentStealthXP, ret 4.
void set_stealth_xp_00506aa0(U area, U value, Ports& p);
// 006349C0 CSWParty::GetIndex(client object id), ret 4. Returns -1 when absent.
U party_index_006349c0(U party, U id, Ports& p);
}
