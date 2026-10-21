// Proposed storage owners for original cells consumed by hover_path.cpp that
// have no M40 owner. Initial values are the pinned PE .data words. Integration
// must decide sole ownership; this file adds no reconstructed-function credit.
#include "e2684e9878-hover_path.h"

namespace kotor_input_cells {
// 007A39F4 g_pGuiManager: PE word 0; written by 0040AB28 / 0040BC15 / 00604280.
void* volatile gui_manager_007a39f4 = nullptr;   // D22 canonical type
}

// Read-only color words (no direct .text writes observed; readers listed).
// 007A23C0 enter color {3F7AE148,3F800000,00000000}: 0067B454, 006265C2, 00644825.
extern "C" const MenuColor borrowed_enter_color_007a23c0 = {{0x3F7AE148ul, 0x3F800000ul, 0x00000000ul}};
// 007A23B4 exit color {00000000,3F28F5C3,3F7AE148}: 0067B474, 006269DC, 00644805.
extern "C" const MenuColor borrowed_exit_color_007a23b4 = {{0x00000000ul, 0x3F28F5C3ul, 0x3F7AE148ul}};
// 0078D3C0 button highlight {3F7AE148,3F800000,00000000}: 00418E24 (sole reference).
extern "C" const MenuColor borrowed_highlight_color_0078d3c0 = {{0x3F7AE148ul, 0x3F800000ul, 0x00000000ul}};
