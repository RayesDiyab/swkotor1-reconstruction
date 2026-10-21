#pragma once

// Phase-1 load-screen thin providers.
//
// These are C-linkage __fastcall definitions; MSVC decorates them exactly as the
// accepted UI declarations in
// ...\deepseek-camera-provider-continuation-20261018T1547Z\lanes\ui-save-state\source\ui_save_state_types.h.
//
// StartupString is only forward-declared here. This unit never defines or lays out
// the type; the pointer is passed through unchanged to the 0886 target.

namespace kotor_reconstruction {
struct StartupString;
}

extern "C" {

void __fastcall load_screen_set_load_from_save_game_004af060(
    void* server, void* unused, unsigned one);
void __fastcall load_screen_set_load_screen_hint_update_timer_005ee040(
    void* internal, void* unused, unsigned zero);
void __fastcall load_screen_set_gameplay_hint_005ee060(
    void* internal, void* unused, unsigned hint);
void __fastcall load_screen_set_story_hint_005ee080(
    void* internal, void* unused, unsigned hint);

void __fastcall frame_R4_005d0fc0(
    void* vm, void* unused, kotor_reconstruction::StartupString* script,
    unsigned arg1, unsigned arg2);

}  // extern "C"
