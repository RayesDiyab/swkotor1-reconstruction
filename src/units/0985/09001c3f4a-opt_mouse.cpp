// lane-m1-opt-gameplay: CSWGuiOptionsMouse (options Gameplay -> Mouse sub-panel, optmouse.gui).
//   ctor 006E2600, dtor body 006E29B0, scalar deleting dtor 006E2990, method table 007585F8, SetUpOptions 006DFE70,
//   event callbacks OnEnterSlider 006DE200, OnRevMouseButtons 006DFD10 (writes "Reverse Mouse Buttons" to the run-dir
//   .\swkotor.ini), OnEnterRevMouseButtons 006DFE60, OnMoveMouseSenSlider 006DFEE0, OnDefault 006E12B0,
//   CClientOptions::SetDefaultMouseOptions 0061D9D0. HandleInputEvent 006E6180 / OnPanelAdded 006E6170 are the
//   Gameplay unit's bodies (same slots in both tables); 006DED90 / 006DF390 come from opt_folded.cpp (fallback).
// Original: swkotor.exe GOG K1, static disassembly only. Conventions D1/D21/D24/D47; no float arithmetic
// (the slider value is an integer byte at options+50h).
#include "0b8c691376-opt_mouse.h"
#include "186fd52797-base_ini.h"      // task-37 base_ini_reconstruction::PublicIni (CExoIni 005E6750 / 005E67D0 / 005E67E0)

using namespace kotor_m1_opt;

namespace {
Mouse* mouse_of(MouseHandlers* h) { return reinterpret_cast<Mouse*>(h); }
template<class H> gui_load::EventFunc handler(void (__thiscall H::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }
template<class H> void add_event(Control* control, I32 id, Mouse* self, void (__thiscall H::*f)(kih::Control*)) {
    gui_load_control_add_event_0041ab20(control, nullptr, id, event_target(self), handler(f));
}
Word control_value(const kih::Control* control) { return word_at(control, 0x4C); }
enum Stage { kPanel = 0, kDesc, kTitle, kList, kBack, kDefault, kSlider, kLabelSen, kCheckbox };
void unwind_mouse(Mouse* self, int built) {                                       // EH states of FuncInfo 00736F2D
    if (built >= kCheckbox) gui_m48_button_toggle_destruct_0041cad0(&self->cb_revbuttons, nullptr);
    if (built >= kLabelSen) gui_load_label_destruct_00419d10(&self->lbl_mousesen, nullptr);
    if (built >= kSlider) gui_m48_slider_destruct_0041bb40(&self->sli_mousesen, nullptr);
    if (built >= kDefault) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= kList) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= kTitle) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= kDesc) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// CExoIni lifetime around WriteIniEntry (005E6750 ctor / 005E67E0 dtor, also run on unwind: EH state 1).
struct IniScope {
    base_ini_reconstruction::PublicIni ini;
    IniScope() { ini.construct_005e6750(); }
    ~IniScope() { ini.destroy_005e67e0(); }
    IniScope(const IniScope&) = delete;
    IniScope& operator=(const IniScope&) = delete;
};
}

// ======================================================================== D21 method table 007585F8 (27 slots)
// Read from the original .rdata 007585F8..00758663: identical to 00758E00 except +00 006E2990.
extern "C" const GuiM1OptGameplayMethods gui_m1_opt_mouse_methods_007585f8 = {
    &kotor_m1_opt_mouse_deleting_destruct_006e2990,      // +00
    &gui_load_panel_set_extent_0040a5a0,                 // +04
    &gui_load_panel_set_active_control_0040a630,         // +08
    &gui_load_slot_ret4_0060e760,                        // +0C
    &gui_load_slot_zero8_00409eb0,                       // +10
    &gui_load_slot_ret_004015a0,                         // +14
    &gui_load_slot_ret_004015a0,                         // +18
    &gui_load_slot_ret_004015a0,                         // +1C
    &gui_load_slot_ret_004015a0,                         // +20
    &gui_load_slot_ret_004015a0,                         // +24
    &gui_load_slot_self_00641db0,                        // +28
    &gui_load_slot_zero_0063e7f0,                        // +2C
    &gui_load_slot_zero_0063e7f0,                        // +30
    &kotor_panel_draw_0040B760,                          // +34
    &gui_load_slot_ret4_0060e760,                        // +38
    &kotor_m1_opt_gameplay_handle_input_006e6180,        // +3C 006E6180
    &gui_load_panel_hit_check_0040b690,                  // +40
    &gui_load_panel_active_control_0040b630,             // +44
    &kotor_m1_opt_gameplay_on_panel_added_006e6170,      // +48 006E6170
    &gui_load_panel_on_removed_0040c170,                 // +4C
    &gui_load_panel_button_a_0040b640,                   // +50
    &gui_load_panel_button_b_0040b650,                   // +54
    &gui_load_panel_button_x_0040b660,                   // +58
    &gui_load_panel_button_y_0040b670,                   // +5C
    &gui_load_panel_button_black_0040b680,               // +60
    &gui_load_panel_full_screen_bg_0040a900,             // +64
    &gui_load_panel_reset_font_0040a9c0,                 // +68
};

