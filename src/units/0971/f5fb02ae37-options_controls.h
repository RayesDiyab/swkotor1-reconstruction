#pragma once
// lane-m1-opt-sound-gfx (Milestone 1, Options SOUND / GRAPHICS sub-panels): shared control layer.
//   * gui_lb::Slider (CSWGuiSlider, 0x1B8, table 0073E9D0) and gui_lb::ButtonToggle (CSWGuiButtonToggle, 0x2B4,
//     table 0073EB88): the existing m48b-gui-controls classes (bodies 0041BAA0/0041BB40/00417EE0/0041CA30/0041CAD0/...
//     already linked by M54). Their definitions live in m48/b/gui/src/gui_m48_controls.cpp (a .cpp, no header), so
//     they are repeated here TOKEN-IDENTICAL (same namespace, same members, ODR-identical definition); a composer may
//     hoist both copies into one shared header without any change of layout or symbol.
//   * kotor_m1_options::OptionsCheckbox (CSWGuiOptionsCheckbox, table 00758258): a CSWGuiButtonToggle whose vptr is
//     overwritten by the owning panel ctor (006E20E8 / 006E2F3A / 006E2F52 / 006E1758 / 006E1770 / 006E1788).
//     Bodies and table are owned by lane-m1-opt-gameplay (single owner); this lane only stores the table address.
// Original: swkotor.exe GOG K1 (sha256 9c10e045...91435, image base 0x400000), static disassembly only.
// Conventions as gui-load / lane-m1-options: D1 fastcall veneers for thiscall entries, D21 typed constant method
// tables, D24 object views of tables, D47 member callbacks (single-inheritance PMFs, one code word).
#include "3370aef598-gui_objects.h"
#include "ce2f5dc6ef-mainmenu.h"
#include "19aa661562-m1_shared.h"
#include <cstddef>
#include <cstdint>

// ======================================================================== m48b-gui-controls classes (token-identical)
namespace gui_lb {
struct Slider : Navigable {
    I32 hit;                   // +6C HitCheckSlider result (004191F9)
    I32 max_value;             // +70 = 0 (0041BAFC)
    I32 cur_value;             // +74 = 0 (0041BB03)
    unsigned char sound;       // +78 = 1 (0041BB0A); PlayGuiSound id (0041AEAD)
    unsigned char pad79[3];
    Border border;             // +7C (0041BAC2) "BORDER"
    Border hilight;            // +F0 (0041BAD8) "HILIGHT"
    Image thumb;               // +164 (0041BAE8) "THUMB"
};
static_assert(offsetof(Slider, border) == 0x7C && offsetof(Slider, hilight) == 0xF0 && offsetof(Slider, thumb) == 0x164 &&
              sizeof(Slider) == 0x1B8, "slider");
struct ButtonToggle : gui_load::Button {
    Word event_id;             // +1C4 = 27h (0041CA8E)
    Word toggle_flags;         // +1C8 bit0 selected (0041CA7B..88)
    Border selected;           // +1CC (0041CA52) "SELECTED"
    Border hilight_selected;   // +240 (0041CA6B) "HILIGHTSELECTED"
};
static_assert(offsetof(ButtonToggle, selected) == 0x1CC && offsetof(ButtonToggle, hilight_selected) == 0x240 &&
              sizeof(ButtonToggle) == 0x2B4, "button toggle");
}

struct MenuColor;   // input-hover donor/matching.h: { unsigned long words[3]; }

extern "C" {
// ---- m48b-gui-controls (existing M54 bodies; declarations repeated exactly as their owner spells them)
gui_load::Control* __fastcall gui_load_slider_construct_0041baa0(void*, void*) noexcept(false);
gui_load::Control* __fastcall gui_load_button_toggle_construct_0041ca30(void*, void*) noexcept(false);
void __fastcall gui_m48_slider_destruct_0041bb40(gui_lb::Slider*, void*);
void __fastcall gui_m48_slider_set_cur_00417ee0(gui_lb::Slider*, void*, gui_lb::I32);
void __fastcall gui_m48_button_toggle_destruct_0041cad0(gui_lb::ButtonToggle*, void*);
void __fastcall gui_m48_button_toggle_draw_00417b10(gui_lb::ButtonToggle*, void*, gui_load::Word);
void __fastcall gui_m48_button_toggle_handle_input_0041adb0(gui_lb::ButtonToggle*, void*, gui_lb::I32, gui_lb::I32);
void __fastcall gui_m48_button_toggle_load_0041cb40(gui_lb::ButtonToggle*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
void __fastcall gui_m48_button_toggle_initialize_00418e40(gui_lb::ButtonToggle*, void*, const gui_load::Extent*, gui_lb::ButtonToggle*);
void __fastcall gui_m48_button_toggle_initialize_00417ca0(gui_lb::ButtonToggle*, void*, const gui_load::Extent*, const void*, const void*,
                                                          const void*, const void*, const void*) noexcept(false);
void __fastcall gui_m48_panel_center_0040a600(gui_load::Panel*, void*);                       // CSWGuiPanel::CenterPanel
// ---- core47 / gui-load / r4-lowers owners
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const MenuColor* color);  // 00414E10 (saveload view)
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, gui_load::Control** controls, gui_lb::I32 count,
    gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);   // 0041C2C0
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                        // PopModalPanel 0040BE00
void* __fastcall kotor_fastcall_005ed700(void* client_app, void*);                                 // CClientExoApp::GetClientOptions
extern const MenuColor borrowed_exit_color_007a23b4;                                               // .data 007A23B4 (input-cells)
extern const MenuColor borrowed_enter_color_007a23c0;                                              // .data 007A23C0 (input-cells)
extern const MenuColor kotor_m1_color_007a2408;                                                    // .data 007A2408 (m1-saveload)
}

