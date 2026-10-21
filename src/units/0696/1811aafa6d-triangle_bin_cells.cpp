// triangle_bin_cells.cpp - storage for the original globals referenced ONLY by the CAurTriangleBin grass bodies
// (cross-reference over the full .text listing: 007A1964/6C/70/74/78 and 00831E40..00831EE4 are read or written only
// by 004A68B0 / 004A6D30 / 004A87C0 and the 004A6D30 unwind funclet 00718150). Cold values read from the pinned exe
// (.data initialised words; BSS = 0). 007A1968 (900.0f) is shared with ManageSceneBSP and lives in bsp_cells.cpp.
#include "ea549265b3-triangle_bin.h"

extern "C" {
volatile float grass_near_007a1964 = 225.0f;                       // .data 43610000
volatile kotor_scene3d::U grass_wind_enabled_007a196c = 1;         // .data 00000001
volatile kotor_scene3d::U grass_textured_007a1970 = 1;             // .data 00000001
volatile kotor_scene3d::U grass_wind_update_007a1974 = 1;          // .data 00000001
volatile float grass_wind_scale_007a1978 = 1.0f;                   // .data 3F800000
volatile kotor_scene3d::U grass_no_fade_00831e40 = 0;              // BSS
kotor_scene3d::Vector grass_point_00831e50 = {0.0f, 0.0f, 0.0f};    // BSS function statics of 004A68B0
kotor_scene3d::Vector grass_point_wind_00831e5c = {0.0f, 0.0f, 0.0f};
kotor_scene3d::Vector grass_global_wind_00831e68 = {0.0f, 0.0f, 0.0f};
kotor_scene3d::Vector grass_wind_sum_00831e74 = {0.0f, 0.0f, 0.0f};
kotor_scene3d::Vector grass_offset_00831e80 = {0.0f, 0.0f, 0.0f};
volatile kotor_scene3d::U grass_wind_guard_00831e8c = 0;
kotor_vertex_program::VertexProgram* volatile grass_vertex_program_00831e90 = nullptr;  // BSS static of 004A6D30
volatile kotor_scene3d::U grass_program_guard_00831e94 = 0;
kotor_scene3d::Vector grass_corner2_00831ec0 = {0.0f, 0.0f, 0.0f};  // BSS function statics of 004A87C0
kotor_scene3d::Vector grass_corner1_00831ecc = {0.0f, 0.0f, 0.0f};
kotor_scene3d::Vector grass_corner0_00831ed8 = {0.0f, 0.0f, 0.0f};
volatile kotor_scene3d::U grass_corner_guard_00831ee4 = 0;
}
