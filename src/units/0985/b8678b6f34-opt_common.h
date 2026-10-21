#pragma once
// lane-m1-opt-gameplay (Milestone 1 options sub-panels): shared declarations for the Gameplay
// (CSWGuiInGameGameplay 006E69A0, incl. CSWGuiOptionsMouse 006E2600 and CSWGuiInGameOptKeyMappings 006EDC90),
// Feedback (CSWGuiOptionsFeedback 006E2A70) and AutoPause (CSWGuiInGameAutoPause 006E76A0) sub-panels, and the
// single-owner shared leaves they use: CSWGuiOptionsCheckbox (table 00758258) and CSWGuiButtonToggle::SetSelected
// 00685540. Original: swkotor.exe GOG K1, static disassembly only; every body cites instruction addresses.
// Conventions (DECISIONS): D1 extern "C" __fastcall(Self*, void* unused_edx, stack args...) veneers for __thiscall
// entries (callee cleanup identical); D21 typed constant method tables; D24 object views of method tables
// (vt<V>(object)); D47 member event callbacks (single-inheritance PMFs, static_cast to the base PMF);
// D30-FINAL (no float arithmetic in these panels except where cited; EH unwind emitted with C++ try/catch).
#include "3370aef598-gui_objects.h"   // gui_load (Object/Control/Panel/Label/Button views), gui_lb (ListBox/Image, vt), kih
#include "ce2f5dc6ef-mainmenu.h"      // AddPanel 0040BC70 (kotor_fastcall_0040bc70)
#include "19aa661562-m1_shared.h"     // 00624BB0 OnBButtonPressed thunk (lane-m1-options L0 single owner)
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace kotor_m1_opt {
namespace kih = kotor_input_hover;
using gui_load::Word;
using gui_load::I32;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Navigable;
using gui_load::Label;
using gui_load::Button;
using gui_load::Border;
using gui_load::Extent;
using gui_load::String;
using gui_load::Ref;
using gui_lb::ListBox;
using gui_lb::vt;
using gui_lb::set_vptr;

// ---- raw accessors (memcpy, no puns)
inline Word word_at(const void* p, unsigned off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void set_word(void* p, unsigned off, Word v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline void* ptr_at(const void* p, unsigned off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline unsigned char byte_at(const void* p, unsigned off) { return static_cast<const unsigned char*>(p)[off]; }
inline void set_byte(void* p, unsigned off, unsigned char v) { static_cast<unsigned char*>(p)[off] = v; }
inline Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }

// CSWGuiButtonToggle view (0x2B4; ctor 0041CA30, table 0073EB88); layout identical to the m48b-gui-controls
// owner (gui_lb::ButtonToggle in gui_m48_controls.cpp: 0041CA52/0041CA6B/0041CA7B..8E).
struct ButtonToggle : Button {
    Word event_id;              // +1C4 = 27h (0041CA8E)
    Word toggle_flags;          // +1C8 bit0 selected (0041CA7B..88; 00685540; 006E72AE..C1)
    Border selected;            // +1CC "SELECTED"
    Border hilight_selected;    // +240 "HILIGHTSELECTED"
};
static_assert(offsetof(ButtonToggle, toggle_flags) == 0x1C8 && offsetof(ButtonToggle, selected) == 0x1CC &&
              offsetof(ButtonToggle, hilight_selected) == 0x240 && sizeof(ButtonToggle) == 0x2B4, "button toggle");
// CSWGuiOptionsCheckbox (0x2B4; ctor 006E00D0 = ButtonToggle ctor + vptr 00758258; no own members).
struct Checkbox : ButtonToggle {};
static_assert(sizeof(Checkbox) == 0x2B4, "options checkbox");
// CSWGuiSlider view (0x1B8; ctor 0041BAA0, table 0073E9D0); layout identical to the m48b-gui-controls owner.
struct Slider : Navigable {
    I32 hit;                    // +6C
    I32 max_value;              // +70 (0041BAFC)
    I32 cur_value;              // +74 (0041BB03; SetCurValue 00417EE0)
    unsigned char sound;        // +78 (0041BB0A)
    unsigned char pad79[3];
    Border border;              // +7C
    Border hilight;             // +F0
    gui_lb::Image thumb;        // +164
};
static_assert(offsetof(Slider, cur_value) == 0x74 && offsetof(Slider, border) == 0x7C &&
              offsetof(Slider, thumb) == 0x164 && sizeof(Slider) == 0x1B8, "slider");

// ---- virtual-call views (D24): only the slots called by these panels are typed.
// Control-family slot +5C: 00641DB0 self_return on CSWGuiButtonToggle / CSWGuiOptionsCheckbox (00758258 +5C),
// 0063E7F0 return_zero on plain buttons/labels; the option panels use it as "as button toggle"
// (006E72AB, 006DE49B, 006E7521).
struct ToggleCastView { const void* s00_58[0x17]; ButtonToggle* (__fastcall* as_toggle)(Object*, void*); };
static_assert(offsetof(ToggleCastView, as_toggle) == 0x5C, "+5C");
inline ButtonToggle* as_toggle(Object* object) { return vt<ToggleCastView>(object)->as_toggle(object, nullptr); }
// CSWGuiLabel slot +98 Initialize (00418CD0; 3 stack words, RET 0Ch), as typed in gui_vtables / options_main.
struct LabelInitView { const void* s00_94[0x26]; void (__fastcall* initialize98)(Object*, void*, Word, Word, Word); };
static_assert(offsetof(LabelInitView, initialize98) == 0x98, "label +98");
// CSWGuiText slot +08 GetIdealHeight (00414EB0).
struct TextIdealView { const void* s00; const void* s04; Word (__fastcall* ideal_height)(gui_load::Text*, void*); };
static_assert(offsetof(TextIdealView, ideal_height) == 0x08, "text +08");
// Control slot +3C HandleInputEvent (event, value) RET 8.
struct InputView { const void* s00_38[0x0F]; void (__fastcall* handle_input_event)(Object*, void*, I32, I32); };
static_assert(offsetof(InputView, handle_input_event) == 0x3C, "+3C");

// ---- application cells
// CAppManager* 007A39FC (M40 owner kotor_menu_window_cells::manager_007a39fc): +4 CClientExoApp*, +8 CServerExoApp*.
inline void* client_app() { return ptr_at(kotor_menu_window_cells::manager_007a39fc, 4); }
inline void* server_app() { return ptr_at(kotor_menu_window_cells::manager_007a39fc, 8); }
// CClientExoApp::GetClientOptions 005ED700 (M41 provider kotor_menu20::ClientApp::get_options_005ed700).
inline void* client_options() { return reinterpret_cast<kotor_menu20::ClientApp*>(client_app())->get_options_005ed700(); }
inline kih::Manager* manager_of(const Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); }  // [this+18h]
inline kih::GuiObject* event_target(void* panel) { return reinterpret_cast<kih::GuiObject*>(panel); }

// ---- shared helpers authored in opt_checkbox.cpp
// String temporary + CSWGuiPanel::InitControl(control, &name, 1) (005E5A90 / 0040B930 / 005E5C20).
void init_control(Panel* panel, Control* control, const char* tag) noexcept(false);
}

extern "C" {
// ======================================================================== authored by this lane
// ---- CSWGuiOptionsCheckbox (opt_checkbox.cpp)
kotor_m1_opt::Checkbox* __fastcall kotor_m1_opt_checkbox_construct_006e00d0(kotor_m1_opt::Checkbox*, void*) noexcept(false);
void __fastcall kotor_m1_opt_checkbox_destruct_006e00f0(kotor_m1_opt::Checkbox*, void*);
void* __fastcall kotor_m1_opt_checkbox_deleting_destruct_006e1640(kotor_m1_opt::Checkbox*, void*, gui_load::Word);
void __fastcall kotor_m1_opt_checkbox_set_extent_006de000(kotor_m1_opt::Checkbox*, void*, const gui_load::Extent*);
void __fastcall kotor_m1_opt_checkbox_draw_006de0e0(kotor_m1_opt::Checkbox*, void*, gui_load::Word);
void __fastcall kotor_m1_opt_checkbox_load_006de160(kotor_m1_opt::Checkbox*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
extern const struct GuiM1OptCheckboxMethods gui_m1_opt_checkbox_methods_00758258;
// ---- CSWGuiButtonToggle::SetSelected 00685540 (opt_checkbox.cpp)
void __fastcall kotor_m1_opt_toggle_set_selected_00685540(kotor_m1_opt::ButtonToggle*, void*, gui_load::Word selected);
// ---- sub-panel constructors called by the CSWGuiOptionsMain handlers (patch to options_main.cpp)
gui_load::Panel* __fastcall kotor_m1_opt_gameplay_construct_006e69a0(void* self, void*, gui_load::Object* manager) noexcept(false);
gui_load::Panel* __fastcall kotor_m1_opt_feedback_construct_006e2a70(void* self, void*, gui_load::Object* manager) noexcept(false);
gui_load::Panel* __fastcall kotor_m1_opt_autopause_construct_006e76a0(void* self, void*, gui_load::Object* manager) noexcept(false);
// ---- sub-panels opened from the Gameplay panel (opt_mouse.cpp / opt_keymap.cpp)
gui_load::Panel* __fastcall kotor_m1_opt_mouse_construct_006e2600(void* self, void*, gui_load::Object* manager) noexcept(false);
gui_load::Panel* __fastcall kotor_m1_opt_keymap_construct_006edc90(void* self, void*, gui_load::Object* manager) noexcept(false);

// ======================================================================== M54 providers (functional.map 20261007T195950859939Z)
// m48b-gui-controls.obj (CSWGuiButtonToggle / CSWGuiSlider; declared with this lane's identical-layout views)
gui_load::Control* __fastcall gui_load_button_toggle_construct_0041ca30(void* memory, void*) noexcept(false);
void __fastcall gui_m48_button_toggle_destruct_0041cad0(kotor_m1_opt::ButtonToggle*, void*);
void __fastcall gui_m48_button_toggle_draw_00417b10(kotor_m1_opt::ButtonToggle*, void*, gui_load::Word);
void __fastcall gui_m48_button_toggle_handle_input_0041adb0(kotor_m1_opt::ButtonToggle*, void*, gui_load::I32, gui_load::I32);
void __fastcall gui_m48_button_toggle_load_0041cb40(kotor_m1_opt::ButtonToggle*, void*, gui_load::Gff*, gui_load::Struct*) noexcept(false);
void __fastcall gui_m48_button_toggle_initialize_00418e40(kotor_m1_opt::ButtonToggle*, void*, const gui_load::Extent*, kotor_m1_opt::ButtonToggle*);
void __fastcall gui_m48_button_toggle_initialize_00417ca0(kotor_m1_opt::ButtonToggle*, void*, const gui_load::Extent*, const void*, const void*,
                                                          const void*, const void*, const void*) noexcept(false);
gui_load::Control* __fastcall gui_load_slider_construct_0041baa0(void* memory, void*) noexcept(false);
void __fastcall gui_m48_slider_destruct_0041bb40(kotor_m1_opt::Slider*, void*);
void __fastcall gui_m48_slider_set_cur_00417ee0(kotor_m1_opt::Slider*, void*, gui_load::I32);
void __fastcall gui_m48_listbox_add_controls_0041c2c0(gui_lb::ListBox*, void*, gui_load::Control** controls, gui_load::I32 count,
    gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false);                   // 0041C2C0
// m48t-f-warp-mainmenu-warp.obj
gui_load::Control* __fastcall kotor_fastcall_0041a5a0(gui_lb::ListBox*, void*, gui_load::I32 index);           // GetControl 0041A5A0
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox*, void*, void* list, gui_load::Word, gui_load::Word,
                                        gui_load::Word) noexcept(false);                                       // AddControls(list,..) 0041C1D0
// core47 / gui-load / r4-lowers
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const MenuColor* color);              // 00414E10 (core47)
// .data color cells: 007A23B4 / 007A23C0 (input-cells owner, donor/matching.h), 007A2408 (m1-saveload owner)
extern const MenuColor kotor_m1_color_007a2408;
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word, gui_load::Word, gui_load::Word);
gui_load::I32 __fastcall gui_load_text_font_height_00414ee0(gui_load::Text*, void*);                            // 00414EE0
void __fastcall gui_load_listbox_clear_items_00419a90(gui_lb::ListBox*, void*);                                 // 00419A90
void __fastcall gui_load_listbox_set_active_control_0041c160(gui_lb::ListBox*, void*, gui_load::Control*, gui_load::Word);
void __fastcall gui_load_listbox_destruct_0041b060(gui_lb::ListBox*, void*);
gui_lb::ListBox* __fastcall gui_load_listbox_construct_0041be40(gui_lb::ListBox*, void*) noexcept(false);
void* __fastcall kotor_fastcall_0040be00(void* gui, void*);                                                    // PopModalPanel 0040BE00
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
}
// M40 cell owner of 007A39FC is declared by window_shared_cells.h (via gui_objects.h).
