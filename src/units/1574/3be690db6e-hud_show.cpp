// Private reconstruction of original 006867F0 CSWGuiMainInterfaceChar::Show.
//
// Original ABI: thiscall(self, int a1, int a2, int a3, int a4), ECX=self,
// four stack arguments, callee RET 0x10. Reconstruction export:
// @kotor_fastcall_006867f0@24 (dummy EDX, four stack args, generated RET 0x10).
//
// No calls, no SEH, no heap. Every store is a raw flag/byte field on self.
// The original's only state predicate is `test bl,bl` (the low byte of a2) for
// the +0xEA0/+0xEA1 pair, not a full 32-bit a2 == 0 test.
//
// Source-only private proposal; not built/run. No native or runtime acceptance.
#include "da79f00dee-w4_providers.hpp"

using w4::U32;

namespace {
void set_bit1(void* self, U32 off, bool set) {
    const U32 old = w4::lw(self, off);
    w4::sw(self, off, (old & ~2u) | (set ? 2u : 0u));
}
}

extern "C" void __fastcall kotor_fastcall_006867f0(
    void* self, void*, int a1, int a2, int a3, int a4) {
    const bool show = (a1 & 1) != 0;
    const unsigned char state = static_cast<unsigned char>(a2);

    // 006867FA..00686861: bit 1 of the five visibility/state fields = a1 & 1.
    set_bit1(self, 0x6AC, show);
    set_bit1(self, 0x06C, show);
    set_bit1(self, 0x7EC, show);
    set_bit1(self, 0x93C, show);

    // 00686870..006868B0: flag bits gated by a1 and the low state byte.
    set_bit1(self, 0x1AC, a1 != 0 && (state & 2) != 0);
    set_bit1(self, 0x2EC, a1 != 0 && (state & 1) != 0);
    set_bit1(self, 0x42C, a1 != 0 && (state & 4) != 0);
    set_bit1(self, 0x56C, a1 != 0 && (state & 4) != 0);

    // 006868D8/EB: only the low byte of a2 selects the counter stores.
    if (state == 0) {
        w4::sb(self, 0xEA0, static_cast<w4::U8>(a3));
        w4::sb(self, 0xEA1, static_cast<w4::U8>(a4));
    } else {
        w4::sb(self, 0xEA0, 0);
        w4::sb(self, 0xEA1, 0);
    }

    // 00686905: bit 1 of +0xD0C = a1 & 1.
    set_bit1(self, 0xD0C, show);
}
