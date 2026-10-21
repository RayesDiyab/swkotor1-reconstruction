// lane-m1-opt-sound-gfx (Milestone 1): Options -> GRAPHICS sub-panel, its RESOLUTION and ADVANCED sub-panels.
//   006E2E70 CSWGuiOptionsGraphics::CSWGuiOptionsGraphics   006E3440 / 006E3420 dtors   007586F8 method table
//   006DEAE0 HandleInputEvent (Back; +18BC set -> ReInitAurora branch 006DEB55..ED09)
//            [fullscreen lane 2026-10-20: the former D83 branch trap 006DEB55 is replaced by the transcribed branch;
//             +18BC still has no non-zero writer (006E33FA ctor = 0, 006DECE9 = 0), so it stays unreached]
//   006DEF30 SetUpOptions  006DEE40 SetDescription  006DED30 OnMoveGammaSlider (CAurInternalGL +24 ResetGammaRamp)
//   006DEDB0 OnShadows  006DEE00 OnGrass  006E0140 OnEnterSlot  006E0150 OnEnterSlider  006E0190 OnDefault
//   006E1660 OnResolution  006E2CF0 OnAdvanced
//   006E0710 CSWGuiOptionsResolution::CSWGuiOptionsResolution (EnumDisplaySettingsA list)  006E0C70 / 006E0C50 dtors
//   006E0CF0 HandleInputEvent  00758348 method table
//   006DF690 CSWGuiOptionsResolution::OnResolutionChosen: full apply path 006DF6C8..006DF98D
//            [fullscreen lane 2026-10-20: the former D83 APPLY-MODE trap 006DF6C8 and the 006E0CF0 +80C ReInitAurora
//             branch trap 006E0D65 are replaced by the transcribed bodies; CClientExoApp::SetVideoMode 005ED8D0 and
//             CSWGuiManager::ReloadToolTipPanel 0040A350 are added here (no other provider)]
//            (SetVideoMode 005ED8D0 -> 005F1830 -> CGuiInGame::ResetInterfaceForSize, TRAP #69; renderer re-init)
//   006E16E0 CSWGuiOptionsGraphicsAdvanced::CSWGuiOptionsGraphicsAdvanced  006E1F40 / 006E1F20 dtors  007584A0 table
//   006E01C0 HandleInputEvent (Back: SetNewOptions; +2330 set -> apply 006E0237..03FD incl. ReInitAurora 00403800)
//            [gfxapply 2026-10-20: the former D83 APPLY-MODE trap 006E0237 is replaced by the transcribed apply path]
//   006DF480 SetUpOptions 006DEFE0 SetNewOptions 006DF070 SetTexQualText 006DF1C0 SetAntiAliasText
//   006DF2C0 SetAnisotropyText 006DF390 SetDescription and the twelve On* callbacks (006DDB10..006E06D0)
//   CClientOptions 0061D6B0 SetShadows 0061D6D0 SetSoftShadows 0061D6F0 SetGrass 0061D730 SetTexQual
//                  0061D9A0 SetDefaultGraphicsOptions 0061D520 SetDefaultAdvancedGraphicsOptions
//   CClientExoApp 005EE2B0 IsValidResolution 005EE2C0 GetAllowHighMonitorFrequency
//   Aurora 0045FA20 AurCheckSoftShadowsAvailable 0044F090 AurQueryMaximumAnisotropy
// Original: swkotor.exe GOG K1 (sha256 9c10e045...91435), static disassembly only; each behaviour cites its
// instruction addresses. D1/D21/D24/D41/D47 as lane-m1-options; D30-FINAL: OnMoveGammaSlider mirrors fild/fidiv/
// fmul dword/fsubr dword/fstp dword as double intermediates rounded once to float at the fstp; 0044F090 uses the
// genuine _ftol port 006FAE8C; GetAllowHighMonitorFrequency's fild/fcomp 85.0f is an exact integer comparison.
#include "f7af356e7f-options_graphics.h"
#include "9c420a0591-kotor_unreachable_trap.h"                     // D83 shared fatal trap (lane-m48-traps)
#include "159c6e17c9-aurora_window_slots.h"          // kotor_aurora_window::table_of (D41 typed 00741518 view)
#include "5f3d4d0193-load.h"               // c2da_current::Table, 004143B0 (lane-startup-misc D38)
#include "e9348ddff5-video_mode_settings.h"            // options_video::video_mode_bindings (0078D1DC), 005F0C60
#include "d0c1f4e0b9-providers.h"  // Options setters
#include "186fd52797-base_ini.h"                                     // base_ini_reconstruction::PublicIni (CExoIni 005E6750 / 005E67C0 / 005E67E0)
#include <windows.h>
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
namespace kotor_menu20 {                                  // token-identical to menu_targets.h (service-menu20-4 owner)
using Word=std::uint32_t;
struct String {char* volatile data;Word volatile capacity;
 __declspec(noinline) std::int32_t __thiscall as_int_005e5630();
};
}
namespace kotor_renderer_gl_cells {                     // gl-renderer-gl-cells (renderer_gl_cells.h)
extern std::uint32_t capabilities_007bb85c;
extern std::uint32_t ati_soft_gate_007bb860;
extern std::uint32_t cache_soft_0078e520;
extern std::uint32_t cache_ati_soft_0078e524;
extern float max_anisotropy_007a6858;
}
namespace kotor_renderer_gl_procs { extern std::uint32_t mask_0078e4e4, mask_0078e4dc, mask_0078e4c8, mask_0078e4d0; }  // gl-renderer-gl-procs
namespace kotor_renderer_init { std::int32_t __cdecl QueryAntiAliasingMode_0044f140(std::int32_t); }                 // gl-aa-query
namespace lane_config_required {                        // startup46-config-leaves / m48b-client-framebuffer
void __cdecl EnableShadows_0044ee20();
void __cdecl DisableShadows_0044f080();
void __cdecl EnableSoftShadows_0044ee50();
void __cdecl DisableSoftShadows_0044ee60();
void __cdecl EnableGrass_0044ee30();
void __cdecl DisableGrass_0044ee40();
void __fastcall SetFrameBuffer_0061d750(void*, void*, std::int32_t);
}
namespace client_options { void __fastcall ClientExoAppSetTexturePack_005ed8f0(void* app, void* unused_edx, unsigned char index); } // lane-config
extern "C" {
void __cdecl kotor_cdecl_0044ef60();                                                 // AurDisableVertexBufferObjects (lane-routes window_leaves)
void __cdecl kotor_cdecl_0044ef70();                                                 // AurDisableVertexBufferObjectsWriteOnly (lane-routes)
void __cdecl kotor_cdecl_0044ef80();                                                 // AurEnableVertexBufferObjectsWriteOnly (lane-routes)
}
extern "C" {
unsigned char __cdecl kotor_cdecl_0045fab0();                                       // AurCheckFrameBufferModificationsAvailable
void __cdecl genuine_x87_ftol_006fae8c();                                            // renderer-x87-ftol-alias (ST0 -> EDX:EAX)
void __fastcall kotor_fastcall_0041c1d0(gui_lb::ListBox* self, void*, kotor_reconstruction::ArrayMutations* controls,
                                        gui_load::Word suppress, gui_load::Word restore, gui_load::Word variable) noexcept(false); // 0041C1D0
__declspec(noinline) std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, std::int32_t, c2da_current::String*, std::int32_t*);
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
menu_providers_40_49::Options* options_view(void* options) { return static_cast<menu_providers_40_49::Options*>(options); }
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
void init_desc_item(Label* item, const gui_lb::ListBox* lb) {               // label vslot +98(proto+4, proto, 1.0f)
    Control* const proto = lb->proto;
    gui_lb::vt<LabelInit98View>(static_cast<Object*>(item))->initialize98(item, nullptr, word_of(&proto->extent), word_of(proto),
                                                                         0x3F800000u);
}
// Common body of the options SetDescription family (006DEE40 / 006DF390; same sequence as 006DE340).
void describe(Label* item, gui_lb::ListBox* lb, Word strref) {
    void* const params = reinterpret_cast<unsigned char*>(&item->text) + 0x18;
    gui_load_text_params_set_strref_00415e50(params, nullptr, strref);                    // 00415E50
    String text(*reinterpret_cast<const String*>(params));                                // 005E5B00
    gui_load_listbox_clear_items_00419a90(lb, nullptr);                                   // 00419A90
    Extent extent = item->extent;
    gui_load::Text* const text_object = &item->text;
    if (text.BorrowedLength005e5790() > 0)                                                // 005E5790
        extent.height = static_cast<I32>(gui_lb::vt<TextIdeal08View>(text_object)->ideal_height(text_object, nullptr));
    else
        extent.height = gui_load_text_font_height_00414ee0(text_object, nullptr);          // 00414EE0
    item->vtbl->set_extent(item, nullptr, &extent);                                       // vslot +04
    Control* items[1] = { item };
    gui_m48_listbox_add_controls_0041c2c0(lb, nullptr, items, 1, 0, 0, 0);                // 0041C2C0
    gui_load_listbox_set_active_control_0041c160(lb, nullptr, item, 0);                   // 0041C160
}
gui_lb::ButtonToggle* toggle_self(gui_lb::ButtonToggle* toggle) {                         // vslot +5C (00641DB0 self_return)
    Object* const self = gui_lb::vt<ControlSelf5CView>(static_cast<Object*>(toggle))->slot5c(toggle, nullptr);
    return static_cast<gui_lb::ButtonToggle*>(static_cast<Button*>(static_cast<Control*>(self)));
}
void set_toggle(gui_lb::ButtonToggle* toggle, Word value) {                               // bit0 of +1C8 := value (xor/and 1/xor)
    gui_lb::ButtonToggle* const t = toggle_self(toggle);
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
// C2DA "texpacks" (006DF5E4..61C / 006DF08B..C5): operator new(54h), C2DA::C2DA(CResRef by value, 0) 00413CC0; the
// allocation is freed if the ctor throws (EH state 0). Returns null when operator new returned null.
c2da_current::Table* new_texpacks() {
    void* const memory = c2da_directory_new_006fa7e6(0x54);
    if (!memory) return nullptr;
    try {
        Ref name;
        name.construct_00406d80("texpacks");                                              // 0074E860 (CResRef 00406D80)
        return c2da_current_construct_00413cc0(memory, nullptr, name, 0);                 // 00413CC0
    } catch (...) {
        live_free(memory);
        throw;
    }
}
void delete_table(c2da_current::Table* table) {                                           // vslot +00 (deleting dtor) with 1
    const c2da_current::TableSlot0* methods;
    std::memcpy(&methods, table, 4);
    methods->deleting(table, nullptr, 1);
}
I32 ftol_float(const float* value) {                                                      // fld dword [p]; call 006FAE8C
    I32 r;
    __asm {
        mov ecx, value
        fld dword ptr [ecx]
        call genuine_x87_ftol_006fae8c
        mov r, eax
    }
    return r;
}
}