extern "C" {
// ======================================================================== 006E2600 ctor (thiscall, RET 4)
Panel* __fastcall kotor_m1_opt_mouse_construct_006e2600(void* memory, void*, Object* manager) noexcept(false) {
    Mouse* const self = static_cast<Mouse*>(memory);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 006E2624..2B
    int built = kPanel;
    try {
        set_vptr(self, &gui_m1_opt_mouse_methods_007585f8);                        // 006E263B
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);     built = kDesc;      // 006E2630..41
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);     built = kTitle;     // 006E2646..51
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);     built = kList;      // 006E2656..61
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);     built = kBack;      // 006E2666..71
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);  built = kDefault;   // 006E2676..81
        gui_load_slider_construct_0041baa0(&self->sli_mousesen, nullptr); built = kSlider;    // 006E2686..93
        gui_load_label_construct_0041acd0(&self->lbl_mousesen, nullptr);  built = kLabelSen;  // 006E2698..A3
        kotor_m1_opt_checkbox_construct_006e00d0(&self->cb_revbuttons, nullptr); built = kCheckbox;   // 006E26A8..BA (inlined 006E00D0)
        {
            Ref layout;
            layout.construct_00406d80("optmouse");                                 // 006E26C0..CE (007585EC)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006E26D3..DA
        }
        init_control(self, &self->sli_mousesen, "SLI_MOUSESEN");                   // 006E26DF..70A (007585DC)
        init_control(self, &self->lbl_mousesen, "LBL_MOUSESEN");                   // 006E270F..40 (007585CC)
        init_control(self, &self->cb_revbuttons, "CB_REVBUTTONS");                 // 006E2745..70 (007585BC)
        init_control(self, &self->lbl_title, "LBL_TITLE");                         // 006E2775..A6 (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                             // 006E27AB..DC (00755D6C)
        init_control(self, &self->btn_back, "BTN_BACK");                           // 006E27E1..812 (007548AC)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                     // 006E2817..48 (007583B4)
        set_byte(self, 0x5A5, 1);                                                  // 006E284F (lb_desc+2C1)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                        // 006E284D..56
        {
            Control* const proto = self->lb_desc.proto;                            // 006E285B ([esi+5B0h])
            vt<LabelInitView>(static_cast<Object*>(&self->desc_item))->initialize98(
                &self->desc_item, nullptr, word_of(&proto->extent), word_of(proto), 0x3F800000u);   // 006E2861..71
        }
        add_event(&self->cb_revbuttons, 0x27, self, &MouseHandlers::OnRevMouseButtons_006dfd10);       // 006E2877..81
        add_event(&self->cb_revbuttons, 0x2D, self, &MouseHandlers::OnRevMouseButtons_006dfd10);       // 006E2886..90
        add_event(&self->cb_revbuttons, 0, self, &MouseHandlers::OnEnterRevMouseButtons_006dfe60);     // 006E2895..9F
        add_event(&self->sli_mousesen, 0x27, self, &MouseHandlers::OnMoveMouseSenSlider_006dfee0);     // 006E28A4..AE
        add_event(&self->sli_mousesen, 0, self, &MouseHandlers::OnEnterSlider_006de200);               // 006E28B3..BD
        add_event(&self->sli_mousesen, 1, self, &FoldedHandlers::OnExitSlider_006ded90);               // 006E28C2..CC
        add_event(&self->sli_mousesen, 0x2F, self, &MouseHandlers::OnMoveMouseSenSlider_006dfee0);     // 006E28D1..DB
        add_event(&self->sli_mousesen, 0x30, self, &MouseHandlers::OnMoveMouseSenSlider_006dfee0);     // 006E28E0..EA
        add_event(&self->sli_mousesen, 0x3F, self, &MouseHandlers::OnMoveMouseSenSlider_006dfee0);     // 006E28EF..F9
        add_event(&self->sli_mousesen, 0x40, self, &MouseHandlers::OnMoveMouseSenSlider_006dfee0);     // 006E28FE..908
        self->sli_mousesen.u58 = 0xBB8Du;                                          // 006E290D..18 (+9A4)
        self->lbl_mousesen.u58 = 0xBB8Du;                                          // 006E291E (+B5C)
        self->cb_revbuttons.u58 = 0xBD42u;                                         // 006E2924 (+C9C)
        self->btn_back.flags &= 0xFFFFFFFBu;                                       // 006E292E..40
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, event_target(self),
            kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));   // 006E2931..43
        self->btn_default.flags &= 0xFFFFFFFBu;                                    // 006E2948..5E
        add_event(&self->btn_default, 0x27, self, &MouseHandlers::OnDefault_006e12b0);                 // 006E2954..61
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->sli_mousesen, 0);   // 006E2966..6B
        kotor_m1_opt_mouse_set_up_options_006dfe70(self, nullptr);                 // 006E2970..72
    } catch (...) {
        unwind_mouse(self, built);
        throw;
    }
    return self;                                                                   // 006E297C
}
// ======================================================================== 006E29B0 dtor body (no vptr store)
void __fastcall kotor_m1_opt_mouse_destruct_006e29b0(Mouse* self, void*) {
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_revbuttons, nullptr);        // 006E29CD..DB
    gui_load_label_destruct_00419d10(&self->lbl_mousesen, nullptr);                // 006E29E0..EB
    gui_m48_slider_destruct_0041bb40(&self->sli_mousesen, nullptr);                // 006E29F0..FB
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                // 006E2A00..0B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006E2A10..1B
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                   // 006E2A20..2B
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                   // 006E2A30..3B
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                   // 006E2A40..48
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006E2A4D..57
}
// ======================================================================== 006E2990 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_opt_mouse_deleting_destruct_006e2990(Object* object, void*, Word flags) {
    Mouse* const self = static_cast<Mouse*>(static_cast<Panel*>(object));
    kotor_m1_opt_mouse_destruct_006e29b0(self, nullptr);                           // 006E2993
    if (flags & 1u) live_free(self);                                               // 006E2998..A5
    return self;                                                                   // 006E29A8
}
// ======================================================================== 006DFE70 SetUpOptions (thiscall, RET)
// Slider value = options+50h byte (SetCurValue 00417EE0 through vslot +64); checkbox selected = options+54h bit0.
void __fastcall kotor_m1_opt_mouse_set_up_options_006dfe70(Mouse* self, void*) {
    void* const options = client_options();                                        // 006DFE70..7C
    if (!options) return;                                                          // 006DFE83..85
    const I32 sensitivity = byte_at(options, 0x50);                                // 006DFE87..8A (movzx)
    gui_m48_slider_set_cur_00417ee0(as_slider(&self->sli_mousesen), nullptr, sensitivity);   // 006DFE8D..9F
    const Word reverse = word_at(options, 0x54);                                   // 006DFEAA
    ButtonToggle* const t = as_toggle(&self->cb_revbuttons);                       // 006DFEA4..B3 vslot +5C
    t->toggle_flags = ((t->toggle_flags ^ reverse) & 1u) ^ t->toggle_flags;        // 006DFEB6..C9
}
// ======================================================================== 0061D9D0 SetDefaultMouseOptions (thiscall, RET)
void __fastcall kotor_m1_opt_client_options_set_default_mouse_0061d9d0(void* options, void*) {
    set_word(options, 0x54, 0);                                                    // 0061D9D0
    set_byte(options, 0x50, 0x2C);                                                 // 0061D9D7
}
}

