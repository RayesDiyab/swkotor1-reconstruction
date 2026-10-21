#pragma once
// lane-3d-anim: 3D animation sub-area of swkotor.exe (GOG K1, image base 00400000).
// Scope (SUBLANES.json "animation" + lane-3d-model REMAINING.json group "animation", deduplicated; see PACKET.json):
// controllers (NewController::Get*/Control/ReportKey[s], MdlNode*::InternalControl, ControlDispatch, Interpolate*),
// hierarchy drivers (AnimateHierarchy, ProceduralAnimateHierarchy, AnimateEvents), animation lookup/run management
// (FindAnimation, CurrentAnimation, AnimRun, Gob::PlayAnimation, Gob::Animate, Part::Animate) and the CPU skin
// path (GatherBones, ExecuteSkinTransforms).
//
// Conventions (DECISIONS D1/D1-amendment/D21/D24/D30-FINAL; same as lane-3d-render scene3d.h):
//  * original __thiscall bodies are __fastcall(Self*, void* unused_edx, stack...); callers pass nullptr for EDX;
//    no slot used here returns a struct by value and none is variadic (D1-amendment scan: clean).
//  * virtual calls reload the receiver's vtable word at every call and read the slot through a typed table VIEW
//    (object-pointer conversion of the vtable word only, D24); no function-pointer casts.
//  * D30-FINAL: x87 wide intermediates are `double`; a value becomes `float` only where the original executes an
//    fstp/fst dword store (each store is cited). The original runs with x87 CW 0x027F (PC_53, RC nearest), so the
//    double intermediates have the same significand width; only the x87 extended exponent range differs.
//  * x87 comparison idioms are mapped to the ordered C++ relation whose NaN result equals the original branch
//    (e.g. `fcomp B; test ah,41h; jne L` falls through only for A > B, so it is written `if (a > b)`).
//  * Objects without a published layout are byte-offset views (kotor_scene3d::field / at); every offset is cited.
#include <cstddef>
#include "901ebaee8b-scene3d.h"        // lane-3d-render canonical header (+ vtables_gen.h: typed Scene/Gob tables, D42)
#include "8ada3b8e0f-model_vtables.hpp"  // lane-3d-model typed Part tables (D39)

namespace texture { struct Basic; }
namespace aurora_model { struct MdlNode; }   // model_load.hpp (forward only; same declaration)   // lane-texture-epoch texture.hpp (forward only; pointer use)

namespace kotor_renderer_init {
// Single GL adapter owner (lane-renderer-bodies aurora-windows-gl-imports); exact copies of the owner declarations
// (glEnable_0073d390 / glDisable_0073d22c are declared in scene3d.h).
extern void __stdcall glBlendFunc_0073d38c(std::uint32_t, std::uint32_t);
extern void __stdcall glEnableClientState_0073d260(std::uint32_t);
extern void __stdcall glDisableClientState_0073d264(std::uint32_t);
// Same alias as lane-renderer-bodies extension_loader.h / lane-3d-mesh trimesh_skin.h (dispatch cell 007A47E0).
using EnableDispatch = void (__cdecl*)(std::uint32_t, texture::Basic*, texture::Basic*, texture::Basic*, texture::Basic*);
}

