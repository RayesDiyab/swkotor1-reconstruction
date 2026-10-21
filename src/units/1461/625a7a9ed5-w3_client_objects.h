#pragma once
// lane-w3 private declarations for the client object units (w3_client_objects.cpp, w3_client_appearance.cpp).
#include "c0ee0c7347-w3_client_area.h"

namespace sound_volume29_required { unsigned __fastcall predicate_006054e0(void*, void*, void*); }   // 006054E0 return_true_4
namespace recovered_save_stats { unsigned short maximum_hp_00647a80(void*); }                    // 00647A80 GetMaxHitPoints

extern "C" {
// ------------------------------------------------------------------ existing owners
void* __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);               // 005E9B90 GetNext RET 4
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);      // 005E9B60 GetAtPos RET 4
void* __fastcall resource_append_005e9dc0(void* list, void*, void* item);                         // 005E9DC0 AddTail RET 4
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                        // 005ED8B0 GetSWParty
std::uint32_t __fastcall kotor_m15_client_is_player_creature_005eda30(void* app, void*, void* creature);   // 005EDA30 RET 4
std::uint32_t __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);      // 005ED550
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, std::uint32_t id);    // 005ED5A0 RET 4
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*);                       // 00616A20 CSWCCreature()
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* a, const void* b);                 // 004AA980 operator==
std::uint32_t __fastcall frame_R4_005ed5b0(void* app, void*, std::uint32_t id);

// ------------------------------------------------------------------ composition bridge
void __fastcall recovered_bridge_dword_array_add_unique_006449d0(void* list, void*, std::uint32_t value);

