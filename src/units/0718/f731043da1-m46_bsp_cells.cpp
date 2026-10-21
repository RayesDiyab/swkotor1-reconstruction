// lane-m46-3dcore fork A: kotor_scene3d_bsp::{bsp_cells, grass_cells, callback_cells} (3d/bsp/bsp.h, triangle_bin.h,
// bsp_callbacks.h). Same single-storage rule and constant initialisation as m46_scene3d_cells.cpp.
#include "c0efdcfcef-bsp_callbacks.h"
#include "cb0b2681a6-m46_cells_storage.h"
// ---- existing single owners (exact declarations of the defining M45 objects; verified by mangled name)
namespace kotor_renderer_gl_cells {          // aurora-reviewed/gl-cohort renderer_gl_cells.cpp (gl-renderer-gl-cells)
extern std::uint32_t width_0078d1d4, capabilities_007bb85c, control_0078e4fc, current_stage_007a6898,
    multitexture_cache_0078e514, soft_requested_0078e420, vertex_enum_0078d46c;
extern std::int32_t programs_0078e5ec;
extern float homogenize_a_00831f18[4];
extern std::uint32_t current_tid_007a683c[6];
extern std::uint8_t soft_enabled_0078dae9;
}
namespace kotor_renderer_gl_procs {          // aurora-reviewed/gl-cohort renderer_gl_procs.cpp
extern std::uint32_t mask_0078e4d0, mask_0078e484;
}
namespace kotor_menu_window_cells { extern std::uint32_t volatile height_0078d1d8; }   // canonical-inputs/window_shared_cells.h
namespace kotor_renderer_cells {             // gui-draw writers/gl_enum_cells.cpp + gui_cells.cpp (D54 GL enum owner)
extern std::uint32_t enable_0078d448, enable_0078d44c, enable_0078d454, enable_0078d458, enable_0078d474, disable_0078d45c;
}
namespace texture_cells {                    // texture-epoch/include/texture_cells.hpp
extern float volatile delta_t_0078e574;
extern unsigned int volatile max_texture_time_007a46b0, max_tex_id_007a46bc;
extern kotor_reconstruction::ArrayMutations texture_indices_007a478c, aur_static_texture_list_007a47a4;
}
namespace kotor_renderer_capability_caches { extern std::uint32_t ati_validate_0078e50c; }   // texture-upload/capabilities_0045f.cpp
namespace kotor_vp_cells { extern unsigned int volatile generation_00830540, refresh_007a193c; }   // 3d/vp/vp_cells.cpp (D54)
namespace r4_lowers_cells { extern std::uint32_t volatile render_counts_00827fa8[6]; }   // r4-lowers/r4_cells.cpp 00827FA8..BC
namespace kotor_world_los { extern kotor_reconstruction::ArrayMutations drawable_squares_007a693c; }   // world-los (D55)
namespace texture_upload_cells { extern std::int32_t anisotropy_007a685c; }   // upload_cells.cpp
extern "C" { extern volatile std::uint32_t aurora_event_time_gate_00798b14; }   // 3d/model/model_lifetime.cpp
// ---- owners outside M45 (undefined externals until their owner lands; see parts/A.json)
// task-7 historical storage extents (3d/model/task7/storage.h; referenced-not-defined in M45 already)
struct CleanupStorage12 { void* volatile head; std::uint8_t opaque[8]; };
extern "C" { extern CleanupStorage12 historical_storage_007b9398, historical_storage_007a8a64; }
// 0078E39C: WinMain stores 00401510 (00404418) into an untyped Word; D34: lane-prepare must publish a typed view.
namespace kotor_winmain { extern kotor_scene3d::Callback volatile callback_0078e39c; }

