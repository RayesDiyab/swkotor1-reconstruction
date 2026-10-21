// lane-m48-fade (D86 f): CExoMoviePlayer::ShowFadePanel 0070C210 and everything it reaches that the composed M48 link
// lacks: CExoMoviePlayerInternal::ShowFadePanel 00404E00, UpdateScreen 00401C10, CSWGuiFade (ctor 00624810, method
// table 0074FC60, ~ 00624920, scalar deleting ~ 00624900, Draw 00624570, HitCheckMouse 006246A0),
// CGuiInGame::GetFadeOverridePanelVisible 0062FAD0 and CClientExoApp::GetPazaakIsRunning 005EE140.
// Evidence: original swkotor.exe disassembly (dumpbin), .rdata words/floats read from the PE (cited per line).
// Conventions: D1 (__thiscall -> __fastcall(self, edx-unused, ...)), D21 typed constant method table, D24, D30-FINAL
// (x87 wide intermediates -> double; float only at the original fstp dword stores).
// Reuse: every callee that the composed link already defines is called through its existing symbol (PACKET.json).
//
// Reachability under the run configuration (Disable Movies=1): both callers of 0070C210 return before it -
// CClientExoAppInternal::PlayMoviesInExoArrayList 00602AF3..00602B00 and ::PlayLoadModuleMovies 00602679..00602681
// test [client_internal+1F8h], which StartServices fills from "Disable Movies" (push 74EAC0h at 005F8F76, CExoString
// ->int 005E5630 at 005F8FFF, store 005F9004). So ShowFadePanel is link-required (lane-config intro binding table)
// but NOT executed at startup in that configuration. It is authored faithfully regardless (D86 f).
#include "e05ffa59c4-mainmenu.h"               // gui-load: Panel/Label/Ref/String, panel/label providers, AddPanel, GetInGameGui
#include "eefeb661b5-gui_ctor.h"               // gui_ctor::Gui::Draw_0040cc50 (CSWGuiManager::Draw)
#include "5907a9dfa7-kotor_unreachable_trap.h" // lane-m48-traps shared D83 trap
#include "7c7db28274-winmain_console_cells.h"  // this lane: 007A3C50 / 007A3A40 / 007A3A4C
#include <cstddef>
#include <cstdint>
#include <cstring>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>

// ---------------------------------------------------------------- existing providers (exact composed-link symbols)
extern "C" {
void __fastcall rim_enter_005e9300(void*, void*);                                  // CExoCriticalSection::EnterCriticalSection
void __fastcall rim_leave_005e9310(void*, void*);                                  // CExoCriticalSection::LeaveCriticalSection
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                     // CClientExoApp::GetSWGuiManager
std::uint32_t __fastcall kotor_fastcall_005edb40(void* outer, void*);             // CClientExoApp::IsMoviePlaying
unsigned __int64 __fastcall kotor_fastcall_004adc70(void* timer, void*);           // CWorldTimer::GetSnapshotTime (EDX:EAX)
void __fastcall gui_load_panel_set_alpha_00404b00(gui_load::Panel*, void*, float); // CSWGuiPanel::SetAlpha (gui-load)
void __cdecl frame_RenderBoundary_0044ed90();                                      // AurMakeCurrent
void __cdecl kotor_cdecl_0044da60();                                               // AurEnableHeadLight (routes-lane body)
void __cdecl kotor_cdecl_0044da80();                                               // AurDisableHeadLight (routes-lane body)
void __cdecl kotor_cdecl_0044d490(const char* text, std::uint32_t, std::int32_t, std::uint32_t); // AurPostString
void __cdecl kotor_cdecl_0044d6a0();                                               // AurRenderStrings
void __cdecl winmain_message_pump_00401510();                                      // messagepump (lane-m48-a)
void __fastcall frame_R4_005d57d0(void*, void*, unsigned);                         // CNetLayer::ProcessReceivedFrames
void __fastcall frame_R4_005d5650(void*, void*, unsigned);                         // CNetLayer::UpdateStatusLoop
}
namespace sound_fade29_required { void __fastcall update_005d5dd0(void*, void*); } // CExoSound::Render thunk (sound_path.h)

