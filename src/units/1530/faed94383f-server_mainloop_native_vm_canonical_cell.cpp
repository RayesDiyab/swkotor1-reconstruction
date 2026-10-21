extern "C" unsigned __fastcall recovered_server_start_new_module_004ba920(void*,void*,void*);   // W1 tier9
extern "C" void __fastcall recovered_module_time_004c4100(void*,void*,void*,void*,void*,void*,void*,void*); // 1127 W2 optional
extern "C" void __fastcall recovered_update_client_objects_004b3ec0(void*,void*,void*,unsigned,unsigned,unsigned); // 1127 W2 t4b
extern "C" void* __fastcall recovered_object_area_004cb120(void*,void*); // CSWSObject::GetArea 004CB120 (1060 creature placement)
extern "C" void __cdecl live_free(void*);
namespace w2 { unsigned ftol2_006fae8c(double value); }
// 0886-gates (Claude): server main-loop ports bound to real bodies (exact original ABI, RET checked).
extern "C" {
void __fastcall recovered_timer_time_of_day_004add90(void*,void*,unsigned,void*,void*,void*,void*);    // 004ADD90 CWorldTimer::ConvertFromTimeOfDay RET 14h (w1_native_b7)
void __fastcall kotor_fastcall_004aebc0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned); // 004AEBC0 SetMoveToWorldTime RET 1Ch (claude_0886_bodies_server)
void __fastcall kotor_fastcall_00565530(void*,void*,int) noexcept(false);                              // 00565530 CSWPartyTable::UpdateMembers RET 4 (claude_w1_route_bodies unit)
unsigned __fastcall kotor_fastcall_004b2520(void*,void*);                                             // 004B2520 HasEnoughDiskSpaceForSaveGame RET 0 (claude_0886_bodies_server)
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(void*,void*,unsigned,unsigned,unsigned,const void*);// 0062ABF0 CGuiInGame::StartGlobalFade RET 10h (0814)
void __fastcall kotor_fastcall_005edc80(void*,void*,unsigned);                                        // 005EDC80 CClientExoApp::SetCanSendDialog RET 4 (claude_0886_bodies_server)
unsigned __fastcall kotor_fastcall_0056c7f0(void*,void*,unsigned);                                    // 0056C7F0 SendServerToPlayerLogin_NeedCharacter RET 4 (claude_0886_bodies_server)
unsigned __fastcall kotor_fastcall_0056b2b0(void*,void*,unsigned,unsigned);                           // 0056B2B0 SendServerToPlayerShutDownServer RET 8 (claude_0886_bodies_server)
}
// closeB (Claude): further former required(0x...) gates bound to real bodies (exact original ABI, RET checked).
extern "C" {
unsigned __fastcall kotor_fastcall_00505af0(void*,void*,void*,void*,unsigned,unsigned);              // 00505AF0 CSWSArea::LoadAreaEffects RET 10h (claude_aoe)
unsigned __fastcall kotor_fastcall_005057a0(void*,void*,void*,void*,unsigned,unsigned);              // 005057A0 CSWSArea::LoadStores RET 10h (claude_aoe_store)
unsigned __fastcall kotor_fastcall_00505060(void*,void*,void*,void*,unsigned,unsigned);              // 00505060 CSWSArea::LoadEncounters RET 10h (claude_encounter_load)
void __fastcall kotor_fastcall_0050c6b0(void*,void*,void*,const void*,void*);                        // 0050C6B0 CSWSArea::ApplyEffect RET 0Ch (claude_aoe)
unsigned __fastcall kotor_fastcall_00576360(void*,void*,void*);                                      // 00576360 SendServerToPlayerCharList RET 4 (claude_p2s_b_charlist)
void __fastcall kotor_fastcall_004b69c0(void*,void*,unsigned,unsigned);                              // 004B69C0 CServerExoAppInternal::Shutdown RET 8 (claude_p2s_b_misc)
void __fastcall closeB_stats_experience_notify_005af555(void*,void*,unsigned,unsigned,void*);           // 005AF555 SetExperience level-change tail (claude_closeB_xp)
void __fastcall kotor_fastcall_005edc40(void*,void*);                                                // 005EDC40 CClientExoApp::RunDeathSequence RET 0 (claude_closeB_death)
unsigned __fastcall kotor_fastcall_00500350(void*,void*,void*,void*,unsigned);                       // 00500350 CSWSCreature::LoadCreature RET 0Ch (claude_closeB_load_b)
void __fastcall closeB_object_load_effect_list_tail_004d1c36(void*,void*,void*,void*);               // 004D1C36 LoadEffectList loop (claude_closeB_load_b)
unsigned __fastcall kotor_fastcall_004d5120(void*,void*,void*,void*);                                // 004D5120 CSWSCombatRound::LoadCombatRound RET 8 (claude_closeB_load_a)
unsigned __fastcall closeB_combat_info_load_tail_0055239d(void*,void*,void*,void*,void*);            // 0055239D CCombatInformation::LoadData tail (claude_closeB_load_a)
void __fastcall kotor_fastcall_00578fc0(void*,void*,void*,unsigned,unsigned,unsigned);               // 00578FC0 CSWSAreaMap::LoadSavedAreaMapData RET 10h (claude_closeB_load_a)
unsigned __fastcall kotor_fastcall_004b58a0(void*,void*,unsigned,void*,unsigned,unsigned);            // 004B58A0 CServerExoAppInternal::SaveGame RET 10h (claude_closeB_save)
unsigned __fastcall kotor_fastcall_004b3000(void*,void*);                                            // 004B3000 ExportAllPlayers RET 0 (claude_closeB_save)
void __fastcall kotor_fastcall_004b3110(void*,void*);                                                // 004B3110 StallEventSaveGame RET 0 (claude_closeB_save)
unsigned __fastcall kotor_fastcall_005cdad0(void*,void*,char*,unsigned);                            // 005CDAD0 CSWPlaceableSurfaceMesh::LoadMeshText RET 8 (claude_closeB_mesh)
unsigned __fastcall kotor_fastcall_005ce160(void*,void*,char*,unsigned);                            // 005CE160 CSWDoorSurfaceMesh::LoadMeshText RET 8 (claude_closeB_mesh)
unsigned __fastcall kotor_fastcall_00582d70(void*,void*,char*,unsigned);                            // 00582D70 CSWRoomSurfaceMesh::LoadMeshText RET 8 (claude_walkmesh)
void __fastcall kotor_fastcall_00596c10(void*,void*);                                                // 00596C10 CSWCollisionMesh::ComputeMeshNormals RET 0 (claude_closeB_mesh)
unsigned __fastcall kotor_fastcall_00580d50(void*,void*,void*,unsigned,unsigned,unsigned);           // 00580D50 WeldEdgeVertices RET 10h (claude_closeB_mesh)
unsigned __fastcall kotor_fastcall_00580e90(void*,void*,void*,unsigned,unsigned,unsigned,unsigned);  // 00580E90 SetRoomAdjacency RET 14h (claude_closeB_mesh)
unsigned __fastcall closeB_server_unload_module_004b9240(void*,void*);                                // 004B9240 CServerExoAppInternal::UnloadModule RET 0 (claude_closeB_unload)
}
// loadgame (Claude): the last two save-load gates bound to real bodies (exact original ABI, RET checked).
extern "C" {
unsigned __fastcall kotor_fastcall_004b0a00(void*,void*,void*,void*);                                // 004B0A00 CServerAIMaster::LoadEventQueue RET 8 (claude_loadgame_queue)
void __fastcall loadgame_object_load_action_queue_tail_004ced20(void*,void*,void*,void*);           // 004CED20 LoadActionQueue loop (claude_loadgame_queue)
}
namespace kotor_game_prepare { void __fastcall SetGlobalUnpauseableState0062b410(void* self, void* edx, unsigned); } // 0062B410 (0461)
namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* s, void*); }          // 005B3190 CExoString() (0603) [0886-gates]
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // 005E5680 CExoString::Format [0886-gates] // 0886-gates: 006FAE8C __ftol2 (w2_ftol_native x87 provider) // 1118 single heap: CRT free -> game CRT heap _live_free 006FA390

#include "f3c98f67c1-vm_entering_commands.hpp"
#include <cstdio>

