// Task B: original 006413C0 CSWCModule::SetFreeLookCamera. Transcribed from the existing
// original disassembly dump, lines 751950-752067. Source-only; no native build.
// ABI: ECX=module, EDX=unused, [esp+4]=U32 id; RET 4; EAX=0 early / 1 normal.
// The frozen caller declares void and discards the result; the /alternatename join at the
// end of this file preserves that caller/header/object and binds it to this typed body.
#include "097d96e71e-camera.hpp"
#include "4644e3d982-camera_stick.hpp"
#include "805fc094ac-camera_free_navigate_contracts.hpp"

using namespace w4;

extern "C" U32 __fastcall kotor_fastcall_006413c0(void* module, void*, U32 id) {
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, id);                  // 006413E4..ED
    if (object == nullptr) return 0;                                                    // 006413EF..400

    void* creature = vcall<void*>(object, 0x2C);                                        // 00641403..0F
    if (creature == nullptr) return 0;                                                  // 0064140F..421

    if (lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 3) {                // 00641424..37
        void* stick = vcall<void*, I32>(lp(module, 0x40), 0x80, 0x106A);                // 00641439..49
        if (stick != nullptr) {                                                         // 0064144B..4D
            kotor_fastcall_0063ba80(stick, nullptr, static_cast<U8*>(module) + 0x10C);  // 0064144F..5A

            W4Vector camera_position;
            W4Vector* const position =
                kotor_fastcall_0063f600(stick, nullptr, &camera_position);              // 0064145D..64
            const double y =
                static_cast<double>(lf(creature, 0x28)) - static_cast<double>(position->y); // 00641469..6C
            const double x =
                static_cast<double>(lf(creature, 0x24)) - static_cast<double>(position->x); // 00641471..78

            W4Vector direction;
            direction.x = static_cast<float>(x);                                        // 00641480
            direction.z = 0.0f;                                                         // 00641484
            direction.y = static_cast<float>(y);                                        // 0064148C

            W4Vector normalized;
            W4Vector* const normalized_ptr =
                vector_normalized_004ab180(&normalized, &direction);                    // 00641490
            vcall<void, W4Vector*>(creature, 0x88, normalized_ptr);                     // 00641498..9B
        }
    }

    void* allocation = live_operator_new(0xC4);                                         // 006414A1..AD
    void* made = nullptr;
    try {
        if (allocation != nullptr) {                                                    // 006414B4..BE
            void* camera = lp(module, 0x40);                                            // 006414C0
            const U32 effect =
                kotor_fastcall_00610490(creature, nullptr);                             // 006414C3..C5
            void* part = vcall<void*>(creature, 0xD0);                                  // 006414CA..CF
            void* owner = vcall<void*>(camera, 0x04);                                   // 006414D6..DA
            made = kotor_missing_0063a5d0_freelook_ctor(
                allocation, nullptr, owner, part, effect);                              // 006414DE..E5
        }
    } catch (...) {
        // Original C++ new-expression heap cleanup: free the allocation if the
        // constructor or its argument evaluation throws, then rethrow. No free on success.
        live_free(allocation);
        throw;
    }

    menu_tables_m41::SetCameraMode_0061b6f0(
        kotor_fastcall_005ed700(client_app(), nullptr), nullptr, 5);                    // 006414EB..505
    vcall<void, void*>(lp(module, 0x40), 0x7C, made);                                   // 0064150A..510
    sw(module, 0x0C, 5);                                                                // 00641519
    return 1;                                                                           // 00641521
}

// Frozen caller binding: keep client_input_event.h / client_input_frame.cpp / the frozen
// object unchanged. The caller's void import keeps its EAX discard; this typed body returns
// the original 0/1. Same precedent as source/patched/pi_0774_joins.cpp:37.
#pragma comment(linker, "/alternatename:?set_free_look_camera_006413c0@kotor_input_required@@YIXPAX0I@Z=@kotor_fastcall_006413c0@12")
