#pragma once
#include "b99619bee4-sound_lower.h"
#include "f43a5a7d3b-startup_views.h"
namespace sound_voice_required {
void __fastcall actor_volume_005db800(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall spatial_query_005d6ef0(void*,void*,void*,void*,std::uint32_t*,std::uint32_t*);
// Two consecutive 12-byte values, RET24, ST0 float result. Six raw DWORD
// carriers avoid assigning a recovered class identity to those values.
float __stdcall spatial_value_0050ccf0(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
void __fastcall actor_obstruction_005daeb0(void*,void*);
}
extern "C" __declspec(dllimport) std::int32_t __stdcall AIL_set_3D_sample_file(void*,void*);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_loop_count(void*,std::int32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_distances(void*,std::uint32_t,std::uint32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_offset(void*,std::uint32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_position(void*,std::uint32_t,std::uint32_t,std::uint32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_playback_rate(void*,std::int32_t);
extern "C" __declspec(dllimport) std::int32_t __stdcall AIL_3D_room_type(void*);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_effects_level(void*,std::uint32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_resume_3D_sample(void*);
