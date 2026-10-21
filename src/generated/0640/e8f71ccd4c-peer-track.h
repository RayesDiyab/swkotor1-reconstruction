#pragma once
#include <cstdint>
namespace renderer_track {
using U32=std::uint32_t;
using Track=void (__stdcall*)(U32,U32,U32,U32);
using Bind=void (__stdcall*)(U32,U32);
// Borrowed words and extension slots. No duplicate original-global ownership.
struct Views { const volatile std::int32_t* arb_0078e5ec; const volatile U32 *matrix_0073f218,*matrix_0073f21c,*transform_0073f224;Track const volatile* track_007bb744;Bind const volatile* arb_bind_007bb788;Bind const volatile* nv_bind_007bb834; };
extern Views& bindings() noexcept;
}
extern "C" void __cdecl renderer_track_004a2400();
