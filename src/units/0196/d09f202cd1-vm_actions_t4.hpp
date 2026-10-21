#pragma once
// W7 tier t4 exports (ACTION ABI as in vm_actions_t1.hpp).
#include <cstdint>
extern "C" {
int __fastcall recovered_vm_get_has_spell_00539440(void*, void*, int, unsigned);              // 377
int __fastcall recovered_vm_get_tag_0053df00(void*, void*, int, unsigned);                    // 168
int __fastcall recovered_vm_get_waypoint_by_tag_0053e2b0(void*, void*, int, unsigned);        // 197
int __fastcall recovered_vm_get_racial_type_0053cba0(void*, void*, int, unsigned);            // 107
int __fastcall recovered_vm_get_standard_faction_00545d80(void*, void*, int, unsigned);       // 713
int __fastcall recovered_vm_get_subrace_00546d90(void*, void*, int, unsigned);                // 497
int __fastcall recovered_vm_random_005419d0(void*, void*, int, unsigned);                     // 0
int __fastcall recovered_vm_get_hit_dice_00539570(void*, void*, int, unsigned);               // 166
int __fastcall recovered_vm_get_is_encounter_creature_00539c10(void*, void*, int, unsigned);  // 409
int __fastcall recovered_vm_set_min_one_hp_00545f90(void*, void*, int, unsigned);             // 716
int __fastcall recovered_vm_get_distance_to_object_2d_00537960(void*, void*, int, unsigned);  // 335
int __fastcall recovered_vm_set_commandable_005426c0(void*, void*, int, unsigned);            // 162
int __fastcall recovered_vm_set_locked_00543240(void*, void*, int, unsigned);                 // 324
int __fastcall recovered_vm_set_is_destroyable_00542dc0(void*, void*, int, unsigned);         // 323
int __fastcall recovered_vm_get_class_information_00536ce0(void*, void*, int, unsigned);      // 341, 342, 343
unsigned __fastcall recovered_vm_outer_push_float_005d1030(void*, void*, unsigned);
int __fastcall recovered_spell_adjusted_force_points_0059b8a0(void*, void*, std::uint32_t level, std::uint32_t kind);
int __fastcall recovered_stats_has_spell_005a6e70(void*, void*, std::uint32_t first, std::uint32_t spell, std::uint32_t check);
int __fastcall recovered_server_faction_of_object_004b40c0(void*, void*, std::uint32_t id, std::uint32_t* out);
unsigned __fastcall recovered_module_find_object_by_tag_type_004c60b0(void*, void*, const void* tag, std::uint32_t type, std::int32_t nth);
unsigned __fastcall recovered_module_get_waypoint_004c6e00(void*, void*, const void* tag);
}
