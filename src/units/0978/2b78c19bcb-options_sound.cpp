// lane-m1-opt-sound-gfx (Milestone 1): Options -> SOUND sub-panel and its ADVANCED sub-panel.
//   006E3550 CSWGuiOptionsSound::CSWGuiOptionsSound          006E3C60 / 006E3C40 ~CSWGuiOptionsSound (+ deleting)
//   006DDB90 CSWGuiOptionsSound::HandleInputEvent            006DFAD0 SetUpOptions        006DDC80 SetDescription
//   006DF9B0 OnMoveVolumeSlider  006DFAA0 OnEnterSlider  006E0F50 OnDefault  006E2D70 OnAdvanced
//   006DED90 CSWGuiOptionsGraphics::OnExitSlider (shared slider callback; registered by Sound and Graphics)
//   007587C0 CSWGuiOptionsSound method table (27 slots)
//   006E20A0 CSWGuiOptionsSoundAdvanced::CSWGuiOptionsSoundAdvanced   006E2520 / 006E2500 dtors
//   006E0F80 HandleInputEvent (Back: SetNewOptions, CExoSound::ReInitialize, [Sound Options] EAX / Force Software
//            written to the run-dir .\swkotor.ini)   006DFC90 SetUpOptions  006DFB60 SetNewOptions  006DDD70 SetEAXText
//   006DDF40 EnableEAXControls  006DDE50 SetDescription  006DDFA0 OnEnterOption  006DDFF0 OnDiscardChanges
//   006DFBB0 OnEAXLeft  006DFBF0 OnEAXRight  006DFC50 OnForceSoftware  006E1270 OnDefault
//   00758550 CSWGuiOptionsSoundAdvanced method table (27 slots)
//   (CSWGuiOptionsCheckbox 00758258 family: single owner lane-m1-opt-gameplay; only its table address is stored here)
//   CClientOptions 0061D540 SetDefaultSoundOptions, 0061D550 SetDefaultAdvancedSoundOptions
//   CExoSound 005D63F0 ReInitialize, 005D6460 GetBestEAXAvailable
// Disable Sound=1 (run configuration, D17/D25): CExoSound's internal object is never created, so the volume setters
// (lane-sound 005D5C50/005D5CD0/005D5D50), GetEAX 005D6450, GetBestEAXAvailable 005D6460 (returns 0) and ReInitialize
// 005D63F0 (returns before 005DA8A0) touch nothing; the client options bytes and the ini entries are still written,
// exactly as the original does.
// Original: swkotor.exe GOG K1 (sha256 9c10e045...91435), static disassembly only; each behaviour cites its
// instruction addresses. D1/D21/D24/D47 as lane-m1-options; D30-FINAL: the only float arithmetic (OnMoveVolumeSlider
// fild/fidiv/fstp) is a double division rounded once to float at the original fstp dword store; every ctor/dtor
// member stage that can throw is unwound with C++ try/catch mirroring the original EH states.
#include "67605decb8-options_sound.h"
#include "186fd52797-base_ini.h"          // base_ini_reconstruction::PublicIni (CExoIni 005E6750 / 005E67D0 / 005E67E0)
#include <cstring>

using namespace kotor_m1_options;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Label;
using gui_load::Button;
using gui_load::Extent;
using gui_load::String;
using gui_load::Ref;
using gui_load::Word;
using gui_load::I32;

// ---- existing owners (declarations repeated exactly as their owners spell them)
namespace kotor_sound_path {                                   // lane-sound sound_path.h (sound-render / sound-path units)
using U = std::uint32_t;
void __fastcall SoundSetMusicVolume005d5c50(void* self, void*, float volume, U apply_group); // RET 8
void __fastcall SoundSetSoundEffectVolume005d5cd0(void* self, void*, float volume);       // RET 4
void __fastcall SoundSetDialogVolume005d5d50(void* self, void*, float volume);            // RET 4
U __fastcall SoundGetEax005d6450(void* self, void*);                                      // RET 0
}
namespace kotor_sound_internal {                               // lane-sound sound_internal.cpp
std::uint32_t __fastcall Initialize005da8a0(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
}

namespace {
Word word_at(const void* p, unsigned off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* ptr_at(const void* p, unsigned off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
unsigned char byte_at(const void* p, unsigned off) { return static_cast<const unsigned char*>(p)[off]; }
void put_byte(void* p, unsigned off, unsigned char v) { static_cast<unsigned char*>(p)[off] = v; }
void put_word(void* p, unsigned off, Word v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
Word word_of(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
void* client_app() { return ptr_at(kotor_menu_window_cells::manager_007a39fc, 4); }               // [[007A39FC]+4]
void* options_object() { return kotor_fastcall_005ed700(client_app(), nullptr); }                  // CClientExoApp::GetClientOptions
void* sound() { return kotor_cell_sound_007a39ec; }                                                // [007A39EC] CExoSound*
kih::Manager* manager_of(const Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); }   // [this+18h]
Object* object_of(kih::Control* control) { return reinterpret_cast<Object*>(control); }
kih::Control* kc(Control* control) { return reinterpret_cast<kih::Control*>(control); }
template<class P> kih::GuiObject* event_target(P* panel) { return reinterpret_cast<kih::GuiObject*>(panel); }
template<class H> gui_load::EventFunc handler(void (__thiscall H::*f)(kih::Control*)) { return static_cast<gui_load::EventFunc>(f); }  // D47
gui_load::EventFunc back_thunk() {                                                                 // 00624BB0 (L0-shared)
    return kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0);
}
void init_control(Panel* panel, Control* control, const char* tag) {        // String temp + InitControl(c, &s, 1)
    String name(tag);                                                       // 005E5A90
    gui_load_panel_init_control_0040b930(panel, nullptr, control, &name, 1); // 0040B930
}                                                                           // 005E5C20
// CSWGuiLabel +98 Initialize(proto+4, proto, 1.0f) of a description item from the LB_DESC prototype.
void init_desc_item(Label* item, const gui_lb::ListBox* lb) {
    Control* const proto = lb->proto;
    gui_lb::vt<LabelInit98View>(static_cast<Object*>(item))->initialize98(item, nullptr, word_of(&proto->extent), word_of(proto),
                                                                         0x3F800000u);
}
// Common body of the options SetDescription family (006DDC80 / 006DDE50; same instruction sequence as 006DE340).
void describe(Label* item, gui_lb::ListBox* lb, Word strref) {
    void* const params = reinterpret_cast<unsigned char*>(&item->text) + 0x18;           // text params (+D0+18)
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);                    // SetStrRef 00415E50
    String text(*reinterpret_cast<const String*>(params));                                // CExoString copy 005E5B00
    gui_load_listbox_clear_items_00419a90(lb, nullptr);                                   // ClearItems 00419A90
    Extent extent = item->extent;                                                         // item extent copy
    gui_load::Text* const text_object = &item->text;
    if (text.BorrowedLength005e5790() > 0)                                                // GetLength 005E5790
        extent.height = static_cast<I32>(gui_lb::vt<TextIdeal08View>(text_object)->ideal_height(text_object, nullptr));  // vslot +08
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);          // 00414EE0
    item->vtbl->set_extent(item, nullptr, &extent);                                       // vslot +04
    Control* items[1] = { item };
    gui_m48_listbox_add_controls_0041c2c0(lb, nullptr, items, 1, 0, 0, 0);                // AddControls(&item,1,0,0,0) 0041C2C0
    gui_load_listbox_set_active_control_0041c160(lb, nullptr, item, 0);                   // 0041C160
}                                                                                         // ~CExoString 005E5C20
// Toggle selected bit: vslot +5C (00641DB0 self_return) then bit0 of +1C8 replaced by `value` (xor/and 1/xor).
void set_toggle(gui_lb::ButtonToggle* toggle, Word value) {
    Object* const self = gui_lb::vt<ControlSelf5CView>(static_cast<Object*>(toggle))->slot5c(toggle, nullptr);
    gui_lb::ButtonToggle* const t = static_cast<gui_lb::ButtonToggle*>(static_cast<gui_load::Button*>(static_cast<Control*>(self)));
    t->toggle_flags ^= (t->toggle_flags ^ value) & 1u;
}
void set_enabled(Control* control, Word enable) {                                         // vslot +88 SetEnabled
    gui_lb::vt<ControlEnable88View>(static_cast<Object*>(control))->set_enabled88(control, nullptr, enable);
}
gui_lb::Slider* slider_of(kih::Control* control) {                                        // vslot +64 (00641DB0 self_return)
    Object* const o = object_of(control);
    Object* const self = gui_lb::vt<ControlSelf64View>(o)->slot64(o, nullptr);
    return static_cast<gui_lb::Slider*>(static_cast<gui_load::Navigable*>(static_cast<Control*>(self)));
}
}

namespace {
// Construct a CSWGuiButtonToggle member and give it the checkbox table (the ctor inlines the vptr store).
void construct_checkbox(OptionsCheckbox* box) {
    gui_load_button_toggle_construct_0041ca30(box, nullptr);                               // 0041CA30
    box->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_m1_opt_checkbox_methods_00758258);   // mov [reg],758258h
}
}