// ======================================================================== CClientOptions / CClientExoApp / Aurora leaves
extern "C" {
// 0061D6B0 SetShadows(enable) (RET 4): +24 = enable; AurEnableShadows 0044EE20 / AurDisableShadows 0044F080.
void __fastcall kotor_m1_client_options_set_shadows_0061d6b0(void* options, void*, std::uint32_t enable) {
    put_word(options, 0x24, enable);                                                      // 0061D6B6
    if (enable) lane_config_required::EnableShadows_0044ee20();                           // 0061D6BB
    else lane_config_required::DisableShadows_0044f080();                                 // 0061D6C3
}
// 0061D6D0 SetSoftShadows(enable): +28 = enable; 0044EE50 / 0044EE60.
void __fastcall kotor_m1_client_options_set_soft_shadows_0061d6d0(void* options, void*, std::uint32_t enable) {
    put_word(options, 0x28, enable);                                                      // 0061D6D6
    if (enable) lane_config_required::EnableSoftShadows_0044ee50();                       // 0061D6DB
    else lane_config_required::DisableSoftShadows_0044ee60();                             // 0061D6E3
}
// 0061D6F0 SetGrass(enable): +2C = enable; 0044EE30 / 0044EE40.
void __fastcall kotor_m1_client_options_set_grass_0061d6f0(void* options, void*, std::uint32_t enable) {
    put_word(options, 0x2C, enable);                                                      // 0061D6F6
    if (enable) lane_config_required::EnableGrass_0044ee30();                             // 0061D6FB
    else lane_config_required::DisableGrass_0044ee40();                                   // 0061D703
}
// 0061D730 SetTexQual(quality) (RET 4): byte +3C = AL; JMP CClientExoApp::SetTexturePack(quality) 005ED8F0 with
// ECX = [[007A39FC]+4] and the same stack word.
void __fastcall kotor_m1_client_options_set_tex_qual_0061d730(void* options, void*, std::uint32_t quality) {
    put_byte(options, 0x3C, static_cast<unsigned char>(quality));                         // 0061D730..34
    client_options::ClientExoAppSetTexturePack_005ed8f0(client_app(), nullptr,
                                                         static_cast<unsigned char>(quality));   // 0061D737..43
}
// 0061D9A0 SetDefaultGraphicsOptions (RET).
void __fastcall kotor_m1_client_options_default_graphics_0061d9a0(void* options, void*) {
    put_byte(options, 0x18, 0x39);                                                        // 0061D9A5
    put_word(options, 0x1C, 0x3E6B851Fu);                                                 // 0061D9A9 (0.23f)
    put_word(options, 0x20, 0x40000000u);                                                 // 0061D9B0 (2.0f)
    put_word(options, 0x24, 1);                                                           // 0061D9B7
    put_word(options, 0x2C, 1);                                                           // 0061D9BA
    put_byte(options, 0x3D, 1);                                                           // 0061D9BD
}
// 0061D520 SetDefaultAdvancedGraphicsOptions (RET).
void __fastcall kotor_m1_client_options_default_advanced_graphics_0061d520(void* options, void*) {
    put_word(options, 0x30, 1);                                                           // 0061D527
    put_word(options, 0x34, 1);                                                           // 0061D52A
    put_word(options, 0x38, 1);                                                           // 0061D52D
    put_byte(options, 0x3C, 0);                                                           // 0061D530
    put_word(options, 0x40, 0);                                                           // 0061D533
    put_byte(options, 0x44, 0);                                                           // 0061D536
    put_word(options, 0x28, 1);                                                           // 0061D539
}
// 005EE2B0 CClientExoApp::IsValidResolution(width, height): MOV ECX,[ECX+4]; JMP 005F0C60 (RET 8 there).
std::int32_t __fastcall kotor_m1_client_is_valid_resolution_005ee2b0(void* client_app_object, void*, std::int32_t width, std::int32_t height) {
    return options_video::IsValidResolution_005f0c60(static_cast<options_video::Client*>(ptr_at(client_app_object, 4)), nullptr,
                                                     width, height);                       // 005EE2B0..B3
}
// 005EE2C0 CClientExoApp::GetAllowHighMonitorFrequency: [[this+4]+4C0h].
std::uint32_t __fastcall kotor_m1_client_allow_high_frequency_005ee2c0(void* client_app_object, void*) {
    return word_at(ptr_at(client_app_object, 4), 0x4C0);                                  // 005EE2C0..C9
}
// 0045FA20 AurCheckSoftShadowsAvailable (AL result): cached tri-state [0078E520] over masks 0078E4E4|0078E4DC, else
// the ATI path gated by [007BB860] with cache [0078E524] over masks 0078E4C8|0078E4D0.
unsigned char __cdecl kotor_cdecl_0045fa20() {
    using namespace kotor_renderer_gl_cells;
    using namespace kotor_renderer_gl_procs;
    const std::uint32_t cached = cache_soft_0078e520;                                     // 0045FA20
    const std::uint32_t caps = capabilities_007bb85c;                                     // 0045FA29
    if (cached == 0xFFFFFFFFu) {                                                          // 0045FA26..35
        const std::uint32_t need = mask_0078e4e4 | mask_0078e4dc;                         // 0045FA37..43
        if ((caps & need) == need) { cache_soft_0078e520 = 1; return 1; }                 // 0045FA45..53
        cache_soft_0078e520 = 0;                                                          // 0045FA54
    } else if (cached != 0) {                                                             // 0045FA60..62
        return 1;                                                                         // 0045FAA6 (EAX = 1)
    }
    if (ati_soft_gate_007bb860 == 0) return 0;                                            // 0045FA64..6C -> 0045FAA4
    const std::uint32_t ati = cache_ati_soft_0078e524;                                    // 0045FA6E
    if (ati == 0xFFFFFFFFu) {                                                             // 0045FA74..77
        const std::uint32_t need = mask_0078e4c8 | mask_0078e4d0;                         // 0045FA79..7F
        if ((caps & need) == need) { cache_ati_soft_0078e524 = 1; return 1; }             // 0045FA85..91
        cache_ati_soft_0078e524 = 0;                                                      // 0045FA92
        return 0;                                                                         // 0045FA9C
    }
    return ati != 0 ? 1 : 0;                                                              // 0045FAA0..A7
}
// 0044F090 AurQueryMaximumAnisotropy: FLD dword [007A6858]; JMP _ftol 006FAE8C.
std::int32_t __cdecl kotor_cdecl_0044f090() {
    return ftol_float(&kotor_renderer_gl_cells::max_anisotropy_007a6858);                  // 0044F090..96
}
}

// ======================================================================== CSWGuiOptionsGraphics
namespace {
OptionsGraphics* gfx_of(OptionsGraphicsHandlers* h) { return reinterpret_cast<OptionsGraphics*>(h); }
OptionsGraphicsHandlers* gfx_handlers(OptionsGraphics* self) { return reinterpret_cast<OptionsGraphicsHandlers*>(self); }
// Constructed-member stages of 006E2E70 (EH states 0..0Ch of FuncInfo 0073716B).
void unwind_graphics(OptionsGraphics* self, int built) {
    if (built >= 13) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= 12) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 11) gui_load_button_destruct_00419da0(&self->btn_advanced, nullptr);
    if (built >= 10) gui_m48_button_toggle_destruct_0041cad0(&self->cb_grass, nullptr);
    if (built >= 9) gui_m48_button_toggle_destruct_0041cad0(&self->cb_shadows, nullptr);
    if (built >= 8) gui_load_button_destruct_00419da0(&self->btn_resolution_right, nullptr);
    if (built >= 7) gui_load_button_destruct_00419da0(&self->btn_resolution_left, nullptr);
    if (built >= 6) gui_load_button_destruct_00419da0(&self->btn_resolution, nullptr);
    if (built >= 5) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= 4) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= 3) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    if (built >= 2) gui_load_label_destruct_00419d10(&self->lbl_gamma, nullptr);
    if (built >= 1) gui_m48_slider_destruct_0041bb40(&self->sli_gamma, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void construct_checkbox(OptionsCheckbox* box) {                                          // ButtonToggle ctor + vptr 00758258
    gui_load_button_toggle_construct_0041ca30(box, nullptr);
    box->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_m1_opt_checkbox_methods_00758258);
}
}

extern "C" const OptionsPanelMethods<OptionsGraphics> kotor_m1_options_graphics_methods_007586f8 = {
    &kotor_m1_options_graphics_deleting_destruct_006e3420,       // +00 006E3420
    KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38,                          // +04..+38 (as 0073E010)
    &kotor_m1_options_graphics_handle_input_006deae0,            // +3C 006DEAE0
    KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68,                          // +40..+68 (as 0073E010)
};

extern "C" {
// ---- 006E2E70 ctor (thiscall, RET 4)
OptionsGraphics* __fastcall kotor_m1_options_graphics_construct_006e2e70(OptionsGraphics* self, void*, Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006E2E94..9B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_options_graphics_methods_007586f8);  // 006E2EAD
        gui_load_slider_construct_0041baa0(&self->sli_gamma, nullptr);              built = 1;   // 006E2EA0..B3
        gui_load_label_construct_0041acd0(&self->lbl_gamma, nullptr);               built = 2;   // 006E2EB8..C3
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);               built = 3;   // 006E2EC8..D3
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);               built = 4;   // 006E2ED8..E3
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);               built = 5;   // 006E2EE8..F3
        gui_load_button_construct_0041c0f0(&self->btn_resolution, nullptr);         built = 6;   // 006E2EF8..03
        gui_load_button_construct_0041c0f0(&self->btn_resolution_left, nullptr);    built = 7;   // 006E2F08..13
        gui_load_button_construct_0041c0f0(&self->btn_resolution_right, nullptr);   built = 8;   // 006E2F18..23
        construct_checkbox(&self->cb_shadows);                                      built = 9;   // 006E2F28..3A
        construct_checkbox(&self->cb_grass);                                        built = 10;  // 006E2F40..52
        gui_load_button_construct_0041c0f0(&self->btn_advanced, nullptr);           built = 11;  // 006E2F58..63
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);               built = 12;  // 006E2F68..73
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);            built = 13;  // 006E2F78..83
        {
            Ref layout;
            layout.construct_00406d80("optgraphics");                                      // 006E2F8A..97 (007586E8)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                  // 006E2F9C..A3
        }
        init_control(self, &self->sli_gamma, "SLI_GAMMA");                                 // 006E2FA8..D2 (007586DC)
        init_control(self, &self->lbl_title, "LBL_TITLE");                                 // 006E2FD7..07 (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                                     // 006E300C..3C (00755D6C)
        init_control(self, &self->lbl_gamma, "LBL_GAMMA");                                 // 006E3041..71 (007586D0)
        init_control(self, &self->btn_resolution, "BTN_RESOLUTION");                       // 006E3076..A6 (007586C0)
        init_control(self, &self->btn_resolution_left, "BTN_RESOLUTIONLEFT");              // 006E30AB..DB (007586AC)
        init_control(self, &self->btn_resolution_right, "BTN_RESOLUTIONRIGHT");            // 006E30E0..10 (00758698)
        init_control(self, &self->cb_shadows, "CB_SHADOWS");                               // 006E3115..45 (0075868C)
        init_control(self, &self->cb_grass, "CB_GRASS");                                   // 006E314A..7A (00758680)
        init_control(self, &self->btn_advanced, "BTN_ADVANCED");                           // 006E317F..AF (00758670)
        self->sli_gamma.u58 = 0xA546u;                                                     // 006E31B4..B9 (+0BC)
        self->lbl_gamma.u58 = 0xA546u;                                                     // 006E31BF (+274)
        {
            String name("BTN_BACK");                                                       // 006E31CA..0A (007548AC)
            self->btn_resolution.u58 = 0xBB5Fu;                                            // 006E31D3 (+914)
            self->btn_resolution_left.u58 = 0xBB5Fu;                                       // 006E31D9 (+AD8)
            self->btn_resolution_right.u58 = 0xBB5Fu;                                      // 006E31DF (+C9C)
            self->cb_shadows.u58 = 0xBB4Fu;                                                // 006E31E5 (+E60)
            self->cb_grass.u58 = 0xBB53u;                                                  // 006E31EF (+1114)
            self->btn_advanced.u58 = 0xBFAEu;                                              // 006E31F9 (+13C8)
            reinterpret_cast<unsigned char*>(self)[0x89D] = 1;                             // 006E3203 (lb_desc+2C1)
            gui_load_panel_init_control_0040b930(self, nullptr, &self->btn_back, &name, 1); // 006E3214..24
        }                                                                                  // 006E3229..31
        init_control(self, &self->btn_default, "BTN_DEFAULT");                             // 006E3236..66 (007583B4)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                // 006E326B..6D
        init_desc_item(&self->desc_item, &self->lb_desc);                                  // 006E3272..8E (+8A8 proto, vslot +98)
        gui_m48_slider_set_cur_00417ee0(&self->sli_gamma, nullptr, 0x39);                  // 006E3294..98 SetCurValue(57)
        kih::GuiObject* const target = event_target(self);
        const gui_load::EventFunc move = handler(&OptionsGraphicsHandlers::OnMoveGammaSlider_006ded30);
        const gui_load::EventFunc defaults = handler(&OptionsGraphicsHandlers::OnDefault_006e0190);
        const gui_load::EventFunc slot = handler(&OptionsGraphicsHandlers::OnEnterSlot_006e0140);
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0, target,
                                            handler(&OptionsGraphicsHandlers::OnEnterSlider_006e0150));  // 006E329D..A7
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 1, target,
                                            handler(&OptionsSliderHandlers::OnExitSlider_006ded90));     // 006E32AC..B6
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x2F, target, move);               // 006E32BB..C5
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x30, target, move);               // 006E32CA..D4
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x3F, target, move);               // 006E32D9..E3
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x40, target, move);               // 006E32E8..F2
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x2A, target, defaults);           // 006E32F7..01
        gui_load_control_add_event_0041ab20(&self->sli_gamma, nullptr, 0x27, target, move);               // 006E3306..10
        gui_load_control_add_event_0041ab20(&self->btn_resolution, nullptr, 0, target, slot);             // 006E3315..25
        gui_load_control_add_event_0041ab20(&self->cb_shadows, nullptr, 0, target, slot);                 // 006E332A..38
        gui_load_control_add_event_0041ab20(&self->cb_grass, nullptr, 0, target, slot);                   // 006E333D..4B
        gui_load_control_add_event_0041ab20(&self->btn_resolution, nullptr, 0x27, target,
                                            handler(&OptionsGraphicsHandlers::OnResolution_006e1660));   // 006E3350..5A
        gui_load_control_add_event_0041ab20(&self->cb_shadows, nullptr, 0x27, target,
                                            handler(&OptionsGraphicsHandlers::OnShadows_006dedb0));      // 006E335F..6D
        gui_load_control_add_event_0041ab20(&self->cb_grass, nullptr, 0x27, target,
                                            handler(&OptionsGraphicsHandlers::OnGrass_006dee00));        // 006E3372..80
        kotor_m1_options_graphics_set_up_options_006def30(self, nullptr);                  // 006E3385..87
        self->btn_back.flags &= 0xFFFFFFFBu;                                               // 006E338C..A7
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target, back_thunk());        // 006E3398..AA (00624BB0)
        self->btn_default.flags &= 0xFFFFFFFBu;                                            // 006E33AF..C5
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, target, defaults);         // 006E33BB..C8
        self->btn_advanced.flags &= 0xFFFFFFFBu;                                           // 006E33CD..E3
        gui_load_control_add_event_0041ab20(&self->btn_advanced, nullptr, 0x27, target,
                                            handler(&OptionsGraphicsHandlers::OnAdvanced_006e2cf0));     // 006E33D9..E6
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->sli_gamma, 0);    // 006E33EB..F0
        self->reinit_pending = 0;                                                          // 006E33FA
    } catch (...) {
        unwind_graphics(self, built);
        throw;
    }
    return self;                                                                           // 006E3404
}

