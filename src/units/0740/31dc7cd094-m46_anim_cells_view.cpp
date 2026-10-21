// m46_anim_cells_view.cpp - lane-m46-3dmodel: the single kotor_anim3d::anim_cells instance (anim3d.h:101-111).
// The view holds POINTERS to the borrowed original globals' storage owners (never copies); it is constant-
// initialised (addresses of external objects only) - no dynamic initializer, no .CRT$XCU entry.
// Owner symbols are declared below with the exact owner namespace/type so the decorated names equal the owners':
//   007A4798 ordered texture list  -> lane-texture-epoch texture_cells::ordered_texture_list_007a4798 (M45 texture-epoch-texture_cells)
//   0078E574 frame delta (float)   -> lane-texture-epoch texture_cells::delta_t_0078e574 (M45 texture-epoch-texture_cells)
//   007A6898 current texture stage -> lane-renderer-bodies kotor_renderer_gl_cells::current_stage_007a6898 (M45 gl-renderer-gl-cells)
//   007BB6B0 glClientActiveTextureARB, 007BB80C glActiveTextureARB -> kotor_renderer_gl_procs (M45 gl-renderer-gl-procs)
//   007A47E0 enable dispatch       -> lane-renderer-bodies kotor_renderer_gl_cells::enable_dispatch_007a47e0
//                                     (renderer_gl_loader_cells.cpp, lane objects obj/epoch; NOT yet in M45 - Tier B)
//   0078D44C / 0078D458 GL enum    -> lane-gui-draw kotor_renderer_cells::enable_0078d44c / enable_0078d458 (M45 gui-writers-gl-enum-cells)
// Read sites of every field are cited in anim3d.h (00420E50.., 00485483, 00487D16.., 00487F06, 00487FA9/00487FBA,
// 00487FC8/00488002, 0048805C).
#include "3775ec35d8-anim3d.h"

namespace kotor_reconstruction { struct ArrayMutations; }
namespace texture_cells {
extern kotor_reconstruction::ArrayMutations ordered_texture_list_007a4798;   // ?..@texture_cells@@3UArrayMutations@kotor_reconstruction@@A
extern float volatile delta_t_0078e574;                                      // ?..@texture_cells@@3MC
}
namespace kotor_renderer_gl_cells {
extern std::uint32_t current_stage_007a6898;                                 // 3IA
extern kotor_renderer_init::EnableDispatch enable_dispatch_007a47e0;         // 3P6AXIPAUBasic@texture@@000@ZA
}
namespace kotor_renderer_gl_procs {
extern void (__stdcall* glClientActiveTextureARB_007bb6b0)(std::uint32_t);   // 3P6GXI@ZA
extern void (__stdcall* glActiveTextureARB_007bb80c)(std::uint32_t);         // 3P6GXI@ZA
}
namespace kotor_renderer_cells {
extern std::uint32_t enable_0078d44c;                                        // 3IA
extern std::uint32_t enable_0078d458;                                        // 3IA
}

namespace kotor_anim3d {
namespace {
const Cells anim_cells_storage = {
    &texture_cells::ordered_texture_list_007a4798,
    &texture_cells::delta_t_0078e574,
    &kotor_renderer_gl_cells::current_stage_007a6898,
    &kotor_renderer_gl_procs::glClientActiveTextureARB_007bb6b0,
    &kotor_renderer_gl_procs::glActiveTextureARB_007bb80c,
    &kotor_renderer_gl_cells::enable_dispatch_007a47e0,
    &kotor_renderer_cells::enable_0078d44c,
    &kotor_renderer_cells::enable_0078d458,
};
}
extern const Cells& anim_cells = anim_cells_storage;   // ?anim_cells@kotor_anim3d@@3ABUCells@1@B
} // namespace kotor_anim3d
