// Five bounded menu leaves, reconstructed from the original GOG K1 x86 image.
#include "56f214f303-menu_projection.h"
#include "63176f6f15-startup_string.h"
#include "67a5d47ab7-m15b_stats.h"
#include <cstring>

extern "C" {
void* __fastcall recovered_equipment_item_005a4c20(void*, void*, std::uint32_t);
extern void* kotor_cell_rules_007a3a28;
}

namespace {
using U = std::uint32_t;
using I = std::int32_t;
using String = kotor_reconstruction::StartupString;
template<class T> T load(const void* base, U offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const void*>(reinterpret_cast<U>(base) + offset), sizeof value);
    return value;
}
}

extern "C" {
void __fastcall kotor_fastcall_0063d300(void* self, void*, String* output, U* tag) {
    output->Assign005e5c50(reinterpret_cast<String*>(reinterpret_cast<U>(self) + 0xE8u));
    *tag = load<U>(self, 0xF0);
}

U __fastcall kotor_fastcall_00685840(void* self, void*) {
    if ((load<std::uint8_t>(self, 0x208) & 1u) != 0) return 1;
    if ((load<std::uint8_t>(self, 0x3CC) & 1u) != 0) return 1;
    if ((load<std::uint8_t>(self, 0x590) & 1u) != 0) return 1;
    return (load<std::uint8_t>(self, 0x44) & 1u) != 0 ? 1u : 0u;
}

I __fastcall kotor_fastcall_006467e0(void* self, void*, I index) {
    if (index >= load<I>(self, 0x120)) return -1;
    // Negative indices and wrapping IMUL/ADD are deliberately unchecked in the original.
    const void* const entry = reinterpret_cast<const void*>(
        load<U>(self, 0x124) + static_cast<U>(index) * 0x1Cu);
    if (load<std::uint16_t>(entry, 0) == 10)
        return static_cast<I>(load<std::uint16_t>(entry, 2));
    return -1;
}

float __fastcall kotor_fastcall_00647dd0(void*, void*, const String* tag) {
    String* const text = const_cast<String*>(tag);
    if (text->BorrowedEquals005e5290("T") != 0 || text->BorrowedEquals005e5290("P") != 0)
        return *reinterpret_cast<const float*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28) + 0x20u);
    if (text->BorrowedEquals005e5290("S") != 0)
        return *reinterpret_cast<const float*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28) + 0x24u);
    if (text->BorrowedEquals005e5290("M") != 0)
        return *reinterpret_cast<const float*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28) + 0x28u);
    if (text->BorrowedEquals005e5290("L") != 0)
        return *reinterpret_cast<const float*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28) + 0x2Cu);
    if (text->BorrowedEquals005e5290("W") != 0)
        return *reinterpret_cast<const float*>(reinterpret_cast<U>(kotor_cell_rules_007a3a28) + 0x30u);
    // 00647E78 FLD DWORD [ESP+8]: unknown tags reinterpret the argument pointer bits.
    const U bits = reinterpret_cast<U>(tag);
    float result;
    std::memcpy(&result, &bits, sizeof result);
    return result;
}

U __fastcall kotor_fastcall_004efa80(void* self, void*) {
    static constexpr U masks[11] = {1, 2, 4, 8, 0x10, 0x20, 0, 0, 0x40, 0x40, 0x40};
    U result = 0;
    for (U slot = 0; slot != 18; ++slot) {
        void* const item = recovered_equipment_item_005a4c20(load<void*>(self, 0xA2C), nullptr, slot);
        if (item != nullptr) {
            void* const base = kotor_m15b_item_get_base_item_005b4790(item, nullptr);
            const U type = static_cast<U>(load<std::uint8_t>(base, 0xAC)) - 31u;
            if (type <= 10u) result |= masks[type];
        }
    }
    return result;
}
}