namespace kotor_renderer_gl_cells {   // lane-renderer-bodies src/cohort/renderer_gl_loader_cells.cpp (written by InitExtensions 00436490)
extern kotor_renderer_init::EnableDispatch enable_dispatch_007a47e0;
}
namespace kotor_renderer_cells { extern std::uint32_t enable_0078d460, enable_0078d464, enable_0078d468; }   // gui-draw (D54 proposed)
namespace renderer_program_cells { extern std::uint32_t parameters_00830560[]; }   // D54 lane-3d-vp / task-22 program_cells.cpp
namespace kotor_scene3d_bsp {
namespace {
// Indexed view of the 12 GL enum cells 0078D448..0078D474 read as [esi*4+0078D448] by GLRender::BindTexture 00420445.
const GlEnumCell gl_enum_table_view[12] = {
    &kotor_renderer_cells::enable_0078d448, &kotor_renderer_cells::enable_0078d44c, &scene3d_gl_fog_0078d450,
    &kotor_renderer_cells::enable_0078d454, &kotor_renderer_cells::enable_0078d458, &kotor_renderer_cells::disable_0078d45c,
    &kotor_renderer_cells::enable_0078d460, &kotor_renderer_cells::enable_0078d464, &kotor_renderer_cells::enable_0078d468,
    &kotor_renderer_gl_cells::vertex_enum_0078d46c, &scene3d_gl_stencil_test_0078d470, &kotor_renderer_cells::enable_0078d474};
const BspCells bsp_storage = {
    &gl_enum_table_view[0],   // gl_enum_table_0078d448
    &kotor_renderer_cells::enable_0078d458,   // gl_texture_2d_0078d458
    &kotor_renderer_cells::disable_0078d45c,   // gl_lighting_0078d45c
    &scene3d_gl_stencil_test_0078d470,   // gl_stencil_test_0078d470
    &kotor_renderer_gl_cells::current_tid_007a683c[0],   // current_tid_007a683c
    &texture_upload_cells::anisotropy_007a685c,   // anisotropy_007a685c
    &kotor_renderer_gl_cells::current_stage_007a6898,   // current_stage_007a6898
    &scene3d_gate_0078e614,   // gate_0078e614
    &scene3d_gate_0078e5a8,   // gate_0078e5a8
    &kotor_vp_cells::refresh_007a193c,   // vertex_programs_reset_007a193c
    &scene3d_tile_step_0078e428,   // tile_step_0078e428
    &scene3d_bins_collected_007fbf28,   // bins_collected_007fbf28
    &scene3d_bins_visited_007fbf2c,   // bins_visited_007fbf2c
    &scene3d_stat_007fbf38,   // stat_007fbf38
    &scene3d_stat_007fbf3c,   // stat_007fbf3c
    &scene3d_stat_007fbf44,   // stat_007fbf44
    reinterpret_cast<const volatile U*>(&scene3d_hit_info_007fbf48),   // hit_mask_007fbf48
    &scene3d_hit_trimeshes_007fbf4c,   // hit_trimeshes_007fbf4c
    &scene3d_model_7a8a68_copy_007fbf30,   // model_7a8a68_copy_007fbf30
    reinterpret_cast<const volatile U*>(&historical_storage_007a8a64.opaque[0]),   // models_007a8a68
    &scene3d_beam_bucket_007fbf6c,   // beam_bucket_007fbf6c
    &scene3d_emitter_bucket_007fbf78,   // emitter_bucket_007fbf78
    &scene3d_static_shadow_bucket_007fbf84,   // static_shadow_bucket_007fbf84
    &scene3d_blur_bucket_007fbf90,   // blur_bucket_007fbf90
    &scene3d_active_bins_007fbf9c,   // active_bins_007fbf9c
    &scene3d_fading_out_bucket_007fbfa8,   // fading_out_bucket_007fbfa8
    &scene3d_mesh_shadow_bucket_007fbfb4,   // mesh_shadow_bucket_007fbfb4
    &scene3d_distortion_bucket_007fbfc0,   // distortion_bucket_007fbfc0
    &scene3d_gob_bucket_007fbfcc,   // gob_bucket_007fbfcc
    &scene3d_fading_in_bucket_007fbfd8,   // fading_in_bucket_007fbfd8
    &scene3d_static_light_bucket_007fbfe4,   // static_light_bucket_007fbfe4
    &scene3d_light_saber_bucket_007fbff0,   // light_saber_bucket_007fbff0
    &scene3d_old_active_bins_007fbffc,   // old_active_bins_007fbffc
    &scene3d_cap_buckets_007fc008[0],   // cap_buckets_007fc008
    &scene3d_bin_bucket_0080aa68,   // bin_bucket_0080aa68
    &scene3d_trigger_bucket_0080aa74,   // trigger_bucket_0080aa74
    &scene3d_fade_buckets_0080aa80[0],   // fade_buckets_0080aa80
    &scene3d_mesh_buckets_008194e0[0],   // mesh_buckets_008194e0
    &scene3d_part_list_00827f40,   // part_list_00827f40
    &scene3d_current_camera_007bb4e8,   // current_camera_007bb4e8
    &scene3d_gob_bucket_snapshot_007b92ec,   // gob_bucket_snapshot_007b92ec
    &kotor_winmain::callback_0078e39c   // callback_0078e39c
};
const GrassCells grass_storage = {
    &scene3d_grass_info_00831e44.data,   // grass_info_00831e44
    &kotor_renderer_gl_cells::enable_dispatch_007a47e0,   // enable_dispatch_007a47e0
    reinterpret_cast<void* volatile*>(&renderer_program_cells::parameters_00830560[0]),   // bss_00830560
    reinterpret_cast<volatile I*>(&r4_lowers_cells::render_counts_00827fa8[2]),   // triangles_00827fb0
    &kotor_renderer_gl_cells::vertex_enum_0078d46c,   // vertex_program_0078d46c
    reinterpret_cast<const volatile U*>(&kotor_renderer_gl_cells::programs_0078e5ec),   // programs_0078e5ec
    &scene3d_active_scene_007b92a0,   // active_scene_007b92a0
    reinterpret_cast<const volatile U*>(&texture_cells::delta_t_0078e574)   // frame_time_0078e574
};
const CallbackCells callback_storage = {
    &scene3d_draw_aabb_007fbf5c,   // draw_aabb_007fbf5c
    reinterpret_cast<const volatile U*>(&texture_cells::delta_t_0078e574),   // frame_time_0078e574
    &scene3d_fade_time_0078e5a0   // fade_time_0078e5a0
};
}
const BspCells& bsp_cells = bsp_storage;
const GrassCells& grass_cells = grass_storage;
const CallbackCells& callback_cells = callback_storage;
}
