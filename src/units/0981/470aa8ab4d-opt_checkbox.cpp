// lane-m1-opt-gameplay: single-owner shared leaves of the options sub-panels.
//   CSWGuiOptionsCheckbox: ctor 006E00D0, dtor 006E00F0, scalar deleting dtor 006E1640, SetExtent 006DE000,
//   Draw 006DE0E0, Load 006DE160, method table 00758258 (42 slots, read from the original .rdata).
//   CSWGuiButtonToggle::SetSelected 00685540.
//   init_control: the per-control String temporary + CSWGuiPanel::InitControl(control, &name, 1) sequence that
//   every sub-panel ctor repeats (e.g. 006E776C..99).
// Consumers: Gameplay 006E69A0, Feedback 006E1300 (PopulateOptionsListbox), AutoPause 006E76A0 (this lane) and the
// Graphics / Sound-advanced panels (lane-m1-opt-sound-gfx binds to these symbols; no second copy).
// Original: swkotor.exe GOG K1, static disassembly only.
#include "b8678b6f34-opt_common.h"

using namespace kotor_m1_opt;

void kotor_m1_opt::init_control(Panel* panel, Control* control, const char* tag) noexcept(false) {
    String name(tag);                                                              // 005E5A90
    gui_load_panel_init_control_0040b930(panel, nullptr, control, &name, 1);       // 0040B930
}                                                                                  // 005E5C20

