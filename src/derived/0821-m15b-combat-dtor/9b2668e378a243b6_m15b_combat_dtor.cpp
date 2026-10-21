// lane-m15-b (L15-B, Milestone 1.5) unit m15b-combat-dtor:
// 00550E00 CCombatInformation::~CCombatInformation (00550E00..00550F2C), called by
// ~CSWCCreatureStats 00649E40 (00649EB1). The constructor 00552280 is lane-m15-0's. Kept as a separate
// unit so the composer can drop it if another lane also provides 00550E00 (single owner).
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly (lane work\asm\00550E00.asm).
// No throwing callee (live_free / ~CExoString): the unwind states set at 00550E26/00550EDD/00550EEA/
// 00550EF8/00550F08 (FuncInfo via 0071D68C) have no observable effect on the normal path.
#include "bf662019daaaa2b6_m15b_stats.h"
#include "e1805047c9b7563b_startup_string.h"   // kotor_reconstruction::StartupString (~CExoString 005E5C20)
#include "20e68b036c30980c_allocator_live.h"   // live_free 006FA390
#include <cstdint>
#include <cstring>

namespace {
template <class T> inline T ld(const void* base, std::uintptr_t off) noexcept {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof(T)); return v;
}
template <class T> inline void st(void* base, std::uintptr_t off, T v) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + off, &v, sizeof(T));
}
// Remove element 0 of the CExoArrayList<void*> at +data/+count and free it (00550E60..00550E98 shape):
// first = data[0]; count--; shift the tail down; free(first) when non-null.
void pop_front_free(void* self, std::uintptr_t data_off, std::uintptr_t count_off) {
    while (ld<std::int32_t>(self, count_off) > 0) {                                  // 00550E2D..00550E5B / 00550E95..00550E98
        void* const first = ld<void**>(self, data_off)[0];                           // 00550E60..00550E63
        const std::int32_t n = ld<std::int32_t>(self, count_off) - 1;                // 00550E65..00550E6A
        st<std::int32_t>(self, count_off, n);                                        // 00550E6F
        for (std::int32_t k = 0; k < ld<std::int32_t>(self, count_off); ++k) {       // 00550E6D..00550E86
            void** const data = ld<void**>(self, data_off);
            data[k] = data[k + 1];                                                   // 00550E74..00550E7E
        }
        if (first) live_free(first);                                                 // 00550E88..00550E8D
    }
}
}  // namespace

extern "C" void __fastcall kotor_m15b_combat_information_destruct_00550e00(void* self, void*) noexcept(false) {
    st<std::uint8_t>(self, 0x09, 3);                                                 // 00550E21..00550E2A
    st<std::uint8_t>(self, 0x00, 0); st<std::uint8_t>(self, 0x01, 0);               // 00550E30..00550E32
    st<std::uint8_t>(self, 0x02, 0); st<std::uint8_t>(self, 0x03, 0);               // 00550E35..00550E38
    st<std::uint8_t>(self, 0x04, 0);                                                 // 00550E3B
    st<std::uint32_t>(self, 0x0C, 0);                                                // 00550E3E
    st<std::uint8_t>(self, 0x05, 0); st<std::uint8_t>(self, 0x06, 0);               // 00550E41..00550E44
    st<std::uint8_t>(self, 0x07, 0);                                                 // 00550E47
    st<std::uint8_t>(self, 0x08, 1);                                                 // 00550E4A
    st<std::uint8_t>(self, 0x0A, 0); st<std::uint8_t>(self, 0x0B, 0);               // 00550E4E..00550E51
    st<std::uint8_t>(self, 0x10, 0);                                                 // 00550E54
    st<std::uint8_t>(self, 0x11, 2);                                                 // 00550E57
    pop_front_free(self, 0x14, 0x18);                                                // 00550E2D / 00550E5B..00550E98
    pop_front_free(self, 0x20, 0x24);                                                // 00550E9A..00550ED8
    reinterpret_cast<kotor_reconstruction::StartupString*>(static_cast<unsigned char*>(self) + 0x3C)->~StartupString(); // 00550EDA..00550EE2
    reinterpret_cast<kotor_reconstruction::StartupString*>(static_cast<unsigned char*>(self) + 0x34)->~StartupString(); // 00550EE7..00550EEF
    live_free(ld<void*>(self, 0x20)); st<std::uint32_t>(self, 0x20, 0);              // 00550EF4..00550F01
    live_free(ld<void*>(self, 0x14)); st<std::uint32_t>(self, 0x14, 0);              // 00550F04..00550F1C
}
