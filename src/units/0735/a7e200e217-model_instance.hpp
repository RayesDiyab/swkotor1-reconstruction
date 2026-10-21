// model_instance.hpp - lane-3d-model: model instance creation (CSWGuiScene::AddModel -> NewCAurObject ->
// Gob::Gob -> CreateInstanceTreeR -> per-node InternalCreateInstance -> Part constructors).
// Objects are raw observed-offset storage. C++ virtual tables of Gob/Part/Scene objects are owned by
// their class owners (render/scene lanes); this lane stores their addresses and dispatches through the
// observed slots only, typed as __fastcall(self, unused_edx, ...) (decision D1).
#pragma once
#include "5d11c14380-model_load.hpp"
#include "7ee14b5ec3-model_vtables.hpp"
#include "6c81d8874d-scene3d.h"   // lane-3d-render canonical header (include root lane-3d-render/source)

namespace aurora_model {
struct Gob;       // CAurObject (0x1CC bytes, 00449DDE)
struct Part;      // Part family (0x4C..0x3CC bytes)
struct Scene;     // CAurScene
struct GuiScene;  // CSWGuiScene (+14 CAurScene*, +1C CExoArrayList<Gob*>)

// Gob vtable 00741268: canonical complete type aurora_model::GobVTable and its definition are owned by
// lane-3d-render (vtables_gen.h / vtables.cpp, D42). Event callback type as typed there: cdecl(gob, event, part).
using GobEventCallback = void (__cdecl*)(void* gob, void* event, Part* arg);

// Consumer view of the Part-family tables (model_vtables.hpp): the member types at these offsets are the
// exact slot types of every table that this lane dispatches through (Slot0 = RET0, Slot1 = RET4).
struct PartVTable {
    U slot00;                         // 00 deleting destructor
    aurora_vt::Slot0 name;            // 04 (Part::Name 00440200, RET0; 004448F4)
    U slots08[(0x24 - 0x08) / 4];
    aurora_vt::Slot0 query24;         // 24 (emitter query; 00453569 UnlistSpecialParts)
    U slot28;                         // 28
    aurora_vt::Slot0 as_trimesh;      // 2C (00442D91, 0047B780, 00478B6C)
    aurora_vt::Slot0 query30;         // 30 (00442E3A)
    aurora_vt::Slot0 query34;         // 34 (0047B789)
    aurora_vt::Slot0 query38;         // 38 (reference query; 004448CF)
    U slots3c[(0x4C - 0x3C) / 4];
    aurora_vt::Slot0 query4c;         // 4C (00442E69)
    aurora_vt::Slot0 query50;         // 50 (00442DFD / 00442E26)
    U slots54[(0x6C - 0x54) / 4];
    aurora_vt::Slot1 animate;         // 6C (Part::Animate 00440110 / PartSkin::PostProcess 00489340, RET4)
    U slot70;                         // 70
    aurora_vt::Slot3 bounding_box;    // 74 (Part::GetMinimumBoundingBox 00440960, RET0Ch; 0043EF5D)
    U slot78;                         // 78
    aurora_vt::Slot0 initialize;      // 7C (emitter tables: 0049CAA0 / 0049D480, RET0)
    U slot80;                         // 80
    aurora_vt::Slot0 query84;         // 84 (emitter tables: 0063E7F0 / 00641DB0; 0049CC5F)
};
static_assert(offsetof(PartVTable, query24) == 0x24 && offsetof(PartVTable, as_trimesh) == 0x2C && offsetof(PartVTable, query30) == 0x30 &&
              offsetof(PartVTable, query34) == 0x34 && offsetof(PartVTable, query4c) == 0x4C &&
              offsetof(PartVTable, query50) == 0x50 && offsetof(PartVTable, animate) == 0x6C &&
              offsetof(PartVTable, initialize) == 0x7C && offsetof(PartVTable, bounding_box) == 0x74 &&
              offsetof(PartVTable, query84) == 0x84 && offsetof(PartVTable, name) == 4 &&
              offsetof(PartVTable, query38) == 0x38, "Part slots");

}