// ======================================================================== D21 method table 00758258 (42 slots)
// Read from the original .rdata 00758258..007582FF: identical to CSWGuiButtonToggle 0073EB88 except
// +00 006E1640, +04 006DE000, +38 006DE0E0, +48 006DE160.
struct GuiM1OptCheckboxMethods {
    void* (__fastcall* s00)(Checkbox*, void*, Word);                                    // +00 006E1640
    void (__fastcall* s04)(Checkbox*, void*, const Extent*);                            // +04 006DE000
    void (__fastcall* s08)(Object*, void*, Word, Word);                                 // +08 005B5E90 void_return (RET 8)
    void (kih::Navigable::* s0c)(int);                                                  // +0C 0041A980 CSWGuiNavigable::HandleMouseOver
    Word (__fastcall* s10)(Object*, void*, Word, Word);                                 // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(Object*, void*);                                             // +14 004015A0 void_return
    void (kih::Control::* s18)();                                                       // +18 0041AB90 HandleLMouseDown
    void (kih::Control::* s1c)();                                                       // +1C 0041ABD0 HandleLMouseUp
    void (__fastcall* s20)(Object*, void*);                                             // +20 004015A0 void_return
    void (__fastcall* s24)(Object*, void*);                                             // +24 0041AC70 HandleRMouseUp
    Word (__fastcall* s28)(Object*, void*);                                             // +28 0063E7F0 return_zero
    Word (__fastcall* s2c)(Object*, void*);                                             // +2C 0063E7F0 return_zero
    Object* (__fastcall* s30)(Object*, void*);                                          // +30 00641DB0 self_return
    Word (__fastcall* s34)(Object*, void*);                                             // +34 004064A0 GetProperty0x10
    void (__fastcall* s38)(Checkbox*, void*, Word);                                     // +38 006DE0E0 CSWGuiOptionsCheckbox::Draw
    void (__fastcall* s3c)(ButtonToggle*, void*, I32, I32);                             // +3C 0041ADB0 CSWGuiButtonToggle::HandleInputEvent
    void (kih::Button::* s40)(int);                                                     // +40 00418E00 CSWGuiButton::SetActive
    kih::Control* (kih::Control::* s44)(int, int);                                      // +44 004187C0 CSWGuiControl::HitCheckMouse
    void (__fastcall* s48)(Checkbox*, void*, gui_load::Gff*, gui_load::Struct*);        // +48 006DE160 CSWGuiOptionsCheckbox::Load
    Object* (__fastcall* s4c)(Object*, void*);                                          // +4C 00641DB0 self_return
    Word (__fastcall* s50)(Object*, void*);                                             // +50 0063E7F0
    Word (__fastcall* s54)(Object*, void*);                                             // +54 0063E7F0
    Object* (__fastcall* s58)(Object*, void*);                                          // +58 00641DB0
    Object* (__fastcall* s5c)(Object*, void*);                                          // +5C 00641DB0 (as button toggle)
    Word (__fastcall* s60)(Object*, void*);                                             // +60 0063E7F0
    Word (__fastcall* s64)(Object*, void*);                                             // +64 0063E7F0
    Word (__fastcall* s68)(Object*, void*);                                             // +68 0063E7F0
    Word (__fastcall* s6c)(Object*, void*);                                             // +6C 0063E7F0
    Word (__fastcall* s70)(Object*, void*);                                             // +70 0063E7F0
    Word (__fastcall* s74)(Object*, void*);                                             // +74 0063E7F0
    Word (__fastcall* s78)(Object*, void*);                                             // +78 0063E7F0
    void (__fastcall* s7c)(Object*, void*, Word);                                       // +7C 00418960 HandleFocusChange
    int (kih::Control::* s80)();                                                        // +80 004189D0 GetIsSelectable
    void (__fastcall* s84)(Object*, void*);                                             // +84 0041AC30 HandleRMouseDown
    void (__fastcall* s88)(Object*, void*, Word);                                       // +88 00418DB0 CSWGuiButton::SetEnabled
    void (__fastcall* s8c)(Object*, void*);                                             // +8C 00418DF0 CSWGuiButton::ReSetFont
    Word (__fastcall* s90)(Object*, void*);                                             // +90 00418A90 DisplayToolTip
    void (__fastcall* s94)(Object*, void*, Word, Word);                                 // +94 005B5E90 void_return
    void (__fastcall* s98)(Object*, void*, Word, Word);                                 // +98 00418D80 CSWGuiButton::Initialize
    void (__fastcall* s9c)(Object*, void*, Word, Word, Word, Word);                     // +9C 00417970 CSWGuiButton::Initialize
    void (__fastcall* sa0)(ButtonToggle*, void*, const Extent*, ButtonToggle*);         // +A0 00418E40 CSWGuiButtonToggle::Initialize
    void (__fastcall* sa4)(ButtonToggle*, void*, const Extent*, const void*, const void*,
                           const void*, const void*, const void*);                      // +A4 00417CA0 CSWGuiButtonToggle::Initialize
};
static_assert(sizeof(GuiM1OptCheckboxMethods) == 0xA8, "00758258 slot count (42)");
static_assert(offsetof(GuiM1OptCheckboxMethods, s5c) == 0x5C && offsetof(GuiM1OptCheckboxMethods, sa4) == 0xA4, "slots");
extern "C" const GuiM1OptCheckboxMethods gui_m1_opt_checkbox_methods_00758258 = {
    &kotor_m1_opt_checkbox_deleting_destruct_006e1640,   // +00
    &kotor_m1_opt_checkbox_set_extent_006de000,          // +04
    &gui_load_slot_ret8_005b5e90,                        // +08
    &kih::Navigable::HandleMouseOver,                    // +0C
    &gui_load_slot_zero8_00409eb0,                       // +10
    &gui_load_slot_ret_004015a0,                         // +14
    &kih::Control::HandleLMouseDown,                     // +18
    &kih::Control::HandleLMouseUp,                       // +1C
    &gui_load_slot_ret_004015a0,                         // +20
    &gui_load_control_handle_rmouse_up_0041ac70,         // +24
    &gui_load_slot_zero_0063e7f0,                        // +28
    &gui_load_slot_zero_0063e7f0,                        // +2C
    &gui_load_slot_self_00641db0,                        // +30
    &gui_load_slot_prop10_004064a0,                      // +34
    &kotor_m1_opt_checkbox_draw_006de0e0,                // +38
    &gui_m48_button_toggle_handle_input_0041adb0,        // +3C
    &kih::Button::SetActive,                             // +40
    &kih::Control::HitCheckMouse,                        // +44
    &kotor_m1_opt_checkbox_load_006de160,                // +48
    &gui_load_slot_self_00641db0,                        // +4C
    &gui_load_slot_zero_0063e7f0,                        // +50
    &gui_load_slot_zero_0063e7f0,                        // +54
    &gui_load_slot_self_00641db0,                        // +58
    &gui_load_slot_self_00641db0,                        // +5C
    &gui_load_slot_zero_0063e7f0,                        // +60
    &gui_load_slot_zero_0063e7f0,                        // +64
    &gui_load_slot_zero_0063e7f0,                        // +68
    &gui_load_slot_zero_0063e7f0,                        // +6C
    &gui_load_slot_zero_0063e7f0,                        // +70
    &gui_load_slot_zero_0063e7f0,                        // +74
    &gui_load_slot_zero_0063e7f0,                        // +78
    &gui_load_control_focus_change_00418960,             // +7C
    &kih::Control::GetIsSelectable,                      // +80
    &gui_load_control_rmouse_down_0041ac30,              // +84
    &gui_load_button_set_enabled_00418db0,               // +88
    &gui_load_button_reset_font_00418df0,                // +8C
    &gui_load_control_tooltip_00418a90,                  // +90
    &gui_load_slot_ret8_005b5e90,                        // +94
    &gui_load_button_initialize_00418d80,                // +98
    &gui_load_button_initialize_00417970,                // +9C
    &gui_m48_button_toggle_initialize_00418e40,          // +A0
    &gui_m48_button_toggle_initialize_00417ca0,          // +A4
};