// ======================================================================== shared slider callback 006DED90
// mov ecx,[esp+4]; call [[ecx]+64h]; [eax+1B0h] &= ~1 (thumb image params flags, +164+28+24); RET 4.
void __thiscall OptionsSliderHandlers::OnExitSlider_006ded90(kih::Control* control) {
    gui_lb::Slider* const s = slider_of(control);                                          // 006DED90..96
    s->thumb.params.flags &= 0xFFFFFFFEu;                                                   // 006DED99..A7
}

// ======================================================================== CClientOptions / CExoSound leaves
extern "C" {
// 0061D540 SetDefaultSoundOptions: +45 = +46 = +47 = 55h, +48 = 64h (RET).
void __fastcall kotor_m1_client_options_default_sound_0061d540(void* options, void*) {
    put_byte(options, 0x45, 0x55);                                                         // 0061D542
    put_byte(options, 0x46, 0x55);                                                         // 0061D545
    put_byte(options, 0x47, 0x55);                                                         // 0061D548
    put_byte(options, 0x48, 0x64);                                                         // 0061D54B
}
// 005D6460 CExoSound::GetBestEAXAvailable: internal ? [internal+104h] : 0 (RET).
std::uint32_t __fastcall kotor_m1_sound_best_eax_005d6460(void* sound_object, void*) {
    void* const internal = ptr_at(sound_object, 0);                                        // 005D6460
    return internal ? word_at(internal, 0x104) : 0u;                                        // 005D6462..6F
}
// 0061D550 SetDefaultAdvancedSoundOptions: +49 = min(GetBestEAXAvailable, 3) (the <= 3 path calls it again), +4C = 0.
void __fastcall kotor_m1_client_options_default_advanced_sound_0061d550(void* options, void*) {
    if (static_cast<I32>(kotor_m1_sound_best_eax_005d6460(sound(), nullptr)) > 3) {         // 0061D553..61 (signed jle)
        put_byte(options, 0x49, 3);                                                        // 0061D563..68
    } else {
        put_byte(options, 0x49, static_cast<unsigned char>(kotor_m1_sound_best_eax_005d6460(sound(), nullptr)));  // 0061D574..7F
    }
    put_word(options, 0x4C, 0);                                                            // 0061D56B / 0061D582
}
// 005D63F0 CExoSound::ReInitialize(eax_level, force_software) (RET 8): null internal -> return (Disable Sound=1);
// else CExoSoundInternal::Initialize(max([+28],18h), max([+29],10h), eax_level, force_software) 005DA8A0.
// EAX on return is not meaningful (null path leaves the caller's EAX; the only caller 006E1030 discards it).
void __fastcall kotor_m1_sound_reinitialize_005d63f0(void* sound_object, void*, std::uint32_t eax_level, std::uint32_t force_software) {
    void* const internal = ptr_at(sound_object, 0);                                        // 005D63F0
    if (!internal) return;                                                                 // 005D63F2..F4
    const unsigned char b29 = byte_at(internal, 0x29);                                     // 005D63F6
    const std::uint32_t second = b29 < 0x10 ? 0x10u : b29;                                 // 005D63F9..02
    const unsigned char b28 = byte_at(internal, 0x28);                                     // 005D6405
    const std::uint32_t first = b28 >= 0x18 ? static_cast<std::uint32_t>(b28) : 0x18u;    // 005D6408..13
    (void)kotor_sound_internal::Initialize005da8a0(internal, nullptr, first, second, eax_level, force_software);  // 005D6417..23
}
}