extern "C" const char* __cdecl kotor_cdecl_00401430(unsigned);          // W4: ClearLogCounters
extern "C" void __fastcall kotor_fastcall_00604d70(void*, void*);       // W4: StartSounds
#include "ce83393fee-vm_execute_live.hpp"
#include "a6dcee3ba2-area_event.hpp"
#include "e36007a58a-area_event_native_declarations.hpp"
#include "b5c74164ef-event_strings.hpp"
#include "e1584ea07b-event_strings_native_declarations.hpp"
#include "04e39532e3-vm_script_trace.hpp"
#include "8a220a8609-vm_script_trace_native_declarations.hpp"
#include "4526b7ab6a-vm_run_live.hpp"
#include "1ce5ca1173-vm_run_native_declarations.hpp"
#include "bfe9321a8f-resource_load.hpp"
#include "e0bb723b78-module_event.hpp"
#include "3a521d2f51-event_values.hpp"
#include "f68a5de3d8-module_event_native_declarations.hpp"
#include "6ef906090d-object_render_link.hpp"
#include "3a15111ca7-ai_event_cleanup.hpp"
#include "6210ae184f-ai_event_cleanup_native_declarations.hpp"
#include "d9f7755c05-ai_loop.hpp"
#include "18e18a7ec6-ai_loop_native_declarations.hpp"
#include "8716583a0a-module_load_status.hpp"
#include "1b27097645-module_complete.hpp"
extern "C" {unsigned recovered_module_debug_00832904=0;}
extern "C" unsigned __cdecl recovered_area_load_numeric_0050e216(unsigned,unsigned);
#include "81f1a0e87b-area_cameras_native_declarations.hpp"
#include "553ccb191d-gff_scratch_contract.hpp"
#include "7dc7b116e6-gff_quaternion.hpp"
#include "abc1c2a0bc-area_map_load.hpp"
#include "5fa3523ad7-area_cameras.hpp"
#include "6d9535057e-area_properties_load.hpp"
#include "4b95e56bd4-area_audio_load.hpp"
#include "6a8e82c82e-area_effect_objects.hpp"
#include "9c421b4500-area_stores.hpp"
#include "f96870dcff-placeable_placement.hpp"
#include "50af928f4c-creature_area_access.hpp"
#include "a6a12df192-item_destruct.hpp"
#include "984bbfd9f3-object_array_remove.hpp"
#include "00f19aee68-item_stack_compatible.hpp"
#include "5bdb2f3868-item_stack_merge.hpp"
#include "e6f1bde445-area_update.hpp"
#include "970e860c82-area_sounds.hpp"
#include "4481c8519a-area_placeables.hpp"
#include "890c36f1b2-placeable_fields.hpp"
#include "44f50851d6-placeable_inventory.hpp"
#include "4cfbea7fee-placeable_light.hpp"
#include "752b51fd44-placeable_finish.hpp"
#include "8a2d621db9-placeable_template.hpp"
#include "a4bd17492e-sound_template.hpp"
#include "2ece695531-sound_load.hpp"
#include "ab8ec18f5f-area_waypoints.hpp"
#include "f1d3ae2ccd-waypoint_load.hpp"
#include "739dd0401e-area_height.hpp"
#include "263488c2cd-waypoint_native_declarations.hpp"
#include "84b421ae7f-area_encounters.hpp"
#include "e0a2b760dc-trigger_geometry_native_declarations.hpp"
#include "4b1f87fb4e-trigger_placement.hpp"
#include "278cc6c186-trigger_placement_native_declarations.hpp"
#include "07079d57d6-trigger_fields.hpp"
#include "19f7561c9b-trigger_fields_native_declarations.hpp"
#include "fcb0600f6c-area_triggers.hpp"
#include "a683c6564d-trigger_template.hpp"
#include "bfb09dffe6-mesh_four_corners.hpp"
#include "8ceda21710-door_bounds.hpp"
#include "ec4ebe8e13-door_rotation.hpp"
#include "a4c425c05a-placeable_rotation.hpp"
#include "f2a59b6950-door_state.hpp"
#include "8c8184cee9-door_finish.hpp"
#include "52d0210857-door_mesh_callbacks.hpp"
#include "6db8ebacae-object_portrait_native_declarations.hpp"
#include "109da4e0f8-object_portrait.hpp"
#include "81f447d6d0-door_fields_tail.hpp"
#include "7740b82fe0-door_mesh.hpp"
#include "67edfc5864-placeable_mesh.hpp"
#include "5eb0fa5d91-door_load.hpp"
#include "d287fde0a5-door_template.hpp"
#include "bb0f4e4640-area_doors.hpp"
#include "49d83a3625-area_items.hpp"
#include "44571fda43-effect_handler_004d9200.hpp"
#include "c3ed1e5412-property_effect_004e5c60.hpp"
#include "adaf1cc47b-effect_handler_004d9170.hpp"
#include "92ab674f82-property_effect_004e60c0.hpp"
#include "db7babd52d-inventory_item.hpp"
#include "007ccc1b96-creature_add_known_spell.hpp"
#include "e19623a665-area_creature_collision_scan.hpp"
#include "73988e798b-area_creature_collision.hpp"
#include "78d3388cc6-area_region_wrapper.hpp"
#include "3e1861bf74-room_valid_helpers.hpp"
#include "d9120c7674-mesh_segment_clip.hpp"
#include "2bc6ebe4af-area_safe_valid.hpp"
#include "e8f03f9cd5-room_height.hpp"
#include "a3f1ab6a14-room_ground_ray.hpp"
#include "e0c2c91ea8-area_safe_position.hpp"
#include "ba8618a38f-area_find_room.hpp"
#include "48ec347b99-creature_template_final.hpp"
#include "d757fbde7b-item_weight.hpp"
#include "e55ff00406-creature_damage_bonus.hpp"
#include "6187622c32-item_property_query.hpp"
#include "dcd1aa48f7-creature_effect_index.hpp"
#include "8dfed8bebc-creature_effect_added.hpp"
#include <cstdio>
#include "3b4eb8c727-server_mainloop.hpp"
#include "a0d8f430bc-object_common.hpp"
#include "a5f7f94bf2-object_position.hpp"
#include "d8aa58838d-effect_construct.hpp"
#include "55a5c0213c-effect_creator.hpp"
#include "24f5ac2100-effect_dispatch.hpp"
#include "27cd9beeab-effect_apply.hpp"
#include "ccce921d25-faction_member.hpp"
#include "490d3436cd-creature_template_refresh.hpp"
#include "54e3e514b0-collision_refresh_numeric.hpp"
#include "40cc0e13c8-faction_member_helpers.hpp"
#include "eb775c622f-effect_insert.hpp"
#include "c259f62d18-effect_type68.hpp"
#include "9e15f28a27-creature_template_finish.hpp"
#include "0be5830053-object_vector_compare.hpp"
#include "a941c43697-creature_known_spells.hpp"
#include "552eaccde2-creature_portrait.hpp"
#include "f238a50c3d-creature_combat_load_guard.hpp"
#include "29c06967d8-creature_movement_rate.hpp"
#include "6faf88e8ec-creature_skill.hpp"
#include "19c787685c-creature_skill_helpers.hpp"
#include "ef1021a15c-creature_effect_skill.hpp"
#include "e1ce62bd4d-creature_stats_tail.hpp"
#include "2a3f53ffad-creature_add_feat.hpp"
#include "0134c5d2f1-creature_forcepoints.hpp"
#include "416de6a024-creature_stats_feats.hpp"
#include "577922da8e-creature_hitpoints_set.hpp"
#include "e66ad0742d-creature_stats_health.hpp"
#include "2f229665e8-creature_force_slots.hpp"
#include "f1680403fe-creature_class_leaves.hpp"
#include "cb72db8d85-gff_signed_short.hpp"
#include "c792940666-creature_stats_classes.hpp"
#include "91909b1eff-creature_stats_saves.hpp"
#include "f520b41803-creature_stats_appearance.hpp"
#include "4b8b9547f9-creature_stats_perception.hpp"
#include "f424106326-server_event.hpp"
#include "a8e114773c-server_system_message.hpp"
#include "ee357f0e9e-client_player_dispatch.hpp"
#include "8ae9986f66-client_loadbar.hpp"
#include "cf2f4b70ab-client_system_message.hpp"
#include "e32646612a-server_run_request.hpp"
#include "64e77ff6e3-server_admin_status.hpp"
#include "5f07c3d905-server_load_request.hpp"
#include "ec8326ac98-server_info.hpp"
#include "2e3df84990-server_join.hpp"
#include "e9b3c80ba5-player_start_message.hpp"
#include "9c10d911ac-local_variables_load.hpp"
#include "2a47bae702-builtin_variables_load.hpp"
#include "cc27e3d8da-server_clock.hpp"
#include "74fd2f505d-server_copy.hpp"
#include "74b4c7eb30-server_state.hpp"
#include "b55d89b194-module_constructor.hpp"
#include "dfb7313851-module_area_step.hpp"
#include "2eb43a0963-area_constructor.hpp"
#include "f2b2a75e47-area_load.hpp"
#include "ae2b8bbe38-area_reference.hpp"
#include "fd1b1456c6-layout_reference.hpp"
#include "2d76b5f080-layout_lifecycle.hpp"
#include "651c9cb395-layout_resource.hpp"
#include "db8bfea0c7-room_walkmesh.hpp"
#include "a645a9db20-room_mesh_load.hpp"
#include "988a79bc4a-room_mesh_binary.hpp"
#include "f5cf6cb077-room_mesh_binary_guards.hpp"
#include "727583093e-layout_room.hpp"
#include "a1a36f122b-layout_extract.hpp"
#include "5f20d706c7-area_primitives.hpp"
#include "dae56070e8-area_gff.hpp"
#include "b86f47b1aa-area_allocations.hpp"
#include "ea219c4834-area_content.hpp"
#include "f5270aca6d-area_path.hpp"
#include "d3477cbe39-area_creatures.hpp"
#include "3a5f1843a1-creature_template.hpp"
#include "435828129b-creature_stats_load.hpp"
#include "8ab2f63861-creature_stats_scalars.hpp"
#include "344d2bae10-creature_tag.hpp"
#include "16519b8e31-creature_stats_owner.hpp"
#include "d589c8408e-creature_owner_values.hpp"
#include "7789c613f3-creature_abilities.hpp"
#include "d975b5acf6-creature_effect_ability.hpp"
#include "54e6387e1b-area_properties.hpp"
#include "4eaccc35ea-area_room_storage.hpp"
#include "a57b1c421f-area_map_numeric.hpp"
#include "f379b6c8bb-area_gff_helpers.hpp"
#include "6e6b879d8a-module_initializers.hpp"
#include "94204f0df1-module_resources.hpp"
#include "97441c2beb-loading_progress.hpp"
#include "2e93fb3ddf-object_registration.hpp"
#include "d5b34ab98c-module_reference.hpp"
#include "5c60efc46b-module_gff.hpp"
#include "7c12c0dc84-module_load.hpp"
#include "1eddf41c28-gff_extra_readers.hpp"
#include "7929c7c3de-module_helpers.hpp"
#include "49f0801127-localized_read.hpp"
#include "57bbcbbbba-module_date.hpp"
#include "c8bea909f8-module_mount.hpp"
#include "39615b5632-module_finish.hpp"
#include "6ed28c15d6-module_factions.hpp"
#include "04373e317f-faction_reputation.hpp"
#include "96a1adb8a5-module_message.hpp"
#include <initializer_list>
#include "34213c6ab8-server_party.hpp"
#include "4ad9ba770f-server_party_update.hpp"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <new>
#if !defined(_M_IX86)
#error Native bridge requires x86
#endif
namespace kotor_scene3d {struct Vector {float x,y,z;};static_assert(sizeof(Vector)==12,"retained Vector ABI");struct Quaternion;}
#include "ba24338184-collision_methods.h"
extern "C" kotor_scene3d::Quaternion* __fastcall quaternion_construct_004ac960(kotor_scene3d::Quaternion*,void*,kotor_scene3d::Vector,float);
namespace kotor_world_los {struct SurfaceHitInfo;}
using namespace kotor::server_mainloop;
void native_ai_loop(Word);
namespace kotor_reconstruction{struct StartupString;}
namespace recovered_player_message{void initialize_read_004d66b0(void*,void*,unsigned,unsigned,unsigned);unsigned overrun_004d6230(void*);unsigned underrun_004d6240(void*);}
namespace directory_list{char __fastcall At005e5600(kotor_reconstruction::StartupString*,void*,int);}
namespace base_ini_reconstruction{kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);}
namespace alias_path{struct AliasListView;}
namespace path_resolution{kotor_reconstruction::StartupString* __fastcall resolve_005e68a0(alias_path::AliasListView*,void*,kotor_reconstruction::StartupString*,kotor_reconstruction::StartupString*,unsigned);}
namespace options_video{struct String;}
namespace client_movies{int __fastcall StringFind_005e5eb0(options_video::String*,void*,options_video::String*,int);}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;extern void* volatile input_007a39e4;}
namespace kotor_menu_10_19::dependencies {extern void* base_007a39e0;}
namespace menu_continuation {struct ExoTimers {__int64 __thiscall GetHighResolutionTimer_005e9260();};}
namespace kotor_input_required {unsigned __fastcall low_resolution_timer_005e9250(void*,void*);}
namespace kotor_reconstruction {struct ArrayMutations;struct StartupString {int __thiscall BorrowedEquals005e5290(const char*);};}
namespace menu_list_growth {void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned);}
namespace ini_comparison {int __fastcall CompareNC005e58c0(kotor_reconstruction::StartupString*,void*,const char*);}
extern "C" {extern void* kotor_cell_rules_007a3a28;int __fastcall c2da_current_get_int_entry_00413510(void*,void*,int,int,int*);int __fastcall c2da_current_get_string_00413270(void*,void*,int,void*,void*);}
namespace directory_list {kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*);}
extern "C" void* __fastcall c2da_manager_004076e0(void*,void*,void*);
extern "C" int __fastcall net_layer_send_message_to_player_005d4cb0(void*,void*,unsigned,const char*,unsigned,unsigned);
extern "C" unsigned __fastcall unresolved_key_entry_from_table_004071a0(void*,void*,void*,void*,unsigned,void*,void*,unsigned);
extern "C" {void* __fastcall exo_locinternal_construct_005ecaf0(void*,void*);void __fastcall exo_locinternal_clear_005ecb10(void*,void*);void* __fastcall resource_append_005e9dc0(void*,void*,void*);void* __fastcall resman_string_counted_005e5b70(void*,void*,const char*,unsigned);unsigned __fastcall exo_locstring_add_005ea340(void*,void*,unsigned,void*,int);}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,const kotor_reconstruction::StartupString*);
namespace menu_resources {struct Ref {Ref* __thiscall construct_00406d80(const char*);};}
namespace menu_resources {struct Manager {unsigned __thiscall encapsulated_004087a0(kotor_reconstruction::StartupString*,unsigned);unsigned __thiscall directory_00408800(kotor_reconstruction::StartupString*);unsigned __thiscall exists_00408bc0(Ref*,unsigned,unsigned*);};Manager* __cdecl BorrowCanonicalManager007a39e8();}
namespace base_ini_reconstruction {kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*,int);}
namespace kotor_reconstruction {void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
namespace kotor_original {void __fastcall kotor_fastcall_00417fb0(void*,void*,int);}
extern "C" {void __fastcall kotor_fastcall_004088d0(void*,void*,void*);int __fastcall find_005e5f20(void*,void*,int,int);unsigned __fastcall async_string_length_005e5790(void*,void*);void __fastcall packed_return_false_0063e7c0(void*,void*,void*);void __cdecl kotor_cdecl_00401c10(float,unsigned,unsigned);void __fastcall rim_enter_005e9300(void*,void*);void __fastcall rim_leave_005e9310(void*,void*);__declspec(dllimport) void __stdcall Sleep(unsigned long);__declspec(dllimport) unsigned long __stdcall ResumeThread(void*);}
namespace resource_ref_assignment {menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*,void*,menu_resources::Ref*,const menu_resources::Ref*);}
namespace kotor_q05 {struct CResGFF;}
extern "C" {void* __fastcall gff_resource_deleting_destroy_00413010(void*,void*,unsigned);kotor_q05::CResGFF* __fastcall gui_load_gff_construct_00410630(kotor_q05::CResGFF*,void*,unsigned,const char*,const menu_resources::Ref*);unsigned __fastcall resource_equal_004060b0(void*,void*,void*);unsigned __fastcall c2da_equal_text_004060d0(void*,void*,const char*);unsigned __fastcall c2da_not_equal_text_00406060(void*,void*,const char*);void __fastcall c2da_cancel_00409b00(void*,void*);unsigned __fastcall c2da_release_00409cf0(void*,void*,void*);void* __fastcall c2da_lookup_004074d0(void*,void*,void*,unsigned);void __fastcall c2da_register_00407680(void*,void*,void*,unsigned,void*);void __fastcall resource_request_00408620(void*,void*);void* __fastcall c2da_resource_base_ctor_00406460(void*,void*);unsigned __fastcall gff_resource_serviced_00410740(void*,void*);unsigned __fastcall gff_resource_freed_00410880(void*,void*);unsigned __stdcall c2da_resource_constant_word_00406490();}
namespace kotor_q05 {struct StructHandle;struct FieldRecord;struct CResGFF {unsigned __thiscall GetTopLevelStruct(StructHandle*);unsigned __thiscall GetElementType(StructHandle*);FieldRecord* __thiscall GetField(StructHandle*,unsigned);unsigned __thiscall GetDataField(FieldRecord*,unsigned*);};}
extern "C" {
extern void* kotor_cell_tlk_007a3a08;
void __fastcall c2da_manager_004070a0(void*,void*);int __fastcall async_string_equal_005e5200(void*,void*,void*);unsigned __fastcall c2da_current_resource_demand_00409b10(void*,void*);void __fastcall c2da_release_demand_00409b80(void*,void*);
unsigned __fastcall gff_label_00411630(void*,void*,void*,const char*);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*,void*,void*,const char*,unsigned*,unsigned char);
unsigned __fastcall gui_load_gff_dword_00411c20(void*,void*,void*,const char*,unsigned*,unsigned);
int __fastcall gui_load_gff_int_00411c90(void*,void*,void*,const char*,unsigned*,int);
float __fastcall gui_load_gff_float_00411d00(void*,void*,void*,const char*,unsigned*,float);
void* __fastcall gui_load_gff_string_00411ec0(void*,void*,void*,void*,const char*,unsigned*,void*);
void* __fastcall gui_load_gff_resref_00411e10(void*,void*,void*,void*,const char*,unsigned*,void*);
void __fastcall exo_locstring_destroy_005ea000(void*,void*);
void __fastcall kotor_fastcall_004adef0(void*,void*,void*);
unsigned __fastcall gff_list_004118c0(void*,void*,void*,void*,const char*);
unsigned __fastcall gff_list_count_00411940(void*,void*,void*);
unsigned __fastcall gff_list_element_00411990(void*,void*,void*,void*,unsigned);
void* __fastcall resource_append_005e9dc0(void*,void*,void*);
void __fastcall c2da_ref_to_string_00405f70(void*,void*,void*);

