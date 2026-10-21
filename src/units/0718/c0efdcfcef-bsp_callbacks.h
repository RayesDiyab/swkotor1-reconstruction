#pragma once
// lane-3d-bsp: BSP visibility callbacks (gate G1; bodies bsp_callbacks.cpp). PROPOSED for append to scene3d.h.
#include "ea549265b3-triangle_bin.h"

namespace kotor_scene3d_bsp {
// Part table +4C query (0046BDF9; lane-3d-model PartVTable::query4c).
struct PartQuerySlots {
    const void* o00[0x4C / 4];
    void* (__fastcall* query_4c)(void* self, void*);                                       // +4C RET 0
};
static_assert(offsetof(PartQuerySlots, query_4c) == 0x4C, "Part +4C");
// Object returned by Part+4C: slot +68 takes the raw frame-time DWORD [0078E574] (0046BE00..0046BE0B, thiscall 1 arg).
struct ControllerSlots {
    const void* o00[0x68 / 4];
    void (__fastcall* update)(void* self, void*, U time_bits);                             // +68 RET 4
};
static_assert(offsetof(ControllerSlots, update) == 0x68, "controller +68");
// Borrowed cells shared with other areas (Main publishes once).
struct CallbackCells {
    const volatile U* draw_aabb_007fbf5c;          // BSS; also PartAABB::Draw 00471560
    const volatile U* frame_time_0078e574;         // = scene3d cells.frame_time_0078e574 (raw float DWORD)
    const volatile float* fade_time_0078e5a0;      // = scene3d cells.fade_time_0078e5a0 (.data 0.25f)
};
extern const CallbackCells& callback_cells;
}

extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Plane;
using kotor_scene3d::PlaneList;
// area-owned cells (storage bsp_callbacks_cells.cpp; only these callbacks reference them)
extern volatile I bsp_trimesh_plane_cache_00827f4c;     // BSS (0046A6D0 / 0046A71E)
extern volatile I bsp_projection_plane_cache_00827f50;  // BSS (0046A800 / 0046A839)
extern volatile I bsp_trigger_plane_cache_00827f54;     // BSS (0046A870 / 0046A8A9)
extern volatile U bsp_gob_cull_disabled_007fbf20;       // BSS (0046A8F2; no writer instruction)
extern volatile U bsp_ignore_tile_hidden_0078e5ac;      // .data cold 1 (0046D3FD)
// providers with existing spellings
std::uint32_t __fastcall kotor_fastcall_0047af00(void* material, void*);   // lane-gui-render Material::GetTextureTID
// [core] 004A7340 CAurTriangleListElem copy ctor (list part): thiscall RET 4
void* __fastcall triangle_list_elem_copy_construct_004a7340(void* self, void*, const void* source);
// [core] 0046B7C0 ConstructCameraFadePlane, agreed reconstruction convention cdecl(scene)
// (declared in bsp.h); original EAX-entry adapter authored here:
void construct_camera_fade_plane_eax_entry_0046b7c0();
// authored (G1)
void __cdecl aur_clear_active_bins_0046a610();
double __cdecl sphere_above_plane_004a9710(const Vector* center, float radius, const Plane* plane);
void __fastcall cswc_object_set_id_00616fa0(void* self, void*, U value);
I __cdecl part_outside_trimesh_0046a6b0(void* part, const PlaneList* planes);
I __cdecl part_outside_emitter_0046a730(void* part, const PlaneList* planes);
I __cdecl part_outside_projection_0046a7e0(void* part, const PlaneList* planes);
I __cdecl part_outside_trigger_0046a850(void* part, const PlaneList* planes);
I __cdecl gob_outside_0046a8c0(void* gob, const PlaneList* planes);
I __cdecl bin_outside_0046a980(void* bin, const PlaneList* planes);
void __cdecl add_part_to_mesh_buckets_0046bdf0(void* part);
// node_has_* / collect_bins_0046cae0 / process_parts_0046d3b0: bsp.h; grass_sort_tris_004a7690: triangle_bin.h
}