// ======================================================================== CSWGuiOptionsSound
namespace {
OptionsSound* sound_of(OptionsSoundHandlers* h) { return reinterpret_cast<OptionsSound*>(h); }
OptionsSoundHandlers* sound_handlers(OptionsSound* self) { return reinterpret_cast<OptionsSoundHandlers*>(self); }
// Constructed-member stages of 006E3550 (EH states 0..0Dh of FuncInfo 00737371; 0Eh+ are CExoString temporaries).
void unwind_sound(OptionsSound* self, int built) {
    if (built >= 14) gui_load_button_destruct_00419da0(&self->btn_advanced, nullptr);
    if (built >= 13) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= 12) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 11) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= 10) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= 9) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= 8) gui_load_label_destruct_00419d10(&self->lbl_movie, nullptr);
    if (built >= 7) gui_load_label_destruct_00419d10(&self->lbl_vo, nullptr);
    if (built >= 6) gui_load_label_destruct_00419d10(&self->lbl_fx, nullptr);
    if (built >= 5) gui_load_label_destruct_00419d10(&self->lbl_music, nullptr);
    if (built >= 4) gui_m48_slider_destruct_0041bb40(&self->sli_movie, nullptr);
    if (built >= 3) gui_m48_slider_destruct_0041bb40(&self->sli_fx, nullptr);
    if (built >= 2) gui_m48_slider_destruct_0041bb40(&self->sli_vo, nullptr);
    if (built >= 1) gui_m48_slider_destruct_0041bb40(&self->sli_music, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

extern "C" const OptionsPanelMethods<OptionsSound> kotor_m1_options_sound_methods_007587c0 = {
    &kotor_m1_options_sound_deleting_destruct_006e3c40,          // +00 006E3C40
    KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38,                          // +04..+38 (as 0073E010)
    &kotor_m1_options_sound_handle_input_006ddb90,               // +3C 006DDB90
    KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68,                          // +40..+68 (as 0073E010)
};

extern "C" {
// ---- 006E3550 ctor (thiscall, RET 4)
OptionsSound* __fastcall kotor_m1_options_sound_construct_006e3550(OptionsSound* self, void*, Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006E3572..77
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_options_sound_methods_007587c0);  // 006E3589
        gui_load_slider_construct_0041baa0(&self->sli_music, nullptr);  built = 1;        // 006E357C..8F
        gui_load_slider_construct_0041baa0(&self->sli_vo, nullptr);     built = 2;        // 006E3594..A1
        gui_load_slider_construct_0041baa0(&self->sli_fx, nullptr);     built = 3;        // 006E35A6..B3
        gui_load_slider_construct_0041baa0(&self->sli_movie, nullptr);  built = 4;        // 006E35B8..C3
        gui_load_label_construct_0041acd0(&self->lbl_music, nullptr);   built = 5;        // 006E35C8..D3
        gui_load_label_construct_0041acd0(&self->lbl_fx, nullptr);      built = 6;        // 006E35D8..E3
        gui_load_label_construct_0041acd0(&self->lbl_vo, nullptr);      built = 7;        // 006E35E8..F3
        gui_load_label_construct_0041acd0(&self->lbl_movie, nullptr);   built = 8;        // 006E35F8..03
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);   built = 9;        // 006E3608..13
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);   built = 10;       // 006E3618..23
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);   built = 11;       // 006E3628..33
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);   built = 12;       // 006E3638..43
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr); built = 13;      // 006E3648..53
        gui_load_button_construct_0041c0f0(&self->btn_advanced, nullptr); built = 14;     // 006E3658..63
        {
            Ref layout;
            layout.construct_00406d80("optsound");                                         // 006E3668..76 (007587B4)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                  // 006E367B..82
        }
        init_control(self, &self->sli_music, "SLI_MUSIC");                                 // 006E3687..B2 (007587A8)
        init_control(self, &self->sli_vo, "SLI_VO");                                       // 006E36B7..E2 (007587A0)
        init_control(self, &self->sli_fx, "SLI_FX");                                       // 006E36E7..12 (00758798)
        init_control(self, &self->sli_movie, "SLI_MOVIE");                                 // 006E3717..48 (0075878C)
        init_control(self, &self->lbl_music, "LBL_MUSIC");                                 // 006E374D..7E (00758780)
        init_control(self, &self->lbl_vo, "LBL_VO");                                       // 006E3783..B4 (00758778)
        init_control(self, &self->lbl_fx, "LBL_FX");                                       // 006E37B9..EA (00758770)
        init_control(self, &self->lbl_movie, "LBL_MOVIE");                                 // 006E37EF..20 (00758764)
        init_control(self, &self->btn_advanced, "BTN_ADVANCED");                           // 006E3825..56 (00758670)
        init_control(self, &self->lbl_title, "LBL_TITLE");                                 // 006E385B..8C (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                                     // 006E3891..C2 (00755D6C)
        self->sli_music.u58 = 0xA547u;                                                     // 006E38C7 (+0BC)
        self->sli_vo.u58 = 0xA548u;                                                        // 006E38D1 (+274)
        self->sli_fx.u58 = 0xA549u;                                                        // 006E38DB (+42C)
        {
            String name("BTN_BACK");                                                       // 006E38E5..09 (007548AC)
            self->sli_movie.u58 = 0xBB31u;                                                 // 006E38EE (+5E4)
            self->btn_advanced.u58 = 0xBFB1u;                                              // 006E38F8 (+1584)
            reinterpret_cast<unsigned char*>(self)[0x1185] = 1;                            // 006E3902 (lb_desc+2C1)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->btn_back, &name, 1); // 006E390E..23
        }                                                                                  // 006E3928..31
        init_control(self, &self->btn_default, "BTN_DEFAULT");                             // 006E3936..67 (007583B4)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                // 006E396C..6E
        init_desc_item(&self->desc_item, &self->lb_desc);                                  // 006E3973..95 (+1190 proto, vslot +98)
        gui_lb::Slider* const sliders[4] = { &self->sli_music, &self->sli_vo, &self->sli_fx, &self->sli_movie };
        for (gui_lb::Slider* s : sliders) gui_m48_slider_set_cur_00417ee0(s, nullptr, 0x32);   // 006E3995..B8 SetCurValue(50)
        kih::GuiObject* const target = event_target(self);
        const gui_load::EventFunc enter = handler(&OptionsSoundHandlers::OnEnterSlider_006dfaa0);
        const gui_load::EventFunc exit = handler(&OptionsSliderHandlers::OnExitSlider_006ded90);
        const gui_load::EventFunc move = handler(&OptionsSoundHandlers::OnMoveVolumeSlider_006df9b0);
        const gui_load::EventFunc defaults = handler(&OptionsSoundHandlers::OnDefault_006e0f50);
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0, target, enter);    // 006E39BD..F8
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 1, target, exit);     // 006E39FD..38
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x2F, target, move);  // 006E3A3D..78
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x30, target, move);  // 006E3A7D..B8
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x3F, target, move);  // 006E3ABD..F8
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x40, target, move);  // 006E3AFD..38
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x2A, target, defaults); // 006E3B3D..78
        for (gui_lb::Slider* s : sliders) gui_load_control_add_event_0041ab20(s, nullptr, 0x27, target, move);  // 006E3B7D..B8
        gui_load_control_add_event_0041ab20(&self->btn_advanced, nullptr, 0x27, target,
                                            handler(&OptionsSoundHandlers::OnAdvanced_006e2d70));               // 006E3BBD..CB
        kotor_m1_options_sound_set_up_options_006dfad0(self, nullptr);                     // 006E3BD0..D2
        self->btn_back.flags &= 0xFFFFFFFBu;                                               // 006E3BD7..F2 (stored before the call)
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target, back_thunk());              // 006E3BE3..F5
        self->btn_default.flags &= 0xFFFFFFFBu;                                            // 006E3BFA..10
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, target, defaults);               // 006E3C00..13
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->sli_music, 0);    // 006E3C18..1D
    } catch (...) {
        unwind_sound(self, built);
        throw;
    }
    return self;                                                                           // 006E3C27
}

