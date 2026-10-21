#pragma once
#include "95e95d0aa0-w3_item.h"
// lane-w3 private declarations for the player-model unit (w3_client_player_model.cpp) and its traps
// (w3_traps_player_model.cpp): the CSWCCreatureAppearance model-request chain reached from
// CSWCCreature::RequestAppearance 0060C910, the CAuroraInterface request leaves it ends in, and the CSWCItem
// constructor / tables reached from CSWCMessage::CreateItem 0064B540. Conventions as w3_client.h.
#include "f147099204-w3_client_appearance.h"
#include "203861aa21-w3_client_door_placeable.h"

// ------------------------------------------------------------------ existing namespace-scope owners (1115 map)
namespace kotor_menu20 { struct Debug; }
namespace base_ini_reconstruction {   // 0107-ini-diagnostic-join.obj: CExoDebug::Warning(line, file, text) RET 0Ch
void __fastcall warning_005e9400(kotor_menu20::Debug* debug, void* unused_edx, int line, const char* file,
                                 const char* text);
}
namespace menu_continuation { struct ExoBase; extern ExoBase* exo_base_007a39e0; }   // [007A39E0] CExoBase*
namespace kotor_list { struct ModelList; }
namespace kotor_requested_models { extern kotor_list::ModelList list_00834094; }    // [00834094] requested models

extern "C" {
// ------------------------------------------------------------------ existing runtime1115 providers
std::uint8_t __fastcall kotor_m15a_appearance_get_anim_type_00696d40(void* self, void*, void* model_type) noexcept(false);   // 00696D40 RET 4 (AL)
void __fastcall kotor_m15a_appearance_get_btype_model_and_texture_00697610(void* self, void*, void* data, void* model_out,
                                                                            void* texture_out) noexcept(false);   // 00697610 RET 0Ch
void* __fastcall kotor_m15e_anim_base_tw_construct_0069cbd0(void* self, void*) noexcept(false);                 // 0069CBD0 RET 0
void* __fastcall kotor_m15e_anim_base_head_construct_0069bb80(void* self, void*, std::uint32_t most_derived) noexcept(false);   // 0069BB80 RET 4
void* __fastcall kotor_m15e_anim_base_wield_construct_00699dd0(void* self, void*, std::uint32_t most_derived) noexcept(false);  // 00699DD0 RET 4
void* __fastcall kotor_m15e_anim_base_head_wield_construct_00698ec0(void* self, void*, std::uint32_t most_derived) noexcept(false);   // 00698EC0 RET 4
std::uint32_t __fastcall kotor_m15e_sw_anim_base_set_00698e30(void* self, void*, std::uint32_t index, std::uint32_t value) noexcept(false);   // 00698E30 RET 8
void* __fastcall resman_string_from_int_005e5bc0(void* self, void*, int value);                    // 005E5BC0 CExoString(int) RET 4
int __cdecl kotor_stricmp_0070acaf(const char* a, const char* b);                                   // 0070ACAF _stricmp
void* __cdecl aurora_binary_search_model_00464090(const char* name, int low, int high);             // 00464090 BinarySearchModel
void* __cdecl model_0070e080(void* name, int mode, void* comparison_out, void* position_out);       // 0070E080 GetModelInternal
extern unsigned char historical_storage_007a8a64[12];                     // [007A8A64] model cache {data, count, capacity}
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void*);                              // 0063E800 ~CSWCObject
void __fastcall recovered_item_base_destroy_005b4680(void* self, void*);                             // 005B4680 ~CSWItem
std::uint32_t __fastcall kotor_m15e_trap_0063e730(void*, void*, std::uint32_t);                      // 0063E730 GetName (1115 D83 trap)
std::uint32_t __fastcall kotor_m15e_object_unload_model_00646650(void*, void*, std::uint32_t);       // 00646650 UnloadModel

// ------------------------------------------------------------------ lane-w3 player-model unit
// method tables (1 / 82 / 1 slots; original .rdata 00751BA0 / 00751A58 / 0074B2A0)
extern const void* const recovered_client_item_methods_00751ba0[1];
extern const void* const recovered_client_item_object_methods_00751a58[82];
extern const void* const recovered_sw_item_methods_0074b2a0[1];
// CSWCCreatureAppearance (this = creature +21Ch; [this+3Ch] = owning CSWCCreature)
std::uint32_t __fastcall recovered_client_appearance_request_btype_00697c80(void* appearance, void*, const void* request) noexcept(false);
std::uint32_t __fastcall recovered_client_appearance_request_simple_00696e10(void* appearance, void*, const void* request) noexcept(false);
// CSWCCreature::RequestModel (client creature table 0074F5F8 slot +158h)
std::uint32_t __fastcall recovered_client_creature_request_model_0060c1e0(void* self, void*, const void* model,
                                                                         const void* texture, std::uint32_t anim_type) noexcept(false);
// CAuroraInterface statics / model cache / CExoLinkedListInternal
std::uint32_t __cdecl recovered_aurora_is_resident_0070d980(const char* name) noexcept(false);
void __cdecl recovered_aurora_request_model_0070d8d0(const char* name) noexcept(false);
void* __cdecl recovered_aurora_binary_find_model_004640f0(const char* name) noexcept(false);
void* __fastcall recovered_linked_list_add_before_005e9ad0(void* list, void*, void* data, void* position) noexcept(false);
// CSWItem / CSWCItem
void* __fastcall recovered_sw_item_construct_005b4660(void* self, void*) noexcept;
void* __fastcall recovered_client_item_as_sw_item_00646380(void* object, void*) noexcept;
std::uint32_t __fastcall recovered_client_item_load_model_00646500(void* object, void*, void* resref, std::uint32_t part,
                                                                   std::uint32_t anim_type) noexcept(false);
void __fastcall recovered_client_item_set_area_00646930(void* object, void*, void* area) noexcept(false);
void __fastcall recovered_client_item_show_visual_effect_00646830(void* item, void*) noexcept(false);
void __fastcall recovered_client_item_destruct_00646cd0(void* item, void*) noexcept(false);
void* __fastcall recovered_client_item_scalar_destructor_006474b0(void* item, void*, std::uint32_t flags) noexcept(false);
void* __fastcall recovered_client_item_object_destructor_thunk_00646cc0(void* object, void*, std::uint32_t flags) noexcept(false);

// ------------------------------------------------------------------ explicit traps (w3_traps_player_model.cpp)
}
