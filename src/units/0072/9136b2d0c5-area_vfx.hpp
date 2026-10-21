#pragma once
// Bounded areaVFX family00604C90; direct native x86 ABI. Canonical lower owners retained.
#include "8fdb3c6e33-w3_client_appearance.h"
extern "C" {
struct AreaVfxMethods;
extern const AreaVfxMethods area_vfx_methods_00754278;
void* __fastcall kotor_m15a_object_construct_0063e530(void*,void*,std::uint32_t);
void __fastcall kotor_m15e_object_destruct_0063e800(void*,void*);
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void*,void*,const kotor_w3::Vector3*,float);
void __fastcall kotor_m15e_anim_base_refresh_hilite_0069ec00(void*,void*);
void __fastcall area_vfx_base_set_position_0063f340(void*,void*,const kotor_w3::Vector3*);
std::uint32_t __fastcall area_vfx_base_hilite_0063cb00(void*,void*,const kotor_w3::Vector3*,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall area_vfx_base_unhilite_0063cb30(void*,void*,std::uint32_t);
std::uint32_t __fastcall area_vfx_anim_hilite_006a03e0(void*,void*,const kotor_w3::Vector3*,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall area_vfx_anim_unhilite_006a0440(void*,void*,std::uint32_t);
void* __fastcall area_vfx_construct_0068fe70(void*,void*,std::uint32_t);
std::uint32_t __fastcall area_vfx_load_00690010(void*,void*,std::uint32_t);
void __fastcall area_vfx_play_00690510(void*,void*,std::uint32_t);
void __fastcall area_vfx_announce_00690940(void*,void*);
void __fastcall area_vfx_destroy_0068ff70(void*,void*);
void* __fastcall area_vfx_scalar_destroy_00690960(void*,void*,std::uint32_t);
void __fastcall area_vfx_ai_update_00690980(void*,void*);
void __fastcall area_vfx_set_position_00690b00(void*,void*,const kotor_w3::Vector3*);
void __fastcall area_vfx_client_announce_005edf60(void*,void*,std::uint32_t,std::uint32_t,const kotor_w3::Vector3*);
void __fastcall area_vfx_internal_announce_005fb960(void*,void*,std::uint32_t,std::uint32_t,const kotor_w3::Vector3*);
std::uint32_t __fastcall area_vfx_register_existing_004d80e0(void*,void*,std::uint32_t*,void*,std::uint32_t);
void __fastcall recovered_world_time_difference_004ae460(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t*,std::uint32_t*);
// sol_vfx_lifecycle sole provider; row/out point to the same exact8-byte StartupString storage.
std::int32_t __fastcall object_vfx_table_00414260(void*,void*,void*,std::int32_t,std::int32_t*);
std::int32_t __fastcall object_vfx_table_00413ec0(void*,void*,void*,std::int32_t,void*);
std::int32_t __fastcall object_vfx_table_00414080(void*,void*,void*,std::int32_t,float*);
}