// ---- 006E3C60 dtor (thiscall, RET): members in reverse order, then CSWGuiPanel::~CSWGuiPanel.
void __fastcall kotor_m1_options_sound_destruct_006e3c60(OptionsSound* self, void*) {
    gui_load_button_destruct_00419da0(&self->btn_advanced, nullptr);                      // 006E3C7D..8B
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                       // 006E3C90..9B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                          // 006E3CA0..AB
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                          // 006E3CB0..BB
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                          // 006E3CC0..CB
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                          // 006E3CD0..DB
    gui_load_label_destruct_00419d10(&self->lbl_movie, nullptr);                          // 006E3CE0..EB
    gui_load_label_destruct_00419d10(&self->lbl_vo, nullptr);                             // 006E3CF0..FB
    gui_load_label_destruct_00419d10(&self->lbl_fx, nullptr);                             // 006E3D00..0B
    gui_load_label_destruct_00419d10(&self->lbl_music, nullptr);                          // 006E3D10..1B
    gui_m48_slider_destruct_0041bb40(&self->sli_movie, nullptr);                          // 006E3D20..2B
    gui_m48_slider_destruct_0041bb40(&self->sli_fx, nullptr);                             // 006E3D30..3B
    gui_m48_slider_destruct_0041bb40(&self->sli_vo, nullptr);                             // 006E3D40..4B
    gui_m48_slider_destruct_0041bb40(&self->sli_music, nullptr);                          // 006E3D50..58
    gui_load_panel_destruct_0040cf70(self, nullptr);                                      // 006E3D5D..67
}
// ---- 006E3C40 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_sound_deleting_destruct_006e3c40(OptionsSound* self, void*, Word flags) {
    kotor_m1_options_sound_destruct_006e3c60(self, nullptr);                              // 006E3C43
    if (flags & 1u) live_free(self);                                                      // 006E3C48..55
    return self;                                                                          // 006E3C58
}

// ---- 006DDB90 HandleInputEvent(event, pressed) (thiscall, RET 8). Jump table 006DDC50 indexed by the byte table
// 006DDC64 for event-28h in 0..12h: 00 04 04 04 04 01 00 04 04 04 04 04 04 04 04 04 04 02 03 (read from the image)
// -> 28h/2Eh Back 006DDBD8; 2Dh 006DDBBF; 39h 006DDC0B; 3Ah 006DDC2D; others 006DDC40.
void __fastcall kotor_m1_options_sound_handle_input_006ddb90(Object* object, void*, I32 event, I32 pressed) {
    OptionsSound* const self = static_cast<OptionsSound*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                                   // 006DDB97..9F
        switch (event) {                                                                  // 006DDBA5..B8
        case 0x28: case 0x2E:                                                             // 006DDBD8 Back (no ini write here)
            manager_of(self)->PlayGuiSound(0);                                            // 006DDBD8..DD (0040A140)
            kotor_fastcall_0040be00(self->manager, nullptr);                              // 006DDBE2..E5 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                           // 006DDBEA..FA
            break;
        case 0x2D:                                                                        // 006DDBBF
            manager_of(self)->PlayGuiSound(0);                                            // 006DDBBF..C4
            break;
        case 0x39: {                                                                      // 006DDC0B: LB_DESC vslot +3C(31h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x31, 1);
            break;
        }
        case 0x3A: {                                                                      // 006DDC2D: LB_DESC vslot +3C(32h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x32, 1);
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);                 // 006DDBCD/C00/C22/C44 (00409E60)
}

// ---- 006DFAD0 SetUpOptions (thiscall, RET): slider positions from CClientOptions +45..+48, then apply each.
void __fastcall kotor_m1_options_sound_set_up_options_006dfad0(OptionsSound* self, void*) {
    void* const options = options_object();                                              // 006DFAD0..DC
    if (!options) return;                                                                 // 006DFAE3..E5
    gui_m48_slider_set_cur_00417ee0(&self->sli_music, nullptr, byte_at(options, 0x45));   // 006DFAE7..F3
    gui_m48_slider_set_cur_00417ee0(&self->sli_vo, nullptr, byte_at(options, 0x46));      // 006DFAF8..05
    gui_m48_slider_set_cur_00417ee0(&self->sli_fx, nullptr, byte_at(options, 0x47));      // 006DFB0A..15
    gui_m48_slider_set_cur_00417ee0(&self->sli_movie, nullptr, byte_at(options, 0x48));   // 006DFB1A..25
    OptionsSoundHandlers* const h = sound_handlers(self);
    h->OnMoveVolumeSlider_006df9b0(kc(&self->sli_music));                                 // 006DFB2A..2D
    h->OnMoveVolumeSlider_006df9b0(kc(&self->sli_vo));                                    // 006DFB32..35
    h->OnMoveVolumeSlider_006df9b0(kc(&self->sli_fx));                                    // 006DFB3A..43
    h->OnMoveVolumeSlider_006df9b0(kc(&self->sli_movie));                                 // 006DFB48..51
}

// ---- 006DDC80 SetDescription(strref) (thiscall, RET 4; EH state 0 FuncInfo 00736788 covers the string copy):
// params +D2C, list +EC4, item +C44 (extent +C48), text +D14.
void __fastcall kotor_m1_options_sound_set_description_006ddc80(OptionsSound* self, void*, Word strref) noexcept(false) {
    describe(&self->desc_item, &self->lb_desc, strref);                                   // 006DDCA0..DD51
}
}

