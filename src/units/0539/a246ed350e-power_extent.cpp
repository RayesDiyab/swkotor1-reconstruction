#include "b6fde7b34f-power_extent.h"
namespace kotor_renderer_extent {
std::uint32_t __cdecl SignificantBit_004273e0(std::uint32_t value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value >>= 1;
    value -= (value >> 1) & 0x55555555u;
    value = (value & 0x33333333u) + ((value >> 2) & 0x33333333u);
    value = (value + (value >> 4)) & 0x0f0f0f0fu;
    value += value >> 8;
    value += value >> 16;
    return value & 0x3fu;
}
}
namespace kotor_soft_shadow {
void __cdecl PowerExtent_00427450(U32 width, U32 height, U32* out_width, U32* out_height) {
    const U32 width_count = kotor_renderer_extent::SignificantBit_004273e0(width) + 1u;
    *out_width = 1u << (width_count & 31u);
    const U32 height_count = kotor_renderer_extent::SignificantBit_004273e0(height) + 1u;
    *out_height = 1u << (height_count & 31u);
}
}