namespace kotor_anim3d {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using I16 = std::int16_t;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;

// Exact .rdata float constants (bit patterns read from the pinned image; C++17 hex literals are exact).
constexpr float kOne_0073d764 = 1.0f;                       // 3F800000
constexpr float kZero_0073d700 = 0.0f;                      // 00000000
constexpr double kOneDouble_0073ede8 = 1.0;                 // qword 3FF0000000000000
constexpr float kHalf_0073e9ac = 0.5f;                      // 3F000000
constexpr float kThousand_0073d6fc = 1000.0f;               // 447A0000
constexpr float kMilli_0073d708 = 0x1.0624dep-10f;          // 3A83126F (0.001f)
constexpr float kMinus100_0074186c = -100.0f;               // C2C80000
constexpr float kTwo_00741be0 = 2.0f;                       // 40000000
constexpr float kBig_00743244 = 1000000000.0f;              // 4E6E6B28
constexpr float kMinusBig_00743248 = -1000000000.0f;        // CE6E6B28
constexpr float kQuat10_0074324c = 0x1.00804p-9f;           // 3B004020 (2/1022)
constexpr float kQuat11_00743250 = 0x1.00401p-10f;          // 3A802008 (2/2046)
constexpr float kMinus95_00743254 = -95.0f;                 // C2BE0000
constexpr float kLodRadius_00743258 = 0.625f;               // 3F200000
constexpr float kEpsilon_0074327c = 0x1.ad7f2ap-24f;        // 33D6BF95 (1e-7f)
constexpr U kMinusEpsilonBits = 0xB3D6BF95u;                // 00485E63 immediate (-1e-7f)
constexpr U kWindDefaultBits = 0x3F99999Au;                 // 004866FC immediate (1.2f)
// .rdata GL enums
constexpr U kGlTextureCoordArray_0073f398 = 0x8078u;
constexpr U kGlSrcAlpha_0073f270 = 0x0302u;
constexpr U kGlOneMinusSrcAlpha_0073f274 = 0x0303u;
constexpr U kGlOne_0073f278 = 0x0001u;
constexpr U kGlZero_0073f27c = 0x0000u;

// ---------------------------------------------------------------- typed tables
// Gob / Scene: lane-3d-render vtables_gen.h (aurora_model::GobVTable 00741268, kotor_scene3d::SceneVTable 00741708).
// Part family: lane-3d-model model_vtables.hpp; every Part table shares the 31-slot prefix of VTable_00740ae8 (Part),
// which is the view used here for slots +24..+6C. Receivers without a published table use the D24 views below.
using PartTable = aurora_vt::VTable_00740ae8;
inline const aurora_model::GobVTable* gob_table(void* gob) { return static_cast<const aurora_model::GobVTable*>(field<void*>(gob, 0)); }
inline const kotor_scene3d::SceneVTable* scene_table(void* scene) { return static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(scene, 0)); }
inline const PartTable* part_table(void* part) { return static_cast<const PartTable*>(field<void*>(part, 0)); }
inline aurora_model::Gob* as_gob(void* gob) { return static_cast<aurora_model::Gob*>(gob); }
inline void* as_object(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U as_word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
struct LookAtSlots {                                          // LookAtAnimation (Gob+174) table (no published table)
    void (__fastcall* scalar_delete)(void* object, void*, U flags);   // +00 (00485D27 / 0048699B push 1; RET 4)
    U8 (__fastcall* query04)(void* object, void*);                     // +04 (0048691E, AL)
    const void* s08;
    void (__fastcall* slot0c)(void* object, void*);                    // +0C (0048692D, RET 0)
};
struct BehaviorSlots {                                        // CAurBehavior (Gob+188) table (no published table)
    const void* s00[2];
    void (__fastcall* slot08)(void* behavior, void*, U raw_dt);        // +08 (00486802; RET 4)
};
struct TextureSlots {                                         // CAurTexture table: lane-texture-epoch texture_vtable.hpp
    const void* s00[0xB4 / 4];                                //   publishes +B4 (Bind) through font::Texture; this view
    void (__fastcall* slotb4)(void* texture, void*);          //   avoids that include chain (+B4: 00420E7D, 00487F14; RET 0)
};
struct MeshObjectSlots {                                      // anim-mesh Part+70 object table (no published table)
    const void* s00[3];
    void* (__fastcall* slot0c)(void* object, void*);                   // +0C (004856DB / 00485867 / 00485995)
};
template <class Slots> inline const Slots* slots(void* object) { return static_cast<const Slots*>(field<void*>(object, 0)); }

// ---------------------------------------------------------------- borrowed globals (Main publishes once)
using ProcU = void (__stdcall*)(std::uint32_t);   // glActiveTextureARB / glClientActiveTextureARB
struct Cells {
    PointerList* ordered_textures_007a4798;      // 00420E50..00420E8F data 7A4798 / count 7A479C (lane-texture-epoch texture_cells::ordered_texture_list_007a4798)
    const volatile float* frame_delta_0078e574;  // 00485483 fld (lane-texture-epoch delta_t_0078e574)
    volatile U* current_stage_007a6898;          // 00487D1C.. (lane-renderer-bodies current_stage_007a6898 / renderer_system zero_007a6898)
    ProcU const volatile* client_active_texture_007bb6b0; // 00487D16 (kotor_renderer_gl_procs::glClientActiveTextureARB_007bb6b0)
    ProcU const volatile* active_texture_007bb80c;        // 00487F06 (kotor_renderer_gl_procs::glActiveTextureARB_007bb80c)
    kotor_renderer_init::EnableDispatch const volatile* enable_dispatch_007a47e0; // 00487FA9 / 00487FBA
    const volatile U* gl_alpha_test_0078d44c;    // 00487FC8 / 00488002 (lane-gui-draw kotor_renderer_cells::enable_0078d44c)
    const volatile U* gl_texture_2d_0078d458;    // 0048805C (lane-gui-draw kotor_renderer_cells::enable_0078d458)
};
extern const Cells& anim_cells;
} // namespace kotor_anim3d

