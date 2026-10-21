#pragma once
// lane-m15-d (L15-D): GUI event-callback (PMF) contract. Callback entries registered through CSWGuiControl::AddEvent
// are __thiscall members of single-inheritance views of kotor_input_hover::GuiObject (one code word per PMF, D47/D107);
// registration converts them with static_cast to gui_load::EventFunc (never a function-pointer cast).
// Include roots: lane-m15-d\src plus the M60 gui-load roots (gui_objects.h).
#include "72ae896ecb-gui_objects.h"   // gui_load::EventFunc, kotor_input_hover::{GuiObject, Control}
#include "8cf67f2aae-m15d.h"

namespace kotor_m15d {
namespace kih = kotor_input_hover;

// Generic panel "minus"/"plus" button thunks (Ghidra names them CSWGuiSkillsCharGen::On{Minus,Plus}ButtonPushed; they
// only use the receiver's vptr). DEFINED by L15-D (unit m15d-skills). Registered by CSWGuiSkillsCharGen 006F61D2 /
// 006F61FA, CSWGuiAbilitiesCharGen 006F82B5 / 006F82CE and CSWGuiPortraitCharGen 006F9430 (L15-C).
//   0067CB40: MOV EAX,[ECX]; PUSH 1; PUSH 2Fh; CALL [EAX+3Ch]; RET 4   -> HandleInputEvent(2Fh, 1)
//   0067CB50: MOV EAX,[ECX]; PUSH 1; PUSH 30h; CALL [EAX+3Ch]; RET 4   -> HandleInputEvent(30h, 1)
struct PanelStepThunks : kih::GuiObject {
    void __thiscall OnMinusButtonPushed_0067cb40(kih::Control* control);
    void __thiscall OnPlusButtonPushed_0067cb50(kih::Control* control);
};
static_assert(sizeof(void (__thiscall PanelStepThunks::*)(kih::Control*)) == 4, "single-inheritance PMF (D107)");
inline gui_load::EventFunc event_func(void (__thiscall PanelStepThunks::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);
}
}
