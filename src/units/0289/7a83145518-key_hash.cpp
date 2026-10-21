#include "abaec19899-hash_text.h"
#include <cstring>

extern "C" uint32_t __fastcall key_table_hash_0040d500(
    void* table, void*, menu_resources::Ref* resource, uint32_t type) {
    uint32_t value = 0;
    for (unsigned index = 0; index < 16; ++index) {
        const unsigned char byte = resource->bytes[index];
        if (!byte) break;
        const auto character = static_cast<int32_t>(byte);
        const uint32_t lowered = static_cast<uint32_t>(kotor_lower_006fb766(character));
#ifdef WRONG_HASH_OMIT_TYPE
        (void)type;
        const uint32_t mixed = lowered;
#else
        const uint32_t mixed = lowered + type;
#endif
#ifdef WRONG_HASH_XOR_TYPE
        (void)mixed;
        const uint32_t slot = (lowered ^ type ^ value) & 255u;
#else
        const uint32_t slot = (mixed ^ value) & 255u;
#endif
        value = (value >> 8) ^ kotor_key_crc_table_007a3d78[slot];
    }
    uint32_t capacity;
    std::memcpy(&capacity, static_cast<unsigned char*>(table) + 12, 4);
    // Original DIV requires a nonzero live capacity, including an empty Ref.
    return value % capacity;
}
