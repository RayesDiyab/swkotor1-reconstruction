#pragma once
// W6 tier t5: ACTION handlers reached by the Endar opening (k_ai_master branches), claims in lane-w7/OWNERSHIP.md
// ("W6 t5 claims"). Reconstructed from the original (dis/t5_a.txt, dis/t5_b.txt, dis/t5_speak.txt):
//   00536E40 ExecuteCommandGetCommandable (163)            0053CF70 ExecuteCommandGetPlayerRestrictMode (83)
//   0053BD20 ExecuteCommandGetListenPatternNumber (195)    00536A80 ExecuteCommandGetBlockingDoor (336)
//   0053D110 ExecuteCommandGetSpellCast (245 GetLastSpellCaster, 246 GetLastSpell, 423 GetLastSpellHarmful)
//   0053C080 ExecuteCommandGetName (253)                   00544090 ExecuteCommandSpeakString (221, 39 ActionSpeakString)
// round 2 (dis/t5_c.txt): 00536860 GetAttemptedAttackTarget (361), 0053B2A0 GetLastPerception (257/258/259/261),
//   0053B410 GetLastSpeaker (254), 00549040 GetCurrentAction (522), 005361B0 GetAbilityScore (139),
//   leaf 0057A2B0 CSWSObject::GetAQActionIDByID (table decoded by tools/aq_table.py)
// round 3 (dis/t5_d.txt): 005368B0 GetAttemptedSpellTarget (375), 0053AF40 GetLastHostileActor (556),
//   leaf 004CC0C0 CSWSObject::SetLastHostileActor (with CSWSObject::GetArea 004CB120 inlined as a helper)
// round 4 (dis/t5_e.txt): 00536800 GetAttackTarget (316), 0053ECB0 GetIsDead (140), 0053AD40 GetLastDamager (346),
//   00540170 ObjectToString (272), 00543840 ShipBuild (761; .data cell 00832834)
// round 5 (dis/t5_f.txt): 0053C780 GetObjectVisibility (289 GetObjectSeen, 290 GetObjectHeard), 00537CE0 GetEmotions
//   (235 GetIsEnemy, 236 GetIsFriend, 237 GetIsNeutral), 005459A0 GetPartyAIStyle (704)
// round 6 (dis/t5_g.txt): 005459D0 GetNPCAIStyle (705)
// round 7 (dis/t5_h.txt): 0053E1B0 GetUserActionsPending (514) and its leaf 00518B30 CSWSCreature::GetUserActionsPending
// Providers are ports (live owners bound by symbol in the native adapter).
#include "8f2a5e8ea0-server_mainloop.hpp"
namespace kotor::vm_commands_t5 {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
constexpr I32 kPushFailed=-2000,kPopFailed=-2001;
constexpr Word kInvalid=0x7f000000;
struct Ports{
 virtual ~Ports()=default;
 // VM
 virtual Word pop_integer(Word* out)=0;
 virtual Word pop_object(Word* out)=0;
 virtual Word pop_string(Word dst)=0;
 virtual Word push_integer(Word)=0;
 virtual Word push_object(Word)=0;
 virtual Word push_string(Word src)=0;
 // server
 virtual Word server_app()=0;
 virtual Word object_array_004aed70(Word server)=0;   // CServerExoApp::GetObjectArray (W2 export)
 virtual Word get_game_object_004d8230(Word array,Word id,Word* out)=0;   // status byte (0 = found; out = 0 otherwise)
 virtual Word get_creature_by_id_004ae770(Word server,Word id)=0;
 virtual Word get_module_004ae6b0(Word server)=0;
 virtual Word module_get_area_004c30f0(Word module)=0;
 virtual Word object_virtual(Word object,Word offset)=0;               // thiscall, no arguments
 virtual Word get_locstring_server_00578870(Word player,Word first,Word last,Word out,Word gender)=0;
 virtual void send_chat_00571960(Word message,Word mode,Word id,Word string_by_value,Word player,Word extra)=0;  // callee destroys the by-value string
 virtual Word add_action_004cea20(Word object,Word type,Word group,const Word (&params)[26])=0;
 virtual Word get_game_object_004ae750(Word server,Word id)=0;
 virtual Word acceptable_action_0057a240(Word object,Word type)=0;
 virtual Word list_next_005e9b90(Word internal,Word position_cell)=0;   // CExoLinkedListInternal::GetNext(&pos): data
 virtual Word list_at_pos_005e9b60(Word internal,Word position)=0;      // CExoLinkedListInternal::GetAtPos(pos): data
 virtual Word creature_stat(Word stats,Word ability)=0;
 virtual Word dead_temp_004ef890(Word creature)=0;
 virtual Word visible_element_00517a20(Word creature,Word id)=0;        // CSWSCreature::GetVisibleListElement (W1)
 virtual Word get_reputation_0057cb80(Word object,Word id,Word flag)=0;  // CSWSObject::GetReputation (W1)        // CSWSCreature::GetDeadTemp
 virtual Word return_zero_0063e7f0(Word creature)=0;      // the original's `return_zero` virtual-less call                 // 005A6190/6550/6250/6310/63D0/6490 by ability 0..5
 // strings
 virtual void string_default_005b3190(Word)=0;
 virtual void string_literal_005e5a90(Word,const char*)=0;
 virtual void string_copy_005e5b00(Word dst,Word src)=0;
 virtual void string_destroy_005e5c20(Word)=0;
 virtual void string_format_hex_005e5680(Word out,Word value)=0;   // CExoString::Format(out, "%x" [0073E2E8], value)
 virtual Word string_cstr_005e5670(Word)=0;
 virtual void ship_build_flag_00832834(Word value)=0;             // .data cell written only by ShipBuild (no reader in the original)
};
// scratch: caller-owned 8-byte CExoString cells (the original's stack locals)
I32 get_commandable_00536e40(Memory&,Ports&,Word implementer,Word argc);
I32 get_player_restrict_mode_0053cf70(Memory&,Ports&);
I32 get_listen_pattern_number_0053bd20(Memory&,Ports&,Word implementer);
I32 get_blocking_door_00536a80(Memory&,Ports&,Word implementer);
I32 get_spell_cast_0053d110(Memory&,Ports&,Word implementer,Word routine);
I32 get_name_0053c080(Memory&,Ports&,Word string_scratch);
I32 get_attempted_attack_target_00536860(Memory&,Ports&,Word implementer);
I32 get_last_perception_0053b2a0(Memory&,Ports&,Word implementer,Word routine);
I32 get_last_speaker_0053b410(Memory&,Ports&,Word implementer);
I32 get_current_action_00549040(Memory&,Ports&,Word position_cell);
I32 get_ability_score_005361b0(Memory&,Ports&);
Word aq_action_id_by_id_0057a2b0(Word type,Word flag);
I32 get_attempted_spell_target_005368b0(Memory&,Ports&,Word implementer);
I32 get_last_hostile_actor_0053af40(Memory&,Ports&);
void set_last_hostile_actor_004cc0c0(Memory&,Ports&,Word self,Word actor,Word force);   // CSWSObject::SetLastHostileActor (leaf)
I32 get_attack_target_00536800(Memory&,Ports&);
I32 get_is_dead_0053ecb0(Memory&,Ports&);
I32 get_last_damager_0053ad40(Memory&,Ports&,Word implementer);
I32 object_to_string_00540170(Memory&,Ports&,Word tmp_scratch,Word string_scratch);
I32 ship_build_00543840(Memory&,Ports&);
I32 get_object_visibility_0053c780(Memory&,Ports&,Word routine);
I32 get_emotions_00537ce0(Memory&,Ports&,Word implementer,Word routine,Word argc);
I32 get_party_ai_style_005459a0(Memory&,Ports&);
I32 get_npc_ai_style_005459d0(Memory&,Ports&);
I32 get_user_actions_pending_0053e1b0(Memory&,Ports&,Word implementer,Word position_cell);
Word creature_user_actions_pending_00518b30(Memory&,Ports&,Word creature,Word position_cell);   // CSWSCreature::GetUserActionsPending (leaf)   // CSWSObject::GetAQActionIDByID (leaf; `this` unused)
I32 speak_string_00544090(Memory&,Ports&,Word implementer,Word routine,Word string_scratch,Word empty_scratch,Word copy_scratch,Word mode_cell);
}
