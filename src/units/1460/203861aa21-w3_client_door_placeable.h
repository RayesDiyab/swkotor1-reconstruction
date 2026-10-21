#pragma once
#include "2b26c2cb7b-placeable_methods.h"
#include "2f4302374b-door_methods.h"
// lane-w3 private declarations for the client door / placeable add unit (w3_client_door_placeable.cpp,
// w3_traps_door_placeable.cpp). Conventions as w3_client.h.
#include "625a7a9ed5-w3_client_objects.h"
#include "b8ee595e56-w3_animation.h"

namespace kotor_reconstruction { struct StartupString; }
namespace startup_rand {   // existing owner 0147-string-substring.obj (CExoString::SubString 005E6270, RET 0Ch)
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString* self, void*,
                                                                  kotor_reconstruction::StartupString* out, int start,
                                                                  int count);
}

extern "C" {
// ------------------------------------------------------------------ existing runtime1115 owners (callable symbols)
void* __fastcall kotor_m15a_object_construct_0063e530(void* self, void*, std::uint32_t object_type);   // 0063E530 RET 4
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void* self, void*, std::uint32_t animation);   // RET 4
void __fastcall kotor_m15a_object_set_looping_animation_0063d830(void* self, void*, std::uint32_t animation);   // RET 4
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void* self, void*);                 // 0063C100 (AX)
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* self, void*);                              // 0063D4B0
void* __fastcall gob_get_property_0x68_0063c070(void* self, void*);                                            // 0063C070 [this+68h]
void* __fastcall kotor_m15e_anim_base_construct_0069dfb0(void* self, void*);                                   // 0069DFB0 CSWCAnimBase()
std::uint32_t __fastcall kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(void* app, void*, std::uint32_t id);   // RET 4
std::uint32_t __fastcall kotor_fastcall_004aea30(void* server_app, void*, std::uint32_t id);   // 004AEA30 CServerExoApp::ClientToServerObjectId
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server_app, void*, std::uint32_t id);   // 004AE7A0 CServerExoApp::GetPlaceableByGameObjectID
std::uint32_t __fastcall kotor_fastcall_005eda50(void* app, void*, std::uint32_t id);          // 005EDA50 ServerToClientObjectId RET 4
void* __fastcall async_ref_construct_string_00406d60(void* self, void*, const void* string);   // 00406D60 CResRef(CExoString&) RET 4
int __fastcall c2da_equal_text_004060d0(const void* ref, void*, const char* text);             // 004060D0 CResRef::operator==(char*) RET 4
std::uint32_t __fastcall async_string_length_005e5790(const void* string, void*);             // 005E5790 CExoString::GetLength
int __fastcall c2da_current_get_float_00413430(void* table, void*, int row, void* column, float* out);   // 00413430 RET 0Ch
std::int32_t __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table* table, void*, std::int32_t row, std::int32_t column,
                                                               kotor_reconstruction::StartupString* out);                     // 00413190 RET 0Ch
extern std::uint32_t kotor_rules_cell32_007a2174;                                                // [007A2174] animations.2da name column
std::uint32_t __fastcall recovered_anim_base_internal_0069e8d0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t,
                                                               std::uint32_t, std::uint32_t);   // 0069E8D0 CSWCAnimBase::SetAnimationInternal RET 14h

