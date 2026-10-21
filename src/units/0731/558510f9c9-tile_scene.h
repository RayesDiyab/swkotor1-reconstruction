#pragma once
// lane-3d-bsp fork B: tiles, room selection, scene distortion/blur/trigger buckets and the grass-bin list helpers
// (bodies: tile_scene.cpp). Includes the area header bsp.h (which includes the canonical scene3d.h + vtables_gen.h).
// No additional borrowed cell is needed beyond kotor_scene3d_bsp::BspCells and the bsp.h area cells, so this unit
// adds no TileCells view and no storage unit.
#include <cstdio>
#include "11b317539a-bsp.h"

namespace kotor_scene3d_bsp {
// Part family table +64 (Draw, RET 4: lane-3d-mesh spelling `U __fastcall(void*, void*, U)`), used on trigger parts
// by Scene::DoTriggerBucket (00450B12..00450B16).
struct PartDrawSlot {
    const void* o00[0x64 / 4];
    U (__fastcall* draw)(void* self, void*, U flag);                                       // +64
};
static_assert(offsetof(PartDrawSlot, draw) == 0x64, "Part draw slot");
}

// ---------------------------------------------------------------- providers with existing spellings (exact copies)
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// lane-3d-room room_spawn.cpp declarations (lane-3d-model providers / core lowers / strict link)
aurora_model::MaxTree* __cdecl aurora_find_model_00464110(const char* name);
void __cdecl aurora_part_init_material_00442d70(aurora_model::Gob* gob, aurora_model::Part* part, const char* prefix);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
void __cdecl aur_texture_set_mode_static_004204b0();
void __cdecl aur_texture_set_mode_dynamic_004204c0();
void* __cdecl find_node_00452000(aurora_model::MdlNode* node, const char* name);
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b);
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b);
aurora_model::Part* __cdecl aurora_create_instance_tree_00449410(aurora_model::MdlNode* node, aurora_model::Gob* gob);
void __cdecl aurora_collapse_part_tree_00453f10(aurora_model::Gob* gob, aurora_model::Part* tree, PointerList* parts,
    Vector position, Quaternion orientation, void* scene, void* room);
// lane-3d-construct construct3d.h (lights area)
void __fastcall gob_collect_lights_00447180(void* gob, void*, kotor_reconstruction::ArrayMutations* out);   // RET 4
void __cdecl update_static_lighting_0046ca30(void* area, void* light);

// ---------------------------------------------------------------- lowers with no provider (names proposed here)
// [core] 004ACAC0 YawPitchRoll(out, yaw_degrees, pitch, roll): cdecl, EAX not consumed (00454CB7)
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion* out, float yaw, float pitch, float roll);
// [core] 0044FAC0 ProcessStaticObject(gob, scene, tile, position*, orientation*): cdecl (cleanup 14h at 00454D52)
void __cdecl process_static_object_0044fac0(void* gob, void* scene, void* tile, const Vector* position,
                                            const Quaternion* orientation);
// [core] 00454B80 ProcessStaticParts(parts*, scene, tile, name, flag): cdecl (cleanup 14h at 00454D6E)
void __cdecl process_static_parts_00454b80(PointerList* parts, void* scene, void* tile, const char* name, U flag);
// [core] 00452500 SetPartTextureRotate(tree, rotation): cdecl
void __cdecl set_part_texture_rotate_00452500(aurora_model::Part* tree, I rotation);
// [handoff lane-3d-model] 00454470 ExpandReferenceParts(tree): cdecl
void __cdecl expand_reference_parts_00454470(aurora_model::Part* tree);
}
