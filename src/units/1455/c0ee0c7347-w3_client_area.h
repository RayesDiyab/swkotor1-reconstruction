#pragma once
// lane-w3 private declarations for the CSWCArea units (w3_client_area.cpp / w3_client_area_load.cpp).
#include "19ba31d3df-w3_client_internal.h"

namespace kotor_reconstruction { void StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }
namespace recovered_server_create { void* construct_list_internal_005e9aa0(void*); }
namespace texture_cells { extern volatile unsigned accounting_007a46d8; }                // [007A46D8]
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }    // [007A39E0] CExoBase*
namespace kotor_input_required { int __fastcall get_snapshot_time_difference_004adc80(void* timer, void*); }   // 004ADC80
namespace ini_comparison { int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*, void*, const char*); }
// existing trivial slot bodies (0486-3d-model-model-vtables.obj), exported extern "C" by the 0486 owner patch
// owner-0486/patches/0486-model_vtables-export-slot-leaves.patch (coordinator ruling t2-review item 3).
extern "C" {
unsigned __fastcall slot_return_zero_0063e7f0_a0(void*, void*);                         // @8   0063E7F0 xor eax,eax; ret
unsigned __fastcall slot_void_return_4_0060e760_a1(void*, void*, unsigned);             // @12  0060E760 ret 4
unsigned __fastcall slot_void_return_8_005b5e90_a2(void*, void*, unsigned, unsigned);   // @16  005B5E90 ret 8
}

extern "C" {
// ------------------------------------------------------------------ existing owners
extern float beam_alpha_0078e6a0;                                                         // [0078E6A0] (m47-scene)
unsigned __fastcall gui_load_slot_zero8_00409eb0(void*, void*, unsigned, unsigned);       // 00409EB0 return_zero_0x8
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, void* low, void* high);  // 004ADE40 RET 8
kotor_w3::Vector3* __cdecl vector_normalized_004ab180(kotor_w3::Vector3* out, const kotor_w3::Vector3* in);   // 004AB180
int __cdecl format_006fadb0(char*, const char*, ...);                                     // 006FADB0 sprintf
void __fastcall kotor_fastcall_0062fb90(void* gui, void*);                                // 0062FB90 FadeAndStopDialogAmbientTrack
void __fastcall kotor_m15e_client_ai_master_remove_object_00604a30(void* master, void*, void* object);   // 00604A30 RET 4
void __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void* list, void*, void* object);      // 00604810 RET 4
void __fastcall recovered_object_array_erase_004d8290(void* array, void*, std::uint32_t id);            // 004D8290 RET 4
unsigned char __fastcall recovered_object_array_remove_004d81a0(void* array, void*, unsigned id, void** out);   // 004D81A0 RET 8
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, std::uint32_t id, void** out);     // 004D8230 GetGameObject RET 8
void* __fastcall recovered_room_base_construct_005c7d10(void* self, void*);              // 005C7D10 CSWRoom::CSWRoom
void __fastcall recovered_room_base_reset_005c7d00(void* self, void*);                   // 005C7D00 CSWRoom::~CSWRoom
void __fastcall recovered_room_base_assign_005c7d70(void* self, void*, void* ref, void* position);   // 005C7D70 SetRoomInfo RET 8
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref* ref, void*, menu_resources::String* out_string);         // 00405F70 CopyToString(CExoString&) RET 4
void* __cdecl new_aur_scene_00458e70(const char* name);                                  // 00458E70 NewCAurScene
void* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob, void* mdl, void* mdx);   // 00449CC0 NewCAurObject
std::uint32_t __stdcall texture_make_current_00421ee0();                                   // 00421EE0 AurTextureMakeCurrent
int __fastcall c2da_current_get_int_00413660(void* table, void*, unsigned row, void* column, unsigned* out);   // 00413660 RET 0Ch
int __fastcall c2da_current_get_string_00413270(void* table, void*, int row, void* column, void* out);       // 00413270 RET 0Ch
extern void* kotor_cell_rules_007a3a28;                                                   // [007A3A28] CSWRules*
extern unsigned aurora_part_count_007a8a10;                                               // [007A8A10] (model-instance)
void* __fastcall c2da_005e9d70(void* list, void*, void* item);                            // 005E9D70 AddHead RET 4
int __cdecl crt_rand_006fc447();                                                          // 006FC447 rand
void* __fastcall kotor_fastcall_005f6410(void* internal, void*);                          // 005F6410 EmptyHitcheckIgnoreList
std::uint64_t __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*);   // bridge (member owner)

