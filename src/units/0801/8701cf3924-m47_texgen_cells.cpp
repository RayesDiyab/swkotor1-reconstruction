// lane-m47-3d: GL texgen enum cells 0078D460 / 0078D464 / 0078D468 (.data), spelling kotor_renderer_cells::enable_0078d46x
// as declared by every consumer (stage 3d/vp/vp_f1_lightmap_nv1x.cpp:54, 3d/model46/m46_mesh_cells_view.cpp:28, core46-bsp-cells).
// D54 proposes lane-gui-draw as owner of these cells (same namespace/style as gui-draw/source/gui_cells.cpp:25
// disable_0078d45c); no lane defines them yet -> this unit is the proposed single storage (integrator may move it to gui-draw).
// Values: original .data bytes 0078D460 = 600C0000 610C0000 620C0000 (GL_TEXTURE_GEN_S/T/R 0x0C60/0x0C61/0x0C62).
// Image-wide reference scan (lane-m47-3d NOTES): 25 references 0046FED8..00470709, all loads (8B 0D / 8B 15 / A1); no writer.
// Constant-initialized (no dynamic initializer); non-const storage to match the consumers' `extern std::uint32_t` (3IA).
#include <cstdint>
namespace kotor_renderer_cells {
std::uint32_t enable_0078d460 = 0x00000C60u;   // GL_TEXTURE_GEN_S
std::uint32_t enable_0078d464 = 0x00000C61u;   // GL_TEXTURE_GEN_T
std::uint32_t enable_0078d468 = 0x00000C62u;   // GL_TEXTURE_GEN_R
}