// CClientExoApp typed method table view (startup46/client_exo_app.cpp, verbatim) and its dispatcher methods_of(),
// which accepts the raw identity word 0074E6D8 the M40 constructor stores (no Word -> function-pointer conversion).
namespace kotor_client_exo_app {
using Word = std::uint32_t;
struct Methods {
    void (__fastcall* slot00_void_return_0c)(void*, void*, Word, Word, Word);                               // 005C5630 `ret 0Ch`
    void (__fastcall* handle_message)(void*, void*, Word, unsigned char*, Word, Word);                         // 005ED790
    float (__fastcall* get_fps)(void*, void*);                                                                  // 005EE350 (ST0)
    Word (__fastcall* continue_message_processing)(void*, void*);                                               // 005ED7A0
    void* (__fastcall* get_net_layer)(void*, void*);                                                            // 005ED6E0
    void (__fastcall* shut_down_to_main_menu)(void*, void*);                                                    // 005ED890
    void* (__fastcall* get_connection_lib)(void*, void*);                                                       // 005ED520
    Word (__fastcall* slot1c_return_zero)(void*, void*);                                                        // 0063E7F0 `xor eax,eax; ret`
    Word (__stdcall* get_application_id)();                                                                     // 005ED990 (stdcall, ECX unused)
    void (__fastcall* set_application_ids_match)(void*, void*, Word);                                           // 005ED8A0 RET 4
};
const Methods* methods_of(const void* client);
}

// ---------------------------------------------------------------- names defined by this unit
extern "C" {
void __fastcall kotor_fastcall_0070c210(void* player, void*);                      // consumer name (lane_config_tables.cpp:28)
void __fastcall movie_internal_show_fade_panel_00404e00(void* internal, void*) noexcept(false);
void __cdecl kotor_cdecl_00401c10(float delta, std::int32_t run_server, std::int32_t keep_frame);
gui_load::Panel* __fastcall gui_fade_construct_00624810(gui_load::Panel*, void*, gui_load::Object* manager) noexcept(false);
void __fastcall gui_fade_destruct_00624920(gui_load::Panel*, void*);
void* __fastcall gui_fade_deleting_destruct_00624900(gui_load::Panel*, void*, gui_load::Word);
void __fastcall gui_fade_draw_00624570(gui_load::Panel*, void*, gui_load::Word);
gui_load::Object* __fastcall gui_fade_hit_check_mouse_006246a0(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
std::uint32_t __fastcall kotor_fastcall_0062fad0(void* in_game, void*);            // CGuiInGame::GetFadeOverridePanelVisible
std::uint32_t __fastcall kotor_fastcall_005ee140(void* client, void*);             // CClientExoApp::GetPazaakIsRunning
// Existing CSWGuiFade members (core47 gui_ingame_leaves.cpp).
void __fastcall kotor_fastcall_006247a0(void* fade, void*);                        // CSWGuiFade::ResetFade
}

namespace gui_fade {
using U32 = std::uint32_t;
inline U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void* pw(const void* p, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w(p, off))); }
inline void* client() { return pw(kotor_menu_window_cells::manager_007a39fc, 4); }          // [[007A39FC]+4]
inline float bits_float(U32 bits) { float f; std::memcpy(&f, &bits, 4); return f; }

// CSWGuiFade (0x1B8: operator new 1B8h at 00404E2E). Panel 0x64 + four words +64..+70 + CSWGuiLabel (0x140) at +74
// (0062483A lea edi,[esi+74h]) + word +1B4. The four words are used only as observed (ResetFade 006247CF..D8 = 0;
// Draw: +64/+6C/+68 floats 0062461E..36, +70 flag word 0062462E). Names are offsets, not invented semantics.
struct Fade : gui_load::Panel {
    float f64;               // +64 compared/subtracted with +6C (00624621, 00624633)
    float f68;               // +68 divisor (00624636)
    float f6c;               // +6C accumulated by the frame delta (0062461B)
    U32 w70;                 // +70 selects the 1-x branch (0062462E..39)
    gui_load::Label label;   // +74 "LBL_MSG" (006248A6 InitControl)
    U32 w1b4;                // +1B4 snapshot time / 1000 (006247FD, 006245A9)
};
static_assert(sizeof(Fade) == 0x1B8 && offsetof(Fade, f64) == 0x64 && offsetof(Fade, w70) == 0x70 &&
              offsetof(Fade, label) == 0x74 && offsetof(Fade, w1b4) == 0x1B4, "00404E2E push 1B8h / 0062483A");
}

