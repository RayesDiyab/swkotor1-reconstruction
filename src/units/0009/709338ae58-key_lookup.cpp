#include "8d84dea37c-key_lookup.h"
extern "C" __declspec(noinline) uint32_t __fastcall unresolved_key_entry_00407230(
    menu_resources::Manager* receiver, void*, menu_resources::Ref* resource,
    uint32_t type, uint32_t* output1, uint32_t* output2) {
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(receiver);
#ifdef WRONG_SEARCH_ORDER
    const unsigned offsets[] = {0x18, 0x14, 0x1c, 0x18, 0x10};
#else
    const unsigned offsets[] = {0x14, 0x18, 0x1c, 0x18, 0x10};
#endif
#ifndef WRONG_KIND
    const uint32_t kinds[] = {0, 1, 0, 2, 0};
#endif
    for (unsigned i = 0; i != 5; ++i) {
#ifdef WRONG_KIND
        const uint32_t kind = 0;
#else
        const uint32_t kind = kinds[i];
#endif
#ifdef WRONG_TYPE_TRUNCATION
        const uint32_t forwarded_type = static_cast<uint16_t>(type);
#else
        const uint32_t forwarded_type = type;
#endif
#ifdef WRONG_OUTPUT_IDENTITY
        const uint32_t status = unresolved_key_entry_from_table_004071a0(
            receiver, nullptr, bytes + offsets[i], resource, forwarded_type,
            output2, output1, kind);
#else
        const uint32_t status = unresolved_key_entry_from_table_004071a0(
            receiver, nullptr, bytes + offsets[i], resource, forwarded_type,
            output1, output2, kind);
#endif
        if (status) {
#ifdef WRONG_STATUS_DOMAIN
            return status;
#else
            return 1;
#endif
        }
    }
#ifndef WRONG_FAILURE_CLEAR
    *output1 = 0;
    *output2 = 0;
#endif
    return 0;
}