// ------------------------------------------------------------------ composition bridge (bridge/)
void* __fastcall recovered_bridge_area_base_construct_0058f6b0(void* self, void*);
void __fastcall recovered_bridge_area_initialize_static_objects_0058f800(void* self, void*);
void* __fastcall recovered_bridge_layout_new_005de770(void*, void*);                      // operator new(60h) + CLYT::CLYT
std::uint32_t __fastcall recovered_bridge_layout_load_005df140(void* layout, void*, void* resref);
std::uint32_t __fastcall recovered_bridge_layout_room_count_005a4c50(void* layout, void*);
void* __fastcall recovered_bridge_layout_room_name_005de470(void* layout, void*, void* out_ref, std::uint32_t i);
void* __fastcall recovered_bridge_layout_room_position_005de4a0(void* layout, void*, void* out_vector, std::uint32_t i);
void __fastcall recovered_bridge_layout_unload_005de450(void* layout, void*);

// ------------------------------------------------------------------ lane-w3 (aurora / mdl units)
void __cdecl recovered_aur_texture_suspend_updates_00420490() noexcept;
void __cdecl recovered_aur_texture_resume_updates_004204a0() noexcept;
void __cdecl recovered_aur_enable_interleaved_animations_0044f760() noexcept;
void __cdecl recovered_aur_disable_interleaved_animations_0044f770() noexcept;
void __cdecl recovered_aur_grass_clear_info_004a67a0() noexcept;
void* __fastcall recovered_aur_grass_info_construct_004a6870(void* self, void*) noexcept;
void __cdecl recovered_aur_grass_add_info_004a74f0(std::int32_t index, const void* info) noexcept(false);
void __fastcall recovered_aur_scene_set_ambient_0044fcd0(void* scene, void*, float r, float g, float b) noexcept;
void __cdecl recovered_set_clear_color_00401690(const char* text) noexcept(false);
unsigned char __cdecl recovered_aur_set_global_wind_004530d0(float x, float y, float z) noexcept(false);
unsigned char __cdecl recovered_aur_set_global_wind_variation_00453140(float a, float b) noexcept(false);
unsigned char __cdecl recovered_aur_control_part_0044b900(const char* command) noexcept(false);

