#pragma once
// lane-3d-construct: 3D sub-areas "construction" and "lights" (claude-takeover/lane-3d-render/SUBLANES.json, D35).
// swkotor.exe GOG K1 (image base 00400000). Every body cites the original instructions it reproduces.
//
// Conventions (same as the canonical 3D header scene3d.h, lane-3d-render):
//  * original __thiscall entries are __fastcall(Self*, void* edx_unused, ...) (D1); callers pass nullptr for EDX;
//    no slot below returns a struct by value except through an explicit out pointer (D1 amendment);
//  * objects are byte-offset views (kotor_scene3d::field / at); every offset is cited by an instruction;
//  * virtual calls reload the receiver's vtable at every call through a TYPED slot view (below): the vtable pointer
//    is read as a pointer to a struct of typed __fastcall/__cdecl members (object-pointer view, D24); there is no
//    function-pointer cast anywhere;
//  * x87 intermediates are evaluated in double, every original `fstp dword` rounding is kept as a float rounding
//    (D30 provisional; risks noted per site);
//  * allocator family: 006FA7E6 operator new -> live_operator_new, 006FA390 free -> live_free (M41),
//    CExoArrayList::Allocate 005E03C0 -> menu_list_growth::allocate_005e03c0 (M41).
// This header is self-contained (only <cstdint> + the M41/task-14 list type) so that both the scene3d.h unit and
// the lane-3d-model model_load.hpp unit can include it; it declares only this lane's providers and lowers whose
// declaration does not already exist in scene3d.h / model_load.hpp / font.hpp.
#include <cstdint>
#include <cstring>
#include <cstddef>
#include "459924a746-array_mutations.h"

namespace aurora_model { struct Gob; struct Part; struct MdlNode; }   // lane-3d-model incomplete types (same entities)
namespace kotor_scene3d { struct Vector; }                              // scene3d.h complete type (same entity)

