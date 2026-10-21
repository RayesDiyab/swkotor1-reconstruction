#pragma once
#include <cstdint>

namespace kotor_renderer_capabilities {
using U32 = std::uint32_t;
// Borrowed addresses only. Main publishes the existing capability/cache objects.
// The four caches retain their original full-width values and -1 cold sentinel.
struct Bindings {
    volatile U32* soft_cache_0078e520;
    volatile U32* ati_soft_cache_0078e524;
    volatile U32* frame_cache_0078e528;
    volatile U32* ati_frame_cache_0078e52c;
    const volatile U32* mask_0078e4c8;
    const volatile U32* mask_0078e4d0;
    const volatile U32* mask_0078e4d4;
    const volatile U32* mask_0078e4dc;
    const volatile U32* mask_0078e4e4;
    const volatile U32* capabilities_007bb85c;
    const volatile U32* ati_gate_007bb860;
};
extern const Bindings& bindings;
}
namespace kotor_soft_shadow {
std::uint32_t __cdecl SoftAvailable_0045f7b0();
std::uint32_t __cdecl ATISoftAvailable_0045f7e0();
}
namespace kotor_renderer_init {
std::uint32_t __cdecl FrameBufferAvailable_0045f820();
std::uint32_t __cdecl ATIFrameBufferAvailable_0045f860();
}