// ------------------------------------------------------------------ lane-w3 (area units)
extern float kotor_cell_0078e408;
extern std::uint32_t recovered_client_area_texture_stats_00832c74[10];
void* __fastcall recovered_client_area_object_destructor_thunk_00606480(void* object, void*, std::uint32_t flags) noexcept(false);
void* __fastcall recovered_client_area_object_as_area_00606470(void* object, void*) noexcept;
void* __fastcall recovered_client_attach_sound_list_construct_00605800(void* self, void*) noexcept(false);
void __fastcall recovered_client_area_stop_weather_00605380(void* self, void*, std::uint32_t value) noexcept;
void __fastcall recovered_client_area_start_weather_006053a0(void* self, void*, std::uint32_t value) noexcept;
std::uint32_t __fastcall recovered_client_area_set_wind_00605260(void* self, void*, std::uint32_t strength) noexcept(false);
std::uint32_t __fastcall recovered_client_area_set_fog_color_00605090(void* self, void*, const kotor_w3::Vector3* colour) noexcept(false);
std::uint32_t __fastcall recovered_client_area_set_fog_00605870(void* self, void*, float, float, const kotor_w3::Vector3*) noexcept(false);
std::uint32_t __fastcall recovered_client_area_set_global_light_diffuse_00605160(void* self, void*, std::uint32_t, float, float, float) noexcept(false);
std::uint32_t __fastcall recovered_client_area_set_global_light_ambient_006051f0(void* self, void*, std::uint32_t, float, float, float) noexcept(false);
void __fastcall recovered_client_area_set_as_day_006058c0(void* self, void*) noexcept(false);
void __fastcall recovered_client_area_set_as_night_006059c0(void* self, void*) noexcept(false);
void __fastcall recovered_client_area_set_as_dawn_00606cf0(void* self, void*) noexcept(false);
void __fastcall recovered_client_area_set_as_dusk_00606c50(void* self, void*) noexcept(false);
void* __fastcall recovered_client_internal_get_sound_object_005f13b0(void* internal, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_get_sound_object_005ed620(void* app, void*, std::uint32_t id) noexcept(false);
void __fastcall recovered_client_area_update_sound_objects_time_of_day_00605b10(void* self, void*) noexcept(false);
void __fastcall recovered_client_module_set_time_of_day_state_00640640(void* self, void*, std::uint32_t, std::uint32_t) noexcept(false);
void __fastcall recovered_client_creature_update_area_id_0060ed90(void* self, void*) noexcept;
void __fastcall recovered_client_area_clear_tile_path_nodes_006075b0(void* self, void*) noexcept;
void __fastcall recovered_client_area_clear_all_map_pins_00606dd0(void* self, void*) noexcept(false);
void __fastcall recovered_client_area_unload_area_00609410(void* self, void*) noexcept(false);
void* __fastcall recovered_client_room_construct_00646080(void* self, void*) noexcept(false);
void __fastcall recovered_client_room_destruct_006460b0(void* self, void*) noexcept;
void* __fastcall recovered_client_room_vector_destructor_00604b60(void* self, void*, std::uint32_t flags) noexcept(false);
void* __fastcall recovered_client_room_spawn_room_006460c0(void* self, void*, void* scene) noexcept(false);
void __fastcall recovered_room_set_env_audio_props_005c7dc0(void* self, void*, std::int32_t count, const void* names,
                                                            const std::uint32_t* env, const std::uint32_t* level) noexcept(false);
std::uint32_t __fastcall recovered_client_area_load_area_00607610(void* self, void*, void* camera, std::uint32_t,
                                                                  std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_area_0064bab0(void* self, void*, std::uint32_t minor) noexcept(false);

// ------------------------------------------------------------------ lane-w3 (sound unit) / explicit traps
void* __fastcall recovered_client_ambient_sound_construct_0068df40(void* self, void*, void* area) noexcept(false);
void __fastcall recovered_client_ambient_sound_load_from_message_0068ee80(void* self, void*, void* message) noexcept(false);
void __fastcall recovered_client_ambient_sound_update_time_of_day_0068d690(void* self, void*) noexcept(false);
void __fastcall recovered_client_ambient_sound_stop_sounds_0068e1b0(void* self, void*, std::uint32_t) noexcept(false);
void __fastcall recovered_client_sound_object_stop_0066b130(void* self, void*) noexcept(false);
void __fastcall recovered_client_sound_object_update_time_of_day_0066ae90(void* self, void*);
void* __fastcall recovered_client_sound_object_construct_0066b200(void* self, void*);
void __fastcall recovered_client_sound_object_load_from_message_0066b420(void* self, void*, void* message);
void __fastcall recovered_client_sound_object_add_to_area_0066b8a0(void* self, void*, void* area, void* objects);
void* __fastcall recovered_client_area_scalar_destructor_0060a2e0(void* self, void*, std::uint32_t flags);
std::uint32_t __fastcall recovered_area_set_fog_0058fce0(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_area_set_is_night_0058fd10(void* self, void*, std::uint32_t);
std::uint32_t __fastcall recovered_client_area_set_weather_00604db0(void* self, void*, std::uint32_t, float);
void __fastcall recovered_client_creature_get_creature_light_00614aa0(void* creature, void*, std::uint32_t);
void* __fastcall recovered_client_minigame_construct_00671c40(void* self, void*, void* area);
std::uint32_t __fastcall recovered_client_minigame_load_006723d0(void* self, void*);
void __fastcall recovered_client_minigame_add_track_006720c0(void* self, void*, void* ref, void* position);
void __fastcall recovered_client_minigame_add_obstacle_00672240(void* self, void*, void* ref, void* position);
void* __fastcall recovered_bridge_layout_track_name_005de4d0(void* layout, void*, void* out_ref, std::uint32_t i);
void* __fastcall recovered_bridge_layout_track_position_005de500(void* layout, void*, void* out_vector, std::uint32_t i);
void* __fastcall recovered_bridge_layout_obstacle_name_005de530(void* layout, void*, void* out_ref, std::uint32_t i);
void* __fastcall recovered_bridge_layout_obstacle_position_005de560(void* layout, void*, void* out_vector, std::uint32_t i);
std::uint32_t __fastcall recovered_layout_track_count_005d64f0(void* layout, void*);
std::uint32_t __fastcall c2da_current_resource_size_00710e50(void* layout, void*);   // 00710E50 mov eax,[ecx+2Ch]; ret (owner 0306-tpc-lowers, t2b)
void* __fastcall recovered_client_map_pin_construct_00692540(void* self, void*);
void __fastcall recovered_client_area_add_placeable_light_00606e30(void* self, void*, std::uint32_t id, std::uint32_t kind,
                                                                   float x, float y, float z);
void __fastcall recovered_client_area_add_placeable_static_006072d0(void* self, void*, std::uint32_t id, std::uint32_t kind,
                                                                    float px, float py, float pz, float ox, float oy, float oz);
void* __fastcall recovered_behavior_attach_sound_construct_00690ef0(void* self, void*, void* sound_gob, void* gob,
                                                                    void* part, void* ref);
void __fastcall recovered_behavior_attach_sound_play_00691000(void* self, void*);
void __cdecl recovered_play_room_sound_event_00604b50(void* gob, void* event, void* part);
void __fastcall kotor_fastcall_00644650(void* module, void*, float delta);   // CSWCModule::Update RET 4; lane-w4 frame/client_update.cpp (ruling t2-3)
void __fastcall recovered_client_creature_set_way_points_0060c790(void* creature, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_area_apply_fire_and_forget_visual_effect_00604c90(void* self, void*, std::uint32_t effect,
                                                                                             float x, float y, float z);
}