// ---- 006E3440 dtor (thiscall, RET); checkboxes destroyed with ~CSWGuiButtonToggle 0041CAD0.
void __fastcall kotor_m1_options_graphics_destruct_006e3440(OptionsGraphics* self, void*) {
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                       // 006E345D..6B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                          // 006E3470..7B
    gui_load_button_destruct_00419da0(&self->btn_advanced, nullptr);                      // 006E3480..8B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_grass, nullptr);                    // 006E3490..9B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_shadows, nullptr);                  // 006E34A0..AB
    gui_load_button_destruct_00419da0(&self->btn_resolution_right, nullptr);              // 006E34B0..BB
    gui_load_button_destruct_00419da0(&self->btn_resolution_left, nullptr);               // 006E34C0..CB
    gui_load_button_destruct_00419da0(&self->btn_resolution, nullptr);                    // 006E34D0..DB
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                          // 006E34E0..EB
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                          // 006E34F0..FB
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                          // 006E3500..0B
    gui_load_label_destruct_00419d10(&self->lbl_gamma, nullptr);                          // 006E3510..1B
    gui_m48_slider_destruct_0041bb40(&self->sli_gamma, nullptr);                          // 006E3520..28
    gui_load_panel_destruct_0040cf70(self, nullptr);                                      // 006E352D..37
}
// ---- 006E3420 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_graphics_deleting_destruct_006e3420(OptionsGraphics* self, void*, Word flags) {
    kotor_m1_options_graphics_destruct_006e3440(self, nullptr);                           // 006E3423
    if (flags & 1u) live_free(self);                                                      // 006E3428..35
    return self;                                                                          // 006E3438
}

}   // extern "C" (fullscreen lane: C++-linkage helper below)
extern "C" void* __fastcall gui_tooltip_panel_construct_006277c0(void* self, void*, void* manager);   // 006277C0 (0602 tooltip panel)
namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }                       // [007A39F4] (0392-input-cells)
namespace {
std::int32_t read_graphics_option(base_ini_reconstruction::PublicIni& ini, String* value, const char* key_text);   // defined below (gfxapply)
std::int32_t as_int(String* value);                                                                                 // defined below (gfxapply)
// Shared ReInitAurora branch of CSWGuiOptionsGraphics::HandleInputEvent 006DEB55..006DED09 (gate +18BC) and
// CSWGuiOptionsResolution::HandleInputEvent 006E0D65..006E0F12 (gate +80C); the two are instruction-for-instruction the
// same apart from the gate offset. CExoIni [esp+0Ch] (EH state 0), value CExoString [esp+10h] (EH state 1):
//   ReadIniEntry "Disable Vertex Buffer Objects" (0073D6C0) -> AsINT == 1 -> AurDisableVertexBufferObjects 0044EF60;
//   push 1 (5th word, pushed before the vslot call); [007A39F8] vslot +04 FullScreen; ReInitAurora 00403800(width
//   [0078D1D4], height [0078D1D8], depth byte [0078D1E0], fullscreen, 1); ReadIniEntry "Disable Write-Only VBO"
//   (0073D694) -> AsINT == 1 ? 0044EF70 : 0044EF80; gate = 0 (006DECE9 / 006E0EED); ~value; ~CExoIni.
void reinit_display_and_vbo(Word& gate) {
    base_ini_reconstruction::PublicIni ini;
    ini.construct_005e6750();                                                             // 006DEB55..59 / 006E0D65..69
    try {
        String value;                                                                     // 006DEB5E..6A / 006E0D6E..7A (005B3190)
        if (read_graphics_option(ini, &value, "Disable Vertex Buffer Objects") != 0 &&    // 006DEB6F..F8 / 006E0D7F..E08
            as_int(&value) == 1)                                                          // 006DEBFA..C06 / 006E0E0A..16
            kotor_cdecl_0044ef60();                                                       // 006DEC08 / 006E0E18
        kotor_renderer_init::Renderer* const renderer =
            static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8);   // 006DEC0D / 006E0E1D
        const std::uint32_t fullscreen = kotor_aurora_window::table_of(renderer)->fullscreen(renderer, nullptr);  // 006DEC17 / 006E0E27
        const auto& v = options_video::video_mode_bindings;
        v.reinit_aurora_00403800(*v.width_0078d1d4, *v.height_0078d1d8, *v.depth_0078d1e0, fullscreen, 1);  // 006DEC1A..31 / 006E0E2A..41
        if (read_graphics_option(ini, &value, "Disable Write-Only VBO") != 0) {           // 006DEC39..BD / 006E0E49..CD
            if (as_int(&value) == 1) kotor_cdecl_0044ef70();                              // 006DECBF..CD / 006E0ECF..DD
            else kotor_cdecl_0044ef80();                                                  // 006DECE0 / 006E0EE4
        }
        gate = 0;                                                                         // 006DECE9 / 006E0EED
    }                                                                                     // 006DECF8 / 006E0EFC ~value
    catch (...) {
        ini.destroy_005e67e0();                                                           // EH state 0
        throw;
    }
    ini.destroy_005e67e0();                                                               // 006DED09 / 006E0F0D
}
}
extern "C" {
// ---- 005ED8D0 CClientExoApp::SetVideoMode(mode) (thiscall, RET 4): ECX = [this+4]; push 0 (style); push mode;
// call CClientExoAppInternal::SetVideoMode 005F1830; EAX = its result.
std::int32_t __fastcall kotor_fastcall_005ed8d0(void* client_app_object, void*, std::uint32_t mode) {
    return options_video::SetVideoMode_005f1830(static_cast<options_video::Client*>(ptr_at(client_app_object, 4)), nullptr,
                                                mode, 0);                                // 005ED8D0..DF
}
// ---- 0040A350 CSWGuiManager::ReloadToolTipPanel (thiscall, RET): old panel [this+3Ch] -> deleting dtor (vslot +00, 1);
// operator new(1A4h); CSWGuiToolTipPanel::CSWGuiToolTipPanel 006277C0(this) (freed if it throws, EH state 0);
// [this+3Ch] = the new panel, or 0 when the allocation returned null.
void __fastcall kotor_fastcall_0040a350(void* manager, void*) noexcept(false) {
    if (void* const old = ptr_at(manager, 0x3C)) {                                        // 0040A369..6E
        void* (__fastcall* destroy)(void*, void*, Word);
        std::memcpy(&destroy, ptr_at(old, 0), sizeof destroy);                            // 0040A370
        (void)destroy(old, nullptr, 1);                                                   // 0040A372..74
    }
    void* const memory = c2da_directory_new_006fa7e6(0x1A4);                             // 0040A376..80
    void* panel = nullptr;
    if (memory) {                                                                         // 0040A387..91
        try {
            panel = gui_tooltip_panel_construct_006277c0(memory, nullptr, manager);       // 0040A393..96
        } catch (...) {
            live_free(memory);
            throw;
        }
    }
    put_word(manager, 0x3C, word_of(panel));                                              // 0040A39B / 0040A3B4
}
// ---- 006DEAE0 HandleInputEvent(event, pressed) (thiscall, RET 8): 28h/2Eh Back, 2Dh click sound.
void __fastcall kotor_m1_options_graphics_handle_input_006deae0(Object* object, void*, I32 event, I32 pressed) {
    OptionsGraphics* const self = static_cast<OptionsGraphics*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                                   // 006DEAFE..07
        if (event == 0x28 || event == 0x2E) {                                             // 006DEB0D..1F
            manager_of(self)->PlayGuiSound(0);                                            // 006DEB25..2A
            kotor_fastcall_0040be00(self->manager, nullptr);                              // 006DEB2F..32 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                           // 006DEB37..44
            if (self->reinit_pending != 0)                                                // 006DEB47..4F
                reinit_display_and_vbo(self->reinit_pending);                             // 006DEB55..006DED09
        } else if (event == 0x2D) {                                                       // 006DEB15..18 -> 006DECD4
            manager_of(self)->PlayGuiSound(0);                                            // 006DECD4..D9
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);                 // 006DED0E..18 (00409E60)
}

// ---- 006DEF30 SetUpOptions (thiscall, RET)
void __fastcall kotor_m1_options_graphics_set_up_options_006def30(OptionsGraphics* self, void*) {
    void* const options = options_object();                                              // 006DEF30..3C
    if (!options) return;                                                                 // 006DEF43..45
    {
        Object* const s = gui_lb::vt<ControlSelf64View>(static_cast<Object*>(&self->sli_gamma))->slot64(&self->sli_gamma, nullptr);  // 006DEF4E..5B
        gui_m48_slider_set_cur_00417ee0(static_cast<gui_lb::Slider*>(static_cast<gui_load::Navigable*>(static_cast<Control*>(s))),
                                        nullptr, byte_at(options, 0x18));                 // 006DEF4B..60
    }
    gfx_handlers(self)->OnMoveGammaSlider_006ded30(kc(&self->sli_gamma));                 // 006DEF65..68
    if (word_at(options, 0x28) != 0)                                                      // 006DEF6D..72 (soft shadows)
        kotor_m1_client_options_set_shadows_0061d6b0(options, nullptr, 1);                // 006DEF74..78
    {
        const Word shadows = word_at(options, 0x24);                                      // 006DEF83
        gui_lb::ButtonToggle* const t = toggle_self(&self->cb_shadows);                   // 006DEF7D..8C vslot +5C
        t->toggle_flags ^= (t->toggle_flags ^ shadows) & 1u;                              // 006DEF8F..A2
    }
    {
        const Word grass = word_at(options, 0x2C);                                        // 006DEFAE
        gui_lb::ButtonToggle* const t = toggle_self(&self->cb_grass);                     // 006DEFA8..B7
        t->toggle_flags ^= (t->toggle_flags ^ grass) & 1u;                                // 006DEFBA..CD
    }
}
// ---- 006DEE40 SetDescription(strref): params +444, list +5DC, item +35C (extent +360), text +42C.
void __fastcall kotor_m1_options_graphics_set_description_006dee40(OptionsGraphics* self, void*, Word strref) noexcept(false) {
    describe(&self->desc_item, &self->lb_desc, strref);                                   // 006DEE60..EF11
}
}

