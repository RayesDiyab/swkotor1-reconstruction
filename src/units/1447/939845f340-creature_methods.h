#pragma once
#include <cstdint>
#include "cc436f6baa-m15e_animbase.h"
struct W4Vector;
namespace c2da_current {struct Table;}
extern "C" {
void __fastcall recovered_creature_sync_position_00611f50(void*, void*, const float*, std::uint32_t, float);
std::uint32_t __fastcall recovered_creature_lip_sync_00616310(void*, void*, std::uint32_t);
double __fastcall recovered_creature_compute_movement_scale_0060bf60(void*, void*, float);
std::uint32_t __fastcall recovered_object_animation_walk_0063c240(void*, void*, std::uint32_t);
std::uint32_t __fastcall recovered_object_animation_run_0063c2a0(void*, void*, std::uint32_t);
double __fastcall recovered_client_appearance_get_run_looks_awful_rate_006975d0(void*, void*);
void __fastcall kotor_fastcall_0060bdf0(void*, void*, W4Vector*);
void __fastcall kotor_fastcall_00611850(void*, void*, W4Vector*);
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void*, void*);
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void*, void*, std::uint32_t);
void* __fastcall gob_get_property_0x68_0063c070(void*, void*);
float __fastcall kotor_fastcall_0063c0e0(void*, void*, std::uint32_t);
std::int32_t __fastcall kotor_m15b_c2da_get_float_entry_00413350(void*, void*, std::int32_t, std::int32_t, float*);
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table*, void*, std::int32_t, std::int32_t, std::int32_t*);
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void*, void*, const void*, float);
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void*, void*, const void*);
extern void* kotor_cell_rules_007a3a28;
extern std::uint32_t kotor_rules_cell32_007a2120, kotor_rules_cell32_007a212c;
extern std::uint32_t kotor_rules_cell32_007a2160, kotor_rules_cell32_007a2164;
}
