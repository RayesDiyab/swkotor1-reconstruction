// lane-r4-lowers: CSWGuiManager / CExoArrayList / list-box pulse lowers reached by
// R4 GUI Update 0040CE70 and Draw 0040CC50 on the first main-menu frame.
// Evidence: dumpbin of swkotor.exe 9c10e045..1435; instruction addresses cited.
#include "46bba3a5643b0682_r4_lowers.h"

using namespace r4_lowers;
using menu_continuation::GuiManager;

// 0040AB70 CSWGuiManager::GetPosInModalStack(panel), RET 4 (R4 frame_FindPopup_0040ab70).
extern "C" unsigned __fastcall frame_FindPopup_0040ab70(GuiManager* gui, void*, void* panel) {
    const I32 count = static_cast<I32>(w(gui, 0x98));              // 0040AB70
    if (count <= 0) return 0xFFFFFFFFu;                            // 0040AB79..0040AB7B / 0040AB93
    U32 slot = w(gui, 0x94);                                       // 0040AB7D
    for (I32 i = 0; i < count; ++i, slot += 4) {                   // 0040AB87..0040AB91
        if (w(at(slot), 0) == reinterpret_cast<U32>(panel)) return static_cast<U32>(i); // 0040AB87..0040AB89
    }
    return 0xFFFFFFFFu;                                            // 0040AB93
}

// 0040ABA0 CSWGuiManager::GetPosInPanelList(panel), RET 4 (null entries never match).
extern "C" std::uint32_t __fastcall kotor_fastcall_0040aba0(void* gui, void*, void* panel) {
    const I32 count = static_cast<I32>(w(gui, 0x8c));              // 0040ABA1
    if (count <= 0) return 0xFFFFFFFFu;                            // 0040ABA9..0040ABAC
    U32 slot = w(gui, 0x88);                                       // 0040ABAE
    for (I32 i = 0; i < count; ++i, slot += 4) {                   // 0040ABB8..0040ABC8
        const U32 entry = w(at(slot), 0);                          // 0040ABB8
        if (entry != 0 && entry == reinterpret_cast<U32>(panel)) return static_cast<U32>(i); // 0040ABBA..0040ABC0
    }
    return 0xFFFFFFFFu;                                            // 0040ABCA
}

// 0040BD70 CSWGuiManager::PanelExists(panel), RET 4 (R4 frame_ContainsPanel_0040bd70).
extern "C" unsigned __fastcall frame_ContainsPanel_0040bd70(GuiManager* gui, void*, void* panel) {
    return kotor_fastcall_0040aba0(gui, nullptr, panel) != 0xFFFFFFFFu ? 1u : 0u; // 0040BD70..0040BD82
}

// 006857A0 CExoArrayList<T*>::deleteAt(index), RET 4 (R4 frame_RemoveArray_006857a0).
// [M47 compose] 006857A0 single body = lane-renderer-bodies kotor_renderer_gl::deleteAt_006857a0 (D32/D59, render_pool Tier B); frame_RemoveArray_006857a0 joined (definition removed; declaration kept)
extern "C" void __fastcall frame_RemoveArray_006857a0(void* list, void*, int index);

// 0040BE00 CSWGuiManager::PopModalPanel, RET, EAX = removed top panel (0 when empty).
extern "C" void* __fastcall kotor_fastcall_0040be00(void* gui, void*) {
    const U32 count = w(gui, 0x98);                                // 0040BE03
    if (count == 0) return nullptr;                                // 0040BE09..0040BE0E (EAX = 0)
    void* top = pw(at(w(gui, 0x94)), (count - 1) * 4u);            // 0040BE0F..0040BE16
    frame_RemoveArray_006857a0(bytes(gui) + 0x94, nullptr, static_cast<int>(count - 1)); // 0040BE1B..0040BE25
    kotor_fastcall_0040acc0(gui, nullptr);                         // 0040BE2A..0040BE2C
    const I32 remaining = static_cast<I32>(w(gui, 0x98));          // 0040BE31
    if (remaining > 0) {                                           // 0040BE37..0040BE39
        void* below = pw(at(w(gui, 0x94)), static_cast<U32>(remaining) * 4u - 4u); // 0040BE3B..0040BE3D
        if (vcall0(below, 0x44) != 0) {                            // 0040BE41..0040BE4A
            const U32 again = w(gui, 0x98);                        // 0040BE4C
            void* panel = pw(at(w(gui, 0x94)), again * 4u - 4u);   // 0040BE52..0040BE54
            void* control = at(vcall0(panel, 0x44));               // 0040BE58..0040BE5A
            vcall1(control, 0x7c, 1);                              // 0040BE5D..0040BE63
        }
    }
    return top;                                                    // 0040BE67
}