// ---- 006DF9B0 OnMoveVolumeSlider(control) (thiscall, RET 4): volume = cur/max (fild/fidiv, fstp dword), then the
// matching CExoSound setter and the CClientOptions byte (+45 music, +46 dialog, +47 effects, +48 movie: no setter).
void __thiscall OptionsSoundHandlers::OnMoveVolumeSlider_006df9b0(kih::Control* control) {
    OptionsSound* const self = sound_of(this);
    const I32 max_value = slider_of(control)->max_value;                                  // 006DF9B7..C6 vslot +64, [eax+70h]
    const I32 cur = slider_of(control)->cur_value;                                        // 006DF9C4..D2 vslot +64, [eax+74h]
    const float volume = static_cast<float>(static_cast<double>(cur) / static_cast<double>(max_value));  // 006DF9D6..E3
    const Object* const c = object_of(control);
    const unsigned char cur_byte = static_cast<unsigned char>(cur);                       // bl
    if (c == &self->sli_music) {                                                          // 006DF9DA..E7
        kotor_sound_path::SoundSetMusicVolume005d5c50(sound(), nullptr, volume, 0);       // 006DF9E9..F6
        put_byte(options_object(), 0x45, cur_byte);                                       // 006DF9FB..0B
    } else if (c == &self->sli_vo) {                                                      // 006DFA13..1B
        kotor_sound_path::SoundSetDialogVolume005d5d50(sound(), nullptr, volume);         // 006DFA1D..28
        put_byte(options_object(), 0x46, cur_byte);                                       // 006DFA2D..3D
    } else if (c == &self->sli_fx) {                                                      // 006DFA45..4D
        kotor_sound_path::SoundSetSoundEffectVolume005d5cd0(sound(), nullptr, volume);    // 006DFA4F..5A
        put_byte(options_object(), 0x47, cur_byte);                                       // 006DFA5F..6F
    } else if (c == &self->sli_movie) {                                                   // 006DFA77..7F
        put_byte(options_object(), 0x48, cur_byte);                                       // 006DFA81..8E
    }
}
// ---- 006DFAA0 OnEnterSlider(control) (thiscall, RET 4): SetDescription([control+58h]); thumb flags |= 1.
void __thiscall OptionsSoundHandlers::OnEnterSlider_006dfaa0(kih::Control* control) {
    kotor_m1_options_sound_set_description_006ddc80(sound_of(this), nullptr, word_at(control, 0x58));   // 006DFAA1..A9
    gui_lb::Slider* const s = slider_of(control);                                         // 006DFAAE..B2
    s->thumb.params.flags |= 1u;                                                          // 006DFAB5..C3 [eax+1B0h]
}
// ---- 006E0F50 OnDefault(control) (thiscall, RET 4)
void __thiscall OptionsSoundHandlers::OnDefault_006e0f50(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                              // 006E0F50..5C
    kotor_m1_client_options_default_sound_0061d540(options_object(), nullptr);            // 006E0F5E..6E
    kotor_m1_options_sound_set_up_options_006dfad0(sound_of(this), nullptr);              // 006E0F73..75
}
// ---- 006E2D70 OnAdvanced(control) (thiscall, RET 4): new(1320h) CSWGuiOptionsSoundAdvanced(manager); AddPanel(p,3,1).
void __thiscall OptionsSoundHandlers::OnAdvanced_006e2d70(kih::Control* control) {
    OptionsSound* const self = sound_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E2D89..91
    void* const memory = c2da_directory_new_006fa7e6(0x1320);                            // 006E2D93..9D operator new 006FA7E6
    OptionsSoundAdvanced* panel = nullptr;                                                // 006E2DBD (xor eax,eax)
    if (memory) {                                                                         // 006E2DA4..AE
        try {
            panel = kotor_m1_options_sound_adv_construct_006e20a0(static_cast<OptionsSoundAdvanced*>(memory), nullptr,
                                                                  self->manager);         // 006E2DB0..B6 ([esi+18h])
        } catch (...) {
            live_free(memory);                                                            // EH state 0 funclet: _free
            throw;
        }
    }
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                         // 006E2DBF..CF AddPanel(p, 3, 1)
}

// ======================================================================== CSWGuiOptionsSoundAdvanced
namespace {
OptionsSoundAdvanced* adv_of(OptionsSoundAdvancedHandlers* h) { return reinterpret_cast<OptionsSoundAdvanced*>(h); }
// Constructed-member stages of 006E20A0 (EH states 0..9 of FuncInfo 00736DD9).
void unwind_sound_adv(OptionsSoundAdvanced* self, int built) {
    if (built >= 10) gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);
    if (built >= 9) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= 8) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 7) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= 6) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= 5) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= 4) gui_load_button_destruct_00419da0(&self->btn_eax_left, nullptr);
    if (built >= 3) gui_load_button_destruct_00419da0(&self->btn_eax_right, nullptr);
    if (built >= 2) gui_load_button_destruct_00419da0(&self->btn_eax, nullptr);
    if (built >= 1) gui_m48_button_toggle_destruct_0041cad0(&self->cb_force_software, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// min(GetBestEAXAvailable, 3) as inlined at 006DDDE8..0A / 006DFBFE..20 (second call on the <= 3 path).
I32 best_eax_capped() {
    if (static_cast<I32>(kotor_m1_sound_best_eax_005d6460(sound(), nullptr)) > 3) return 3;
    return static_cast<I32>(kotor_m1_sound_best_eax_005d6460(sound(), nullptr));
}
// One [Sound Options] entry of .\swkotor.ini (006E106E..10E9 / 006E1131..11AC): key, section, file temporaries;
// WriteIniEntry(value, file, section, key); temporaries destroyed file, section, key.
void write_sound_option(base_ini_reconstruction::PublicIni& ini, String* value, const char* key_text) {
    String key(key_text);                                                                 // "EAX" 0074D2A8 / "Force Software" 0074E8EC
    String section("Sound Options");                                                     // 0073D724
    String file(".\\swkotor.ini");                                                      // 0073D648
    (void)ini.write_005e67d0(value, &file, &section, &key);                               // 006E10C0 / 006E1183 (005E67D0)
}
}