namespace kotor_scene3d_construct {
using U = std::uint32_t;
using I = std::int32_t;
using U8 = std::uint8_t;
using U16 = std::uint16_t;
using List = kotor_reconstruction::ArrayMutations;   // {U data, I size, I capacity}

// ---------------------------------------------------------------- event callback type
// Gob::DoEvent 00486560 calls a registered handler as `push context|arg; push event; push gob; call [entry+20h];
// add esp,0Ch` (0048662B..00486631) and ignores EAX. The register slot type is lane-3d-model's
// aurora_model::GobRegisterCallback (model_instance.hpp: Gob vtbl+28, 00484E40, RET 14h); the handler type below is
// spelled exactly as in that declaration so the handlers of this lane can be passed without any cast.
using EventHandler = void (__cdecl*)(void* gob, void* event, aurora_model::Part* context);

// ---------------------------------------------------------------- typed slot views (receiver vtables)
// Each member is cited by the call site(s) of this lane. Unused slots are opaque words. The class owner of every
// table must define the cited slots with exactly these types (PACKET.json review_needed).
struct Vec3 { float x, y, z; };          // same layout as kotor_scene3d::Vector (12 bytes)
struct Quat { float w, x, y, z; };       // same layout as kotor_scene3d::Quaternion (16 bytes)
static_assert(sizeof(Vec3) == 12 && sizeof(Quat) == 16, "x86 layouts");

// CAurObject/Gob slots: the canonical typed table aurora_model::GobVTable (lane-3d-render vtables_gen.h, D39/D42)
// is used by the scene3d.h unit; its raw-word contract (one U per stack DWORD) is followed at every call.

// Part (tables 00740AE8 family, owner lane-3d-model D39; its aurora_model::PartVTable types +2C identically and does not
// yet name +0C / +24 - proposed additions in PACKET.json proposed_diffs).
struct PartSlots {
    const void* opaque00[3];                                              // +00..+08
    void (__fastcall* get_transform)(aurora_model::Part* self, void*, void* position_out, void* orientation_out); // +0C (0049FBB3, 0049ECE4, 0046AB81, 00450675)
    const void* opaque10[5];                                              // +10..+20
    void* (__fastcall* as_light)(aurora_model::Part* self, void*);        // +24 (004471FF, 0048556C)
    const void* opaque28;                                                 // +28
    aurora_model::Part* (__fastcall* as_trimesh)(aurora_model::Part* self, void*); // +2C (0046DBEA; = aurora_model::PartVTable::as_trimesh)
};
static_assert(offsetof(PartSlots, get_transform) == 0x0C && offsetof(PartSlots, as_light) == 0x24 &&
              offsetof(PartSlots, as_trimesh) == 0x2C, "Part slot offsets");

// CAurBehavior family. Base table 00743864 (8 slots, stored by ~CAurBehavior 0049FA8F / ctor 0049F114) and the four
// CAurBehaviorAttach tables 00743894 / 007438B8 / 007438DC / 00743900 (9 slots: the base 8 + UpdateGobPosition).
struct BehaviorBaseSlots {
    void* (__fastcall* scalar_delete)(void* self, void*, U flags);        // +00 (0049FFD0 / 0049FFF0 / 004A0010, RET 4)
    void (__fastcall* set_scene)(void* self, void*);                      // +04 (004015A0 / 0049F0C0; 0049EB5E tail jump)
    U (__fastcall* control)(void* self, void*, U raw_delta);              // +08 (0049F270 / 0049F3B0.., RET 4)
    void (__fastcall* message)(void* self, void*, char* line);            // +0C (0060E760 / 0049ED00.., RET 4)
    U (__fastcall* get_type)(void* self, void*);                          // +10 (0063E7F0 / 0049FCD0: 3E9h)
    void* (__fastcall* self_return)(void* self, void*);                   // +14 (0063E7F0 base / 00641DB0)
    U (__fastcall* return_zero_18)(void* self, void*);                    // +18 (0063E7F0)
    U (__fastcall* return_zero_1c)(void* self, void*);                    // +1C (0063E7F0)
};
struct BehaviorSlots : BehaviorBaseSlots {
    void (__fastcall* update_gob_position)(void* self, void*);            // +20 (0049ECD0; 0049EBAC tail jump)
};
static_assert(sizeof(BehaviorBaseSlots) == 0x20 && sizeof(BehaviorSlots) == 0x24 &&
              offsetof(BehaviorSlots, update_gob_position) == 0x20, "8 / 9 slots per behavior table");

// Object held at Gob+188 (0049EC78 `push 0; call [edx+8]`; 0049FD2A `call [eax+14h]` -> object whose +14 is compared).
struct RenderTargetSlots {
    const void* opaque00[2];
    void (__fastcall* reset)(void* self, void*, U zero);                  // +08
    const void* opaque0c[2];
    void* (__fastcall* query_14)(void* self, void*);                      // +14
};
static_assert(offsetof(RenderTargetSlots, query_14) == 0x14, "Gob+188 object slot +14");

// LightManager table 00743958 (25 slots, 00743958..007439BB; the next word 358637BDh is data). Published typed table
// (lane-3d-mesh / lane-3d-bsp / core consumers bind to these member types). Float stack words are raw U (D1 raw-word).
struct LightManagerVTable {
    void (__fastcall* prioritize_lights)(void* self, void*, const kotor_scene3d::Vector* position, U flag);   // +00 004A2F30 RET 8
    void (__fastcall* enable_lights)(void* self, void*);                                                    // +04 004A3080
    void (__fastcall* enable_lights_dynamic_only)(void* self, void*);                                       // +08 004A4480
    void (__fastcall* add_light)(void* self, void*, void* light);                                           // +0C 004A39F0 RET 4
    void (__fastcall* remove_light)(void* self, void*, void* light, U flag);                                // +10 004A3AE0 RET 8
    void (__fastcall* add_render_light)(void* self, void*, void* light);                                    // +14 004A3CA0 RET 4
    void (__fastcall* reset)(void* self, void*);                                                            // +18 004A2E00
    void (__fastcall* update_lights)(void* self, void*);                                                    // +1C 004A2E10
    void (__fastcall* enable_highest_priority_lights)(void* self, void*, U count);                         // +20 004A2E70 RET 4
    void (__fastcall* enable_highest_priority_lights_fade)(void* self, void*, U count, void* fade);         // +24 004A3F70 RET 8
    void (__fastcall* void_return_16)(void* self, void*, U, U, U, U);                                       // +28 0063CFC0 RET 10h
    void (__fastcall* disable_bump_map_lights)(void* self, void*);                                          // +2C 004A5680
    void (__fastcall* enable_bump_map_lights)(void* self, void*);                                           // +30 004A5710
    void (__fastcall* collect_lights_for_given_room)(void* self, void*, void* room);                        // +34 0046DE10 RET 4
    void (__fastcall* setup_lights_for_this_gob)(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);   // +38 004A2DB0 RET 8
    void (__fastcall* setup_lights_for_this_room)(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);  // +3C 004A2DE0 RET 8
    void (__fastcall* setup_lights_for_position)(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);   // +40 004A36B0 RET 8
    void (__fastcall* setup_lights_for_position_in)(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits,
                                                    kotor_reconstruction::ArrayMutations* lights);          // +44 004A35B0 RET 0Ch
    void (__fastcall* void_return_8)(void* self, void*, U, U);                                              // +48 005B5E90 RET 8
    void (__fastcall* prioritize_illumination)(void* self, void*, U force_byte);                           // +4C 004A3CF0 RET 4
    void (__fastcall* prioritize_shadow)(void* self, void*);                                                // +50 004A3E30
    void (__fastcall* prioritize_bump)(void* self, void*);                                                  // +54 004A3EE0
    void (__fastcall* void_return)(void* self, void*);                                                      // +58 004015A0
    void (__fastcall* control_lights_fade)(void* self, void*, kotor_reconstruction::ArrayMutations* active,
                                           kotor_reconstruction::ArrayMutations* previous,
                                           kotor_reconstruction::ArrayMutations* desired, U dynamic_only);  // +5C 004A4580 RET 10h
    void (__fastcall* enable_head_light)(void* self, void*);                                                // +60 004A2FF0
};
static_assert(sizeof(LightManagerVTable) == 25 * 4 && offsetof(LightManagerVTable, collect_lights_for_given_room) == 0x34 &&
              offsetof(LightManagerVTable, setup_lights_for_position_in) == 0x44 &&
              offsetof(LightManagerVTable, control_lights_fade) == 0x5C && offsetof(LightManagerVTable, enable_head_light) == 0x60,
              "LightManager table 00743958");

// Object held at PartTriMesh+70 (vertex primitive; its four tables are lane-3d-model's, D39): slot +28 is called with
// no stack words after the vertex colour pool is created or deleted (0046AB23 / 0046AB6B); EAX unused.
struct VertexSlots {
    const void* opaque00[10];                                             // +00..+24
    void (__fastcall* invalidate)(void* self, void*);                     // +28
};
static_assert(offsetof(VertexSlots, invalidate) == 0x28, "vertex primitive slot +28");

template<class Slots> inline const Slots* slots_of(void* object) {
    // mov reg,[object] - the live vtable pointer, reloaded at each call site (object-pointer view, D24).
    return *static_cast<const Slots* const volatile*>(object);
}
} // namespace kotor_scene3d_construct

