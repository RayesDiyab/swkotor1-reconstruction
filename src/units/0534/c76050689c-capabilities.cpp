#include "958b3f24f7-capabilities.h"
// Use the existing Task15 declaration/namespace for the shared lower body.
#include "ea46a70e8e-enable_bindings.h"

namespace kotor_soft_shadow {
std::uint32_t __cdecl SoftAvailable_0045f7b0() {
    const auto& b = kotor_renderer_capabilities::bindings;
    std::uint32_t value = *b.soft_cache_0078e520;
    if (value == 0xffffffffu) {
        const std::uint32_t e4 = *b.mask_0078e4e4;
        const std::uint32_t dc = *b.mask_0078e4dc;
        const std::uint32_t required = dc | e4;
        value = ((*b.capabilities_007bb85c & required) == required) ? 1u : 0u;
        *b.soft_cache_0078e520 = value;
    }
    return value;
}
std::uint32_t __cdecl ATISoftAvailable_0045f7e0() {
    const auto& b = kotor_renderer_capabilities::bindings;
    if (*b.ati_gate_007bb860 == 0) return 0;
    std::uint32_t value = *b.ati_soft_cache_0078e524;
    if (value == 0xffffffffu) {
        const std::uint32_t d0 = *b.mask_0078e4d0;
        const std::uint32_t c8 = *b.mask_0078e4c8;
        const std::uint32_t required = c8 | d0;
        value = ((*b.capabilities_007bb85c & required) == required) ? 1u : 0u;
        *b.ati_soft_cache_0078e524 = value;
    }
    return value;
}
}
namespace kotor_renderer_init {
std::uint32_t __cdecl FrameBufferAvailable_0045f820() {
    const auto& b = kotor_renderer_capabilities::bindings;
    std::uint32_t value = *b.frame_cache_0078e528;
    if (value == 0xffffffffu) {
        const std::uint32_t dc = *b.mask_0078e4dc;
        const std::uint32_t d4 = *b.mask_0078e4d4;
        const std::uint32_t required = (d4 | dc) | *b.mask_0078e4c8;
        value = ((*b.capabilities_007bb85c & required) == required) ? 1u : 0u;
        *b.frame_cache_0078e528 = value;
    }
    return value;
}
std::uint32_t __cdecl ATIFrameBufferAvailable_0045f860() {
    const auto& b = kotor_renderer_capabilities::bindings;
    std::uint32_t value = *b.ati_frame_cache_0078e52c;
    if (value == 0xffffffffu) {
        const std::uint32_t d0 = *b.mask_0078e4d0;
        const std::uint32_t d4 = *b.mask_0078e4d4;
        const std::uint32_t c8 = *b.mask_0078e4c8;
        const std::uint32_t required = (d4 | d0) | c8;
        value = ((*b.capabilities_007bb85c & required) == required) ? 1u : 0u;
        *b.ati_frame_cache_0078e52c = value;
        // Original always validates after the cold cache write, including value=0.
        // Success returns the saved value even if validation changes the cache.
        if (kotor_vertex_program::ValidateNonRadeon_0045f5e0() == 0) {
            *b.ati_frame_cache_0078e52c = 0;
            return 0;
        }
    }
    return value;
}
}
