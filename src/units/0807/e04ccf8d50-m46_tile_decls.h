#pragma once
// lane-m46-3dcore fork E: exported declarations of the tile / part / shadow helpers authored in m46_tile.cpp.
// Exact consumer ABIs (M45 stage): tile_scene.h (find_node, set_part_texture_rotate, process_static_object,
// process_static_parts), room_spawn.cpp:53/55 (find_node, compute_part_bbox), construct3d.h:194
// (project_dynamic_shadow). round_004a96e0 / animate_static_light_0046ddf0 are new names (callees of the above).
#include <cstdint>

namespace aurora_model { struct MdlNode; struct Part; }
namespace kotor_scene3d { struct Vector; struct Quaternion; }
namespace kotor_reconstruction { struct ArrayMutations; }

extern "C" {
void* __cdecl find_node_00452000(aurora_model::MdlNode* node, const char* name);
void __cdecl set_part_texture_rotate_00452500(aurora_model::Part* tree, std::int32_t rotation);
void __cdecl compute_part_bbox_00451dc0(void* part);
void __cdecl process_static_object_0044fac0(void* gob, void* scene, void* tile, const kotor_scene3d::Vector* position,
                                            const kotor_scene3d::Quaternion* orientation);
void __cdecl process_static_parts_00454b80(kotor_reconstruction::ArrayMutations* parts, void* scene, void* tile,
                                           const char* name, std::uint32_t flag);
void __cdecl project_dynamic_shadow_0044ff50(void* gob, void* light);
// 004A96E0 Round(value, precision): cdecl, result in ST0 (double).
double __cdecl round_004a96e0(float value, float precision);
// 0046DDF0 AnimateStaticLight: Gob event callback (cdecl gob, event, part), registered by 0044FB21.
void __cdecl animate_static_light_0046ddf0(void* gob, void* event, aurora_model::Part* part);

// cell defined in m46_tile.cpp (no owner anywhere: build/E_xref.py, readers 0044FF51 and 00474345..00474BAD)
extern volatile std::uint32_t project_shadow_gate_007b9374;   // BSS 0
}
