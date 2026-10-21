#include "077047d631-gff_hash_init.h"
extern "C" __declspec(noinline) void __stdcall unresolved_gff_hash_init_00410520() {
#ifndef WRONG_NO_INITIALIZATION
#ifdef WRONG_TABLE_EXTENT
    const unsigned limit = 255;
#else
    const unsigned limit = 256;
#endif
    for (unsigned index = 0; index != limit; ++index) {
        uint32_t value = index;
        for (unsigned bit = 0; bit != 8; ++bit) {
            const uint32_t low = value & 1u;
#ifdef WRONG_SIGNED_SHIFT
            value = static_cast<uint32_t>(static_cast<int32_t>(value) >> 1);
#else
            value >>= 1;
#endif
#ifdef WRONG_POLYNOMIAL
            if (low) value ^= 0x04c11db7u;
#else
            if (low) value ^= 0xedb88320u;
#endif
        }
        kotor_gff_crc_table_007a4178[index] = value;
    }
#endif
}
