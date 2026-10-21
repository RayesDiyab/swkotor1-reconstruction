#pragma once
// lane-m1-opt-gameplay: CSWGuiOptionsMouse (0xEF8; operator new at 006E64C3, ctor 006E2600, table 007585F8),
// opened from CSWGuiInGameGameplay::OnMouse 006E64A0. Original: swkotor.exe GOG K1, static disassembly only.
#include "93783737f5-opt_gameplay.h"   // shared HandleInputEvent 006E6180 / OnPanelAdded 006E6170 / 27-slot table type
#include "77e070ab58-opt_folded.h"     // 006DED90 / 006DF390 (fallback owner) + Slider cast view

namespace kotor_m1_opt {
// Layout established by the ctor 006E2600 and dtor 006E29B0 (reverse order).
struct Mouse : Panel {                          // +000 CSWGuiPanel (0040B570 at 006E262B)
    Label desc_item;                            // +064 (006E2641); Initialize98 from LB_DESC proto (006E2871)
    Label lbl_title;                            // +1A4 (006E2651) "LBL_TITLE" (006E2775)
    ListBox lb_desc;                            // +2E4 (006E2661) "LB_DESC" (006E27AB); +5A5 = lb+2C1
    Button btn_back;                            // +5C4 (006E2671) "BTN_BACK" (006E27E1)
    Button btn_default;                         // +788 (006E2681) "BTN_DEFAULT" (006E2817)
    Slider sli_mousesen;                        // +94C (006E2693) "SLI_MOUSESEN" (006E26DF)
    Label lbl_mousesen;                         // +B04 (006E26A3) "LBL_MOUSESEN" (006E270F)
    Checkbox cb_revbuttons;                     // +C44 (006E26B5) "CB_REVBUTTONS" (006E2745)
};
static_assert(offsetof(Mouse, lb_desc) == 0x2E4 && offsetof(Mouse, sli_mousesen) == 0x94C &&
              offsetof(Mouse, lbl_mousesen) == 0xB04 && offsetof(Mouse, cb_revbuttons) == 0xC44 &&
              sizeof(Mouse) == 0xEF8, "CSWGuiOptionsMouse 0xEF8");

struct MouseHandlers : kih::GuiObject {
    void __thiscall OnEnterSlider_006de200(kih::Control* control);
    void __thiscall OnRevMouseButtons_006dfd10(kih::Control* control);
    void __thiscall OnEnterRevMouseButtons_006dfe60(kih::Control* control);
    void __thiscall OnMoveMouseSenSlider_006dfee0(kih::Control* control);
    void __thiscall OnDefault_006e12b0(kih::Control* control);
};
}
extern "C" {
void __fastcall kotor_m1_opt_mouse_destruct_006e29b0(kotor_m1_opt::Mouse* self, void*);
void* __fastcall kotor_m1_opt_mouse_deleting_destruct_006e2990(gui_load::Object* self, void*, gui_load::Word flags);
void __fastcall kotor_m1_opt_mouse_set_up_options_006dfe70(kotor_m1_opt::Mouse* self, void*);
void __fastcall kotor_m1_opt_client_options_set_default_mouse_0061d9d0(void* options, void*);
extern const kotor_m1_opt::GuiM1OptGameplayMethods gui_m1_opt_mouse_methods_007585f8;
}