extern "C" const OptionsPanelMethods<OptionsSoundAdvanced> kotor_m1_options_sound_adv_methods_00758550 = {
    &kotor_m1_options_sound_adv_deleting_destruct_006e2500,      // +00 006E2500
    KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38,                          // +04..+38 (as 0073E010)
    &kotor_m1_options_sound_adv_handle_input_006e0f80,           // +3C 006E0F80
    KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68,                          // +40..+68 (as 0073E010)
};

extern "C" {
// ---- 006E20A0 ctor (thiscall, RET 4)
OptionsSoundAdvanced* __fastcall kotor_m1_options_sound_adv_construct_006e20a0(OptionsSoundAdvanced* self, void*,
                                                                               Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006E20C4..CB
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_options_sound_adv_methods_00758550);  // 006E20DD
        construct_checkbox(&self->cb_force_software);                    built = 1;      // 006E20D0..E8
        gui_load_button_construct_0041c0f0(&self->btn_eax, nullptr);       built = 2;      // 006E20EE..F9
        gui_load_button_construct_0041c0f0(&self->btn_eax_right, nullptr); built = 3;      // 006E20FE..0B
        gui_load_button_construct_0041c0f0(&self->btn_eax_left, nullptr);  built = 4;      // 006E2110..1B
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);      built = 5;      // 006E2120..2B
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);      built = 6;      // 006E2130..3B
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);      built = 7;      // 006E2140..4B
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);      built = 8;      // 006E2150..5B
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);   built = 9;      // 006E2160..6B
        gui_load_button_construct_0041c0f0(&self->btn_cancel, nullptr);    built = 10;     // 006E2170..7B
        {
            Ref layout;
            layout.construct_00406d80("optsoundadv");                                      // 006E2180..8E (00758544)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                  // 006E2193..9A
        }
        init_control(self, &self->cb_force_software, "CB_FORCESOFTWARE");                  // 006E219F..CA (00758530)
        init_control(self, &self->btn_eax, "BTN_EAX");                                     // 006E21CF..00 (00758528)
        init_control(self, &self->btn_eax_right, "BTN_EAXRIGHT");                          // 006E2205..30 (00758518)
        init_control(self, &self->btn_eax_left, "BTN_EAXLEFT");                            // 006E2235..66 (0075850C)
        init_control(self, &self->lbl_title, "LBL_TITLE");                                 // 006E226B..9C (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                                     // 006E22A1..D2 (00755D6C)
        init_control(self, &self->btn_back, "BTN_BACK");                                   // 006E22D7..08 (007548AC)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                             // 006E230D..3E (007583B4)
        init_control(self, &self->btn_cancel, "BTN_CANCEL");                               // 006E2343..74 (0074FD90)
        self->cb_force_software.u58 = 0xBDC1u;                                             // 006E237B (+0BC)
        self->btn_eax.u58 = 0xBDC2u;                                                       // 006E2385 (+374)
        reinterpret_cast<unsigned char*>(self)[0xDA9] = 1;                                 // 006E238F (lb_desc+2C1)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                // 006E2379..96
        init_desc_item(&self->desc_item, &self->lb_desc);                                  // 006E239B..B7 (+DB4 proto, vslot +98)
        kih::GuiObject* const target = event_target(self);
        const gui_load::EventFunc enter = handler(&OptionsSoundAdvancedHandlers::OnEnterOption_006ddfa0);
        const gui_load::EventFunc force = handler(&OptionsSoundAdvancedHandlers::OnForceSoftware_006dfc50);
        const gui_load::EventFunc left = handler(&OptionsSoundAdvancedHandlers::OnEAXLeft_006dfbb0);
        const gui_load::EventFunc right = handler(&OptionsSoundAdvancedHandlers::OnEAXRight_006dfbf0);
        gui_load_control_add_event_0041ab20(&self->cb_force_software, nullptr, 0x27, target, force);   // 006E23BD..C7
        gui_load_control_add_event_0041ab20(&self->cb_force_software, nullptr, 0x2D, target, force);   // 006E23CC..D6
        gui_load_control_add_event_0041ab20(&self->cb_force_software, nullptr, 0, target, enter);      // 006E23DB..E5
        gui_load_control_add_event_0041ab20(&self->btn_eax_left, nullptr, 0x27, target, left);         // 006E23EA..F8
        gui_load_control_add_event_0041ab20(&self->btn_eax_left, nullptr, 0x2D, target, left);         // 006E23FD..0B
        gui_load_control_add_event_0041ab20(&self->btn_eax_left, nullptr, 0, target, enter);           // 006E2410..1E
        gui_load_control_add_event_0041ab20(&self->btn_eax_right, nullptr, 0x27, target, right);       // 006E2423..2D
        gui_load_control_add_event_0041ab20(&self->btn_eax_right, nullptr, 0x2D, target, right);       // 006E2432..3C
        gui_load_control_add_event_0041ab20(&self->btn_eax_right, nullptr, 0, target, enter);          // 006E2441..4B
        gui_load_control_add_event_0041ab20(&self->btn_eax, nullptr, 0, target, enter);                // 006E2450..5E
        kotor_m1_options_sound_adv_set_up_options_006dfc90(self, nullptr);                 // 006E2463..65
        self->btn_back.flags &= 0xFFFFFFFBu;                                               // 006E246A..85
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target, back_thunk());     // 006E2476..88 (00624BB0)
        self->btn_default.flags &= 0xFFFFFFFBu;                                            // 006E248D..A3
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, target,
                                            handler(&OptionsSoundAdvancedHandlers::OnDefault_006e1270));      // 006E2499..A6
        self->btn_cancel.flags &= 0xFFFFFFFBu;                                             // 006E24AB..C1
        gui_load_control_add_event_0041ab20(&self->btn_cancel, nullptr, 0x27, target,
                                            handler(&OptionsSoundAdvancedHandlers::OnDiscardChanges_006ddff0)); // 006E24B7..C4
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->cb_force_software, 0);  // 006E24C9..CE
        self->discard = 0;                                                                 // 006E24D8
    } catch (...) {
        unwind_sound_adv(self, built);
        throw;
    }
    return self;                                                                           // 006E24E2
}

