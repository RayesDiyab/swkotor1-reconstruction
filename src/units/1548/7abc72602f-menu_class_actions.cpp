// 0064AF10: forward each class list, preserving the mutable BYTE loop bound.
#include "56f214f303-menu_projection.h"
#include "8e5b891251-allocator_live.h"
#include <cstring>

namespace {
template<class T> T load(const void* base, std::uint32_t offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
// The original constructs an unused empty DWORD list with unwind cleanup and
// calls free(0) on normal completion as well. Do not elide its real allocator call.
struct EmptyList {
    ~EmptyList() { live_free(nullptr); }
};
}

extern "C" void __fastcall kotor_fastcall_0064af10(
    void* self, void*, void* output, std::uint32_t kind,
    std::int32_t hit_points, std::uint32_t target_filter,
    std::uint32_t equipment_mask, float range) {
    EmptyList cleanup;
    std::uint8_t count = load<std::uint8_t>(self, 0x32);
    if (count == 0) return;
    std::uint8_t index = 0;
    do {
        const std::int32_t alignment = load<std::int16_t>(self, 0x80);
        kotor_fastcall_0064a870(static_cast<unsigned char*>(self) + 0xD4u +
            static_cast<std::uint32_t>(index) * 0x20u, nullptr, output,
            kind, hit_points, alignment, target_filter, equipment_mask, range);
        count = load<std::uint8_t>(self, 0x32);
        index = static_cast<std::uint8_t>(index + 1u);
    } while (index < count);
}
