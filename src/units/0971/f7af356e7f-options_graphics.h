#pragma once
// lane-m1-opt-sound-gfx: CSWGuiOptionsGraphics (0x18C0, layout "optgraphics"), CSWGuiOptionsResolution (0x810,
// layout "optresolution") and CSWGuiOptionsGraphicsAdvanced (0x2338, layout "optgraphicsadv"); opened from
// CSWGuiOptionsMain::OnGraphicsOpt 006E3D80, CSWGuiOptionsGraphics::OnResolution 006E1660 / OnAdvanced 006E2CF0.
// Layouts established by the ctors 006E2E70 / 006E0710 / 006E16E0 and the dtors 006E3440 / 006E0C70 / 006E1F40.
// Original: swkotor.exe GOG K1, static disassembly only.
#include "f5fb02ae37-options_controls.h"

namespace kotor_m1_options {
struct OptionsGraphics : gui_load::Panel {            // +0000 CSWGuiPanel (006E2E9B); vptr 007586F8 (006E2EAD)
    gui_lb::Slider sli_gamma;                          // +0064 "SLI_GAMMA" (006E2FC2); strref A546h (006E31B9)
    gui_load::Label lbl_gamma;                         // +021C "LBL_GAMMA" (006E3056); strref A546h (006E31BF)
    gui_load::Label desc_item;                         // +035C description item (006E3272..8E)
    gui_load::Label lbl_title;                         // +049C "LBL_TITLE" (006E2FEC)
    gui_lb::ListBox lb_desc;                           // +05DC "LB_DESC" (006E3021); +089D = lb+2C1 (006E3203), +08A8 proto
    gui_load::Button btn_resolution;                   // +08BC "BTN_RESOLUTION" (006E308B); strref BB5Fh
    gui_load::Button btn_resolution_left;              // +0A80 "BTN_RESOLUTIONLEFT" (006E30C0); strref BB5Fh
    gui_load::Button btn_resolution_right;             // +0C44 "BTN_RESOLUTIONRIGHT" (006E30F5); strref BB5Fh
    OptionsCheckbox cb_shadows;                        // +0E08 "CB_SHADOWS" (006E312A); vptr 00758258 (006E2F3A); strref BB4Fh
    OptionsCheckbox cb_grass;                          // +10BC "CB_GRASS" (006E315F); vptr 00758258 (006E2F52); strref BB53h
    gui_load::Button btn_advanced;                     // +1370 "BTN_ADVANCED" (006E3194); strref BFAEh
    gui_load::Button btn_back;                         // +1534 "BTN_BACK" (006E321B)
    gui_load::Button btn_default;                      // +16F8 "BTN_DEFAULT" (006E324B)
    gui_load::Word reinit_pending;                     // +18BC: written only 0 (006E33FA ctor, 006DECE9 Back); read 006DEB47
};
static_assert(offsetof(OptionsGraphics, sli_gamma) == 0x64 && offsetof(OptionsGraphics, lbl_gamma) == 0x21C &&
              offsetof(OptionsGraphics, desc_item) == 0x35C && offsetof(OptionsGraphics, lbl_title) == 0x49C &&
              offsetof(OptionsGraphics, lb_desc) == 0x5DC && offsetof(OptionsGraphics, btn_resolution) == 0x8BC &&
              offsetof(OptionsGraphics, btn_resolution_left) == 0xA80 && offsetof(OptionsGraphics, btn_resolution_right) == 0xC44 &&
              offsetof(OptionsGraphics, cb_shadows) == 0xE08 && offsetof(OptionsGraphics, cb_grass) == 0x10BC &&
              offsetof(OptionsGraphics, btn_advanced) == 0x1370 && offsetof(OptionsGraphics, btn_back) == 0x1534 &&
              offsetof(OptionsGraphics, btn_default) == 0x16F8 && offsetof(OptionsGraphics, reinit_pending) == 0x18BC &&
              sizeof(OptionsGraphics) == 0x18C0, "CSWGuiOptionsGraphics 0x18C0 (operator new 006E3DA3)");

struct OptionsResolution : gui_load::Panel {          // +0000 CSWGuiPanel (006E073E); vptr 00758348 (006E0751)
    gui_load::Label lbl_resolution;                    // +0064 "LBL_RESOLUTION" (006E07D3)
    gui_lb::ListBox lb_resolutions;                    // +01A4 "LB_RESOLUTIONS" (006E080F); +0470 proto
    gui_load::Button btn_ok;                           // +0484 "BTN_OK" (006E0845)
    gui_load::Button btn_cancel;                       // +0648 "BTN_CANCEL" (006E087B)
    gui_load::Word reinit_pending;                     // +080C (006E0C01 = 0; 006DF981 = 1 inside OnResolutionChosen; 006E0EED = 0)
};
static_assert(offsetof(OptionsResolution, lbl_resolution) == 0x64 && offsetof(OptionsResolution, lb_resolutions) == 0x1A4 &&
              offsetof(OptionsResolution, btn_ok) == 0x484 && offsetof(OptionsResolution, btn_cancel) == 0x648 &&
              offsetof(OptionsResolution, reinit_pending) == 0x80C && sizeof(OptionsResolution) == 0x810,
              "CSWGuiOptionsResolution 0x810 (operator new 006E1686)");

struct OptionsGraphicsAdvanced : gui_load::Panel {    // +0000 CSWGuiPanel (006E170B); vptr 007584A0 (006E171B)
    gui_load::Label desc_item;                         // +0064 description item (006E1C5E..74)
    gui_load::Label lbl_title;                         // +01A4 "LBL_TITLE" (006E1889)
    gui_lb::ListBox lb_desc;                           // +02E4 "LB_DESC" (006E18BE); +05A5 = lb+2C1 (006E1BB1), +05B0 proto
    OptionsCheckbox cb_framebuffer;                    // +05C4 "CB_FRAMEBUFF" (006E1A2B); strref BB55h
    OptionsCheckbox cb_soft_shadows;                   // +0878 "CB_SOFTSHADOWS" (006E1AF9); strref BB51h
    OptionsCheckbox cb_vsync;                          // +0B2C "CB_VSYNC" (006E1B2E); strref BB61h
    gui_load::Button btn_anisotropy;                   // +0DE0 "BTN_ANISOTROPY" (006E1A5A); strref BB59h; text params +F4C
    gui_load::Button btn_anisotropy_left;              // +0FA4 "BTN_ANISOTROPYLEFT" (006E1A8F); flags +FE8
    gui_load::Button btn_anisotropy_right;             // +1168 "BTN_ANISOTROPYRIGHT" (006E1AC4); flags +11AC
    gui_load::Button btn_antialias;                    // +132C "BTN_ANTIALIAS" (006E18ED); strref BB63h; text params +1498
    gui_load::Button btn_antialias_left;               // +14F0 "BTN_ANTIALIASLEFT" (006E1922); flags +1534
    gui_load::Button btn_antialias_right;              // +16B4 "BTN_ANTIALIASRIGHT" (006E195C); flags +16F8
    gui_load::Button btn_texqual;                      // +1878 "BTN_TEXQUAL" (006E198C); strref BB5Dh; text params +19E4
    gui_load::Button btn_texqual_left;                 // +1A3C "BTN_TEXQUALLEFT" (006E19C1); flags +1A80
    gui_load::Button btn_texqual_right;                // +1C00 "BTN_TEXQUALRIGHT" (006E19F6); flags +1C44
    gui_load::Button btn_back;                         // +1DC4 "BTN_BACK" (006E1BCD)
    gui_load::Button btn_default;                      // +1F88 "BTN_DEFAULT" (006E1C02)
    gui_load::Button btn_cancel;                       // +214C "BTN_CANCEL" (006E1C37)
    gui_lb::I32 tex_qual;                              // +2310 (SetUpOptions 006DF5CC; OnTexQualLeft/Right)
    gui_lb::I32 tex_qual_max;                          // +2314 rows of texpacks.2da - 1 (006DF653..57)
    gui_lb::I32 anti_alias;                            // +2318 (006DF5C2; OnAntiAliasLeft/Right)
    gui_lb::I32 anisotropy;                            // +231C (006DF51E; OnAnisotropyLeft/Right)
    gui_load::Word frame_buffer;                       // +2320 (006DF4CC / 006DF514; OnFrameBuffer)
    gui_load::Word vsync;                              // +2324 (006DF5A0; OnVSync)
    gui_load::Word soft_shadows;                       // +2328 (006DF53F / 006DF587; OnSoftShadows)
    gui_load::Word discard;                            // +232C (006E1EF0 = 0; 006DDB82 = 1; 006E020B = 0)
    gui_load::Word changed;                            // +2330 (006E1EF6 = 0; set by the Set*Text / On* handlers)
    OptionsGraphics* parent;                           // +2334 ctor arg 2 (006E1EFC); SetUpOptions 006E042F
};
static_assert(offsetof(OptionsGraphicsAdvanced, desc_item) == 0x64 && offsetof(OptionsGraphicsAdvanced, lbl_title) == 0x1A4 &&
              offsetof(OptionsGraphicsAdvanced, lb_desc) == 0x2E4 && offsetof(OptionsGraphicsAdvanced, cb_framebuffer) == 0x5C4 &&
              offsetof(OptionsGraphicsAdvanced, cb_soft_shadows) == 0x878 && offsetof(OptionsGraphicsAdvanced, cb_vsync) == 0xB2C &&
              offsetof(OptionsGraphicsAdvanced, btn_anisotropy) == 0xDE0 && offsetof(OptionsGraphicsAdvanced, btn_anisotropy_left) == 0xFA4 &&
              offsetof(OptionsGraphicsAdvanced, btn_anisotropy_right) == 0x1168 && offsetof(OptionsGraphicsAdvanced, btn_antialias) == 0x132C &&
              offsetof(OptionsGraphicsAdvanced, btn_antialias_left) == 0x14F0 && offsetof(OptionsGraphicsAdvanced, btn_antialias_right) == 0x16B4 &&
              offsetof(OptionsGraphicsAdvanced, btn_texqual) == 0x1878 && offsetof(OptionsGraphicsAdvanced, btn_texqual_left) == 0x1A3C &&
              offsetof(OptionsGraphicsAdvanced, btn_texqual_right) == 0x1C00 && offsetof(OptionsGraphicsAdvanced, btn_back) == 0x1DC4 &&
              offsetof(OptionsGraphicsAdvanced, btn_default) == 0x1F88 && offsetof(OptionsGraphicsAdvanced, btn_cancel) == 0x214C &&
              offsetof(OptionsGraphicsAdvanced, tex_qual) == 0x2310 && offsetof(OptionsGraphicsAdvanced, anisotropy) == 0x231C &&
              offsetof(OptionsGraphicsAdvanced, discard) == 0x232C && offsetof(OptionsGraphicsAdvanced, parent) == 0x2334 &&
              sizeof(OptionsGraphicsAdvanced) == 0x2338, "CSWGuiOptionsGraphicsAdvanced 0x2338 (operator new 006E2D13)");

struct OptionsGraphicsHandlers : kih::GuiObject {
    void __thiscall OnEnterSlot_006e0140(kih::Control* control);         // event 0 BTN_RESOLUTION / CB_SHADOWS / CB_GRASS
    void __thiscall OnEnterSlider_006e0150(kih::Control* control);       // event 0 SLI_GAMMA
    void __thiscall OnMoveGammaSlider_006ded30(kih::Control* control);   // 2Fh/30h/3Fh/40h/27h SLI_GAMMA; SetUpOptions
    void __thiscall OnDefault_006e0190(kih::Control* control);           // 2Ah SLI_GAMMA, 27h BTN_DEFAULT
    void __thiscall OnShadows_006dedb0(kih::Control* control);           // 27h CB_SHADOWS
    void __thiscall OnGrass_006dee00(kih::Control* control);             // 27h CB_GRASS
    void __thiscall OnResolution_006e1660(kih::Control* control);        // 27h BTN_RESOLUTION
    void __thiscall OnAdvanced_006e2cf0(kih::Control* control);          // 27h BTN_ADVANCED
};
struct OptionsResolutionHandlers : kih::GuiObject {
    void __thiscall OnResolutionChosen_006df690(kih::Control* control);  // 27h BTN_OK / list entries (D83 apply trap)
};
struct OptionsGraphicsAdvancedHandlers : kih::GuiObject {
    void __thiscall OnFrameBuffer_006ddb10(kih::Control* control);
    void __thiscall OnSoftShadows_006ddb30(kih::Control* control);
    void __thiscall OnVSync_006ddb60(kih::Control* control);
    void __thiscall OnDiscardChanges_006ddb80(kih::Control* control);
    void __thiscall OnEnterSlot_006e0460(kih::Control* control);
    void __thiscall OnDefault_006e0470(kih::Control* control);
    void __thiscall OnTexQualLeft_006e04a0(kih::Control* control);
    void __thiscall OnTexQualRight_006e04e0(kih::Control* control);
    void __thiscall OnAntiAliasLeft_006e0520(kih::Control* control);
    void __thiscall OnAntiAliasRight_006e05e0(kih::Control* control);
    void __thiscall OnAnisotropyLeft_006e0680(kih::Control* control);
    void __thiscall OnAnisotropyRight_006e06d0(kih::Control* control);
};
}