// ---- 006E2520 dtor (thiscall, RET); the checkbox member is destroyed with ~CSWGuiButtonToggle 0041CAD0 (006E25D8).
void __fastcall kotor_m1_options_sound_adv_destruct_006e2520(OptionsSoundAdvanced* self, void*) {
    gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);                        // 006E253D..4B
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                       // 006E2550..5B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                          // 006E2560..6B
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                          // 006E2570..7B
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                          // 006E2580..8B
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                          // 006E2590..9B
    gui_load_button_destruct_00419da0(&self->btn_eax_left, nullptr);                      // 006E25A0..AB
    gui_load_button_destruct_00419da0(&self->btn_eax_right, nullptr);                     // 006E25B0..BB
    gui_load_button_destruct_00419da0(&self->btn_eax, nullptr);                           // 006E25C0..CB
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_force_software, nullptr);           // 006E25D0..D8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                      // 006E25DD..E7
}
// ---- 006E2500 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_sound_adv_deleting_destruct_006e2500(OptionsSoundAdvanced* self, void*, Word flags) {
    kotor_m1_options_sound_adv_destruct_006e2520(self, nullptr);                          // 006E2503
    if (flags & 1u) live_free(self);                                                      // 006E2508..15
    return self;                                                                          // 006E2518
}

// ---- 006E0F80 HandleInputEvent(event, pressed) (thiscall, RET 8). Jump table 006E1240 indexed by the byte table
// 006E1254: 00 04 04 04 04 01 00 04 04 04 04 04 04 04 04 04 04 02 03 (read from the image)
// -> 28h/2Eh 006E0FDB (Back / apply); 2Dh 006E0FCD; 39h 006E11F4; 3Ah 006E1209; others 006E121C.
void __fastcall kotor_m1_options_sound_adv_handle_input_006e0f80(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    OptionsSoundAdvanced* const self = static_cast<OptionsSoundAdvanced*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                                   // 006E0F9E..AA
        switch (event) {                                                                  // 006E0FB0..C6
        case 0x28: case 0x2E: {                                                           // 006E0FDB
            manager_of(self)->PlayGuiSound(0);                                            // 006E0FDB..DF
            if (self->discard != 0) {                                                     // 006E0FE4..EA
                self->discard = 0;                                                        // 006E0FEC
            } else {
                kotor_m1_options_sound_adv_set_new_options_006dfb60(self, nullptr);       // 006E0FF7..F9
                if (self->changed != 0) {                                                 // 006E0FFE..04
                    const Word force = word_at(options_object(), 0x4C) == 0 ? 1u : 0u;   // 006E100A..2B (sete dl)
                    kotor_m1_sound_reinitialize_005d63f0(sound(), nullptr,
                        static_cast<std::uint32_t>(self->eax_level), force);              // 006E101B..30 (eax level, force)
                    base_ini_reconstruction::PublicIni ini;                               // [esp+0Ch]
                    ini.construct_005e6750();                                             // 006E1035..39 (CExoIni 005E6750)
                    try {
                        String value;                                                     // 006E103E..46 (005B3190)
                        kotor_reconstruction::StartupFormat005e5680(&value, "%d",
                            kotor_sound_path::SoundGetEax005d6450(sound(), nullptr));     // 006E104B..6B ("%d" 0073D720)
                        write_sound_option(ini, &value, "EAX");                           // 006E106E..E9 (0074D2A8)
                        void* const app = client_app();                                   // 006E10EE..F9
                        const unsigned char eax_now = static_cast<unsigned char>(
                            kotor_sound_path::SoundGetEax005d6450(sound(), nullptr));     // 006E10FC..03 (bl)
                        put_byte(kotor_fastcall_005ed700(app, nullptr), 0x49, eax_now);   // 006E1101..0A
                        kotor_reconstruction::StartupFormat005e5680(&value, "%d",
                            word_at(options_object(), 0x4C));                             // 006E110D..2E
                        write_sound_option(ini, &value, "Force Software");                // 006E1131..AC (0074E8EC)
                    }                                                                     // 006E11B1..BA ~value
                    catch (...) {
                        ini.destroy_005e67e0();                                           // EH state 0 (~CExoIni)
                        throw;
                    }
                    ini.destroy_005e67e0();                                               // 006E11BF..CB
                }
            }
            self->changed = 0;                                                            // 006E11D2..D5 (edi = 0)
            kotor_fastcall_0040be00(self->manager, nullptr);                              // 006E11D2..DB PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                           // 006E11E0..EF
            break;
        }
        case 0x2D:                                                                        // 006E0FCD
            manager_of(self)->PlayGuiSound(0);
            break;
        case 0x39: {                                                                      // 006E11F4: LB_DESC vslot +3C(31h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x31, 1);
            break;
        }
        case 0x3A: {                                                                      // 006E1209: LB_DESC vslot +3C(32h, 1)
            gui_lb::ListBox* const lb = &self->lb_desc;
            gui_lb::vt<gui_lb::ControlCallVtbl>(static_cast<Control*>(lb))->handle_input_event(lb, nullptr, 0x32, 1);
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);                 // 006E121C..26 (00409E60)
}

// ---- 006DFC90 SetUpOptions (thiscall, RET)
void __fastcall kotor_m1_options_sound_adv_set_up_options_006dfc90(OptionsSoundAdvanced* self, void*) noexcept(false) {
    void* const options = options_object();                                              // 006DFC90..9C
    if (options) {                                                                        // 006DFCA3..A5
        self->eax_level = byte_at(options, 0x49);                                         // 006DFCA7..AB
        kotor_m1_options_sound_adv_set_eax_text_006ddd70(self, nullptr);                  // 006DFCB2..B4
        const Word force = word_at(options, 0x4C);                                        // 006DFCB9
        self->force_software = force;                                                     // 006DFCC2
        set_toggle(&self->cb_force_software, force);                                      // 006DFCBC..DE vslot +5C, +1C8 bit0
        kotor_m1_options_sound_adv_enable_eax_controls_006ddf40(self, nullptr,
            word_at(options, 0x4C) == 0 ? 1u : 0u);                                       // 006DFCE4..F1
    }
    self->changed = 0;                                                                    // 006DFCF8
}
// ---- 006DFB60 SetNewOptions (thiscall, RET): +49 = (byte)eax level, +4C = force software (GetClientOptions each time).
void __fastcall kotor_m1_options_sound_adv_set_new_options_006dfb60(OptionsSoundAdvanced* self, void*) {
    if (!options_object()) return;                                                        // 006DFB60..72
    put_byte(options_object(), 0x49, static_cast<unsigned char>(self->eax_level));        // 006DFB74..89
    put_word(options_object(), 0x4C, self->force_software);                               // 006DFB8C..A0
}
// ---- 006DDD70 SetEAXText (thiscall, RET; EH state 0 FuncInfo 00736728 covers an unused local CExoString)
void __fastcall kotor_m1_options_sound_adv_set_eax_text_006ddd70(OptionsSoundAdvanced* self, void*) noexcept(false) {
    String unused;                                                                        // 006DDD8B..8F (005B3190)
    void* const params = reinterpret_cast<unsigned char*>(&self->btn_eax.text) + 0x18;   // +488
    Word strref;
    switch (self->eax_level) {                                                            // 006DDD94..B3
    case 0: strref = 0xBDBDu; break;                                                      // 006DDDCA
    case 2: strref = 0xBDBFu; break;                                                      // 006DDDC3
    case 3: strref = 0xBDC0u; break;                                                      // 006DDDBC
    default: strref = 0xBDBEu; break;                                                     // 006DDDB5
    }
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);                    // 006DDDD5
    self->btn_eax_left.flags |= 2u;                                                       // 006DDDDA (+6E8)
    self->btn_eax_right.flags |= 2u;                                                      // 006DDDE1 (+524)
    const I32 best = best_eax_capped();                                                   // 006DDDE8..0A
    if (self->eax_level == 0) self->btn_eax_left.flags &= 0xFFFFFFFDu;                    // 006DDE0A..1B
    else if (self->eax_level == best) self->btn_eax_right.flags &= 0xFFFFFFFDu;           // 006DDE1D..21
}                                                                                         // 006DDE28..34 ~CExoString
// ---- 006DDF40 EnableEAXControls(enable) (thiscall, RET 4)
void __fastcall kotor_m1_options_sound_adv_enable_eax_controls_006ddf40(OptionsSoundAdvanced* self, void*, Word enable) {
    set_enabled(&self->btn_eax, enable);                                                  // 006DDF48..55 vslot +88
    gui_load_text_params_set_color_00414e10(reinterpret_cast<unsigned char*>(&self->btn_eax.text) + 0x18, nullptr,
        enable != 0 ? &borrowed_exit_color_007a23b4 : &kotor_m1_color_007a2408);          // 006DDF5B..70 (+488)
    set_enabled(&self->btn_eax_right, enable);                                            // 006DDF75..82
    set_enabled(&self->btn_eax_left, enable);                                             // 006DDF88..95
}
// ---- 006DDE50 SetDescription(strref): params +950, list +AE8, item +868 (extent +86C), text +938.
void __fastcall kotor_m1_options_sound_adv_set_description_006dde50(OptionsSoundAdvanced* self, void*, Word strref) noexcept(false) {
    describe(&self->desc_item, &self->lb_desc, strref);                                   // 006DDE70..DF21
}
}