// ---- 006DED30 OnMoveGammaSlider(control) (thiscall, RET 4): gamma = 2.0f - (cur/max) * 1.77f (fild, fidiv, fmul dword
// 007581E4, fsubr dword 00741BE0, fstp dword), CAurInternalGL vslot +24 ResetGammaRamp(gamma, 0) on [007A39F8];
// CClientOptions byte +18 = cur.
void __thiscall OptionsGraphicsHandlers::OnMoveGammaSlider_006ded30(kih::Control* control) {
    const I32 max_value = slider_of(control)->max_value;                                  // 006DED33..3E vslot +64, [eax+70h]
    const I32 cur = slider_of(control)->cur_value;                                        // 006DED41..4C vslot +64, [eax+74h]
    const float scale_007581e4 = 1.77f;                                                   // .rdata 007581E4 = 3FE28F5Ch
    const float base_00741be0 = 2.0f;                                                     // .rdata 00741BE0 = 40000000h
    const double ratio = static_cast<double>(cur) / static_cast<double>(max_value);       // 006DED53..61 fild / fidiv
    const float gamma = static_cast<float>(static_cast<double>(base_00741be0) -
                                           ratio * static_cast<double>(scale_007581e4));   // 006DED66..72 fmul / fsubr / fstp
    std::uint32_t gamma_bits;
    std::memcpy(&gamma_bits, &gamma, 4);
    kotor_renderer_init::Renderer* const renderer =
        static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8);   // 006DED57 [007A39F8]
    (void)kotor_aurora_window::table_of(renderer)->gamma(renderer, nullptr, gamma_bits, 0);   // 006DED5D..75 vslot +24
    put_byte(options_object(), 0x18, static_cast<unsigned char>(cur));                    // 006DED78..87 (bl)
}
// ---- 006E0140 OnEnterSlot(control): MOV EDX,[control+58h]; replace the stack word; JMP 006DEE40.
void __thiscall OptionsGraphicsHandlers::OnEnterSlot_006e0140(kih::Control* control) {
    kotor_m1_options_graphics_set_description_006dee40(gfx_of(this), nullptr, word_at(control, 0x58));  // 006E0140..4B
}
// ---- 006E0150 OnEnterSlider(control): SetDescription(A546h) for SLI_GAMMA; thumb flags |= 1.
void __thiscall OptionsGraphicsHandlers::OnEnterSlider_006e0150(kih::Control* control) {
    OptionsGraphics* const self = gfx_of(this);
    if (object_of(control) == &self->sli_gamma)                                           // 006E0155..5A
        kotor_m1_options_graphics_set_description_006dee40(self, nullptr, 0xA546u);       // 006E015C..61
    slider_of(control)->thumb.params.flags |= 1u;                                         // 006E0166..7B [eax+1B0h]
}
// ---- 006E0190 OnDefault(control)
void __thiscall OptionsGraphicsHandlers::OnDefault_006e0190(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                              // 006E0197..9C
    kotor_m1_client_options_default_graphics_0061d9a0(options_object(), nullptr);         // 006E019E..AE
    kotor_m1_options_graphics_set_up_options_006def30(gfx_of(this), nullptr);             // 006E01B3..B5
}
// ---- 006DEDB0 OnShadows(control)
void __thiscall OptionsGraphicsHandlers::OnShadows_006dedb0(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                              // 006DEDB4..B9
    void* const options = options_object();                                              // 006DEDBB..C5
    if (!options) return;                                                                 // 006DEDCC..CE
    kotor_m1_client_options_set_shadows_0061d6b0(options, nullptr, word_at(options, 0x24) == 0 ? 1u : 0u);   // 006DEDD0..DD
    if (word_at(options, 0x24) == 0)                                                      // 006DEDE2..E7
        kotor_m1_client_options_set_soft_shadows_0061d6d0(options, nullptr, 0);           // 006DEDE9..ED
}
// ---- 006DEE00 OnGrass(control)
void __thiscall OptionsGraphicsHandlers::OnGrass_006dee00(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                              // 006DEE04..09
    const Word grass = word_at(options_object(), 0x2C);                                   // 006DEE0B..22
    kotor_m1_client_options_set_grass_0061d6f0(options_object(), nullptr, grass == 0 ? 1u : 0u);   // 006DEE19..34
}
// ---- 006E1660 OnResolution(control): new(810h) CSWGuiOptionsResolution(manager); CenterPanel; AddPanel(p, 1, 1).
void __thiscall OptionsGraphicsHandlers::OnResolution_006e1660(kih::Control* control) {
    OptionsGraphics* const self = gfx_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E1679..84
    void* const memory = c2da_directory_new_006fa7e6(0x810);                             // 006E1686..90
    OptionsResolution* panel = nullptr;                                                   // 006E167D (esi = 0)
    if (memory) {                                                                         // 006E1697..9D
        try {
            panel = kotor_m1_options_resolution_construct_006e0710(static_cast<OptionsResolution*>(memory), nullptr,
                                                                   self->manager);        // 006E169F..AA
        } catch (...) {
            live_free(memory);                                                            // EH state 0
            throw;
        }
    }
    gui_m48_panel_center_0040a600(panel, nullptr);                                        // 006E16AC..B6 (null when new failed, as original)
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 1, 1);                         // 006E16BB..C3 AddPanel(p, 1, 1)
}
// ---- 006E2CF0 OnAdvanced(control): new(2338h) CSWGuiOptionsGraphicsAdvanced(manager, this); AddPanel(p, 3, 1).
void __thiscall OptionsGraphicsHandlers::OnAdvanced_006e2cf0(kih::Control* control) {
    OptionsGraphics* const self = gfx_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E2D09..11
    void* const memory = c2da_directory_new_006fa7e6(0x2338);                            // 006E2D13..1D
    OptionsGraphicsAdvanced* panel = nullptr;                                             // 006E2D3E
    if (memory) {
        try {
            panel = kotor_m1_options_graphics_adv_construct_006e16e0(static_cast<OptionsGraphicsAdvanced*>(memory), nullptr,
                                                                     self->manager, self);   // 006E2D30..37 (push esi; push [esi+18h])
        } catch (...) {
            live_free(memory);
            throw;
        }
    }
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                         // 006E2D40..50 AddPanel(p, 3, 1)
}

// ======================================================================== CSWGuiOptionsResolution
namespace {
OptionsResolution* res_of(OptionsResolutionHandlers* h) { return reinterpret_cast<OptionsResolution*>(h); }
OptionsResolutionHandlers* res_handlers(OptionsResolution* self) { return reinterpret_cast<OptionsResolutionHandlers*>(self); }
void unwind_resolution(OptionsResolution* self, int built) {                              // EH states 0..3 (FuncInfo 0073699A)
    if (built >= 4) gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);
    if (built >= 3) gui_load_button_destruct_00419da0(&self->btn_ok, nullptr);
    if (built >= 2) gui_load_listbox_destruct_0041b060(&self->lb_resolutions, nullptr);
    if (built >= 1) gui_load_label_destruct_00419d10(&self->lbl_resolution, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
// Local CExoArrayList<CSWGuiControl*> [esp+14h] of 006E0710; only its data block is released (006E0C14..21 _free).
struct ButtonList {
    kotor_reconstruction::ArrayMutations list{};                                          // 006E08F4..00 zeroed
    ~ButtonList() { live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list.data))); }
};
// operator new(1C4h) + CSWGuiButton ctor (EH: freed if the ctor throws); a null allocation yields a null button that
// the original dereferences next (006E0A1A / 006E0B01) - same here (only on allocation failure).
Button* new_button() {
    void* const memory = c2da_directory_new_006fa7e6(0x1C4);                             // 006E09F0..FA / 006E0AD7..E1
    if (!memory) return nullptr;                                                          // 006E0A18 / 006E0AFF
    try {
        return gui_load_button_construct_0041c0f0(static_cast<Button*>(memory), nullptr); // 006E0A0D..0F / 006E0AF4..F6
    } catch (...) {
        live_free(memory);
        throw;
    }
}
}

extern "C" const OptionsPanelMethods<OptionsResolution> kotor_m1_options_resolution_methods_00758348 = {
    &kotor_m1_options_resolution_deleting_destruct_006e0c50,     // +00 006E0C50
    KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38,                          // +04..+38 (as 0073E010)
    &kotor_m1_options_resolution_handle_input_006e0cf0,          // +3C 006E0CF0
    KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68,                          // +40..+68 (as 0073E010)
};

