// Authored private source — new TU; no shared source edit.
// Plan review: PLAN-REVIEW.md ACCEPT-WITH-FIXES; fixes applied.
//
// Original: swkotor.exe 004CD2D0 CSWSObject::IsDialogDelay (0x182 code bytes).
// ABI: ECX=self, no stack args, RET 0, EAX 0/1.
// Selected symbol: @kotor_fastcall_004cd2d0@8.
//
// Dependency-complete, link-incomplete: the ten one-level missing providers are
// declared in the private header and are not defined here. The deeper 0059FB60
// is a missing provider under 004CD660 (004CD695, RET 4), not a direct edge of
// this body. Do not add a stub, trap or safe fallback to make this link.
#include "71e53ce66d-endar_dialog_core_contracts.hpp"

using namespace endar_dialog_core;

extern "C" int __fastcall kotor_fastcall_004cd2d0(void* self, void* /*unused_edx*/) {
    // 004CD2D6..004CD302: owner/dialog ids and the GetCanSendDialog gate.
    void* const dialog_for_gate = load_ptr(self, 0x34);
    void* const id_source = load_ptr(dialog_for_gate, 0x64);
    const U32 id2 = (load_u32(id_source, 0) != 0)
                        ? load_u32(dialog_for_gate, 0x7C)
                        : 0x7F000000u;
    if (kotor_fastcall_005edc90(client_internal(), nullptr,
                                load_u32(self, 0x04), id2,
                                dialog_for_gate) == 0) {
        return 1;  // 004CD414 shared return-1 block
    }

    // 004CD308..004CD328: flags are read before the world-time snapshot;
    // the snapshot is taken before any predicate.
    const U32 flags = load_u32(self, 0x5C);
    U32 result = 0;
    U32 day = 0;
    U32 tick = 0;
    void* timer = kotor_fastcall_004aede0(server_app(), nullptr);
    frame_GetWorldTime_004ade40(timer, nullptr, &day, &tick);

    // 004CD32D..004CD33F: flags bit 2.
    if ((flags & 2u) != 0) {
        result = static_cast<U32>(kotor_fastcall_005edbc0(client_internal(), nullptr));
    }

    // 004CD341..004CD35B: flags bit 4. GetInGameGui is reloaded.
    if ((flags & 4u) != 0) {
        const unsigned gui_bits = frame_R4_005ed690(client_internal(), nullptr);
        void* const gui = reinterpret_cast<void*>(static_cast<std::uintptr_t>(gui_bits));
        result |= static_cast<U32>(kotor_fastcall_0062d570(gui, nullptr));
    }

    // 004CD35D..004CD38F: flags bit 1. The second GetInGameGui and the
    // high-resolution timer calls discard their results; keep the calls at
    // the original sites and in the original order.
    if ((flags & 1u) != 0) {
        void* const module = kotor_fastcall_005ed530(client_internal(), nullptr);
        result |= static_cast<U32>(kotor_fastcall_006412b0(module, nullptr));
        (void)frame_R4_005ed690(client_internal(), nullptr);
        void* const timers = load_ptr(menu_continuation::exo_base_007a39e0, 4);
        (void)reinterpret_cast<menu_continuation::ExoTimers*>(timers)
            ->GetHighResolutionTimer_005e9260();
    }

    // 004CD394..004CD3AF: flags bit 8. GetInGameGui is reloaded.
    if ((flags & 8u) != 0) {
        const unsigned gui_bits = frame_R4_005ed690(client_internal(), nullptr);
        void* const gui = reinterpret_cast<void*>(static_cast<std::uintptr_t>(gui_bits));
        result |= static_cast<U32>(kotor_fastcall_0062abb0(gui, nullptr));
    }

    // 004CD3B1..004CD40F: delay end-path. The dialog pointer is re-read at
    // 004CD3BB after the +0x1F4 test; it is not the B0 pointer.
    if (load_u32(self, 0x1F4) != 0) {
        void* const dialog_for_end = load_ptr(self, 0x34);
        if (load_u32(dialog_for_end, 0x3C) != 0) {
            store_u32(self, 0x220, 1);
            kotor_fastcall_005edbd0(client_internal(), nullptr);
            timer = kotor_fastcall_004aede0(server_app(), nullptr);
            (void)kotor_fastcall_004adf50(
                timer, nullptr, day, tick, 0, 1,
                reinterpret_cast<U32*>(reinterpret_cast<std::uintptr_t>(self) + 0x38),
                reinterpret_cast<U32*>(reinterpret_cast<std::uintptr_t>(self) + 0x3C));
            return 0;  // AddWorldTimes result discarded, including 0xFFFFFFFE
        }
    }

    // 004CD410..004CD41E: any active predicate returns 1.
    if (result != 0) {
        return 1;
    }

    // 004CD41F..004CD451: current time strictly before the stored end time.
    timer = kotor_fastcall_004aede0(server_app(), nullptr);
    const int cmp = recovered_event_time_compare_004adfa0(
        timer, nullptr, day, tick, load_u32(self, 0x38), load_u32(self, 0x3C));
    return (cmp == -1) ? 1 : 0;
}
