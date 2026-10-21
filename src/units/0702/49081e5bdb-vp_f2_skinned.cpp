// lane-3d-vp fork F2: RenderSkinnedVP 0048A520 (swkotor.exe GOG K1, base 00400000), cdecl(VertexPrimitive*),
// plain RET (0048C64E); caller PartSkin::Draw 0048D6C0 (lane-3d-mesh trimesh_f1.cpp).
// Evidence: lane-3d-vp/d/0048A520.s (dumpbin /disasm of the pinned exe); body-frame offsets in comments are
// "F<x>" = [esp+x] minus the outstanding push depth (build/vp_f2_frame.py; consistent at every join point):
// locals F10..F16F (sub esp,160h), EH state F178, argument F180.
//
// D30-FINAL: values the original keeps on the x87 stack are double (W); float only at fstp-dword stores (S).
// Unwind (FuncInfo via handler 00717865; funclets 00717760..00717851): they only (a) clear a guard bit of
// [0083042C] if VertexProgram::VertexProgram or operator new throws, (b) free the operator-new block, (c) free the
// copied light list (006FA390 / 0047ABC0). live_operator_new returns NULL (no throw) and no callee here throws on
// reachable paths, so the funclets are cited, not emitted (D30-FINAL unwind rule); the light list is freed on the
// normal path exactly where the original frees it.
// Function-local statics: the two CExoArrayList objects 00830414 / 00830420 (guard bits 2 / 1 of 0083042C) register
// their original atexit destructors 0073BD70 / 0073BD90 (free data if non-null) via _atexit 006FB332; reproduced
// with std::atexit of functions performing the same frees. VertexProgram statics (guard bits 4..100h) have no dtor.
#include <cstdlib>
#include <cstring>
#include "4bf744a1f4-vp_nv_ati.h"
#include "7ee14b5ec3-model_vtables.hpp"   // lane-3d-model typed Part / VertexPrimitive tables (D39)

// ---------------------------------------------------------------- existing owners (exact copies)
namespace aurora_model { struct MdlNode; }
extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;
// lane-3d-mesh trimesh_skin.h
void __cdecl render_flat_0046f900(void* primitive);
void __cdecl render_environment_mapped_0046ff70(void* primitive, U arg1, U arg2);
std::uint8_t __fastcall material_has_bumpy_shiny_map_0047b0c0(void* material, void*);      // AL
std::uint8_t __fastcall material_has_env_map_0047b0e0(void* material, void*);               // AL
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx); // AL
Vector* __fastcall material_get_bump_map_specular_color_00471430(void* material, void*, Vector* out); // RET 4 -> out
PointerList* __fastcall light_manager_get_bump_map_lights_004a28a0(void* lights, void*);   // lane-3d-construct spelling
void __cdecl gl_render_activate_texture_stage_00425250(U stage);                          // client-texture stage
Vector* __cdecl vector_multiply_004ab1e0(Vector* out, const Vector* a, const Vector* b);
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __cdecl gl_blend_func_indexed_00401670(U source_index, U destination_index);          // glBlendFunc(tbl[a],tbl[b]) tbl 0073F270
U __fastcall light_manager_disable_bump_map_lights_004a5680(void* lights, void* unused_edx);
U __fastcall light_manager_enable_bump_map_lights_004a5710(void* lights, void* unused_edx);
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations,
                                   Vector translation, Quaternion rotation);              // cdecl, add esp,28h
// lane-3d-model model_instance.hpp (aurora_model::U8 == std::uint8_t)
std::uint8_t __fastcall aurora_trimesh_has_tangent_bases_0043e410(aurora_model::MdlNode* self, void* unused_edx);
// lane-texture-epoch texture_controller.hpp (ECX = Material, as lane-3d-mesh trimesh_f2.cpp 00470D51)
std::uint8_t __fastcall texture_controller_is_linear_0041f250(void*, void*);
// lane-texture-epoch texture_vtable.hpp (slot bodies; used only as slot types for the typed view below)
std::uint8_t __fastcall texture_is_cube_map_0041ebf0(void*, void*);                         // AL
float __fastcall texture_get_bump_intensity_0041edd0(void*, void*);                         // ST0
void* __fastcall texture_get_specular_color_0041ed60(void*, void*, void*);                  // ret 4 (hidden Vector result)
// lane-3d-vp fork F4 spelling (NEW, owner sublane "core"): Matrix::Matrix 00437AF0, thiscall RET 0, EAX = this.
float* __fastcall matrix_identity_00437af0(float* matrix9, void* unused_edx);
}
namespace renderer_program_cells {   // task-22 renderer-adoption-packet-r1 program_cells.cpp (lane-window spelling)
extern void* output_0083037C; extern void* output_00830380;
}
namespace kotor_aurora_window { extern void* output_00830384; }   // lane-window aurora_window_slots.h

// ---------------------------------------------------------------- cells owned here
// Full-image xref (lane-gui-draw/exe-text.asm): 008303F8..00830428, 0083042C, 00830588, 00830378, 00798B50/54 are
// referenced only by 0048A520 (plus its unwind funclets / atexit thunks). Cells marked SHARED are also read by
// lane-3d-mesh (trimesh_skin) bodies, which borrow them through MeshCells; this unit proposes to be their single
// storage owner (cold values from the pinned image, no writer instruction anywhere).
namespace kotor_vp_cells {
using kotor_scene3d::PointerList;
PointerList bone_positions_00830414;            // CExoArrayList<Vector> static (data 830414, size 830418, cap 83041C)
PointerList bone_orientations_00830420;         // CExoArrayList<Quaternion> static (data 830420, size 830424, cap 830428)
volatile U statics_guard_0083042c = 0;          // guard bits 1,2 (lists), 4,8,10h,20h,40h,80h,100h (programs)
VertexProgram* volatile skinned_program_008303f8 = nullptr;   // bit 100h (0048C506)
VertexProgram* volatile skinned_program_008303fc = nullptr;   // bit 80h  (0048BD0A)
VertexProgram* volatile skinned_program_00830400 = nullptr;   // bit 40h  (0048BBBC)
VertexProgram* volatile skinned_program_00830404 = nullptr;   // bit 20h  (0048B5D4)
VertexProgram* volatile skinned_program_00830408 = nullptr;   // bit 10h  (0048B4CB)
VertexProgram* volatile skinned_program_0083040c = nullptr;   // bit 8    (0048B3EA)
VertexProgram* volatile skinned_program_00830410 = nullptr;   // bit 4    (0048AE0E)
std::uint32_t* volatile parameters_00830588 = nullptr;        // BSS, no store in the image
std::uint32_t* volatile parameters_00830294 = nullptr;        // SHARED (also 0047721E); BSS, no store in the image
volatile U select_00830378 = 0;                               // BSS, no store in the image (0048ACEE / 0048C4A5)
volatile U gate_00798b50 = 1, gate_00798b54 = 1;              // .data cold 1
volatile U gate_0078e5fc = 1, gate_0078e650 = 1, gate_0078e66c = 1;   // SHARED .data cold 1
volatile U gate_00798b48 = 1, gate_00798b4c = 1;              // SHARED .data cold 1
volatile float scale_0078e630 = 1.0f, scale_0078e648 = 1.0f;  // SHARED .data cold 3F800000
}

