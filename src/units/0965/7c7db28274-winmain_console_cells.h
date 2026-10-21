#pragma once
// lane-m48-fade (D86 f): single owner of the WinMain/UpdateScreen/console .data cells 007A3C50, 007A3A40, 007A3A4C.
// Definitions: src/winmain_console_cells.cpp (one object per original cell, link cells, no view copies).
// Consumers (WinMain replacement, UpdateScreen 00401C10, f-keys wm_keys.cpp replacement) declare exactly these.
#include <cstdint>
extern "C" {
// 007A3C50 headlight flag word: .data, PE image value 0; readers 00401C3A (UpdateScreen), 00404687 (WinMain); no writer
// anywhere in the image (full-image xref) -> keeps its image value 0.
extern volatile std::uint32_t winmain_headlight_007a3c50;
// 007A3A40 console-open word: .data, image value 0; readers 00401871, 00401C7E, 004023F2, 00402499, 004025FC,
// 004047C2, 00404854; writers 00401F50/004021E4 (DoConsole), 0040240F/00402453 (HandleWMCharMessage).
extern volatile std::uint32_t winmain_console_open_007a3a40;
// 007A3A4C console text buffer: .data, image bytes all 0. Extent 0x200 = gap to the next referenced cell 007A3C4C
// (INFERRED from layout; DoConsole bounds its index with 1F4h at 00401FF9/00402129). Written by DoConsole/
// DoConsoleHist (00401F56..0040219A, 004019DC) and WinMain 0040482B (byte 0); passed as a string to AurPostString
// at 00401CA0 (UpdateScreen) and 00404800/00404810 (WinMain).
extern char winmain_console_text_007a3a4c[0x200];
}
