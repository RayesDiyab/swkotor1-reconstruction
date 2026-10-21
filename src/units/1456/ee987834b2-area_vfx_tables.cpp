#include "42237bfd9e-object_projectile_base.h"
#include "3c510cfa48-model_methods_a.h"
#include "7c3ff1b3b2-model_inherited_b.h"
// Exact82-slot area and62-slot derived model tables. Existing owner declarations projected verbatim.
#include "b4799296be-m15e_animbase.h"
namespace kotor_w3 { struct Vector3; }
extern "C" {
void __fastcall area_vfx_ai_update_00690980(void*,void*);
kotor_m15e::U __fastcall kotor_m15e_trap_0063cb00(void*,void*,kotor_m15e::U,kotor_m15e::U,kotor_m15e::U,kotor_m15e::U);
void* __fastcall area_vfx_base_name_0063e730(void*,void*,void*);
unsigned __fastcall area_vfx_base_unhilite_0063cb30(void*,void*,unsigned);
unsigned __fastcall area_vfx_load_model_006903d0(void*,void*,void*,unsigned,unsigned);
unsigned __fastcall area_vfx_model_animate_006e5e60(void*,void*,void*,unsigned,unsigned,unsigned,unsigned);
void* __fastcall area_vfx_model_animation_name_006e5dc0(void*,void*,void*,unsigned);
void* __fastcall area_vfx_model_delete_006d9070(void*,void*,unsigned);
unsigned __fastcall area_vfx_model_load_006e5d50(void*,void*,void*,unsigned);
void* __fastcall area_vfx_scalar_destroy_00690960(void*,void*,unsigned);
void __fastcall area_vfx_set_position_00690b00(void*,void*,const kotor_w3::Vector3*);
unsigned __fastcall camera_return_zero_00401590(void*,void*,unsigned,unsigned,unsigned,unsigned);
void __fastcall cswc_object_set_id_00616fa0(void* self, void*, std::uint32_t value);
kotor_m15e::U __fastcall kotor_m15e_ab_slot09c_set_env_map_0069d7b0(void* self, void*, kotor_m15e::U a1, kotor_m15e::U a2,
                                                                    kotor_m15e::U resref) noexcept(false);
kotor_m15e::U __fastcall anim_base_add_animation_flag_0069a3a0(void*, void*, kotor_m15e::U) noexcept(false);
kotor_m15e::U __fastcall anim_base_remove_animation_flag_0069a3c0(void*, void*, kotor_m15e::U) noexcept(false);
















std::uint8_t __fastcall object_vfx_center_0069f1e0(void*, void*, void*);
kotor_m15e::U __fastcall anim_base_get_bounding_rect_0069f630(void*, void*, void*) noexcept(false);

kotor_m15e::U __fastcall kotor_m15e_anim_base_animation_exists_0069d910(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_apply_shadow_blob_0069da00(void* self, void*, void* name_string) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_attach_to_scene_0069cd80(void* self, void*, void* scene) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_disable_distortion_0069de70(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_enable_distortion_0069de50(void* self, void*) noexcept(false);
float __fastcall kotor_m15e_anim_base_get_animation_length_0069d250(void* self, void*, kotor_m15e::U animation) noexcept(false);
float __fastcall kotor_m15e_anim_base_get_animation_time_0069d180(void* self, void*, kotor_m15e::U animation) noexcept(false);
void* __fastcall kotor_m15e_anim_base_get_model_0069cda0(void* self, void*, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_get_overlay_animation_playing_0069d320(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_hide_shadow_blob_0069db10(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_remove_shadow_blob_0069da70(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069cdc0(void* self, void*, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069ce50(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_replace_texture_0069cef0(void* self, void*, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_restore_texture_0069d100(void* self, void*, kotor_m15e::U part) noexcept(false);
std::uint16_t __fastcall kotor_m15e_anim_base_server_to_client_animation_0069f650(void* self, void*, kotor_m15e::U animation) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_alpha_0069eb20(void* self, void*, kotor_m15e::U alpha, kotor_m15e::U part) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_env_map_0069d830(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_orientation_0069d5d0(void* self, void*, const float* vector3) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_orientation_0069d640(void* self, void*, const float* quaternion) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_set_position_0069d690(void* self, void*, const float* vector3) noexcept(false);
void __fastcall kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50(void* self, void*, const float* quaternion) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_show_shadow_blob_0069dac0(void* self, void*, kotor_m15e::U force) noexcept(false);
void __fastcall kotor_m15e_anim_base_turn_on_shadows_0069dc10(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_unload_model_0069d740(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_anim_base_unload_model_0069d770(void* self, void*, kotor_m15e::U part) noexcept(false);
void __fastcall recovered_client_object_set_area_0063f570(void* self, void*, void* area) noexcept(false);
void __fastcall kotor_m15e_object_add_fire_forget_animation_0063d8c0(void*, void*, kotor_m15e::U, kotor_m15e::U) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_object_attach_to_scene_0063d820(void* self, void*, void* scene);
void __fastcall kotor_m15e_object_detach_visual_effects_0063e450(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_object_hide_shadow_blob_00616fd0(void* self, void*);
void __fastcall kotor_m15e_object_kill_all_visual_effects_0063dae0(void* self, void*);
void __fastcall kotor_m15e_object_reattach_visual_effects_0063e4b0(void* self, void*);
void __fastcall kotor_m15e_object_remove_shadow_blob_0063d070(void* self, void*);
kotor_m15e::U __fastcall kotor_m15e_object_set_animation_0063e930(void* self, void*, kotor_m15e::U animation, kotor_m15e::U a2, kotor_m15e::U a3) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_object_set_env_map_0063ccc0(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false);
void __fastcall kotor_m15e_object_set_orientation_0063c110(void* self, void*, const float* orientation);
kotor_m15e::U __fastcall kotor_m15e_object_unload_model_00646650(void* self, void*, kotor_m15e::U part);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*) ;
kotor_m15e::U __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*) ;
void* __fastcall kotor_m15e_slot02c_zero_0063e7f0(void* self, void*) ;
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*) ;
kotor_m15e::U __fastcall kotor_m15e_slot0b0_set_env_map_0063cb70(void* self, void*, kotor_m15e::U a1, kotor_m15e::U a2,
                                                                 kotor_m15e::U resref) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_slot0d4_true_0063e7a0(void* self, void*) ;
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*) ;
void __fastcall recovered_client_object_show_shadow_blob_00616fb0(void* self, void*) noexcept(false);
kotor_m15e::U __fastcall kotor_m15e_slot128_false4_0063e7c0(void* self, void*, kotor_m15e::U a2) ;
double __fastcall kotor_m15e_trap_00616fe0(void*, void*) ;
std::uint8_t __fastcall kotor_fastcall_0063d560(void*, void*, void*);
double __fastcall kotor_m15e_trap_0063d780(void*, void*) ;
kotor_m15e::U __fastcall kotor_m15e_trap_0063d970(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U) ;
kotor_m15e::U __fastcall kotor_m15e_trap_0063db20(void*, void*, kotor_m15e::U) ;
kotor_m15e::U __fastcall kotor_m15e_trap_0063ddf0(void*, void*, kotor_m15e::U) ;
kotor_m15e::U __fastcall kotor_m15e_trap_0063f400(void*, void*, kotor_m15e::U) ;
void __fastcall object_vfx_0063db90(void*,void*,unsigned,unsigned);
void __fastcall object_vfx_0063f490(void*,void*);
kotor_m15e::U __fastcall recovered_anim_base_set_animation_0069d4f0(void* self, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U,
                                                                   kotor_m15e::U) noexcept(false);
void __fastcall recovered_anim_base_turn_off_shadows_0069dc30(void* self, void*) noexcept(false);
void* __fastcall recovered_client_object_get_model_0063c080(void* self, void*, std::uint32_t part, std::uint32_t) noexcept(false);
void __fastcall recovered_client_object_turn_off_shadows_0060b320(void* self, void*) noexcept(false);
void __fastcall recovered_client_object_turn_on_shadows_0060b310(void* self, void*) noexcept(false);
void __fastcall recovered_client_object_unload_model_006825f0(void*,void*);
void __fastcall slot_void_return_4_0060e760_a1(void*,void*,unsigned);
void __fastcall slot_void_return_8_005b5e90_a2(void*,void*,unsigned,unsigned);
struct AreaVfxMethods {
    decltype(&area_vfx_scalar_destroy_00690960) s000;
    decltype(KotorM15ObjectMethods_007516c8::s004_set_id) s004;
    decltype(KotorM15ObjectMethods_007516c8::s008) s008;
    decltype(KotorM15ObjectMethods_007516c8::s00c_as_creature_or_self) s00c;
    decltype(KotorM15ObjectMethods_007516c8::s010) s010;
    decltype(KotorM15ObjectMethods_007516c8::s014) s014;
    decltype(KotorM15ObjectMethods_007516c8::s018) s018;
    decltype(KotorM15ObjectMethods_007516c8::s01c) s01c;
    decltype(KotorM15ObjectMethods_007516c8::s020) s020;
    decltype(KotorM15ObjectMethods_007516c8::s024) s024;
    decltype(KotorM15ObjectMethods_007516c8::s028) s028;
    decltype(KotorM15ObjectMethods_007516c8::s02c_as_creature) s02c;
    decltype(KotorM15ObjectMethods_007516c8::s030) s030;
    decltype(KotorM15ObjectMethods_007516c8::s034) s034;
    decltype(KotorM15ObjectMethods_007516c8::s038) s038;
    decltype(KotorM15ObjectMethods_007516c8::s03c) s03c;
    decltype(KotorM15ObjectMethods_007516c8::s040) s040;
    decltype(KotorM15ObjectMethods_007516c8::s044) s044;
    decltype(KotorM15ObjectMethods_007516c8::s048) s048;
    decltype(KotorM15ObjectMethods_007516c8::s04c) s04c;
    decltype(KotorM15ObjectMethods_007516c8::s050) s050;
    decltype(KotorM15ObjectMethods_007516c8::s054) s054;
    decltype(KotorM15ObjectMethods_007516c8::s058) s058;
    decltype(KotorM15ObjectMethods_007516c8::s05c) s05c;
    decltype(KotorM15ObjectMethods_007516c8::s060) s060;
    decltype(KotorM15ObjectMethods_007516c8::s064) s064;
    decltype(KotorM15ObjectMethods_007516c8::s068) s068;
    decltype(KotorM15ObjectMethods_007516c8::s06c) s06c;
    decltype(&area_vfx_base_name_0063e730) s070;
    decltype(&area_vfx_ai_update_00690980) s074;
    decltype(&area_vfx_load_model_006903d0) s078;
    decltype(KotorM15ObjectMethods_007516c8::s07c_unload_model) s07c;
    decltype(KotorM15ObjectMethods_007516c8::s080_unload_model) s080;
    decltype(KotorM15ObjectMethods_007516c8::s084_set_area) s084;
    decltype(KotorM15ObjectMethods_007516c8::s088_set_orientation) s088;
    decltype(&area_vfx_set_position_00690b00) s08c;
    decltype(KotorM15ObjectMethods_007516c8::s090_position_changed) s090;
    decltype(KotorM15ObjectMethods_007516c8::s094_attach_to_scene) s094;
    decltype(KotorM15ObjectMethods_007516c8::s098_get_model) s098;
    decltype(KotorM15ObjectMethods_007516c8::s09c_set_animation) s09c;
    decltype(KotorM15ObjectMethods_007516c8::s0a0_add_fire_forget_animation) s0a0;
    decltype(&kotor_m15e_trap_0063cb00) s0a4;
    decltype(&area_vfx_base_unhilite_0063cb30) s0a8;
    decltype(KotorM15ObjectMethods_007516c8::s0ac_set_env_map) s0ac;
    decltype(KotorM15ObjectMethods_007516c8::s0b0_set_env_map) s0b0;
    decltype(KotorM15ObjectMethods_007516c8::s0b4_add_visual_effect) s0b4;
    decltype(KotorM15ObjectMethods_007516c8::s0b8_delete_visual_effect) s0b8;
    decltype(KotorM15ObjectMethods_007516c8::s0bc_kill_all_visual_effects) s0bc;
    decltype(KotorM15ObjectMethods_007516c8::s0c0_refresh_visual_effects) s0c0;
    decltype(&object_vfx_0063f490) s0c4;
    decltype(KotorM15ObjectMethods_007516c8::s0c8_stop_all_visual_effects) s0c8;
    decltype(KotorM15ObjectMethods_007516c8::s0cc_has_visual_effect_applied) s0cc;
    decltype(KotorM15ObjectMethods_007516c8::s0d0_get_camera_follow_part) s0d0;
    decltype(KotorM15ObjectMethods_007516c8::s0d4_get_following_body) s0d4;
    decltype(KotorM15ObjectMethods_007516c8::s0d8_get_head_height_offset) s0d8;
    decltype(&object_vfx_0063db90) s0dc;
    decltype(KotorM15ObjectMethods_007516c8::s0e0_detach_visual_effects) s0e0;
    decltype(KotorM15ObjectMethods_007516c8::s0e4_reattach_visual_effects) s0e4;
    decltype(KotorM15ObjectMethods_007516c8::s0e8_update_pop_up_bubble) s0e8;
    decltype(KotorM15ObjectMethods_007516c8::s0ec_set_portrait) s0ec;
    decltype(KotorM15ObjectMethods_007516c8::s0f0_get_portrait) s0f0;
    decltype(KotorM15ObjectMethods_007516c8::s0f4_set_portrait_id) s0f4;
    decltype(KotorM15ObjectMethods_007516c8::s0f8_get_portrait_id) s0f8;
    decltype(KotorM15ObjectMethods_007516c8::s0fc_apply_shadow_blob) s0fc;
    decltype(KotorM15ObjectMethods_007516c8::s100_remove_shadow_blob) s100;
    decltype(KotorM15ObjectMethods_007516c8::s104_show_shadow_blob) s104;
    decltype(KotorM15ObjectMethods_007516c8::s108_hide_shadow_blob) s108;
    decltype(KotorM15ObjectMethods_007516c8::s10c_set_ground_normal) s10c;
    decltype(KotorM15ObjectMethods_007516c8::s110_turn_on_shadows) s110;
    decltype(KotorM15ObjectMethods_007516c8::s114_turn_off_shadows) s114;
    decltype(KotorM15ObjectMethods_007516c8::s118_set_queued_animation_callback) s118;
    decltype(KotorM15ObjectMethods_007516c8::s11c_clear_queued_animation_callback) s11c;
    decltype(KotorM15ObjectMethods_007516c8::s120_bark_string) s120;
    decltype(KotorM15ObjectMethods_007516c8::s124_bark_bubble) s124;
    decltype(KotorM15ObjectMethods_007516c8::s128_lip_sync) s128;
    decltype(KotorM15ObjectMethods_007516c8::s12c_sync_position) s12c;
    decltype(KotorM15ObjectMethods_007516c8::s130_sync_orientation) s130;
    decltype(KotorM15ObjectMethods_007516c8::s134_sync_animation) s134;
    decltype(KotorM15ObjectMethods_007516c8::s138_is_hostile) s138;
    decltype(KotorM15ObjectMethods_007516c8::s13c_get_reticle_point) s13c;
    decltype(KotorM15ObjectMethods_007516c8::s140_return_1_5) s140;
    decltype(KotorM15ObjectMethods_007516c8::s144_get_sticky_mouse_threshold) s144;
};
static_assert(sizeof(AreaVfxMethods)==82*4,"exact original table size");
extern const AreaVfxMethods area_vfx_methods_00754278 = {
    &area_vfx_scalar_destroy_00690960, // +000 00690960
    &cswc_object_set_id_00616fa0, // +004 00616FA0
    &slot_void_return_8_005b5e90_a2, // +008 005B5E90
    &kotor_m15e_slot00c_self_00641db0, // +00C 00641DB0
    &kotor_m15e_slot010_zero_0063e7f0, // +010 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +014 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +018 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +01C 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +020 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +024 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +028 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +02C 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +030 0063E7F0
    &kotor_m15e_slot02c_zero_0063e7f0, // +034 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +038 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +03C 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +040 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +044 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +048 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +04C 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +050 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +054 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +058 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +05C 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +060 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +064 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +068 0063E7F0
    &kotor_m15e_slot010_zero_0063e7f0, // +06C 0063E7F0
    &area_vfx_base_name_0063e730, // +070 0063E730
    &area_vfx_ai_update_00690980, // +074 00690980
    &area_vfx_load_model_006903d0, // +078 006903D0
    &kotor_m15e_object_unload_model_00646650, // +07C 00646650
    &recovered_client_object_unload_model_006825f0, // +080 006825F0
    &recovered_client_object_set_area_0063f570, // +084 0063F570
    &kotor_m15e_object_set_orientation_0063c110, // +088 0063C110
    &area_vfx_set_position_00690b00, // +08C 00690B00
    &kotor_m15e_slot090_void0_004015a0, // +090 004015A0
    &kotor_m15e_object_attach_to_scene_0063d820, // +094 0063D820
    &recovered_client_object_get_model_0063c080, // +098 0063C080
    &kotor_m15e_object_set_animation_0063e930, // +09C 0063E930
    &kotor_m15e_object_add_fire_forget_animation_0063d8c0, // +0A0 0063D8C0
    &kotor_m15e_trap_0063cb00, // +0A4 0063CB00
    &area_vfx_base_unhilite_0063cb30, // +0A8 0063CB30
    &kotor_m15e_object_set_env_map_0063ccc0, // +0AC 0063CCC0
    &kotor_m15e_slot0b0_set_env_map_0063cb70, // +0B0 0063CB70
    &kotor_m15e_trap_0063d970, // +0B4 0063D970
    &kotor_m15e_trap_0063f400, // +0B8 0063F400
    &kotor_m15e_object_kill_all_visual_effects_0063dae0, // +0BC 0063DAE0
    &kotor_m15e_trap_0063db20, // +0C0 0063DB20
    &object_vfx_0063f490, // +0C4 0063F490
    &object_projectile_base_0063daa0, // +0C8 0063DAA0
    &kotor_m15e_trap_0063ddf0, // +0CC 0063DDF0
    &object_projectile_base_0063e760, // +0D0 0063E760
    &kotor_m15e_slot0d4_true_0063e7a0, // +0D4 0063E7A0
    &kotor_m15e_trap_00616fe0, // +0D8 00616FE0
    &object_vfx_0063db90, // +0DC 0063DB90
    &kotor_m15e_object_detach_visual_effects_0063e450, // +0E0 0063E450
    &kotor_m15e_object_reattach_visual_effects_0063e4b0, // +0E4 0063E4B0
    &kotor_m15e_slot090_void0_004015a0, // +0E8 004015A0
    &object_projectile_base_0063cfd0, // +0EC 0063CFD0
    &object_projectile_base_0063e770, // +0F0 0063E770
    &object_projectile_base_0063d140, // +0F4 0063D140
    &object_projectile_base_0063e790, // +0F8 0063E790
    &kotor_m15e_slot0d4_true_0063e7a0, // +0FC 0063E7A0
    &kotor_m15e_object_remove_shadow_blob_0063d070, // +100 0063D070
    &recovered_client_object_show_shadow_blob_00616fb0, // +104 00616FB0
    &kotor_m15e_object_hide_shadow_blob_00616fd0, // +108 00616FD0
    &object_projectile_base_0063d0a0, // +10C 0063D0A0
    &recovered_client_object_turn_on_shadows_0060b310, // +110 0060B310
    &recovered_client_object_turn_off_shadows_0060b320, // +114 0060B320
    &kotor_m15e_slot090_void0_004015a0, // +118 004015A0
    &kotor_m15e_slot010_zero_0063e7f0, // +11C 0063E7F0
    &object_projectile_base_0063e7b0, // +120 0063E7B0
    &object_projectile_base_0060f560, // +124 0060F560
    &kotor_m15e_slot128_false4_0063e7c0, // +128 0063E7C0
    &object_projectile_base_0063e7d0, // +12C 0063E7D0
    &object_projectile_base_0063e7e0, // +130 0063E7E0
    &object_projectile_base_0063d3f0, // +134 0063D3F0
    &kotor_m15e_slot010_zero_0063e7f0, // +138 0063E7F0
    &kotor_fastcall_0063d560, // +13C 0063D560
    &kotor_m15e_trap_00616fe0, // +140 00616FE0
    &kotor_m15e_trap_0063d780, // +144 0063D780
};
struct AreaVfxModelMethods {
    decltype(&area_vfx_model_delete_006d9070) s000;
    decltype(KotorM15eAnimBaseVtbl::s004) s004;
    decltype(KotorM15eAnimBaseVtbl::s008) s008;
    decltype(&area_vfx_model_load_006e5d50) s00c;
    decltype(KotorM15eAnimBaseVtbl::s010) s010;
    decltype(KotorM15eAnimBaseVtbl::s014) s014;
    decltype(KotorM15eAnimBaseVtbl::s018) s018;
    decltype(KotorM15eAnimBaseVtbl::s01c) s01c;
    decltype(KotorM15eAnimBaseVtbl::s020) s020;
    decltype(KotorM15eAnimBaseVtbl::s024) s024;
    decltype(KotorM15eAnimBaseVtbl::s028) s028;
    decltype(KotorM15eAnimBaseVtbl::s02c) s02c;
    decltype(KotorM15eAnimBaseVtbl::s030) s030;
    decltype(KotorM15eAnimBaseVtbl::s034) s034;
    decltype(KotorM15eAnimBaseVtbl::s038) s038;
    decltype(KotorM15eAnimBaseVtbl::s03c) s03c;
    decltype(&area_vfx_model_animation_name_006e5dc0) s040;
    decltype(KotorM15eAnimBaseVtbl::s044) s044;
    decltype(KotorM15eAnimBaseVtbl::s048) s048;
    decltype(KotorM15eAnimBaseVtbl::s04c) s04c;
    decltype(KotorM15eAnimBaseVtbl::s050) s050;
    decltype(KotorM15eAnimBaseVtbl::s054) s054;
    decltype(KotorM15eAnimBaseVtbl::s058) s058;
    decltype(KotorM15eAnimBaseVtbl::s05c) s05c;
    decltype(KotorM15eAnimBaseVtbl::s060) s060;
    decltype(KotorM15eAnimBaseVtbl::s064) s064;
    decltype(KotorM15eAnimBaseVtbl::s068) s068;
    decltype(KotorM15eAnimBaseVtbl::s06c) s06c;
    decltype(KotorM15eAnimBaseVtbl::s070) s070;
    decltype(KotorM15eAnimBaseVtbl::s074) s074;
    decltype(KotorM15eAnimBaseVtbl::s078) s078;
    decltype(&camera_return_zero_00401590) s07c;
    decltype(&slot_void_return_8_005b5e90_a2) s080;
    decltype(&kotor_m15e_slot090_void0_004015a0) s084;
    decltype(&kotor_m15e_slot090_void0_004015a0) s088;
    decltype(&kotor_m15e_slot090_void0_004015a0) s08c;
    decltype(&slot_void_return_4_0060e760_a1) s090;
    decltype(KotorM15eAnimBaseVtbl::s094) s094;
    decltype(KotorM15eAnimBaseVtbl::s098) s098;
    decltype(KotorM15eAnimBaseVtbl::s09c) s09c;
    decltype(KotorM15eAnimBaseVtbl::s0a0) s0a0;
    decltype(KotorM15eAnimBaseVtbl::s0a4) s0a4;
    decltype(KotorM15eAnimBaseVtbl::s0a8) s0a8;
    decltype(KotorM15eAnimBaseVtbl::s0ac) s0ac;
    decltype(KotorM15eAnimBaseVtbl::s0b0) s0b0;
    decltype(KotorM15eAnimBaseVtbl::s0b4) s0b4;
    decltype(KotorM15eAnimBaseVtbl::s0b8) s0b8;
    decltype(KotorM15eAnimBaseVtbl::s0bc) s0bc;
    decltype(KotorM15eAnimBaseVtbl::s0c0) s0c0;
    decltype(&slot_void_return_4_0060e760_a1) s0c4;
    decltype(KotorM15eAnimBaseVtbl::s0c8) s0c8;
    decltype(KotorM15eAnimBaseVtbl::s0cc) s0cc;
    decltype(KotorM15eAnimBaseVtbl::s0d0) s0d0;
    decltype(KotorM15eAnimBaseVtbl::s0d4) s0d4;
    decltype(KotorM15eAnimBaseVtbl::s0d8) s0d8;
    decltype(KotorM15eAnimBaseVtbl::s0dc) s0dc;
    decltype(KotorM15eAnimBaseVtbl::s0e0) s0e0;
    decltype(KotorM15eAnimBaseVtbl::s0e4) s0e4;
    decltype(KotorM15eAnimBaseVtbl::s0e8) s0e8;
    decltype(&slot_void_return_4_0060e760_a1) s0ec;
    decltype(&area_vfx_model_animate_006e5e60) s0f0;
    decltype(KotorM15eAnimBaseVtbl::s0f4) s0f4;
};
static_assert(sizeof(AreaVfxModelMethods)==62*4,"exact original model table size");
extern const AreaVfxModelMethods area_vfx_model_methods_00758b68 = {
    &area_vfx_model_delete_006d9070, // +000 006D9070
    &kotor_m15e_anim_base_attach_to_scene_0069cd80, // +004 0069CD80
    &kotor_m15e_anim_base_get_model_0069cda0, // +008 0069CDA0
    &area_vfx_model_load_006e5d50, // +00C 006E5D50
    &kotor_m15e_anim_base_replace_texture_0069cfd0, // +010 0069CFD0
    &kotor_m15e_anim_base_replace_texture_0069cef0, // +014 0069CEF0
    &kotor_m15e_anim_base_replace_texture_0069cdc0, // +018 0069CDC0
    &kotor_m15e_anim_base_replace_texture_0069ce50, // +01C 0069CE50
    &kotor_m15e_anim_base_restore_texture_0069d100, // +020 0069D100
    &kotor_m15e_anim_base_restore_texture_0069d0a0, // +024 0069D0A0
    &kotor_m15e_anim_base_enable_bumped_out_shell_0069dd90, // +028 0069DD90
    &model_disable_bumped_out_shell_0069ddf0, // +02C 0069DDF0
    &model_set_motion_blur_0069de10, // +030 0069DE10
    &model_set_max_motion_blur_length_0069de40, // +034 0069DE40
    &kotor_m15e_anim_base_get_animation_length_0069d250, // +038 0069D250
    &kotor_m15e_anim_base_get_animation_time_0069d180, // +03C 0069D180
    &area_vfx_model_animation_name_006e5dc0, // +040 006E5DC0
    &kotor_m15e_anim_base_set_overlay_animation_006a03c0, // +044 006A03C0
    &kotor_m15e_anim_base_get_overlay_animation_0069d3e0, // +048 0069D3E0
    &kotor_m15e_anim_base_get_overlay_animation_playing_0069d320, // +04C 0069D320
    &kotor_m15e_anim_base_animation_exists_0069d910, // +050 0069D910
    &recovered_anim_base_set_animation_0069d4f0, // +054 0069D4F0
    &model_set_animation_on_subobject_0069d520, // +058 0069D520
    &model_set_animation_visemes_0069d540, // +05C 0069D540
    &kotor_m15e_anim_base_set_orientation_0069d640, // +060 0069D640
    &kotor_m15e_anim_base_set_orientation_0069d5d0, // +064 0069D5D0
    &kotor_m15e_anim_base_set_position_0069d690, // +068 0069D690
    &model_set_always_render_0069d6e0, // +06C 0069D6E0
    &model_set_in_cutscene_0069d710, // +070 0069D710
    &kotor_m15e_anim_base_unload_model_0069d770, // +074 0069D770
    &kotor_m15e_anim_base_unload_model_0069d740, // +078 0069D740
    &camera_return_zero_00401590, // +07C 00401590
    &slot_void_return_8_005b5e90_a2, // +080 005B5E90
    &kotor_m15e_slot090_void0_004015a0, // +084 004015A0
    &kotor_m15e_slot090_void0_004015a0, // +088 004015A0
    &kotor_m15e_slot090_void0_004015a0, // +08C 004015A0
    &slot_void_return_4_0060e760_a1, // +090 0060E760
    &kotor_m15e_anim_base_set_alpha_0069eb20, // +094 0069EB20
    &kotor_m15e_anim_base_set_env_map_0069d830, // +098 0069D830
    &kotor_m15e_ab_slot09c_set_env_map_0069d7b0, // +09C 0069D7B0
    &kotor_m15e_anim_base_restore_env_map_0069d8b0, // +0A0 0069D8B0
    &kotor_m15e_anim_base_apply_shadow_blob_0069da00, // +0A4 0069DA00
    &kotor_m15e_anim_base_remove_shadow_blob_0069da70, // +0A8 0069DA70
    &kotor_m15e_anim_base_show_shadow_blob_0069dac0, // +0AC 0069DAC0
    &kotor_m15e_anim_base_hide_shadow_blob_0069db10, // +0B0 0069DB10
    &kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50, // +0B4 0069DB50
    &kotor_m15e_anim_base_turn_on_shadows_0069dc10, // +0B8 0069DC10
    &recovered_anim_base_turn_off_shadows_0069dc30, // +0BC 0069DC30
    &kotor_m15e_anim_base_get_orientation_0069dc50, // +0C0 0069DC50
    &slot_void_return_4_0060e760_a1, // +0C4 0060E760
    &anim_base_add_animation_flag_0069a3a0, // +0C8 0069A3A0
    &anim_base_remove_animation_flag_0069a3c0, // +0CC 0069A3C0
    &model_set_lean_0069dd30, // +0D0 0069DD30
    &kotor_m15e_anim_base_lean_0069dc90, // +0D4 0069DC90
    &anim_base_get_bounding_rect_0069f630, // +0D8 0069F630
    &object_vfx_center_0069f1e0, // +0DC 0069F1E0
    &kotor_m15e_anim_base_server_to_client_animation_0069f650, // +0E0 0069F650
    &kotor_m15e_anim_base_enable_distortion_0069de50, // +0E4 0069DE50
    &kotor_m15e_anim_base_disable_distortion_0069de70, // +0E8 0069DE70
    &slot_void_return_4_0060e760_a1, // +0EC 0060E760
    &area_vfx_model_animate_006e5e60, // +0F0 006E5E60
    &model_set_animation_on_subobject_internal_0069d5a0, // +0F4 0069D5A0
};
}