namespace {
using namespace kotor_vp;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::cells;
namespace gl = kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
namespace rc = kotor_renderer_gl_cells;
namespace vc = kotor_vp_cells;
namespace vd = kotor_vp_data;

// .rdata words read by `mov reg,[addr]` (values from the pinned image).
constexpr U kGlSrcAlpha_0073f270 = 0x302, kGlOneMinusSrcAlpha_0073f274 = 0x303;
constexpr U kGlOne_0073f278 = 1, kGlZero_0073f27c = 0;
constexpr U kTexCoordArray_0073f398 = 0x8078, kNormalArray_0073f39c = 0x8075;
constexpr U kAttrib12_0073f3b8 = 0x865C, kAttrib13_0073f3bc = 0x865D, kAttrib14_0073f3c0 = 0x865E;
constexpr double kNine_00742f00 = 9.0;          // .rdata 41100000h (fdivr dword)
constexpr U kOne = 0x3F800000u, kHalf = 0x3F000000u, kTwo = 0x40000000u, kThree = 0x40400000u;
constexpr U kVertexProgram = 0x8620;            // GL_VERTEX_PROGRAM_ARB / NV immediates

// Typed dispatch views (object-pointer conversions only, D24); receiver table reloaded at each call.
using PartTable = aurora_vt::VTable_00740e10;          // PartSkin (slots +0C GetWorld, +2C/+50 self_return)
using PrimitiveTable = aurora_vt::VTable_00742fac;     // VertexPrimitiveSkinned (+18 SetupVertexArraysStandard)
struct TextureSlotsView {                              // CAurTexture table prefix (lane-texture-epoch CommonSlots types)
    const void* s00_s08[3];
    decltype(&texture_is_cube_map_0041ebf0) s0c;
    const void* s10_s78[27];
    decltype(&texture_get_bump_intensity_0041edd0) s7c;
    const void* s80_s84[2];
    decltype(&texture_get_specular_color_0041ed60) s88;
};
static_assert(offsetof(TextureSlotsView, s0c) == 0x0C && offsetof(TextureSlotsView, s7c) == 0x7C &&
              offsetof(TextureSlotsView, s88) == 0x88, "texture slot offsets");
struct LightSlotsView {                                // LightManager table 00743958 prefix (lane-3d-mesh request)
    const void* s00_s28[11];
    decltype(&light_manager_disable_bump_map_lights_004a5680) s2c;
    decltype(&light_manager_enable_bump_map_lights_004a5710) s30;
};
static_assert(offsetof(LightSlotsView, s2c) == 0x2C && offsetof(LightSlotsView, s30) == 0x30, "light slot offsets");
const PartTable* part_table(void* part) { return static_cast<const PartTable*>(field<void*>(part, 0)); }
const PrimitiveTable* primitive_table(void* p) { return static_cast<const PrimitiveTable*>(field<void*>(p, 0)); }
const TextureSlotsView* texture_table(void* t) { return static_cast<const TextureSlotsView*>(field<void*>(t, 0)); }
const LightSlotsView* light_table(void* l) { return static_cast<const LightSlotsView*>(field<void*>(l, 0)); }
const kotor_scene3d::SceneVTable* scene_table(void* s) { return static_cast<const kotor_scene3d::SceneVTable*>(field<void*>(s, 0)); }

void* scene() { return *cells.active_scene_007b92a0; }                     // mov reg,[007B92A0h] (re-read per use)
void* light_manager() { return field<void*>(scene(), 0x80); }              // [scene+80h]
void lights_disable() { void* l = light_manager(); light_table(l)->s2c(l, nullptr); }   // call [edx+2Ch]
void lights_enable() { void* l = light_manager(); light_table(l)->s30(l, nullptr); }    // call [edx+30h]
void scene_do_fog() { void* s = scene(); scene_table(s)->s124_scene_do_fog(s, nullptr); } // call [eax+124h]
texture::Basic* tex(void* material, U offset) { return field<texture::Basic*>(material, offset); }
void dispatch(U op, texture::Basic* a, texture::Basic* b, texture::Basic* c, texture::Basic* d) {
    rc::enable_dispatch_007a47e0(op, a, b, c, d);                          // call dword ptr ds:[007A47E0h]
}
void constant0(const Vector* v) { rc::constant0_dispatch_007a6820(&v->x, 1.0f); }   // push 3F800000h; call [007A6820h]
void constant1(const Vector* v) { rc::constant1_dispatch_007a47dc(&v->x, 1.0f); }   // push 3F800000h; call [007A47DCh]
void env(U index, float x, float y, float z, float w) { gl_program_env_parameter_0044f820(index, x, y, z, w); }
// `cmp [0078E5EC],0 ; ... je ; call [007BB850] | call [007BB7AC]` with target 8620h.
void program_param(U index, float x, float y, float z, float w) {
    if (programs_arb()) procs::glProgramEnvParameter4fARB_007bb850(kVertexProgram, index, x, y, z, w);
    else procs::glProgramParameter4fNV_007bb7ac(kVertexProgram, index, x, y, z, w);
}
void enable(VertexProgram* program, void* part, U8 lighting) { program->Enable_004a24d0(part, lighting); }
VertexProgram* program_in(VertexProgram* volatile const& cell) { return cell; }
// Static `new VertexProgram(text, *params)` under guard bit `bit` of 0083042C:
//   test [0083042C],bit ; jne ; or [0083042C],bit ; push 201Ch ; call 006FA7E6 ; test eax,eax ; je (xor eax,eax)
//   ; mov ecx,[0078E5EC] ; select text ; push [params] ; push text ; call 004A27D0 ; mov [slot],eax
template<class ParamsCell>
void static_program(U bit, VertexProgram* volatile& slot, char* arb_text, char* nv_text, ParamsCell& params) {
    if ((vc::statics_guard_0083042c & bit) != 0) return;
    vc::statics_guard_0083042c = vc::statics_guard_0083042c | bit;
    void* const memory = live_operator_new(0x201Cu);
    VertexProgram* result = nullptr;
    if (memory != nullptr) {
        char* const text = programs_arb() ? arb_text : nv_text;
        std::uint32_t* const parameters = params;
        result = static_cast<VertexProgram*>(memory)->Construct_004a27d0(text, parameters);
    }
    slot = result;
}
// Disable the three tangent-space client arrays (0048AC80..0048ACBE / 0048B2FA..0048B33B / 0048C1F1..0048C232).
void disable_tangent_arrays() {
    if (programs_arb()) {
        procs::glDisableVertexAttribArrayARB_007bb770(0x0C);
        procs::glDisableVertexAttribArrayARB_007bb770(0x0D);
        procs::glDisableVertexAttribArrayARB_007bb770(0x0E);
    } else {
        gl::glDisableClientState_0073d264(kAttrib12_0073f3b8);
        gl::glDisableClientState_0073d264(kAttrib13_0073f3bc);
        gl::glDisableClientState_0073d264(kAttrib14_0073f3c0);
    }
}
// Part::GetWorld into zero-initialised locals (pos {0,0,0}, quat {1,0,0,0}) then eye = conj(q) * (camera - pos).
void world_and_eye(void* part, Vector* position, Quaternion* orientation, Vector* eye) {
    *orientation = Quaternion{1.0f, 0.0f, 0.0f, 0.0f};
    *position = Vector{0.0f, 0.0f, 0.0f};
    part_table(part)->s00c(part, nullptr, word_of(position), word_of(orientation));
    void* const camera = *cells.current_camera_007bb4e8;
    Vector relative;
    relative.x = S(W(field<float>(camera, 0x7C)) - W(position->x));
    relative.y = S(W(field<float>(camera, 0x80)) - W(position->y));
    relative.z = S(W(field<float>(camera, 0x84)) - W(position->z));
    Quaternion conjugate;
    quaternion_rotate_004a9bb0(eye, quaternion_negate_w_004a9b50(&conjugate, orientation), &relative);
}
// One per-light pass (BLOCK1 0048AFE0..0048B24E, BLOCK2 0048B7D0..0048BA3B, BLOCK3 0048BEEE..0048C159):
//   node_scale = node+8C words, specular = specular colour; constant1 = (node_scale*[0078E648]) x light colour,
//   constant0 = ((specular*[0078E630]) * bump intensity) x light colour; env 0C = light in object space,
//   env 0D = eye, env 0E = (1, 0, 9/(2 r^2), 1); RenderFlat.
void light_pass(void* light, void* primitive, void* material, const Vector& position, const Quaternion& orientation,
                const Vector& eye, const Vector& node_scale, const Vector& specular) {
    Vector light_world;
    std::memcpy(&light_world, at(light, 0x94), sizeof light_world);           // [light+94h..9Ch] raw dwords
    Vector relative;
    relative.x = S(W(light_world.x) - W(position.x));                          // fld ; fsub ; fstp dword
    relative.y = S(W(light_world.y) - W(position.y));
    relative.z = S(W(light_world.z) - W(position.z));
    Quaternion conjugate;
    Vector light_object;
    quaternion_rotate_004a9bb0(&light_object, quaternion_negate_w_004a9b50(&conjugate, &orientation), &relative);
    Vector scaled;
    scaled.x = S(W(node_scale.x) * W(vc::scale_0078e648));                     // fmul [0078E648h] ; fstp dword
    scaled.y = S(W(node_scale.y) * W(vc::scale_0078e648));
    scaled.z = S(W(node_scale.z) * W(vc::scale_0078e648));
    Vector colour;
    vector_multiply_004ab1e0(&colour, static_cast<const Vector*>(at(light, 0x4C)), &scaled);
    Vector zero1{0.0f, 0.0f, 0.0f};
    constant1(vc::gate_00798b48 != 0 ? &colour : &zero1);                     // [00798B48] ? colour : {0,0,0}
    Vector specular_scaled;
    specular_scaled.z = S(W(specular.z) * W(vc::scale_0078e630));
    specular_scaled.y = S(W(specular.y) * W(vc::scale_0078e630));
    specular_scaled.x = S(W(specular.x) * W(vc::scale_0078e630));
    void* const texture = field<void*>(material, 0x10);                       // [[part+44h]+10h]
    const float intensity = texture_table(texture)->s7c(texture, nullptr);    // call [edx+7Ch] ; fstp dword
    Vector specular_intensity;
    specular_intensity.x = S(W(specular_scaled.x) * W(intensity));
    specular_intensity.y = S(W(specular_scaled.y) * W(intensity));
    specular_intensity.z = S(W(specular_scaled.z) * W(intensity));
    Vector product;
    vector_multiply_004ab1e0(&product, static_cast<const Vector*>(at(light, 0x4C)), &specular_intensity);
    std::memcpy(&colour, &product, sizeof colour);                            // 3 dword copies
    Vector zero0{0.0f, 0.0f, 0.0f};
    constant0(vc::gate_00798b4c != 0 ? &colour : &zero0);                     // [00798B4C] ? colour : {0,0,0}
    env(0x0C, light_object.x, light_object.y, light_object.z, 1.0f);
    env(0x0D, eye.x, eye.y, eye.z, 1.0f);
    const double r = W(field<float>(light, 0x58));                            // fld [light+58h]
    double t = r * r;                                                         // fld st(0) ; fmul st,st(1)
    t = t + t;                                                                // fadd st(0),st
    env(0x0E, 1.0f, 0.0f, S(kNine_00742f00 / t), 1.0f);                       // fdivr [00742F00h] ; fstp [esp]
    render_flat_0046f900(primitive);
}
// The "no lights" pass after each light loop: Enable, zero constants, RenderFlat.
void no_light_pass(VertexProgram* program, void* part, U8 lighting, void* primitive) {
    enable(program, part, lighting);
    Vector zero{0.0f, 0.0f, 0.0f};
    constant1(&zero);
    zero = Vector{0.0f, 0.0f, 0.0f};
    constant0(&zero);
    render_flat_0046f900(primitive);
}
void free_list_data(PointerList& list) {
    if (list.data != 0) live_free(as_object(list.data));                     // test ; je ; push ; call 006FA390
}
// atexit thunks 0073BD70 / 0073BD90.
void __cdecl free_bone_positions_0073bd70() {
    if (vc::bone_positions_00830414.data != 0) live_free(as_object(vc::bone_positions_00830414.data));
}
void __cdecl free_bone_orientations_0073bd90() {
    if (vc::bone_orientations_00830420.data != 0) live_free(as_object(vc::bone_orientations_00830420.data));
}
inline aurora_model::MdlNode* mdl(void* node) { return static_cast<aurora_model::MdlNode*>(node); }
} // namespace

