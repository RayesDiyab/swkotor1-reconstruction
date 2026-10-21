#pragma once
// Sole owners of the original GL capability-enum cells 0078D448..0078D474.
// All are file-backed .data DWORDs with NO writer anywhere in the image
// (dumpbin /disasm xref scan; no stored pointer in .rdata/.data).
// 0078D45C keeps its packet-1 definition (kotor_renderer_cells::disable_0078d45c
// in lane-gui-draw/source/gui_cells.cpp); the other five are defined in
// gl_enum_cells.cpp with the same type (std::uint32_t, writable .data).
#include <cstdint>
namespace kotor_renderer_cells {
extern std::uint32_t enable_0078d448; // GL_BLEND       0x0BE2, 42 reads
extern std::uint32_t enable_0078d44c; // GL_ALPHA_TEST  0x0BC0, 59 reads
extern std::uint32_t enable_0078d454; // GL_CULL_FACE   0x0B44,  9 reads
extern std::uint32_t enable_0078d458; // GL_TEXTURE_2D  0x0DE1, 30 reads
extern std::uint32_t disable_0078d45c;// GL_LIGHTING    0x0B50, 60 reads (packet 1)
extern std::uint32_t enable_0078d474; // GL_DEPTH_TEST  0x0B71, 32 reads
}
