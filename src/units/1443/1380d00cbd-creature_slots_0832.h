#pragma once
#include <cstdint>
extern "C" {
void __fastcall creature_reset_update_times_0060b900(void*,void*,std::uint32_t,std::uint32_t);
void __fastcall creature_unload_model_0060c8e0(void*,void*);
std::uint32_t __fastcall creature_hilite_0060e770(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall creature_unhilite_0060e7a0(void*,void*,std::uint32_t);
void* __fastcall creature_portrait_ref_00617030(void*,void*,void*,std::uint32_t);
std::uint16_t __fastcall creature_portrait_id_00617070(void*,void*);
std::uint32_t __fastcall creature_bark_string_0060f510(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall creature_sync_orientation_00611ec0(void*,void*,const float*);
void __fastcall creature_sync_animation_0060c100(void*,void*,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall creature_is_hostile_006170c0(void*,void*);
void* __fastcall creature_current_waypoint_0060c190(void*,void*);
void* __fastcall creature_end_waypoint_0060c1a0(void*,void*);
void* __fastcall creature_start_waypoint_0060c1c0(void*,void*);
std::uint32_t __fastcall creature_next_waypoint_0060c4a0(void*,void*);
void* __fastcall object_scalar_destroy_0063f5c0(void*,void*,std::uint32_t);
}
