#pragma once
// lane-m1-options / L1-options: CSWGuiOptionsMain (0x1220; operator new at 0067B32C) opened from
// CSWGuiMainMenu::OnOptionsPicked 0067B2F0; Back returns to the main menu (006DFF58) and rewrites the run-dir
// swkotor.ini through CClientExoApp::SaveOptions 005EDF70 (M49 lane-config owner).
// Original: swkotor.exe GOG K1, static disassembly only. Conventions as gui-load (D1/D21/D24/D47).
#include "3370aef598-gui_objects.h"
#include "ce2f5dc6ef-mainmenu.h"      // gui-load: MainMenuHandlers (0067B2F0 owner type), AddPanel 0040BC70, Panel::Draw 0040B760
#include "19aa661562-m1_shared.h"
#include <cstddef>
#include <cstdint>

namespace kotor_m1_options {
namespace kih = kotor_input_hover;
using gui_load::Word;
using gui_load::I32;

// Layout established by the ctor 006E3E80 (member ctor calls) and dtor 006E42C0 (reverse order).
struct OptionsMain : gui_load::Panel {                 // +0000 CSWGuiPanel (0040B570 at 006E3EA7)
    gui_load::Button buttons[6];                        // +0064 eh_vector_constructor_iterator(+64, 1C4h, 6) 006E3ECF:
                                                        //   [0] +064 BTN_GAMEPLAY  (006E3FBD)   [1] +228 BTN_FEEDBACK (006E3FF2)
                                                        //   [2] +3EC BTN_AUTOPAUSE (006E4027) [3] +5B0 BTN_GRAPHICS (006E405C)
                                                        //   [4] +774 BTN_SOUND     (006E4091)   [5] +938 never loaded
    gui_load::Label desc_item;                          // +0AFC (006E3EDF) description item; Initialize from LB_DESC proto (006E418D)
    gui_load::Label lbl_title;                          // +0C3C (006E3EEF) "LBL_TITLE" (006E3F59)
    gui_load::Button btn_back;                          // +0D7C (006E3F01) "BTN_BACK" (006E40F2)
    gui_lb::ListBox lb_desc;                            // +0F40 (006E3F11) "LB_DESC" (006E3F8E); +1201 = lb+2C1, +120C = lb proto
};
static_assert(offsetof(OptionsMain, buttons) == 0x64 && offsetof(OptionsMain, desc_item) == 0xAFC &&
              offsetof(OptionsMain, lbl_title) == 0xC3C && offsetof(OptionsMain, btn_back) == 0xD7C &&
              offsetof(OptionsMain, lb_desc) == 0xF40 && sizeof(OptionsMain) == 0x1220, "CSWGuiOptionsMain 0x1220");

// CSWGuiControlEvent callbacks registered by the ctor (thiscall members, RET 4; single-inheritance view, D47).
struct OptionsMainHandlers : kih::GuiObject {
    void __thiscall OnEnterSlot_006e0010(kih::Control* control);      // event 0 on buttons[0..5]
    void __thiscall OnGameplayOpt_006de240(kih::Control* control);    // events 27h/2Dh buttons[0] (D83 trap after guard)
    void __thiscall OnFeedbackOpt_006e2df0(kih::Control* control);    // buttons[1]
    void __thiscall OnAutopauseOpt_006de2c0(kih::Control* control);   // buttons[2]
    void __thiscall OnGraphicsOpt_006e3d80(kih::Control* control);    // buttons[3]
    void __thiscall OnSoundOpt_006e3e00(kih::Control* control);       // buttons[4]
};
}

extern "C" {
kotor_m1_options::OptionsMain* __fastcall kotor_m1_options_construct_006e3e80(kotor_m1_options::OptionsMain* self, void*,
                                                                             gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_options_destruct_006e42c0(kotor_m1_options::OptionsMain* self, void*);
void* __fastcall kotor_m1_options_deleting_destruct_006e42a0(kotor_m1_options::OptionsMain* self, void*, gui_load::Word flags);
void __fastcall kotor_m1_options_handle_input_006dff10(gui_load::Object* self, void*, gui_load::I32 event, gui_load::I32 pressed);
void __fastcall kotor_m1_options_set_description_006de340(kotor_m1_options::OptionsMain* self, void*, gui_load::Word strref) noexcept(false);
extern const struct GuiM1OptionsMainMethods gui_m1_options_main_methods_00758838;
}
