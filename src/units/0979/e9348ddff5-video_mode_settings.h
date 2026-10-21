#pragma once
// lane-config: original video-mode route bodies (swkotor.exe GOG K1, base 0x400000)
//   005F0C60 IsValidResolution      005F0CE0 ReadVideoModeSettings
//   0061B710 SetFullScreenEnabled   0070D220 SetDisplayDevMode
//   0070D0A0 GetNearestVideoMode    005F1830 SetVideoMode
// D1 epoch of the task28 options-video headers (canonical copies: lane-menu-tables
// d1/new, D37). Original __thiscall entries are ABI-identical
// __fastcall(self, unused EDX, ...) with the same callee stack cleanup.
// Shared String/Ini operations come from options_bindings (lane-menu-tables).
#include "dd46b2bf71-options.h"
#include "c66382cced-video.h"
#include "b8b88fc063-window_shared_cells.h"  // task6 cells: reinit_007a3a2c, aurora_007a39f8, height_0078d1d8
#include <cstdint>
namespace options_video {
// Literals, Views-member cells and direct callees used by these bodies. The single
// definition (constant data, D1 slot types) is lane-config src/lane_config_tables.cpp.
// The original user32 imports EnumDisplaySettingsA (IAT 0073D3E4), GetDesktopWindow
// (0073D3BC) and GetWindowRect (0073D3C0) are called directly (original imports).
struct VideoModeBindings {
    const char* width_key_00749ff8;          // "Width"
    const char* height_key_00747580;         // "Height"
    const char* high_frequency_key_0074e818; // "AllowHighMonitorFrequency"
    const char* refresh_key_0074e80c;        // "RefreshRate"
    U32 volatile* width_0078d1d4;            // .data 800: WinMain Views width_78d1d4 (runtime attach, F1)
    U32 volatile* height_0078d1d8;           // .data 600: kotor_menu_window_cells::height_0078d1d8
    U32 volatile* refresh_0078d1dc;          // .data 60:  ReInit Views mode_78d1dc (runtime attach, F1)
    U8 volatile* depth_0078d1e0;             // .data 32:  WinMain Views depth_78d1e0 (runtime attach, F1)
    void (__cdecl* reinit_aurora_00403800)(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t); // ReInitAurora: lane-window kotor_window_reinit_aurora_00403800 (D46)
    void (__fastcall* gui_set_size_0040be70)(void*,void*,I32,I32);                 // CSWGuiManager::SetSize RET 8
    I32 (__fastcall* gui_panel_exists_0040bd70)(void*,void*,void*);                // CSWGuiManager::PanelExists RET 4
    void (__fastcall* main_menu_load_layout_0067ace0)(void*,void*,void*);          // CSWGuiMainMenu::LoadFromLayout RET 4
    void* (__cdecl* allocate_006fa7e6)(std::uint32_t);                              // operator new
    void* (__fastcall* load_screen_0067a710)(void*,void*,void*);                   // CSWGuiLoadScreen ctor RET 4
    void (__fastcall* in_game_reset_0062f5f0)(void*,void*);                        // CGuiInGame::ResetInterfaceForSize
};
extern VideoModeBindings& video_mode_bindings;
// F1 runtime attach of the three Views words (no standalone M40 cell). Returns
// false on null input or a conflicting prior binding; detach restores null.
bool attach_video_mode_cells(U32* width_78d1d4,U32* mode_78d1dc,U8* depth_78d1e0) noexcept;
void detach_video_mode_cells() noexcept;
I32 __fastcall IsValidResolution_005f0c60(Client* self,void* unused_edx,I32 width,I32 height);                              // RET 8
I32 __fastcall ReadVideoModeSettings_005f0ce0(Client* self,void* unused_edx,I32* width,I32* height,U8* depth,U32* refresh); // RET 10h
void __fastcall SetFullScreenEnabled_0061b710(void* options,void* unused_edx,I32 enabled);                                  // RET 4
U32 __cdecl SetDisplayDevMode_0070d220(U32 width,U32 height,U8 depth,U32 refresh);
U32 __cdecl GetNearestVideoMode_0070d0a0(U32 width,U32 height,U8 depth,U32 refresh);
I32 __fastcall SetVideoMode_005f1830(Client* self,void* unused_edx,U32 mode,U8 style);                                      // RET 8
}
