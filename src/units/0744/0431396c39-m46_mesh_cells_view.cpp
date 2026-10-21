// m46_mesh_cells_view.cpp - lane-m46-3dmodel: the single kotor_trimesh::mesh_cells instance (trimesh_skin.h:81-123).
// Pointers to the owners' storage (never copies). Statically initialised: addresses of external objects; the object-
// pointer conversions (D24; marked CAST) are folded by cl into .rdata relocations - verified: no .CRT$XCU / dynamic
// initializer in the object. Each CAST exists only because the MeshCells field type differs from the owner's declared
// type; proposed/trimesh_skin.h.MeshCells.proposed.diff retypes those fields so the casts can be dropped.
// Owner per field (decorated owner names; M45 = defined in the M45 stage objects):
//   0078D460/64/68 texgen enums   -> kotor_renderer_cells::enable_0078d46x (lane-gui-draw spelling, D54 proposed owner;
//                                    NOT defined anywhere yet - already an unresolved name of 3d-vp-vp-f1)
//   0078D46C                      -> kotor_renderer_gl_cells::vertex_enum_0078d46c (M45 gl-renderer-gl-cells)
//   0078D478                      -> texture_upload_cells::gl_texture_cube_map_0078d478 (M45 texture-upload-cells)
//   0078E5FC/630/648/650/66C, 00798B48/4C -> kotor_vp_cells (M45 3d-vp-vp-f2-skinned)
//   0078E608/614/844, 0078E8C0    -> kotor_vp_cells (m46_orphan_cells.cpp; no owner existed; D54 vp cell area)
//   00798A8C / 00798A90           -> kotor_gui_render_cells::alpha_test_gate_00798a8c / alpha_test_ref_00798a90 (M45
//                                    gui-render-aur-gui-image; ref is declared unsigned there: CAST to float view)
//   007A47E0 / 007A6820 / 007A47DC / 007A68A4 -> kotor_renderer_gl_cells (lane-renderer-bodies renderer_gl_loader_cells.cpp;
//                                    not in M45 yet; 007A68A4 is already an M44-base unresolved name)
//   007BB80C / 007BB7B4 / 007BB79C / 007BB770 -> kotor_renderer_gl_procs (M45 gl-renderer-gl-procs)
//   00827FB0 / 00827FB4           -> r4_lowers_cells::render_counts_00827fa8[2] / [3] (M45 r4-lowers-r4-cells, 6-word array
//                                    00827FA8..00827FBC; unsigned elements: CAST to the I view). lane-3d-fb's second
//                                    00827FB0 storage (_aurora_triangle_count_00827fb0) is a C3-class conflict (reported).
//   00827FEC..00827FFC, 00828014  -> renderer_program_cells::output_008xxxxx (lane-window aurora_window_slots owner spelling;
//                                    M44-base unresolved names; void* cells: CAST for the U views)
//   00830294                      -> kotor_vp_cells::parameters_00830294 (M45 3d-vp-vp-f2-skinned; U* volatile: CAST)
//   00830560                      -> renderer_program_cells::parameters_00830560 (vp_nv_ati.h:103 spelling; U*: CAST)
#include "df9dab63cf-trimesh_skin.h"