// ================================================================ CSWGuiFade method table 0074FC60 (27 slots)
// Slot words read from the PE (.rdata 0074FC60..0074FCC8); the next word 5F4E5442 is string bytes ("BTN_EXIT").
// Identical to CSWGuiPanel 0073E010 except +00 (00624900), +34 Draw (00624570), +40 HitCheckMouse (006246A0).
extern "C" {
struct GuiFadeMethods {
    void* (__fastcall* s00)(gui_load::Panel*, void*, gui_load::Word);
    void (__fastcall* s04)(gui_load::Object*, void*, const gui_load::Extent*);
    void (__fastcall* s08)(gui_load::Object*, void*, gui_load::Object*, gui_load::Word);
    void (__fastcall* s0c)(gui_load::Object*, void*, gui_load::Word);
    gui_load::Word (__fastcall* s10)(gui_load::Object*, void*, gui_load::Word, gui_load::Word);
    void (__fastcall* s14)(gui_load::Object*, void*);
    void (__fastcall* s18)(gui_load::Object*, void*);
    void (__fastcall* s1c)(gui_load::Object*, void*);
    void (__fastcall* s20)(gui_load::Object*, void*);
    void (__fastcall* s24)(gui_load::Object*, void*);
    gui_load::Object* (__fastcall* s28)(gui_load::Object*, void*);
    gui_load::Word (__fastcall* s2c)(gui_load::Object*, void*);
    gui_load::Word (__fastcall* s30)(gui_load::Object*, void*);
    void (__fastcall* s34)(gui_load::Panel*, void*, gui_load::Word);
    void (__fastcall* s38)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s3c)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
    gui_load::Object* (__fastcall* s40)(gui_load::Object*, void*, gui_load::I32, gui_load::I32);
    gui_load::Object* (__fastcall* s44)(gui_load::Object*, void*);
    void (__fastcall* s48)(gui_load::Panel*, void*);
    void (__fastcall* s4c)(gui_load::Panel*, void*);
    void (__fastcall* s50)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s54)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s58)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s5c)(gui_load::Object*, void*, gui_load::Word);
    void (__fastcall* s60)(gui_load::Object*, void*, gui_load::Word);
    gui_load::String* (__fastcall* s64)(gui_load::Panel*, void*, gui_load::String*);
    void (__fastcall* s68)(gui_load::Panel*, void*);
};
static_assert(sizeof(GuiFadeMethods) == 108, "0074FC60 slot count (next word 5F4E5442 = string bytes)");
extern const GuiFadeMethods gui_fade_methods_0074fc60;
const GuiFadeMethods gui_fade_methods_0074fc60 = {
    &gui_fade_deleting_destruct_00624900,           // +00 00624900 CSWGuiFade::~ (scalar deleting)
    &gui_load_panel_set_extent_0040a5a0,            // +04 0040A5A0
    &gui_load_panel_set_active_control_0040a630,    // +08 0040A630
    &gui_load_slot_ret4_0060e760,                   // +0C 0060E760
    &gui_load_slot_zero8_00409eb0,                  // +10 00409EB0
    &gui_load_slot_ret_004015a0,                    // +14 004015A0
    &gui_load_slot_ret_004015a0,                    // +18 004015A0
    &gui_load_slot_ret_004015a0,                    // +1C 004015A0
    &gui_load_slot_ret_004015a0,                    // +20 004015A0
    &gui_load_slot_ret_004015a0,                    // +24 004015A0
    &gui_load_slot_self_00641db0,                   // +28 00641DB0
    &gui_load_slot_zero_0063e7f0,                   // +2C 0063E7F0
    &gui_load_slot_zero_0063e7f0,                   // +30 0063E7F0
    &gui_fade_draw_00624570,                        // +34 00624570 CSWGuiFade::Draw
    &gui_load_slot_ret4_0060e760,                   // +38 0060E760
    &gui_load_panel_handle_input_00409e60,          // +3C 00409E60
    &gui_fade_hit_check_mouse_006246a0,             // +40 006246A0 CSWGuiFade::HitCheckMouse
    &gui_load_panel_active_control_0040b630,        // +44 0040B630
    &gui_load_panel_on_added_0040b870,              // +48 0040B870
    &gui_load_panel_on_removed_0040c170,            // +4C 0040C170
    &gui_load_panel_button_a_0040b640,              // +50 0040B640
    &gui_load_panel_button_b_0040b650,              // +54 0040B650
    &gui_load_panel_button_x_0040b660,              // +58 0040B660
    &gui_load_panel_button_y_0040b670,              // +5C 0040B670
    &gui_load_panel_button_black_0040b680,          // +60 0040B680
    &gui_load_panel_full_screen_bg_0040a900,        // +64 0040A900
    &gui_load_panel_reset_font_0040a9c0,            // +68 0040A9C0
};

