#pragma once
// lane-w3 tier t3 private declarations for w3_client_t3.cpp (W2 review: the client trapped on the first 5/1 update
// message). GameObjectUpdate records 'I' (00668BB0 PlayerInfo) and 'G' (0066A1F0 GuiElements + its Inventory /
// Repository / Container / QuickbarItemUseCount readers), the item 'A' record (0064E7B0) and the trigger 'A' record
// (0064F4B0), plus the leaves they reach that no existing owner provides. Original: swkotor.exe GOG K1 (read-only).
// Conventions: w3_client.h (D1 thiscall veneers, live_operator_new / live_free only, D83 traps).
#include "835c32b896-w3_client_t2.h"
#include "8fdb3c6e33-w3_client_appearance.h"
#include "31d5f4a7dd-w3_client_door_placeable.h"

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // 004AEE70 (0575)
namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {   // 005E03C0 CExoArrayList::Allocate (0098-viewport-list-growth)
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned capacity);
}

extern "C" {
// ------------------------------------------------------------------ existing owners (1122 map / lanes; RET n checked)
void __fastcall kotor_m15b_creature_stats_clear_feats_00648320(void* stats, void*);              // 00648320 RET (0819)
void __fastcall kotor_m15b_creature_stats_clear_known_spells_00649520(void* stats, void*);       // 00649520 RET (0819)
void __fastcall kotor_m15e_creature_clear_visibility_list_006145d0(void* creature, void*);       // 006145D0 RET (0831)
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, std::uint32_t value); // 005A7670 RET 4 (0811)
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, std::uint32_t id);   // 0059B6D0 RET 4 (0820)
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* app, void*);                     // 005EDB00 RET (0831)
void* __fastcall kotor_m15e_item_get_equipped_by_00646630(void* item, void*);                    // 00646630 RET (0835)
void __fastcall recovered_known_spell_add_00649660(void* class_info, void*, std::uint32_t list,
                                                   std::uint32_t spell);                         // 00649660 RET 8 (1083)
void __fastcall recovered_event_object_004d7780(void* data, void*, unsigned, unsigned);          // 004D7780 RET 8 (1017)
void* __fastcall recovered_message_data_construct_004d69f0(void* self, void*);                   // 004D69F0 RET (lane-w1 tier 4)
unsigned __fastcall recovered_npc_object_00564700(void* party, void*, unsigned index, unsigned create,
                                                  unsigned apply);                               // 00564700 RET 0Ch (lane-w2)
char __fastcall c2da_current_string_index_005e5600(void* string, void*, int index);              // 005E5600 operator[] RET 4 (0288)

// ------------------------------------------------------------------ lane-w3 tier t3 (w3_client_t3.cpp)
// GameObjectUpdate records
std::uint32_t __fastcall recovered_client_handle_gui_inventory_0064fc10(void* self, void*) noexcept(false);         // RET
std::uint32_t __fastcall recovered_client_handle_gui_repository_0064fdf0(void* self, void*, std::uint32_t) noexcept(false);   // RET 4
std::uint32_t __fastcall recovered_client_handle_gui_container_0064fef0(void* self, void*) noexcept(false);         // RET
std::uint32_t __fastcall recovered_client_handle_gui_quickbar_use_count_0064fb20(void* self, void*) noexcept(false);   // RET
// PlayerInfo leaves
void __fastcall recovered_client_creature_delete_sla_list_00617a70(void* creature, void*) noexcept;           // RET
void __fastcall recovered_client_creature_create_sla_list_00617a20(void* creature, void*) noexcept(false);    // RET
void __fastcall recovered_sla_list_allocate_00610bc0(void* list, void*, std::uint32_t count) noexcept(false); // RET 4
void* __fastcall recovered_sla_construct_006475c0(void* self, void*) noexcept;                                // RET
void __fastcall recovered_gui_in_game_set_level_up_mode_00628650(void* gui, void*, std::uint32_t mode) noexcept;   // RET 4
void __fastcall recovered_level_up_stats_set_skill_rank_00647590(void* stats, void*, std::uint32_t skill,
                                                                 std::uint32_t rank) noexcept;               // RET 8
void __fastcall recovered_client_set_gui_skill_rank_0064ce90(void* self, void*, void* creature, std::uint32_t value,
                                                             std::uint32_t mask, std::uint32_t skill) noexcept;   // RET 10h
void __fastcall recovered_client_creature_set_special_walk_animation_0060e1d0(void* creature, void*,
                                                                              std::uint32_t on) noexcept(false);   // RET 4
void __fastcall recovered_creature_stats_remove_known_spell_00649fe0(void* stats, void*, std::uint32_t class_index,
                                                                     std::uint32_t spell) noexcept(false);   // RET 8
void __fastcall recovered_creature_stats_add_known_spell_00649f90(void* stats, void*, std::uint32_t class_index,
                                                                  std::uint32_t spell) noexcept(false);      // RET 8
