// lane-m46-3dcore fork A: original cell storage (constant-initialised; no dynamic initialiser, no .CRT$XCU).
// Each line: original address, .data/BSS, consumer evidence (instruction addresses from the canonical scene3d.h /
// bsp.h comments) and, where the original constructs the object through an __xc entry, that entry. Floats are
// exact hex literals of the original float32 words (D57). Original atexit disposers (0073BA00..0073BEB0) only
// free list data at shutdown and are not registered here (D60 precedent: shutdown-only deviation).
#include "cb0b2681a6-m46_cells_storage.h"
namespace {
using namespace m46_cells;
// CHitInfo state after the original static initializer 0073AEF0: push 0 / push BFFFFFFFh / push 0; mov ecx,7B93A8h;
// call 004578F0. 004578F0 zeroes +08,+0C,+10,+1C..+30,+38,+40..+48 (0045790F..0045793B), stores args at +00
// (0045793E), +04 (00457946) and +14 (00457956), byte +5D = 1 (00457951), +58 = 0, bytes +5C/+5E/+5F/+60 = 0, then
// 00456A60: +18 = 0 (00456A6D), [+38] == 0 so no 00445080 call, +38 = 0, +40/+1C/+28 triples = 0, +34/+4C/+50/+54 = 0,
// +3C = FFFFFFFFh (00456AD5). Bytes +61..+63 are never written (BSS 0). Pure stores of constants: this image is
// the exact constructed state. The atexit destructor 00456B30 (unregister +38, free +08) is not registered.
constexpr CHitInfoImage kHitInfoConstructed = {
    {0x00000000u, 0xBFFFFFFFu, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0xFFFFFFFFu,   // +00 arg0, +04 arg1, +3C
     0u, 0u, 0u, 0u, 0u, 0u, 0u},                                                         // +40..+58
    {0x00u, 0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u}};   // +5C, +5D = 1, +5E..+60 = 0, +61..+63 unwritten
}
extern "C" {
volatile U scene3d_gui_scene_color_0078d3d8[3] = {0xBF800000u, 0xBF800000u, 0xBF800000u}; // 0078D3D8 .data | 00417569..00417580 three DWORDs 78D3D8/78D3DC/78D3E0
volatile U scene3d_gl_fog_0078d450 = 0x00000B60u;                      // 0078D450 .data | 004519D0 glDisable (0x0B60; no owner yet)
volatile U scene3d_gl_stencil_test_0078d470 = 0x00000B90u;             // 0078D470 .data | 004515C8 / 0045199F glDisable (0x0B90; no owner yet)
volatile U scene3d_gl_fog_start_0078d484 = 0x00000B63u;                // 0078D484 .data | .data 0x0B63
volatile U scene3d_gl_fog_end_0078d488 = 0x00000B64u;                  // 0078D488 .data | .data 0x0B64
volatile U scene3d_gl_fog_density_0078d48c = 0x00000B62u;              // 0078D48C .data | .data 0x0B62
volatile U scene3d_gl_fog_color_0078d490 = 0x00000B66u;                // 0078D490 .data | .data 0x0B66
volatile U scene3d_matrix_view_0078d494 = 0x00000001u;                 // 0078D494 .data | 00425CBE
volatile U scene3d_texture_flag_0078e330 = 0x00000001u;                // 0078E330 .data | 00451372 byte = 1
volatile U scene3d_shadows_0078e3a8 = 0x00000001u;                     // 0078E3A8 .data | 00451750 gate (.data cold 1)
volatile U scene3d_bin_bucket_0078e3ac = 0x00000001u;                  // 0078E3AC .data | 00450150 (.data cold 1)
volatile float scene3d_bin_alpha_ref_0078e3b0 = 0x1.99999ap-2f;        // 0078E3B0 .data | 004501DA (.data 0.4f) (raw 3ECCCCCD)
volatile U scene3d_order_by_flag_0078e3b4 = 0x00000001u;               // 0078E3B4 .data | 004502DD (.data cold 1)
volatile U scene3d_animations_0078e3b8 = 0x00000001u;                  // 0078E3B8 .data | 00451C8F (.data cold 1)
volatile float scene3d_visibility_interval_0078e3bc = 0x1.51eb86p-2f;  // 0078E3BC .data | 00457A8C (.data 0.33f) (raw 3EA8F5C3)
volatile float scene3d_visibility_ratio_0078e3c0 = 0x1.47ae14p-5f;     // 0078E3C0 .data | 00457A4C (.data 0.04f) (raw 3D23D70A)
volatile U scene3d_render_gobs_0078e3c4 = 0x00000001u;                 // 0078E3C4 .data | 00457E43 (.data cold 1)
volatile float scene3d_shadow_distance_0078e3c8 = 0x1.18p+5f;          // 0078E3C8 .data | 004506BA fcomp (.data 35.0f) (raw 420C0000)
volatile U scene3d_shadow_distance_check_0078e3cc = 0x00000001u;       // 0078E3CC .data | 00450678 (.data cold 1)
volatile float scene3d_flare_size_scale_0078e3e0 = 0x1.99999ap-4f;     // 0078E3E0 .data | 00458562 (.data 0.1f) (raw 3DCCCCCD)
volatile U scene3d_flare_hit_test_0078e3e4 = 0x00000001u;              // 0078E3E4 .data | 0045804D (.data cold 1)
volatile float scene3d_flare_fade_rate_0078e3e8 = 0x1.8p+1f;           // 0078E3E8 .data | 0045815E (.data 3.0f) (raw 40400000)
volatile U scene3d_flare_hit_order_0078e3ec = 0x00000001u;             // 0078E3EC .data | 004580C8 (.data cold 1)
volatile U scene3d_lens_flares_0078e3f0 = 0x00000001u;                 // 0078E3F0 .data | 00457EDB (.data cold 1)
volatile U scene3d_render_enabled_0078e3f4 = 0x00000001u;              // 0078E3F4 .data | 00451325 (.data cold 1)
volatile U static_mesh_beams_0078e3d0 = 0x00000001u;                   // 0078E3D0 .data
volatile float projection_min_radius_0078e3d4 = 0x1.4p+2f;             // 0078E3D4 .data (raw 40A00000)
volatile I scene3d_frame_mod3_0078e3f8 = 2;                            // 0078E3F8 .data | 00451317..0045132B (.data cold 2) (raw 00000002)
volatile U scene3d_dynamic_geometry_0078e3fc = 0x00000001u;            // 0078E3FC .data | 004517A1 gate (.data cold 1)
volatile U scene3d_static_geometry_0078e400 = 0x00000001u;             // 0078E400 .data | 00451644 gate (.data cold 1)
volatile float scene3d_tile_step_0078e428 = 0x1.4cccccp+1f;            // 0078E428 .data | cold 2.6f (00456E16; also 00452CC0) (raw 40266666)
Quaternion scene3d_alt_view_orientation_0078e440 = {0x1p+0f, 0.0f, 0.0f, 0.0f}; // 0078E440 .data | 0045CB3C push 78E440h (.data)
volatile I scene3d_stencil_bits_0078e54c = -1;                         // 0078E54C .data | 0045FD00 cache (cold 0xFFFFFFFF) written by glGetIntegerv(0D57) (raw FFFFFFFF)
volatile I scene3d_stencil_clear_0078e550 = -1;                        // 0078E550 .data | 0045FD48 cache (cold 0xFFFFFFFF) (raw FFFFFFFF)
volatile float scene3d_fade_time_0078e5a0 = 0x1p-2f;                   // 0078E5A0 .data | 004783FD (.data 0.25f) (raw 3E800000)
volatile U scene3d_gate_0078e5a8 = 0x00000001u;                        // 0078E5A8 .data | cold 1 (0046B663; also 0060F1E0)
volatile float scene3d_fade_plane_fraction_0078e5bc = 0x1.554c98p-2f;  // 0078E5BC .data | .data 0.3333f (raw 3EAAA64C)
volatile float scene3d_fade_plane_width_0078e5c0 = 0x1p+0f;            // 0078E5C0 .data | .data 1.0f (raw 3F800000)
volatile I scene3d_part_depth_0078e5e0 = 999999;                       // 0078E5E0 .data | 00473F39 recursion budget (.data cold 999999) (raw 000F423F)
volatile U scene3d_transform_enabled_0078e5f0 = 0x00000001u;           // 0078E5F0 .data | 00473EDA / 004784E0 (.data cold 1)
volatile U scene3d_gate_0078e614 = 0x00000001u;                        // 0078E614 .data | 00456364 (.data cold 1)
float scene3d_walkmesh_colors_0078e6a8[96] = {0x1.99999ap-1f, 0x1.99999ap-1f, 0x1.99999ap-1f, 0x1.292924p-1f, 0x1.f1f212p-2f, 0x1.414164p-2f, 0x1.99999ap-1f, 0x1.99999ap-1f, 0.0f, 0x1.f1f1dp-3f, 0x1.2d2d24p-1f, 0x1.0100e6p-3f, 0x1.99999ap-2f, 0x1.99999ap-2f, 0x1.99999ap-2f, 0x1.313122p-1f, 0x1.65658p-2f, 0x1.79799ep-3f, 0.0f, 0x1.7d7d7cp-1f, 0x1.63637p-1f, 0x1.99999ap-1f, 0.0f, 0.0f, 0x1.fdfdfp-1f, 0x1.99999ap-1f, 0x1.7d7d7cp-1f, 0x1.050508p-1f, 0.0f, 0x1.010108p-1f, 0x1.3f3f32p-1f, 0x1.73736cp-1f, 0x1.73736cp-1f, 0x1.010108p-1f, 0x1.fdfdfp-1f, 0x1.99999ap-1f, 0x1.131316p-1f, 0x1.3b3b32p-1f, 0.0f, 0x1.252546p-2f, 0x1.d1d14ep-4f, 0.0f, 0x1.1d1d04p-2f, 0x1.6d6d7ep-2f, 0x1.191926p-3f, 0x1.99999ap-1f, 0x1.8d8d7ap-2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x1.81819p-4f, 0x1.99999ap-1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // 0078E6A8 .data | .data float[32][3]
volatile U scene3d_saber_simple_0078e828 = 0x00000001u;                // 0078E828 .data | .data 1 (00471988: plain strips while history < 2)
volatile U scene3d_saber_glow_0078e834 = 0x00000001u;                  // 0078E834 .data | .data 1 (00472819: second, extrapolated pass)
volatile float scene3d_saber_step_0078e838 = 0x1.0624dep-10f;          // 0078E838 .data | .data 0.001f (raw 3A83126F)
volatile float scene3d_saber_min_ratio_0078e83c = 0x1p-1f;             // 0078E83C .data | .data 0.5f (raw 3F000000)
volatile U scene3d_saber_enabled_0078e840 = 0x00000001u;               // 0078E840 .data | .data 1 (00471866)
volatile float scene3d_fade_minimum_0078e858 = 0x1p-2f;                // 0078E858 .data | 0047843A (.data 0.25f) (raw 3E800000)
volatile U scene3d_render_flagged_0078e85c = 0x00000001u;              // 0078E85C .data | 00478292 (.data cold 1)
volatile U scene3d_render_enabled_0078e860 = 0x00000001u;              // 0078E860 .data | 0047826E (.data cold 1)
Vector scene3d_part_draw_vector_0078e8ac = {0x1p+0f, 0x1p+0f, 0x1p+0f}; // 0078E8AC .data | 00473EB7 saved/restored per part (.data)
volatile float scene3d_saber_max_accum_00798b30 = 0x1.89374cp-7f;      // 00798B30 .data | .data 0.012f (raw 3C449BA6)
volatile U scene3d_quaternion_shortcut_007a1980 = 0x00000001u;         // 007A1980 .data | 004A9BB0
void* volatile scene3d_distortion_texture_007a68d8 = nullptr;          // 007A68D8 BSS | 00451940 = scene+E8 (BSS)
volatile I64 scene3d_render_start_007b9280 = 0;                        // 007B9280 BSS | 004512E9 / 004512F5
volatile I64 scene3d_animate_end_007b9288 = 0;                         // 007B9288 BSS | 00451D3E / 00451D4F
volatile I64 scene3d_render_end_007b9290 = 0;                          // 007B9290 BSS | 004514A9 / 004514B8
volatile I64 scene3d_animate_start_007b9298 = 0;                       // 007B9298 BSS | 00451C8A / 00451C96
void* volatile scene3d_active_scene_007b92a0 = nullptr;                // 007B92A0 BSS | 00451C9C / 00451D29 / 0045138A / 0045140F
volatile float scene3d_pass_ms_007b92b0 = 0.0f;                        // 007B92B0 BSS | 00450CF9 (raw 00000000)
volatile float scene3d_pass_ms_007b92b4 = 0.0f;                        // 007B92B4 BSS | 00450D3F (raw 00000000)
volatile float scene3d_pass_ms_007b92b8 = 0.0f;                        // 007B92B8 BSS | 00451703 / 004518AF (accumulated) (raw 00000000)
volatile float scene3d_pass_ms_007b92bc = 0.0f;                        // 007B92BC BSS | 00451791 (raw 00000000)
volatile float scene3d_pass_ms_007b92c0 = 0.0f;                        // 007B92C0 BSS | 0045182B (raw 00000000)
volatile float scene3d_pass_ms_007b92c4 = 0.0f;                        // 007B92C4 BSS | 004517E0 (raw 00000000)
volatile float scene3d_pass_ms_007b92c8 = 0.0f;                        // 007B92C8 BSS | 00451904 (raw 00000000)
volatile float scene3d_pass_ms_007b92cc = 0.0f;                        // 007B92CC BSS | 00451A29 (raw 00000000)
volatile float scene3d_animate_ms_007b92d0 = 0.0f;                     // 007B92D0 BSS | 00451D5C (raw 00000000)
volatile float scene3d_render_ms_007b92d4 = 0.0f;                      // 007B92D4 BSS | 004514C4 (raw 00000000)
volatile I scene3d_focus_counter_007b92d8 = 0;                         // 007B92D8 BSS | 004588EB / 0045890D (raw 00000000)
volatile U scene3d_dynamic_lights_007b92dc = 0x00000000u;              // 007B92DC BSS | 004A3773 GetNearestLights gate (BSS cold 0; no writer: a raw-image search for
volatile U scene3d_counter_007b92e0 = 0x00000000u;                     // 007B92E0 BSS | 00451337 = 0
volatile U scene3d_stencil_pass_007b92e4 = 0x00000000u;                // 007B92E4 BSS | 0045168B / 0045183B gate (BSS)
volatile float scene3d_alpha_ref_007b92e8 = 0.0f;                      // 007B92E8 BSS | 004513D9 glAlphaFunc ref (raw DWORD pushed) (raw 00000000)
volatile I scene3d_gob_bucket_snapshot_007b92ec = 0;                   // 007B92EC BSS | 00457BDA (raw 00000000)
volatile U scene3d_depth_toggle_007b92f0 = 0x00000000u;                // 007B92F0 BSS | 00457C69 / 00457E55 (BSS)
volatile U8 scene3d_interleaved_007b92f4 = 0;                          // 007B92F4 BSS | 00451CA4 byte (AurEnable/DisableInterleavedAnimations 0044F760/0044F770) (byte cell: 0044F760/0044F770 mov byte ptr)
volatile U scene3d_visibility_test_007b92f8 = 0x00000000u;             // 007B92F8 BSS | 004579CE (BSS)
volatile U scene3d_gob_bucket_flag_007b92fc = 0x00000000u;             // 007B92FC BSS | 00457BAD = 0
volatile U scene3d_focus_lights_007b9300 = 0x00000000u;                // 007B9300 BSS | 00457DDA (BSS)
volatile U scene3d_force_fog_007b930c = 0x00000000u;                   // 007B930C BSS | 00450DE0 (BSS)
volatile U scene3d_counter_007b9310 = 0x00000000u;                     // 007B9310 BSS | 0045133D = 0
volatile U scene3d_personal_space_all_007b9314 = 0x00000000u;          // 007B9314 BSS | 00450FD0 (BSS)
volatile U scene3d_render_paths_007b9318 = 0x00000000u;                // 007B9318 BSS | 004510E1 (BSS)
volatile I scene3d_stat_007b9338 = 0;                                  // 007B9338 BSS | 0045146C.. (raw 00000000)
volatile I scene3d_stat_007b933c = 0;                                  // 007B933C BSS (raw 00000000)
volatile I scene3d_stat_007b9340 = 0;                                  // 007B9340 BSS (raw 00000000)
volatile I scene3d_stat_007b9344 = 0;                                  // 007B9344 BSS (raw 00000000)
volatile I scene3d_stat_007b9348 = 0;                                  // 007B9348 BSS (raw 00000000)
void* volatile scene3d_render_scene_007b935c = nullptr;                // 007B935C BSS | 004512DE
CHitInfoImage scene_hit_info_007b93a8 = kHitInfoConstructed;           // 007B93A8 BSS | static CHitInfo used when HitCheck gets null (00456B73) | original static initializer 0073AEF0 (CHitInfo::CHitInfo 004578F0(0, BFFFFFFFh, 0) + reset 00456A60; atexit 00456B30) (D60-style: zero image == constructed state; dispose not registered)
volatile I64 scene3d_timer_frequency_007b9410 = 0;                     // 007B9410 BSS | 0044F353/0044F359
volatile U scene3d_timer_guard_007b9418 = 0x00000000u;                 // 007B9418 BSS | 0044F320 bit 0 = frequency cached
Quaternion scene3d_cutscene_orientation_007b9424 = {0.0f, 0.0f, 0.0f, 0.0f}; // 007B9424 BSS | 00457D50
Vector scene3d_cutscene_position_007b9434 = {0.0f, 0.0f, 0.0f};        // 007B9434 BSS | 00457D50
volatile U scene3d_cutscene_guard_007b9440 = 0x00000000u;              // 007B9440 BSS | 00457CE6 static-init guard bits
U scene3d_flare_pool_b_007b9444 = 0x00000000u;                         // 007B9444 BSS | 004585B5 (BSS) (non-volatile: consumer view is U*)
Vector scene3d_flare_hit_vector_007b9448 = {0.0f, 0.0f, 0.0f};         // 007B9448 BSS | 004580B6 static Vector
U scene3d_flare_pool_a_007b9454 = 0x00000000u;                         // 007B9454 BSS | 00457F40 (BSS) (non-volatile: consumer view is U*)
volatile U scene3d_flare_guard_007b9458 = 0x00000000u;                 // 007B9458 BSS | static-init guard bits 1/2/4 (BSS)
volatile float scene3d_plane_distance_007bb4e4 = 0.0f;                 // 007BB4E4 BSS | 0045CBF8 fstp [007BB4E4] (raw 00000000)
void* volatile scene3d_current_camera_007bb4e8 = nullptr;              // 007BB4E8 BSS | 0045CC0F = camera, 0045CC3B = 0
volatile I scene3d_plane_index_007bb4ec = 0;                           // 007BB4EC BSS | 0045CBC4 mov eax,[007BB4EC]; shl eax,4 (raw 00000000)
volatile U scene3d_wireframe_007bb4f0 = 0x00000000u;                   // 007BB4F0 BSS | 0045C55A test; sete -> polygon-mode table index
volatile U scene3d_alt_view_007bb4f4 = 0x00000000u;                    // 007BB4F4 BSS | 0045CB33 nonzero -> SetViewTransform(7BB504,78E440)
Vector scene3d_plane_normal_007bb4f8 = {0.0f, 0.0f, 0.0f};             // 007BB4F8 BSS | 0045CBD6..0045CBEE 3 DWORD copies (7BB4F8/7BB4FC/7BB500)
Vector scene3d_alt_view_position_007bb504 = {0.0f, 0.0f, 0.0f};        // 007BB504 BSS | 0045CB41 push 7BB504h (BSS)
volatile I scene3d_bins_collected_007fbf28 = 0;                        // 007FBF28 BSS | ClearBuckets = 0 (0046BF93); ProcessParts ++ (raw 00000000)
volatile I scene3d_bins_visited_007fbf2c = 0;                          // 007FBF2C BSS | ClearBuckets = 0 (0046BF99); CollectBins ++ (raw 00000000)
volatile U scene3d_model_7a8a68_copy_007fbf30 = 0x00000000u;           // 007FBF30 BSS | ClearBuckets = [007A8A68] (0046BF9F)
volatile I scene3d_stat_007fbf38 = 0;                                  // 007FBF38 BSS | ManageSceneBSP = 0 (0046E85F); ProcessParts ++ (raw 00000000)
volatile I scene3d_stat_007fbf3c = 0;                                  // 007FBF3C BSS | ManageSceneBSP = 0 (0046E865); ProcessParts ++ (raw 00000000)
volatile I scene3d_stat_007fbf44 = 0;                                  // 007FBF44 BSS | ManageSceneBSP = 0 (0046E871); ProcessParts ++ (raw 00000000)
void* volatile scene3d_hit_info_007fbf48 = nullptr;                    // 007FBF48 BSS | 00456B83 / 00456CF3 (lane-3d-bsp reads it as hit_mask_007fbf48)
volatile U scene3d_hit_trimeshes_007fbf4c = 0x00000000u;               // 007FBF4C BSS | 0046B5A0 gate (footstep / SetPosition writers)
volatile U scene3d_draw_aabb_007fbf5c = 0x00000000u;                   // 007FBF5C BSS | 00471578 (BSS)
volatile float scene3d_fade_plane_offset_007fbf64 = 0.0f;              // 007FBF64 BSS | BSS (raw 00000000)
PointerList scene3d_beam_bucket_007fbf6c = {0u, 0, 0};                 // 007FBF6C BSS | size 7FBF70 cleared (0046BF59) | atexit dispose 0073BA70 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_emitter_bucket_007fbf78 = {0u, 0, 0};              // 007FBF78 BSS | size 7FBF7C cleared (0046BF12) | atexit dispose 0073BAB0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_static_shadow_bucket_007fbf84 = {0u, 0, 0};        // 007FBF84 BSS | size 7FBF88 cleared (0046BF53) | atexit dispose 0073BA50 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_blur_bucket_007fbf90 = {0u, 0, 0};                 // 007FBF90 BSS | 00478552 CExoArrayList<int>::Add target | atexit dispose 0073BB90 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_active_bins_007fbf9c = {0u, 0, 0};                 // 007FBF9C BSS | 7FBFA0 (0046BBC0 / 0046BF8D); CollectBins adds | atexit dispose 0073BC10 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_fading_out_bucket_007fbfa8 = {0u, 0, 0};           // 007FBFA8 BSS | size 7FBFAC cleared (0046BF77) | atexit dispose 0073BBD0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_mesh_shadow_bucket_007fbfb4 = {0u, 0, 0};          // 007FBFB4 BSS | size 7FBFB8 cleared (0046BF5F) | atexit dispose 0073BA90 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_distortion_bucket_007fbfc0 = {0u, 0, 0};           // 007FBFC0 BSS | 7FBFC0..C8 (004503F1 / 0046EA4A); RenderSinglePass reads size | atexit dispose 0073BAF0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_gob_bucket_007fbfcc = {0u, 0, 0};                  // 007FBFCC BSS | 00457BAC qsort base / 00457C20 (BSS) | atexit dispose 0073BAD0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_fading_in_bucket_007fbfd8 = {0u, 0, 0};            // 007FBFD8 BSS | size 7FBFDC cleared (0046BF7D) | atexit dispose 0073BBB0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_static_light_bucket_007fbfe4 = {0u, 0, 0};         // 007FBFE4 BSS | 0046CAA0 AddUnique target / 0046DDA0..0046DDC0 (BSS {data,size,capacity}; | atexit dispose 0073BB30 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_light_saber_bucket_007fbff0 = {0u, 0, 0};          // 007FBFF0 BSS | 00450450 (data 7FBFF0, size 7FBFF4) | atexit dispose 0073BB10 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_old_active_bins_007fbffc = {0u, 0, 0};             // 007FBFFC BSS | 7FBFFC..C004 (0046BF83 operator=; 0046BBE8) | atexit dispose 0073BBF0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_cap_buckets_007fc008[5000] = {};                   // 007FC008 BSS | PointerList[5000] (0046BF1A sizes); DoCapBucket reads | original __xc 0073AF90: eh vector ctor 006FB55F(7FC008, 0Ch, 1388h, 005C5A30 zero-ctor); atexit 0073BA20 (D60-style: zero image == constructed state; dispose not registered)
PointerList scene3d_bin_bucket_0080aa68 = {0u, 0, 0};                  // 0080AA68 BSS | 0045021B (BSS) | atexit dispose 0073BB70 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_trigger_bucket_0080aa74 = {0u, 0, 0};              // 0080AA74 BSS | data 80AA74 / size 80AA78 | atexit dispose 0073BB50 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
PointerList scene3d_fade_buckets_0080aa80[5000] = {};                  // 0080AA80 BSS | PointerList[5000] (0046BF37 sizes); DoFadeBuckets reads | original __xc 0073AF60: eh vector ctor 006FB55F(80AA80, 0Ch, 1388h, 005C5A30); atexit 0073BA00 (D60-style: zero image == constructed state; dispose not registered)
PointerList scene3d_mesh_buckets_008194e0[5000] = {};                  // 008194E0 BSS | 004557BD array of {data,size,capacity}, indexed by texture ordering | original __xc 0073AF30: eh vector ctor 006FB55F(8194E0, 0Ch, 1388h, 005C5A30); atexit 0073BC30 (D60-style: zero image == constructed state; dispose not registered)
PointerList scene3d_part_list_00827f40 = {0u, 0, 0};                   // 00827F40 BSS | 0045573C (data 827F40, size 827F44) | original __xc 0073AFC0: 005C5A30 zero-ctor; atexit 0073BA40 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
Vector scene3d_fade_plane_up_00827f68 = {0.0f, 0.0f, 0.0f};            // 00827F68 BSS | static Vector (0,0,1)
volatile U scene3d_fade_plane_guard_00827f74 = 0x00000000u;            // 00827F74 BSS | static-init guard (BSS)
volatile U scene3d_render_all_00828024 = 0x00000000u;                  // 00828024 BSS | 004782F7 (BSS)
volatile U scene3d_saber_additive_00828028 = 0x00000000u;              // 00828028 BSS | BSS (00471908: GL_ONE/GL_ONE, no depth/stencil writes)
volatile U scene3d_saber_strip_count_00828030 = 0x00000000u;           // 00828030 BSS | BSS (strip counter, reset 00471D57)
volatile U scene3d_saber_wireframe_00828034 = 0x00000000u;             // 00828034 BSS | BSS (0047195A: polygon mode LINE when set)
volatile U scene3d_saber_lit_00828038 = 0x00000000u;                   // 00828038 BSS | BSS (0047192E: lighting left on when set)
volatile U scene3d_debug_frames_0082805c = 0x00000000u;                // 0082805C BSS | 004785DF (BSS)
volatile I scene3d_debug_spheres_00828060 = 0;                         // 00828060 BSS | 00478599 cmp ...,1 (BSS) (raw 00000000)
volatile U scene3d_fade_test_0082806c = 0x00000000u;                   // 0082806C BSS | 00478340 (BSS)
Quaternion scene3d_transform_orientation_00828080 = {0.0f, 0.0f, 0.0f, 0.0f}; // 00828080 BSS | 0046F13E
Vector scene3d_transform_position_00828090 = {0.0f, 0.0f, 0.0f};       // 00828090 BSS | 0046F118
volatile U scene3d_transform_guard_0082809c = 0x00000000u;             // 0082809C BSS | 0046F0AF static-init guard bits
float scene3d_transform_matrix_008280a0[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // 008280A0 BSS | 0046F093 static float[16]
U* scene3d_walkmesh_table_008280e0 = nullptr;                          // 008280E0 BSS
volatile U scene3d_walkmesh_guard_008280e4 = 0x00000000u;              // 008280E4 BSS
U* scene3d_walkmesh_inverse_table_008280e8 = nullptr;                  // 008280E8 BSS
volatile U scene3d_walkmesh_inverse_guard_008280ec = 0x00000000u;      // 008280EC BSS
U* scene3d_aabb_inverse_colors_00828110 = nullptr;                     // 00828110 BSS
U* scene3d_aabb_colors_00828114 = nullptr;                             // 00828114 BSS
volatile U scene3d_aabb_guard_00828118 = 0x00000000u;                  // 00828118 BSS
volatile float scene3d_trigger_alpha_008304b8 = 0.0f;                  // 008304B8 BSS | 0049DA43 (raw 00000000)
volatile U scene_black_fog_008305e0 = 0x00000000u;                     // 008305E0 BSS | 00450E30 / 004501A3 (BSS)
PointerList scene3d_grass_info_00831e44 = {0u, 0, 0};                  // 00831E44 BSS | 004A6853 (0x54-byte records) | atexit dispose 0073BEB0 (D60-style: zero image == constructed state; dispose not registered) {data,size,capacity}
Quaternion scene3d_rotation_arc_00831f64 = {0.0f, 0.0f, 0.0f, 0.0f};   // 00831F64 BSS
volatile U scene3d_rotation_arc_guard_00831f74 = 0x00000000u;          // 00831F74 BSS
}
