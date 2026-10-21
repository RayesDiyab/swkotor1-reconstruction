#include "abaec19899-hash_text.h"

extern "C" int __fastcall c2da_equal_text_004060d0(
    menu_resources::Ref* resource, void*, const char* text) {
    for (unsigned index = 0; index < 16; ++index) {
#ifdef WRONG_TEXT_UNSIGNED
        const auto character = static_cast<int32_t>(static_cast<unsigned char>(text[index]));
#else
        const auto character = static_cast<int32_t>(static_cast<signed char>(text[index]));
#endif
        const int32_t left = kotor_lower_006fb766(character);
        // Load the Ref byte after the text-lowering call, as the original does.
        const int32_t right = kotor_lower_006fb766(resource->bytes[index]);
#ifdef WRONG_TEXT_LOW_BYTE
        if (static_cast<unsigned char>(left) != static_cast<unsigned char>(right)) return 0;
#else
        if (left != right) return 0;
#endif
#ifdef WRONG_TEXT_RAW_ZERO
        if (!text[index]) return 1;
#else
        if (!left) return 1;
#endif
    }
#ifdef WRONG_TEXT_FULL16_FAIL
    return 0;
#else
    return 1;
#endif
}
