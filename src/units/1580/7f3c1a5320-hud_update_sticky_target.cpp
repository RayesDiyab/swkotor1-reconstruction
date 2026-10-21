// Private reconstruction of original 006889C0
// CSWGuiMainInterface::UpdateStickyTarget.
//
// Original ABI: thiscall() RET 0; ECX = self; no stack arguments; plain ret.
// Original body 006889C0..00688DAB: sub esp,0x3C; saves EBP/ESI/EDI/EBX; no SEH
// frame; no /GS cookie; no heap allocation in the body.
// Reconstruction export: @kotor_fastcall_006889c0@8 (dummy EDX, no stack args).
//
// Selected 1566 providers (exact symbols from server_harness_1566.map):
//   005ED580  @kotor_fastcall_005ed580@12         00703070  0701-m48b-client-leaves-a.obj
//   005ED700  @kotor_fastcall_005ed700@8          00617750  0021-service-menu20-4.obj
//   005ED8B0  @kotor_fastcall_005ed8b0@8          00704EC0  0707-m48b-client-framebuffer.obj
//   00635460  @kotor_fastcall_00635460@8          00472C50  0767-frame_menu_path_client-w4.obj
//   0063D4B0  @kotor_m15a_object_get_server_object_0063d4b0@8
//                                                 0047AF10  0817-m15a-object.obj
//   0057CB80  @recovered_object_get_reputation_0057cb80@16
//                                                 0045F4A0  w1_native.obj
//   006FAE8C  _genuine_x87_ftol_006fae8c          0062A9B0  0206-renderer-x87-ftol-alias.obj
//             -> _unresolved_ftol2_006fae8c        0061D8E0  0064-world-timer-x87-conversion.obj
//   00406D80  ?construct_00406d80@Ref@menu_resources@@QAEPAU12@PBD@Z
//                                                 00617FD0  0030-service-resource-13.obj
//   00414C00  @gui_load_border_params_set_fill_00414c00@16
//                                                 006321D0  0295-gui-load-gui-params.obj
// Label +4 SetExtent: _gui_load_label_methods_0073e5b8 @ 0082CBD8 slot +4 =
//   @gui_load_label_set_extent_00417780@12 @ 006332D0
//   (0296-gui-load-gui-objects.obj).
//
// D6 0057CB80 is REAL and ABI-compatible: selected body 0xA4 bytes (COFF section
// 122 .text, value 0), reads [ebp+8]/[ebp+0xC], calls 00446410, RET 8 at
// 0045F527.  The @16 decoration counts ECX + dummy EDX + the two stack args.
// Source declaration: int __fastcall recovered_object_get_reputation_0057cb80(
//   void*, void*, U32, int).
//
// C++ member binding for 00406D80: the selected provider is the public thiscall
// member menu_resources::Ref::construct_00406d80(const char*), returning Ref*,
// RET 4.  It is declared below with the exact class/namespace/member name so the
// mangled symbol matches ?construct_00406d80@Ref@menu_resources@@QAEPAU12@PBD@Z.
// The provider wrapper is real RET 4; its inner @unresolved_ref_assign_00406290@16
// is real RET 8.  The 16-byte local is a POD; no destructor is called.
//
// Virtual slots used (selected bindings; see PLAN-U.md §7):
//   +0x0C AsSWCObject; +0x13C GetReticlePoint(Vector*); +0x2C AsSWCCreature;
//   +0x138 IsHostile; +0x14 AsSWCDoor; +0x3C AsSWCTrigger; +0x4C AsSWCPlaceable;
//   label +4 SetExtent.
//
// Fault behavior is intentionally unchecked, matching the original: null app
// manager/client, null options, null self, null label/border objects, null
// GetServerObject(pc)/GetServerObject(placeable) results, and repeated AsSWC*
// returns are not guarded.  No traps, fakes, null guards or invented defaults.
// No raw original addresses are dereferenced: all data/functions bind through the
// selected symbols/helpers below.

// Integration placement: this TU belongs at <candidate>/src/hud/ so that
// ../common/w4_providers.hpp resolves to <candidate>/src/common/w4_providers.hpp
// (the donor t1b layout).  Do not fork the common header.
#include "8d1d1f9e87-w4_providers.hpp"

#include <cstdint>

using w4::I32;
using w4::U32;

// Selected [0x8338F0] cell: ?word_008338f0@kotor_input_cells_required@@3HC
// @ 009B4314, 0774-m48t-f-input-process-input-w4.obj.  The original caches this
// dword in EDI once, before GetGameObject; the options branch does not re-read it.
namespace kotor_input_cells_required {
extern volatile int word_008338f0;
}