extern "C" {
// ======================================================================== 006E00D0 ctor (thiscall, RET)
// 006E00D3 CSWGuiButtonToggle::CSWGuiButtonToggle; 006E00D8 vptr 00758258; returns this.
Checkbox* __fastcall kotor_m1_opt_checkbox_construct_006e00d0(Checkbox* self, void*) noexcept(false) {
    gui_load_button_toggle_construct_0041ca30(self, nullptr);                      // 006E00D3
    set_vptr(self, &gui_m1_opt_checkbox_methods_00758258);                         // 006E00D8
    return self;                                                                   // 006E00DE
}
// ======================================================================== 006E00F0 dtor: JMP 0041CAD0
void __fastcall kotor_m1_opt_checkbox_destruct_006e00f0(Checkbox* self, void*) {
    gui_m48_button_toggle_destruct_0041cad0(self, nullptr);                        // 006E00F0
}
// ======================================================================== 006E1640 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_checkbox_deleting_destruct_006e1640(Checkbox* self, void*, Word flags) {
    kotor_m1_opt_checkbox_destruct_006e00f0(self, nullptr);                        // 006E1643
    if (flags & 1u) live_free(self);                                               // 006E1648..55 (_free 006FA390)
    return self;                                                                   // 006E1658
}
// ======================================================================== 006DE000 SetExtent(extent) (RET 4)
// Box extent {x, y + (h - 19h)/2 + 2, 19h, 19h} (signed halving: cdq/sub/sar 006DE020..2C) to the four borders
// (+6C, +E0, +240, +1CC order 006DE03E..7A, vslot +04), text extent {x + 1Eh, y, w - 1Eh, h} (006DE07D..AD,
// text +154 vslot +04), then this->extent = *extent (006DE0B0..C6).
void __fastcall kotor_m1_opt_checkbox_set_extent_006de000(Checkbox* self, void*, const Extent* extent) {
    struct DrawableExtentView { const void* s00; void (__fastcall* set_extent)(void*, void*, const Extent*); };
    Extent box;
    box.left = extent->left;                                                       // 006DE008..0A
    box.width = 0x19;                                                              // 006DE018
    box.height = 0x19;                                                             // 006DE01C
    box.top = extent->top + (extent->height - 0x19) / 2 + 2;                       // 006DE00E..32
    vt<DrawableExtentView>(&self->border)->set_extent(&self->border, nullptr, &box);                       // 006DE029..3E (+6C)
    vt<DrawableExtentView>(&self->hilight)->set_extent(&self->hilight, nullptr, &box);                     // 006DE041..52 (+E0)
    vt<DrawableExtentView>(&self->hilight_selected)->set_extent(&self->hilight_selected, nullptr, &box);   // 006DE055..66 (+240)
    vt<DrawableExtentView>(&self->selected)->set_extent(&self->selected, nullptr, &box);                   // 006DE069..7A (+1CC)
    Extent text;
    text.left = extent->left + 0x1E;                                               // 006DE091
    text.top = extent->top;                                                        // 006DE081..84
    text.width = extent->width - 0x1E;                                             // 006DE08E
    text.height = extent->height;                                                  // 006DE08B..94
    vt<DrawableExtentView>(&self->text)->set_extent(&self->text, nullptr, &text); // 006DE09C..AD (+154)
    self->extent = *extent;                                                        // 006DE0B0..C6
}
// ======================================================================== 006DE0E0 Draw(delta) (RET 4)
// flags bit0 (active): text color 007A23C0 + text flags |= 40h; else bit3 clear: color 007A2408; else color
// 007A23B4 + text flags &= ~40h; then CSWGuiButtonToggle::Draw 00417B10.
void __fastcall kotor_m1_opt_checkbox_draw_006de0e0(Checkbox* self, void*, Word delta) {
    unsigned char* const text_params = reinterpret_cast<unsigned char*>(self) + 0x16C;   // +154 text, +18 params
    const Word flags = self->flags;                                                // 006DE0E3
    if (flags & 1u) {                                                              // 006DE0E6..E9
        gui_load_text_params_set_color_00414e10(text_params, nullptr, &borrowed_enter_color_007a23c0);   // 006DE0EB..F8
        set_word(text_params, 0x38, word_at(text_params, 0x38) | 0x40u);           // 006DE0FD
    } else if ((flags & 8u) == 0) {                                                // 006DE112..14
        gui_load_text_params_set_color_00414e10(text_params, nullptr, &kotor_m1_color_007a2408);         // 006DE116..21
    } else {
        gui_load_text_params_set_color_00414e10(text_params, nullptr, &borrowed_exit_color_007a23b4);    // 006DE137..44
        set_word(text_params, 0x38, word_at(text_params, 0x38) & 0xFFFFFFBFu);     // 006DE149
    }
    gui_m48_button_toggle_draw_00417b10(self, nullptr, delta);                     // 006DE101..08 / 126..2D / 14D..54
}
// ======================================================================== 006DE160 Load(gff, struct) (RET 8)
// CSWGuiButtonToggle::Load 0041CB40, then the border colours: border (+90) and selected (+1F0) = 007A23B4,
// hilight (+104) and hilight-selected (+264) = 007A23C0 (border params +14, colour +10).
void __fastcall kotor_m1_opt_checkbox_load_006de160(Checkbox* self, void*, gui_load::Gff* gff, gui_load::Struct* s) noexcept(false) {
    gui_m48_button_toggle_load_0041cb40(self, nullptr, gff, s);                    // 006DE160..6F
    unsigned char* const bytes = reinterpret_cast<unsigned char*>(self);
    std::memcpy(bytes + 0x90, borrowed_exit_color_007a23b4.words, 12);            // 006DE174..8D
    std::memcpy(bytes + 0x1F0, borrowed_exit_color_007a23b4.words, 12);           // 006DE192..AE
    std::memcpy(bytes + 0x104, borrowed_enter_color_007a23c0.words, 12);          // 006DE1B1..CE
    std::memcpy(bytes + 0x264, borrowed_enter_color_007a23c0.words, 12);          // 006DE1D1..ED
}
// ======================================================================== 00685540 CSWGuiButtonToggle::SetSelected
// thiscall RET 4: [+1C8] = (([+1C8] ^ v) & 1) ^ [+1C8].
void __fastcall kotor_m1_opt_toggle_set_selected_00685540(ButtonToggle* self, void*, Word selected) {
    const Word flags = self->toggle_flags;                                         // 00685540 / 0068554A
    self->toggle_flags = ((flags ^ selected) & 1u) ^ flags;                        // 00685546..55
}
}
