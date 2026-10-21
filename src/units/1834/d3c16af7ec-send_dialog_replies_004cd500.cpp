// Authored private source — new TU; no shared source edit.
// Plan review: PLAN-REVIEW.md ACCEPT-WITH-FIXES; fixes applied.
//
// Original: swkotor.exe 004CD500 CSWSObject::SendDialogReplies (0x72 code bytes).
// ABI: ECX=self, no stack args, RET 0, EAX 0/1.
// Selected symbol: @kotor_fastcall_004cd500@8.
#include "71e53ce66d-endar_dialog_core_contracts.hpp"

using namespace endar_dialog_core;

extern "C" int __fastcall kotor_fastcall_004cd500(void* self, void* /*unused_edx*/) {
    // 004CD503..004CD511: explicit null dialog -> 0.
    if (load_ptr(self, 0x34) == nullptr) {
        return 0;
    }

    // 004CD512..004CD520: +0x50 / IsDialogDelay queue gate.
    if (load_u32(self, 0x50) != 0 ||
        kotor_fastcall_004cd2d0(self, nullptr) != 0) {
        return queue_replies(self);  // 004CD55C: +0x48 then +0x4C = -1
    }

    // 004CD522..004CD535: reply handler. Dialog is re-read after the gate.
    void* dialog = load_ptr(self, 0x34);
    store_u32(self, 0x1F4, 0);
    const int handler_result = kotor_fastcall_005a3820(
        dialog, nullptr, self, -1);
    if (handler_result != 0) {
        return 1;  // 004CD56A
    }

    // 004CD537..004CD55B: abort path, re-reading the dialog pointer. The
    // original passes the re-read pointer to RunEndConversationScript without
    // a null check; preserve that.
    dialog = load_ptr(self, 0x34);
    kotor_fastcall_005a0a40(dialog, nullptr, self, 0);
    store_u32(self, 0x38, 0);
    store_u32(self, 0x3C, 0);
    store_u32(self, 0x48, 0);
    store_u32(self, 0x214, 1);
    return 1;
}
