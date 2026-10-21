// Sole storage definitions for the GUI-draw runtime cells. Every initial value
// is the original image's cold content: file-backed .data bytes where present
// (.data raw data covers 0078D000..007A3FFF, file offset == VA-0x400000), and
// loader zero-fill for the .data virtual tail 007A4000..00835497. No runtime
// value, clamp, setter or fallback is supplied; runtime state is produced only
// by the original writers listed per cell (see NOTES.md for instruction sites).
#include "33a7cc3451-gui_cells.h"

static_assert(sizeof(short) == 2 && sizeof(float) == 4 && sizeof(std::uint32_t) == 4,
              "MSVC x86 widths");
static_assert(kotor_renderer_cells::viewport_capacity == 10, "Viewport[10] layout");

namespace kotor_renderer_cells {
// 0078D3D4 pulsingAlpha, .data file bytes 00 00 80 3F (binary32 1.0).
// Sole writer CSWGuiDrawable::UpdatePulsing 00414A50 (FSTP m32 at 00414AC8);
// readers pass the raw DWORD (00415CB9, 00416257, 004168F5, 00416903).
std::uint32_t pulse_0078d3d4 = 0x3F800000u;

// 007BB4D0 noGUIRender, .data virtual tail: loader zero. Eight DWORD reads
// (004592F0..0045B293); no direct writer and no stored pointer in the image.
std::uint32_t busy_007bb4d0;

// 0078D45C GL_LIGHTING, .data file bytes 50 0B 00 00. Sixty reads (renderer,
// GUI, SetupOpenGL 00401BD6); no writer in the image.
std::uint32_t disable_0078d45c = 0x00000B50u;

// 007A689C enablePausePlane, .data virtual tail: loader zero. Only three DWORD
// stores of 0 (00459948, 0045A189, 0045A890); no direct reader in the image.
std::uint32_t volatile reset_007a689c;

// 007B9460 currentViewPortIndex, .data virtual tail: loader zero. Signed WORD:
// MOVSX readers; stores 0045921A (=0, StartLayer), 004592BD (=-1, StopLayer),
// 004593A1 (+1, SetupViewport), 00459591 (-1, CloseViewport).
short viewport_index_007b9460;

// 007B9468 viewportStack Viewport[10] (10-byte x,y,width,height,depth WORDs),
// 007B9468..007B94CB, .data virtual tail: loader zero. Writers 004591B0
// (record 0) and 004592F0 (record index+1). Next independently addressed
// object is aurGUIBuffer at 007B94D0.
std::uint16_t viewport_frame_words_007b9468[viewport_capacity * 5];

// 007B9248 viewportAlpha float[10], 007B9248..007B926F, .data virtual tail:
// loader zero. Writers 00459210 (element 0 = 1.0) and 0045938C (FSTP element
// index+1). Next independently addressed object sleepSeconds at 007B9270.
float viewport_alphas_007b9248[viewport_capacity];
}