extern "C" {
// ---- 006E0710 ctor (thiscall, RET 4): layout "optresolution", then one list button per 32-bit display mode
// accepted by IsValidResolution (and <= 85 Hz unless AllowHighMonitorFrequency), button+58h = mode index.
OptionsResolution* __fastcall kotor_m1_options_resolution_construct_006e0710(OptionsResolution* self, void*, Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006E0737..3E
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_options_resolution_methods_00758348);  // 006E0751
        gui_load_label_construct_0041acd0(&self->lbl_resolution, nullptr);   built = 1;  // 006E0743..57
        gui_load_listbox_construct_0041be40(&self->lb_resolutions, nullptr); built = 2;  // 006E075C..6A
        gui_load_button_construct_0041c0f0(&self->btn_ok, nullptr);          built = 3;  // 006E076F..7F
        gui_load_button_construct_0041c0f0(&self->btn_cancel, nullptr);      built = 4;  // 006E0784..94
        {
            Ref layout;
            layout.construct_00406d80("optresolution");                                    // 006E0799..AA (00758338)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                  // 006E07AF..B6
        }
        init_control(self, &self->lbl_resolution, "LBL_RESOLUTION");                       // 006E07BB..EF (00758328)
        init_control(self, &self->lb_resolutions, "LB_RESOLUTIONS");                       // 006E07F4..2B (00758318)
        init_control(self, &self->btn_ok, "BTN_OK");                                       // 006E0830..61 (0074FD9C)
        init_control(self, &self->btn_cancel, "BTN_CANCEL");                               // 006E0866..97 (0074FD90)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                // 006E089C..9E
        kih::GuiObject* const target = event_target(self);
        const gui_load::EventFunc chosen = handler(&OptionsResolutionHandlers::OnResolutionChosen_006df690);
        gui_load_control_add_event_0041ab20(&self->btn_ok, nullptr, 0x27, target, chosen);                // 006E08A3..AD
        self->btn_ok.flags &= 0xFFFFFFFBu;                                                 // 006E08B2..C2
        gui_load_control_add_event_0041ab20(&self->btn_cancel, nullptr, 0x27, target, back_thunk());      // 006E08B5..C5 (00624BB0)
        self->btn_cancel.flags &= 0xFFFFFFFBu;                                             // 006E08CA..D6
        gui_lb::ListBox* const lb = &self->lb_resolutions;
        gui_lb::vt<ListBoxSelectA4View>(static_cast<Object*>(lb))->select_left_a4(lb, nullptr, 1);         // 006E08D0..DF vslot +A4(1)
        gui_load_panel_set_active_control_0040a630(self, nullptr, lb, 0);                  // 006E08E5..EB
        Button* selected = nullptr;                                                        // [esp+30h] (006E08F0)
        I32 index = 0;                                                                     // [esp+10h] (006E08F4)
        ButtonList buttons;                                                                // [esp+14h..1Ch]
        String text;                                                                       // [esp+34h] (006E0904..10, 005B3190)
        DEVMODEA mode;                                                                     // [esp+40h] (9Ch bytes)
        std::memset(&mode, 0, sizeof mode);                                                // 006E0915..20 rep stos 27h dwords
        static_assert(sizeof(DEVMODEA) == 0x9C, "DEVMODEA 9Ch");
        mode.dmSize = 0x9C;                                                                // 006E0932
        Control* const proto_control = lb->proto;                                          // 006E0922 [esi+470h]
        if (proto_control) {                                                               // 006E0928..39
            Object* const proto = gui_lb::vt<ControlSelf58View>(static_cast<Object*>(proto_control))->slot58(proto_control, nullptr);  // 006E093F..44
            BOOL more = EnumDisplaySettingsA(nullptr, 0, &mode);                           // 006E0946..4F (IAT 0073D3E4)
            while (more) {                                                                 // 006E0955..57
                const I32 frequency = static_cast<I32>(mode.dmDisplayFrequency);           // [esp+B8h]
                const I32 width = static_cast<I32>(mode.dmPelsWidth);                      // [esp+ACh]
                const I32 height = static_cast<I32>(mode.dmPelsHeight);                    // [esp+B0h]
                bool compare_current = false;
                Button* button = nullptr;
                if (frequency == 0 || frequency == 1) {                                    // 006E0960..72 (default rate)
                    if (mode.dmBitsPerPel == 0x20 &&                                       // 006E0AA3..AB
                        kotor_m1_client_is_valid_resolution_005ee2b0(client_app(), nullptr, width, height) != 0) {  // 006E0AB1..D1
                        button = new_button();                                             // 006E0AD7..FF
                        gui_lb::vt<ButtonInit98View>(static_cast<Object*>(button))->initialize98(button, nullptr,
                            word_of(&proto->extent), word_of(proto));                      // 006E0B01..12 vslot +98(proto+4, proto)
                        kotor_reconstruction::StartupFormat005e5680(&text, "%d x %d", width, height);   // 006E0B18..37 (00758300)
                        gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(&button->text) + 0x18,
                                                               nullptr, &text);            // 006E0B3A..45 (+16C)
                        button->u58 = static_cast<Word>(index);                            // 006E0B4A / 006E0B58
                        gui_load_control_add_event_0041ab20(button, nullptr, 0x27, target, chosen);   // 006E0B4E..5B
                        kotor_reconstruction::ArrayMutations& l = buttons.list;            // inlined Add (006E0B60..8C)
                        if (l.size == l.capacity)
                            menu_list_growth::allocate_005e03c0(&l, nullptr,
                                l.capacity != 0 ? static_cast<Word>(l.capacity) * 2u : 0x10u);   // 006E0B6A..7C (005E03C0)
                        std::memcpy(reinterpret_cast<void*>(static_cast<std::uintptr_t>(l.data) + static_cast<std::uint32_t>(l.size) * 4u),
                                    &button, 4);                                           // 006E0B81..89
                        l.size = l.size + 1;                                               // 006E0B8C
                        compare_current = true;                                            // 006E0B90
                    }
                } else if (mode.dmBitsPerPel == 0x20 && static_cast<std::uint32_t>(frequency) >= 0x3Cu &&  // 006E0978..89
                           kotor_m1_client_is_valid_resolution_005ee2b0(client_app(), nullptr, width, height) != 0 &&  // 006E098F..AF
                           (kotor_m1_client_allow_high_frequency_005ee2c0(client_app(), nullptr) != 0 ||   // 006E09B5..C5
                            !(static_cast<double>(static_cast<std::uint32_t>(frequency)) > static_cast<double>(85.0f)))) {  // 006E09C7..EA (00752C90)
                    button = new_button();                                                 // 006E09F0..18
                    gui_lb::vt<ButtonInit98View>(static_cast<Object*>(button))->initialize98(button, nullptr,
                        word_of(&proto->extent), word_of(proto));                          // 006E0A1A..2B
                    kotor_reconstruction::StartupFormat005e5680(&text, "%d x %d @ %d Hz", width, height, frequency);   // 006E0A31..58 (00758308)
                    gui_load_text_params_set_text_00415e00(reinterpret_cast<unsigned char*>(&button->text) + 0x18,
                                                           nullptr, &text);                // 006E0A5B..66 (+16C)
                    button->u58 = static_cast<Word>(index);                                // 006E0A6B / 006E0A75
                    gui_load_control_add_event_0041ab20(button, nullptr, 0x27, target, chosen);   // 006E0A6F..7C
                    gui_load_array_add_00671c00(&buttons.list, nullptr, word_of(button));  // 006E0A81..86 CExoArrayList::Add
                    compare_current = static_cast<std::uint32_t>(frequency) ==
                                      *options_video::video_mode_bindings.refresh_0078d1dc;   // 006E0A8B..9E [0078D1DC]
                }
                if (compare_current &&
                    static_cast<std::uint32_t>(width) == kotor_renderer_gl_cells::width_0078d1d4 &&   // 006E0B90..9D [0078D1D4]
                    static_cast<std::uint32_t>(height) == kotor_menu_window_cells::height_0078d1d8)  // 006E0B9F..AC [0078D1D8]
                    selected = button;                                                     // 006E0BAE
                ++index;                                                                   // 006E0BB2..BF
                more = EnumDisplaySettingsA(nullptr, static_cast<DWORD>(index), &mode);    // 006E0BB6..C3
            }
            kotor_fastcall_0041c1d0(lb, nullptr, &buttons.list, 0, 1, 0);                 // 006E0BD1..E4 AddControls(list, 0, 1, 0)
            if (selected) gui_load_listbox_set_active_control_0041c160(lb, nullptr, selected, 0);  // 006E0BE9..F6
        }
        self->reinit_pending = 0;                                                          // 006E0BFD..C01
    } catch (...) {                                                                        // 006E0C0F ~text, 006E0C14..21 free(list)
        unwind_resolution(self, built);
        throw;
    }
    return self;                                                                           // 006E0C31
}
// ---- 006E0C70 dtor (thiscall, RET)
void __fastcall kotor_m1_options_resolution_destruct_006e0c70(OptionsResolution* self, void*) {
    gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);                        // 006E0C8D..9B
    gui_load_button_destruct_00419da0(&self->btn_ok, nullptr);                            // 006E0CA0..AB
    gui_load_listbox_destruct_0041b060(&self->lb_resolutions, nullptr);                   // 006E0CB0..BB
    gui_load_label_destruct_00419d10(&self->lbl_resolution, nullptr);                     // 006E0CC0..C8
    gui_load_panel_destruct_0040cf70(self, nullptr);                                      // 006E0CCD..D7
}
// ---- 006E0C50 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_resolution_deleting_destruct_006e0c50(OptionsResolution* self, void*, Word flags) {
    kotor_m1_options_resolution_destruct_006e0c70(self, nullptr);                         // 006E0C53
    if (flags & 1u) live_free(self);                                                      // 006E0C58..65
    return self;                                                                          // 006E0C68
}
// ---- 006E0CF0 HandleInputEvent(event, pressed) (thiscall, RET 8): 27h -> click + OnResolutionChosen(0);
// 28h/2Eh Back (+80C set by OnResolutionChosen 006DF981 -> ReInitAurora branch 006E0D65..0F12).
void __fastcall kotor_m1_options_resolution_handle_input_006e0cf0(Object* object, void*, I32 event, I32 pressed) {
    OptionsResolution* const self = static_cast<OptionsResolution*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                                   // 006E0D0E..17
        if (event == 0x27) {                                                              // 006E0D1D..23 -> 006E0F14
            manager_of(self)->PlayGuiSound(0);                                            // 006E0F14..19
            res_handlers(self)->OnResolutionChosen_006df690(nullptr);                     // 006E0F1E..22
        } else if (event == 0x28 || event == 0x2E) {                                      // 006E0D29..2F
            manager_of(self)->PlayGuiSound(0);                                            // 006E0D35..3A
            kotor_fastcall_0040be00(self->manager, nullptr);                              // 006E0D3F..42 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                           // 006E0D47..54
            if (self->reinit_pending != 0)                                                // 006E0D57..5F
                reinit_display_and_vbo(self->reinit_pending);                             // 006E0D65..006E0F12
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);                 // 006E0F27..31 (00409E60)
}
}
// ---- 006DF690 OnResolutionChosen(control) (thiscall, RET 4). Guard 006DF6B4..C2: a non-null control with
// [control+4Ch] == 0 returns. Apply path 006DF6C8..006DF98D:
//   mode index = [LB_RESOLUTIONS.GetSelectedControl() + 58h] (0041A5D0; no null test, as original);
//   DEVMODEA (9Ch, dmSize 9Ch) EnumDisplaySettingsA(0, index) -> success: [0078D1DC] = dmDisplayFrequency;
//   CClientExoApp::SetVideoMode(index) 005ED8D0 -> non-zero: CExoString value + CExoIni; for Width [0078D1D4],
//   Height [0078D1D8], RefreshRate [0078D1DC]: Format(value, "%d", v); key/section/file strings built in that order;
//   WriteIniEntry 005E67D0(value, file, section, key); strings destroyed file/section/key; then ~CExoIni, ~value.
//   Always (also when SetVideoMode returned 0): PopModalPanel [this+18h]; state 400h; [this+80Ch] = 1 (ebx, 006DF981);
//   CSWGuiManager::ReloadToolTipPanel 0040A350 on [007A39F4].
namespace {
void write_graphics_option(base_ini_reconstruction::PublicIni& ini, String* value, const char* key_text, std::uint32_t number) {
    kotor_reconstruction::StartupFormat005e5680(value, "%d", number);                     // 006DF742..5E / 006DF7F2..807 / 006DF89B..8B0 (0073D720)
    String key(key_text);                                                                 // "Width" 00749FF8 / "Height" 00747580 / "RefreshRate" 0074E80C
    String section("Graphics Options");                                                   // 0073D6AC
    String file(".\\swkotor.ini");                                                       // 0073D648
    (void)ini.write_005e67d0(value, &file, &section, &key);                               // 006DF7BB / 006DF864 / 006DF90D (005E67D0)
}                                                                                         // ~file ~section ~key
}
void __thiscall OptionsResolutionHandlers::OnResolutionChosen_006df690(kih::Control* control) {
    OptionsResolution* const self = res_of(this);
    if (control != nullptr && word_at(control, 0x4C) == 0) return;                        // 006DF6B1..C2
    gui_load::Control* const selected = kotor_m1_listbox_get_selected_control_0041a5d0(&self->lb_resolutions, nullptr);  // 006DF6C8..CE
    const Word index = word_at(selected, 0x58);                                           // 006DF6D3
    DEVMODEA mode;
    std::memset(&mode, 0, sizeof mode);                                                   // 006DF6D6..E1 rep stos 27h
    mode.dmSize = 0x9C;                                                                   // 006DF6EB
    if (EnumDisplaySettingsA(nullptr, index, &mode) != 0)                                 // 006DF6E3..FA (IAT 0073D3E4)
        *options_video::video_mode_bindings.refresh_0078d1dc = mode.dmDisplayFrequency;   // 006DF6FC..703
    if (kotor_fastcall_005ed8d0(client_app(), nullptr, index) != 0) {                     // 006DF709..71F
        String value;                                                                     // 006DF725..29 (005B3190)
        base_ini_reconstruction::PublicIni ini;
        ini.construct_005e6750();                                                         // 006DF72E..3D
        try {
            const auto& v = options_video::video_mode_bindings;
            write_graphics_option(ini, &value, "Width", *v.width_0078d1d4);               // 006DF742..7ED
            write_graphics_option(ini, &value, "Height", *v.height_0078d1d8);             // 006DF7F2..896
            write_graphics_option(ini, &value, "RefreshRate", *v.refresh_0078d1dc);       // 006DF89B..93F
        } catch (...) {
            ini.destroy_005e67e0();
            throw;
        }
        ini.destroy_005e67e0();                                                           // 006DF944..50
    }                                                                                     // 006DF955..64 ~value
    kotor_fastcall_0040be00(self->manager, nullptr);                                      // 006DF969..6C PopModalPanel
    self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                                   // 006DF971..7E
    self->reinit_pending = 1;                                                             // 006DF981 (ebx = 1)
    kotor_fastcall_0040a350(kotor_menu_window_cells::gui_manager_007a39f4, nullptr);       // 006DF987..8D ReloadToolTipPanel
}

// ======================================================================== CSWGuiOptionsGraphicsAdvanced
namespace {
OptionsGraphicsAdvanced* gadv_of(OptionsGraphicsAdvancedHandlers* h) { return reinterpret_cast<OptionsGraphicsAdvanced*>(h); }
// Constructed-member stages of 006E16E0 (EH states 0..11h of FuncInfo 00736BE9).
void unwind_graphics_adv(OptionsGraphicsAdvanced* self, int built) {
    if (built >= 18) gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);
    if (built >= 17) gui_load_button_destruct_00419da0(&self->btn_default, nullptr);
    if (built >= 16) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);
    if (built >= 15) gui_load_button_destruct_00419da0(&self->btn_texqual_right, nullptr);
    if (built >= 14) gui_load_button_destruct_00419da0(&self->btn_texqual_left, nullptr);
    if (built >= 13) gui_load_button_destruct_00419da0(&self->btn_texqual, nullptr);
    if (built >= 12) gui_load_button_destruct_00419da0(&self->btn_antialias_right, nullptr);
    if (built >= 11) gui_load_button_destruct_00419da0(&self->btn_antialias_left, nullptr);
    if (built >= 10) gui_load_button_destruct_00419da0(&self->btn_antialias, nullptr);
    if (built >= 9) gui_load_button_destruct_00419da0(&self->btn_anisotropy_right, nullptr);
    if (built >= 8) gui_load_button_destruct_00419da0(&self->btn_anisotropy_left, nullptr);
    if (built >= 7) gui_load_button_destruct_00419da0(&self->btn_anisotropy, nullptr);
    if (built >= 6) gui_m48_button_toggle_destruct_0041cad0(&self->cb_vsync, nullptr);
    if (built >= 5) gui_m48_button_toggle_destruct_0041cad0(&self->cb_soft_shadows, nullptr);
    if (built >= 4) gui_m48_button_toggle_destruct_0041cad0(&self->cb_framebuffer, nullptr);
    if (built >= 3) gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);
    if (built >= 2) gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);
    if (built >= 1) gui_load_label_destruct_00419d10(&self->desc_item, nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* text_params(Button* button) { return reinterpret_cast<unsigned char*>(&button->text) + 0x18; }   // +16C
}

extern "C" const OptionsPanelMethods<OptionsGraphicsAdvanced> kotor_m1_options_graphics_adv_methods_007584a0 = {
    &kotor_m1_options_graphics_adv_deleting_destruct_006e1f20,   // +00 006E1F20
    KOTOR_M1_OPTIONS_PANEL_SLOTS_04_38,                          // +04..+38 (as 0073E010)
    &kotor_m1_options_graphics_adv_handle_input_006e01c0,        // +3C 006E01C0
    KOTOR_M1_OPTIONS_PANEL_SLOTS_40_68,                          // +40..+68 (as 0073E010)
};

