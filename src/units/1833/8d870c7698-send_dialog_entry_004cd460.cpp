// Authored private source — new TU; no shared source edit.
// Plan review: PLAN-REVIEW.md ACCEPT-WITH-FIXES; fixes applied.
//
// Original: swkotor.exe 004CD460 CSWSObject::SendDialogEntry (0x97 code bytes).
// ABI: ECX=self, one stack dword (entry), callee-clean RET 4, EAX 0/1.
// Selected symbol: @kotor_fastcall_004cd460@12.
//
// The original stores the handler status in the incoming entry argument slot.
// That slot is caller-allocated and callee-cleaned by RET 4, so a local status
// dword is behaviorally equivalent (it is not observable to the caller).
#include "71e53ce66d-endar_dialog_core_contracts.hpp"

using namespace endar_dialog_core;

extern "C" int __fastcall kotor_fastcall_004cd460(void* self, void* /*unused_edx*/, U32 entry) {
    // 004CD464..004CD471: explicit null dialog -> 0.
    if (load_ptr(self, 0x34) == nullptr) {
        return 0;
    }

    // 004CD475..004CD48C: queue path for a real entry, with the +0x50 gate
    // short-circuiting IsDialogDelay.
    if (entry != 0xFFFFFFFFu &&
        (load_u32(self, 0x50) != 0 ||
         kotor_fastcall_004cd2d0(self, nullptr) != 0)) {
        return queue_entry(self, entry);
    }

    // 004CD49F..004CD4BF: handler call. Dialog is re-read after the queue gate.
    void* dialog = load_ptr(self, 0x34);
    U32 status = 0;
    store_u32(self, 0x1F4, 0);
    const int handler_result = kotor_fastcall_005a4010(
        dialog, nullptr, self, -1, entry, &status);
    if (status == 10u) {
        return queue_entry(self, entry);
    }
    if (handler_result != 0) {
        return 1;  // 004CD4EE
    }

    // 004CD4C5..004CD4E6: abort path, re-reading the dialog pointer.
    dialog = load_ptr(self, 0x34);
    if (dialog != nullptr) {
        kotor_fastcall_005a0a40(dialog, nullptr, self, 0);
        store_u32(self, 0x38, 0);
        store_u32(self, 0x3C, 0);
        store_u32(self, 0x48, 0);
        store_u32(self, 0x214, 1);
    }
    return 0;  // 004CD4E6
}
