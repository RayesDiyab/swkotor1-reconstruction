// Source-faithful bodies for the four admitted CSWGuiStatusSummary bark-status
// leaves.  Original addresses, ABIs and field offsets come from the pinned
// oracle disassembly (lane-c/evidence/orig/):
//
//   00624AB0  AddCredits          48 bytes, one exit  RET 4, no callees
//   0062B580  AddXp               28 bytes, one exit  RET 4, no callees
//   0062B5A0  AddStealthXp        28 bytes, one exit  RET 4, no callees
//   00624A70  AddAlignmentShift   50 bytes, two exits RET 4, no callees
//
// All four are __fastcall(self, unused EDX, one stack dword) and are leaves:
// no globals, no allocation, no EH, no indirect calls.  Arithmetic is performed
// on unsigned 32-bit values so wrapping cannot invoke signed-overflow UB; the
// signed comparisons use as_i32()/as_i8() payload reads.
#include "3258499c4f-hud_status_leaves.hpp"

using namespace kotor::hud_status;

// 00624AB0 CSWGuiStatusSummary::AddCredits (bark status 1)
extern "C" void __fastcall kotor_fastcall_00624ab0(void* self, void*, std::uint32_t amount_bits) {
    // 00624AB4..00624AB6: a zero amount returns without touching +0x64 or +0x6C.
    if (amount_bits == 0) return;

    // 00624AB8..00624AC9.  The 0x100 bit is chosen from the SIGNS of the old
    // credits and the incoming amount, not from the result.  Old credits == 0
    // takes the fall-through jge arm and never sets the sign bit.
    const std::int32_t amount = as_i32(amount_bits);
    const std::uint32_t credits_before_bits = load_u32(self, kCreditsOffset);
    const std::int32_t credits_before = as_i32(credits_before_bits);

    if (credits_before > 0) {
        if (amount < 0) {
            // 00624ACB: or dword ptr [ecx+0x64], 0x100
            store_u32(self, kFlagsOffset, load_u32(self, kFlagsOffset) | 0x100u);
        }
    } else if (credits_before < 0) {
        if (amount > 0) {
            // 00624ACB: or dword ptr [ecx+0x64], 0x100
            store_u32(self, kFlagsOffset, load_u32(self, kFlagsOffset) | 0x100u);
        }
    }

    // 00624AD2..00624AD7: 32-bit wrapping add, store, then OR bit 0.
    store_u32(self, kCreditsOffset, credits_before_bits + amount_bits);
    store_u32(self, kFlagsOffset, load_u32(self, kFlagsOffset) | 0x1u);
}

// 0062B580 CSWGuiStatusSummary::AddXp (bark status 2)
extern "C" void __fastcall kotor_fastcall_0062b580(void* self, void*, std::uint32_t amount_bits) {
    // 0062B584..0062B586: a zero amount is a complete no-op (bit 0 is not set).
    if (amount_bits == 0) return;

    // 0062B588..0062B596, in the oracle's order: compute the wrapped sum, read
    // the flags, OR bit 0, store the sum, then store the flags.
    const std::uint32_t xp = load_u32(self, kXpOffset) + amount_bits;
    const std::uint32_t flags = load_u32(self, kFlagsOffset) | 0x1u;
    store_u32(self, kXpOffset, xp);
    store_u32(self, kFlagsOffset, flags);
}

// 0062B5A0 CSWGuiStatusSummary::AddStealthXp (bark status 3)
extern "C" void __fastcall kotor_fastcall_0062b5a0(void* self, void*, std::uint32_t amount_bits) {
    // 0062B5A4..0062B5A6: a zero amount is a complete no-op (bit 0 is not set).
    if (amount_bits == 0) return;

    // 0062B5A8..0062B5B6, in the oracle's order.
    const std::uint32_t stealth_xp = load_u32(self, kStealthXpOffset) + amount_bits;
    const std::uint32_t flags = load_u32(self, kFlagsOffset) | 0x1u;
    store_u32(self, kStealthXpOffset, stealth_xp);
    store_u32(self, kFlagsOffset, flags);
}

// 00624A70 CSWGuiStatusSummary::AddAlignmentShift (bark statuses 4 and 5)
extern "C" void __fastcall kotor_fastcall_00624a70(void* self, void*, std::uint32_t value_bits) {
    // 00624A70..00624A76: only the low byte is observed; a zero byte returns
    // without touching either alignment byte or the flags.
    const std::uint8_t raw_shift = static_cast<std::uint8_t>(value_bits & 0xFFu);
    const std::int8_t shift = as_i8(raw_shift);
    if (shift == 0) return;

    if (shift > 0) {
        // 00624A7A..00624A88: add dl,al (8-bit wrap), OR bit 0, store byte first.
        const std::uint8_t light =
            static_cast<std::uint8_t>(load_u8(self, kAlignmentLightOffset) + raw_shift);
        const std::uint32_t flags = load_u32(self, kFlagsOffset) | 0x1u;
        store_u8(self, kAlignmentLightOffset, light);
        store_u32(self, kFlagsOffset, flags);
    } else {
        // 00624A8E..00624A9C: sub dl,al (8-bit wrap), OR bit 0, store byte first.
        // A negative shift therefore increases the dark-side byte by |shift|.
        const std::uint8_t dark =
            static_cast<std::uint8_t>(load_u8(self, kAlignmentDarkOffset) - raw_shift);
        const std::uint32_t flags = load_u32(self, kFlagsOffset) | 0x1u;
        store_u8(self, kAlignmentDarkOffset, dark);
        store_u32(self, kFlagsOffset, flags);
    }
}
