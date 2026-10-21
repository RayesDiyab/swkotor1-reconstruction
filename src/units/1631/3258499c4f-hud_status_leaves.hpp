#pragma once
// Private typed view of the CSWGuiStatusSummary fields touched by the four
// admitted HUD bark-status leaves:
//
//   00624AB0  CSWGuiStatusSummary::AddCredits
//   0062B580  CSWGuiStatusSummary::AddXp
//   0062B5A0  CSWGuiStatusSummary::AddStealthXp
//   00624A70  CSWGuiStatusSummary::AddAlignmentShift
//
// The original object is larger than this range; this header is a partial field
// view, not a claim about the complete class layout.  The four leaves only touch
// +0x64..+0x77.  Access goes through memcpy helpers (the same rule the selected
// W4 provider helpers lw/sw/lb/sb use), so no unrelated object is
// reinterpret-cast and no strict-aliasing assumption is introduced.
//
// Oracle: swkotor.exe sha256
//   9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435
// Contracts: lane-c/CONTRACTS.json rows 00624AB0 / 0062B580 / 0062B5A0 / 00624A70
#include <cstddef>
#include <cstdint>
#include <cstring>

#if !defined(_M_IX86)
#error "The admitted HUD status leaves are 32-bit x86 __fastcall code."
#endif
static_assert(sizeof(void*) == 4, "x86 only: original receiver/argument slots are 4 bytes");

namespace kotor::hud_status {

// Compile-time field-offset pins.  Only the fields named here are part of this
// view; the original object continues beyond +0x78.
struct SummaryLayout {
    std::uint8_t  reserved_00[0x64];  // +0x00..+0x63: untouched by these leaves
    std::uint32_t flags;              // +0x64
    std::uint8_t  alignment_light;    // +0x68
    std::uint8_t  alignment_dark;     // +0x69
    std::uint8_t  reserved_6a[0x02];  // +0x6A..+0x6B
    std::uint32_t credits;            // +0x6C
    std::uint32_t xp;                 // +0x70
    std::uint32_t stealth_xp;         // +0x74
};

static_assert(offsetof(SummaryLayout, flags) == 0x64, "flags must be +0x64");
static_assert(offsetof(SummaryLayout, alignment_light) == 0x68, "alignment_light must be +0x68");
static_assert(offsetof(SummaryLayout, alignment_dark) == 0x69, "alignment_dark must be +0x69");
static_assert(offsetof(SummaryLayout, credits) == 0x6C, "credits must be +0x6C");
static_assert(offsetof(SummaryLayout, xp) == 0x70, "xp must be +0x70");
static_assert(offsetof(SummaryLayout, stealth_xp) == 0x74, "stealth_xp must be +0x74");
static_assert(sizeof(SummaryLayout) == 0x78, "partial view ends immediately after stealth_xp");

constexpr std::size_t kFlagsOffset = offsetof(SummaryLayout, flags);
constexpr std::size_t kAlignmentLightOffset = offsetof(SummaryLayout, alignment_light);
constexpr std::size_t kAlignmentDarkOffset = offsetof(SummaryLayout, alignment_dark);
constexpr std::size_t kCreditsOffset = offsetof(SummaryLayout, credits);
constexpr std::size_t kXpOffset = offsetof(SummaryLayout, xp);
constexpr std::size_t kStealthXpOffset = offsetof(SummaryLayout, stealth_xp);

inline std::uint32_t load_u32(const void* object, std::size_t offset) noexcept {
    std::uint32_t value;
    std::memcpy(&value, static_cast<const std::uint8_t*>(object) + offset, sizeof value);
    return value;
}

inline void store_u32(void* object, std::size_t offset, std::uint32_t value) noexcept {
    std::memcpy(static_cast<std::uint8_t*>(object) + offset, &value, sizeof value);
}

inline std::uint8_t load_u8(const void* object, std::size_t offset) noexcept {
    return static_cast<const std::uint8_t*>(object)[offset];
}

inline void store_u8(void* object, std::size_t offset, std::uint8_t value) noexcept {
    static_cast<std::uint8_t*>(object)[offset] = value;
}

// Reinterpret a 32-bit payload as a signed value without implementation-defined
// unsigned-to-signed conversion (the original uses signed jcc predicates).
inline std::int32_t as_i32(std::uint32_t bits) noexcept {
    std::int32_t value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

// Reinterpret the low payload byte as a signed 8-bit shift (00624A70: `mov al`
// followed by signed `jle`).
inline std::int8_t as_i8(std::uint8_t bits) noexcept {
    std::int8_t value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace kotor::hud_status

extern "C" {
// ECX = CSWGuiStatusSummary*, EDX = unused dummy, one stack dword, RET 4.
// The selected owner symbols are @kotor_fastcall_00624ab0@12 and siblings.
// AddAlignmentShift keeps a 32-bit declaration because its caller pushes a
// 32-bit value; the oracle reads only the low byte into AL.
void __fastcall kotor_fastcall_00624ab0(void* self, void*, std::uint32_t amount);
void __fastcall kotor_fastcall_0062b580(void* self, void*, std::uint32_t amount);
void __fastcall kotor_fastcall_0062b5a0(void* self, void*, std::uint32_t amount);
void __fastcall kotor_fastcall_00624a70(void* self, void*, std::uint32_t value);
}
