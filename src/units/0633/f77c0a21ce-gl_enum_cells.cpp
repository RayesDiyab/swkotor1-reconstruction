// Cold values are the original file-backed .data bytes (file offset == VA-0x400000).
#include "7310ddd144-gl_enum_cells.h"
namespace kotor_renderer_cells {
std::uint32_t enable_0078d448 = 0x00000BE2u; // E2 0B 00 00
std::uint32_t enable_0078d44c = 0x00000BC0u; // C0 0B 00 00
std::uint32_t enable_0078d454 = 0x00000B44u; // 44 0B 00 00
std::uint32_t enable_0078d458 = 0x00000DE1u; // E1 0D 00 00
std::uint32_t enable_0078d474 = 0x00000B71u; // 71 0B 00 00
// lane-m48-a (D32/D51: lane-gui-draw owns GL enum cell 0078D47C): GL_NORMALIZE; only reference image-wide is the load
// 00420C95 mov edx,[0078D47C] (AurTexture normalization-cube-map enable, texture-epoch consumer spelling joined by /alternatename).
std::uint32_t enable_0078d47c = 0x00000BA1u; // A1 0B 00 00
}