// ---------------------------------------------------------------- cells owned by this area
// Only functions of this area reference these addresses (exe-text scan: no other reader/writer, no data-table
// reference). Storage and original initial values: anim_cells.cpp.
extern "C" {
// .data (no writer instruction in .text; values read from the pinned image)
extern volatile kotor_scene3d::I anim_max_depth_00798b18;       // 1000 (00483E7E)
extern volatile float anim_lod_near_00798b1c;                   // 12.0f (00483E5D)
extern volatile float anim_lod_far_00798b20;                    // 30.0f (00483E41)
extern volatile kotor_scene3d::I anim_lod_far_depth_00798b24;   // 8 (00483E53)
extern volatile kotor_scene3d::I anim_lod_near_depth_00798b28;  // 12 (00483E73)
extern volatile kotor_scene3d::U anim_lod_enabled_00798b2c;     // 1 (00483DFF)
extern volatile kotor_scene3d::U anim_slot188_enabled_00798b3c; // 1 (00486FDA)
extern volatile kotor_scene3d::U anim_enabled_00798b40;         // 1 (00486696)
extern volatile kotor_scene3d::U anim_lod_parts_00798b44;       // 1 (00486FA4)
// BSS
extern void* volatile anim_report_run_008302cc;                 // 00486E49 / 004854E0..00485509
extern volatile kotor_scene3d::U anim_report_reverse_008302c8;  // 00486E4F / 0048531D (bit 0 tested)
extern volatile float anim_report_length_008302d0;              // 00486E5A / 0048528F
extern volatile kotor_scene3d::I anim_depth_008302d4;           // 00483D80..00484247
extern volatile kotor_scene3d::I anim_max_seen_depth_008302d8;  // 00483D98..
extern volatile kotor_scene3d::U anim_debug_008302e0;           // 00486FC7 (no writer)
extern volatile kotor_scene3d::U anim_focus_override_008302e4;  // 00486848 (no writer)
extern volatile float anim_last_blend_008302e8;                 // 00486EF1
extern float anim_vector_key_b_008302ec[3];                     // GetVectorValue static (guard bit 2)
extern float anim_vector_key_a_008302f8[3];                     // GetVectorValue static (guard bit 1)
extern volatile kotor_scene3d::U anim_vector_guard_00830304;
extern kotor_scene3d::Quaternion anim_quaternion_key_b_00830308; // GetQuaternionValue static (guard bit 2)
extern kotor_scene3d::Quaternion anim_quaternion_key_a_00830318; // GetQuaternionValue static (guard bit 1)
extern volatile kotor_scene3d::U anim_quaternion_guard_00830328;
extern float anim_control_previous_0083032c;                    // NewController::Control statics (addresses passed as out-params)
extern kotor_scene3d::U anim_control_current_00830330;
extern float anim_control_value_00830334;
extern kotor_scene3d::U skin_normal_pool_008303a8;              // ExecuteSkinTransforms statics (addresses passed to RequestPool)
extern kotor_scene3d::U skin_vertex_pool_008303ac;
extern volatile kotor_scene3d::U skin_pool_vertices_008303b0;
extern volatile kotor_scene3d::U skin_guard_008303b4;
}

// ---------------------------------------------------------------- authored in this area
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// texture / scene flags
// void __cdecl aur_texture_force_controller_update_00420e50();   (declared in scene3d.h)
// scene_disable_animations_0044f3b0 (U, Scene vtbl+64) / scene_is_animating_0044f3c0 (U8, Scene vtbl+A8): declared by vtables_gen.h
// interpolation
Vector* __cdecl vector_interpolate_0046eea0(Vector* out, const Vector* a, const Vector* b, float t);
Quaternion* __cdecl quaternion_interpolate_00481aa0(Quaternion* out, const Quaternion* a, const Quaternion* b, float t);
double __cdecl interpolate_bezier_004aaae0(float p0, float p3, float p1, float p2, float t);  // ST0
Vector* __cdecl vector_interpolate_bezier_004ac110(Vector* out, const Vector* p0, const Vector* p3,
    const Vector* p1, const Vector* p2, float t);
