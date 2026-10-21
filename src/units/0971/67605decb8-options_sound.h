#pragma once
// lane-m1-opt-sound-gfx: CSWGuiOptionsSound (0x16F0, layout "optsound") and CSWGuiOptionsSoundAdvanced (0x1320,
// layout "optsoundadv"), opened from CSWGuiOptionsMain::OnSoundOpt 006E3E00 / CSWGuiOptionsSound::OnAdvanced 006E2D70.
// Layouts established by the ctors 006E3550 / 006E20A0 (member ctor calls, InitControl tags) and the dtors
// 006E3C60 / 006E2520 (reverse member order). Original: swkotor.exe GOG K1, static disassembly only.
#include "f5fb02ae37-options_controls.h"

namespace kotor_m1_options {
struct OptionsSound : gui_load::Panel {               // +0000 CSWGuiPanel (0040B570 at 006E3577); vptr 007587C0 (006E3589)
    gui_lb::Slider sli_music;                          // +0064 "SLI_MUSIC" (006E369C)   strref A547h (006E38C7)
    gui_lb::Slider sli_vo;                             // +021C "SLI_VO"    (006E36CC)   strref A548h (006E38D1)
    gui_lb::Slider sli_fx;                             // +03D4 "SLI_FX"    (006E36FC)   strref A549h (006E38DB)
    gui_lb::Slider sli_movie;                          // +058C "SLI_MOVIE" (006E3732)   strref BB31h (006E38EE)
    gui_load::Label lbl_music;                         // +0744 "LBL_MUSIC" (006E3768)
    gui_load::Label lbl_fx;                            // +0884 "LBL_FX"    (006E37D4)
    gui_load::Label lbl_vo;                            // +09C4 "LBL_VO"    (006E379E)
    gui_load::Label lbl_movie;                         // +0B04 "LBL_MOVIE" (006E380A)
    gui_load::Label desc_item;                         // +0C44 description item (Initialize from LB_DESC proto 006E3973..8F)
    gui_load::Label lbl_title;                         // +0D84 "LBL_TITLE" (006E3876)
    gui_lb::ListBox lb_desc;                           // +0EC4 "LB_DESC"   (006E38AC); +1185 = lb+2C1 (006E3902), +1190 = proto
    gui_load::Button btn_back;                         // +11A4 "BTN_BACK"  (006E391B)
    gui_load::Button btn_default;                      // +1368 "BTN_DEFAULT" (006E394B)
    gui_load::Button btn_advanced;                     // +152C "BTN_ADVANCED" (006E3840) strref BFB1h (006E38F8)
};
static_assert(offsetof(OptionsSound, sli_music) == 0x64 && offsetof(OptionsSound, sli_vo) == 0x21C &&
              offsetof(OptionsSound, sli_fx) == 0x3D4 && offsetof(OptionsSound, sli_movie) == 0x58C &&
              offsetof(OptionsSound, lbl_music) == 0x744 && offsetof(OptionsSound, lbl_fx) == 0x884 &&
              offsetof(OptionsSound, lbl_vo) == 0x9C4 && offsetof(OptionsSound, lbl_movie) == 0xB04 &&
              offsetof(OptionsSound, desc_item) == 0xC44 && offsetof(OptionsSound, lbl_title) == 0xD84 &&
              offsetof(OptionsSound, lb_desc) == 0xEC4 && offsetof(OptionsSound, btn_back) == 0x11A4 &&
              offsetof(OptionsSound, btn_default) == 0x1368 && offsetof(OptionsSound, btn_advanced) == 0x152C &&
              sizeof(OptionsSound) == 0x16F0, "CSWGuiOptionsSound 0x16F0 (operator new 006E3E23)");

struct OptionsSoundAdvanced : gui_load::Panel {       // +0000 CSWGuiPanel (006E20CB); vptr 00758550 (006E20DD)
    OptionsCheckbox cb_force_software;                 // +0064 "CB_FORCESOFTWARE" (006E21B4); vptr 00758258 (006E20E8); strref BDC1h
    gui_lb::I32 eax_level;                             // +0318 (SetUpOptions 006DFCAB; OnEAXLeft/Right; SetNewOptions 006DFB7E)
    gui_load::Button btn_eax;                          // +031C "BTN_EAX" (006E21E9); strref BDC2h (006E2385); text params +488
    gui_load::Button btn_eax_right;                    // +04E0 "BTN_EAXRIGHT" (006E221A); flags +524
    gui_load::Button btn_eax_left;                     // +06A4 "BTN_EAXLEFT" (006E224A); flags +6E8
    gui_load::Label desc_item;                         // +0868 description item (006E239B..B7)
    gui_load::Label lbl_title;                         // +09A8 "LBL_TITLE" (006E2280)
    gui_lb::ListBox lb_desc;                           // +0AE8 "LB_DESC" (006E22B6); +0DA9 = lb+2C1 (006E238F), +0DB4 proto
    gui_load::Button btn_back;                         // +0DC8 "BTN_BACK" (006E22EC)
    gui_load::Button btn_default;                      // +0F8C "BTN_DEFAULT" (006E2322)
    gui_load::Button btn_cancel;                       // +1150 "BTN_CANCEL" (006E2358)
    gui_load::Word force_software;                     // +1314 (006DFCC2, 006DFC72, 006DFB95)
    gui_load::Word changed;                            // +1318 (006DFBE0/006DFC42/006DFC80/006E129A = 1; 006DFCF8/006E11D5 = 0)
    gui_load::Word discard;                            // +131C (006E24D8 = 0; 006DDFF2 = 1; 006E0FEC = 0)
};
static_assert(offsetof(OptionsSoundAdvanced, cb_force_software) == 0x64 && offsetof(OptionsSoundAdvanced, eax_level) == 0x318 &&
              offsetof(OptionsSoundAdvanced, btn_eax) == 0x31C && offsetof(OptionsSoundAdvanced, btn_eax_right) == 0x4E0 &&
              offsetof(OptionsSoundAdvanced, btn_eax_left) == 0x6A4 && offsetof(OptionsSoundAdvanced, desc_item) == 0x868 &&
              offsetof(OptionsSoundAdvanced, lbl_title) == 0x9A8 && offsetof(OptionsSoundAdvanced, lb_desc) == 0xAE8 &&
              offsetof(OptionsSoundAdvanced, btn_back) == 0xDC8 && offsetof(OptionsSoundAdvanced, btn_default) == 0xF8C &&
              offsetof(OptionsSoundAdvanced, btn_cancel) == 0x1150 && offsetof(OptionsSoundAdvanced, force_software) == 0x1314 &&
              offsetof(OptionsSoundAdvanced, changed) == 0x1318 && offsetof(OptionsSoundAdvanced, discard) == 0x131C &&
              sizeof(OptionsSoundAdvanced) == 0x1320, "CSWGuiOptionsSoundAdvanced 0x1320 (operator new 006E2D93)");

// CSWGuiControlEvent callbacks (thiscall members, RET 4; single-inheritance views, D47).
struct OptionsSoundHandlers : kih::GuiObject {
    void __thiscall OnEnterSlider_006dfaa0(kih::Control* control);        // event 0 on the four sliders
    void __thiscall OnMoveVolumeSlider_006df9b0(kih::Control* control);   // events 2Fh/30h/3Fh/40h/27h; SetUpOptions
    void __thiscall OnDefault_006e0f50(kih::Control* control);            // event 2Ah sliders, 27h BTN_DEFAULT
    void __thiscall OnAdvanced_006e2d70(kih::Control* control);           // event 27h BTN_ADVANCED
};
struct OptionsSoundAdvancedHandlers : kih::GuiObject {
    void __thiscall OnEnterOption_006ddfa0(kih::Control* control);        // event 0 on checkbox / EAX buttons
    void __thiscall OnDiscardChanges_006ddff0(kih::Control* control);     // 27h BTN_CANCEL
    void __thiscall OnEAXLeft_006dfbb0(kih::Control* control);            // 27h/2Dh BTN_EAXLEFT
    void __thiscall OnEAXRight_006dfbf0(kih::Control* control);           // 27h/2Dh BTN_EAXRIGHT
    void __thiscall OnForceSoftware_006dfc50(kih::Control* control);      // 27h/2Dh CB_FORCESOFTWARE
    void __thiscall OnDefault_006e1270(kih::Control* control);            // 27h BTN_DEFAULT
};
}