void __fastcall recovered_u16_list_remove_004e3b20(void* list, void*, std::uint32_t value) noexcept;          // RET 4
void __fastcall recovered_creature_stats_set_feat_remaining_uses_00649430(void* stats, void*, std::uint32_t feat,
                                                                          std::uint32_t uses) noexcept(false);   // RET 8
void __fastcall recovered_client_creature_update_relative_perception_0060c980(void* creature, void*,
                                                                              void* entry) noexcept(false);   // RET 4
std::uint8_t __fastcall recovered_client_creature_appearance_gender_char_00696ca0(void* appearance,
                                                                                  void*) noexcept(false);    // RET (AL)
// Trigger
extern volatile std::uint32_t recovered_client_show_all_triggers_0083285c;                       // [0083285C] (0)
extern const void* const recovered_client_trigger_methods_00754440[82];
void* __fastcall recovered_client_trigger_construct_006911e0(void* self, void*) noexcept(false);          // RET
void __fastcall recovered_client_trigger_set_cursor_006914d0(void* self, void*, std::uint32_t cursor) noexcept(false);   // RET 4
void __fastcall recovered_client_trigger_display_00691470(void* self, void*, std::uint32_t on) noexcept(false);        // RET 4
void __fastcall recovered_client_trigger_add_to_render_list_00691cc0(void* self, void*, std::uint32_t color) noexcept(false);   // RET 4
void __fastcall recovered_client_trigger_special_display_00691e00(void* self, void*, std::uint32_t all) noexcept(false);  // RET 4
void __fastcall recovered_client_trigger_update_color_006913b0(void* self, void*, std::uint32_t unused) noexcept(false);    // RET 4
void* __fastcall recovered_client_trigger_get_name_006912b0(void* self, void*, void* out) noexcept(false);   // RET 4 (hidden return)
std::uint32_t __fastcall recovered_client_trigger_is_hostile_006912e0(void* self, void*) noexcept;        // RET
std::uint32_t __fastcall recovered_client_trigger_load_model_006910d0(void* self, void*, void* resref, std::uint32_t part,
                                                                      std::uint32_t flag) noexcept(false);   // RET 0Ch
extern const void* const recovered_anim_base_trigger_methods_00758c70[62];
void* __fastcall recovered_anim_base_trigger_construct_006e5f40(void* self, void*) noexcept(false);       // RET
std::uint32_t __fastcall recovered_anim_base_trigger_load_model_006e5fb0(void* self, void*, void* resref,
                                                                         std::uint32_t part) noexcept(false);   // RET 8

// ------------------------------------------------------------------ explicit traps (w3_traps_t3.cpp; owner noted)
void __fastcall recovered_client_add_feedback_message_005ed360(void* app, void*, std::uint32_t type, void* data,
                                                               void* text);                      // RET 0Ch
std::uint32_t __fastcall recovered_client_handle_character_sheet_00667cc0(void* self, void*);    // RET
std::uint32_t __fastcall recovered_gui_in_game_get_party_access_panel_up_0062b4d0(void* gui, void*, std::uint32_t);   // RET 4
std::int32_t __fastcall recovered_gui_in_game_get_current_equip_screen_npc_0062fea0(void* gui, void*);   // RET
std::uint32_t __fastcall recovered_client_dont_use_this_set_player_creature_005ee2e0(void* app, void*, std::uint32_t id);   // RET 4
// trigger slots / render core not reconstructed in t3
void* __fastcall recovered_client_trigger_scalar_destructor_00691ca0(void* self, void*, std::uint32_t flags);   // RET 4
std::uint32_t __fastcall recovered_client_trigger_ai_update_00692410(void* self, void*);                        // RET
std::uint32_t __fastcall recovered_client_trigger_get_reticle_point_00691b00(void* self, void*, std::uint32_t); // RET 4
void* __fastcall recovered_anim_base_trigger_destructor_006e6020(void* self, void*, std::uint32_t flags);       // RET 4
void* __fastcall recovered_anim_base_trigger_get_animation_name_006e6040(void* self, void*, void* out, std::uint32_t anim);   // RET 8
std::uint32_t __fastcall recovered_anim_base_trigger_server_to_client_animation_006e6100(void* self, void*, std::uint32_t anim);   // RET 4
void* __cdecl recovered_aur_part_trigger_create_0049e630(void* scene, void* points, std::uint32_t count, std::uint32_t flag,
                                                         void* zero, void* trigger);                   // cdecl
void __cdecl recovered_aur_part_trigger_set_highlight_params_0049d9b0(void* part, void* color, std::uint32_t alpha,
                                                                      std::uint32_t, std::uint32_t, std::uint32_t);   // cdecl
void __cdecl recovered_aur_part_trigger_set_highlight_0049e610(void* part, std::uint32_t on, float width, std::uint32_t);   // cdecl
}
