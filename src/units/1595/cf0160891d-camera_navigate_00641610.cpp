// Task B: original 00641610 CSWCModule::SetNavigateCamera. Transcribed from the existing
// original disassembly dump, lines 752154-752219. Source-only; no native build.
// ABI: ECX=module, EDX=unused, no stack arg; RET 0; EAX=0 behavior-null / 1 normal.
// The frozen caller declares void and discards the result; the /alternatename join at the
// end of this file preserves that caller/header/object and binds it to this typed body.
#include "097d96e71e-camera.hpp"
#include "4644e3d982-camera_stick.hpp"
#include "805fc094ac-camera_free_navigate_contracts.hpp"

using namespace w4;

extern "C" U32 __fastcall kotor_fastcall_00641610(void* module, void*) {
    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 3) {                 // 00641626..3B
        void* stick = vcall<void*, I32>(lp(module, 0x40), 0x80, 0x106A);                 // 0064163D..47
        if (stick == nullptr) return 0;                                                  // 0064164D..A1
        kotor_fastcall_0063ba80(stick, nullptr, static_cast<U8*>(module) + 0x10C);       // 00641651..5A
    }

    void* allocation = live_operator_new(0x30);                                          // 0064165F..69
    void* made = nullptr;
    try {
        if (allocation != nullptr) {                                                     // 00641670..7A
            void* owner = vcall<void*>(lp(module, 0x40), 0x04);                          // 0064167C..84
            made = kotor_missing_00638670_navigate_ctor(
                allocation, nullptr, owner);                                             // 00641685..8C
        }
    } catch (...) {
        // Original C++ new-expression heap cleanup: free the allocation if the
        // constructor or its argument evaluation throws, then rethrow. No free on success.
        live_free(allocation);
        throw;
    }

    menu_tables_m41::SetCameraMode_0061b6f0(
        kotor_fastcall_005ed700(client_app(), nullptr), nullptr, 7);                     // 006416A4..BD
    vcall<void, void*>(lp(module, 0x40), 0x7C, made);                                    // 006416C2..C8
    return 1;                                                                            // 006416D0
}

// Frozen caller binding: keep client_input_event.h / client_input_frame.cpp / the frozen
// object unchanged. The caller's void import keeps its EAX discard; this typed body returns
// the original 0/1. Same precedent as source/patched/pi_0774_joins.cpp:37.
#pragma comment(linker, "/alternatename:?set_navigate_camera_00641610@kotor_input_required@@YIXPAX0@Z=@kotor_fastcall_00641610@8")