// ------------------------------------------------------------------ lane-w3 (objects unit)
// [007A2484] camera-follow flag: single owner lane-w4 (src/frame/client_update.cpp `kotor_cell_007a2484`, initial 1;
// coordinator ruling t2-4). The former lane-w3 cell recovered_client_camera_follow_flag_007a2484 was dropped.
extern volatile std::uint32_t kotor_cell_007a2484;
void __fastcall recovered_client_set_character_change_in_progress_005ee1a0(void* app, void*, std::uint32_t) noexcept;
void __fastcall recovered_client_set_player_creature_005ed780(void* app, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_internal_get_player_by_id_005f5cb0(void* internal, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_get_player_by_id_005ed590(void* app, void*, std::uint32_t id) noexcept(false);
// 00677410 CSWCMessage::SendPlayerToServerMessage RET 10h: lane-w2 owner (coordinator ruling t3).
unsigned __fastcall recovered_client_send_00677410(void* self, void*, unsigned major, unsigned minor, void* data, unsigned size);
std::uint8_t __fastcall recovered_object_array_add_external_004d7ff0(void* array, void*, std::uint32_t* id, void* object,
                                                                     std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_client_ai_list_add_object_006047a0(void* list, void*, void* object) noexcept(false);
std::uint32_t __fastcall recovered_client_ai_master_add_object_00604a20(void* master, void*, void* object) noexcept(false);
void __fastcall recovered_client_creature_set_moving_orientation_0060b380(void* self, void*, const kotor_w3::Vector3*) noexcept(false);
// 0066DCC0 CExoArrayList<int>::remove RET 4: lane-w1 owner (lane-w1/tiers/tier4 w1_native.cpp).
void __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, std::uint32_t value);
void __fastcall recovered_client_object_set_fade_state_0063e220(void* self, void*, std::uint32_t) noexcept(false);
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(void* self, void*, std::uint32_t, std::uint32_t,
                                                                        std::uint32_t, std::uint32_t) noexcept(false);
void __fastcall recovered_client_object_set_visibility_on_set_area_0063e350(void* self, void*, void* area) noexcept(false);
void __fastcall recovered_client_object_set_area_0063f570(void* self, void*, void* area) noexcept(false);
void __fastcall recovered_client_creature_set_area_00614b20(void* self, void*, void* area) noexcept(false);
void __fastcall recovered_client_object_show_shadow_blob_00616fb0(void* self, void*) noexcept(false);
void __fastcall recovered_client_creature_turn_on_shadows_0060e490(void* self, void*) noexcept(false);
void __fastcall recovered_client_creature_turn_off_shadows_0060e520(void* self, void*) noexcept(false);
void __fastcall recovered_client_creature_update_relative_perception_00613a70(void* self, void*, std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_creature_add_0064c0a0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_creature_control_object_0064caf0(void* self, void*, std::uint32_t) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_update_add_00656bd0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_update_delete_00656c90(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_update_appearance_00656c60(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_update_update_0066a2a0(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_game_object_update_0066a4a0(void* self, void*, std::uint32_t minor) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_creature_appearance_00655e40(void* self, void*) noexcept(false);

// ------------------------------------------------------------------ lane-w4 providers (coordinator rulings t2-3 and the OVERLAP-W4
// decision: exact W4 symbols and prototypes from lane-w4/src; RET n checked against the original callee bodies)
void __fastcall kotor_fastcall_005f2bb0(void* client, void*, std::uint32_t id);                  // SetPlayerCreature RET 4 (frame/client_update.cpp)
void __fastcall kotor_fastcall_0063f9d0(void* module, void*, std::uint32_t id, std::uint32_t flag);   // CSWCModule::SetCameraFollow RET 8 (camera/module_camera.cpp)
void __fastcall kotor_fastcall_005ed720(void* app, void*) noexcept(false);                       // CClientExoApp::AttachPlayerGameObjectToArea RET (frame/client_update.cpp)
void __fastcall kotor_fastcall_0060b3c0(void* creature, void*, kotor_w3::Vector3* direction);    // CSWCCreature::SetStationaryOrientation RET 4 (creature/creature_anim.cpp)
std::uint32_t __fastcall kotor_fastcall_0060ef50(void* creature, void*, std::uint32_t in_party); // CSWCCreature::SetInParty RET 4 (party/membership.cpp)
float __fastcall kotor_fastcall_0063dee0(void* object, void*);                                   // CSWCObject::GetFinalFadeAlpha RET, ST0 (creature/creature_misc.cpp)
void* __fastcall kotor_fastcall_00682e00(void* placeable, void*);                                // CSWCPlaceable::GetServerPlaceable RET (party/selection.cpp)
// ------------------------------------------------------------------ explicit traps (w3_traps_objects.cpp; owner noted)
std::uint32_t __fastcall recovered_client_handle_item_add_0064e7b0(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_trigger_add_0064f4b0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_door_add_0064cf30(void*, void*);
std::uint32_t __fastcall recovered_client_handle_aoe_add_00651680(void*, void*);
std::uint32_t __fastcall recovered_client_handle_placeable_add_0064d500(void*, void*);
std::uint32_t __fastcall recovered_client_handle_creature_delete_0064cc90(void*, void*);
std::uint32_t __fastcall recovered_client_handle_item_delete_0064eac0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_trigger_delete_0064f960(void*, void*);
std::uint32_t __fastcall recovered_client_handle_door_delete_0064d3a0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_aoe_delete_006517b0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_placeable_delete_0064da20(void*, void*);
std::uint32_t __fastcall recovered_client_handle_creature_update_006655e0(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_generic_object_update_00651880(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_item_update_0064eb90(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_trigger_update_0064f430(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_door_update_0064d420(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_aoe_update_006517f0(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_placeable_update_0064dae0(void*, void*, std::uint32_t, std::uint32_t);
std::uint32_t __fastcall recovered_client_handle_update_gui_elements_0066a1f0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_update_player_info_00668bb0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_update_work_remaining_0064dcf0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_creature_visual_effect_0064c3c0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_creature_strip_equipped_items_00651af0(void*, void*);
std::uint32_t __fastcall recovered_client_handle_item_destroy_00651b30(void*, void*);
std::uint32_t __fastcall recovered_client_handle_item_script_add_00654b10(void*, void*);
}

