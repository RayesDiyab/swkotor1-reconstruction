#include "2c53709d8f-w3_trigger_table.h"
#include "0996d9d6f1-w3_trigger_render.h"
#include <cstddef>
#include <cstdint>
using U=std::uint32_t;
extern "C" {
U __fastcall part_name_00440200(void*,void*);
U __fastcall part_get_local_0043e6a0(void*,void*,U,U);
U __fastcall part_get_world_0043e4a0(void*,void*,U,U);
U __fastcall part_get_children_004401e0(void*,void*);
char* __fastcall aurora_processed_pointer_00712f90(void*,void*) noexcept(false);
U __fastcall part_get_aur_object_004401f0(void*,void*);
U __fastcall part_hit_check_004429d0(void*,void*,U,U,U,U,U,U,U,U,U);
void* __fastcall kotor_m15e_slot02c_zero_0063e7f0(void*,void*);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*,void*);
void __fastcall light_manager_void_return_16_0063cfc0(void*,void*,U,U,U,U);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*,void*);
void __fastcall part_trigger_draw_0049da40(void*,void*,U);
U __fastcall part_animate_derived_00484260(void*,void*,U);
U __fastcall part_animate_00440110(void*,void*,U);
U __fastcall part_set_part_position_0043f5c0(void*,void*,U,U,U,U);
U __fastcall part_get_minimum_bounding_box_00440960(void*,void*,U,U,U);
U __fastcall part_get_minimum_sphere_00443080(void*,void*,U,U);
void __fastcall part_trigger_draw_volume_0049da00(void*,void*);
}
struct W3TriggerPartMethods {
    decltype(&w3_trigger_part_scalar_0049db10) s000;
    decltype(&part_name_00440200) s004;
    decltype(&part_get_local_0043e6a0) s008;
    decltype(&part_get_world_0043e4a0) s00c;
    decltype(&part_get_children_004401e0) s010;
    decltype(&aurora_processed_pointer_00712f90) s014;
    decltype(&part_get_aur_object_004401f0) s018;
    decltype(&part_hit_check_004429d0) s01c;
    decltype(&w3_trigger_part_hit_0049d810) s020;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s024;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s028;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s02c;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s030;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s034;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s038;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s03c;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s040;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s044;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s048;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s04c;
    decltype(&kotor_m15e_slot02c_zero_0063e7f0) s050;
    decltype(&kotor_m15e_slot00c_self_00641db0) s054;
    decltype(&kotor_m15e_slot00c_self_00641db0) s058;
    decltype(&light_manager_void_return_16_0063cfc0) s05c;
    decltype(&kotor_m15e_slot090_void0_004015a0) s060;
    decltype(&part_trigger_draw_0049da40) s064;
    decltype(&part_animate_derived_00484260) s068;
    decltype(&part_animate_00440110) s06c;
    decltype(&part_set_part_position_0043f5c0) s070;
    decltype(&part_get_minimum_bounding_box_00440960) s074;
    decltype(&part_get_minimum_sphere_00443080) s078;
    decltype(&part_trigger_draw_volume_0049da00) s07c;
};
static_assert(sizeof(W3TriggerPartMethods)==0x80,"original32slot table");
static_assert(offsetof(W3TriggerPartMethods,s07c)==0x7c,"last original slot");
extern "C" const W3TriggerPartMethods w3_trigger_part_methods_007436d0 = {
    &w3_trigger_part_scalar_0049db10, // +000 0049DB10
    &part_name_00440200, // +004 00440200
    &part_get_local_0043e6a0, // +008 0043E6A0
    &part_get_world_0043e4a0, // +00C 0043E4A0
    &part_get_children_004401e0, // +010 004401E0
    &aurora_processed_pointer_00712f90, // +014 00712F90
    &part_get_aur_object_004401f0, // +018 004401F0
    &part_hit_check_004429d0, // +01C 004429D0
    &w3_trigger_part_hit_0049d810, // +020 0049D810
    &kotor_m15e_slot02c_zero_0063e7f0, // +024 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +028 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +02C 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +030 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +034 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +038 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +03C 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +040 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +044 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +048 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +04C 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +050 0063E7F0
    &kotor_m15e_slot00c_self_00641db0, // +054 00641DB0
    &kotor_m15e_slot00c_self_00641db0, // +058 00641DB0
    &light_manager_void_return_16_0063cfc0, // +05C 0063CFC0
    &kotor_m15e_slot090_void0_004015a0, // +060 004015A0
    &part_trigger_draw_0049da40, // +064 0049DA40
    &part_animate_derived_00484260, // +068 00484260
    &part_animate_00440110, // +06C 00440110
    &part_set_part_position_0043f5c0, // +070 0043F5C0
    &part_get_minimum_bounding_box_00440960, // +074 00440960
    &part_get_minimum_sphere_00443080, // +078 00443080
    &part_trigger_draw_volume_0049da00, // +07C 0049DA00
};