extern "C" {
kotor_m1_options::OptionsSound* __fastcall kotor_m1_options_sound_construct_006e3550(kotor_m1_options::OptionsSound*, void*,
                                                                                     gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_options_sound_destruct_006e3c60(kotor_m1_options::OptionsSound*, void*);
void* __fastcall kotor_m1_options_sound_deleting_destruct_006e3c40(kotor_m1_options::OptionsSound*, void*, gui_load::Word);
void __fastcall kotor_m1_options_sound_handle_input_006ddb90(gui_load::Object*, void*, gui_load::I32 event, gui_load::I32 pressed);
void __fastcall kotor_m1_options_sound_set_up_options_006dfad0(kotor_m1_options::OptionsSound*, void*);
void __fastcall kotor_m1_options_sound_set_description_006ddc80(kotor_m1_options::OptionsSound*, void*, gui_load::Word strref) noexcept(false);
extern const kotor_m1_options::OptionsPanelMethods<kotor_m1_options::OptionsSound> kotor_m1_options_sound_methods_007587c0;

kotor_m1_options::OptionsSoundAdvanced* __fastcall kotor_m1_options_sound_adv_construct_006e20a0(kotor_m1_options::OptionsSoundAdvanced*,
    void*, gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_options_sound_adv_destruct_006e2520(kotor_m1_options::OptionsSoundAdvanced*, void*);
void* __fastcall kotor_m1_options_sound_adv_deleting_destruct_006e2500(kotor_m1_options::OptionsSoundAdvanced*, void*, gui_load::Word);
void __fastcall kotor_m1_options_sound_adv_handle_input_006e0f80(gui_load::Object*, void*, gui_load::I32 event, gui_load::I32 pressed) noexcept(false);
void __fastcall kotor_m1_options_sound_adv_set_up_options_006dfc90(kotor_m1_options::OptionsSoundAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_sound_adv_set_new_options_006dfb60(kotor_m1_options::OptionsSoundAdvanced*, void*);
void __fastcall kotor_m1_options_sound_adv_set_eax_text_006ddd70(kotor_m1_options::OptionsSoundAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_sound_adv_enable_eax_controls_006ddf40(kotor_m1_options::OptionsSoundAdvanced*, void*, gui_load::Word enable);
void __fastcall kotor_m1_options_sound_adv_set_description_006dde50(kotor_m1_options::OptionsSoundAdvanced*, void*, gui_load::Word strref) noexcept(false);
extern const kotor_m1_options::OptionsPanelMethods<kotor_m1_options::OptionsSoundAdvanced> kotor_m1_options_sound_adv_methods_00758550;
}