void __fastcall kotor_fastcall_0041db50(void*,void*,unsigned,void*);
}
namespace table29_required {void* __fastcall gui_game_object_004d7d60(void*,void*,unsigned,unsigned);}
namespace lane_config_required {void* __fastcall GetPartyTable_004aee70(void*,void*);}
extern "C" {void* __fastcall c2da_default_ref_00405ed0(void*,void*);
void* __fastcall unresolved_ref_assign_00406290(void*,void*,void*,const char*);
void* __fastcall exo_locstring_construct_005e9e10(void*,void*);
void __fastcall frame_LogStringCtor_005e5a90(void*,void*,const char*);
void* __fastcall async_assign_string_005e5c50(void*,void*,void*);
__declspec(dllimport) int __stdcall CopyFileA(const char*,const char*,int);
__declspec(dllimport) unsigned long __stdcall GetFileAttributesA(const char*);
__declspec(dllimport) int __stdcall SetFileAttributesA(const char*,unsigned long);
const char* __fastcall WindowCStr005e5670(void*,void*);
void* __fastcall kotor_fastcall_005ed8b0(void*,void*);
void* __fastcall frame_GetCharacter_006346c0(void*,void*,unsigned);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void**);
extern unsigned char kotor_rules_cell8_007a39dc;
void __fastcall kotor_fastcall_004adc70(void*,void*);
void __fastcall frame_R4_004adbd0(void*,void*);
void __fastcall frame_R4_005d57d0(void*,void*,unsigned);
void __fastcall frame_R4_005d5650(void*,void*,unsigned);
unsigned __fastcall frame_R4_005ed690(void*,void*);
void* __fastcall kotor_fastcall_004aede0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(void*,void*,unsigned*,unsigned*);
void* __fastcall server_internal_get_module_004b14f0(void*,void*);
unsigned __fastcall kotor_fastcall_004aea40(void*,void*);
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
void __fastcall GuiAssign005e5140(void*,void*,const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
void* __fastcall WindowCopy005e5b00(void*,void*,void*);
void __fastcall kotor_fastcall_005ed310(void*,void*);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
unsigned __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
unsigned __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
void __fastcall kotor_fastcall_00401280(void*,void*);
}
extern "C" { unsigned recovered_saved_state_007a1b40=1; unsigned recovered_saved_state_007a1b44=0; unsigned recovered_area_width_007a1b48=40; unsigned recovered_area_height_007a1b4c=40; }
extern "C" {const char* recovered_server_status_007a1f14="ServerStatus";const char* recovered_player_list_007a1f3c="PlayerList";}
namespace kotor_rules {void* __fastcall kotor_fastcall_00550c00(void*,void*,unsigned short);}
extern "C" void __fastcall kotor_m15_array_u16_add_unique_005a7670(void*,void*,unsigned);
extern "C" {extern unsigned kotor_rules_cell32_007a2244;extern unsigned kotor_rules_cell32_007a2248;}
extern "C" unsigned __fastcall frame_LogIsEmpty_005e58a0(void*,void*);
extern "C" void* __fastcall kotor_m15e_slot00c_self_00641db0(void*,void*);
namespace async_load {struct Ref;struct String;}
extern "C" async_load::Ref* __fastcall async_ref_assign_string_004061f0(async_load::Ref*,void*,async_load::Ref*,async_load::String*);
extern "C" {unsigned recovered_door_bounds_debug_00832858=0;}
namespace {
void* ptr(Word x){return reinterpret_cast<void*>(x);}Word word(void* x){return reinterpret_cast<Word>(x);}
[[noreturn]] void required(Word){std::terminate();}
extern "C" {unsigned extern kotor_rules_cell32_007a2118;unsigned extern kotor_rules_cell32_007a20d0;unsigned extern kotor_rules_cell32_007a20cc;unsigned extern kotor_rules_cell32_007a2108;unsigned extern kotor_rules_cell32_007a2104;}
extern "C" {extern unsigned kotor_rules_cell32_007a20d4;extern unsigned kotor_rules_cell32_007a2124;extern unsigned kotor_rules_cell32_007a2110;}
struct NativeMemory:Memory {
 Word read(Word a) override {switch(a){case 0x7a3a00:return frame_control::message_manager_007a3a00;case 0x73f400:return 0x3e000000;case 0x7a20d4:return kotor_rules_cell32_007a20d4;case 0x7a2124:return kotor_rules_cell32_007a2124;case 0x7a2110:return kotor_rules_cell32_007a2110;case 0x74673c:return 48;case 0x7466a4:return 10;case 0x7466ac:return 12;case 0x746684:return 2;case 0x7466b0:return 13;case 0x7466d8:return 23;case 0x746738:return 47;case 0x746778:return 63;case 0x74670c:return 36;case 0x746700:return 33;case 0x746794:return 70;case 0x746798:return 71;case 0x7467a8:return 75;case 0x7467ac:return 76;case 0x7467a0:return 73;case 0x746764:return 58;case 0x746780:return 65;case 0x746744:return 50;case 0x746770:return 61;case 0x746758:return 55;case 0x7466e4:return 26;case 0x746680:return 1;case 0x7466d4:return 22;case 0x746768:return 59;case 0x7466f0:return 29;case 0x746830:return 110;case 0x746710:return 37;case 0x7a2118:return kotor_rules_cell32_007a2118;case 0x7a20d0:return kotor_rules_cell32_007a20d0;case 0x7a20cc:return kotor_rules_cell32_007a20cc;case 0x7a2108:return kotor_rules_cell32_007a2108;case 0x7a2104:return kotor_rules_cell32_007a2104;case 0x746678:return 1;case 0x74671c:return 40;case 0x7466f4:return 30;case 0x746788:return 67;case 0x7a2244:return kotor_rules_cell32_007a2244;case 0x7a2248:return kotor_rules_cell32_007a2248;case 0x746838:return 0;case 0x74683c:return 1;case 0x746840:return 2;case 0x746844:return 3;case 0x746848:return 4;case 0x74684c:return 5;case 0x74682c:return 109;case 0x7a249c:return word(const_cast<char*>("ServerStatus"));case 0x7a24a4:return word(const_cast<char*>("ModuleLoaded"));case 0x7a24a8:return word(const_cast<char*>("Idle"));case 0x7a24ac:return word(const_cast<char*>("ModuleRunning"));case 0x7a24b4:return word(const_cast<char*>("ModuleList"));case 0x7a24bc:return word(const_cast<char*>("CharactervaultList"));case 0x7a24c4:return word(const_cast<char*>("PlayerList"));case 0x7a24d4:return word(const_cast<char*>("Module"));case 0x7a24dc:return word(const_cast<char*>("Run"));case 0x7a24e8:return word(const_cast<char*>("SavegameList"));case 0x7a1c60:return word(const_cast<char*>("Echo"));case 0x7a1c64:return word(const_cast<char*>("ServerStatus"));case 0x7a1c68:return word(const_cast<char*>("GetStatus"));case 0x7a1c78:return word(const_cast<char*>("GetModuleList"));case 0x7a1c88:return word(const_cast<char*>("GetPlayerList"));case 0x7a1c9c:return word(const_cast<char*>("Module"));case 0x7a1ca0:return word(const_cast<char*>("Load"));case 0x7a1ca4:return word(const_cast<char*>("Run"));case 0x7a1ca8:return word(const_cast<char*>("Save"));case 0x7a1f1c:return word(const_cast<char*>("ModuleLoaded"));case 0x7a1f20:return word(const_cast<char*>("Idle"));case 0x7a1f24:return word(const_cast<char*>("ModuleRunning"));case 0x7a1f14:return reinterpret_cast<Word>(recovered_server_status_007a1f14);case 0x7a1f3c:return reinterpret_cast<Word>(recovered_player_list_007a1f3c);case 0x7a3a28:return word(kotor_cell_rules_007a3a28);case 0x7a1b48:return recovered_area_width_007a1b48;case 0x7a1b4c:return recovered_area_height_007a1b4c;case 0x7a1b40:return recovered_saved_state_007a1b40;case 0x7a1b44:return recovered_saved_state_007a1b44;case 0x7a3a08:return word(kotor_cell_tlk_007a3a08);case 0x7a39e8:return word(menu_resources::BorrowCanonicalManager007a39e8());case 0x7a39fc:return word(kotor_menu_window_cells::manager_007a39fc);case 0x7a39e4:return word(kotor_menu_window_cells::input_007a39e4);case 0x7a39e0:return word(kotor_menu_10_19::dependencies::base_007a39e0);case 0x746644:return 0x7f000000;case 0x74410c:return 0x53c;case 0x74c53c:return 0x800000;case 0x74c52c:return 0xfffffff7;case 0x74c534:return 0xfffffff5;default:Word x;std::memcpy(&x,ptr(a),4);return x;}}
 void write(Word a,Word x) override {if(a==0x7a1b48){recovered_area_width_007a1b48=x;return;}if(a==0x7a1b4c){recovered_area_height_007a1b4c=x;return;}if(a==0x7a1b40){recovered_saved_state_007a1b40=x;return;}if(a==0x7a1b44){recovered_saved_state_007a1b44=x;return;}std::memcpy(ptr(a),&x,4);}
 Word read16(Word a) override {if(a==0x74668c)return 4;if(a==0x7473b8)return 0;/* 0886-gates: .rdata words read by party_update_004b6da0 */if(a==0x7473c0)return 1;if(a==0x74678c)return 68;if(a==0x7473c8)return 4;if(a==0x74bf10)return 5;unsigned short x;std::memcpy(&x,ptr(a),2);return x;}
 void write16(Word a,Word x) override {const unsigned short v=static_cast<unsigned short>(x);std::memcpy(ptr(a),&v,2);}
 Word read8(Word a) override {if(a==0x746634)return 5;if(a==0x74666c)return 0;if(a==0x746638)return 9;if(a>=0x74b31c&&a<=0x74b31e)return a-0x74b31c+3;if(a==0x73ecd2)return 7;if(a==0x74411c)return 6;if(a==0x744128)return 1;if(a==0x744134)return 23;if(a==0x746632)return 3;if(a==0x746633)return 4;if(a==0x7a39dc)return kotor_rules_cell8_007a39dc;return *static_cast<unsigned char*>(ptr(a));}
};
struct NativeClock:ClockPorts {
 void world_timer_reset_004adc70(Word self) override {kotor_fastcall_004adc70(ptr(self),nullptr);}
 Word low_time_005e9250(Word self) override{return kotor_input_required::low_resolution_timer_005e9250(ptr(self),nullptr);}
 Wide divide_006fb030(Wide a,Wide b) override{return a/b;}
 void notify_timeout_0056b2b0(Word a0,Word a1,Word a2) override{(void)kotor_fastcall_0056b2b0(ptr(a0),nullptr,a1,a2);}
};
#include "525309f8fa-party_native.inc"
struct NativeCopy:CopyPorts {
 Word cstr_005e5670(Word a) override{return reinterpret_cast<Word>(WindowCStr005e5670(ptr(a),nullptr));}
 Word copy_file(Word a,Word b,Word c) override{return static_cast<Word>(CopyFileA(static_cast<const char*>(ptr(a)),static_cast<const char*>(ptr(b)),static_cast<int>(c)));}
 Word attributes(Word a) override{return GetFileAttributesA(static_cast<const char*>(ptr(a)));}
 Word set_attributes(Word a,Word b) override{return static_cast<Word>(SetFileAttributesA(static_cast<const char*>(ptr(a)),b));}
};
struct NativeState:StatePorts {
 Wide high_time_005e9260(Word a) override{return static_cast<Wide>(static_cast<menu_continuation::ExoTimers*>(ptr(a))->GetHighResolutionTimer_005e9260());}
 Word list_at_005e9b60(Word a,Word b) override{return unresolved_list_at_pos_005e9b60(ptr(a),nullptr,ptr(b));}
 void list_next_005e9b90(Word a,Word b) override{(void)unresolved_list_next_005e9b90(ptr(a),nullptr,static_cast<void**>(ptr(b)));}
 void update_player_004b3ec0(Word a,Word b,Word c,Wide d) override{recovered_update_client_objects_004b3ec0(ptr(a),nullptr,ptr(b),c,static_cast<unsigned>(d),static_cast<unsigned>(d>>32));}
};
#include "f406b641e5-module_resources_native_typed_ref.inc"

#include "ff6f9706ef-module_reference_native.inc"
#include "c874725a0e-module_native.inc"
#include "74cf6f8c57-local_variables_native.inc"
#include "1853d1265c-module_load_native.inc"
#include "c70d03c1e2-module_complete_native.inc"
#include "b9a47ccdd1-vm_script_trace_native.inc"
#include "0ef80eee55-area_properties_native.inc"
#include "9e8e358803-layout_native.inc"
#include "d8f2c023b0-room_walkmesh_native_mask.inc"
#include "fec1a79de0-door_mesh_callbacks_native.inc"
#include "2f5478f1d8-door_mesh_native_callbacks.inc"
#include "f9d5b7e8b6-placeable_mesh_native.inc" // closeB: private shadow copy (text loader bound)
#include "177f052ea7-room_mesh_load_native_typed_data.inc"
#include "594262dd30-layout_room_native_observation.inc"
#include "c77ede2355-creature_portrait_native.inc"
#include "4936095274-object_portrait_native.inc"
#include "26e59c8344-creature_stats_native_combatguard.inc" // 0886-gates: private copy of the level-history-t2 overlay
#include "c39af0d361-object_common_native.inc"
#include "66868433ce-creature_known_spells_native_add.inc"
#include "e55375876f-effect_construct_native.inc"
#include "83bee8ebf4-effect_creator_native.inc"
#include "d9126a3449-effect_dispatch_native.inc"
#include "541fd6b2fb-faction_member_native.inc"
#include "e76b0ec54d-effect_apply_native_faction.inc"
#include "d8838d6fe5-area_update_native.inc"
#include "f9d00e65d4-object_position_native_update.inc"
#include "0d23fa770c-creature_template_refresh_native.inc"
#include "5e7f6242c6-creature_template_final_native.inc"
#include "c43641483c-creature_template_finish_native_inventory_item.inc"
#include "8d141ee0b5-creature_template_native_finish.inc"
#include "3de2fc93f3-room_ground_ray_native_typed.inc"
#include "98dabf2601-door_bounds_native.inc"
#include "69b638662e-door_rotation_native.inc"
#include "d4b82bff74-placeable_rotation_native.inc"
#include "edc02c57f8-room_height_native.inc"
#include "8f3f45f682-area_safe_position_native_reaction.inc"
#include "99ac8d7e1d-door_state_native.inc"
#include "23a64c932e-door_finish_native_state.inc"
#include "6c0a1f211f-area_native_load_numeric.inc"
#include "c558b29b91-server_join_native.inc"
#include "71d348db0d-server_event_native.inc"
#include "a198219e07-server_load_request_native.inc"
#include "036ce85a52-server_run_native.inc"
#include "f55522e9e7-server_system_native.inc"
#include "99ad2aa5b8-client_loadbar_native.inc"
#include "a93ff13810-client_player_dispatch_party_overlay.inc" // claude-build: private copy (server-to-player routes)
#include "fd06e16969-client_system_native.inc"
struct NativePorts:Ports {NativeMemory& m;NativeClock clock;explicit NativePorts(NativeMemory& v):m(v){}
 void update_004b1ee0(Word a0) override {(void)kotor::server_mainloop::update_004b1ee0(m,clock,a0);}
 void tick_004adbd0(Word a0) override {frame_R4_004adbd0(ptr(a0),nullptr);}
 void ai_mode_005d57d0(Word a0,Word a1) override {frame_R4_005d57d0(ptr(a0),nullptr,a1);}
 void ai_update_005d5650(Word a0,Word a1) override {frame_R4_005d5650(ptr(a0),nullptr,a1);}
 void party_update_004b6da0(Word a0) override {NativePartyUpdate p(m);alignas(8) unsigned char scratch[0x80];kotor::server_mainloop::party_update_004b6da0(m,p,a0,word(scratch));}
 void party_positions_004b4e80(Word a0) override {NativePartyPosition p(m);kotor::server_mainloop::party_positions_004b4e80(m,p,a0);}
 void net_update_004b0b70(Word a) override {native_ai_loop(a);}
 void update_state_004b6950(Word a0,Word a1) override {NativeState p;Word node;kotor::server_mainloop::update_state_004b6950(m,p,a0,a1,word(&node));}
 Word gui_005ed690(Word a0) override {return frame_R4_005ed690(ptr(a0),nullptr);}
 Word timer_004aede0(Word a0) override {return word(kotor_fastcall_004aede0(ptr(a0),nullptr));}
 void timer_fields_004ade40(Word a0,Word a1,Word a2) override {frame_GetWorldTime_004ade40(ptr(a0),nullptr,static_cast<unsigned*>(ptr(a1)),static_cast<unsigned*>(ptr(a2)));}
 void timer_split_004add90(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5) override {recovered_timer_time_of_day_004add90(ptr(a0),nullptr,a1,ptr(a2),ptr(a3),ptr(a4),ptr(a5));}
 Word module_004b14f0(Word a0) override {return word(server_internal_get_module_004b14f0(ptr(a0),nullptr));}
 void module_date_004c4100(Word a,Word b,Word c,Word d,Word e,Word f,Word g) override {recovered_module_time_004c4100(ptr(a),nullptr,ptr(b),ptr(c),ptr(d),ptr(e),ptr(f),ptr(g));}
 void set_date_004aebc0(Word a0,Word a1,Word a2,Word a3,Word a4,Word a5,Word a6,Word a7) override {kotor_fastcall_004aebc0(ptr(a0),nullptr,a1,a2,a3,a4,a5,a6,a7);}
 void set_tick_004af0c0(Word a0,Word a1) override {m.write(m.read(a0+4)+0x100ac,a1);}
 void set_day_004af0a0(Word a0,Word a1) override {m.write(m.read(a0+4)+0x100b0,a1);}
 void party_flag_00565530(Word a0,Word a1) override {kotor_fastcall_00565530(ptr(a0),nullptr,static_cast<int>(a1));}
 Word transition_004ba920(Word a,Word b) override {return recovered_server_start_new_module_004ba920(ptr(a),nullptr,ptr(b));}   // W1 tier9
 Word player_id_004aea40(Word a0) override {return kotor_fastcall_004aea40(ptr(a0),nullptr);}
 Word creature_004ae770(Word a0,Word a1) override {return word(kotor_fastcall_004ae770(ptr(a0),nullptr,a1));}
 Word creature_area_004cb120(Word a0) override {return word(recovered_object_area_004cb120(ptr(a0),nullptr));}  // CSWSObject::GetArea, selected 1060 provider
 Word client_busy_005edad0(Word a0) override {return m.read(m.read(a0+4)+0x288);}
 Word can_save_004b2520(Word a0) override {return kotor_fastcall_004b2520(ptr(a0),nullptr);}
 void empty_005b3190(Word a0) override {m.write(a0,0);m.write(a0+4,0);}
 void literal_005e5140(Word a0,Word a1) override {if(a1!=0x7455a0)required(a1);GuiAssign005e5140(ptr(a0),nullptr,"AUTOSAVE");}
 Word save_004b58a0(Word a0,Word a1,Word a2,Word a3,Word a4) override {return kotor_fastcall_004b58a0(ptr(a0),nullptr,a1,ptr(a2),a3,a4);}  // closeB
 void destroy_005e5c20(Word a0) override {frame_LogStringDtor_005e5c20(ptr(a0),nullptr);}
 void fade_0062abf0(Word a0,Word a1,Word a2,Word a3,Word a4) override {kotor_m15_ingame_start_global_fade_0062abf0(ptr(a0),nullptr,a1,a2,a3,ptr(a4));}
 void client_mode_005edc80(Word a0,Word a1) override {kotor_fastcall_005edc80(ptr(a0),nullptr,a1);}
 void input_005ed310(Word a0) override {kotor_fastcall_005ed310(ptr(a0),nullptr);}
 void gui_mode_0062b410(Word a0,Word a1) override {kotor_game_prepare::SetGlobalUnpauseableState0062b410(ptr(a0),nullptr,a1);}
 Word client_internal_005ed530(Word a0) override {return word(kotor_fastcall_005ed530(ptr(a0),nullptr));}
 void reset_00604d70(Word a) override {kotor_fastcall_00604d70(reinterpret_cast<void*>(static_cast<unsigned long>(a)),nullptr);}
 void save_done_004b3000(Word a0) override {(void)kotor_fastcall_004b3000(ptr(a0),nullptr);}  // closeB
 void player_notice_0056c7f0(Word a0,Word a1) override {(void)kotor_fastcall_0056c7f0(ptr(a0),nullptr,a1);}
 void unload_phase_004b3110(Word a0) override {kotor_fastcall_004b3110(ptr(a0),nullptr);}  // closeB
 void log_00401430(Word a) override {(void)kotor_cdecl_00401430(a);}
 void copy_string_005e5b00(Word a0,Word a1) override {(void)WindowCopy005e5b00(ptr(a0),nullptr,ptr(a1));}
 Word load_mode_005edfb0(Word a0) override {return kotor::server_mainloop::load_mode_005edfb0(m,a0);}
 Word copy_module_004b1680(Word a0,Word a1,Word a2,Word a3) override {(void)a0;(void)a3;NativeCopy p;return kotor::server_mainloop::copy_module_004b1680(p,a1,a2);}
 Word allocate_006fa7e6(Word a0) override {return word(::operator new(a0));}
 Word construct_module_004c84a0(Word a0,Word a1,Word a2,Word a3) override {NativeModuleConstructor p(m);alignas(8) unsigned char scratch[0x80];return module_constructor_004c84a0(m,p,a0,a1,a2,a3,word(scratch));}
 void assign_005e5c50(Word a0,Word a1) override {(void)async_assign_string_005e5c50(ptr(a0),nullptr,ptr(a1));}
 Word start_module_004c9050(Word a0,Word a1,Word a2) override {NativeModuleLoad p(m);alignas(8) unsigned char scratch[0xc0];return module_load_004c9050(m,p,a0,a1,a2,word(scratch));}
 // closeB: 004BA5B0 CServerExoAppInternal::DealWithLoadGameError(int error) RET 4, transcribed in place.
 void error_004ba5b0(Word self,Word error) override {const Word state=m.read(m.read(0x7a39fc)+0x14);/*004BA5B0..B6*/(void)closeB_server_unload_module_004b9240(ptr(self),nullptr);/*004BA5BC*/m.write(state+0x38,error);m.write(state+0x14,1);/*004BA5C1..C8*/if(m.read8(0x7a39dc)==2){m.write(state+0x14,0);m.write(state,0);m.write(state+4,0);}/*004BA5CF..E1*/m.write16(self+0x10008,0);m.write(self+0x100b4,0);/*004BA5E4..EB*/status_0056c980(message_004aedb0(m.read(m.read(0x7a39fc)+8)),1,error);/*004BA5F1..04 EndStallEvent(1,error)*/}
 void party_reset_005639c0(Word a0) override {kotor::server_mainloop::party_reset_005639c0(m,a0);}
 Word area_step_004c5720(Word a0,Word a1,Word a2) override {NativeAreaStep p(m);return module_area_step_004c5720(m,p,a0,a1,a2);}
 Word finish_module_004c5880(Word a) override {;const Word result=native_module_complete(a);;return result;}
 Word message_004aedb0(Word a0) override {return m.read(m.read(a0+4)+0x10010);}
 void status_0056c980(Word a,Word b,Word c) override {NativeModuleMessage p(m);alignas(4) unsigned scratch[2];(void)module_load_status_0056c980(m,p,a,b,c,word(scratch));}
 void notify_00568310(Word a0,Word a1) override {NativeModuleMessage p(m);(void)player_status_00568310(m,p,a0,a1);}
 void notify_00579920(Word a,Word b) override {NativeAdminStatus p(m);alignas(4) unsigned char scratch[0x20];admin_status_00579920(m,p,a,b,word(scratch));}
 Word list_at_005e9b60(Word a0,Word a1) override {return unresolved_list_at_pos_005e9b60(ptr(a0),nullptr,ptr(a1));}
 void list_next_005e9b90(Word a0,Word a1) override {(void)unresolved_list_next_005e9b90(ptr(a0),nullptr,static_cast<void**>(ptr(a1)));}
 void player_ready_00576360(Word a0,Word a1) override {(void)kotor_fastcall_00576360(ptr(a0),nullptr,ptr(a1));}  // closeB
 void free_006fa390(Word a0) override {::operator delete(ptr(a0));}
 Wide time_005e9260(Word a0) override {return static_cast<Wide>(static_cast<menu_continuation::ExoTimers*>(ptr(a0))->GetHighResolutionTimer_005e9260());}
 Word timeout_004b4ab0(Word a0,Wide a1) override {return kotor::server_mainloop::timeout_004b4ab0(m,clock,a0,a1);}
 void shutdown_004b69c0(Word a0,Word a1,Word a2) override {kotor_fastcall_004b69c0(ptr(a0),nullptr,a1,a2);}  // closeB
 void quit_00401280(Word a0) override {kotor_fastcall_00401280(ptr(a0),nullptr);}
 void tail_004b1e10(Word a0,Wide a1) override {(void)kotor::server_mainloop::tail_004b1e10(m,a0,a1);}
};
}
extern "C" void* __fastcall recovered_collision_construct_005d0ce0(void*self,void*){NativeMemory m;NativeAreaAllocation p;return ptr(collision_construct_005d0ce0(m,p,word(self)));}
extern "C" unsigned __fastcall recovered_client_system_00675710(void*self,void*,const void*data,unsigned size){NativeMemory m;NativeClientSystem p(m);alignas(8) unsigned char scratch[0x40];return client_system_00675710(m,p,word(self),word(const_cast<void*>(data)),size,word(scratch));}
extern "C" unsigned __fastcall recovered_client_player_0066a640(void*self,void*,const void*data,unsigned size){NativeMemory m;NativeClientDispatch p(m);alignas(8) unsigned char scratch[0x28];return client_player_dispatch_0066a640(m,p,word(self),word(const_cast<void*>(data)),size,word(scratch));}
extern "C" unsigned __fastcall recovered_server_system_00528380(void*self,void*,unsigned id,const void*packet,unsigned length){NativeMemory m;NativeServerSystem p(m);alignas(8) unsigned char scratch[0x80];return server_system_00528380(m,p,word(self),id,word(const_cast<void*>(packet)),length,word(scratch));}
extern "C" unsigned __fastcall recovered_player_message_00524c40(void* self,void*,void* player,unsigned subtype){NativeMemory m;NativeModuleMessage p(m);return player_start_message_00524c40(m,p,word(self),word(player),subtype);}
extern "C" void __fastcall recovered_server_event_004b78f0(void* self,void*,unsigned id,unsigned kind,unsigned third){NativeMemory m;NativeServerEvent p(m);alignas(8) unsigned char scratch[0x30];server_event_004b78f0(m,p,word(self),id,kind,third,word(scratch));}
extern "C" unsigned __fastcall kotor_fastcall_004b1680(void*,void*,unsigned source,unsigned destination,unsigned){NativeCopy p;return kotor::server_mainloop::copy_module_004b1680(p,source,destination);}
extern "C" unsigned __fastcall kotor_fastcall_004babb0(void* self,void*){NativeMemory m;NativePorts p(m);alignas(8) unsigned char scratch[0x80];return mainloop_004babb0(m,p,word(self),word(scratch));}
extern "C" unsigned __fastcall kotor_fastcall_004ae860(void* self,void*){Word internal;std::memcpy(&internal,static_cast<unsigned char*>(self)+4,4);return kotor_fastcall_004babb0(ptr(internal),nullptr);}