// 00624920 CSWGuiFade::~CSWGuiFade: label (+74) 00419D10 (EH state 0), then panel base 0040CF70 (unwind 00728A60).
void __fastcall gui_fade_destruct_00624920(gui_load::Panel* self, void*) {
    gui_load_label_destruct_00419d10(&static_cast<gui_fade::Fade*>(self)->label, nullptr);         // 0062493D..48
    gui_load_panel_destruct_0040cf70(self, nullptr);                                                // 0062494D..57
}
// 00624900 scalar deleting destructor, RET 4.
void* __fastcall gui_fade_deleting_destruct_00624900(gui_load::Panel* self, void*, gui_load::Word flags) {
    gui_fade_destruct_00624920(self, nullptr);                                                      // 00624903
    if ((flags & 1u) != 0u) live_free(self);                                                        // 00624908..15 (006FA390)
    return self;                                                                                    // 00624918
}

// 00624810 CSWGuiFade::CSWGuiFade(CSWGuiManager*) RET 4. EH: state 0 -> panel dtor (00728A30), state 1 -> label dtor
// (00728A38), state 2 -> CExoString temp dtor (00728A43).
gui_load::Panel* __fastcall gui_fade_construct_00624810(gui_load::Panel* self_panel, void*, gui_load::Object* manager) noexcept(false) {
    using namespace gui_load;
    gui_fade::Fade* const self = static_cast<gui_fade::Fade*>(self_panel);
    gui_load_panel_construct_0040b570(self, nullptr, manager);                                     // 00624828..35
    try {                                                                                           // 0062483F state 0
        self->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_fade_methods_0074fc60);               // 00624847
        gui_load_label_construct_0041acd0(&self->label, nullptr);                                   // 0062483A..4D
        try {                                                                                       // 00624857 state 1
            if (self->manager == nullptr)                                                           // 00624852..5C
                self->manager = static_cast<Object*>(kotor_fastcall_005eda40(gui_fade::client(), nullptr)); // 0062485E..6C
            Ref layout;                                                                             // [esp+0Ch]
            layout.construct_00406d80("fade");                                                      // 0062486F..78 (.rdata 0074FC58 "fade")
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);                           // 0062487D..84
            {
                String tag("LBL_MSG");                                                              // 00624889..92 (.rdata 0074FC50), state 2
                gui_load_panel_init_control_0040b930(self, nullptr, &self->label, &tag, 1);         // 00624897..A6
            }                                                                                       // 006248AB..B4 ~tag (state 1)
            gui_load_panel_stop_layout_0040b8f0(self, nullptr);                                     // 006248B9..BB
            const unsigned char* const m = reinterpret_cast<const unsigned char*>(self->manager);  // 006248C0 eax=[esi+18h]
            std::int16_t mw, mh;
            std::memcpy(&mw, m + 0x6C, 2); std::memcpy(&mh, m + 0x6E, 2);                          // movsx word [eax+6Ch]/[eax+6Eh]
            self->extent.top = 0;                                                                   // 006248C3 [esi+8]
            self->extent.left = 0;                                                                  // 006248CA [esi+4]
            self->extent.width = mw;                                                                // 006248D1..D5 [esi+0Ch]
            self->extent.height = mh;                                                               // 006248D8..DE [esi+10h]
            kotor_fastcall_006247a0(self, nullptr);                                                 // 006248DC..E1 ResetFade
        } catch (...) {
            gui_load_label_destruct_00419d10(&self->label, nullptr);                               // 00728A38
            throw;
        }
    } catch (...) {
        gui_load_panel_destruct_0040cf70(self, nullptr);                                           // 00728A30
        throw;
    }
    return self;                                                                                    // 006248EB
}