// Table-slot owners referenced only by address from the four tables (D24 typed variants and D83 traps of lane-m15-e
// tables 007516C8 / 00754F60; declared with their decorated stack size from kotor-vm-cell1115.map).
void __fastcall cswc_object_set_id_00616fa0(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_00401590_s07c(void*, void*, std::uint32_t, void*, std::uint32_t, std::uint32_t);
void __fastcall kotor_m15e_ab_trap_004015a0_s084(void*, void*);
void __fastcall kotor_m15e_ab_trap_004015a0_s088(void*, void*);
void __fastcall kotor_m15e_ab_trap_004015a0_s08c(void*, void*);
void __fastcall kotor_m15e_ab_trap_005b5e90_s080(void*, void*, void*, void*);
void __fastcall kotor_m15e_ab_trap_0060e760_s090(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_ab_trap_0060e760_s0c4(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_ab_trap_0060e760_s0ec(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069a3a0_s0c8(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069a3c0_s0cc(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069cfd0_s010(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d0a0_s024(void*, void*);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d3e0_s048(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d520_s058(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d540_s05c(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d5a0_s0f4(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_m15e_ab_trap_0069d6e0_s06c(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_ab_trap_0069d710_s070(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069d8b0_s0a0(void*, void*);
void* __fastcall kotor_m15e_ab_trap_0069dc50_s0c0(void*, void*, void*);
void __fastcall kotor_m15e_ab_trap_0069dc90_s0d4(void*, void*);
void __fastcall kotor_m15e_ab_trap_0069dd30_s0d0(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069dd90_s028(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069ddf0_s02c(void*, void*);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069de10_s030(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069de40_s034(void*, void*, std::uint32_t);
unsigned char __fastcall kotor_m15e_ab_trap_0069f1e0_s0dc(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_ab_trap_0069f630_s0d8(void*, void*, void*);
std::uint32_t __fastcall kotor_m15e_ab_trap_006a03c0_s044(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_animation_exists_0069d910(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_apply_shadow_blob_0069da00(void*, void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_attach_to_scene_0069cd80(void*, void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_disable_distortion_0069de70(void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_enable_distortion_0069de50(void*, void*);
float __fastcall kotor_m15e_anim_base_get_animation_length_0069d250(void*, void*, std::uint32_t);
float __fastcall kotor_m15e_anim_base_get_animation_time_0069d180(void*, void*, std::uint32_t);
void* __fastcall kotor_m15e_anim_base_get_model_0069cda0(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_get_overlay_animation_playing_0069d320(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_hide_shadow_blob_0069db10(void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_remove_shadow_blob_0069da70(void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_replace_texture_0069cdc0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_replace_texture_0069ce50(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_replace_texture_0069cef0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_restore_texture_0069d100(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_set_alpha_0069eb20(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_anim_base_set_env_map_0069d830(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_set_orientation_0069d5d0(void*, void*, const float*);
std::uint32_t __fastcall kotor_m15e_anim_base_set_orientation_0069d640(void*, void*, const float*);
std::uint32_t __fastcall kotor_m15e_anim_base_set_position_0069d690(void*, void*, const float*);
void __fastcall kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50(void*, void*, const float*);
std::uint32_t __fastcall kotor_m15e_anim_base_show_shadow_blob_0069dac0(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_anim_base_turn_on_shadows_0069dc10(void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_unload_model_0069d740(void*, void*);
std::uint32_t __fastcall kotor_m15e_anim_base_unload_model_0069d770(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_object_add_fire_forget_animation_0063d8c0(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_object_attach_to_scene_0063d820(void*, void*, void*);
void __fastcall kotor_m15e_object_detach_visual_effects_0063e450(void*, void*);
std::uint32_t __fastcall kotor_m15e_object_hide_shadow_blob_00616fd0(void*, void*);
void __fastcall kotor_m15e_object_kill_all_visual_effects_0063dae0(void*, void*);
void __fastcall kotor_m15e_object_reattach_visual_effects_0063e4b0(void*, void*);
void __fastcall kotor_m15e_object_remove_shadow_blob_0063d070(void*, void*);
std::uint32_t __fastcall kotor_m15e_object_set_animation_0063e930(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_object_set_env_map_0063ccc0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, void*);
void __fastcall kotor_m15e_object_set_orientation_0063c110(void*, void*, const float*);
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, std::uint32_t, std::uint32_t);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);
std::uint32_t __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
void* __fastcall kotor_m15e_slot02c_zero_0063e7f0(void*, void*);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);
std::uint32_t __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*, void*);
std::uint32_t __fastcall kotor_m15e_slot128_false4_0063e7c0(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_trap_0060f560(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
double __fastcall kotor_m15e_trap_00616fe0(void*, void*);
std::uint32_t __fastcall kotor_m15e_trap_0063cb00(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_trap_0063cb30(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_trap_0063cfd0(void*, void*, void*);
void __fastcall kotor_m15e_trap_0063d0a0(void*, void*, const float*);
void __fastcall kotor_m15e_trap_0063d140(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_trap_0063d3f0(void*, void*, std::uint32_t, std::uint32_t);
std::uint8_t __fastcall kotor_m15e_trap_0063d560(void*, void*, std::uint32_t);
double __fastcall kotor_m15e_trap_0063d780(void*, void*);
std::uint32_t __fastcall kotor_m15e_trap_0063d970(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_m15e_trap_0063daa0(void*, void*);
std::uint32_t __fastcall kotor_m15e_trap_0063db20(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_trap_0063db90(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall kotor_m15e_trap_0063ddf0(void*, void*, std::uint32_t);
void* __fastcall kotor_m15e_trap_0063e760(void*, void*);
void* __fastcall kotor_m15e_trap_0063e770(void*, void*, void*, std::uint32_t);
std::uint16_t __fastcall kotor_m15e_trap_0063e790(void*, void*);
std::uint32_t __fastcall kotor_m15e_trap_0063e7b0(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_m15e_trap_0063e7d0(void*, void*, const float*, std::uint32_t, float);
void __fastcall kotor_m15e_trap_0063e7e0(void*, void*, const float*);
void __fastcall kotor_m15e_trap_0063f340(void*, void*, const float*);
std::uint32_t __fastcall kotor_m15e_trap_0063f400(void*, void*, std::uint32_t);
void __fastcall kotor_m15e_trap_0063f490(void*, void*);

// ------------------------------------------------------------------ lane-w3 door / placeable unit
// method tables (82 / 82 / 62 / 62 slots; original .rdata 00753960 / 007537D0 / 00758A08 / 007588B0)
extern const void* const recovered_client_door_methods_00753960[82];
extern const void* const recovered_client_placeable_methods_007537d0[82];
extern const void* const recovered_anim_base_door_methods_00758a08[62];
extern const void* const recovered_anim_base_placeable_methods_007588b0[62];
// CClientExoApp / CClientExoAppInternal
void* __fastcall recovered_client_get_door_by_id_005ed600(void* app, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_internal_get_door_by_id_005f1310(void* internal, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* app, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_internal_get_placeable_by_id_005f12c0(void* internal, void*, std::uint32_t id) noexcept(false);
void __fastcall recovered_client_add_to_hitcheck_ignore_list_005ed750(void* app, void*, std::uint32_t id) noexcept(false);
void __fastcall recovered_client_internal_add_to_hitcheck_ignore_list_005f6340(void* internal, void*, std::uint32_t id) noexcept(false);
// CSWCArea
void* __fastcall recovered_client_area_get_placeable_object_light_00605bf0(void* area, void*, std::uint32_t id) noexcept;
void __fastcall recovered_client_area_remove_placeable_object_light_00605b50(void* area, void*, std::uint32_t id,
                                                                             std::uint32_t destroy) noexcept(false);
std::uint32_t __fastcall recovered_client_area_parent_corpse_exists_006057b0(void* area, void*, std::uint32_t id) noexcept(false);
// CSWCObject (shared by doors and placeables)
void* __fastcall recovered_client_object_get_model_0063c080(void* self, void*, std::uint32_t part, std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_client_object_set_env_map_0063cb70(void* self, void*, std::uint32_t a1, std::uint32_t a2,
                                                                      void* resref) noexcept(false);
void __fastcall recovered_client_object_turn_on_shadows_0060b310(void* self, void*) noexcept(false);
void __fastcall recovered_client_object_turn_off_shadows_0060b320(void* self, void*) noexcept(false);
// CSWCDoor
void* __fastcall recovered_client_door_construct_00683bb0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_door_load_model_006842e0(void* self, void*, void* resref, std::uint32_t part,
                                                                   std::uint32_t a3) noexcept(false);
void __fastcall recovered_client_door_set_state_00683d40(void* self, void*, std::uint32_t state) noexcept(false);
void __fastcall recovered_client_door_set_animation_without_trigger_00683d00(void* self, void*, std::uint32_t anim) noexcept(false);
void __fastcall recovered_client_door_set_is_area_transition_00683cc0(void* self, void*, std::uint32_t flag) noexcept(false);
// CSWCPlaceable
void* __fastcall recovered_client_placeable_construct_00682270(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_placeable_load_model_006823f0(void* self, void*, void* resref, std::uint32_t part,
                                                                        std::uint32_t a3) noexcept(false);
void __fastcall recovered_client_placeable_set_appearance_00682e20(void* self, void*, std::uint32_t appearance) noexcept(false);
std::uint32_t __fastcall recovered_client_placeable_load_light_006826e0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_placeable_set_animation_00683630(void* self, void*, std::uint32_t anim, std::uint32_t a2,
                                                                           std::uint32_t a3) noexcept(false);
// CSWCAnimBase / CSWCAnimBaseDoor / CSWCAnimBasePlaceable
std::uint32_t __fastcall recovered_anim_base_set_animation_0069d4f0(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t,
                                                                   std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_anim_base_set_env_map_0069d7b0(void* self, void*, std::uint32_t a1, std::uint32_t a2,
                                                                 void* resref) noexcept(false);
void __fastcall recovered_anim_base_turn_off_shadows_0069dc30(void* self, void*) noexcept(false);
void* __fastcall recovered_anim_base_door_construct_006e56a0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_anim_base_door_load_model_006e5710(void* self, void*, void* resref, std::uint32_t part) noexcept(false);
std::uint32_t __fastcall recovered_anim_base_door_set_animation_internal_006e57b0(void* self, void*, void* gob, std::uint32_t anim,
                                                                                 std::uint32_t speed, std::uint32_t a4,
                                                                                 std::uint32_t a5) noexcept(false);
void* __fastcall recovered_anim_base_door_get_animation_name_006e59c0(void* self, void*, void* out, std::uint32_t anim) noexcept(false);
std::uint32_t __fastcall recovered_anim_base_door_server_to_client_animation_006e5ac0(void* self, void*, std::uint32_t anim) noexcept(false);
void* __fastcall recovered_anim_base_placeable_construct_006e4e50(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_anim_base_placeable_load_model_006e4ec0(void* self, void*, void* resref, std::uint32_t part) noexcept(false);

// ------------------------------------------------------------------ explicit traps (w3_traps_door_placeable.cpp)
// 00646650 CSWCObject::UnloadModel: existing owner kotor_m15e_object_unload_model_00646650 (0831-m15e-creature.obj; t2 review item 1).
std::uint32_t __fastcall kotor_m15e_object_unload_model_00646650(void*, void*, std::uint32_t);
void __fastcall recovered_client_object_unload_model_006825f0(void*, void*);
void __fastcall recovered_client_object_acquire_animation_base_0063d700(void*, void*, void*);
void __cdecl recovered_door_move_next_door_state_00683f00(void* gob, const char* event, void* door);
// 00682E00 CSWCPlaceable::GetServerPlaceable: lane-w4 kotor_fastcall_00682e00 (declared in w3_client_objects.h).
}
