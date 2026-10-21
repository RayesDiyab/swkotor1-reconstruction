// lane-m48-fade (D86 f): storage of the original .data cells 007A3C50 / 007A3A40 / 007A3A4C (one owner each).
// PE evidence: section .data VA 0078D000 raw size 17000h -> all three lie in the initialized part; image bytes at
// 007A3A40, 007A3A4C..007A3C4B and 007A3C50 are all zero (read from swkotor.exe). Zero-initialised storage reproduces
// the image value exactly. The WinMain view struct fields Globals::headlight_7a3c50 / debug_7a3a40 / text_7a3a4c
// are no longer read or written by any composed unit (see PACKET composer_instructions) - no dual storage.
#include "7c7db28274-winmain_console_cells.h"
extern "C" {
volatile std::uint32_t winmain_headlight_007a3c50 = 0;     // 007A3C50
volatile std::uint32_t winmain_console_open_007a3a40 = 0;  // 007A3A40
char winmain_console_text_007a3a4c[0x200] = {};            // 007A3A4C..007A3C4B (extent inferred, see header)
}
