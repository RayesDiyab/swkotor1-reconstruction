#pragma once
#include "bd291ef956-w1_ports.hpp"
#include "2cb5482018-w1_x87.hpp"
// G03 perception refresh: CSWSCreature::UpdateVisibleList and its small W1-owned leaves.
namespace w1::perception {
// 004EFC70 / 004EFD20 CSWSCreature::Get{Spot,Listen}CheckDistance, thiscall ret, float in ST0 (returned as bits).
// `frame` = 0x10-byte scratch: frame+4 result float, frame+8 CExoString column name.
U spot_distance_004efc70(U creature, U frame, Ports& p);
U listen_distance_004efd20(U creature, U frame, Ports& p);
// 00517A20 CSWSCreature::GetVisibleListElement(id), thiscall ret 4.
U visible_element_00517a20(U creature, U id, Ports& p);
// 00517960 CSWSCreature::RemoveFromVisibleList(id), thiscall ret 4.
void remove_visible_00517960(U creature, U id, Ports& p);
// 0051B050 CSWSCreature::UpdateVisibleList(BYTE mode), thiscall ret 4. `frame` = 0x40-byte scratch
// (frame+N == [esp+N] after the prologue; frame+0x2c is the argument slot) + 0x10 at frame+0x40 for the range getters.
void update_visible_0051b050(U creature, U frame, Ports& p);
// 00502AC0 CSWSCreature::DoPerceptionUpdateOnCreature(CSWSCreature* target), thiscall ret 4.
// `frame` = 0x80-byte scratch: frame+N == [esp+N] after the last prologue push (0x70 bytes) + 0x10 range-getter frame.
void do_perception_00502ac0(U creature, U target, U frame, Ports& p);
// 00500C00 CSWSCreature::DoStealthDetection(target, line_of_sight, int* spot_out, int* listen_out, int flag), ret 14h.
U stealth_detection_00500c00(U creature, U target, U los, U spot_out, U listen_out, U flag, Ports& p);
// 0051B370 CSWSCreature::AddToVisibleList(id, seen, heard, a4, flag), ret 14h.
void add_visible_0051b370(U creature, U id, U a2, U a3, U a4, U a5, Ports& p);
// 005179E0 CSWSCreature::CheckVisibleList(id), ret 4.
U check_visible_005179e0(U creature, U id, Ports& p);
// 00634620 CSWParty::IsPartyMember(client id), ret 4.
U party_member_00634620(U party, U id, Ports& p);
// 00501950 CSWSCreature::GetInvisible(viewer object), thiscall ret 4. `scratch` = 4-byte slot (reputation lookup).
U invisible_00501950(U creature, U viewer_object, U scratch, Ports& p);
// 00503690 CGameEffect::GetInteger(index), thiscall ret 4.
U effect_integer_00503690(U effect, U index, Ports& p);
// 0057CD90 CSWSObject::GetAIStateReputation(id), thiscall ret 4 (AL: 1 friendly, 2 hostile, 0 other).
U ai_state_reputation_0057cd90(U object, U other_id, Ports& p, U scratch);
// 004FDA20 CSWSCreature::PacifyCreature(id), thiscall ret 4.
void pacify_004fda20(U creature, U other_id, Ports& p);
// 004FB4B0 CSWSCreature::DoListenDetection(target, flag), thiscall ret 8. `frame` = 0xB8 bytes
// (frame+N == [esp+N] after the prologue; +0x70 range getter, +0x88 ST0 slot, +0x94 attack-range scratch).
U listen_detection_004fb4b0(U creature, U target, U flag, U frame, Ports& p);
// 004F1FD0 CSWSCreature::DoSpotDetection(target, flag), thiscall ret 8. `frame` = 0x8C bytes (+0x70 getter, +0x80 ST0).
U spot_detection_004f1fd0(U creature, U target, U flag, U frame, Ports& p);
// 004FB0F0 CSWSCreature::MaxAttackRange(id, a2, a3) / 004F1310 DesiredAttackRange(id, flag): ST0 results (80-bit).
x87::X80 max_attack_range_004fb0f0(U creature, U id, U a2, U a3, U scratch, Ports& p);
x87::X80 desired_attack_range_004f1310(U creature, U id, U flag, U scratch, Ports& p);
// 004EDAE0 GetDetectMode (AL), 004CAE60 AnimationStationary(WORD), 004EFB20 GetClientCreature.
U detect_mode_004edae0(U creature, Ports& p);
U animation_stationary_004cae60(U animation, Ports& p);
U client_creature_004efb20(U creature, Ports& p);
// 00563A00 CSWPartyTable::GetIsLeader(id), ret 4.
U party_leader_00563a00(U table, U id, Ports& p);
}