extern "C" void __cdecl render_skinned_vp_0048a520(void* primitive) {
    void* const host = field<void*>(primitive, 8);                                         // 0048A545 [esi+8]
    void* const part = as_object(part_table(host)->s050(host, nullptr));                   // 0048A548..0048A54E (ebp)
    void* const node = field<void*>(part, 4);                                              // 0048A550 edi = [ebp+4] ([FB4])
    if (field<U16>(node, 0x182) == 0) return;                                              // 0048A555..0048A563 -> 0048C636
    void* bump_node = nullptr;                                                             // 0048A573 [F1C] = 0
    if (part_table(part)->s02c(part, nullptr) != 0) bump_node = field<void*>(part, 4);     // 0048A569..0048A57C
    primitive_table(primitive)->s018(primitive, nullptr);                                  // 0048A580..0048A584
    float matrix[9];                                                                       // [F108..F128]
    matrix_identity_00437af0(matrix, nullptr);                                             // 0048A587..0048A58E
    if ((vc::statics_guard_0083042c & 1u) == 0) {                                          // 0048A593..0048A59D
        vc::statics_guard_0083042c = vc::statics_guard_0083042c | 1u;                     // 0048A59F
        vc::bone_orientations_00830420.size = 0;                                           // 0048A5AB
        vc::bone_orientations_00830420.capacity = 0;                                       // 0048A5B1
        vc::bone_orientations_00830420.data = 0;                                           // 0048A5B7
        std::atexit(free_bone_orientations_0073bd90);                                      // 0048A5A6 / 0048A5BD
    }
    if ((vc::statics_guard_0083042c & 2u) == 0) {                                          // 0048A5CC..0048A5D3
        vc::statics_guard_0083042c = vc::statics_guard_0083042c | 2u;                     // 0048A5D5
        vc::bone_positions_00830414.size = 0;                                              // 0048A5E1
        vc::bone_positions_00830414.capacity = 0;                                          // 0048A5E7
        vc::bone_positions_00830414.data = 0;                                              // 0048A5ED
        std::atexit(free_bone_positions_0073bd70);                                         // 0048A5DC / 0048A5F3
    }
    gl::glEnable_0073d390(programs_arb() ? kVertexProgram : rc::vertex_enum_0078d46c);     // 0048A602..0048A617
    program_param(0x10, 0.0f, F(kTwo), F(kThree), F(kOne));                                // 0048A61D..0048A644
    {
        void* const s = scene();                                                           // 0048A64A
        const U fog_flag = field<U>(s, 0x60) != 0 ? kOne : 0u;                             // 0048A64F..0048A65F [F80]
        env(0x11, field<float>(s, 0x64), field<float>(s, 0x68), F(fog_flag), F(kHalf));   // 0048A66A..0048A681
    }
    if (field<U8>(field<void*>(host, 0x40), 0x13) != 0) {                                  // 0048A686..0048A69D
        void* root = part;                                                                 // 0048A69B eax = ebp
        while (field<void*>(root, 0x3C) != nullptr) root = field<void*>(root, 0x3C);       // 0048A6A3..0048A6B7
        vc::bone_orientations_00830420.size = 0;                                           // 0048A74A
        vc::bone_positions_00830414.size = 0;                                              // 0048A750
        gather_bones_00488930(root, &vc::bone_positions_00830414, &vc::bone_orientations_00830420,
                              Vector{0.0f, 0.0f, 0.0f}, Quaternion{1.0f, 0.0f, 0.0f, 0.0f}); // 0048A6B9..0048A759
        field<U8>(field<void*>(field<void*>(primitive, 8), 0x40), 0x13) = 0;               // 0048A75E..0048A767
    }
    // Inverse world transform of the part: (inverse_position [FE0], inverse_orientation [FF8]).
    Vector negated;                                                                        // [F30]
    negated.x = -field<float>(part, 0x08);                                                 // 0048A76B..0048A790 fchs
    negated.y = -field<float>(part, 0x0C);
    negated.z = -field<float>(part, 0x10);
    Quaternion conjugate;                                                                  // [F70]
    Vector inverse_position;                                                               // [FE0]
    quaternion_rotate_004a9bb0(&inverse_position,
        quaternion_negate_w_004a9b50(&conjugate, static_cast<const Quaternion*>(at(part, 0x14))), &negated); // 0048A794..0048A7A5
    Quaternion inverse_orientation;                                                        // [FF8]
    quaternion_negate_w_004a9b50(&inverse_orientation, static_cast<const Quaternion*>(at(part, 0x14))); // 0048A7AA..0048A7B3
    for (void* parent = field<void*>(part, 0x3C); parent != nullptr; parent = field<void*>(parent, 0x3C)) { // 0048A7B8..0048A8B5
        negated.x = -field<float>(parent, 0x08);                                           // 0048A7C6..0048A7EB
        negated.y = -field<float>(parent, 0x0C);
        negated.z = -field<float>(parent, 0x10);
        Vector local;                                                                      // [FEC]
        quaternion_rotate_004a9bb0(&local,
            quaternion_negate_w_004a9b50(&conjugate, static_cast<const Quaternion*>(at(parent, 0x14))), &negated); // 0048A7EF..0048A800
        Vector rotated;                                                                    // [F10]
        quaternion_rotate_004a9bb0(&rotated, &inverse_orientation, &local);                // 0048A805..0048A813
        const float sx = S(W(inverse_position.x) + W(rotated.x));                          // 0048A82D..0048A836
        const float sy = S(W(inverse_position.y) + W(rotated.y));                          // 0048A823..0048A845
        const float sz = S(W(inverse_position.z) + W(rotated.z));                          // 0048A818..0048A854
        inverse_position = Vector{sx, sy, sz};                                             // 0048A83E / 0048A85D / 0048A864
        Quaternion parent_conjugate;                                                       // [FD0]
        quaternion_negate_w_004a9b50(&parent_conjugate, static_cast<const Quaternion*>(at(parent, 0x14))); // 0048A822..0048A86B
        Quaternion product;                                                                // [F84]
        quaternion_multiply_004a9ac0(&product, &inverse_orientation, &parent_conjugate);   // 0048A870..0048A881
        std::memcpy(&inverse_orientation, &product, sizeof product);                       // 0048A886..0048A8A6
    }
    // Bone matrices: program parameters 18 + 3*index (rows of the 3x3 rotation, translation in w).
    for (I i = 0; i < field<I>(node, 0x1B4); ++i) {                                        // 0048A8BB..0048AB50 (count re-read)
        const I index = vp_ftol2(W(field<float*>(node, 0x1B0)[i]));                        // 0048A8E0..0048A8F1 [F20]
        if (index == -1) continue;                                                         // 0048A8EE..0048A8F5
        const Vector* const position = static_cast<const Vector*>(
            as_object(vc::bone_positions_00830414.data + static_cast<U>(i) * 12u));        // 0048A8FB..0048A90B
        Vector r1;                                                                         // [FEC]
        quaternion_rotate_004a9bb0(&r1, &inverse_orientation, position);                   // 0048A90C..0048A91C
        const float t1z = S(W(inverse_position.z) + W(r1.z));                              // 0048A921..0048A93A [F98]
        const float t1y = S(W(inverse_position.y) + W(r1.y));                              // 0048A942..0048A953 [F94]
        const float t1x = S(W(inverse_position.x) + W(r1.x));                              // 0048A95A..0048A96D [F84]
        const Quaternion* const bone_orientation = static_cast<const Quaternion*>(
            as_object(vc::bone_orientations_00830420.data + static_cast<U>(i) * 16u));     // 0048A963..0048A96B
        Vector r2a;                                                                        // [FA8]
        quaternion_rotate_004a9bb0(&r2a, bone_orientation,
            static_cast<const Vector*>(at(field<void*>(node, 0x1C4), static_cast<U>(i) * 12u))); // 0048A92B..0048A974
        Vector r2;                                                                         // [F9C]
        quaternion_rotate_004a9bb0(&r2, &inverse_orientation, &r2a);                       // 0048A979..0048A98A
        const float tz = S(W(t1z) + W(r2.z));                                              // 0048A98F..0048A9A4 [F98]
        const float ty = S(W(t1y) + W(r2.y));                                              // 0048A9AC..0048A9BD [F94]
        const float tx = S(W(t1x) + W(r2.x));                                              // 0048A9C4..0048A9D7 [F10]
        Quaternion q1;                                                                     // [FD0]
        quaternion_multiply_004a9ac0(&q1, &inverse_orientation, bone_orientation);         // 0048A9D5..0048A9E3
        Quaternion rotation;                                                               // [F70]
        quaternion_multiply_004a9ac0(&rotation, &q1,
            static_cast<const Quaternion*>(at(field<void*>(node, 0x1B8), static_cast<U>(i) * 16u))); // 0048A999..0048A9F1
        float scratch[9];                                                                  // [F14C]
        std::memcpy(matrix, quaternion_get_matrix_0044f470(&rotation, nullptr, scratch), sizeof matrix); // 0048A9F9..0048AA1C
        const U base = static_cast<U>(index) * 3u + 0x12u;                                 // 0048AA1E lea esi,[eax+eax*2+12h]
        program_param(base, matrix[0], matrix[3], matrix[6], tx);                          // 0048AA22..0048AA79
        program_param(base + 1, matrix[1], matrix[4], matrix[7], ty);                      // 0048AA7F..0048AADD
        program_param(base + 2, matrix[2], matrix[5], matrix[8], tz);                      // 0048AAE3..0048AB1E
    }
    auto material_now = [part]() { return field<void*>(part, 0x44); };                    // mov ecx,[ebp+44h] (re-read per use)
    void* const nodes = bump_node;                                                          // edi = [F1C] (0048AB7C / 0048AD4F)
    // ---------------------------------------------------------- bumpy-shiny route 0048ABA9..0048AD4A
    if (aurora_material_has_bump_map_0047b8f0(material_now(), nullptr) != 0 &&                   // 0048AB56..0048AB66
        material_has_bumpy_shiny_map_0047b0c0(material_now(), nullptr) != 0 &&                   // 0048AB6C..0048AB76
        aurora_trimesh_has_tangent_bases_0043e410(mdl(nodes), nullptr) != 0 &&             // 0048AB7C..0048AB89
        kotor_renderer_gl::BumpyShinyAvailable_0045f680() != 0 &&                          // 0048AB8F..0048AB96
        vc::gate_0078e66c != 0) {                                                          // 0048AB9C..0048ABA3
        gl::glPushAttrib_0073d268(0x6040);                                                 // 0048ABA9..0048ABAE
        gl::glEnable_0073d390(*cells.gl_blend_0078d448);                                   // 0048ABB4..0048ABC0
        gl::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                             // 0048ABC2..0048ABC9
        if (vc::gate_00798b54 != 0) {                                                      // 0048ABCB..0048ABD2
            gl_blend_func_indexed_00401670(2, 3);                                          // 0048ABD8..0048ABDC
            gl::glDisableClientState_0073d264(kNormalArray_0073f39c);                      // 0048ABE1..0048ABF1
            env(0x0F, 1.0f, 0.0f, 1.0f, 0.0f);                                             // 0048ABF3..0048AC03
            enable(static_cast<VertexProgram*>(renderer_program_cells::output_0083037C), part, 0); // 0048AC08..0048AC14
            dispatch(0x1D, tex(material_now(), 0x10), nullptr, nullptr, tex(material_now(), 0x14));    // 0048AC19..0048AC2A
            dispatch(0x18, nullptr, nullptr, nullptr, nullptr);                            // 0048AC30..0048AC3A
            render_flat_0046f900(primitive);                                               // 0048AC40..0048AC48
            dispatch(0, nullptr, nullptr, nullptr, nullptr);                               // 0048AC4D..0048AC57
            dispatch(0x1C, nullptr, nullptr, nullptr, nullptr);                            // 0048AC60..0048AC6A
            dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);                            // 0048AC70..0048AC7A
            disable_tangent_arrays();                                                      // 0048AC80..0048ACBE
            gl::glEnableClientState_0073d260(kNormalArray_0073f39c);                       // 0048ACC0..0048ACC7
        }
        gl::glPopAttrib_0073d26c();                                                        // 0048ACCD
        env(0x0F, 1.0f, 1.0f, 1.0f, 1.0f);                                                 // 0048ACD3..0048ACE9
        void* const program = vc::select_00830378 != 0 ? kotor_aurora_window::output_00830384  // 0048ACEE..0048AD03
                                                       : renderer_program_cells::output_00830380;
        enable(static_cast<VertexProgram*>(program), part, 1);                             // 0048ACFE..0048AD09
        if (vc::gate_0078e650 != 0) {                                                      // 0048AD0E..0048AD15
            gl_render_activate_texture_stage_00425250(0);                                  // 0048AD17..0048AD19
            material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);           // 0048AD1E..0048AD24
            gl_blend_func_indexed_00401670(2, 1);                                          // 0048AD29..0048AD2D
            render_flat_0046f900(primitive);                                               // 0048AD32..0048AD3A
        }
        gl::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                              // 0048AD42..0048AD48
    }
    // ---------------------------------------------------------- BLOCK1: bump-mapped NV2x route 0048ADA6..0048B52F
    else if (aurora_material_has_bump_map_0047b8f0(material_now(), nullptr) != 0 &&              // 0048AD53..0048AD5D
             get_ati_fragment_shaders_disabled_0045f5a0() != 0 &&                          // 0048AD63..0048AD6A
             aurora_trimesh_has_tangent_bases_0043e410(mdl(nodes), nullptr) != 0 &&        // 0048AD70..0048AD79
             aur_nv2x_level_shaders_available_0045fbd0() &&                                // 0048AD7F..0048AD86
             vc::gate_0078e5fc != 0 && vc::gate_00798b50 != 0) {                           // 0048AD8C..0048ADA0
        gl::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                             // 0048ADA6..0048ADAD
        static_program(4u, vc::skinned_program_00830410, vd::arb_skin_vp_00796020,          // 0048ADAF..0048AE13
                       vd::nv_skin_vp_0079d720, vc::parameters_00830588);
        Vector node_scale{0.0f, 0.0f, 0.0f};                                               // [F54..F5C] 0048AE20..0048AE30
        Vector specular{0.0f, 0.0f, 0.0f};                                                 // [F60..F68] 0048AE38..0048AE48
        if (nodes != nullptr) {                                                            // 0048AE1E / 0048AE50
            Vector out;                                                                    // [FEC]
            const Vector* const colour =
                material_get_bump_map_specular_color_00471430(field<void*>(part, 0x44), nullptr, &out); // 0048AE52..0048AE5D
            std::memcpy(&specular, colour, sizeof specular);                               // 0048AE62..0048AE7D
            std::memcpy(&node_scale, at(nodes, 0x8C), sizeof node_scale);                  // 0048AE6A..0048AE8C
        }
        PointerList lights;                                                                // [F48..F50]
        int_list_copy_construct_0046d250(&lights, nullptr,
            light_manager_get_bump_map_lights_004a28a0(light_manager(), nullptr));         // 0048AE90..0048AEA6
        dispatch(6, tex(material_now(), 0x10), field<texture::Basic*>(scene(), 0xA4),           // 0048AEAB..0048AED0
                 field<texture::Basic*>(scene(), 0xA4), tex(material_now(), 0));
        constant0(static_cast<const Vector*>(at(scene(), 0x88)));                          // 0048AED6..0048AEE6
        Vector position;                                                                   // [F24]
        Quaternion orientation;                                                            // [F70]
        Vector eye;                                                                        // [F10]
        world_and_eye(part, &position, &orientation, &eye);                                // 0048AEEC..0048AF8B
        gl::glEnable_0073d390(rc::vertex_enum_0078d46c);                                   // 0048AF90..0048AF99
        gl_blend_func_indexed_00401670(2, 3);                                              // 0048AF9F..0048AFA3
        lights_disable();                                                                  // 0048AFA8..0048AFB9
        gl::glDisable_0073d22c(*cells.gl_fog_0078d450);                                    // 0048AFBC..0048AFC2
        for (I i = 0; i < lights.size; ++i) {                                              // 0048AFC4..0048B25D
            enable(program_in(vc::skinned_program_00830410), part, 0);                     // 0048AFD2..0048AFDB
            light_pass(field<void*>(as_object(lights.data), static_cast<U>(i) * 4u), primitive, // 0048AFE0..0048B24E
                       field<void*>(part, 0x44), position, orientation, eye, node_scale, specular);
        }
        if (lights.size == 0)                                                              // 0048B263 / 0048B265
            no_light_pass(program_in(vc::skinned_program_00830410), part, 0, primitive);   // 0048B26B..0048B2E9
        disable_tangent_arrays();                                                          // 0048B2FA..0048B33B
        gl::glBlendFunc_0073d38c(kGlOne_0073f278, kGlOne_0073f278);                        // 0048B33D..0048B34A
        dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);                                // 0048B34C..0048B356
        dispatch(0, nullptr, nullptr, nullptr, nullptr);                                   // 0048B35C..0048B366
        env(0x0F, 1.0f, 1.0f, 1.0f, 1.0f);                                                 // 0048B36C..0048B382
        static_program(8u, vc::skinned_program_0083040c, vd::arb_skin_vp_00792658,         // 0048B387..0048B3EF
                       vd::nv_skin_vp_0079b5a8, vc::parameters_00830588);
        enable(program_in(vc::skinned_program_0083040c), part, 1);                         // 0048B3F7..0048B400
        if (vc::gate_0078e650 != 0) {                                                      // 0048B405..0048B40C
            gl_render_activate_texture_stage_00425250(0);                                  // 0048B40E..0048B410
            material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);           // 0048B415..0048B41B
            render_flat_0046f900(primitive);                                               // 0048B420..0048B421
        }
        gl::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                              // 0048B429..0048B42F
        lights_enable();                                                                   // 0048B435..0048B443
        scene_do_fog();                                                                    // 0048B446..0048B44E
        gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);      // 0048B454..0048B462
        if (field<U>(scene(), 0x60) != 0) {                                                // 0048B464..0048B46E
            // NV route constructs with a NULL source (neg/sbb/and 7931C8h, 0048B4B4..0048B4B9): original quirk kept.
            static_program(0x10u, vc::skinned_program_00830408, vd::arb_skin_vp_007931c8,   // 0048B474..0048B4D0
                           static_cast<char*>(nullptr), vc::parameters_00830588);
            enable(program_in(vc::skinned_program_00830408), part, 0);                     // 0048B4D8..0048B4E1
            dispatch(2, nullptr, nullptr, nullptr, nullptr);                               // 0048B4E6..0048B4F0
            render_flat_0046f900(primitive);                                               // 0048B4F6..0048B4F7
            dispatch(0, nullptr, nullptr, nullptr, nullptr);                               // 0048B4FC..0048B506
        }
        free_list_data(lights);                                                            // 0048B50F..0048B527
    }
    // ---------------------------------------------------------- BLOCK2: linear-controller NV2x route 0048B578..0048BC5B
    else if (texture_controller_is_linear_0041f250(material_now(), nullptr) != 0 &&             // 0048B534..0048B53E
             get_ati_fragment_shaders_disabled_0045f5a0() != 0 &&                          // 0048B544..0048B54B
             aur_nv2x_level_shaders_available_0045fbd0() &&                                // 0048B551..0048B558
             vc::gate_0078e5fc != 0 && vc::gate_00798b50 != 0) {                           // 0048B55E..0048B572
        gl::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                             // 0048B578..0048B57E
        // Source 007978B0 on both routes (no [0078E5EC] test at 0048B5BD..0048B5C4).
        static_program(0x20u, vc::skinned_program_00830404, vd::arb_skin_vp_007978b0,       // 0048B580..0048B5D9
                       vd::arb_skin_vp_007978b0, vc::parameters_00830294);
        Vector specular{0.0f, 0.0f, 0.0f};                                                 // [F54..F5C] 0048B5FE..0048B60E
        Vector node_scale{0.0f, 0.0f, 0.0f};                                               // [F60..F68] 0048B5E6..0048B5F6
        if (nodes != nullptr) {                                                            // 0048B5E4 / 0048B616
            void* const texture = field<void*>(field<void*>(part, 0x44), 0x10);           // 0048B618..0048B61B
            Vector out;                                                                    // [F10]
            texture_table(texture)->s88(texture, nullptr, &out);                           // 0048B61E..0048B625 (RET 4)
            std::memcpy(&specular, &out, sizeof specular);                                 // 0048B62B..0048B64A
            std::memcpy(&node_scale, at(nodes, 0x8C), sizeof node_scale);                  // 0048B637..0048B659
        }
        PointerList lights;                                                                // [F48..F50]
        int_list_copy_construct_0046d250(&lights, nullptr,
            light_manager_get_bump_map_lights_004a28a0(light_manager(), nullptr));         // 0048B65D..0048B673
        dispatch(7, tex(material_now(), 0), tex(material_now(), 0x10), field<texture::Basic*>(scene(), 0xA4), // 0048B678..0048B69D
                 field<texture::Basic*>(scene(), 0xA4));
        constant0(static_cast<const Vector*>(at(scene(), 0x88)));                          // 0048B6A3..0048B6B3
        gl_render_activate_texture_stage_00425250(1);                                      // 0048B6B9..0048B6BB
        gl::glEnableClientState_0073d260(kTexCoordArray_0073f398);                         // 0048B6C0..0048B6C9
        gl_render_activate_texture_stage_00425250(0);                                      // 0048B6CF..0048B6D1
        Vector position;                                                                   // [F3C]
        Quaternion orientation;                                                            // [F70]
        Vector eye;                                                                        // [FB8]
        world_and_eye(part, &position, &orientation, &eye);                                // 0048B6D6..0048B778
        gl::glEnable_0073d390(rc::vertex_enum_0078d46c);                                   // 0048B77D..0048B786
        gl::glBlendFunc_0073d38c(kGlOne_0073f278, kGlZero_0073f27c);                       // 0048B78C..0048B79A
        lights_disable();                                                                  // 0048B7A0..0048B7AD
        void* const list_data = as_object(lights.data);                                    // 0048B7B4 edi = [F48]
        for (I i = 0; i < lights.size; ++i) {                                              // 0048B7B0..0048BA4A
            enable(program_in(vc::skinned_program_00830404), part, 0);                     // 0048B7C2..0048B7CB
            light_pass(field<void*>(list_data, static_cast<U>(i) * 4u), primitive,         // 0048B7D0..0048BA3B
                       field<void*>(part, 0x44), position, orientation, eye, node_scale, specular);
        }
        if (lights.size == 0)                                                              // 0048BA50..0048BA58
            no_light_pass(program_in(vc::skinned_program_00830404), part, 0, primitive);   // 0048BA5A..0048BAA7
        if (kotor_renderer_gl::MultitextureAvailable_0045f710() != 0)                      // 0048BAAF..0048BAB6
            procs::glClientActiveTextureARB_007bb6b0(0x84C1);                              // 0048BAB8..0048BABD
        rc::current_stage_007a6898 = 1;                                                    // 0048BACA
        gl::glDisableClientState_0073d264(kTexCoordArray_0073f398);                        // 0048BAC3..0048BAD4
        if (kotor_renderer_gl::MultitextureAvailable_0045f710() != 0)                      // 0048BADA..0048BAE1
            procs::glClientActiveTextureARB_007bb6b0(0x84C0);                              // 0048BAE3..0048BAE8
        rc::current_stage_007a6898 = 0;                                                    // 0048BAF4
        scene_do_fog();                                                                    // 0048BAEE..0048BAFC
        gl::glBlendFunc_0073d38c(kGlOne_0073f278, kGlOne_0073f278);                        // 0048BB02..0048BB0F
        dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);                                // 0048BB11..0048BB17
        dispatch(0, nullptr, nullptr, nullptr, nullptr);                                   // 0048BB1D..0048BB22
        program_param(0x0F, 1.0f, 1.0f, 1.0f, 1.0f);                                       // 0048BB28..0048BB57
        static_program(0x40u, vc::skinned_program_00830400, vd::arb_skin_vp_00792658,       // 0048BB5D..0048BBC1
                       vd::nv_skin_vp_0079b5a8, vc::parameters_00830294);
        enable(program_in(vc::skinned_program_00830400), part, 1);                         // 0048BBC9..0048BBD2
        if (vc::gate_0078e650 != 0) {                                                      // 0048BBD7..0048BBDD
            if (kotor_renderer_gl::MultitextureAvailable_0045f710() != 0)                  // 0048BBDF..0048BBE6
                procs::glClientActiveTextureARB_007bb6b0(0x84C0);                          // 0048BBE8..0048BBED
            rc::current_stage_007a6898 = 0;                                                // 0048BBF3
            material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);           // 0048BBF9..0048BBFC
            render_flat_0046f900(primitive);                                               // 0048BC01..0048BC09
        }
        gl::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                              // 0048BC11..0048BC18
        lights_enable();                                                                   // 0048BC1E..0048BC2C
        gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);      // 0048BC2F..0048BC3D
        if (list_data != nullptr) live_free(list_data);                                    // 0048BC3F..0048BC53 (edi)
    }
    // ---------------------------------------------------------- BLOCK3: bump-mapped per-light lighting route 0048BCA6..0048C294
    else if (aurora_material_has_bump_map_0047b8f0(material_now(), nullptr) != 0 &&              // 0048BC60..0048BC6A
             get_ati_fragment_shaders_disabled_0045f5a0() != 0 &&                          // 0048BC70..0048BC77
             aurora_trimesh_has_tangent_bases_0043e410(mdl(nodes), nullptr) != 0 &&        // 0048BC7D..0048BC86
             aur_nv2x_level_shaders_available_0045fbd0() &&                                // 0048BC8C..0048BC93
             vc::gate_0078e5fc != 0) {                                                     // 0048BC99..0048BCA0
        static_program(0x80u, vc::skinned_program_008303fc, vd::arb_skin_vp_00795170,       // 0048BCA6..0048BD0F
                       vd::nv_skin_vp_0079ce58, vc::parameters_00830588);
        Vector specular{0.0f, 0.0f, 0.0f};                                                 // [F54..F5C] 0048BD34..0048BD44
        Vector node_scale{0.0f, 0.0f, 0.0f};                                               // [F60..F68] 0048BD1C..0048BD2C
        if (nodes != nullptr) {                                                            // 0048BD1A / 0048BD4C
            void* const texture = field<void*>(field<void*>(part, 0x44), 0x10);           // 0048BD4E..0048BD51
            Vector out;                                                                    // [F10]
            texture_table(texture)->s88(texture, nullptr, &out);                           // 0048BD54..0048BD5B (RET 4)
            std::memcpy(&specular, &out, sizeof specular);                                 // 0048BD61..0048BD80
            std::memcpy(&node_scale, at(nodes, 0x8C), sizeof node_scale);                  // 0048BD6D..0048BD8F
        }
        PointerList lights;                                                                // [F48..F50]
        int_list_copy_construct_0046d250(&lights, nullptr,
            light_manager_get_bump_map_lights_004a28a0(light_manager(), nullptr));         // 0048BD93..0048BDA8
        dispatch(6, tex(material_now(), 0x10), field<texture::Basic*>(scene(), 0xA4),           // 0048BDAD..0048BDD2
                 field<texture::Basic*>(scene(), 0xA4), tex(material_now(), 0));
        constant0(static_cast<const Vector*>(at(scene(), 0x88)));                          // 0048BDD8..0048BDE8
        Vector position;                                                                   // [F3C]
        Quaternion orientation;                                                            // [F70]
        Vector eye;                                                                        // [FB8]
        world_and_eye(part, &position, &orientation, &eye);                                // 0048BDEE..0048BE90
        gl::glEnable_0073d390(rc::vertex_enum_0078d46c);                                   // 0048BE95..0048BE9E
        gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);      // 0048BEA4..0048BEB2
        void* const list_data = as_object(lights.data);                                    // 0048BEBC ebx = [F48]
        for (I i = 0; i < lights.size; ++i) {                                              // 0048BEB8..0048C179
            lights_disable();                                                              // 0048BED0..0048BEDD
            enable(program_in(vc::skinned_program_008303fc), part, 1);                     // 0048BEE0..0048BEE9
            light_pass(field<void*>(list_data, static_cast<U>(i) * 4u), primitive,         // 0048BEEE..0048C159
                       field<void*>(part, 0x44), position, orientation, eye, node_scale, specular);
            lights_enable();                                                               // 0048C15E..0048C16F
        }
        if (lights.size == 0)                                                              // 0048C17F / 0048C181
            no_light_pass(program_in(vc::skinned_program_008303fc), part, 1, primitive);   // 0048C183..0048C1E9
        disable_tangent_arrays();                                                          // 0048C1F1..0048C232
        scene_do_fog();                                                                    // 0048C234..0048C23C
        gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);      // 0048C242..0048C24F
        dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);                                // 0048C255..0048C25F
        dispatch(0, nullptr, nullptr, nullptr, nullptr);                                   // 0048C265..0048C26F
        if (list_data != nullptr) live_free(list_data);                                    // 0048C278..0048C28C (ebx)
    }
    // ---------------------------------------------------------- environment-mapped skin route 0048C299..0048C3FD
    else if ((field<void*>(part, 0x40) != nullptr && field<void*>(field<void*>(part, 0x40), 0x1A8) != nullptr) || // 0048C299..0048C2A8
             material_has_env_map_0047b0e0(field<void*>(part, 0x44), nullptr) != 0) {     // 0048C2AA..0048C2B4
        void* environment = nullptr;
        if (field<void*>(part, 0x40) != nullptr) environment = field<void*>(field<void*>(part, 0x40), 0x1A8); // 0048C2BA..0048C2C9
        if (environment == nullptr) environment = field<void*>(field<void*>(part, 0x44), 0x14);              // 0048C2CB..0048C2CE
        VertexProgram* const program = texture_table(environment)->s0c(environment, nullptr) != 0  // 0048C2D1..0048C2DA
            ? get_skin_cube_map_vp_00487460()                                              // 0048C2DC
            : get_skin_sphere_map_vp_004873c0();                                           // 0048C2E3 (ebx)
        if (gl::ATIValidate_0045f640()) {                                                  // 0048C2EA..0048C2F1
            if (field<U>(scene(), 0x60) == 0) {                                            // 0048C2F7..0048C301
                // glFogfv(GL_FOG_COLOR) reads 4 floats from [F10]: three zeros (0048C30F..0048C31F) and the 4th DWORD
                // [F1C] = the bump-node word stored at 0048A573/0048A57C (never rewritten on this route).
                float colour[4] = {0.0f, 0.0f, 0.0f, bits_to_float(word_of(bump_node))};
                gl::glFogfv_0073d354(*cells.gl_fog_color_0078d490, colour);                // 0048C303..0048C327
                gl::glFogf_0073d350(*cells.gl_fog_start_0078d484, 1000.0f);                // 0048C32D..0048C33E 447A0000h
                gl::glFogf_0073d350(*cells.gl_fog_end_0078d488, 2000.0f);                  // 0048C340..0048C34C 44FA0000h
            }
            gl::glDisable_0073d22c(*cells.gl_fog_0078d450);                                // 0048C34E..0048C355
            void* const gob = field<void*>(part, 0x40);                                    // 0048C357 (no null test: original)
            const U constant[4] = {field<U>(field<void*>(gob, 0x68), 0x74),                // 0048C35A..0048C360
                                   field<U>(field<void*>(gob, 0x68), 0x78),                // 0048C364..0048C36A
                                   field<U>(field<void*>(gob, 0x68), 0x7C), kOne};         // 0048C36E..0048C385
            float constant_f[4];
            std::memcpy(constant_f, constant, sizeof constant_f);
            procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, constant_f);            // 0048C374..0048C390
        }
        enable(program, part, 1);                                                          // 0048C396..0048C39B
        const U ati = gl::ATIValidate_0045f640() ? 1u : 0u;                                // 0048C3A0..0048C3C0
        render_environment_mapped_0046ff70(primitive, 1, ati);                             // 0048C3A9..0048C3C3
        if (gl::ATIValidate_0045f640()) {                                                  // 0048C3CB..0048C3D2
            dispatch(0, tex(field<void*>(part, 0x44), 0), nullptr, nullptr, nullptr);      // 0048C3D8..0048C3E6
            scene_do_fog();                                                                // 0048C3EC..0048C3F7
        }
    }
    // ---------------------------------------------------------- default skin route 0048C402..0048C613
    else {
        if (kotor_renderer_gl::MultitextureAvailable_0045f710() != 0)                      // 0048C402..0048C409
            procs::glClientActiveTextureARB_007bb6b0(0x84C0);                              // 0048C40B..0048C410
        rc::current_stage_007a6898 = 0;                                                    // 0048C416
        material_bind_texture0_0047adf0(field<void*>(part, 0x44), nullptr);               // 0048C420..0048C423
        if (gl::ATIValidate_0045f640() && field<U>(scene(), 0x60) != 0) {                  // 0048C428..0048C43B
            gl::glDisable_0073d22c(*cells.gl_fog_0078d450);                                // 0048C43D..0048C444
            dispatch(3, tex(field<void*>(part, 0x44), 0), nullptr, nullptr, nullptr);      // 0048C446..0048C454
            void* const gob = field<void*>(part, 0x40);                                    // 0048C45A
            const U constant[4] = {field<U>(field<void*>(gob, 0x68), 0x74),                // 0048C45D..0048C463
                                   field<U>(field<void*>(gob, 0x68), 0x78),                // 0048C46A..0048C470
                                   field<U>(field<void*>(gob, 0x68), 0x7C), kOne};         // 0048C477..0048C494
            float constant_f[4];
            std::memcpy(constant_f, constant, sizeof constant_f);
            procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, constant_f);            // 0048C480..0048C49F
        }
        VertexProgram* program;
        if (vc::select_00830378 != 0) {                                                    // 0048C4A5..0048C4AC
            static_program(0x100u, vc::skinned_program_008303f8, renderer_program_cells::source_0079C310, // 0048C4AE..0048C50B
                           renderer_program_cells::source_0079C310, renderer_program_cells::parameters_00830580);
            program = program_in(vc::skinned_program_008303f8);                            // 0048C516
        } else {
            program = get_skin_vp_00487310();                                              // 0048C51E..0048C523
        }
        enable(program, part, 1);                                                          // 0048C525..0048C528
        if (aurora_material_has_bump_map_0047b8f0(field<void*>(part, 0x44), nullptr) != 0 && // 0048C52D..0048C537
            aurora_trimesh_has_tangent_bases_0043e410(mdl(nodes), nullptr) != 0 &&         // 0048C539..0048C542
            !aur_nv2x_level_shaders_available_0045fbd0()) {                                // 0048C544..0048C54B
            gl::glBlendFunc_0073d38c(kGlOne_0073f278, kGlZero_0073f27c);                   // 0048C54D..0048C560
            render_flat_0046f900(primitive);                                               // 0048C562..0048C56A
            gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);  // 0048C56F..0048C57F
        } else if (aurora_material_has_bump_map_0047b8f0(field<void*>(part, 0x44), nullptr) != 0 && // 0048C583..0048C58D
                   aurora_trimesh_has_tangent_bases_0043e410(mdl(nodes), nullptr) != 0 &&  // 0048C58F..0048C598
                   get_ati_fragment_shaders_disabled_0045f5a0() == 0) {                    // 0048C59A..0048C5A1
            gl::glBlendFunc_0073d38c(kGlOne_0073f278, kGlZero_0073f27c);                   // 0048C5A3..0048C5B7
            render_flat_0046f900(primitive);                                               // 0048C5B9..0048C5C1
            gl::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274);  // 0048C5C6..0048C5D7
        } else {
            render_flat_0046f900(primitive);                                               // 0048C5DB..0048C5E3
        }
        if (gl::ATIValidate_0045f640()) {                                                  // 0048C5EB..0048C5F2
            dispatch(0, tex(field<void*>(part, 0x44), 0), nullptr, nullptr, nullptr);      // 0048C5F4..0048C602
            scene_do_fog();                                                                // 0048C608..0048C613
        }
    }
    gl::glDisable_0073d22c(programs_arb() ? kVertexProgram : rc::vertex_enum_0078d46c);    // 0048C619..0048C630
}
