#pragma once
// lane-m46-3dcore fork C2: callees of the Scene bucket/shadow/face bodies that no stage header (reachable from the
// bsp include profile) declares. Exact original ABIs (D1: __thiscall -> __fastcall(self, unused_edx, stack...)).
// Each entry: original address, RET, and the existing provider (if any) whose symbol this declaration matches.
#include "11b317539a-bsp.h"

namespace aurora_model { struct MdlNode; }

extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::Plane; using kotor_scene3d::PointerList;

// ---- existing providers / declarations copied exactly from their owners' headers
// lane-3d-model model_load.hpp (M45 object 3d-model-model-binary defines @aurora_node_as_trimesh_0043e400@8)
aurora_model::MdlNode* __fastcall aurora_node_as_trimesh_0043e400(aurora_model::MdlNode* self, void* unused_edx);
// lane-3d-construct construct3d.h (fork E of this lane authors the body)
void __cdecl project_dynamic_shadow_0044ff50(void* gob, void* light);
// lane-3d-emitter emitter.h (CExoArrayList::AddUnique 0046BD10, RET 4; join to texture_join::add_unique_0046bd10)
void __fastcall exo_array_list_add_unique_0046bd10(PointerList* list, void* unused_edx, U value);

// ---- no provider yet (new undefined externals)
// 004A4B10 LightManager::GetShadowLights(int mode): RET 4, returns the address of a CExoArrayList (004A4F08).
PointerList* __fastcall light_manager_get_shadow_lights_004a4b10(void* lights, void* unused_edx, U mode);
// 004AB4C0 Plane::Transform(const Vector* translation, const Quaternion* rotation): RET 8 (004AB62C); EAX unused.
void __fastcall plane_transform_004ab4c0(Plane* plane, void* unused_edx, const Vector* translation,
    const Quaternion* rotation);
// 00471450 SetBeaming(): cdecl, no arguments, plain RET (004714A8).
void __cdecl set_beaming_00471450();
// 004A96E0 Round(float value, float precision): cdecl, result in ST0 = floor(value/precision + 0.5f) * precision.
double __cdecl round_004a96e0(float value, float precision);
// 004A82B0 CAurTriangleBin::CAurTriangleBin(): RET 0, EAX = this (004A8336).
void* __fastcall triangle_bin_construct_004a82b0(void* self, void* unused_edx);
// 004A7860 CAurTriangleBin::AddTriangle(face, part, trimesh node): RET 0Ch (004A81A4).
void __fastcall triangle_bin_add_triangle_004a7860(void* bin, void* unused_edx, void* face, void* part, void* node);

// ---- original globals not (yet) in kotor_scene3d::Cells / BspCells (header diff: parts/C2-scene3d.h.diff)
extern volatile U static_mesh_beams_0078e3d0;          // .data cold 1 (00450970 gate)
extern volatile float projection_min_radius_0078e3d4;  // .data cold 5.0f (00453C6B / 00453C78)
}