// 00624570 CSWGuiFade::Draw(param) RET 4.
void __fastcall gui_fade_draw_00624570(gui_load::Panel* self_panel, void*, gui_load::Word param) {
    using gui_fade::U32;
    gui_fade::Fade* const self = static_cast<gui_fade::Fade*>(self_panel);
    void* const timer = gui_fade::pw(gui_fade::pw(gui_fade::client(), 4), 0x2C);                 // 00624571..7F
    const std::int64_t now = static_cast<std::int64_t>(kotor_fastcall_004adc70(timer, nullptr));   // 00624582
    const U32 seconds = static_cast<U32>(now / 1000);                                               // 00624587..90 __alldiv 3E8h (EAX)
    const U32 delta = seconds - self->w1b4;                                                         // 00624595..9D
    double wide = static_cast<double>(static_cast<std::int32_t>(delta));                            // 006245A1..A5 fild dword (signed)
    self->w1b4 = seconds;                                                                           // 006245A9
    if (static_cast<std::int32_t>(delta) < 0) wide += 4294967296.0;                                 // 006245AF..B1 fadd [0073D770] = 2^32
    const float dt = static_cast<float>(wide * static_cast<double>(0.001f));                        // 006245B7 fmul [0073D708]; 006245C5 fstp
    if (frame_R4_005ed690(gui_fade::client(), nullptr) != nullptr) {                                // 006245BD..D0 GetInGameGui
        void* const in_game = frame_R4_005ed690(gui_fade::client(), nullptr);                       // 006245D2..DB
        if (kotor_fastcall_0062fad0(in_game, nullptr) != 0) return;                                 // 006245E0..E9 -> 00624693 (no draw)
    }
    if (self->f6c > 0.0f) {                                                                         // 006245EF..FD fcomp [0073D700]=0.0, ah&41h
        if (dt < 5.0f)                                                                              // 00624603..12 fcomp [00743264]=5.0, ah&5 / jp
            self->f6c = static_cast<float>(static_cast<double>(dt) + static_cast<double>(self->f6c)); // 00624614..1B
        if (self->f6c > self->f64) {                                                                // 0062461E..29
            double x = (static_cast<double>(self->f6c) - static_cast<double>(self->f64))
                       / static_cast<double>(self->f68);                                            // 0062462B..36
            float alpha;
            if (self->w70 != 0) {                                                                   // 0062462E..39
                x = 1.0 - x;                                                                        // 0062463B fsubr [0073D764]=1.0
                alpha = (0.0 > x) ? 0.0f : static_cast<float>(x);                                   // 00624641..58 / 00624677
            } else {
                alpha = (1.0 < x) ? 1.0f : static_cast<float>(x);                                   // 0062465C..75 / 00624677
            }
            gui_load_panel_set_alpha_00404b00(self, nullptr, alpha);                                // 0062467B..82
        }
    }
    kotor_panel_draw_0040B760(self, nullptr, param);                                                // 00624687..8E CSWGuiPanel::Draw
}

// 006246A0 CSWGuiFade::HitCheckMouse(x, y) RET 8: panel hit check only while alpha > 0.001f.
gui_load::Object* __fastcall gui_fade_hit_check_mouse_006246a0(gui_load::Object* self, void*, gui_load::I32 x, gui_load::I32 y) {
    if (static_cast<gui_load::Panel*>(self)->alpha > 0.001f)                                        // 006246A0..AE fcomp [0073D708], ah&41h
        return gui_load_panel_hit_check_0040b690(self, nullptr, x, y);                              // 006246B0 jmp 0040B690
    return nullptr;                                                                                 // 006246B5..B7
}

// 005EE140 CClientExoApp::GetPazaakIsRunning: return [[this+4]+70h].
std::uint32_t __fastcall kotor_fastcall_005ee140(void* client, void*) {
    return gui_fade::w(gui_fade::pw(client, 4), 0x70);                                              // 005EE140..46
}

// 0062FAD0 CGuiInGame::GetFadeOverridePanelVisible.
std::uint32_t __fastcall kotor_fastcall_0062fad0(void* in_game, void*) {
    using gui_fade::w; using gui_fade::pw;
    if (w(in_game, 0x108) == 0) return 0;                                                           // 0062FAD3..DB
    void* const gui = pw(in_game, 0x38);                                                            // 0062FAE1
    if (gui == nullptr) return 0;                                                                   // 0062FAE4..E6
    const unsigned char bit = 4;                                                                    // 0062FAF5 bl
    static const std::uint32_t offsets[4] = {0x80, 0x78, 0x84, 0x08};                               // 0062FAEC, 0062FB0E, 0062FB2A, 0062FB4C
    for (const std::uint32_t off : offsets) {
        void* const panel = pw(in_game, off);
        if (panel == nullptr) continue;                                                             // test/je per panel
        if (frame_ContainsPanel_0040bd70(pw(in_game, 0x38), nullptr, panel) == 0) continue;         // 0040BD70 (ecx=[esi+38h])
        if ((static_cast<const unsigned char*>(pw(in_game, off))[0x44] & bit) != 0) return 1;       // reload; test byte [p+44h],bl -> 0062FB7D
    }
    return kotor_fastcall_005ee140(gui_fade::client(), nullptr) != 0 ? 1u : 0u;                     // 0062FB68..7E (0 path returns EAX = 0)
}
}  // extern "C"

