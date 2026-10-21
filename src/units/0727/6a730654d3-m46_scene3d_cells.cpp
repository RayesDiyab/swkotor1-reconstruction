// lane-m46-3dcore fork A: kotor_scene3d::cells (canonical 3d/render/scene3d.h struct Cells). Every member points at
// the ONE object that stores the original cell: an existing M45 owner (declarations below) or m46_cells_storage.cpp.
// Constant-initialised address constants only (D57): no .CRT$XCU, no binding through an extern reference.
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
extern kotor_scene3d::ProgramParameter4f glProgramEnvParameter4fARB_007bb850, glProgramParameter4fNV_007bb7ac;
extern kotor_scene3d::ClientActiveTexture glClientActiveTextureARB_007bb6b0;
extern kotor_scene3d::BindBuffer glBindBufferARB_007bb7b4;
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

namespace kotor_scene3d {
namespace {
const Cells cells_storage = {
    &kotor_renderer_gl_cells::width_0078d1d4,   // width_0078d1d4
    &kotor_menu_window_cells::height_0078d1d8,   // height_0078d1d8
    &scene3d_wireframe_007bb4f0,   // wireframe_007bb4f0
    &scene3d_alt_view_007bb4f4,   // alt_view_007bb4f4
    &scene3d_alt_view_position_007bb504,   // alt_view_position_007bb504
    &scene3d_alt_view_orientation_0078e440,   // alt_view_orientation_0078e440
    &scene3d_plane_index_007bb4ec,   // plane_index_007bb4ec
    &scene3d_plane_normal_007bb4f8,   // plane_normal_007bb4f8
    &scene3d_plane_distance_007bb4e4,   // plane_distance_007bb4e4
    &scene3d_current_camera_007bb4e8,   // current_camera_007bb4e8
    &scene3d_matrix_view_0078d494,   // matrix_view_0078d494
    &scene3d_quaternion_shortcut_007a1980,   // quaternion_shortcut_007a1980
    &scene3d_timer_guard_007b9418,   // timer_guard_007b9418
    &scene3d_timer_frequency_007b9410,   // timer_frequency_007b9410
    &scene3d_animate_start_007b9298,   // animate_start_007b9298
    &scene3d_animate_end_007b9288,   // animate_end_007b9288
    &scene3d_animate_ms_007b92d0,   // animate_ms_007b92d0
    &scene3d_animations_0078e3b8,   // animations_0078e3b8
    &scene3d_interleaved_007b92f4,   // interleaved_007b92f4
    &scene3d_active_scene_007b92a0,   // active_scene_007b92a0
    &scene3d_render_scene_007b935c,   // render_scene_007b935c
    &scene3d_render_start_007b9280,   // render_start_007b9280
    &scene3d_render_end_007b9290,   // render_end_007b9290
    &scene3d_render_ms_007b92d4,   // render_ms_007b92d4
    &scene3d_frame_mod3_0078e3f8,   // frame_mod3_0078e3f8
    &scene3d_render_enabled_0078e3f4,   // render_enabled_0078e3f4
    &scene3d_counter_007b92e0,   // counter_007b92e0
    &scene3d_counter_007b9310,   // counter_007b9310
    reinterpret_cast<volatile U8*>(&scene3d_texture_flag_0078e330),   // texture_flag_0078e330
    &kotor_renderer_cells::enable_0078d44c,   // gl_alpha_test_0078d44c
    &kotor_renderer_cells::disable_0078d45c,   // gl_lighting_0078d45c
    &scene3d_alpha_ref_007b92e8,   // alpha_ref_007b92e8
    reinterpret_cast<const volatile U*>(&texture_cells::delta_t_0078e574),   // frame_time_0078e574
    &scene3d_stat_007b9338,   // stat_007b9338
    &scene3d_stat_007b933c,   // stat_007b933c
    &scene3d_stat_007b9340,   // stat_007b9340
    &scene3d_stat_007b9344,   // stat_007b9344
    &scene3d_stat_007b9348,   // stat_007b9348
    reinterpret_cast<const volatile I*>(&texture_cells::max_texture_time_007a46b0),   // stat_007a46b0
    &kotor_renderer_capability_caches::ati_validate_0078e50c,   // ati_cache_0078e50c
    &kotor_renderer_gl_procs::mask_0078e4d0,   // mask_0078e4d0
    &kotor_renderer_gl_cells::capabilities_007bb85c,   // capabilities_007bb85c
    &kotor_renderer_gl_cells::control_0078e4fc,   // control_0078e4fc
    &scene3d_gui_scene_color_0078d3d8[0],   // gui_scene_color_0078d3d8
    &scene3d_focus_counter_007b92d8,   // focus_counter_007b92d8
    reinterpret_cast<PointerList*>(&historical_storage_007b9398),   // scene_list_007b9398
    &kotor_renderer_cells::enable_0078d448,   // gl_blend_0078d448
    &scene3d_gl_fog_0078d450,   // gl_fog_0078d450
    &scene3d_gl_stencil_test_0078d470,   // gl_stencil_test_0078d470
    &kotor_renderer_cells::enable_0078d474,   // gl_depth_test_0078d474
    &scene3d_static_geometry_0078e400,   // static_geometry_0078e400
    &scene3d_shadows_0078e3a8,   // shadows_0078e3a8
    &scene3d_dynamic_geometry_0078e3fc,   // dynamic_geometry_0078e3fc
    &scene3d_stencil_pass_007b92e4,   // stencil_pass_007b92e4
    &scene3d_distortion_bucket_007fbfc0.size,   // framebuffer_mode_007fbfc4
    &scene3d_distortion_texture_007a68d8,   // distortion_texture_007a68d8
    reinterpret_cast<const volatile U*>(&kotor_renderer_gl_cells::programs_0078e5ec),   // programs_0078e5ec
    &kotor_renderer_gl_procs::glProgramEnvParameter4fARB_007bb850,   // program_env_parameter_007bb850
    &kotor_renderer_gl_procs::glProgramParameter4fNV_007bb7ac,   // program_parameter_nv_007bb7ac
    &scene3d_stencil_bits_0078e54c,   // stencil_bits_0078e54c
    &kotor_renderer_gl_cells::homogenize_a_00831f18[0],   // homogenize_00831f18
    &scene3d_pass_ms_007b92b8,   // pass_ms_007b92b8
    &scene3d_pass_ms_007b92bc,   // pass_ms_007b92bc
    &scene3d_pass_ms_007b92c0,   // pass_ms_007b92c0
    &scene3d_pass_ms_007b92c4,   // pass_ms_007b92c4
    &scene3d_pass_ms_007b92c8,   // pass_ms_007b92c8
    &scene3d_pass_ms_007b92cc,   // pass_ms_007b92cc
    &scene3d_pass_ms_007b92b0,   // pass_ms_007b92b0
    &scene3d_pass_ms_007b92b4,   // pass_ms_007b92b4
    &scene3d_stencil_clear_0078e550,   // stencil_clear_0078e550
    &kotor_vp_cells::generation_00830540,   // vertex_program_epoch_00830540
    &scene3d_gob_bucket_flag_007b92fc,   // gob_bucket_flag_007b92fc
    reinterpret_cast<void** const volatile*>(&scene3d_gob_bucket_007fbfcc.data),   // gob_bucket_007fbfcc
    &scene3d_gob_bucket_007fbfcc.size,   // gob_bucket_count_007fbfd0
    &scene3d_gob_bucket_snapshot_007b92ec,   // gob_bucket_snapshot_007b92ec
    &kotor_winmain::callback_0078e39c,   // callback_0078e39c
    &scene3d_depth_toggle_007b92f0,   // depth_toggle_007b92f0
    &scene3d_cutscene_guard_007b9440,   // cutscene_guard_007b9440
    &scene3d_cutscene_position_007b9434,   // cutscene_position_007b9434
    &scene3d_cutscene_orientation_007b9424,   // cutscene_orientation_007b9424
    &scene3d_focus_lights_007b9300,   // focus_lights_007b9300
    &scene3d_render_gobs_0078e3c4,   // render_gobs_0078e3c4
    &scene3d_dynamic_lights_007b92dc,   // dynamic_lights_007b92dc
    &scene3d_static_light_bucket_007fbfe4,   // static_light_rooms_007fbfe4
    &scene3d_shadow_distance_0078e3c8,   // shadow_distance_0078e3c8
    &scene3d_shadow_distance_check_0078e3cc,   // shadow_distance_check_0078e3cc
    &static_mesh_beams_0078e3d0,   // static_mesh_beams_0078e3d0 (C2 Cells member; storage m46_cells_storage.cpp, .data cold 1)
    &projection_min_radius_0078e3d4,   // projection_min_radius_0078e3d4 (C2 Cells member; storage m46_cells_storage.cpp, .data 5.0f)
    &scene3d_order_by_flag_0078e3b4,   // order_by_flag_0078e3b4
    &scene3d_visibility_test_007b92f8,   // visibility_test_007b92f8
    &scene3d_visibility_ratio_0078e3c0,   // visibility_ratio_0078e3c0
    &scene3d_visibility_interval_0078e3bc,   // visibility_interval_0078e3bc
    &scene3d_force_fog_007b930c,   // force_fog_007b930c
    &scene_black_fog_008305e0,   // black_fog_008305e0
    &scene3d_gl_fog_start_0078d484,   // gl_fog_start_0078d484
    &scene3d_gl_fog_end_0078d488,   // gl_fog_end_0078d488
    &scene3d_gl_fog_density_0078d48c,   // gl_fog_density_0078d48c
    &scene3d_gl_fog_color_0078d490,   // gl_fog_color_0078d490
    &kotor_renderer_cells::enable_0078d454,   // gl_cull_face_0078d454
    &scene3d_bin_bucket_0078e3ac,   // bin_bucket_0078e3ac
    &scene3d_bin_alpha_ref_0078e3b0,   // bin_alpha_ref_0078e3b0
    &kotor_renderer_gl_procs::glClientActiveTextureARB_007bb6b0,   // client_active_texture_007bb6b0
    &kotor_renderer_gl_cells::current_stage_007a6898,   // current_stage_007a6898
    &scene3d_grass_info_00831e44.size,   // grass_materials_00831e48
    &scene3d_bin_bucket_0080aa68.size,   // bin_count_0080aa6c
    reinterpret_cast<void* const* const volatile*>(&scene3d_bin_bucket_0080aa68.data),   // bins_0080aa68
    &kotor_renderer_gl_cells::multitexture_cache_0078e514,   // multitexture_cache_0078e514
    &kotor_renderer_gl_procs::mask_0078e484,   // mask_0078e484
    &scene3d_part_list_00827f40,   // part_list_00827f40
    &scene3d_mesh_buckets_008194e0[0],   // mesh_buckets_008194e0
    &r4_lowers_cells::render_counts_00827fa8[0],   // transform_count_00827fa8
    &scene3d_transform_matrix_008280a0[0],   // transform_matrix_008280a0
    &scene3d_transform_guard_0082809c,   // transform_guard_0082809c
    &scene3d_transform_position_00828090,   // transform_position_00828090
    &scene3d_transform_orientation_00828080,   // transform_orientation_00828080
    &scene3d_part_draw_vector_0078e8ac,   // part_draw_vector_0078e8ac
    &scene3d_transform_enabled_0078e5f0,   // transform_enabled_0078e5f0
    &scene3d_part_depth_0078e5e0,   // part_depth_0078e5e0
    &scene3d_render_enabled_0078e860,   // render_enabled_0078e860
    &scene3d_render_flagged_0078e85c,   // render_flagged_0078e85c
    &scene3d_render_all_00828024,   // render_all_00828024
    &scene3d_fade_test_0082806c,   // fade_test_0082806c
    &scene3d_fade_time_0078e5a0,   // fade_time_0078e5a0
    &scene3d_fade_minimum_0078e858,   // fade_minimum_0078e858
    &r4_lowers_cells::render_counts_00827fa8[1],   // gob_render_count_00827fac
    &scene3d_blur_bucket_007fbf90,   // deferred_gobs_007fbf90
    &scene3d_debug_spheres_00828060,   // debug_spheres_00828060
    &scene3d_debug_frames_0082805c,   // debug_frames_0082805c
    &aurora_event_time_gate_00798b14,   // event_time_gate_00798b14
    &kotor_world_los::drawable_squares_007a693c,   // drawable_squares_007a693c
    &scene3d_light_saber_bucket_007fbff0,   // saber_bucket_007fbff0
    &texture_cells::texture_indices_007a478c,   // texture_ids_007a478c
    &texture_cells::aur_static_texture_list_007a47a4,   // texture_ordered_007a47a4
    &texture_cells::max_tex_id_007a46bc,   // texture_max_id_007a46bc
    &scene3d_personal_space_all_007b9314,   // personal_space_all_007b9314
    &scene3d_render_paths_007b9318,   // render_paths_007b9318
    &kotor_renderer_cells::enable_0078d458,   // gl_texture_2d_0078d458
    &scene3d_lens_flares_0078e3f0,   // lens_flares_0078e3f0
    &scene3d_flare_guard_007b9458,   // flare_guard_007b9458
    &scene3d_flare_pool_a_007b9454,   // flare_pool_a_007b9454
    &scene3d_flare_pool_b_007b9444,   // flare_pool_b_007b9444
    &scene3d_flare_hit_test_0078e3e4,   // flare_hit_test_0078e3e4
    &scene3d_flare_hit_order_0078e3ec,   // flare_hit_order_0078e3ec
    &scene3d_flare_hit_vector_007b9448,   // flare_hit_vector_007b9448
    &scene3d_flare_fade_rate_0078e3e8,   // flare_fade_rate_0078e3e8
    &scene3d_flare_size_scale_0078e3e0,   // flare_size_scale_0078e3e0
    &kotor_renderer_gl_procs::glBindBufferARB_007bb7b4,   // bind_buffer_007bb7b4
    reinterpret_cast<unsigned char* const volatile*>(&scene3d_grass_info_00831e44.data),   // grass_material_table_00831e44
    &scene3d_trigger_alpha_008304b8,   // trigger_alpha_008304b8
    &scene3d_walkmesh_colors_0078e6a8[0],   // walkmesh_colors_0078e6a8
    &scene3d_walkmesh_guard_008280e4,   // walkmesh_guard_008280e4
    &scene3d_walkmesh_table_008280e0,   // walkmesh_table_008280e0
    &scene3d_walkmesh_inverse_guard_008280ec,   // walkmesh_inverse_guard_008280ec
    &scene3d_walkmesh_inverse_table_008280e8,   // walkmesh_inverse_table_008280e8
    &scene3d_rotation_arc_guard_00831f74,   // rotation_arc_guard_00831f74
    &scene3d_rotation_arc_00831f64,   // rotation_arc_00831f64
    &scene_hit_info_007b93a8,   // scene_hit_info_007b93a8
    &scene3d_hit_info_007fbf48,   // bsp_hit_info_007fbf48
    &kotor_renderer_gl_cells::soft_requested_0078e420,   // soft_requested_0078e420
    &kotor_renderer_gl_cells::soft_enabled_0078dae9,   // soft_enabled_0078dae9
    &scene3d_draw_aabb_007fbf5c,   // walkmesh_debug_007fbf5c
    &scene3d_aabb_guard_00828118,   // aabb_guard_00828118
    &scene3d_aabb_colors_00828114,   // aabb_colors_00828114
    &scene3d_aabb_inverse_colors_00828110,   // aabb_inverse_colors_00828110
    &r4_lowers_cells::render_counts_00827fa8[2],   // aabb_face_count_00827fb0
    &scene3d_gate_0078e614,   // trigger_highlight_0078e614
    &scene3d_trigger_bucket_0080aa74,   // trigger_list_0080aa74
    &scene3d_fade_plane_guard_00827f74,   // fade_plane_guard_00827f74
    &scene3d_fade_plane_up_00827f68,   // fade_plane_up_00827f68
    &scene3d_fade_plane_width_0078e5c0,   // fade_plane_width_0078e5c0
    &scene3d_fade_plane_fraction_0078e5bc,   // fade_plane_fraction_0078e5bc
    &scene3d_fade_plane_offset_007fbf64,   // fade_plane_offset_007fbf64
    &scene3d_saber_enabled_0078e840,   // saber_enabled_0078e840
    &scene3d_saber_simple_0078e828,   // saber_simple_0078e828
    &scene3d_saber_glow_0078e834,   // saber_glow_0078e834
    &scene3d_saber_step_0078e838,   // saber_step_0078e838
    &scene3d_saber_min_ratio_0078e83c,   // saber_min_ratio_0078e83c
    &scene3d_saber_max_accum_00798b30,   // saber_max_accum_00798b30
    &scene3d_saber_additive_00828028,   // saber_additive_00828028
    &scene3d_saber_strip_count_00828030,   // saber_strip_count_00828030
    &scene3d_saber_wireframe_00828034,   // saber_wireframe_00828034
    &scene3d_saber_lit_00828038   // saber_lit_00828038
};
}
const Cells& cells = cells_storage;
}
