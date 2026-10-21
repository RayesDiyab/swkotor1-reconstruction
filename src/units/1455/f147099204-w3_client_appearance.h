#pragma once
// lane-w3 private declarations for the creature appearance unit (w3_client_appearance.cpp) and its traps
// (w3_traps_appearance.cpp).
#include "625a7a9ed5-w3_client_objects.h"

// ------------------------------------------------------------------ existing namespace-scope / member owners
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void* unused_edx, const void* resref);   // 005D60E0 CExoSoundSource(CResRef) RET 4
void __fastcall SourceSetPriorityGroup005d5900(void* self, void* unused_edx, unsigned group);   // 005D5900 SetPriorityGroup RET 4
}
namespace menu_resources {
// 00408BC0 CExoResMan::Exists(CResRef const&, WORD type, DWORD* out) (0031-service-resource-14.obj member owner).
struct Manager { unsigned __thiscall exists_00408bc0(Ref* ref, unsigned type, unsigned* out); };
}

extern "C" {
// ------------------------------------------------------------------ existing runtime1115 providers
void __fastcall kotor_m15a_creature_set_name_0060d600(void* self, void*, const void* first, const void* last);   // 0060D600 RET 8
void __fastcall kotor_m15a_creature_set_sound_set_0060b7c0(void* self, void*, std::uint32_t sound_set);         // 0060B7C0 RET 4
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* array, void*, std::uint32_t id);       // 005B31A0 RET 4
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                                     // 005B4790
void __fastcall kotor_m15a_item_set_equipped_by_00646600(void* item, void*, void* creature);                   // 00646600 RET 4
std::uint32_t __fastcall kotor_m15a_item_power_item_00647130(void* item, void*, std::uint32_t, std::uint32_t,
                                                             std::uint32_t, std::uint32_t);                    // 00647130 RET 10h
void __fastcall kotor_m15a_creature_update_weapon_class_00613710(void* self, void*);                           // 00613710
void __cdecl kotor_m15a_switch_weapon_event_00610f40(std::uint32_t, const char*, void* creature);              // 00610F40 cdecl
void __fastcall kotor_m15a_creature_update_shadow_blob_0060e5b0(void* self, void*);                            // 0060E5B0
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void* self, void*, std::uint32_t anim);   // 0063C300 RET 4
void __fastcall kotor_m15a_object_set_looping_animation_0063d830(void* self, void*, std::uint32_t anim);        // 0063D830 RET 4
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void* self, void*);                  // 0063C100 (AX)
std::uint32_t __fastcall kotor_m15a_creature_set_appearance_006134c0(void* self, void*, void* appearance,
                                                                     std::uint32_t mask, std::uint32_t flag);  // 006134C0 RET 0Ch
std::uint32_t __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* self, void*, std::uint32_t slot);   // 0060CA80 RET 4
std::uint32_t __fastcall kotor_m15a_creature_get_last_instant_0060ced0(void* self, void*, std::uint32_t slot);      // 0060CED0 RET 4
std::uint32_t __fastcall kotor_m15_client_get_area_not_ready_005edad0(void* app, void*);                       // 005EDAD0
std::uint32_t __fastcall kotor_fastcall_005edc10(void* app, void*);                                            // 005EDC10 GetPausedByCombat
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);                                    // 005D5A20 CExoSoundSource::Stop
void __fastcall kotor_m15_sound_source_set_looping_005d59d0(void* source, void*, std::uint32_t looping);       // 005D59D0 RET 4
void* __fastcall async_ref_construct_string_00406d60(void* self, void*, const void* text);                    // 00406D60 CResRef(CExoString&) RET 4
void* __fastcall gob_get_property_0x68_0063c070(void* self, void*);                                           // 0063C070 [this+68h]

// ------------------------------------------------------------------ lane-w3 (appearance unit)
std::uint32_t __fastcall recovered_client_internal_get_pause_state_005ee8f0(void* internal, void*, std::uint32_t mask) noexcept;
std::uint32_t __fastcall recovered_client_get_pause_state_005ed640(void* app, void*, std::uint32_t mask) noexcept;
std::uint32_t __fastcall recovered_client_creature_get_armour_class_0060d540(void* self, void*) noexcept(false);
std::uint32_t __fastcall recovered_client_creature_request_appearance_0060c910(void* self, void*, const void* appearance,
                                                                               std::uint32_t mask) noexcept(false);
void __fastcall recovered_client_creature_delete_looping_sound_0060d510(void* self, void*) noexcept(false);
void __fastcall recovered_client_creature_set_new_looping_sound_0060d460(void* self, void*, const void* name) noexcept(false);
void __fastcall recovered_client_creature_set_last_instant_0060cd50(void* self, void*, std::uint32_t slot,
                                                                    std::uint32_t value) noexcept;
void __fastcall recovered_client_creature_set_equipped_item_id_0060cbd0(void* self, void*, std::uint32_t slot,
                                                                        std::uint32_t id) noexcept;
void __fastcall recovered_client_creature_set_item_unequipped_by_me_0060d020(void* self, void*, std::uint32_t id) noexcept(false);
// 00610A70 PlayDrawAnimation: lane-w4 owner (creature/creature_ai.cpp), coordinator ruling t2-5.
void __fastcall kotor_fastcall_00610a70(void* self, void*, std::uint32_t anim);
void __fastcall recovered_client_creature_update_visible_weapons_00613850(void* self, void*, std::uint32_t slot, void* item,
                                                                         std::uint32_t instant) noexcept(false);
void __fastcall recovered_client_creature_equip_to_inventory_slot_00613ba0(void* self, void*, std::uint32_t slot,
                                                                           void* item) noexcept(false);
void __fastcall recovered_client_item_set_num_active_properties_00646660(void* item, void*, std::uint32_t count) noexcept(false);
void* __fastcall recovered_client_item_get_active_property_006466a0(void* item, void*, std::int32_t index) noexcept;
void* __fastcall recovered_client_item_set_stack_size_00646810(void* item, void*, std::uint32_t size) noexcept(false);
void __fastcall recovered_client_item_set_property_uses_left_006469b0(void* item, void*, std::int32_t index,
                                                                      std::uint32_t uses) noexcept;
void* __fastcall recovered_base_item_get_icon_resref_005b3050(void* base, void*, void* out, std::uint32_t variation,
                                                              std::uint32_t gender) noexcept(false);
void* __fastcall recovered_base_item_get_model_resref_005b30e0(void* base, void*, void* out, std::uint32_t variation,
                                                               std::uint32_t gender) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_item_appearance_0064e730(void* self, void*, std::uint32_t* base_item,
                                                                          std::uint8_t* variation, std::uint8_t* extra_a,
                                                                          std::uint8_t* extra_b) noexcept(false);
std::uint32_t __fastcall recovered_client_handle_active_item_properties_0064e3d0(void* self, void*, std::uint32_t id) noexcept(false);
void* __fastcall recovered_client_create_item_0064b540(void* self, void*, std::uint32_t base_item, std::uint32_t variation,
                                                       std::uint32_t gender) noexcept(false);

// ------------------------------------------------------------------ explicit traps (w3_traps_appearance.cpp)
void* __fastcall recovered_client_item_construct_00646230(void* self, void*, std::uint32_t base_item);
std::uint32_t __fastcall recovered_client_appearance_request_body_00698a40(void* appearance, void*, const void* request);
std::uint32_t __fastcall recovered_client_appearance_request_head_00696f60(void* appearance, void*, const void* request);
// 0063C760 InsertFireForgetAnimation: no lane-w3 caller left (reached only through lane-w4 kotor_fastcall_00610a70).
}
