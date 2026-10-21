// A2 production TU: original 0068B4A0 CSWGuiMainInterface::Draw (thiscall, RET 4)
// and 00684E70 target-action countdown (thiscall, RET 4). Derived from the
// existing real donor lane-w4/src/hud/main_interface_draw.cpp (36621e97...) with
// its seven explicit trap definitions removed. No missing leaf is defined here:
// every external callee below is an exact import and the link must fail if its
// real body is absent. Intended placement: lane-w4/src/hud/main_interface_draw.cpp
// (replacement) so that ../common/w4_providers.hpp resolves; the shared headers
// are reused read-only and are never copied.
#include "23f8911e3d-w4_providers.hpp"
#include <initializer_list>

using namespace w4;

extern "C" {
// Existing selected providers. These are declarations only; no definitions are
// duplicated from their owners.
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);              // GetSWParty
void* __fastcall kotor_fastcall_00635460(void*, void*);              // GetPlayerCharacter
std::uint32_t __fastcall kotor_fastcall_005ede70(void*, void*);      // GetCombatMode
void __fastcall kotor_fastcall_006874c0(void*, void*, std::uint32_t); // ShowActionQueue
void __fastcall kotor_fastcall_006858e0(void*, void*, float);        // Action::Update
void __fastcall kotor_fastcall_00686a20(void*, void*, float);        // Status::Update
void __fastcall kotor_fastcall_00689d80(void*, void*);               // PopulateMenus
void __fastcall kotor_panel_draw_0040B760(void*, void*, float);      // CSWGuiPanel::Draw
void __stdcall kotor_AurGUIStartLayer_004591B0();
int __cdecl kotor_AurGUISetupViewport_004592F0(int, int, int, int, void*, int, int);
void __stdcall kotor_AurGUICloseViewport_00459580();
void __stdcall kotor_AurGUIStopLayer_004592B0();

// External HUD leaves owned by other lanes. No body, no trap, no fallback here.
void __fastcall kotor_fastcall_00687860(void*, void*);               // UpdatePortraits
void __fastcall kotor_fastcall_0068a310(void*, void*, float);        // UpdateIndicator
void __fastcall kotor_fastcall_0068a010(void*, void*);               // UpdateActionQueue
void __fastcall kotor_fastcall_0068ab10(void*, void*, float);        // DrawMap
void __fastcall kotor_fastcall_006851b0(void*, void*, float);        // Adjust
void __fastcall kotor_fastcall_00685330(void*, void*, float, std::uint32_t, std::uint32_t); // DrawStack
void __fastcall kotor_fastcall_00685ed0(void*, void*, float);        // TargetActionMenu::Draw

// Existing W1 data cell, C linkage.
extern volatile std::uint32_t recovered_camera_reset_cell_007a2288;
}

namespace {
const float kzero_0073d700 = 0.0f;
const U32 kNone = 0xBF800000u;  // -1.0f

// fld [t]; fsub delta; fst [t]; fcomp 0; fnstsw; test ah,0x41; jp keep.
// The original leaves the rounded store in place for >0 or unordered; only
// an ordered <0/==0 falls through to the -1.0f sentinel.
void count_down(void* self, U32 off, float delta) {
    const double v = static_cast<double>(lf(self, off)) - delta;
    sf(self, off, static_cast<float>(v));
    if (!jcc_jp(v, kzero_0073d700, 0x41)) sw(self, off, kNone);
}
}

// 00684E70: two -1-terminated countdowns, thiscall(float) RET 4.
extern "C" void __fastcall kotor_fastcall_00684e70(void* self, void*, float delta) {
    if (lw(self, 0x1ADC) != kNone) count_down(self, 0x1ADC, delta);  // 00684E70..00684E9C
    if (lw(self, 0x1AE0) != kNone) count_down(self, 0x1AE0, delta);  // 00684EA2..00684EC7
}