extern "C" {
// ---- 006E16E0 ctor (thiscall, RET 8: manager, parent graphics panel)
OptionsGraphicsAdvanced* __fastcall kotor_m1_options_graphics_adv_construct_006e16e0(OptionsGraphicsAdvanced* self, void*,
    Object* manager, OptionsGraphics* parent) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                            // 006E1704..0B
    int built = 0;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m1_options_graphics_adv_methods_007584a0);  // 006E171B
        gui_load_label_construct_0041acd0(&self->desc_item, nullptr);                built = 1;   // 006E1710..21
        gui_load_label_construct_0041acd0(&self->lbl_title, nullptr);                built = 2;   // 006E1726..31
        gui_load_listbox_construct_0041be40(&self->lb_desc, nullptr);                built = 3;   // 006E1736..41
        construct_checkbox(&self->cb_framebuffer);                                   built = 4;   // 006E1746..58
        construct_checkbox(&self->cb_soft_shadows);                                  built = 5;   // 006E175E..70
        construct_checkbox(&self->cb_vsync);                                         built = 6;   // 006E1776..88
        gui_load_button_construct_0041c0f0(&self->btn_anisotropy, nullptr);         built = 7;   // 006E178E..99
        gui_load_button_construct_0041c0f0(&self->btn_anisotropy_left, nullptr);    built = 8;   // 006E179E..A9
        gui_load_button_construct_0041c0f0(&self->btn_anisotropy_right, nullptr);   built = 9;   // 006E17AE..B9
        gui_load_button_construct_0041c0f0(&self->btn_antialias, nullptr);          built = 10;  // 006E17BE..C9
        gui_load_button_construct_0041c0f0(&self->btn_antialias_left, nullptr);     built = 11;  // 006E17CE..D9
        gui_load_button_construct_0041c0f0(&self->btn_antialias_right, nullptr);    built = 12;  // 006E17DE..E9
        gui_load_button_construct_0041c0f0(&self->btn_texqual, nullptr);            built = 13;  // 006E17EE..F9
        gui_load_button_construct_0041c0f0(&self->btn_texqual_left, nullptr);       built = 14;  // 006E17FE..09
        gui_load_button_construct_0041c0f0(&self->btn_texqual_right, nullptr);      built = 15;  // 006E180E..19
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);               built = 16;  // 006E181E..29
        gui_load_button_construct_0041c0f0(&self->btn_default, nullptr);            built = 17;  // 006E182E..39
        gui_load_button_construct_0041c0f0(&self->btn_cancel, nullptr);             built = 18;  // 006E183E..49
        {
            Ref layout;
            layout.construct_00406d80("optgraphicsadv");                                   // 006E1850..5D (0075848C)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                  // 006E1862..69
        }
        init_control(self, &self->lbl_title, "LBL_TITLE");                                 // 006E186E..9E (00751A20)
        init_control(self, &self->lb_desc, "LB_DESC");                                     // 006E18A3..D3 (00755D6C)
        init_control(self, &self->btn_antialias, "BTN_ANTIALIAS");                         // 006E18D8..08 (0075847C)
        init_control(self, &self->btn_antialias_left, "BTN_ANTIALIASLEFT");                // 006E190D..3D (00758468)
        init_control(self, &self->btn_antialias_right, "BTN_ANTIALIASRIGHT");              // 006E1942..72 (00758454)
        init_control(self, &self->btn_texqual, "BTN_TEXQUAL");                             // 006E1977..A7 (00758448)
        init_control(self, &self->btn_texqual_left, "BTN_TEXQUALLEFT");                    // 006E19AC..DC (00758438)
        init_control(self, &self->btn_texqual_right, "BTN_TEXQUALRIGHT");                  // 006E19E1..11 (00758424)
        init_control(self, &self->cb_framebuffer, "CB_FRAMEBUFF");                         // 006E1A16..40 (00758414)
        init_control(self, &self->btn_anisotropy, "BTN_ANISOTROPY");                       // 006E1A45..75 (00758404)
        init_control(self, &self->btn_anisotropy_left, "BTN_ANISOTROPYLEFT");              // 006E1A7A..AA (007583F0)
        init_control(self, &self->btn_anisotropy_right, "BTN_ANISOTROPYRIGHT");            // 006E1AAF..DF (007583DC)
        init_control(self, &self->cb_soft_shadows, "CB_SOFTSHADOWS");                      // 006E1AE4..14 (007583CC)
        init_control(self, &self->cb_vsync, "CB_VSYNC");                                   // 006E1B19..49 (007583C0)
        self->btn_antialias.u58 = 0xBB63u;                                                 // 006E1B4E..53 (+1384)
        self->btn_antialias_left.u58 = 0xBB63u;                                            // 006E1B59 (+1548)
        self->btn_antialias_right.u58 = 0xBB63u;                                           // 006E1B5F (+170C)
        self->btn_texqual.u58 = 0xBB5Du;                                                   // 006E1B6A (+18D0)
        self->btn_texqual_left.u58 = 0xBB5Du;                                              // 006E1B70 (+1A94)
        self->btn_texqual_right.u58 = 0xBB5Du;                                             // 006E1B76 (+1C58)
        self->cb_framebuffer.u58 = 0xBB55u;                                                // 006E1B81 (+61C)
        self->btn_anisotropy.u58 = 0xBB59u;                                                // 006E1B8B (+E38)
        self->btn_anisotropy_left.u58 = 0xBB59u;                                           // 006E1B91 (+FFC)
        self->btn_anisotropy_right.u58 = 0xBB59u;                                          // 006E1B97 (+11C0)
        self->cb_soft_shadows.u58 = 0xBB51u;                                               // 006E1B9D (+8D0)
        self->cb_vsync.u58 = 0xBB61u;                                                      // 006E1BA7 (+B84)
        reinterpret_cast<unsigned char*>(self)[0x5A5] = 1;                                 // 006E1BB1 (lb_desc+2C1)
        init_control(self, &self->btn_back, "BTN_BACK");                                   // 006E1BB8..E8 (007548AC)
        init_control(self, &self->btn_default, "BTN_DEFAULT");                             // 006E1BED..1D (007583B4)
        init_control(self, &self->btn_cancel, "BTN_CANCEL");                               // 006E1C22..52 (0074FD90)
        gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                // 006E1C57..59
        init_desc_item(&self->desc_item, &self->lb_desc);                                  // 006E1C5E..74 (+5B0 proto, vslot +98)
        kih::GuiObject* const target = event_target(self);
        const gui_load::EventFunc slot = handler(&OptionsGraphicsAdvancedHandlers::OnEnterSlot_006e0460);
        Control* const slotted[12] = { &self->btn_antialias, &self->btn_antialias_left, &self->btn_antialias_right,
                                       &self->btn_texqual, &self->btn_texqual_left, &self->btn_texqual_right,
                                       &self->cb_framebuffer, &self->btn_anisotropy, &self->btn_anisotropy_left,
                                       &self->btn_anisotropy_right, &self->cb_soft_shadows, &self->cb_vsync };
        for (Control* c : slotted) gui_load_control_add_event_0041ab20(c, nullptr, 0, target, slot);   // 006E1C7A..57
        gui_load_control_add_event_0041ab20(&self->btn_antialias_left, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnAntiAliasLeft_006e0520));          // 006E1D5C..6A
        gui_load_control_add_event_0041ab20(&self->btn_antialias_right, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnAntiAliasRight_006e05e0));         // 006E1D6F..7D
        gui_load_control_add_event_0041ab20(&self->btn_texqual_left, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnTexQualLeft_006e04a0));            // 006E1D82..90
        gui_load_control_add_event_0041ab20(&self->btn_texqual_right, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnTexQualRight_006e04e0));           // 006E1D95..A3
        gui_load_control_add_event_0041ab20(&self->btn_anisotropy_left, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnAnisotropyLeft_006e0680));         // 006E1DA8..B6
        gui_load_control_add_event_0041ab20(&self->btn_anisotropy_right, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnAnisotropyRight_006e06d0));        // 006E1DBB..C9
        gui_load_control_add_event_0041ab20(&self->cb_framebuffer, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnFrameBuffer_006ddb10));            // 006E1DCE..D8
        gui_load_control_add_event_0041ab20(&self->cb_soft_shadows, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnSoftShadows_006ddb30));            // 006E1DDD..EB
        gui_load_control_add_event_0041ab20(&self->cb_vsync, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnVSync_006ddb60));                  // 006E1DF0..FE
        kotor_m1_options_graphics_adv_set_up_options_006df480(self, nullptr);              // 006E1E03..05
        self->btn_back.flags &= 0xFFFFFFFBu;                                               // 006E1E0A..25
        gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target, back_thunk());   // 006E1E15..28 (00624BB0)
        self->btn_default.flags &= 0xFFFFFFFBu;                                            // 006E1E2D..43
        gui_load_control_add_event_0041ab20(&self->btn_default, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnDefault_006e0470));                // 006E1E33..46
        self->btn_cancel.flags &= 0xFFFFFFFBu;                                             // 006E1E4B..61
        gui_load_control_add_event_0041ab20(&self->btn_cancel, nullptr, 0x27, target,
            handler(&OptionsGraphicsAdvancedHandlers::OnDiscardChanges_006ddb80));         // 006E1E51..64
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->btn_antialias, 0);   // 006E1E69..6F
        if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(2) == 0 &&                 // 006E1E74..80
            kotor_renderer_init::QueryAntiAliasingMode_0044f140(4) == 0 &&                 // 006E1E82..8E
            kotor_renderer_init::QueryAntiAliasingMode_0044f140(6) == 0 &&                 // 006E1E90..9C
            kotor_renderer_init::QueryAntiAliasingMode_0044f140(8) == 0) {                 // 006E1E9E..AA
            set_enabled(&self->btn_antialias, 0);                                          // 006E1EAC..B1 vslot +88(0)
            gui_load_text_params_set_color_00414e10(text_params(&self->btn_antialias), nullptr,
                                                    &kotor_m1_color_007a2408);             // 006E1EB7..C2 (+1498)
            set_enabled(&self->btn_antialias_left, 0);                                     // 006E1EC7..D4
            set_enabled(&self->btn_antialias_right, 0);                                    // 006E1EDA..E7
        }
        self->discard = 0;                                                                 // 006E1EF0
        self->changed = 0;                                                                 // 006E1EF6
        self->parent = parent;                                                             // 006E1EED..FC ([ebp+0Ch])
    } catch (...) {
        unwind_graphics_adv(self, built);
        throw;
    }
    return self;                                                                           // 006E1F07
}
// ---- 006E1F40 dtor (thiscall, RET); checkboxes destroyed with ~CSWGuiButtonToggle 0041CAD0.
void __fastcall kotor_m1_options_graphics_adv_destruct_006e1f40(OptionsGraphicsAdvanced* self, void*) {
    gui_load_button_destruct_00419da0(&self->btn_cancel, nullptr);                        // 006E1F5D..6B
    gui_load_button_destruct_00419da0(&self->btn_default, nullptr);                       // 006E1F70..7B
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                          // 006E1F80..8B
    gui_load_button_destruct_00419da0(&self->btn_texqual_right, nullptr);                 // 006E1F90..9B
    gui_load_button_destruct_00419da0(&self->btn_texqual_left, nullptr);                  // 006E1FA0..AB
    gui_load_button_destruct_00419da0(&self->btn_texqual, nullptr);                       // 006E1FB0..BB
    gui_load_button_destruct_00419da0(&self->btn_antialias_right, nullptr);               // 006E1FC0..CB
    gui_load_button_destruct_00419da0(&self->btn_antialias_left, nullptr);                // 006E1FD0..DB
    gui_load_button_destruct_00419da0(&self->btn_antialias, nullptr);                     // 006E1FE0..EB
    gui_load_button_destruct_00419da0(&self->btn_anisotropy_right, nullptr);              // 006E1FF0..FB
    gui_load_button_destruct_00419da0(&self->btn_anisotropy_left, nullptr);               // 006E2000..0B
    gui_load_button_destruct_00419da0(&self->btn_anisotropy, nullptr);                    // 006E2010..1B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_vsync, nullptr);                    // 006E2020..2B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_soft_shadows, nullptr);             // 006E2030..3B
    gui_m48_button_toggle_destruct_0041cad0(&self->cb_framebuffer, nullptr);              // 006E2040..4B
    gui_load_listbox_destruct_0041b060(&self->lb_desc, nullptr);                          // 006E2050..5B
    gui_load_label_destruct_00419d10(&self->lbl_title, nullptr);                          // 006E2060..6B
    gui_load_label_destruct_00419d10(&self->desc_item, nullptr);                          // 006E2070..78
    gui_load_panel_destruct_0040cf70(self, nullptr);                                      // 006E207D..87
}
// ---- 006E1F20 scalar deleting dtor (RET 4)
void* __fastcall kotor_m1_options_graphics_adv_deleting_destruct_006e1f20(OptionsGraphicsAdvanced* self, void*, Word flags) {
    kotor_m1_options_graphics_adv_destruct_006e1f40(self, nullptr);                       // 006E1F23
    if (flags & 1u) live_free(self);                                                      // 006E1F28..35
    return self;                                                                          // 006E1F38
}

}   // extern "C" (gfxapply: helpers below have C++ linkage)
namespace {
// One ReadIniEntry(value, ".\swkotor.ini", "Graphics Options", key) of 006E0237..03BB: key, section and file CExoStrings
// are built in that order and destroyed in reverse (file, section, key) around the 005E67C0 call; the value string is
// the caller's (shared by both reads, never reset). Returns the ReadIniEntry result (edi).
std::int32_t read_graphics_option(base_ini_reconstruction::PublicIni& ini, String* value, const char* key_text) {
    String key(key_text);                                                                 // 006E024D..5B / 006E0335..3E
    String section("Graphics Options");                                                   // 006E0260..6E / 006E0343..51 (0073D6AC)
    String file(".\\swkotor.ini");                                                       // 006E0273..81 / 006E0356..64 (0073D648)
    return ini.read_005e67c0(value, &file, &section, &key);                               // 006E0286..A3 / 006E0369..86 (005E67C0)
}                                                                                         // 006E02A8..CF / 006E038B..B2 ~file ~section ~key
std::int32_t as_int(String* value) {                                                      // CExoString::AsINT 005E5630
    return reinterpret_cast<kotor_menu20::String*>(value)->as_int_005e5630();
}
// 006E0237..03FD: the apply path of HandleInputEvent, taken when +2330 (changed) is set after SetNewOptions.
// CExoIni [esp+0Ch] (EH state 0), value CExoString [esp+10h] (EH state 1).
void apply_advanced_graphics(OptionsGraphicsAdvanced* self) {
    base_ini_reconstruction::PublicIni ini;                                               // [esp+0Ch]
    ini.construct_005e6750();                                                             // 006E0237..3B (CExoIni 005E6750)
    try {
        String value;                                                                     // 006E0240..48 (005B3190)
        if (read_graphics_option(ini, &value, "Disable Vertex Buffer Objects") != 0 &&    // 006E024D..D6 (0073D6C0)
            as_int(&value) == 1)                                                          // 006E02D8..E4
            kotor_cdecl_0044ef60();                                                       // 006E02E6 AurDisableVertexBufferObjects
        options_view(options_object())->antialias_0061d710(
            static_cast<unsigned char>(self->anti_alias));                                // 006E02EB..0304 (dl = byte +2318; no null test)
        kotor_renderer_init::Renderer* const renderer =
            static_cast<kotor_renderer_init::Renderer*>(kotor_menu_window_cells::aurora_007a39f8);   // 006E0309 [007A39F8]
        const std::uint32_t fullscreen = kotor_aurora_window::table_of(renderer)->fullscreen(renderer, nullptr);  // 006E030F..13 vslot +04
        const auto& v = options_video::video_mode_bindings;
        const std::uint32_t height = *v.height_0078d1d8;                                  // 006E0316 [0078D1D8]
        const std::uint32_t depth = *v.depth_0078d1e0;                                    // 006E031C..1E movzx byte [0078D1E0]
        const std::uint32_t width = *v.width_0078d1d4;                                    // 006E0325 [0078D1D4]
        v.reinit_aurora_00403800(width, height, depth, fullscreen, 1);                    // 006E0311 push 1 (5th word) .. 006E032D ReInitAurora; add esp,14h
        if (read_graphics_option(ini, &value, "Disable Write-Only VBO") != 0) {           // 006E0335..B9 (0073D694)
            if (as_int(&value) == 1) kotor_cdecl_0044ef70();                              // 006E03BB..C9 AurDisableVertexBufferObjectsWriteOnly
            else kotor_cdecl_0044ef80();                                                  // 006E03D0 AurEnableVertexBufferObjectsWriteOnly
        }
        self->changed = 0;                                                                // 006E03D9
    }                                                                                     // 006E03E3..E8 ~value
    catch (...) {
        ini.destroy_005e67e0();                                                           // EH state 0 (~CExoIni)
        throw;
    }
    ini.destroy_005e67e0();                                                               // 006E03ED..F9 (CExoIni::~CExoIni 005E67E0)
}
}
extern "C" {

// ---- 006E01C0 HandleInputEvent(event, pressed) (thiscall, RET 8)
// 28h/2Eh: discard set -> clear it; else SetNewOptions 006DEFE0 and, when +2330 (changed) is then set, the apply path
// 006E0237..03FD (apply_advanced_graphics: ini "Disable Vertex Buffer Objects", SetAntiAlias, CAurInternalGL +04 FullScreen,
// ReInitAurora 00403800(width, height, depth, fullscreen, 1), ini "Disable Write-Only VBO", changed = 0).
// Then: click sound, changed = 0, PopModalPanel, state 400h, parent->SetUpOptions (006DEF30).
void __fastcall kotor_m1_options_graphics_adv_handle_input_006e01c0(Object* object, void*, I32 event, I32 pressed) {
    OptionsGraphicsAdvanced* const self = static_cast<OptionsGraphicsAdvanced*>(static_cast<Panel*>(object));
    if (pressed != 0) {                                                                   // 006E01DE..E9
        if (event == 0x28 || event == 0x2E) {                                             // 006E01EF..FD
            if (self->discard != 0) {                                                     // 006E0203..09
                self->discard = 0;                                                        // 006E020B
            } else {
                kotor_m1_options_graphics_adv_set_new_options_006defe0(self, nullptr);    // 006E0224..26
                if (self->changed != 0)                                                   // 006E022B..31
                    apply_advanced_graphics(self);                                        // 006E0237..03FD
            }
            manager_of(self)->PlayGuiSound(0);                                            // 006E0400..0A
            self->changed = 0;                                                            // 006E0404
            kotor_fastcall_0040be00(self->manager, nullptr);                              // 006E040F..12 PopModalPanel
            self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                           // 006E0417..2C
            kotor_m1_options_graphics_set_up_options_006def30(self->parent, nullptr);     // 006E041A..2F ([esi+2334h])
        } else if (event == 0x2D) {                                                       // 006E01F7..FA -> 006E0216
            manager_of(self)->PlayGuiSound(0);                                            // 006E0216..1A
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);                 // 006E0434..3E (00409E60)
}

// ---- 006DF480 SetUpOptions (thiscall, RET; EH state 0 FuncInfo 007367AB covers the C2DA allocation)
void __fastcall kotor_m1_options_graphics_adv_set_up_options_006df480(OptionsGraphicsAdvanced* self, void*) noexcept(false) {
    void* const options = options_object();                                              // 006DF48E..A5
    if (options) {                                                                        // 006DF4AA..AE
        if (kotor_cdecl_0045fab0() != 0) {                                                // 006DF4B4..BB
            const Word fb = word_at(options, 0x30);                                       // 006DF4BD
            self->frame_buffer = fb;                                                      // 006DF4CC
            set_toggle(&self->cb_framebuffer, fb);                                        // 006DF4C0..E8
        } else {
            toggle_self(&self->cb_framebuffer)->toggle_flags &= 0xFFFFFFFEu;              // 006DF4F0..01
            set_enabled(&self->cb_framebuffer, 0);                                        // 006DF508..0E vslot +88(0)
            self->frame_buffer = 0;                                                       // 006DF514
        }
        self->anisotropy = static_cast<I32>(word_at(options, 0x34));                      // 006DF51E..21
        if (kotor_cdecl_0045fa20() != 0) {                                                // 006DF527..2E
            const Word soft = word_at(options, 0x28);                                     // 006DF530
            self->soft_shadows = soft;                                                    // 006DF53F
            set_toggle(&self->cb_soft_shadows, soft);                                     // 006DF533..5B
        } else {
            toggle_self(&self->cb_soft_shadows)->toggle_flags &= 0xFFFFFFFEu;             // 006DF563..74
            set_enabled(&self->cb_soft_shadows, 0);                                       // 006DF57B..81
            self->soft_shadows = 0;                                                       // 006DF587
        }
        const Word vsync = word_at(options, 0x40);                                        // 006DF591
        self->vsync = vsync;                                                              // 006DF5A0
        set_toggle(&self->cb_vsync, vsync);                                               // 006DF594..BC
        self->anti_alias = byte_at(options, 0x44);                                        // 006DF5C2..C6
        self->tex_qual = byte_at(options, 0x3C);                                          // 006DF5CC..D0
    }
    kotor_m1_options_graphics_adv_set_anti_alias_text_006df1c0(self, nullptr);            // 006DF5D6..D8
    kotor_m1_options_graphics_adv_set_anisotropy_text_006df2c0(self, nullptr);            // 006DF5DD..DF
    c2da_current::Table* const table = new_texpacks();                                    // 006DF5E4..1E
    if (!table) return;                                                                   // 006DF622..2C
    if (c2da_current_load_004143b0(table, nullptr) == 0) {                                // 006DF62E..39
        delete_table(table);                                                              // 006DF63B..3F vslot +00(1)
        return;
    }
    self->tex_qual_max = static_cast<I32>(c2da_current::load_word(table, 0x24)) - 1;      // 006DF653..57 (row count - 1)
    c2da_current_unload_004139e0(table, nullptr);                                         // 006DF65D
    delete_table(table);                                                                  // 006DF662..68
    kotor_m1_options_graphics_adv_set_tex_qual_text_006df070(self, nullptr);              // 006DF66A..6C
}
// ---- 006DEFE0 SetNewOptions (thiscall, RET)
void __fastcall kotor_m1_options_graphics_adv_set_new_options_006defe0(OptionsGraphicsAdvanced* self, void*) {
    void* const options = options_object();                                              // 006DEFE0..EC
    if (!options) return;                                                                 // 006DEFF3..F5
    options_view(options)->antialias_0061d710(static_cast<unsigned char>(self->anti_alias));   // 006DEFF7..02
    kotor_m1_client_options_set_tex_qual_0061d730(options, nullptr, static_cast<unsigned char>(self->tex_qual));  // 006DF007..12 (movzx-free dl)
    lane_config_required::SetFrameBuffer_0061d750(options, nullptr, static_cast<std::int32_t>(self->frame_buffer)); // 006DF017..20
    options_view(options)->anisotropy_0061d8c0(static_cast<std::uint32_t>(self->anisotropy));  // 006DF025..2E
    options_view(options)->vsync_0061d8e0(self->vsync);                                   // 006DF033..3C
    kotor_m1_client_options_set_soft_shadows_0061d6d0(options, nullptr, self->soft_shadows);   // 006DF041..4A
    if (self->soft_shadows != 0)                                                          // 006DF04F..57
        kotor_m1_client_options_set_shadows_0061d6b0(options, nullptr, 1);                // 006DF059..5D
}
// ---- 006DF070 SetTexQualText (thiscall, RET; EH state 0 FuncInfo 00736773)
void __fastcall kotor_m1_options_graphics_adv_set_tex_qual_text_006df070(OptionsGraphicsAdvanced* self, void*) noexcept(false) {
    c2da_current::Table* const table = new_texpacks();                                    // 006DF08B..C9
    if (!table) return;                                                                   // 006DF0CB..D5
    if (c2da_current_load_004143b0(table, nullptr) == 0) {                                // 006DF0DB..E4
        delete_table(table);                                                              // 006DF0E6..EC
        return;
    }
    if (static_cast<I32>(c2da_current::load_word(table, 0x24)) > 0) {                     // 006DF100..05
        String column("StrrefName");                                                      // 006DF107..10 (007581A4)
        std::int32_t strref;                                                              // [esp+0Ch] (written by 00413660 on success)
        if (c2da_current_get_int_00413660(table, nullptr, self->tex_qual, &column, &strref) != 0)   // 006DF115..4A
            gui_load_text_params_set_strref_00415e50(text_params(&self->btn_texqual), nullptr,
                                                     static_cast<Word>(strref));          // 006DF14C..57 (+19E4)
    }                                                                                     // 006DF135..43 ~column
    self->btn_texqual_left.flags |= 2u;                                                   // 006DF15C..69 (+1A80)
    self->btn_texqual_right.flags |= 2u;                                                  // 006DF16F (+1C44)
    if (self->tex_qual == 0) self->btn_texqual_left.flags &= 0xFFFFFFFDu;                 // 006DF175..86
    else if (self->tex_qual == 2) self->btn_texqual_right.flags &= 0xFFFFFFFDu;           // 006DF188..8C
    self->changed = 1;                                                                    // 006DF195
    c2da_current_unload_004139e0(table, nullptr);                                         // 006DF19F
    delete_table(table);                                                                  // 006DF1A4..AA
}
// ---- 006DF1C0 SetAntiAliasText (thiscall, RET). Jump table 006DF294 over 0..8: 0 BB7Ch, 2 BB7Dh, 4 BB7Fh, 6 BFE4h,
// 8 BFE5h, odd values none (read from the image).
void __fastcall kotor_m1_options_graphics_adv_set_anti_alias_text_006df1c0(OptionsGraphicsAdvanced* self, void*) noexcept(false) {
    Word strref = 0;
    bool set = true;
    switch (self->anti_alias) {                                                           // 006DF1C3..CF (unsigned ja 8)
    case 0: strref = 0xBB7Cu; break;                                                      // 006DF1D6
    case 2: strref = 0xBB7Du; break;                                                      // 006DF1DD
    case 4: strref = 0xBB7Fu; break;                                                      // 006DF1E4
    case 6: strref = 0xBFE4u; break;                                                      // 006DF1EB
    case 8: strref = 0xBFE5u; break;                                                      // 006DF1F2
    default: set = false; break;                                                          // 006DF202
    }
    if (set) gui_load_text_params_set_strref_00415e50(text_params(&self->btn_antialias), nullptr, strref);   // 006DF1F7..FD (+1498)
    self->btn_antialias_left.flags |= 2u;                                                 // 006DF202 (+1534)
    self->btn_antialias_right.flags |= 2u;                                                // 006DF209 (+16F8)
    if (self->anti_alias == 0) self->btn_antialias_left.flags &= 0xFFFFFFFDu;             // 006DF210..1A
    I32 best = 0;                                                                         // 006DF223
    if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(2) != 0) best = 2;            // 006DF221..31
    if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(4) != 0) best = 4;            // 006DF236..44
    if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(6) != 0) best = 6;            // 006DF249..57
    if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(8) != 0) best = 8;            // 006DF25C..6A
    if (self->anti_alias == 8 || !(self->anti_alias < best))                              // 006DF26F..7C
        self->btn_antialias_right.flags &= 0xFFFFFFFDu;                                   // 006DF27E
    self->changed = 1;                                                                    // 006DF286
}
// ---- 006DF2C0 SetAnisotropyText (thiscall, RET). Byte table 006DF380 over value-1 in 0..0Fh -> jump table 006DF368:
// 1 BFB7h, 2 BFB8h, 4 BFB9h, 8 BFBAh, 16 BFBBh, others none (read from the image).
void __fastcall kotor_m1_options_graphics_adv_set_anisotropy_text_006df2c0(OptionsGraphicsAdvanced* self, void*) noexcept(false) {
    Word strref = 0;
    bool set = true;
    switch (self->anisotropy) {                                                           // 006DF2C4..D7
    case 1: strref = 0xBFB7u; break;                                                      // 006DF2DE
    case 2: strref = 0xBFB8u; break;                                                      // 006DF2E5
    case 4: strref = 0xBFB9u; break;                                                      // 006DF2EC
    case 8: strref = 0xBFBAu; break;                                                      // 006DF2F3
    case 16: strref = 0xBFBBu; break;                                                     // 006DF2FA
    default: set = false; break;                                                          // 006DF30A
    }
    if (set) gui_load_text_params_set_strref_00415e50(text_params(&self->btn_anisotropy), nullptr, strref);  // 006DF2FF..05 (+F4C)
    self->btn_anisotropy_left.flags |= 2u;                                                // 006DF30A..17 (+FE8)
    self->btn_anisotropy_right.flags |= 2u;                                               // 006DF31D (+11AC)
    if (self->anisotropy <= 1) {                                                          // 006DF323..30
        self->btn_anisotropy_left.flags &= 0xFFFFFFFDu;                                   // 006DF332
        self->changed = 1;                                                                // 006DF339
        return;
    }
    const I32 value = self->anisotropy;                                                   // 006DF343
    if (value >= kotor_cdecl_0044f090() || value >= 0x10)                                 // 006DF349..55
        self->btn_anisotropy_right.flags &= 0xFFFFFFFDu;                                  // 006DF357
    self->changed = 1;                                                                    // 006DF35F
}
// ---- 006DF390 SetDescription(strref): params +14C, list +2E4, item +64 (extent +68), text +134.
void __fastcall kotor_m1_options_graphics_adv_set_description_006df390(OptionsGraphicsAdvanced* self, void*, Word strref) noexcept(false) {
    describe(&self->desc_item, &self->lb_desc, strref);                                   // 006DF3B0..F45B
}
}