extern "C" {
kotor_m1_options::OptionsGraphics* __fastcall kotor_m1_options_graphics_construct_006e2e70(kotor_m1_options::OptionsGraphics*, void*,
                                                                                           gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_options_graphics_destruct_006e3440(kotor_m1_options::OptionsGraphics*, void*);
void* __fastcall kotor_m1_options_graphics_deleting_destruct_006e3420(kotor_m1_options::OptionsGraphics*, void*, gui_load::Word);
void __fastcall kotor_m1_options_graphics_handle_input_006deae0(gui_load::Object*, void*, gui_load::I32 event, gui_load::I32 pressed);
void __fastcall kotor_m1_options_graphics_set_up_options_006def30(kotor_m1_options::OptionsGraphics*, void*);
void __fastcall kotor_m1_options_graphics_set_description_006dee40(kotor_m1_options::OptionsGraphics*, void*, gui_load::Word strref) noexcept(false);
extern const kotor_m1_options::OptionsPanelMethods<kotor_m1_options::OptionsGraphics> kotor_m1_options_graphics_methods_007586f8;

kotor_m1_options::OptionsResolution* __fastcall kotor_m1_options_resolution_construct_006e0710(kotor_m1_options::OptionsResolution*, void*,
                                                                                               gui_load::Object* manager) noexcept(false);
void __fastcall kotor_m1_options_resolution_destruct_006e0c70(kotor_m1_options::OptionsResolution*, void*);
void* __fastcall kotor_m1_options_resolution_deleting_destruct_006e0c50(kotor_m1_options::OptionsResolution*, void*, gui_load::Word);
void __fastcall kotor_m1_options_resolution_handle_input_006e0cf0(gui_load::Object*, void*, gui_load::I32 event, gui_load::I32 pressed);
extern const kotor_m1_options::OptionsPanelMethods<kotor_m1_options::OptionsResolution> kotor_m1_options_resolution_methods_00758348;

kotor_m1_options::OptionsGraphicsAdvanced* __fastcall kotor_m1_options_graphics_adv_construct_006e16e0(
    kotor_m1_options::OptionsGraphicsAdvanced*, void*, gui_load::Object* manager, kotor_m1_options::OptionsGraphics* parent) noexcept(false);
void __fastcall kotor_m1_options_graphics_adv_destruct_006e1f40(kotor_m1_options::OptionsGraphicsAdvanced*, void*);
void* __fastcall kotor_m1_options_graphics_adv_deleting_destruct_006e1f20(kotor_m1_options::OptionsGraphicsAdvanced*, void*, gui_load::Word);
void __fastcall kotor_m1_options_graphics_adv_handle_input_006e01c0(gui_load::Object*, void*, gui_load::I32 event, gui_load::I32 pressed);
void __fastcall kotor_m1_options_graphics_adv_set_up_options_006df480(kotor_m1_options::OptionsGraphicsAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_graphics_adv_set_new_options_006defe0(kotor_m1_options::OptionsGraphicsAdvanced*, void*);
void __fastcall kotor_m1_options_graphics_adv_set_tex_qual_text_006df070(kotor_m1_options::OptionsGraphicsAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_graphics_adv_set_anti_alias_text_006df1c0(kotor_m1_options::OptionsGraphicsAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_graphics_adv_set_anisotropy_text_006df2c0(kotor_m1_options::OptionsGraphicsAdvanced*, void*) noexcept(false);
void __fastcall kotor_m1_options_graphics_adv_set_description_006df390(kotor_m1_options::OptionsGraphicsAdvanced*, void*, gui_load::Word strref) noexcept(false);
extern const kotor_m1_options::OptionsPanelMethods<kotor_m1_options::OptionsGraphicsAdvanced> kotor_m1_options_graphics_adv_methods_007584a0;

// Renderer / client leaves authored here (single owner; names follow the existing kotor_cdecl_/m1 spellings).
unsigned char __cdecl kotor_cdecl_0045fa20();                                                   // AurCheckSoftShadowsAvailable
std::int32_t __cdecl kotor_cdecl_0044f090();                                                    // AurQueryMaximumAnisotropy
std::int32_t __fastcall kotor_m1_client_is_valid_resolution_005ee2b0(void* client_app, void*, std::int32_t width, std::int32_t height); // RET 8
std::uint32_t __fastcall kotor_m1_client_allow_high_frequency_005ee2c0(void* client_app, void*);  // RET
}
