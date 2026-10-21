#pragma once

// Phase-2 A-directed load-screen thunks. These are the two external contracts
// consumed by the accepted UI source; each jumps to an accepted lane-A provider.
// Source-only successor to the accepted phase-1 source.

extern "C" {

void __fastcall load_screen_send_load_game_request_005ed680(
    void* internal, void* unused, int number, void* suffix, void* module);
void __fastcall load_screen_busy_wait_mute_sound_005ee130(
    void* internal, void* unused);

}  // extern "C"