extern "C" void* __fastcall recovered_gff_localized_00411fd0(void*gff,void*,void*out,void*root,const char*label,unsigned*status,void*def){NativeMemory m;NativeLocalized p(m);unsigned scratch[6];return ptr(gff_localized_00411fd0(m,p,word(gff),word(out),word(root),word(const_cast<char*>(label)),word(status),word(def),word(scratch)));}
extern "C" void* __fastcall recovered_localized_assign_005e9e70(void*dst,void*,void*src){NativeMemory m;NativeLocalized p(m);unsigned scratch;return ptr(localized_assign_005e9e70(m,p,word(dst),word(src),word(&scratch)));}
extern "C" unsigned short __fastcall recovered_gff_word_00411b40(void*self,void*,void*root,const char*label,unsigned*status,unsigned short fallback){NativeMemory m;NativeGffExtra p(m);return static_cast<unsigned short>(gff_short_00411b40(m,p,word(self),word(root),word(const_cast<char*>(label)),word(status),fallback));}

extern "C" void* __fastcall recovered_effect_construct_00503e40(void* a,void*,unsigned b){return ptr(native_effect_construct(word(a),b));}
extern "C" void __fastcall recovered_effect_integer_005036a0(void*a,void*,unsigned i,unsigned v){NativeMemory m;effect_integer_005036a0(m,word(a),i,v);}