// NewController (16-byte records in MdlNode+38)
void __fastcall controller_get_key_index_and_alpha_00482b70(void* controller, void*, float t, void* node, I* index, float* alpha); // RET 10h
U __fastcall controller_get_float_value_00482bf0(void* controller, void*, float t, void* node, float* out);       // RET 0Ch
U __fastcall controller_get_vector_value_00482d20(void* controller, void*, float t, void* node, Vector* out);     // RET 0Ch
void __fastcall controller_get_quaternion_from_index_00483050(void* controller, void*, I index, void* node, Quaternion* out); // RET 0Ch
U __fastcall controller_get_quaternion_value_004831b0(void* controller, void*, float t, void* node, Quaternion* out); // RET 0Ch
void __fastcall controller_control_00483330(void* controller, void*, void* part, void* node, float t0, float t1, float weight); // RET 14h
void __fastcall controller_report_key_00484ec0(void* controller, void*, void* part, void* node, I key, U raw_delta); // RET 10h
void __fastcall controller_report_keys_00485250(void* controller, void*, void* part, void* node, float t0, float t1); // RET 10h
// node InternalControl family (MdlNode* receiver, RET 10h)
void __fastcall node_internal_control_004854d0(void* node, void*, void* part, float t0, float t1, float weight);
// 00485560 MdlNodeLight::InternalControl -> lane-3d-construct (light_nodes.cpp, SUBLANES lights);
// 00485590 MdlNodeEmitter::InternalControl -> lane-3d-emitter (emitter_core.cpp, SUBLANES part_emitter). Same spellings:
void __fastcall node_light_internal_control_00485560(void* node, void*, void* part, float t0, float t1, float weight);
void __fastcall node_emitter_internal_control_00485590(void* node, void*, void* part, float t0, float t1, float weight);
void __fastcall node_camera_internal_control_004855d0(void* node, void*, void* part, float t0, float t1, float weight);
void __fastcall node_reference_internal_control_00485600(void* node, void*, void* part, float t0, float t1, float weight);
void __fastcall node_trimesh_internal_control_00485630(void* node, void*, void* part, float t0, float t1, float weight);
void __fastcall node_anim_mesh_internal_control_00485660(void* node, void*, void* part, float t0, float t1, float weight);
// ControlDispatch: symbol already consumed by lane-3d-model (model_instance.hpp declares a U return; PROPOSED void,
// see proposed/model_instance.hpp.diff - the original returns no value: tail jumps / `ret 10h` with EAX unset).
void __fastcall aurora_node_control_dispatch_0043f890(aurora_model::MdlNode* self, void* unused_edx,
    aurora_model::Part* part, aurora_model::U raw_time, aurora_model::U raw_a, aurora_model::U raw_weight);
U __fastcall part_animate_00440110(void* self, void* unused_edx, U raw_dt);                 // Part vtbl+6C, RET 4 (model_vtables.cpp spelling)
// hierarchy / events / lookup
void __cdecl animate_hierarchy_00483d80(void* part, void* node, float t0, float t1, float weight, bool force);
void __cdecl procedural_animate_hierarchy_00483630(void* part);
void __cdecl animate_events_004826b0(void* gob, void* animation, float t0, float t1, U flags);
void* __cdecl find_animation_004822a0(void* model, const char* name);
void* __cdecl current_animation_00484ca0(void* gob);
double __cdecl animation_scale_00484ce0(void* model, void* animation);                        // ST0
void* __fastcall anim_run_construct_00481890(void* run, void*, void* animation);             // RET 4
void __fastcall animation_destruct_004850a0(aurora_model::MaxTree* self, void*);            // RET 0 (model_destroy.cpp spelling)
// gob_animate_00486670 (Gob vtbl+14, RET 4) / gob_play_animation_00485bd0 (Gob vtbl+18, RET 10h): declared by vtables_gen.h
// CPU skin path
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations, Vector position, Quaternion orientation);
void __cdecl execute_skin_transforms_00487570(void* node, PointerList* positions, PointerList* matrices, void* primitive, U flag);   // flag byte read (00487C0D)
}

// contested addresses (SUBLANES.json assigns them to another sub-area; authored in anim_contested.cpp so the
// integrator can stage or drop that single unit; see PACKET.json review_needed)
extern "C" {
void __fastcall quaternion_list_allocate_00437cc0(PointerList* list, void*, I capacity);          // core; RET 4
void __fastcall quaternion_list_add_00438af0(PointerList* list, void*, Quaternion value);        // core; RET 10h
Quaternion* __cdecl quaternion_scalar_multiply_004a9b80(Quaternion* out, const Quaternion* q, float s); // core
double __cdecl quaternion_dot_004a9d30(const Quaternion* a, const Quaternion* b);                // core; ST0
Quaternion* __cdecl quaternion_slerp_004a9e00(Quaternion* out, Quaternion from, const Quaternion* to, float t); // core
}

