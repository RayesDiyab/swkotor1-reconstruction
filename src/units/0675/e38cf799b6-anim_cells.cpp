// anim_cells.cpp - lane-3d-anim: storage for the original globals owned by the animation sub-area (anim3d.h).
// .data values are read from the pinned GOG swkotor.exe (sha256 9c10e045...); no .text instruction writes them.
// BSS cells start at zero, as in the image.
#include "3775ec35d8-anim3d.h"

extern "C" {
volatile kotor_scene3d::I anim_max_depth_00798b18 = 1000;            // .data 000003E8
volatile float anim_lod_near_00798b1c = 12.0f;                      // .data 41400000
volatile float anim_lod_far_00798b20 = 30.0f;                       // .data 41F00000
volatile kotor_scene3d::I anim_lod_far_depth_00798b24 = 8;          // .data 00000008
volatile kotor_scene3d::I anim_lod_near_depth_00798b28 = 12;        // .data 0000000C
volatile kotor_scene3d::U anim_lod_enabled_00798b2c = 1;            // .data 00000001
volatile kotor_scene3d::U anim_slot188_enabled_00798b3c = 1;        // .data 00000001
volatile kotor_scene3d::U anim_enabled_00798b40 = 1;                // .data 00000001
volatile kotor_scene3d::U anim_lod_parts_00798b44 = 1;              // .data 00000001
void* volatile anim_report_run_008302cc = nullptr;
volatile kotor_scene3d::U anim_report_reverse_008302c8 = 0;
volatile float anim_report_length_008302d0 = 0.0f;
volatile kotor_scene3d::I anim_depth_008302d4 = 0;
volatile kotor_scene3d::I anim_max_seen_depth_008302d8 = 0;
volatile kotor_scene3d::U anim_debug_008302e0 = 0;
volatile kotor_scene3d::U anim_focus_override_008302e4 = 0;
volatile float anim_last_blend_008302e8 = 0.0f;
float anim_vector_key_b_008302ec[3] = {0.0f, 0.0f, 0.0f};
float anim_vector_key_a_008302f8[3] = {0.0f, 0.0f, 0.0f};
volatile kotor_scene3d::U anim_vector_guard_00830304 = 0;
kotor_scene3d::Quaternion anim_quaternion_key_b_00830308 = {0.0f, 0.0f, 0.0f, 0.0f};
kotor_scene3d::Quaternion anim_quaternion_key_a_00830318 = {0.0f, 0.0f, 0.0f, 0.0f};
volatile kotor_scene3d::U anim_quaternion_guard_00830328 = 0;
float anim_control_previous_0083032c = 0.0f;
kotor_scene3d::U anim_control_current_00830330 = 0;
float anim_control_value_00830334 = 0.0f;
kotor_scene3d::U skin_normal_pool_008303a8 = 0;
kotor_scene3d::U skin_vertex_pool_008303ac = 0;
volatile kotor_scene3d::U skin_pool_vertices_008303b0 = 0;
volatile kotor_scene3d::U skin_guard_008303b4 = 0;
}