// ---------------------------------------------------------------- providers authored by this lane
extern "C" {
using kotor_scene3d_construct::U; using kotor_scene3d_construct::I; using kotor_scene3d_construct::List;
// construction
void* __cdecl operator_new_0043e1b0(U size);                                               // jmp 006FA7E6
void* __cdecl find_gob_00440460(const char* name);                                          // scene3d.h name
void __fastcall gob_add_attachment_00447310(void* gob, void*, void* attachment);           // RET 4
void __fastcall behavior_add_dependency_0049f130(void* behavior, void*, void* dependency);  // RET 4
void* __fastcall behavior_attach_0049fb10(void*, void*, void* gob, void* object, void* part);            // RET 0Ch
void* __fastcall behavior_attach_translate_0049fdc0(void*, void*, void* gob, void* object, void* part);  // RET 0Ch
void* __fastcall behavior_attach_rotate_0049fe00(void*, void*, void* gob, void* object, void* part);     // RET 0Ch
void* __fastcall behavior_attach_no_orientation_0049fe40(void*, void*, void* gob, void* object, void* part); // RET 0Ch
void* __fastcall wind_manager_construct_004a6640(void*, void*);                             // RET 0
// event handlers registered by 0049FB10 (function-pointer immediates 0049FC18..0049FCAB)
void __cdecl behavior_update_scenes_attach_0049eb40(void* gob, void* event, aurora_model::Part* context);
void __cdecl behavior_update_dependent_position_0049eb90(void* gob, void* event, aurora_model::Part* context);
void __cdecl behavior_propagate_color_shifting_0049ebb0(void* gob, void* event, aurora_model::Part* context);
void __cdecl behavior_propagate_illumination_0049ec00(void* gob, void* event, aurora_model::Part* context);
void __cdecl behavior_propagate_render_0049ec50(void* gob, void* event, aurora_model::Part* context);
void __cdecl behavior_propagate_view_volume_0049ec90(void* gob, void* event, aurora_model::Part* context);
// lights
void* __fastcall light_manager_construct_004a38b0(void*, void*);                            // RET 0
List* __fastcall light_manager_get_bump_map_lights_004a28a0(void* lights, void*);           // lea eax,[ecx+5Ch]
List* __fastcall light_manager_get_lens_flare_lights_004a28b0(void* lights, void*);         // lea eax,[ecx+0C4h]
void __cdecl set_light_gl_004a2a00(I index, const void* position, const void* color, const float* radius, const U* ambient_flag);
void __fastcall gob_collect_lights_00447180(void* gob, void*, List* out);                  // RET 4
void __cdecl update_static_lighting_0046ca30(void* area, void* light);
void __cdecl add_light_to_rooms_0046cc00(void* area, void* light);
void __cdecl do_static_light_buckets_0046dda0(void* scene);
void __cdecl light_bins_0046b790(void* node, void* context);                                 // BSPTraverse callback 0046DDDC
// model-node lights (unit light_nodes.cpp; aurora_model::MdlNode receivers)
void __fastcall aurora_light_node_internal_destructor_00448000(aurora_model::MdlNode* self, void*);
void __fastcall aurora_light_node_destruct_00448f80(aurora_model::MdlNode* self, void*);
void __fastcall aurora_light_node_binary_internal_destructor_0044a500(aurora_model::MdlNode* self, void*);
// 00485560: spelling and type of the consumer lane-3d-anim (anim3d.h, node InternalControl family; EAX unused by 0043F890)
void __fastcall node_light_internal_control_00485560(void* node, void*, void* part, float t0, float t1, float weight); // RET 10h
void __cdecl light_node_tile_0046db60(void* room, void* scene);
void __cdecl light_part_trimesh_0046a9e0(void* trimesh_part, List lights_by_value, void* scene);
}
// GetNearestLights 004A3770 is declared by scene3d.h (void: the sole caller 00450DBC discards the EAX count, 00450DC1).
// WindManager::fillNoiseField 004A57C0 and CExoArrayList<int> copy ctor 0046D250 are declared by scene3d.h (core).