// 0068B4A0 CSWGuiMainInterface::Draw(float delta), thiscall RET 4.
extern "C" void __fastcall kotor_fastcall_0068b4a0(void* self, void*, float delta) {
    if ((lb(self, 0x44) & 0x80) == 0) return;                                                // 0068B4A3..0068B4A8
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr); // 0068B4AE..0068B4BD
    if (pc == nullptr || lw(pc, 4) == 0x7F000000u) return;                                   // 0068B4C2..0068B4D1
    if (lw(self, 0x6C) != kNone) count_down(self, 0x6C, delta);                              // 0068B4D7..0068B4FA
    kotor_fastcall_00687860(self, nullptr);                                                  // 0068B500..0068B502 UpdatePortraits
    kotor_fastcall_0068a310(self, nullptr, lf(lp(self, 0x18), 0x60));                        // 0068B507..0068B513 UpdateIndicator
    kotor_fastcall_00684e70(static_cast<U8*>(self) + 0xBC, nullptr, delta);                  // 0068B518..0068B523
    kotor_fastcall_00689d80(self, nullptr);                                                  // 0068B528..0068B52A PopulateMenus
    for (U32 i = 0; i < 9; ++i)                                                              // 0068B52F..0068B558
        kotor_fastcall_00686a20(static_cast<U8*>(self) + 0xA460 + i * 0x14C, nullptr, lf(lp(self, 0x18), 0x60));
    if (!jcc_jne(lf(self, 0x5A28), kzero_0073d700, 0x41)) {                                  // 0068B55A..0068B56B (timer > 0)
        const double t = static_cast<double>(lf(self, 0x5A28)) - delta;                      // 0068B571..0068B57C
        sf(self, 0x5A28, static_cast<float>(t));                                             // 0068B586
        if (!jcc_jne(t, kzero_0073d700, 0x41)) {                                             // 0068B58C..0068B597 (still > 0)
            for (U32 i = 0; i < 3; ++i)                                                      // 0068B5A0..0068B5B5
                kotor_fastcall_006851b0(static_cast<U8*>(self) + 0x1F88 + i * 0xEA8, nullptr, lf(self, 0x5A28));
            kotor_fastcall_006851b0(static_cast<U8*>(self) + 0x4B80, nullptr, lf(self, 0x5A28)); // 0068B5B7..0068B5C4
        } else {
            for (U32 i = 0; i < 3; ++i)                                                      // 0068B5D0..0068B5E3
                kotor_fastcall_006851b0(static_cast<U8*>(self) + 0x1F88 + i * 0xEA8, nullptr, 0.25f);
            U8* c = static_cast<U8*>(self) + 0x4B80;
            kotor_fastcall_006851b0(c, nullptr, 0.25f);                                      // 0068B5E5..0068B5F2
            for (U32 off : {0x6ACu, 0x6Cu, 0x7ECu, 0x93Cu, 0x1ACu, 0x2ECu, 0x42Cu, 0x56Cu})  // 0068B5F7..0068B62B
                sw(c, off, lw(c, off) & 0xFFFFFFFDu);
            sb(c, 0xEA0, 0); sb(c, 0xEA1, 0);                                                // 0068B631..0068B638
            sw(c, 0xD0C, lw(c, 0xD0C) & 0xFFFFFFFDu);                                        // 0068B63F
            sw(self, 0x5A28, 0);                                                             // 0068B645
        }
    }
    kotor_fastcall_006874c0(self, nullptr, kotor_fastcall_005ede70(client_app(), nullptr));   // 0068B64F..0068B660
    for (U32 i = 0; i < 6; ++i)                                                              // 0068B665..0068B67D
        kotor_fastcall_006858e0(static_cast<U8*>(self) + 0x772C + i * 0x71C, nullptr, delta);
    if (lb(self, 0xA45C) & 2) kotor_fastcall_0068a010(self, nullptr);                        // 0068B67F..0068B68A
    if (recovered_camera_reset_cell_007a2288 == 0) return;                                   // 0068B68F..0068B696
    kotor_fastcall_0068ab10(self, nullptr, delta);                                           // 0068B69C..0068B69F DrawMap
    kotor_panel_draw_0040B760(self, nullptr, delta);                                         // 0068B6A4..0068B6A7
    kotor_fastcall_00685ed0(static_cast<U8*>(self) + 0xBC, nullptr, delta);                  // 0068B6AC..0068B6B3
    kotor_AurGUIStartLayer_004591B0();                                                       // 0068B6B8
    if (kotor_AurGUISetupViewport_004592F0(li(self, 4), li(self, 8), li(self, 0xC), li(self, 0x10),
                                           static_cast<U8*>(self) + 0x50, 0, li(self, 0x4C)) != 0) { // 0068B6BD..0068B6E1
        // Original: mov esi,count; dec esi; js end; loop while the sign bit is clear.
        // `count-1` and the sign-bit test preserve the exact 0x80000000 wrap and skip
        // all negative counts without signed-overflow UB.
        for (U32 i = lw(self, 0x5CB8) - 1; (i & 0x80000000u) == 0; --i) {                      // 0068B6E3..0068B704
            void* control = lp(lp(self, 0x5CB4), i * 4);
            if (control != nullptr) vcall<void, float>(control, 0x38, delta);                // 0068B6F9..0068B700
        }
        kotor_AurGUICloseViewport_00459580();                                                // 0068B706
    }
    for (U32 i = 0; i < 3; ++i) {                                                            // 0068B70B..0068B761
        U8* s = static_cast<U8*>(self) + 0x2E28 + i * 0xEA8;
        const U32 first = (i == 0) ? 1u : 0u;                                                // 0068B713..0068B71A
        if (lb(s - 0x7F4, 0) & 2) {                                                          // 0068B71C..0068B723
            if (s[0] != 0) kotor_fastcall_00685330(s - 0xEA0, nullptr, delta, first, 1);      // 0068B725..0068B738
            if (s[1] != 0) kotor_fastcall_00685330(s - 0xEA0, nullptr, delta, first, 0);      // 0068B73D..0068B752
        }
    }
    if (lb(self, 0x522C) & 2) {                                                              // 0068B763..0068B771
        U8* c = static_cast<U8*>(self) + 0x4B80;
        if (c[0xEA0] != 0) kotor_fastcall_00685330(c, nullptr, delta, 0, 1);                 // 0068B773..0068B788
        if (c[0xEA1] != 0) kotor_fastcall_00685330(c, nullptr, delta, 0, 0);                 // 0068B78D..0068B7A2
    }
    kotor_AurGUIStopLayer_004592B0();                                                        // 0068B7A7
}