// ---- callbacks (thiscall, RET 4)
// 006DDB10 OnFrameBuffer (no [control+4Ch] guard): frame_buffer = !frame_buffer; changed = 1.
void __thiscall OptionsGraphicsAdvancedHandlers::OnFrameBuffer_006ddb10(kih::Control*) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    const Word v = self->frame_buffer == 0 ? 1u : 0u;                                     // 006DDB10..1A
    self->changed = 1;                                                                    // 006DDB1D
    self->frame_buffer = v;                                                               // 006DDB27
}
// 006DDB30 OnSoftShadows
void __thiscall OptionsGraphicsAdvancedHandlers::OnSoftShadows_006ddb30(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006DDB30..39
    const Word v = self->soft_shadows == 0 ? 1u : 0u;                                     // 006DDB3B..45
    self->changed = 1;                                                                    // 006DDB48
    self->soft_shadows = v;                                                               // 006DDB52
}
// 006DDB60 OnVSync (no guard, no changed flag)
void __thiscall OptionsGraphicsAdvancedHandlers::OnVSync_006ddb60(kih::Control*) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    self->vsync = self->vsync == 0 ? 1u : 0u;                                             // 006DDB60..6D
}
// 006DDB80 OnDiscardChanges: discard = 1; JMP [vptr+54h] (OnBButtonPressed, same stack word).
void __thiscall OptionsGraphicsAdvancedHandlers::OnDiscardChanges_006ddb80(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    self->discard = 1;                                                                    // 006DDB82
    gui_lb::vt<PanelButtonB54View>(static_cast<Object*>(self))->button_b54(self, nullptr, word_of(control));  // 006DDB8C
}
// 006E0460 OnEnterSlot: SetDescription([control+58h]) (tail jump).
void __thiscall OptionsGraphicsAdvancedHandlers::OnEnterSlot_006e0460(kih::Control* control) {
    kotor_m1_options_graphics_adv_set_description_006df390(gadv_of(this), nullptr, word_at(control, 0x58));   // 006E0460..6B
}
// 006E0470 OnDefault
void __thiscall OptionsGraphicsAdvancedHandlers::OnDefault_006e0470(kih::Control* control) {
    if (word_at(control, 0x4C) == 0) return;                                              // 006E0477..7C
    kotor_m1_client_options_default_advanced_graphics_0061d520(options_object(), nullptr);   // 006E047E..8E
    kotor_m1_options_graphics_adv_set_up_options_006df480(gadv_of(this), nullptr);        // 006E0493..95
}
// 006E04A0 OnTexQualLeft
void __thiscall OptionsGraphicsAdvancedHandlers::OnTexQualLeft_006e04a0(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E04A7..AC
    if (self->tex_qual <= 0) return;                                                      // 006E04AE..B6
    --self->tex_qual;                                                                     // 006E04B8..BB
    kotor_m1_options_graphics_adv_set_tex_qual_text_006df070(self, nullptr);              // 006E04C1
    manager_of(self)->PlayGuiSound(1);                                                    // 006E04C6..CB
}
// 006E04E0 OnTexQualRight (bound: tex_qual > tex_qual_max returns; the original allows tex_qual_max + 1)
void __thiscall OptionsGraphicsAdvancedHandlers::OnTexQualRight_006e04e0(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E04E7..EC
    if (self->tex_qual > self->tex_qual_max) return;                                      // 006E04EE..FA (jg)
    ++self->tex_qual;                                                                     // 006E04FC..FF
    kotor_m1_options_graphics_adv_set_tex_qual_text_006df070(self, nullptr);              // 006E0505
    manager_of(self)->PlayGuiSound(1);                                                    // 006E050A..0F
}
// 006E0520 OnAntiAliasLeft: step down to the next supported mode among 6, 4, 2, else 0.
void __thiscall OptionsGraphicsAdvancedHandlers::OnAntiAliasLeft_006e0520(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E0527..2C
    if (self->anti_alias <= 0) return;                                                    // 006E0532..3A
    bool found = false;                                                                   // 006E0541 (edi = 0)
    if (self->anti_alias >= 8) {                                                          // 006E0543..46
        self->anti_alias = 6;                                                             // 006E054A
        if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(6) != 0) found = true;    // 006E0548..60
    }
    if (self->anti_alias >= 6 && !found) {                                                // 006E0565..70
        self->anti_alias = 4;                                                             // 006E0574
        if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(4) != 0) found = true;    // 006E0572..8A
    }
    if (self->anti_alias >= 4) {                                                          // 006E058F..96
        if (!found) {                                                                     // 006E0598..9A
            self->anti_alias = 2;                                                         // 006E059E
            if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(2) == 0)              // 006E059C..B2
                self->anti_alias = 0;                                                     // 006E05BA
        }
    } else if (!found) {                                                                  // 006E05B6..B8
        self->anti_alias = 0;                                                             // 006E05BA
    }
    kotor_m1_options_graphics_adv_set_anti_alias_text_006df1c0(self, nullptr);            // 006E05C4..C6
    manager_of(self)->PlayGuiSound(1);                                                    // 006E05CB..D0
}
// 006E05E0 OnAntiAliasRight: step up to the first supported mode above the current one (2, 4, 6, 8); 8 returns.
void __thiscall OptionsGraphicsAdvancedHandlers::OnAntiAliasRight_006e05e0(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E05E7..EC
    const I32 v = self->anti_alias;                                                       // 006E05F2
    if (v == 8) return;                                                                   // 006E05F8..FB
    static const I32 modes[4] = { 2, 4, 6, 8 };                                           // 006E0602 / 061B / 0634 / 064D
    int first = v <= 0 ? 0 : v <= 2 ? 1 : v <= 4 ? 2 : v <= 6 ? 3 : 4;                    // 006E05FD..4B (jg chain)
    for (int k = first; k < 4; ++k) {
        if (kotor_renderer_init::QueryAntiAliasingMode_0044f140(modes[k]) != 0) {         // 006E0607..5D
            self->anti_alias = modes[k];                                                  // 006E065F
            break;
        }
    }
    kotor_m1_options_graphics_adv_set_anti_alias_text_006df1c0(self, nullptr);            // 006E0665..67
    manager_of(self)->PlayGuiSound(1);                                                    // 006E066C..71
}
// 006E0680 OnAnisotropyLeft: halve (cdq/sub/sar: toward zero), minimum 1.
void __thiscall OptionsGraphicsAdvancedHandlers::OnAnisotropyLeft_006e0680(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E0687..8C
    if (self->anisotropy <= 0) return;                                                    // 006E068E..96
    self->anisotropy = self->anisotropy / 2;                                              // 006E0698..9F
    if (self->anisotropy <= 0) self->anisotropy = 1;                                      // 006E06A5..A7
    kotor_m1_options_graphics_adv_set_anisotropy_text_006df2c0(self, nullptr);            // 006E06B1..B3
    manager_of(self)->PlayGuiSound(1);                                                    // 006E06B8..BD
}
// 006E06D0 OnAnisotropyRight: double while below AurQueryMaximumAnisotropy.
void __thiscall OptionsGraphicsAdvancedHandlers::OnAnisotropyRight_006e06d0(kih::Control* control) {
    OptionsGraphicsAdvanced* const self = gadv_of(this);
    if (word_at(control, 0x4C) == 0) return;                                              // 006E06D7..DC
    const I32 v = self->anisotropy;                                                       // 006E06DF
    if (v >= kotor_cdecl_0044f090()) return;                                              // 006E06E5..EC
    self->anisotropy = v + v;                                                             // 006E06EE..F1
    kotor_m1_options_graphics_adv_set_anisotropy_text_006df2c0(self, nullptr);            // 006E06F7..F9
    manager_of(self)->PlayGuiSound(1);                                                    // 006E06FE..03
}