// ---------------------------------------------------------------- lowers with another owner (undefined externals)
extern "C" {
// lane-3d-model (authored there)
aurora_model::MaxTree* __fastcall aurora_tree_as_model_0043e1c0(aurora_model::MaxTree* self, void* unused_edx);
void* __fastcall aurora_node_as_trimesh_0043e400(void* self, void* unused_edx);   // model_load.hpp spells MdlNode*; same ABI/symbol
kotor_scene3d::U8 __fastcall aurora_trimesh_has_tangent_bases_0043e410(void* self, void* unused_edx);
void __fastcall aurora_list_insert_00445020(PointerList* self, void* unused_edx, U value, I index);
U __cdecl aurora_pool_request_00423ee0(U a1, U a2, U a3, U unused4, U* out);
void* __cdecl aurora_pool_lock_00423f10(U handle, U unused2, U unused3, U a4);
U __cdecl aurora_pool_unlock_00423f30(U handle);
extern volatile aurora_model::U aurora_control_weight_00798b38;                     // .data 1.0f (model_instance.hpp)
// handoff lane-3d-model: MaxTree::~MaxTree (not yet authored)
void __fastcall maxtree_destruct_0043f730(void* tree, void*);                        // RET 0
// lane-r4-lowers (authored): Vector difference out = a - b
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);
// lane-font (M43): CExoArrayList<Vector>::allocate
// (declared in font_txi.hpp with font_txi::List*; same ABI; declared here over PointerList)
void __fastcall vector_list_allocate_00421310(PointerList* list, void* unused_edx, I size);  // RET 4
// core (SUBLANES "core", not yet authored)
void __fastcall gob_process_00444840(void* gob, void*);                              // RET 0
double __cdecl aurora_fabs_00423b70(float value);                                    // ST0
Vector* __fastcall vector_add_assign_004a9740(Vector* self, void*, const Vector* other); // RET 4, EAX = self
Vector* __cdecl matrix_multiply_vector_004a9a60(Vector* out, const float* matrix9, const Vector* v);
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
Quaternion* __cdecl quaternion_add_004a9d00(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __fastcall kotor_fastcall_00445080(void* list, void*, void* element);           // CExoArrayList::Remove(value), RET 4 (lane-r4-lowers spelling; lane-input: array_remove_00445080)
kotor_scene3d::U8 __fastcall gob_model_flag2_00481d60(void* gob, void*);             // AL = (model+50 >> 2) & 1 (core)
kotor_scene3d::U8 __fastcall gob_model_flag4_00481d80(void* gob, void*);             // AL = (model+50 >> 4) & 1 (core)
void __fastcall wind_manager_add_point_source_wind_004a6510(void* wind, void*, Vector position, U raw_a, U raw_b, U raw_c); // RET 18h
U __cdecl gl_render_resize_pool_00423f70(U handle, U bytes);
void __cdecl gl_render_set_vertex_buffer_00425900(U stride, U offset, U pool, I count);
void __cdecl gl_render_set_normal_buffer_004259e0(U stride, U offset, U pool, I count);
void __cdecl gl_render_set_tex_coord_buffer_00425a40(U stride, U offset, U pool, I count);
// lane-renderer-bodies (D32): CExoArrayList::deleteAt
// declared in render_pool.h as deleteAt_006857a0(List*, void*, I32); same ABI
void __fastcall deleteAt_006857a0(PointerList* self, void* unused_edx, I index);
// sub-lane vp_nv_ati
U __cdecl aur_vertex_programs_nv_available_0045f740();
kotor_scene3d::U8 __cdecl aur_ati_fragment_shaders_bump_map_available_0045fb90();
// sub-lane trimesh_skin (lane-3d-mesh spellings)
void __cdecl render_flat_0046f900(void* primitive);
void __cdecl render_environment_mapped_0046ff70(void* primitive, U arg1, U arg2);
std::uint8_t __fastcall material_has_env_map_0047b0e0(void* material, void*);
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx);
// sub-lane bsp_visibility (lane-3d-bsp spelling)
void __cdecl gl_render_bind_texture_00420440(U target_index, U texture);
// sub-lane part_emitter
void __fastcall part_emitter_calculate_radius_0048e1b0(void* emitter, void*);        // RET 0
// CRT (strict link: string-compare-existing.obj) / M41
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// M40 world-timer-x87-conversion.obj (strict link): original _ftol2 006FAE8C, argument ST0, result EDX:EAX.
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
}

namespace kotor_anim3d {
// __ftol2 006FAE8C on the x87 value `value` (D30-FINAL: original CRT port; truncation, indefinite 0x80000000 for
// NaN / out of range). The double is loaded exactly (fld qword), then the M40 port converts ST0 -> EDX:EAX.
inline I ftol2(double value) {
    I converted;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov converted, eax
    }
    return converted;
}
}