extern "C" void __fastcall recovered_effect_creator_00503a00(void*a,void*,unsigned id){native_effect_creator(word(a),id);}

extern "C" unsigned __fastcall recovered_effect_dispatch_004d8320(void*self,void*,void*target,void*effect,unsigned mode){NativeMemory m;NativeEffectDispatch p;return effect_dispatch_004d8320(m,p,word(self),word(target),word(effect),mode);}
extern "C" unsigned __fastcall recovered_effect_type68_004dccc0(void*,void*,void*target,void*effect,unsigned){NativeMemory m;NativeEffectType68 p(m);return effect_type68_004dccc0(m,p,word(target),word(effect));}

extern "C" void __fastcall recovered_effect_apply_004d14f0(void*a,void*,void*b,unsigned c,unsigned d){native_effect_apply(word(a),word(b),c,d);}

extern "C" void __fastcall kotor_fastcall_004ffaf0(void*,void*,void*); // 004FFAF0 CSWSCreature::UpdateAttributesOnEffect RET 4 (claude_0886_bodies_creature; re-checks the type as 004FFB15..004FFB32)
struct NativeCreatureEffectAdded final:CreatureEffectAddedPorts { void ability_continuation_004ffb38(Word a,Word b) override {kotor_fastcall_004ffaf0(ptr(a),nullptr,ptr(b));} };
extern "C" void __fastcall recovered_creature_effect_index_004f14f0(void*self,void*) {NativeMemory m;creature_effect_index_004f14f0(m,word(self));}
extern "C" void __fastcall recovered_creature_effect_added_004ffaf0(void*self,void*,void*effect) {NativeMemory m;NativeCreatureEffectAdded p;creature_effect_added_004ffaf0(m,p,word(self),word(effect));}
#include "c65e5b78d5-creature_damage_native.inc"
extern "C" unsigned __fastcall recovered_creature_melee_damage_005a7a20(void*self,void*,unsigned offhand){NativeMemory m;NativeCreatureDamage p(m);return creature_melee_damage_005a7a20(m,p,word(self),offhand);}
extern "C" unsigned __fastcall recovered_creature_ranged_damage_005a7db0(void*self,void*){NativeMemory m;NativeCreatureDamage p(m);unsigned scratch;return creature_ranged_damage_005a7db0(m,p,word(self),word(&scratch));}
extern "C" unsigned __fastcall recovered_item_property_query_005539c0(void*self,void*,void**output,unsigned type,unsigned subtype){NativeMemory m;return item_property_query_005539c0(m,word(self),word(output),type,subtype);}

