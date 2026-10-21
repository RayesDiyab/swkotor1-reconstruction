// bsp_cells.cpp - storage for the original globals referenced ONLY by the bsp_visibility area (and the unassigned
// BSP callbacks 0046CAE0 / 0046D3B0 / 0046B340.. listed in PACKET.json): cross-reference over the full .text
// listing (gref.py) shows no other function, and no .data/.rdata table holds these addresses.
// Cold values read from the pinned exe (.data initialised words; BSS = 0).
#include "11b317539a-bsp.h"

extern "C" {
volatile kotor_scene3d::U bsp_manage_enabled_0078e5cc = 1u;        // .data 00000001
volatile kotor_scene3d::I bsp_forced_room_0078e5d0 = -1;           // .data FFFFFFFF
volatile kotor_scene3d::U bsp_use_current_room_0078e5c8 = 1u;      // .data 00000001
volatile kotor_scene3d::U bsp_first_frame_0078e5d4 = 1u;           // .data 00000001
volatile float bsp_tile_spacing_0078e424 = 10.0f;                  // .data 41200000
volatile float bsp_bin_radius_sq_007a1968 = 900.0f;                // .data 44610000
volatile kotor_scene3d::I bsp_unlit_lights_007fbf18 = 0;
volatile kotor_scene3d::I bsp_stat_007fbf34 = 0;
volatile kotor_scene3d::I bsp_stat_007fbf40 = 0;
volatile kotor_scene3d::U bsp_hit_plane_007fbf50 = 0u;
volatile kotor_scene3d::U bsp_hit_ground_007fbf54 = 0u;
volatile kotor_scene3d::I bsp_hit_calls_007fbf58 = 0;
volatile kotor_scene3d::I bsp_room_index_007fbf68 = 0;
volatile kotor_scene3d::U bsp_camera_far_00827f78 = 0u;
volatile kotor_scene3d::U bsp_camera_near_00827f7c = 0u;
volatile kotor_scene3d::U bsp_camera_orientation_00827f80[4] = {0u, 0u, 0u, 0u};
volatile kotor_scene3d::U bsp_camera_position_00827f90[3] = {0u, 0u, 0u};
volatile kotor_scene3d::U bsp_camera_guard_00827f9c = 0u;
}
