#pragma once
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 source/physical ABI only
#endif
// Incoming objects and handles belong to their real producer. These are byte
// offset contracts, not new class definitions or an admission of nil receivers.
// Outer+0 is a nullable internal pointer. A nonnull internal has a mandatory
// source pointer at+0x20, source+8 readiness, stream+0 and raw loop word+4.
namespace sound_streaming29 {
std::uint32_t __fastcall InternalIsPlaying005dc300(void* internal,void* unused_edx);
}
namespace table29_required {
std::uint32_t __fastcall gui_playing_005d5af0(void* outer,void* unused_edx);
void __fastcall gui_loop_005d5b40(void* outer,void* unused_edx,std::uint32_t loop);
}
// Exact PE imports:0073D568/_AIL_stream_status@4 and
//0073D56C/_AIL_set_stream_loop_count@8 in mss32.dll. No runtime fallback.
extern "C" __declspec(dllimport) std::uint32_t __stdcall AIL_stream_status(void* stream);
extern "C" __declspec(dllimport) void __stdcall AIL_set_stream_loop_count(void* stream,std::int32_t count);
static_assert(sizeof(void*)==4 && sizeof(std::uint32_t)==4,"observed x86 words");
