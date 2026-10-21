// Authored private source — new TU; no shared source edit.
// Plan review: PLAN-REVIEW.md ACCEPT-WITH-FIXES; fixes applied.
//
// Original: swkotor.exe 004CB4F0 CSWSObject::StopDialog (0xAE code bytes).
// ABI: ECX=self, no stack args, RET 0, EAX 0/1.
// Selected symbol: @kotor_fastcall_004cb4f0@8.
//
// The original calls the vtable +0x30 slot twice. At 004CB4F7 the first call
// has no outgoing stack args. At 004CB500 the original reads the invalid-object
// cell, then at 004CB50C calls the same slot again with the two
// SetLockOrientationToObject argument dwords already pushed, and 004F34A0
// consumes them with RET 8. The source projection reads the cell into a local
// before the second vcall and then calls 004F34A0; native review must confirm
// the emitted order and that the slot is RET 0 (G-CORE-11/G-CORE-18).
#include "71e53ce66d-endar_dialog_core_contracts.hpp"

using namespace endar_dialog_core;

extern "C" int __fastcall kotor_fastcall_004cb4f0(void* self, void* /*unused_edx*/) {
    // 004CB4F4..004CB511: two vtable +0x30 calls and the lock-orientation call.
    void* const creature = vcall0(self, 0x30);
    if (creature != nullptr) {
        const U32 invalid = menu_tables_m41::cells::invalid_object_00746644;
        w5_creature_set_lock_orientation_to_object_004f34a0(
            vcall0(self, 0x30), nullptr, invalid, 0);
    }

    // 004CB516..004CB523: owner gate.
    const U32 owner = load_u32(self, 0x54);
    if (owner == 0x7F000000u) {
        return 0;  // 004CB56C
    }
    if (owner != load_u32(self, 0x04)) {
        // 004CB525..004CB540: lookup path.
        void* const object_array = recovered_object_array_004aed70(server_app(), nullptr);
        void* found_object = nullptr;
        const unsigned found = kotor_fastcall_004d8230(
            object_array, nullptr, owner, &found_object);
        if (static_cast<U8>(found) != object_projectile_literal_0074666c[0]) {
            return 1;  // 004CB595
        }
        // 004CB548..004CB564: remove the found object from the dialog and mark
        // this object's owner invalid.
        kotor_fastcall_004cd660(found_object, nullptr, load_u32(self, 0x04));
        store_u32(self, 0x54, 0x7F000000u);
        return 1;
    }

    // 004CB565..004CB571: owner matches this object; null dialog -> 0.
    void* dialog = load_ptr(self, 0x34);
    if (dialog == nullptr) {
        return 0;
    }

    // 004CB572..004CB592: end the conversation, clear state, delete the dialog,
    // then clear the dialog pointer. The dialog pointer is re-read after the
    // end-conversation call and before the virtual delete.
    kotor_fastcall_005a0a40(dialog, nullptr, self, 0);
    dialog = load_ptr(self, 0x34);
    store_u32(self, 0x38, 0);
    store_u32(self, 0x3C, 0);
    store_u32(self, 0x48, 0);
    store_u32(self, 0x50, 0);
    if (dialog != nullptr) {
        vdel(dialog, 1);
    }
    store_ptr(self, 0x34, nullptr);
    return 1;  // 004CB595
}