// ---- 006DDFA0 OnEnterOption(control) (thiscall, RET 4): replaces the stack word with a strref and tail-jumps to
// SetDescription 006DDE50 (BDC1h for the checkbox, BDC2h for the three EAX buttons; nothing otherwise).
void __thiscall OptionsSoundAdvancedHandlers::OnEnterOption_006ddfa0(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    const Object* const c = object_of(control);
    if (c == &self->cb_force_software) {                                                  // 006DDFA4..A9
        kotor_m1_options_sound_adv_set_description_006dde50(self, nullptr, 0xBDC1u);      // 006DDFAB..B3
    } else if (c == &self->btn_eax || c == &self->btn_eax_right || c == &self->btn_eax_left) {   // 006DDFB8..D4
        kotor_m1_options_sound_adv_set_description_006dde50(self, nullptr, 0xBDC2u);      // 006DDFD6..DE
    }
}
// ---- 006DDFF0 OnDiscardChanges(control): discard = 1; JMP [vptr+54h] (OnBButtonPressed, same stack word).
void __thiscall OptionsSoundAdvancedHandlers::OnDiscardChanges_006ddff0(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    self->discard = 1;                                                                    // 006DDFF2
    gui_lb::vt<PanelButtonB54View>(static_cast<Object*>(self))->button_b54(self, nullptr, word_of(control));  // 006DDFFC
}
// ---- 006DFBB0 OnEAXLeft(control)
void __thiscall OptionsSoundAdvancedHandlers::OnEAXLeft_006dfbb0(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006DFBB7..BC
    if (self->eax_level <= 0) return;                                                     // 006DFBBE..C6
    --self->eax_level;                                                                    // 006DFBC8..CB
    kotor_m1_options_sound_adv_set_eax_text_006ddd70(self, nullptr);                      // 006DFBD1
    manager_of(self)->PlayGuiSound(1);                                                    // 006DFBD6..DB
    self->changed = 1;                                                                    // 006DFBE0
}
// ---- 006DFBF0 OnEAXRight(control)
void __thiscall OptionsSoundAdvancedHandlers::OnEAXRight_006dfbf0(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006DFBF7..FC
    const I32 best = best_eax_capped();                                                   // 006DFBFE..20
    if (self->eax_level == best) return;                                                  // 006DFC20..28
    ++self->eax_level;                                                                    // 006DFC2A..2B
    kotor_m1_options_sound_adv_set_eax_text_006ddd70(self, nullptr);                      // 006DFC33
    manager_of(self)->PlayGuiSound(1);                                                    // 006DFC38..3D
    self->changed = 1;                                                                    // 006DFC42
}
// ---- 006DFC50 OnForceSoftware(control)
void __thiscall OptionsSoundAdvancedHandlers::OnForceSoftware_006dfc50(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006DFC57..5C
    const Word value = self->force_software == 0 ? 1u : 0u;                               // 006DFC5E..68
    self->force_software = value;                                                         // 006DFC72
    kotor_m1_options_sound_adv_enable_eax_controls_006ddf40(self, nullptr, value == 0 ? 1u : 0u);   // 006DFC6B..7B
    self->changed = 1;                                                                    // 006DFC80
}
// ---- 006E1270 OnDefault(control)
void __thiscall OptionsSoundAdvancedHandlers::OnDefault_006e1270(kih::Control* control) {
    OptionsSoundAdvanced* const self = adv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E1277..7C
    kotor_m1_client_options_default_advanced_sound_0061d550(options_object(), nullptr);   // 006E127E..8E
    kotor_m1_options_sound_adv_set_up_options_006dfc90(self, nullptr);                    // 006E1293..95
    self->changed = 1;                                                                    // 006E129A
}
