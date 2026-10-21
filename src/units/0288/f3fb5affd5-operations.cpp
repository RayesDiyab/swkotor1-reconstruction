#include "13c249825e-operations.h"

namespace kotor_reconstruction {

int CResRef::operator==(const CResRef* rhs) const noexcept {







    for (unsigned i = 0; i < 16; ++i)
        if (bytes[i] != rhs->bytes[i]) return 0;

    return 1;
}

int CResRef::operator!=(const CResRef* rhs) const noexcept {






    for (unsigned i = 0; i < 16; ++i)
        if (bytes[i] != rhs->bytes[i]) return 1;

    return 0;
}

void CResRef::CopyToString(char* out) const noexcept {
    // Each four-byte load completes before its corresponding store. Byte
    // accesses avoid unaligned/type-punned C++ loads; volatile prevents the
    // optimizer from replacing the observed overlapping copy order.
    const volatile std::uint8_t* source = bytes;
    volatile unsigned char* destination = reinterpret_cast<unsigned char*>(out);





    for (unsigned offset = 0; offset < 16; offset += 4) {
        const std::uint32_t word = static_cast<std::uint32_t>(source[offset]) |
            (static_cast<std::uint32_t>(source[offset + 1]) << 8) |
            (static_cast<std::uint32_t>(source[offset + 2]) << 16) |
            (static_cast<std::uint32_t>(source[offset + 3]) << 24);
        destination[offset] = static_cast<unsigned char>(word);
        destination[offset + 1] = static_cast<unsigned char>(word >> 8);
        destination[offset + 2] = static_cast<unsigned char>(word >> 16);
        destination[offset + 3] = static_cast<unsigned char>(word >> 24);
    }

    destination[16] = 0;
}
}