// ---------------------------------------------------------------- lowers with no provider yet (undefined externals)
extern "C" {
// bsp_visibility sub-lane: BSPSphereCheck 004AD560 cdecl(bsp, center, radius, out) (0046CA75..0046CA86);
// BSPTraverse 004ACEA0 cdecl(node, fn, context) recursing on [node+10]/[node+14] (004ACEB3..004ACECA, callback result ignored)
void __cdecl bsp_sphere_check_004ad560(void* bsp, const void* center, float radius, List* out);
void __cdecl bsp_traverse_004acea0(void* node, void (__cdecl* fn)(void* node, void* context), void* context);
// core: Vector operator== 004AA980 cdecl(a,b) -> 1/0 (004AA980..004AA9B5); spelling of lane-3d-emitter
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// core: ProjectDynamicShadow 0044FF50 cdecl(gob, light) (0045070C..00450713); GLRender::RenderObjectsVCUP 00431F70
// cdecl(mode, vertices, count) (00450943..00450953); _glBlendFunc 00401670 cdecl(i, j) -> glBlendFunc(T[j]... T = 0073F270)
void __cdecl project_dynamic_shadow_0044ff50(void* gob, void* light);
void __cdecl gl_render_objects_vcup_00431f70(U mode, void* vertices, I count);
void __cdecl gl_blend_func_indexed_00401670(U source_index, U destination_index);   // spelling of lane-3d-mesh
// animation sub-lane (lane-3d-anim anim3d.h, identical declaration): MdlNode::InternalControl 004854D0 RET 10h
void __fastcall node_internal_control_004854d0(void* node, void*, void* part, float t0, float t1, float weight);
// lane-3d-model hand-off: MdlNode::~MdlNode 0044A040 thiscall RET 0 (0044906F)
void __fastcall aurora_node_destruct_0044a040(aurora_model::MdlNode* self, void*);
}