extern "C" {
// ---------------- cells defined by this lane ----------------
extern aurora_model::List aurora_gobs_007a8a58;                 // sorted Gob list {data, size, capacity}
extern volatile aurora_model::I aurora_part_count_007a8a10;     // ++ 004457F8
extern volatile aurora_model::I aurora_trimesh_part_count_007a8a14; // ++ 00445911
extern volatile aurora_model::I aurora_gob_count_007a8a18;      // ++ 004495E4..004495ED
extern volatile aurora_model::I aurora_gob_created_007a8a1c;    // ++ 004495CD
extern volatile aurora_model::I aurora_mesh_serial_0078e328;    // .data initial 1; 004458DE..004458E9
extern volatile aurora_model::I aurora_emitter_count_00830470;  // ++ 0049B658..0049B65F
extern volatile aurora_model::U aurora_control_weight_00798b38; // .data initial 3F800000 (1.0f); 0044923A
extern volatile aurora_model::U8 aurora_model_gate_007b8fc0;    // saved/cleared/restored by 00449CC0

// ---------------- providers authored by this lane ----------------
aurora_model::Gob* __fastcall aurora_gui_scene_add_model_name_00417620(aurora_model::GuiScene* self, void* unused_edx, void* name_string, aurora_model::I index);
aurora_model::I __fastcall aurora_gui_scene_add_model_004175c0(aurora_model::GuiScene* self, void* unused_edx, aurora_model::Gob* gob, aurora_model::I index, aurora_model::I attach);
void __fastcall aurora_list_insert_at_004167a0(aurora_model::List* self, void* unused_edx, aurora_model::U value, aurora_model::I index);
void __fastcall aurora_list_add16_00671c00(aurora_model::List* self, void* unused_edx, aurora_model::U value);
void __cdecl aurora_part_init_material_00442d70(aurora_model::Gob* gob, aurora_model::Part* part, const char* prefix);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob_name, std::FILE* mdl, std::FILE* mdx);
aurora_model::Gob* __fastcall gob_construct_00449440(aurora_model::Gob* self, void* unused_edx, const char* name);
void* __fastcall aurora_path_info_construct_0044ee00(void* self, void* unused_edx);
void* __cdecl find_scene_0044fd60(const char* name);   // symbol expected by lane-3d-render
aurora_model::Part* __cdecl find_part_0043fe00(aurora_model::Part* part, const char* name);   // symbol expected by lane-3d-render
aurora_model::I __cdecl aurora_find_gob_index_00440410(const char* name);
aurora_model::I __fastcall aurora_gob_attach_to_scene_0044f7c0(aurora_model::Gob* self, void* unused_edx, aurora_model::Scene* scene);
void __fastcall gob_set_scene_0043f200(void* gob, void* unused_edx, void* scene);   // authored by lane-3d-render (scene3d.h)
aurora_model::Part* __cdecl aurora_create_instance_tree_00449200(aurora_model::MdlNode* node, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_create_instance_00448a70(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_header_00447b20(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_light_00447b90(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_camera_00447c80(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_reference_00447ce0(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_trimesh_00447d60(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_anim_mesh_00447dd0(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_aabb_00447e30(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_dangly_00447e90(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_saber_004480c0(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_skin_00487510(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::Part* __fastcall aurora_node_instance_emitter_0049d5c0(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Gob* gob);
aurora_model::U __fastcall aurora_return_false_4_0063e7c0(void* self, void* unused_edx, aurora_model::U unused);
void __fastcall aurora_part_list_set_size_00444fa0(aurora_model::List* self, void* unused_edx, aurora_model::I count);
aurora_model::Part* __fastcall aurora_part_construct_004457a0(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_part_light_construct_00445a40(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_part_trimesh_construct_00445840(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_part_dangly_construct_00447980(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_part_saber_construct_004478d0(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_part_emitter_construct_0049b380(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
aurora_model::Part* __fastcall aurora_lightning_emitter_construct_0049d1d0(aurora_model::Part* self, void* unused_edx, aurora_model::MdlNode* node);
void* __fastcall aurora_hit_info_construct_004578f0(void* self, void* unused_edx, aurora_model::U a, aurora_model::U b, aurora_model::U c);
void __fastcall aurora_hit_info_clear_output_00456a60(void* self, void* unused_edx);
void __cdecl aurora_emitter_detonate_0048d770(void* unused1, void* unused2, aurora_model::Part* part);
void __cdecl set_emitter_target_0049b240(void* gob, void* event, aurora_model::Part* part);   // 0049B240 (not authored; address registered by 0049CBAF)
void __fastcall aurora_material_load_textures_0047b770(void* material, void* unused_edx, aurora_model::Part* part);
void* __fastcall aurora_vertex_flat_construct_004792a0(void* self, void* unused_edx, aurora_model::Part* part);
void* __fastcall aurora_vertex_anim_construct_004793a0(void* self, void* unused_edx, aurora_model::Part* part);
void* __fastcall aurora_vertex_dangly_construct_00479430(void* self, void* unused_edx, aurora_model::Part* part);
void* __fastcall aurora_vertex_skinned_construct_004794c0(void* self, void* unused_edx, aurora_model::Part* part);
aurora_model::U __fastcall aurora_vertex_flat_build_arrays_00478b50(void* self, void* unused_edx);
aurora_model::U8 __fastcall aurora_trimesh_has_tangent_bases_0043e410(aurora_model::MdlNode* self, void* unused_edx);

// ---------------- imports ----------------
char* __fastcall async_string_cstr_005e5670(void* self, void* carrier);   // M41 CExoString::CStr
// class tables owned by the Gob/Part/Scene owners (render/scene lanes; not in M41): addresses stored only
// Part/emitter/PathInfo/LightContainer/VertexPrimitive tables: model_vtables.hpp (this lane, D39).
// instance-side workers not authored here (sized in PACKET.json remaining)
void __fastcall aurora_node_process_skin_seams_00438fa0(aurora_model::MdlNode* self, void* unused_edx);
void __cdecl aurora_create_reference_objects_004448c0(aurora_model::Gob* gob, aurora_model::Part* part);
aurora_model::U __fastcall aurora_node_control_dispatch_0043f890(aurora_model::MdlNode* self, void* unused_edx, aurora_model::Part* part, aurora_model::U raw_time, aurora_model::U raw_a, aurora_model::U raw_weight);
void* __fastcall aurora_material_construct_0047b290(void* self, void* unused_edx);   // join to material_owner::Material::Material (model_material.cpp)
}
