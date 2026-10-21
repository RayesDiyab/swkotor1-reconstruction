#include "6fe11fddcb-pbuffer.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This implementation requires the inspected MSVC x86 ABI.
#endif

namespace kotor_pbuffer {
void* __cdecl CreatePBuffer_00426560(U32 width, U32 height) {
    // 00426564: retain this DC even if the chooser changes the canonical cell.
    const U32 dc_bits = *bindings.main_dc_007a47e4;
    void* const dc = reinterpret_cast<void*>(static_cast<std::uintptr_t>(dc_bits));
    const I32 pixel_attributes[17] = {
        0x2010, 1, 0x202d, 1, 0x2071, 1,
        0x2015, 8, 0x2017, 8, 0x2019, 8, 0x201b, 8,
        0x2011, 0, 0
    };
    const float float_attributes[1] = {0.0f};
    // D2 (documented deviation): 004265FB passes &format; if the chooser does not write
    // it, 0042660E reads a stack local with no initializing store. Value-initialized to 0.
    I32 pixel_format = 0;
    U32 count;
    // 004265fb: neither success nor count is inspected by the original.
    (*bindings.choose_007bb730)(dc, pixel_attributes, float_attributes, 1, &pixel_format, &count);
    const I32 pbuffer_attributes[6] = {0x2072, 0x2076, 0x2073, 0x207a, 0, 0};
    const I32 format_bits = pixel_format;  // 0042660E mov edx,[esp+10h]
    return (*bindings.create_007bb530)(dc, format_bits,
        static_cast<I32>(width), static_cast<I32>(height), pbuffer_attributes);
}
}
