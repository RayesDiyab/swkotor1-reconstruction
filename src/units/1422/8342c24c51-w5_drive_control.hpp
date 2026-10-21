#pragma once
// W5 G16 server input: drive-control handler and the creature/action resets it performs.
#include "58f963adf7-w5_types.hpp"
#include "1a370ea669-w5_foreign.hpp"
namespace recovered_player_message { unsigned overrun_004d6230(void*); unsigned underrun_004d6240(void*); }   // [0917]
namespace frame_control { extern unsigned message_manager_007a3a00; }                                       // [0456] VM pointer cell
extern "C" {
using w5::U; using w5::I;
// authored here
// 004F00A0 SetActivity: W2-owned (recovered_set_activity_004f00a0, w5_foreign.hpp)
void __fastcall w5_creature_set_combat_mode_0050ee80(void* creature, void*, U mode, U check);   // RET 8
void __fastcall w5_combat_clear_all_special_attacks_004d4f60(void* round, void*);               // RET 0
void __fastcall w5_combat_attack_clear_004d3010(void* data, void*);                             // RET 0
void __fastcall w5_combat_clear_all_attacks_004d37e0(void* round, void*);                        // RET 0
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);      // RET 4
void __fastcall w5_creature_action_manager_004f8770(void* creature, void*, U reason);           // RET 4
U __fastcall w5_message_input_drive_control_00523450(void* message, void*, void* player);      // RET 4
// reused natives (1115)
void* __fastcall recovered_equipment_item_005a4c20(void* inventory, void*, unsigned slot);      // [1025]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, int id); // [0820]
extern void* kotor_cell_rules_007a3a28;
void* __fastcall c2da_assign_text_005e5140(void* self, void*, const char* text);               // CExoString::operator=(char*) [0205]
void __fastcall c2da_current_string_destroy_005e5c20(void* self, void*);                       // ~CExoString [0377]
void __fastcall c2da_005e9c10(void* list, void*, void* position);                              // CExoLinkedList::Remove [0191]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                  // CSWSPlayer::GetGameObject [0595]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);
unsigned __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);
}
