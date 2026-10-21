#pragma once
#include "e846d283df-lower_ports.h"
#include "6b2fc6096c-streaming_state.h"
namespace sound_lower29 {
std::uint32_t __fastcall ActorPlaying005db0e0(void*,void*);
}
namespace table29_required { void __fastcall gui_resume_005d5b30(void*,void*); }
namespace sound_lower29_required {
// Exact observed stack carriers; final volume DWORD is an IEEE float bit pattern.
void __fastcall volume_005dc930(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t);
// ECX=source system, stack=[actor,voice], RET8. No return is consumed.
void __fastcall start3d_005d71c0(void*,void*,void*,void*);
void __fastcall start2d_005d7480(void*,void*,void*,void*);
}
extern "C" __declspec(dllimport) void __stdcall AIL_set_stream_position(void*,std::int32_t);
extern "C" __declspec(dllimport) void __stdcall AIL_pause_stream(void*,std::int32_t);
extern "C" __declspec(dllimport) std::uint32_t __stdcall AIL_3D_sample_status(void*);
extern "C" __declspec(dllimport) std::uint32_t __stdcall AIL_sample_status(void*);
extern "C" __declspec(dllimport) std::uint32_t __stdcall AIL_3D_sample_offset(void*);
extern "C" __declspec(dllimport) std::uint32_t __stdcall AIL_sample_position(void*);
extern "C" __declspec(dllimport) void __stdcall AIL_stop_3D_sample(void*);
extern "C" __declspec(dllimport) void __stdcall AIL_stop_sample(void*);