// 0040C830 CSWGuiManager::RemovePanel(panel), RET 4, EAX = 0 (0040C888..0040C88C) or 1 (0040C8D1).
// lane-m15-0: CClientExoAppInternal::HideLoadScreen 005F6E95..005F6E9C consumes EAX. The consumer headers
// (composition.h, gui_objects.h) declare the entry void, so the body returns U32 and the exported entry is a
// D21 naked JMP to it (ECX/EDX/stack unchanged; the body's RET 4 returns EAX to the caller).
namespace { U32 __fastcall remove_panel_body_0040c830(GuiManager* gui, void*, void* panel); }
extern "C" __declspec(naked) void __fastcall frame_RemovePanel_0040c830(GuiManager*, void*, void*) {
    __asm jmp remove_panel_body_0040c830                           // 0040C830
}
namespace {
U32 __fastcall remove_panel_body_0040c830(GuiManager* gui, void*, void* panel) {
    if (!panel) return 0;                                          // 0040C836..0040C83A -> 0040C888 XOR EAX,EAX
    if (kotor_fastcall_0040aba0(gui, nullptr, panel) == 0xFFFFFFFFu) return 0; // 0040C83C..0040C845
    const U32 modal = frame_FindPopup_0040ab70(gui, nullptr, panel); // 0040C847..0040C84A
    if (modal != 0xFFFFFFFFu) {                                    // 0040C84F..0040C852
        if (modal == w(gui, 0x98) - 1u) kotor_fastcall_0040be00(gui, nullptr); // 0040C854..0040C861
        else frame_RemoveArray_006857a0(bytes(gui) + 0x94, nullptr, static_cast<int>(modal)); // 0040C868..0040C86F
    }
    vcall0(panel, 0x4c);                                           // 0040C874..0040C878
    const U32 position = kotor_fastcall_0040aba0(gui, nullptr, panel); // 0040C87B..0040C87E
    if (position == 0xFFFFFFFFu) return 0;                         // 0040C883..0040C886 -> 0040C888
    frame_RemoveArray_006857a0(bytes(gui) + 0x88, nullptr, static_cast<int>(position)); // 0040C88F..0040C896
    if (b8(panel, 0x44) & 8u) --bytes(gui)[0x70];                  // 0040C89B..0040C8A1
    if (!(b8(panel, 0x44) & 0x10u)) --bytes(gui)[0x71];            // 0040C8A4..0040C8AA
    kotor_fastcall_0040acc0(gui, nullptr);                         // 0040C8AD..0040C8AF
    const U32 y = w(gui, 4), x = w(gui, 0);                        // 0040C8B4 / 0040C8B7
    sw(gui, 8, 0);                                                 // 0040C8BD
    kotor_fastcall_0040c1e0(gui, nullptr, x, y);                   // 0040C8B9..0040C8C4
    sw(gui, 0x10, 0);                                              // 0040C8CA
    return 1;                                                      // 0040C8D1 MOV EAX,1
}
}

// 00418300 CSWGuiListBox::UpdateArrowPulsing(float), cdecl RET (R4 _frame_SceneUpdate_00418300).
// Exact x87 sequence; cells 0078D3E8 / 007A457C / 0078D3E4 owned by this lane.
namespace {
const U32 k_zero_0073d700 = 0x00000000u;       // 0.0f
const U32 k_one_0073d764 = 0x3F800000u;        // 1.0f
const U32 k_low_0073e46c = 0x3E4CCCCDu;        // 0.2f
const U32 k_period_0073e31c = 0x3F400000u;     // 0.75f
const U32 k_rate_0073e318 = 0x3FAAAAABu;       // 1.3333334f
}
extern "C" void __cdecl frame_SceneUpdate_00418300(float delta) {
    __asm {
        fld dword ptr r4_lowers_cells::arrow_pulse_timer_0078d3e8      // 00418300
        fsub delta                                                     // 00418306
        fst dword ptr r4_lowers_cells::arrow_pulse_timer_0078d3e8      // 0041830A
        fcomp dword ptr k_zero_0073d700                                // 00418310
        fnstsw ax
        test ah, 41h
        jp no_flip                                                     // 0041831B
        mov al, byte ptr r4_lowers_cells::arrow_pulse_up_007a457c      // 0041831D
        test al, al
        sete byte ptr r4_lowers_cells::arrow_pulse_up_007a457c         // 00418324
        mov dword ptr r4_lowers_cells::arrow_pulse_timer_0078d3e8, 3F400000h // 0041832B
    no_flip:
        mov al, byte ptr r4_lowers_cells::arrow_pulse_up_007a457c      // 00418335
        test al, al
        je pulse_down                                                  // 0041833C
        fld dword ptr k_one_0073d764                                   // 0041833E
        fld dword ptr k_low_0073e46c                                   // 00418344
        jmp pulse_mix
    pulse_down:
        fld dword ptr k_low_0073e46c                                   // 0041834C
        fld dword ptr k_one_0073d764                                   // 00418352
    pulse_mix:
        fld dword ptr k_period_0073e31c                                // 00418358
        fsub dword ptr r4_lowers_cells::arrow_pulse_timer_0078d3e8     // 0041835E
        fmul dword ptr k_rate_0073e318                                 // 00418364
        fxch st(2)                                                     // 0041836A
        fsub st, st(1)                                                 // 0041836C
        fmulp st(2), st                                                // 0041836E
        fxch st(1)                                                     // 00418370
        fadd st, st(1)                                                 // 00418372
        fstp dword ptr r4_lowers_cells::arrow_pulse_alpha_0078d3e4     // 00418374
        fstp st(0)                                                     // 0041837A
    }
}
