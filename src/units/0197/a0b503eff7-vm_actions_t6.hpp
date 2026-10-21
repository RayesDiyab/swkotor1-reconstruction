#pragma once
// W7 tier t6 exports (ACTION ABI as in vm_actions_t1.hpp; leaves are __thiscall modelled as __fastcall).
extern "C" {
int __fastcall recovered_vm_play_movie_00540ed0(void*, void*, int, unsigned);                 // 733 PlayMovie
int __fastcall recovered_vm_set_return_strref_005434c0(void*, void*, int, unsigned);          // 152 SetReturnStrref
int __fastcall recovered_vm_action_start_conversation_0052d5b0(void*, void*, int, unsigned);  // 204 ActionStartConversation
int __fastcall recovered_vm_give_xp_to_creature_0053e750(void*, void*, int, unsigned);        // 393 GiveXPToCreature
int __fastcall recovered_vm_get_has_skill_00539380(void*, void*, int, unsigned);              // 286 GetHasSkill
void __fastcall recovered_party_set_return_strref_00563ab0(void*, void*, unsigned, unsigned, unsigned);   // RET 0Ch
}
