#pragma once
// lane-m1-opt-gameplay FALLBACK unit (drop if lane-m1-opt-sound-gfx provides these addresses; composer keeps one):
// two COMDAT-folded bodies shared by CSWGuiOptionsGraphics / CSWGuiOptionsGraphicsAdvanced and CSWGuiOptionsMouse.
//   006DED90 CSWGuiOptionsGraphics::OnExitSlider (event 1 on Graphics/Sound sliders; Mouse slider 006E28C2)
//   006DF390 CSWGuiOptionsGraphicsAdvanced::SetDescription (also reached by CSWGuiOptionsMouse 006DE214 / 006DFE68)
#include "b8678b6f34-opt_common.h"

namespace kotor_m1_opt {
// Control slot +64: 00641DB0 self_return on CSWGuiSlider (table 0073E9D0 +64), "as slider" (006DE21D, 006DED96).
struct SliderCastView { const void* s00_60[0x19]; Slider* (__fastcall* as_slider)(Object*, void*); };
static_assert(offsetof(SliderCastView, as_slider) == 0x64, "+64");
inline Slider* as_slider(Object* object) { return vt<SliderCastView>(object)->as_slider(object, nullptr); }
// Thumb image params flags word: slider +164 image, +28 params, +24 flags (= slider +1B0; 006DE220..2E).
constexpr unsigned kThumbParamsFlags = 0x1B0;

struct FoldedHandlers : kih::GuiObject {
    void __thiscall OnExitSlider_006ded90(kih::Control* control);   // ECX ignored (006DED90 reloads ECX from the stack)
};
}
extern "C" {
// 006DF390 SetDescription(strref) (thiscall RET 4) on a panel whose description label is at +64 and LB_DESC at +2E4.
void __fastcall kotor_m1_opt_graphics_adv_set_description_006df390(gui_load::Panel* self, void*, gui_load::Word strref) noexcept(false);
}