extern "C" void* __fastcall recovered_item_lookup_004ae760(void*,void*,unsigned);
struct NativeItemWeight final:ItemWeightPorts{Word item(Word a,Word b)override{return word(recovered_item_lookup_004ae760(ptr(a),nullptr,b));}};
extern "C" unsigned __fastcall recovered_item_weight_005562a0(void*self,void*){NativeMemory m;NativeItemWeight p;return item_weight_005562a0(m,p,word(self));}
extern "C" unsigned __fastcall recovered_inventory_weight_00556100(void*self,void*){NativeMemory m;NativeItemWeight p;return inventory_weight_00556100(m,p,word(self));}

extern "C" void __fastcall recovered_object_position_004cd1a0(void*self,void*,void*vector,unsigned update,unsigned unused,unsigned suppress){native_object_position(word(self),word(vector),update,unused,suppress);}
#include "e0d6b3404e-property_effect_native.inc"
#include "2dad380019-effect_handler_native_query.inc"
#include "8bf4aad64b-property_effect_second_native.inc"

extern "C" unsigned __fastcall recovered_effect_handler_004d9200(void*,void*,void*target,void*effect,unsigned mode){NativeMemory m;NativeEffectHandler p(m);return effect_handler_004d9200(m,p,word(target),word(effect),mode);}
#include "2a93054e4f-effect_array_copy.hpp"
extern "C" void* __fastcall recovered_array_copy_004fdd30(void*self,void*,void*source){NativeMemory m;NativeEffectInsert p;return ptr(effect_array_copy_004fdd30(m,p,word(self),word(source)));}
#include "334eb94be9-combat_modifier_construct.hpp"
struct NativeCombatModifier final:CombatModifierPorts{void byte(Word a,Word b)override{*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(b);}};
extern "C" void* __fastcall recovered_combat_modifier_construct_0054f260(void*self,void*){NativeMemory m;NativeCombatModifier p;return ptr(combat_modifier_construct_0054f260(m,p,word(self)));}
#include "d2d01aad9d-effect_rank_native.inc"
extern "C" unsigned __cdecl recovered_effect_log_round_00501652(int value){effect_rank_logarithm(static_cast<unsigned>(value),&recovered_effect_rank_scratch_008327e0);return recovered_effect_rank_scratch_008327e0;}

