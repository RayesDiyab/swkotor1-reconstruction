// lane-m46-startup: leaf callees of lane-config lane_config_tables.cpp (namespace lane_config_required, lines
// 48-58) and the HideFadePanel pair used by its movie bindings (line 29). Evidence: lane-m46-startup/dis/*.asm.
// Original Aur* toggles are __stdcall(void) with RET 0 - ABI-identical to the consumer's __cdecl(void).
#include <cstdint>
#include <cstring>

// Cell owners (declarations identical to the owners'):
//  0078E3A8 / 0078E3AC: lane-m46-3dcore m46_cells_storage.h (extern "C" volatile U32, .data cold 1).
//  0078E420 / 007BB860: gl-renderer-gl-cells renderer_gl_cells.h (kotor_renderer_gl_cells::U32).
extern "C" {
extern volatile std::uint32_t scene3d_shadows_0078e3a8;
extern volatile std::uint32_t scene3d_bin_bucket_0078e3ac;   // AurEnableGrass/AurDisableGrass flag (owner's name)
}
namespace kotor_renderer_gl_cells {
extern std::uint32_t soft_requested_0078e420;
extern std::uint32_t ati_soft_gate_007bb860;
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // window_shared_cells.h (M40)

namespace gui_load { struct Panel; }
extern "C" {
void __fastcall rim_enter_005e9300(void*, void*);                                  // rim-critical-joins (CExoCriticalSection::Enter)
void __fastcall rim_leave_005e9310(void*, void*);                                  // rim-critical-joins (Leave)
void __fastcall gui_load_panel_set_alpha_00404b00(gui_load::Panel*, void*, float); // gui-load-gui-control-load
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                     // CClientExoApp::GetSWGuiManager
void __fastcall frame_RemovePanel_0040c830(void* gui, void*, void* panel);         // r4-lowers-r4-gui-lowers
void __fastcall kotor_fastcall_0070c220(void*, void*);                             // lane_config_tables.cpp:29
void __fastcall movie_internal_hide_fade_panel_00404f00(void*, void*);             // new name (this unit)
}

namespace lane_config_required {
// 0044EE20 AurEnableShadows: [0078E3A8]=1.
void __cdecl EnableShadows_0044ee20() { scene3d_shadows_0078e3a8 = 1; }
// 0044EE30 AurEnableGrass: [0078E3AC]=1.   0044EE40 AurDisableGrass: [0078E3AC]=0.
void __cdecl EnableGrass_0044ee30() { scene3d_bin_bucket_0078e3ac = 1; }
void __cdecl DisableGrass_0044ee40() { scene3d_bin_bucket_0078e3ac = 0; }
// 0044EE50 AurEnableSoftShadows: [0078E420]=1 then [0078E3A8]=1 (0044EE55, 0044EE5A).
void __cdecl EnableSoftShadows_0044ee50() { kotor_renderer_gl_cells::soft_requested_0078e420 = 1; scene3d_shadows_0078e3a8 = 1; }
// 0044EE60 AurDisableSoftShadows: [0078E420]=0.
void __cdecl DisableSoftShadows_0044ee60() { kotor_renderer_gl_cells::soft_requested_0078e420 = 0; }
// 0044EF90 AurATIUnlockSoftShadowsForHotFix: [007BB860]=1 then [0078E420]=0.
void __cdecl UnlockSoftShadows_0044ef90() { kotor_renderer_gl_cells::ati_soft_gate_007bb860 = 1; kotor_renderer_gl_cells::soft_requested_0078e420 = 0; }
// 0044F080 AurDisableShadows: [0078E3A8]=0 then [0078E420]=0 (0044F082, 0044F087).
void __cdecl DisableShadows_0044f080() { scene3d_shadows_0078e3a8 = 0; kotor_renderer_gl_cells::soft_requested_0078e420 = 0; }
// 004AEE70 CServerExoApp::GetPartyTable: return [this+4] + 1B770h (address arithmetic, no load of the table).
void* __fastcall GetPartyTable_004aee70(void* server, void*) {
    std::uint32_t internal;
    std::memcpy(&internal, static_cast<const unsigned char*>(server) + 4, 4);                         // 004AEE70
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(internal + 0x1b770u));                 // 004AEE73
}
}

// 00404F00..00404F38 CExoMoviePlayerInternal::HideFadePanel (thiscall RET 0):
//  cs = this+40h; Enter(cs); p=[this+44h]; if p { SetAlpha(p, 0.0f) 00404B00; gui = GetSWGuiManager([[007A39FC]+4]);
//  RemovePanel(gui, [this+44h] reloaded) 0040C830 } ; tail JMP Leave(cs) 005E9310.
extern "C" void __fastcall movie_internal_hide_fade_panel_00404f00(void* internal, void*) {
    unsigned char* const self = static_cast<unsigned char*>(internal);
    void* const cs = self + 0x40;                                                  // 00404F04
    rim_enter_005e9300(cs, nullptr);                                               // 00404F07..00404F09
    void* panel;
    std::memcpy(&panel, self + 0x44, 4);                                      // 00404F0E
    if (panel) {                                                                   // 00404F11..00404F13
        gui_load_panel_set_alpha_00404b00(static_cast<gui_load::Panel*>(panel), nullptr, 0.0f);   // 00404F15..00404F17 (push 0 = +0.0f)
        void* app;
        std::memcpy(&app, static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 4, 4);   // 00404F1C..00404F24
        void* panel_again;
        std::memcpy(&panel_again, self + 0x44, 4);                            // 00404F21
        void* const gui = kotor_fastcall_005eda40(app, nullptr);                   // 00404F27..00404F28
        frame_RemovePanel_0040c830(gui, nullptr, panel_again);                     // 00404F2D..00404F2F
    }
    rim_leave_005e9310(cs, nullptr);                                               // 00404F34..00404F38 (tail)
}
// 0070C220 CExoMoviePlayer::HideFadePanel: mov ecx,[ecx]; jmp 00404F00.
extern "C" void __fastcall kotor_fastcall_0070c220(void* player, void*) {
    void* internal;
    std::memcpy(&internal, player, 4);                                        // 0070C220
    movie_internal_hide_fade_panel_00404f00(internal, nullptr);                    // 0070C222
}
