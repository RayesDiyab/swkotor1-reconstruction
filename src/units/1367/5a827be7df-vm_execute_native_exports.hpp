#pragma once
// W6 native exports, for consumers in other objects such as 0886's vm_run_native_execute.inc.
// This header replaces the removed `#include "vm_execute_native_strings.inc"` line in 0886.
// Definitions: native/vm_execute_native_t1.cpp (t1b/t2) and native/vm_situation_native_t2.cpp (t2).
extern "C" {
int __fastcall recovered_vm_execute_file_005d4270(void* internal,void*,unsigned initial_pc);
int __fastcall recovered_vm_execute_command_0052c0d0(void* implementer,void*,int routine,unsigned count);
// ACTION trap naming (t1b)
__declspec(noreturn) void __cdecl recovered_vm_action_unimplemented(unsigned handler);
// t4: CVirtualMachine vector wrappers (for W1 ACTIONs 27/143) and the internal push
unsigned __fastcall recovered_vm_push_vector_005d18b0(void* internal,void*,unsigned x,unsigned y,unsigned z);
unsigned __fastcall recovered_vm_outer_pop_vector_005d1040(void* vm,void*,void* out12);                    // RET 4
unsigned __fastcall recovered_vm_outer_push_vector_005d1050(void* vm,void*,unsigned x,unsigned y,unsigned z); // Vector by value, RET C
// t5: ACTION bodies reached by the Endar opening (k_ai_master)
int __fastcall recovered_vm_get_commandable_00536e40(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_player_restrict_mode_0053cf70(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_listen_pattern_number_0053bd20(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_blocking_door_00536a80(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_spell_cast_0053d110(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_name_0053c080(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_speak_string_00544090(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_attempted_attack_target_00536860(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_last_perception_0053b2a0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_last_speaker_0053b410(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_current_action_00549040(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_ability_score_005361b0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_attempted_spell_target_005368b0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_last_hostile_actor_0053af40(void* implementer,void*,int routine,unsigned count);
void __fastcall recovered_object_set_last_hostile_actor_004cc0c0(void* object,void*,unsigned actor,int force);   // RET 8
int __fastcall recovered_vm_get_attack_target_00536800(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_is_dead_0053ecb0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_last_damager_0053ad40(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_object_to_string_00540170(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_ship_build_00543840(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_object_visibility_0053c780(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_emotions_00537ce0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_party_ai_style_005459a0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_npc_ai_style_005459d0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_user_actions_pending_0053e1b0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_creature_user_actions_pending_00518b30(void* creature,void*);   // RET 0
extern unsigned recovered_ship_build_flag_00832834;   // original .data 00832834
unsigned short __fastcall recovered_object_aq_action_id_by_id_0057a2b0(void* object,void*,unsigned type,int flag);   // RET 8
// t4: ACTION bodies and AcquireItem leaves
int __fastcall recovered_vm_apply_effect_to_object_0052e570(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_add_available_npc_by_template_0052dcd0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_set_area_unescapable_005424e0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_is_object_party_member_0053f160(void* implementer,void*,int routine,unsigned count);
unsigned __fastcall recovered_creature_acquire_item_005158e0(void* self,void*,void* item_cell,unsigned container,unsigned repository_owner,unsigned flag);
unsigned __fastcall recovered_placeable_acquire_item_00584b10(void* self,void*,void* item_cell,unsigned container,unsigned flag);
unsigned __fastcall recovered_store_acquire_item_005c7610(void* self,void*,void* item);
// t3: VM stack helpers
unsigned __fastcall recovered_vm_pop_vector_005d2560(void* internal,void*,void* out12);
// t3: assigned ACTION bodies
int __fastcall recovered_vm_effect_complex_005313f0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_create_item_on_object_0052f570(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_reveal_map_00547240(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_is_available_npc_0053ed70(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_item_activated_0053f220(void* implementer,void*,int routine,unsigned count);
void __fastcall recovered_effect_set_object_id_005036f0(void* effect,void*,unsigned index,unsigned id);
// t3: VM engine-structure stack helpers
unsigned __fastcall recovered_vm_push_engine_structure_005d19c0(void* internal,void*,int type,unsigned value);
unsigned __fastcall recovered_vm_pop_engine_structure_005d2630(void* internal,void*,int type,void* out);
unsigned __fastcall recovered_vm_outer_push_engine_structure_005d10b0(void* vm,void*,int type,unsigned value);
unsigned __fastcall recovered_vm_outer_pop_engine_structure_005d10a0(void* vm,void*,int type,void* out);
// t3: String/Location Global* ACTION bodies (578-581 are W7's) (command handlers, RET 8)
int __fastcall recovered_vm_get_global_string_00538ed0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_set_global_string_00542c60(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_get_global_location_00538cd0(void* implementer,void*,int routine,unsigned count);
int __fastcall recovered_vm_set_global_location_00542a50(void* implementer,void*,int routine,unsigned count);
// t2: command implementer slots (CSWVirtualMachineCommands)
// slot 4 ReportError 005B5E90 is the live 0296 export @gui_load_slot_ret8_005b5e90@16 (no W6 body since t3b)
void __fastcall gui_load_slot_ret8_005b5e90(void*,void*,unsigned,unsigned);
void __fastcall recovered_vm_commands_destroy_engine_00548360(void*,void*,int type,unsigned value);
int __fastcall recovered_vm_commands_equal_engine_0052c510(void*,void*,int type,unsigned a,unsigned b);
unsigned __fastcall recovered_vm_commands_copy_engine_0052c140(void*,void*,int type,unsigned source);
// t2: script situations
int __fastcall recovered_vm_stack_pop_command_005d10e0(void* vm,void*,void* out_script);
void __fastcall recovered_vm_delete_script_situation_005d1110(void* vm,void*,void* script);
int __fastcall recovered_vm_run_script_situation_005d0fd0(void* vm,void*,void* script,unsigned object_self,unsigned flag);
int __fastcall recovered_vm_internal_stack_pop_command_005d4380(void* internal,void*,void* out_script);
void __fastcall recovered_vm_internal_delete_script_situation_005d48e0(void* internal,void*,void* script);
int __fastcall recovered_vm_internal_run_script_situation_005d4ad0(void* internal,void*,void* script,unsigned object_self,unsigned flag);
}