extern "C" unsigned __fastcall recovered_mesh_four_corners_00596e40(void*self,void*,void*a,void*b,void*c,void*d){NativeMemory m;NativeMeshVertexBounds p(m);alignas(4)unsigned char scratch[0x48];return mesh_four_corners_00596e40(m,p,word(self),word(a),word(b),word(c),word(d),word(scratch));}

extern "C" void* __fastcall recovered_placeable_lookup_004ae7a0(void*server,void*,unsigned id){NativeMemory m;NativeCreatureSkill p(m);return ptr(p.area(word(server),id));}
#include "c9b98979d7-item_stack_native_localized.inc"
#include "d5f982ba03-item_destruct_native.inc"
#include "d7b9d3dfaa-area_audio_load_native.inc"
#include "3188a9cfa0-ai_event_cleanup_native.inc"
#include "0a3a00d39a-object_render_link_native.inc"
#include "1125508ce4-module_event_native.inc"
#include "4c06c27d4b-area_event_native.inc" // 0886-gates: private copy of area-creatures/area_event_native.inc
#include "9a2cfbc8c6-ai_loop_native_area.inc"
#include "5a827be7df-vm_execute_native_exports.hpp" // 1126 W6 t4b export header
#include "64d4468dd3-vm_run_native_execute.inc"
#include "b4d968b047-0886_read_script_file_export.inc"
#include "9c891ae3a2-event_strings_native.inc"

