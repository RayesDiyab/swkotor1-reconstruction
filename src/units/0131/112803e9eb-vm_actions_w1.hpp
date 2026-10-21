#pragma once
#include "93d73ae698-w1_ports.hpp"
// W1 tier 6 NWScript ACTION bodies (lane-w7/OWNERSHIP.md "W1 claims") and the server leaves they reach.
// Every ACTION core takes the CSWVirtualMachineCommands `self` and a frame standing for the original locals; it returns
// 0, 0xFFFFF82F (-2001, pop failure) or 0xFFFFF830 (-2000, push failure).
namespace w1::vm_actions {
U get_last_perceived_0053b230(U self, U frame, Ports& p);             // 256 (frame 4)
U get_party_member_by_index_0053c870(U self, U frame, Ports& p);      // 577 (frame 4)
U signal_event_005439d0(U self, U frame, Ports& p);                   // 131 (frame 0x1C)
U event_user_defined_00535a90(U self, U frame, Ports& p);             // 132 (frame 4)
U action_open_door_00540260(U self, U frame, Ports& p);               // 43 (frame 0xC)
U action_close_door_0052f4f0(U self, U frame, Ports& p);              // 44 (frame 0xC)
U cancel_combat_00546510(U self, U argc, U frame, Ports& p);          // 54 (frame 0x18: +0xC..+0x17 is the 004F61D0 scratch)
U destroy_object_0052ff20(U self, U argc, U frame, Ports& p);         // 241 (frame 0x18)
U play_animation_00540550(U self, U routine, U frame, Ports& p);      // 40 ActionPlayAnimation, 300 PlayAnimation (frame 0x28)
U get_position_0053cae0(U self, U frame, Ports& p);                   // 27 (frame 0x14)
U set_facing_00542830(U self, U routine, U frame, Ports& p);         // 10 SetFacing, 143 SetFacingPoint (frame 0x4C)
U get_nearest_object_0054b550(U self, U routine, U argc, U frame, Ports& p);   // 38/226/227/228/229 (frame 0xC0)
// leaves
U has_spell_effect_applied_004cdac0(U object, U spell, Ports& p);              // CSWSObject::HasSpellEffectApplied, ret 4
U set_lock_orientation_00610950(U client_creature, U target, U force, Ports& p);   // CSWCCreature::SetLockOrientationToObject, ret 8
void set_delete_no_fade_004ce920(U object, U value, U scratch, Ports& p);   // CSWSObject::SetDeleteNoFade, ret 4
U party_table_npc_id_005638a0(U table, U id, Ports& p);                     // CSWPartyTable::GetNPCID, ret 4
void party_table_clear_member_005639e0(U table, U index, Ports& p);         // CSWPartyTable::ClearMember, ret 4
void creature_cancel_combat_004fdaa0(U creature, U flag, U scratch, Ports& p);   // CSWSCreature::CancelCombat, ret 4 (scratch: 0xC bytes, 004F61D0 positions/out)
}
