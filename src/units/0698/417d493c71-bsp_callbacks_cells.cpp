// Storage for cells referenced only by the BSP visibility callbacks (gref.py cross-reference); cold values from the
// pinned exe (.data 0078E5AC = 1; others BSS = 0).
#include "c0efdcfcef-bsp_callbacks.h"
extern "C" {
volatile kotor_scene3d::I bsp_trimesh_plane_cache_00827f4c = 0;
volatile kotor_scene3d::I bsp_projection_plane_cache_00827f50 = 0;
volatile kotor_scene3d::I bsp_trigger_plane_cache_00827f54 = 0;
volatile kotor_scene3d::U bsp_gob_cull_disabled_007fbf20 = 0u;
volatile kotor_scene3d::U bsp_ignore_tile_hidden_0078e5ac = 1u;
}