// Binding-only reuse: W1 caller name denotes the existing W2-owned004BE5E0.
#pragma comment(linker, "/alternatename:@w1x_area_test_safe_point_004be5e0@16=@w2x_area_safe_point_004be5e0@16")
// 005AF480 CSWSCreatureStats::SetExperience(xp, flag) RET 8 over 0978 creature_experience_005af480 (0886 NativeCreatureOwnerValues).
extern "C" void __fastcall w2x_stats_set_experience_005af480(void* stats, void*, unsigned xp, unsigned flag) {
  NativeMemory m; NativeCreatureOwnerValues p(m); alignas(8) unsigned char scratch[0x20];
  kotor::server_mainloop::creature_experience_005af480(m, p, word(stats), xp, flag, word(scratch));
}
#include "d84ed24ada-vm_entering_commands_native.inc"
#include "6890debc31-w1_0886_exports.inc"
#include "0837493feb-claude_0886_exports.inc" // 0886-gates: C wrappers for claude_0886_bodies_*.cpp
#include "c7b3927c26-claude_vm_vma_0886_exports.inc" // vmA: LoadFromTemplate / LoadWaypoint wrappers
#include "ef1fe3c711-w2_0886_exports.inc"
#include "660700dbfd-w5_0886_exports.inc"
#include "4d3fec33ce-w3_bridge_native.inc"
// W6 server harness (HARNESS-ONLY export, no behaviour change): CServerAIMaster::UpdateState 004B0B70 for --frames.
extern "C" void __fastcall recovered_native_ai_update_004b0b70(void* ai_master, void*) { native_ai_loop(word(ai_master)); }

// Private case40 G1 bridge (interface-only; see deliverables).
#include "be6ac8e644-case40_g1_module_bridge.v2.inc"

// Private module G1 nested bodies (source-only; see deliverables).
#include "439c8731fa-case40_module_nested_bodies.inc"

// Private module G1 parent bodies (source-only; see deliverables).
#include "028e16ba55-case40_module_parents.inc"

// Private module G1 004C44D0 body (source-only; see deliverables).
#include "228a514478-case40_004c44d0.inc"

// Private module G1 remaining bodies (source-only; see deliverables).
#include "a2478a03d8-case40_remaining_bodies.inc"