// ======================================================================== event callbacks (thiscall, RET 4)
// ---- 006DE200 OnEnterSlider: if control == this+94C: SetDescription(BB8Dh) (006DF390); then the control's thumb
// params flags (vslot +64, +1B0) |= 1.
void __thiscall MouseHandlers::OnEnterSlider_006de200(kih::Control* control) {
    Mouse* const self = mouse_of(this);
    Object* const object = reinterpret_cast<Object*>(control);
    if (object == static_cast<Object*>(&self->sli_mousesen))                       // 006DE205..0D
        kotor_m1_opt_graphics_adv_set_description_006df390(self, nullptr, 0xBB8Du);   // 006DE20F..14
    Slider* const slider = as_slider(object);                                      // 006DE219..1D
    set_word(slider, kThumbParamsFlags, word_at(slider, kThumbParamsFlags) | 1u);  // 006DE220..2E
}
// ---- 006DFD10 OnRevMouseButtons: if [control+4Ch]: options+54h = (options+54h == 0); write
// "Reverse Mouse Buttons" = "%d" value to [Game Options] of .\swkotor.ini (run cwd) via CExoIni::WriteIniEntry.
// EH states 0..4 (FuncInfo 00736878) mirrored by C++ destructors (value, ini, key, section, file).
void __thiscall MouseHandlers::OnRevMouseButtons_006dfd10(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006DFD29..31
    String value;                                                                  // 006DFD38..3C (005B3190)
    IniScope scope;                                                                // 006DFD41..4D (005E6750)
    const Word old = word_at(client_options(), 0x54);                              // 006DFD52..6E
    set_word(client_options(), 0x54, old == 0 ? 1u : 0u);                          // 006DFD65..7D (sete)
    kotor_reconstruction::StartupFormat005e5680(&value, "%d", word_at(client_options(), 0x54));   // 006DFD80..A1 (0073D720)
    String key("Reverse Mouse Buttons");                                           // 006DFDA4..AD (0074FB28)
    String section("Game Options");                                                // 006DFDB2..C0 (0073D658)
    String file(".\\swkotor.ini");                                                 // 006DFDC5..D3 (0073D648)
    scope.ini.write_005e67d0(&value, &file, &section, &key);                       // 006DFDD8..F5 WriteIniEntry(value, file, section, key)
}                                                                                  // 006DFDFA..3E: ~file, ~section, ~key, ~ini, ~value
// ---- 006DFE60 OnEnterRevMouseButtons: replace the stack word with BD42h; JMP 006DF390.
void __thiscall MouseHandlers::OnEnterRevMouseButtons_006dfe60(kih::Control*) {
    kotor_m1_opt_graphics_adv_set_description_006df390(mouse_of(this), nullptr, 0xBD42u);   // 006DFE60..68
}
// ---- 006DFEE0 OnMoveMouseSenSlider: vslot +64 twice (first result unused), options+50h = (byte)slider+74h.
void __thiscall MouseHandlers::OnMoveMouseSenSlider_006dfee0(kih::Control* control) {
    Object* const object = reinterpret_cast<Object*>(control);
    (void)as_slider(object);                                                       // 006DFEE6..EA
    const I32 value = as_slider(object)->cur_value;                                // 006DFEED..F4
    set_byte(client_options(), 0x50, static_cast<unsigned char>(value));           // 006DFEF7..05
}
// ---- 006E12B0 OnDefault: SetDefaultMouseOptions + SetUpOptions.
void __thiscall MouseHandlers::OnDefault_006e12b0(kih::Control* control) {
    if (control_value(control) == 0) return;                                       // 006E12B7..BC
    kotor_m1_opt_client_options_set_default_mouse_0061d9d0(client_options(), nullptr);   // 006E12BE..CE
    kotor_m1_opt_mouse_set_up_options_006dfe70(mouse_of(this), nullptr);           // 006E12D3..D5
}