// ================================================================ UpdateScreen 00401C10 (__cdecl, caller cleans 0Ch)
// Args: [esp+4] float delta (forwarded unchanged to CSWGuiManager::Draw), [esp+8] run_server, [esp+0Ch] keep_frame.
extern "C" void __cdecl kotor_cdecl_00401c10(float delta, std::int32_t run_server, std::int32_t keep_frame) {
    using gui_fade::client;
    frame_RenderBoundary_0044ed90();                                                                // 00401C11 AurMakeCurrent
    if (keep_frame == 0 && kotor_fastcall_005edb40(client(), nullptr) == 0) {                       // 00401C16..2D
        glClear(0x4500);                                                                            // 00401C2F..34 (IAT 0073D21C)
        if (winmain_headlight_007a3c50 != 0) kotor_cdecl_0044da60();                                // 00401C3A..43
        else kotor_cdecl_0044da80();                                                                // 00401C4A
    }
    static_cast<gui_ctor::Gui*>(kotor_fastcall_005eda40(client(), nullptr))->Draw_0040cc50(delta);  // 00401C4F..64
    if (keep_frame == 0 && kotor_fastcall_005edb40(client(), nullptr) == 0) {                       // 00401C69..7C
        if (winmain_console_open_007a3a40 != 0) {                                                   // 00401C7E..85
            static const char prompt_0073d640[] = ">";                                              // .rdata 0073D640 bytes 3E 00
            kotor_cdecl_0044d490(prompt_0073d640, 0, 0, 0x40A00000u);                               // 00401C87..93 (ESI=0; 5.0f)
            kotor_cdecl_0044d490(winmain_console_text_007a3a4c, 2, 0, 0x40A00000u);                 // 00401C98..A5
        }
        kotor_cdecl_0044d6a0();                                                                     // 00401CAD AurRenderStrings
        HDC const dc = GetDC(kotor_menu_window_cells::render_007a39d8);                             // 00401CB2..B9 (IAT 0073D408)
        SwapBuffers(dc);                                                                            // 00401CC1..C2 (IAT 0073D00C)
        ReleaseDC(kotor_menu_window_cells::render_007a39d8, dc);                                    // 00401CC8..D0 (IAT 0073D404)
    }
    winmain_message_pump_00401510();                                                                // 00401CD6
    if (run_server == 1) {                                                                          // 00401CDB..E1
        void* const manager = kotor_menu_window_cells::manager_007a39fc;                            // 00401CE3
        if (manager != nullptr) {                                                                   // 00401CE8..EA
            if (gui_fade::pw(manager, 8) != nullptr) {                                              // 00401CEC..F1
                // D83 branch trap: CServerExoApp::MainLoop 004AE860 (server main loop; module/save/AI subsystems). The
                // only caller in the composed link is ShowFadePanel 00404ED1, which passes run_server = 0 (push 0 at
                // 00404EC3), so this region is statically unreachable (TRAPS entry in PACKET.json).
                kotor_unreachable_trap(0x00401CF3ul, "UpdateScreen@00401CF3 CServerExoApp::MainLoop 004AE860: run_server==1 never passed by a linked caller (00404EC3 push 0)");
            }
            void* const c = gui_fade::pw(manager, 4);                                               // 00401D17..1C
            if (c != nullptr) {                                                                     // 00401D1F..21
                const kotor_client_exo_app::Methods* const m = kotor_client_exo_app::methods_of(c);
                frame_R4_005d57d0(m->get_net_layer(c, nullptr), nullptr, 0);                        // 00401D23..2C (push 0)
            }
        }
    }
    if (kotor_cell_sound_007a39ec != nullptr)                                                       // 00401D31..39
        sound_fade29_required::update_005d5dd0(kotor_cell_sound_007a39ec, nullptr);                 // 00401D3B
    void* const manager = kotor_menu_window_cells::manager_007a39fc;                                // 00401D40
    if (manager != nullptr) {                                                                       // 00401D45..47
        void* const c = gui_fade::pw(manager, 4);                                                   // 00401D49
        if (c != nullptr) {                                                                         // 00401D4C..4E
            if (kotor_client_exo_app::methods_of(c)->get_net_layer(c, nullptr) != nullptr) {        // 00401D50..57
                void* const c2 = client();                                                          // 00401D59..5F
                static const std::uint32_t timeout_0074c538 = 0x00400000u;                          // .rdata 0074C538 (PE word)
                frame_R4_005d5650(kotor_client_exo_app::methods_of(c2)->get_net_layer(c2, nullptr), nullptr,
                                  timeout_0074c538);                                                // 00401D62..70
            }
        }
    }
    static_cast<gui_ctor::Gui*>(kotor_fastcall_005eda40(client(), nullptr))->Draw_0040cc50(0.0f);  // 00401D75..87 (push 0)
}

