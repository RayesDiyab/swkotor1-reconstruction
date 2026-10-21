#pragma once
#include "ffd1b0b297-contracts.h"
namespace options_video {
struct alignas(4) DisplayMode { U8 bytes[156]; };
struct VideoBindings {
    DefaultString default_string_005b3190;
    DestroyString destroy_string_005e5c20;
    ConstructIni construct_ini_005e6750;
    DestroyIni destroy_ini_005e67e0;
    I32 (__fastcall* read_settings_005f0ce0)(Client*,void*,I32*,I32*,U8*,U32*);
    void (__fastcall* fullscreen_0061b710)(void*,void*,I32);
    U32 (__cdecl* exact_mode_0070d220)(U32,U32,U8,U32);
    U32 (__cdecl* nearest_mode_0070d0a0)(U32,U32,U8,U32);
    I32 (__stdcall* enum_modes_0073d3e4)(const char*,U32,DisplayMode*);
    I32 (__fastcall* set_mode_005f1830)(Client*,void*,U32,U8);
    U32* movie_flag_007a3c54;
    // Runtime-attached: the live words are members of the WinMain Views object
    // (winmain.h width_78d1d4 Word, depth_78d1e0 uint8_t); static value null.
    U32* saved_width_0078d1d4;
    // Canonical cell kotor_menu_window_cells::height_0078d1d8 (U32 volatile).
    U32 volatile* saved_height_0078d1d8;
    U8* saved_depth_0078d1e0;
};
extern VideoBindings& video_bindings;
}
