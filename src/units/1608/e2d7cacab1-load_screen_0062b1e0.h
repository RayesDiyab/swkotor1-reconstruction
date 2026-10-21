#pragma once

// 0062B1E0 phase-2 body. One exact external definition; no cells, adapters or
// helper bodies. EDX is unused; two stack args; __fastcall RET 8.

extern "C" {

void __fastcall load_screen_update_created_in_game_gui_0062b1e0(
    void* self, void* unused, unsigned old_index, unsigned new_index);

}  // extern "C"