// ================================================================ ShowFadePanel
// 00404E00 CExoMoviePlayerInternal::ShowFadePanel (thiscall RET 0). EH state 0 (00404E43..00404E95) frees the
// allocation if the CSWGuiFade constructor throws (unwind 007140B0: push [ebp-1Ch]; call 006FA390).
extern "C" void __fastcall movie_internal_show_fade_panel_00404e00(void* internal, void*) noexcept(false) {
    unsigned char* const self = static_cast<unsigned char*>(internal);
    void* const cs = self + 0x40;                                                                   // 00404E1C
    rim_enter_005e9300(cs, nullptr);                                                                // 00404E1F..21
    if (gui_fade::pw(self, 0x44) == nullptr) {                                                      // 00404E26..2B
        void* const memory = c2da_directory_new_006fa7e6(0x1B8);                                    // 00404E2E..33
        gui_load::Panel* fade = nullptr;
        if (memory != nullptr) {                                                                    // 00404E41..4B
            try {
                gui_load::Object* const manager =
                    static_cast<gui_load::Object*>(kotor_fastcall_005eda40(gui_fade::client(), nullptr)); // 00404E4D..55
                fade = gui_fade_construct_00624810(static_cast<gui_load::Panel*>(memory), nullptr, manager); // 00404E5A..5D
            } catch (...) {
                live_free(memory);                                                                  // 007140B0..B4
                throw;
            }
        }
        std::memcpy(self + 0x44, &fade, 4);                                                         // 00404E66
        // 00404E69..00404E9D: three zero stack words stored to [fade+50h],[fade+54h],[fade+58h] (panel colour) with no
        // null check (a failed allocation faults here in the original too). Integer address, volatile stores.
        const std::uintptr_t colour = reinterpret_cast<std::uintptr_t>(fade) + 0x50;               // 00404E75 add eax,50h
        reinterpret_cast<volatile std::uint32_t*>(colour)[0] = 0;                                   // 00404E78
        reinterpret_cast<volatile std::uint32_t*>(colour)[1] = 0;                                   // 00404E92
        reinterpret_cast<volatile std::uint32_t*>(colour)[2] = 0;                                   // 00404E9D
    }
    void* const panel = gui_fade::pw(self, 0x44);                                                   // 00404EA7
    kotor_fastcall_0040bc70(kotor_fastcall_005eda40(gui_fade::client(), nullptr), nullptr, panel, 3, 1); // 00404EA1..B9 AddPanel
    static_cast<gui_load::Panel*>(gui_fade::pw(self, 0x44))->alpha = 1.0f;                         // 00404EBE..CA [esi+4Ch]=3F800000h
    kotor_cdecl_00401c10(gui_fade::bits_float(0x3D088889u), 0, 1);                                  // 00404EC1..D6 UpdateScreen(1/30, 0, 1)
    rim_leave_005e9310(cs, nullptr);                                                                // 00404ED9..DB
}

// 0070C210 CExoMoviePlayer::ShowFadePanel: mov ecx,[ecx]; jmp 00404E00.
extern "C" void __fastcall kotor_fastcall_0070c210(void* player, void*) {
    movie_internal_show_fade_panel_00404e00(gui_fade::pw(player, 0), nullptr);                      // 0070C210..12
}
