#pragma once
#include <cstddef>
#include <cstdint>

namespace kotor_reconstruction {
// Integration binding for the dword observed at original VA 007a2280.
extern std::uint32_t exo_seed_mask;

#pragma pack(push, 1)
struct CExoRandInternal {
    std::uint32_t state_vars[4];             // 00
    std::uint32_t masks[4];                  // 10
    std::uint32_t right_shifts[4];           // 20
    std::uint32_t total_shifts[4];           // 30
    std::uint32_t left_shifts[4];            // 40
    std::uint32_t additional_left_shifts[4]; // 50
    std::int32_t seeded;                     // 60
    unsigned char* random_pool;             // 64
    std::uint16_t pool_position;             // 68
    unsigned char untouched_tail[0x26];     // 6a..8f, opaque in this group

    __declspec(noinline) std::uint32_t Rand();
    __declspec(noinline) void ReSeed(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
    __declspec(noinline) void Add(unsigned char*, std::int32_t);
};
struct CExoRand {
    CExoRandInternal* internal;
    __declspec(noinline) std::uint32_t Rand();
};
#pragma pack(pop)
static_assert(sizeof(void*) == 4, "x86 only");
static_assert(sizeof(CExoRandInternal) == 0x90 && sizeof(CExoRand) == 4, "saved extents");
static_assert(offsetof(CExoRandInternal, masks) == 0x10, "masks");
static_assert(offsetof(CExoRandInternal, right_shifts) == 0x20, "right shifts");
static_assert(offsetof(CExoRandInternal, total_shifts) == 0x30, "total shifts");
static_assert(offsetof(CExoRandInternal, left_shifts) == 0x40, "left shifts");
static_assert(offsetof(CExoRandInternal, additional_left_shifts) == 0x50, "additional shifts");
static_assert(offsetof(CExoRandInternal, seeded) == 0x60, "seeded");
static_assert(offsetof(CExoRandInternal, random_pool) == 0x64, "pool pointer");
static_assert(offsetof(CExoRandInternal, pool_position) == 0x68, "word position");
}