// Selected 00406D80 provider.  The member name is part of the mangled symbol; do
// not rename it.  Four scalar members keep the 16-byte CResRef local out of the
// /GS array-instrumentation shape while preserving layout and size.
namespace menu_resources {
struct Ref {
    U32 a;
    U32 b;
    U32 c;
    U32 d;
    Ref* __thiscall construct_00406d80(const char* text);
};
}  // namespace menu_resources

static_assert(sizeof(menu_resources::Ref) == 16, "16-byte CResRef local");

// Exact selected direct-callee declarations.
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*, void*, U32);
void* __fastcall kotor_fastcall_005ed700(void*, void*);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
void* __fastcall kotor_fastcall_00635460(void*, void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*);
int __fastcall recovered_object_get_reputation_0057cb80(void*, void*, U32, int);
int __cdecl genuine_x87_ftol_006fae8c();  // real __ftol2 body; ST0 -> EAX, pops ST0
void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const void*, int);
}

namespace {

struct Vec3 {
    float x, y, z;
};

struct Extent {
    int x, y, w, h;
};

// Integer-based pointer arithmetic keeps the original unchecked null faults
// (e.g. a null options pointer faults at +8, not through a C++ UB shortcut).
void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

// Original 006FAE8C call contract: value on ST0, EAX = low dword, ST0 popped.
// The arithmetic feeding this helper is carried in double (53-bit), matching the
// original PC53 x87 sequence for these finite operations.
int ftol2_X87(double value) {
    int result;
    volatile double v = value;
    __asm {
        fld qword ptr [v]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

}  // namespace

// ------------------------------------------------------------------ 006889C0
extern "C" void __fastcall kotor_fastcall_006889c0(void* self, void*) {
    // All locals are declared/initialized before any forward goto (the original
    // has no destructible locals; the CResRef local is a POD).
    void* appManager = nullptr;                 // B0
    U32 targetId = 0;                          // B0
    void* client = nullptr;                    // B0 / B1 / B3
    I32 drive = 0;                             // B0 cached [0x8338F0]
    void* obj = nullptr;                       // B0
    void* esi = nullptr;                       // B0 target client object
    void* options = nullptr;                   // B1
    unsigned edi = 0;                          // B1 options/drive predicate
    void* party = nullptr;                     // B3
    void* pc = nullptr;                        // B3 player character
    Vec3 screen = {0.0f, 0.0f, 0.0f};          // B4
    Extent extent = {0, 0, 0, 0};              // B4 dead copy, later placement
    unsigned char result = 0;                  // B4
    int size = 0x40;                           // B4
    unsigned friendFlag = 1;                   // B4
    menu_resources::Ref ref;                   // B11 placed-path CResRef
    unsigned placed = 1;                       // B9 placement flag

    // B0: target resolution.  Global cell read #1; self+0x5CAC; [app+4];
    // [0x8338F0] cached once; then GetGameObject.
    appManager = w4::app_manager();
    targetId = w4::lw(self, 0x5CAC);
    client = w4::lp(appManager, 4);
    drive = static_cast<I32>(kotor_input_cells_required::word_008338f0);
    obj = kotor_fastcall_005ed580(client, nullptr, targetId);
    if (obj != nullptr) {
        esi = w4::vcall<void*>(obj, 0x0C);     // AsSWCObject
    } else {
        esi = nullptr;
    }

    // B1: options gate.  Global cell read #2; [app+4] re-read.
    options = kotor_fastcall_005ed700(w4::client_app(), nullptr);
    edi = static_cast<unsigned>(drive);
    if ((w4::lb(options, 8) & 2u) != 0) {
        edi = (drive == 0) ? 1u : 0u;
    }

    // B2: drop target (self+0x5CAC = 0); early return, no +0x5BB0 write.
    if (esi != 0 && edi != 0) {
        w4::sw(self, 0x5CAC, 0u);
        return;
    }

    // B3: party / player character.  Global cell read #3; [app+4] re-read.
    party = kotor_fastcall_005ed8b0(w4::client_app(), nullptr);
    pc = kotor_fastcall_00635460(party, nullptr);
    if (esi == 0 || pc == 0) {
        // B13: clear bit 1, then fall through to the shared epilogue (B14).
        w4::sw(self, 0x5BB0, w4::lw(self, 0x5BB0) & ~2u);
        goto shared_epilogue;
    }

    // B4: dead 4-dword copy of self+0x5B70; zero 3-float screen; GetReticlePoint
    // with a 3-float out vector.  The copied extent is overwritten later and is
    // never read; the volatile reads preserve the original read/fault order.
    {
        volatile const U32* dead =
            reinterpret_cast<volatile const U32*>(atv(self, 0x5B70));
        extent.x = static_cast<int>(dead[0]);
        extent.y = static_cast<int>(dead[1]);
        extent.w = static_cast<int>(dead[2]);
        extent.h = static_cast<int>(dead[3]);
    }
    screen.x = 0.0f;
    screen.y = 0.0f;
    screen.z = 0.0f;
    result = w4::vcall<unsigned char, void*>(esi, 0x13C, &screen);
    size = 0x40;
    friendFlag = 1;
    if (result == 0) {
        // B14 via the shared epilogue: no self+0x5BB0 read/write.
        goto shared_epilogue;
    }

    // B5/B6: distance, AsSWCCreature coefficient, size, friend logic.
    {
        const double dx =
            static_cast<double>(w4::lf(pc, 0x24)) - static_cast<double>(w4::lf(esi, 0x24));
        const double dy =
            static_cast<double>(w4::lf(pc, 0x28)) - static_cast<double>(w4::lf(esi, 0x28));
        const double dz =
            static_cast<double>(w4::lf(pc, 0x2C)) - static_cast<double>(w4::lf(esi, 0x2C));
        // Original FPU order: dz^2, then + dy^2, then + dx^2; fstp dword.
        double sum = dz * dz;
        sum += dy * dy;
        sum += dx * dx;
        const float d = static_cast<float>(sum);

        // Called after the distance and before the 900/25 comparisons.  Its
        // return selects the 48.0 (creature) vs 32.0 (non-creature) coefficient.
        const unsigned asCreature = w4::vcall<unsigned>(esi, 0x2C);

        if (asCreature != 0) {
            // Creature branch: far 0x10, near 0x40, mid 48.0.
            if (!w4::jcc_jne(static_cast<double>(d), 900.0, 0x01)) {
                size = 0x10;
            } else if (!w4::jcc_jne(static_cast<double>(d), 25.0, 0x41)) {
                double s = w4::x87_fsqrt(static_cast<double>(d));
                s = s - 5.0;
                s = s * 0.04;
                s = s * 48.0;
                size = 0x40 - ftol2_X87(s);
            }
            friendFlag = (w4::vcall<unsigned>(esi, 0x138) == 0) ? 1u : 0u;
        } else {
            // Non-creature branch: far 0x20, near 0x40, mid 32.0.
            if (!w4::jcc_jne(static_cast<double>(d), 900.0, 0x01)) {
                size = 0x20;
            } else if (!w4::jcc_jne(static_cast<double>(d), 25.0, 0x41)) {
                double s = w4::x87_fsqrt(static_cast<double>(d));
                s = s - 5.0;
                s = s * 0.04;
                s = s * 32.0;
                size = 0x40 - ftol2_X87(s);
            }

            // B7/B8: door (up to 3 vcalls), trigger (1 vcall), placeable
            // (up to 6 vcalls).  Repeated AsSWC* calls are intentional.
            bool decided = false;

            void* door1 = w4::vcall<void*>(esi, 0x14);
            if (door1 != nullptr) {
                void* door2 = w4::vcall<void*>(esi, 0x14);
                if (w4::lw(door2, 0x110) != 0) {
                    void* door3 = w4::vcall<void*>(esi, 0x14);
                    if (w4::lw(door3, 0x114) != 0) {
                        friendFlag = 0u;
                        decided = true;
                    }
                }
            }

            if (!decided) {
                void* trigger = w4::vcall<void*>(esi, 0x3C);
                if (trigger != nullptr) {
                    // IsHostile is called on esi (the original ECX=ESI), not on
                    // the returned trigger pointer.
                    friendFlag =
                        (w4::vcall<unsigned>(esi, 0x138) == 0) ? 1u : 0u;
                    decided = true;
                }
            }

            if (!decided) {
                void* placeable1 = w4::vcall<void*>(esi, 0x4C);
                if (placeable1 != nullptr) {
                    void* placeable2 = w4::vcall<void*>(esi, 0x4C);
                    if (w4::lw(placeable2, 0x11C) != 0) {
                        void* placeable3 = w4::vcall<void*>(esi, 0x4C);
                        if (w4::lw(placeable3, 0x124) != 0) {
                            friendFlag = 0u;
                            decided = true;
                        }
                    }
                }
            }

            if (!decided) {
                // Call #4 still runs when call #1 returned null.
                void* placeable4 = w4::vcall<void*>(esi, 0x4C);
                if (placeable4 == nullptr) {
                    friendFlag = 1u;
                } else {
                    void* placeable5 = w4::vcall<void*>(esi, 0x4C);
                    if (w4::vcall<unsigned>(placeable5, 0x138) == 0) {
                        friendFlag = 1u;
                    } else {
                        // Evaluate pc's server id before the final placeable
                        // AsSWCPlaceable/GetServerObject pair, matching the
                        // original call/argument-evaluation order.
                        void* serverPc =
                            kotor_m15a_object_get_server_object_0063d4b0(pc, nullptr);
                        const U32 pcId = w4::lw(serverPc, 4);  // unchecked
                        void* placeable6 = w4::vcall<void*>(esi, 0x4C);
                        void* serverPlaceable =
                            kotor_m15a_object_get_server_object_0063d4b0(
                                placeable6, nullptr);
                        const int rep = recovered_object_get_reputation_0057cb80(
                            serverPlaceable, nullptr, pcId, 0);
                        friendFlag = (rep > 10) ? 1u : 0u;  // signed jg
                    }
                }
            }
        }
    }

    // B9/B10: self+0xBF70 bounds; result==2 is a hide; the centered placement is
    // reached only when the screen point is inside the inner margins.  The code
    // only special-cases 0 and 2; any non-0/2 nonzero result enters this test.
    {
        const int x0 = w4::li(self, 0xBF70);
        const int y0 = w4::li(self, 0xBF74);
        const int w0 = w4::li(self, 0xBF78);
        const int h0 = w4::li(self, 0xBF7C);

        placed = 1;
        if (result == 2) {
            placed = 0;
        } else {
            const int leftMargin =
                static_cast<int>(static_cast<U32>(x0) + 0x20u);
            const int rightEdge = static_cast<int>(
                static_cast<U32>(x0) + static_cast<U32>(w0) - 0x20u);
            const int topMargin =
                static_cast<int>(static_cast<U32>(y0) + 0x20u);
            const int bottom = static_cast<int>(
                static_cast<U32>(y0) + static_cast<U32>(h0) - 0x20u);

            // fild edge; fcomp screen coord; exact C0/C3 or C0/C2 predicate.
            if (w4::jcc_je(static_cast<double>(leftMargin),
                           static_cast<double>(screen.x), 0x41)) {
                placed = 0;
            } else if (w4::jcc_jnp(static_cast<double>(rightEdge),
                                   static_cast<double>(screen.x), 0x05)) {
                placed = 0;
            } else if (w4::jcc_je(static_cast<double>(topMargin),
                                  static_cast<double>(screen.y), 0x41)) {
                placed = 0;
            } else if (w4::jcc_jnp(static_cast<double>(bottom),
                                   static_cast<double>(screen.y), 0x05)) {
                placed = 0;
            } else {
                const int half = size / 2;  // signed truncation toward zero
                // Top first, then left; ftol2 keeps `half` on the FPU stack.
                extent.y = ftol2_X87(static_cast<double>(screen.y) -
                                     static_cast<double>(half));
                extent.x = ftol2_X87(static_cast<double>(screen.x) -
                                     static_cast<double>(half));
            }
        }
        extent.w = size;
        extent.h = size;
    }

    if (!placed) {
        // B12: not placed (result==2 or off-margin): clear bit 1, return.
        w4::sw(self, 0x5BB0, w4::lw(self, 0x5BB0) & ~2u);
        return;
    }

    // B11: placed.  Flag set BEFORE SetExtent; then literal choice, CResRef
    // construction, SetFillImage, and the 0.5f field store.
    w4::sw(self, 0x5BB0, w4::lw(self, 0x5BB0) | 2u);
    w4::vcall<void, const void*>(atv(self, 0x5B6C), 0x04, &extent);
    {
        const char* const literal =
            (friendFlag != 0) ? "friendlyreticle" : "hostilereticle";
        ref.construct_00406d80(literal);  // selected 00406D80 C++ member
    }
    gui_load_border_params_set_fill_00414c00(atv(self, 0x5BDC), nullptr, &ref, 0);
    w4::sf(self, 0x5BE8, 0.5f);
    return;

shared_epilogue:
    // B14: shared epilogue reached by B13 (after the clear) and by result==0
    // (which performs no self+0x5BB0 write).
    return;
}
