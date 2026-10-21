#pragma once
#include "ed6f7a65b7-startup_string.h"
#include "00a0946121-resources.h"
#include "a64453a0cc-menu_actions.h"
#include "6dab7adcb5-menu_cohort.h"
#include "3f38d05b1d-menu_callbacks.h"
#include "56f214f303-menu_projection.h"
#include "98d2400bef-menu_rows.h"
#include "014ad7e09d-menu_active_property.h"
#include <cstdint>

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall frame_GetServerArea_00605640(void*, void*);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*, void*);
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void*, void*);
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void*, void*);
void* __fastcall kotor_fastcall_005ed580(void*, void*, std::uint32_t);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*, void*, std::uint32_t);
void* __fastcall kotor_m15closure_00646c90(void*, void*);
void __fastcall kotor_fastcall_0063d470(void*, void*);
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
void __fastcall kotor_fastcall_005edf30(void*, void*);
std::uint32_t __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_004ae910(void*, void*, std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_005eda50(void*, void*, std::uint32_t);
void __fastcall kotor_fastcall_004fddd0(void*, void*, std::int32_t);
void* __fastcall kotor_fastcall_00682e00(void*, void*);
std::int32_t __fastcall kotor_m15b_creature_stats_get_can_use_skill_006477e0(void*, void*, std::uint16_t);
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005edeb0(
    void*, void*, kotor_reconstruction::StartupString*, std::uint32_t) noexcept(false);
kotor_reconstruction::StartupString* __fastcall feedback_spell_name_0059b940(
    void*, void*, kotor_reconstruction::StartupString*);
menu_resources::Ref* __fastcall async_ref_assign_string_004061f0(
    menu_resources::Ref*, void*, menu_resources::Ref*, kotor_reconstruction::StartupString*);
int __fastcall c2da_equal_text_004060d0(menu_resources::Ref*, void*, const char*);
menu_resources::Ref* __fastcall c2da_copy_00406120(
    menu_resources::Ref*, void*, menu_resources::Ref*, menu_resources::Ref*);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*, void*, std::int32_t);
void __fastcall w1x_pointer_list_insert_00616940(void*, void*, std::uint32_t, std::uint32_t);
void __fastcall kotor_m15_array_i16_insert_006169a0(void*, void*, std::uint32_t, std::int32_t);
void __fastcall kotor_fastcall_00616800(void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_006826a0(void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_00683e90(void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_00691900(void*, void*, std::uint32_t, void*);
void __fastcall kotor_fastcall_00691950(void*, void*, std::uint32_t, void*);
}