namespace kotor_m1_options {
// CSWGuiOptionsCheckbox: no own fields (every access is inside the CSWGuiButtonToggle extent).
struct OptionsCheckbox : gui_lb::ButtonToggle {};
static_assert(sizeof(OptionsCheckbox) == 0x2B4, "CSWGuiOptionsCheckbox = CSWGuiButtonToggle extent");
}

extern "C" {
// ---- CSWGuiOptionsCheckbox family: single owner lane-m1-opt-gameplay (coordinator ruling; unit m1-opt-checkbox,
// src/opt_checkbox.cpp: 006E00D0 / 006E00F0 / 006E1640 / 006DE000 / 006DE0E0 / 006DE160 / table 00758258 / 00685540).
// The owning panel ctors here inline the checkbox ctor exactly as the original does (0041CA30 + vptr store of
// 00758258) and destroy the members with ~CSWGuiButtonToggle 0041CAD0, so only the table address is referenced.
extern const struct GuiM1OptCheckboxMethods gui_m1_opt_checkbox_methods_00758258;
// ---- CClientOptions leaves (authored in options_sound.cpp / options_graphics.cpp; thiscall -> D1 fastcall)
void __fastcall kotor_m1_client_options_default_sound_0061d540(void* options, void*);                     // 0061D540
void __fastcall kotor_m1_client_options_default_advanced_sound_0061d550(void* options, void*);            // 0061D550
void __fastcall kotor_m1_client_options_default_advanced_graphics_0061d520(void* options, void*);         // 0061D520
void __fastcall kotor_m1_client_options_set_shadows_0061d6b0(void* options, void*, std::uint32_t enable); // 0061D6B0
void __fastcall kotor_m1_client_options_set_soft_shadows_0061d6d0(void* options, void*, std::uint32_t enable); // 0061D6D0
void __fastcall kotor_m1_client_options_set_grass_0061d6f0(void* options, void*, std::uint32_t enable);   // 0061D6F0
void __fastcall kotor_m1_client_options_set_tex_qual_0061d730(void* options, void*, std::uint32_t quality); // 0061D730
void __fastcall kotor_m1_client_options_default_graphics_0061d9a0(void* options, void*);                  // 0061D9A0
// ---- CExoSound leaves (authored in options_sound.cpp; lane-sound family)
void __fastcall kotor_m1_sound_reinitialize_005d63f0(void* sound, void*, std::uint32_t eax_level, std::uint32_t force_software); // 005D63F0 (EAX not meaningful)
std::uint32_t __fastcall kotor_m1_sound_best_eax_005d6460(void* sound, void*);                             // 005D6460
}