// Typed vtable objects defined by this lane (behavior_tables.cpp / light_manager.cpp; D1/D21 typed constant tables).
namespace kotor_scene3d_construct {
extern const BehaviorBaseSlots behavior_vtable_00743864;
extern const BehaviorSlots behavior_attach_vtable_00743894;
extern const BehaviorSlots behavior_attach_translate_vtable_007438b8;
extern const BehaviorSlots behavior_attach_rotate_vtable_007438dc;
extern const BehaviorSlots behavior_attach_no_orientation_vtable_00743900;
extern const LightManagerVTable light_manager_vtable_00743958;
inline const LightManagerVTable* light_manager_vt(void* lights) { return slots_of<LightManagerVTable>(lights); }
}

// ---------------------------------------------------------------- behavior family (behavior_tables.cpp)
extern "C" {
void* __fastcall behavior_scalar_delete_0049ffd0(void* self, void*, U flags);              // RET 4
void* __fastcall behavior_attach_scalar_delete_0049fff0(void* self, void*, U flags);       // RET 4
void* __fastcall behavior_attach_scalar_delete_004a0010(void* self, void*, U flags);       // RET 4
void __fastcall behavior_attach_destruct_004a0030(void* self, void*);                      // jmp 0049FCE0
void __fastcall behavior_attach_destruct_0049fce0(void* self, void*);
void __fastcall behavior_destruct_0049fa70(void* self, void*);
void __fastcall behavior_attach_set_scene_0049f0c0(void* self, void*);
U __fastcall behavior_control_0049f270(void* self, void*, U raw_delta);                    // RET 4
U __fastcall behavior_attach_control_0049f3b0(void* self, void*, U raw_delta);             // RET 4
U __fastcall behavior_attach_translate_control_0049f4f0(void* self, void*, U raw_delta);   // RET 4
U __fastcall behavior_attach_rotate_control_0049f5f0(void* self, void*, U raw_delta);      // RET 4
U __fastcall behavior_attach_no_orientation_control_0049f670(void* self, void*, U raw_delta); // RET 4
void __fastcall behavior_attach_message_0049ed00(void* self, void*, char* line);           // RET 4
void __fastcall behavior_attach_translate_message_0049ed40(void* self, void*, char* line); // RET 4
void __fastcall behavior_attach_rotate_message_0049eda0(void* self, void*, char* line);    // RET 4
U __fastcall behavior_attach_get_type_0049fcd0(void* self, void*);
U __fastcall behavior_attach_translate_get_type_0049fdf0(void* self, void*);
U __fastcall behavior_attach_rotate_get_type_0049fe30(void* self, void*);
U __fastcall behavior_attach_no_orientation_get_type_0049fe70(void* self, void*);
void __fastcall behavior_attach_update_gob_position_0049ecd0(void* self, void*);
void __fastcall gob_remove_attachment_00447370(void* gob, void*, void* attachment);        // RET 4
// typed instances of shared one-instruction bodies (D24: a function of each slot's own type)
void __fastcall behavior_void_return_004015a0(void* self, void*);                          // ret
void __fastcall behavior_void_return_4_0060e760(void* self, void*, char* line);            // ret 4
U __fastcall behavior_return_zero_0063e7f0(void* self, void*);                             // xor eax,eax; ret
void* __fastcall behavior_null_return_0063e7f0(void* self, void*);                         // xor eax,eax; ret (pointer slot +14)
void* __fastcall behavior_self_return_00641db0(void* self, void*);                         // mov eax,ecx; ret
}
// ---------------------------------------------------------------- LightManager family (light_manager.cpp)
extern "C" {
void __fastcall light_manager_prioritize_lights_004a2f30(void* self, void*, const kotor_scene3d::Vector* position, U flag);
void __fastcall light_manager_enable_lights_004a3080(void* self, void*);
void __fastcall light_manager_enable_lights_dynamic_only_004a4480(void* self, void*);
void __fastcall light_manager_add_light_004a39f0(void* self, void*, void* light);
void __fastcall light_manager_remove_light_004a3ae0(void* self, void*, void* light, U flag);
void __fastcall light_manager_add_render_light_004a3ca0(void* self, void*, void* light);
void __fastcall light_manager_reset_004a2e00(void* self, void*);
void __fastcall light_manager_update_lights_004a2e10(void* self, void*);
void __fastcall light_manager_enable_highest_priority_lights_004a2e70(void* self, void*, U count);
void __fastcall light_manager_enable_highest_priority_lights_fade_004a3f70(void* self, void*, U count, void* fade);
void __fastcall light_manager_void_return_16_0063cfc0(void* self, void*, U, U, U, U);
void __fastcall light_manager_disable_bump_map_lights_004a5680(void* self, void*);
void __fastcall light_manager_enable_bump_map_lights_004a5710(void* self, void*);
void __fastcall light_manager_collect_lights_for_given_room_0046de10(void* self, void*, void* room);
void __fastcall light_manager_setup_lights_for_this_gob_004a2db0(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);
void __fastcall light_manager_setup_lights_for_this_room_004a2de0(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);
void __fastcall light_manager_setup_lights_for_position_004a36b0(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits);
void __fastcall light_manager_setup_lights_for_position_in_004a35b0(void* self, void*, const kotor_scene3d::Vector* position, U radius_bits,
                                                                     kotor_reconstruction::ArrayMutations* lights);
void __fastcall light_manager_void_return_8_005b5e90(void* self, void*, U, U);
void __fastcall light_manager_prioritize_illumination_004a3cf0(void* self, void*, U force_byte);
void __fastcall light_manager_prioritize_shadow_004a3e30(void* self, void*);
void __fastcall light_manager_prioritize_bump_004a3ee0(void* self, void*);
void __fastcall light_manager_void_return_004015a0(void* self, void*);
void __fastcall light_manager_control_lights_fade_004a4580(void* self, void*, kotor_reconstruction::ArrayMutations* active,
                                                           kotor_reconstruction::ArrayMutations* previous,
                                                           kotor_reconstruction::ArrayMutations* desired, U dynamic_only);
void __fastcall light_manager_enable_head_light_004a2ff0(void* self, void*);
U __fastcall light_manager_disable_this_light_004a4f10(void* self, void*, void* light);    // RET 4, EAX 1/0
U __fastcall light_manager_enable_this_light_004a4ff0(void* self, void*, void* light);     // RET 4, EAX 1/0
int __cdecl light_compare_004a28c0(const void* left, const void* right);                   // qsort comparator
void __cdecl aur_enable_head_light_0044da60();
void __cdecl aur_disable_head_light_0044da80();
}
