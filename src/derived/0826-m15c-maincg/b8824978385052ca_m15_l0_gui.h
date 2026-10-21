#pragma once
// lane-m15-0 (L15-0): GUI event-callback (PMF) contract for Milestone 1.5.
// Callback entries registered through CSWGuiControl::AddEvent / CSWGuiMessageBox::SetCallback are __thiscall members
// of single-inheritance views of kotor_input_hover::GuiObject (one code word per PMF, D47); registration converts them
// with static_cast to gui_load::EventFunc (never a function-pointer cast). Include root: lane-m15-0\src plus the M60
// gui-load roots (gui_objects.h).
#include "8c61ee1430fb8439_gui_objects.h"   // gui_load::EventFunc, kotor_input_hover::{GuiObject, Control}
#include "f155d76cd432eed7_m15_l0.h"

namespace kotor_m15_l0 {
namespace kih = kotor_input_hover;

// 00644720 CSWGuiPanel::OnYButtonPressed vcall thunk (Ghidra name): MOV EAX,[ECX]; JMP [EAX+5Ch] - the panel's
// method-table slot +5C with the same stack word (CSWGuiControl*), whose RET 4 cleans it. DEFINED by L15-0 (m15-l0-gui).
// Registered by CSWGuiNameChargen 006F9E70 (L15-C).
struct PanelEventThunks15 : kih::GuiObject {
    void __thiscall OnYButtonPressed_00644720(kih::Control* control);
};
inline gui_load::EventFunc event_func(void (__thiscall PanelEventThunks15::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);
}

// CSWGuiTutorialBox message-box callbacks registered by CSWGuiTutorialBox::SetTutorialReason 006AA900 (L15-0).
// All are __thiscall(CSWGuiControl*) RET 4 (006AA25B, 006AA2B9, 006AA34B, 006AA3BE, 006AA430, 006AA479, 006AA4F4,
// 006AA58A, 006AA5C9, 006AA614, 006AA66B).
//   MessageBoxUnpause_006aa230      DEFINED by L15-0 (m15-l0-gui)
//   Perform*_006aa260..006aa620     DEFINED by L15-F as D83 function traps (gameplay-only tutorial reasons); L15-0
//                                   only references them (depends_on_symbols in lane-m15-0\PACKET.json).
struct TutorialBoxHandlers : kih::GuiObject {
    void __thiscall MessageBoxUnpause_006aa230(kih::Control* control);
    void __thiscall PerformCombatFeat_006aa260(kih::Control* control);
    void __thiscall PerformForcePower_006aa2c0(kih::Control* control);
    void __thiscall PerformUseGrenade_006aa350(kih::Control* control);
    void __thiscall PerformSetMine_006aa3d0(kih::Control* control);
    void __thiscall PerformPlayerFlourishWeapons_006aa440(kih::Control* control);
    void __thiscall PerformBash_006aa480(kih::Control* control);
    void __thiscall PerformAttack_006aa520(kih::Control* control);
    void __thiscall PerformNonCombatXButton_006aa590(kih::Control* control);
    void __thiscall PerformCombatYButton_006aa5d0(kih::Control* control);
    void __thiscall PerformReturnToBase_006aa620(kih::Control* control);
};
inline gui_load::EventFunc event_func(void (__thiscall TutorialBoxHandlers::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);
}
}