namespace kotor_m1_options {
namespace kih = kotor_input_hover;
// Object views of the ORIGINAL tables' called slots (field types as the owning tables declare them; D24).
struct ControlSelf5CView { const void* s00_58[0x17]; gui_load::Object* (__fastcall* slot5c)(gui_load::Object*, void*); };   // toggle +5C 00641DB0
struct ControlSelf64View { const void* s00_60[0x19]; gui_load::Object* (__fastcall* slot64)(gui_load::Object*, void*); };   // slider +64 00641DB0
struct ControlEnable88View { const void* s00_84[0x22]; void (__fastcall* set_enabled88)(gui_load::Object*, void*, gui_load::Word); }; // button +88 00418DB0
struct LabelInit98View { const void* s00_94[0x26]; void (__fastcall* initialize98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word, gui_load::Word); }; // label +98 00418CD0
struct ButtonInit98View { const void* s00_94[0x26]; void (__fastcall* initialize98)(gui_load::Object*, void*, gui_load::Word, gui_load::Word); };  // button +98 00418D80
struct ListBoxSelectA4View { const void* s00_a0[0x29]; void (__fastcall* select_left_a4)(gui_lb::ListBox*, void*, gui_load::Word); }; // listbox +A4 0041B120
struct ControlSelf58View { const void* s00_54[0x16]; gui_load::Object* (__fastcall* slot58)(gui_load::Object*, void*); };   // button +58 00641DB0
struct TextIdeal08View { const void* s00; const void* s04; gui_load::Word (__fastcall* ideal_height)(gui_load::Text*, void*); }; // text +08 00414EB0
struct PanelButtonB54View { const void* s00_50[0x15]; void (__fastcall* button_b54)(gui_load::Object*, void*, gui_load::Word); }; // panel +54 0040B650
// CSWGuiOptionsGraphics::OnExitSlider 006DED90 (registered by both the Sound 006E3A0C.. and Graphics 006E32AC panels):
// any panel ECX, one stack word (the slider); single owner options_sound.cpp.
struct OptionsSliderHandlers : kih::GuiObject {
    void __thiscall OnExitSlider_006ded90(kih::Control* control);
};
// Panel method tables (27 slots; identical to CSWGuiPanel 0073E010 except +00 and +3C), one typed struct per class.
template<class Self> struct OptionsPanelMethods {
    void* (__fastcall* s00)(Self*, void*, gui_load::Word);                                     // +00 deleting dtor
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);                 // +04 0040A5A0
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);       // +08 0040A630
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);                          // +0C 0060E760
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word); // +10 00409EB0
    void (__fastcall* s14)(gui_load::Object*, void*);                                          // +14 004015A0
    void (__fastcall* s18)(gui_load::Object*, void*);                                          // +18 004015A0
    void (__fastcall* s1c)(gui_load::Object*, void*);                                          // +1C 004015A0
    void (__fastcall* s20)(gui_load::Object*, void*);                                          // +20 004015A0
    void (__fastcall* s24)(gui_load::Object*, void*);                                          // +24 004015A0
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);                             // +28 00641DB0
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);                                // +2C 0063E7F0
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);                                // +30 0063E7F0
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);                           // +34 0040B760
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);                          // +38 0060E760
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);            // +3C HandleInputEvent
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32); // +40 0040B690
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);                             // +44 0040B630
    void (__fastcall* s48)(gui_load::Panel*, void*);                                           // +48 0040B870
    void (__fastcall* s4c)(gui_load::Panel*, void*);                                           // +4C 0040C170
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);                          // +50 0040B640
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);                          // +54 0040B650
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);                          // +58 0040B660
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);                          // +5C 0040B670
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);                          // +60 0040B680
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);           // +64 0040A900
    void (__fastcall* s68)(gui_load::Panel*, void*);                                           // +68 0040A9C0
};
static_assert(sizeof(OptionsPanelMethods<gui_load::Panel>) == 0x6C, "27 panel slots");

static_assert(offsetof(ControlSelf5CView, slot5c) == 0x5C && offsetof(ControlSelf64View, slot64) == 0x64 &&
              offsetof(ControlEnable88View, set_enabled88) == 0x88 && offsetof(LabelInit98View, initialize98) == 0x98 &&
              offsetof(ButtonInit98View, initialize98) == 0x98 && offsetof(ListBoxSelectA4View, select_left_a4) == 0xA4 &&
              offsetof(ControlSelf58View, slot58) == 0x58 && offsetof(TextIdeal08View, ideal_height) == 0x08 &&
              offsetof(PanelButtonB54View, button_b54) == 0x54, "called slot offsets");
}

// Aggregate initialisers of the 25 shared CSWGuiPanel slots (constant initialisation; addresses read from the original
// tables 007587C0 / 00758550 / 007586F8 / 00758348 / 007584A0, all equal to 0073E010 outside +00 and +3C).
#define KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38                                                                    \
    &gui_load_panel_set_extent_0040a5a0, &gui_load_panel_set_active_control_0040a630,                         \
    &gui_load_slot_ret4_0060e760, &gui_load_slot_zero8_00409eb0, &gui_load_slot_ret_004015a0,                 \
    &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0, &gui_load_slot_ret_004015a0,                    \
    &gui_load_slot_ret_004015a0, &gui_load_slot_self_00641db0, &gui_load_slot_zero_0063e7f0,                  \
    &gui_load_slot_zero_0063e7f0, &kotor_panel_draw_0040B760, &gui_load_slot_ret4_0060e760
#define KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68                                                                    \
    &gui_load_panel_hit_check_0040b690, &gui_load_panel_active_control_0040b630,                              \
    &gui_load_panel_on_added_0040b870, &gui_load_panel_on_removed_0040c170, &gui_load_panel_button_a_0040b640, \
    &gui_load_panel_button_b_0040b650, &gui_load_panel_button_x_0040b660, &gui_load_panel_button_y_0040b670,  \
    &gui_load_panel_button_black_0040b680, &gui_load_panel_full_screen_bg_0040a900,                           \
    &gui_load_panel_reset_font_0040a9c0
