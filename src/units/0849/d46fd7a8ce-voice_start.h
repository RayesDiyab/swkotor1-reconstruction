#pragma once
#include "a5814b3d07-sound_lower.h"
// Opaque physical transport: ECX actor; three stack DWORDs; RET12.
// This is an unresolved real lower implementation, not a replacement body.
namespace sound_voice_required {
void __fastcall actor_volume_005db800(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t);
}
extern "C" __declspec(dllimport) void __stdcall AIL_init_sample(void*);
extern "C" __declspec(dllimport) std::int32_t __stdcall AIL_set_sample_file(void*,void*,std::int32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_sample_loop_count(void*,std::int32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_sample_position(void*,std::uint32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_set_sample_playback_rate(void*,std::int32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_resume_sample(void*);
