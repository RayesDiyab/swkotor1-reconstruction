#pragma once
// lane-m1-opt-gameplay: typed 27-slot CSWGuiPanel-family method table shape (D21) shared by the AutoPause
// (00758EE0) and Feedback (007581E8) tables. Both were read from the original .rdata and are identical to
// CSWGuiPanel 0073E010 except +00 (scalar deleting destructor) and +3C (HandleInputEvent).
#include "b8678b6f34-opt_common.h"

namespace kotor_m1_opt {
template<class Self>
struct PanelMethods27 {
    void* (__fastcall* s00)(Self*, void*, Word);                               // +00 scalar deleting destructor
    void (__fastcall* s04)(Object*, void*, const Extent*);                     // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(Object*, void*, Object*, Word);                     // +08 0040A630 CSWGuiPanel::SetActiveControl
    void (__fastcall* s0c)(Object*, void*, Word);                              // +0C 0060E760 void_return [RET 4]
    Word (__fastcall* s10)(Object*, void*, Word, Word);                        // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(Object*, void*);                                    // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                    // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                    // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                    // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                    // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                 // +28 00641DB0 self_return
    Word (__fastcall* s2c)(Object*, void*);                                    // +2C 0063E7F0 return_zero
    Word (__fastcall* s30)(Object*, void*);                                    // +30 0063E7F0 return_zero
    void (__fastcall* s34)(Panel*, void*, Word);                               // +34 0040B760 CSWGuiPanel::Draw
    void (__fastcall* s38)(Object*, void*, Word);                              // +38 0060E760 void_return [RET 4]
    void (__fastcall* s3c)(Object*, void*, I32, I32);                          // +3C HandleInputEvent (per class)
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                       // +40 0040B690 CSWGuiPanel::HitCheckMouse
    Object* (__fastcall* s44)(Object*, void*);                                 // +44 0040B630 CSWGuiPanel::GetActiveControl
    void (__fastcall* s48)(Panel*, void*);                                     // +48 0040B870 CSWGuiPanel::OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                     // +4C 0040C170 CSWGuiPanel::OnPanelRemoved
    void (__fastcall* s50)(Object*, void*, Word);                              // +50 0040B640 OnAButtonPressed
    void (__fastcall* s54)(Object*, void*, Word);                              // +54 0040B650 OnBButtonPressed
    void (__fastcall* s58)(Object*, void*, Word);                              // +58 0040B660 OnXButtonPressed
    void (__fastcall* s5c)(Object*, void*, Word);                              // +5C 0040B670 OnYButtonPressed
    void (__fastcall* s60)(Object*, void*, Word);                              // +60 0040B680 OnBlackButtonPressed
    String* (__fastcall* s64)(Panel*, void*, String*);                         // +64 0040A900 GetFullScreenBG
    void (__fastcall* s68)(Panel*, void*);                                     // +68 0040A9C0 ResetFont
};
}
// Common initializer body for the 25 inherited CSWGuiPanel slots (+04..+38, +40..+68); the two class slots are
// supplied by the caller. Expands to a braced aggregate initializer (constant initialization, no dynamic init).
#define KOTOR_M1_OPT_PANEL_METHODS27(DTOR, INPUT) {                                                        \
    &DTOR, &gui_load_panel_set_extent_0040a5a0, &gui_load_panel_set_active_control_0040a630,                \
    &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0, &gui_load_slot_ret_004015a0,               \
    &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,                  \
    &gui_load_slot_ret_004015a0, &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0,                \
    &gui_load_slot_zero_0063e7f0, &kotor_panel_draw_0040B760, &gui_load_slot_ret4_0060e760, &INPUT,         \
    &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630,                            \
    &gui_load_panel_on_added_0040b870, &gui_load_panel_on_removed_0040c170, &gui_load_panel_button_a_0040b640, \
    &gui_load_panel_button_b_0040b650, &gui_load_panel_button_x_0040b660, &gui_load_panel_button_y_0040b670,   \
    &gui_load_panel_button_black_0040b680, &gui_load_panel_full_screen_bg_0040a900,                         \
    &gui_load_panel_reset_font_0040a9c0 }