namespace kotor_renderer_cells {
extern std::uint32_t enable_0078d460;   // 3IA
extern std::uint32_t enable_0078d464;   // 3IA
extern std::uint32_t enable_0078d468;   // 3IA
}
namespace kotor_renderer_gl_cells {
extern std::uint32_t vertex_enum_0078d46c;                                   // 3IA
extern kotor_renderer_init::EnableDispatch enable_dispatch_007a47e0;
extern kotor_renderer_init::ConstantDispatch constant0_dispatch_007a6820;    // 3P6AXPBMM@ZA
extern kotor_renderer_init::ConstantDispatch constant1_dispatch_007a47dc;
extern kotor_renderer_gl::RequestPoolManager* pool_007a68a4;                 // 3PAVRequestPoolManager@kotor_renderer_gl@@A
}
namespace texture_upload_cells { extern std::uint32_t gl_texture_cube_map_0078d478; }   // 3IA
namespace kotor_vp_cells {
extern volatile std::uint32_t gate_0078e5fc, gate_0078e650, gate_0078e66c, gate_00798b48, gate_00798b4c;   // 3IC (M45)
extern volatile float scale_0078e630, scale_0078e648;                                                       // 3MC (M45)
extern volatile std::uint32_t gate_0078e608, gate_0078e614, gate_0078e844, skin_programs_0078e8c0;         // m46_orphan_cells.cpp
extern std::uint32_t* volatile parameters_00830294;                                                         // 3RAIA (M45)
}
namespace kotor_gui_render_cells {
extern volatile std::uint32_t alpha_test_gate_00798a8c;   // 3IC
extern volatile std::uint32_t alpha_test_ref_00798a90;    // 3IC (.data 3EB33333 = 0.35f)
}
namespace kotor_renderer_gl_procs {
extern void (__stdcall* glActiveTextureARB_007bb80c)(std::uint32_t);
extern void (__stdcall* glBindBufferARB_007bb7b4)(std::uint32_t, std::uint32_t);
extern void (__stdcall* glEnableVertexAttribArrayARB_007bb79c)(std::uint32_t);
extern void (__stdcall* glDisableVertexAttribArrayARB_007bb770)(std::uint32_t);
}
namespace r4_lowers_cells { extern std::uint32_t volatile render_counts_00827fa8[6]; }
namespace renderer_program_cells {
extern void* output_00827FEC;   // 3PAXA
extern void* output_00827FF0;
extern void* output_00827FF4;
extern void* output_00827FF8;
extern void* output_00827FFC;
extern void* output_00828014;
extern std::uint32_t* parameters_00830560;   // 3PAIA
}

namespace kotor_trimesh {
namespace {
const MeshCells mesh_cells_storage = {
    &kotor_renderer_cells::enable_0078d460,
    &kotor_renderer_cells::enable_0078d464,
    &kotor_renderer_cells::enable_0078d468,
    &kotor_renderer_gl_cells::vertex_enum_0078d46c,
    &texture_upload_cells::gl_texture_cube_map_0078d478,
    &kotor_vp_cells::gate_0078e5fc,
    &kotor_vp_cells::gate_0078e608,
    &kotor_vp_cells::gate_0078e614,
    &kotor_vp_cells::scale_0078e630,
    &kotor_vp_cells::scale_0078e648,
    &kotor_vp_cells::gate_0078e650,
    &kotor_vp_cells::gate_0078e66c,
    &kotor_vp_cells::gate_0078e844,
    &kotor_vp_cells::skin_programs_0078e8c0,
    &kotor_gui_render_cells::alpha_test_gate_00798a8c,
    &kotor_gui_render_cells::alpha_test_ref_00798a90,        // [M46 compose] no CAST: MeshCells field retyped (proposed diff)
    &kotor_vp_cells::gate_00798b48,
    &kotor_vp_cells::gate_00798b4c,
    &kotor_renderer_gl_cells::enable_dispatch_007a47e0,
    &kotor_renderer_gl_cells::constant0_dispatch_007a6820,
    &kotor_renderer_gl_cells::constant1_dispatch_007a47dc,
    &kotor_renderer_gl_cells::pool_007a68a4,
    &kotor_renderer_gl_procs::glActiveTextureARB_007bb80c,
    &kotor_renderer_gl_procs::glBindBufferARB_007bb7b4,
    &kotor_renderer_gl_procs::glEnableVertexAttribArrayARB_007bb79c,
    &kotor_renderer_gl_procs::glDisableVertexAttribArrayARB_007bb770,
    &r4_lowers_cells::render_counts_00827fa8[2],                      // 00827FB0 (D69 r4 owner; field retyped U)
    &r4_lowers_cells::render_counts_00827fa8[3],                      // 00827FB4 (field retyped U)
    &renderer_program_cells::output_00827FEC,                         // field retyped void**
    &renderer_program_cells::output_00827FF0,                         // field retyped void**
    &renderer_program_cells::output_00827FF4,                         // field retyped void**
    &renderer_program_cells::output_00827FF8,                         // field retyped void**
    &renderer_program_cells::output_00827FFC,                         // field retyped void**
    &renderer_program_cells::output_00828014,
    &kotor_vp_cells::parameters_00830294,                         // field retyped U* volatile*
    &renderer_program_cells::parameters_00830560,                 // field retyped U**
};
}
extern const MeshCells& mesh_cells = mesh_cells_storage;   // ?mesh_cells@kotor_trimesh@@3ABUMeshCells@1@B
} // namespace kotor_trimesh
