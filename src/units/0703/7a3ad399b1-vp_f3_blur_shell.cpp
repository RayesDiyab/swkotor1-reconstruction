// vp_f3_blur_shell.cpp - lane-3d-vp fork F3 (SUBLANES "vp_nv_ati"):
//   0048C650 RenderSkinnedVPBlur            cdecl(VertexPrimitive*), plain RET (callers 0048D6C0: add esp,4)
//   0048D040 PartSkin::DrawBumpedOutShellVP thiscall(PartSkin*), RET 0 (tail-jumped from 0048D69F)
// swkotor.exe GOG K1 (base 00400000). Every statement cites the instruction(s) it reproduces.
// Object views (offsets observed in these two bodies; no layout is published for them):
//   VertexPrimitive: +08 Part (0048C673 / 0048D0AF); vtable VertexPrimitive (lane-3d-model VTable_00742f74 prefix)
//   Part (PartSkin):  +04 MdlNodeSkin, +08..+10 local position, +14 local orientation (w,x,y,z), +3C parent Part,
//                     +40 owner object, +44 Material, +70 VertexPrimitive (0048D0A5); vtable lane-3d-model VTable_00740b68
//   MdlNodeSkin:      +182 u16 bone-map count (0048C685), +1B0 float* bone map, +1B4 int bone count,
//                     +1B8 Quaternion* bind orientations, +1C4 Vector* bind positions (0048C950..0048C9D5)
//   owner object:     +78..+80 Vector, +84..+90 Quaternion (w,x,y,z), +94..+9C Vector, +11C float, +180 float
//   Material:         +00 CAurTexture* (0048CFC4), +0C CAurTexture* (0048D08A), +2C int blend state (0048D098)
// D30-FINAL: values the original keeps on the x87 stack are double (W), fstp-dword stores are float (S).
// EH: FuncInfo 007178A8 / 007178DC only reset the guard bits / free a failed 201C allocation (funclets 00717870..,
// 007178C0..); operator new 006FA7E6 returns null (no throw) and no callee here throws, so they are cited, not emitted.
#include <cstdlib>
#include <new>
#include "4bf744a1f4-vp_nv_ati.h"
#include "7ee14b5ec3-model_vtables.hpp"        // lane-3d-model: typed Part / VertexPrimitive tables (D39/D41)
#include "ccf5cb551c-render_shared_lowers.h"   // lane-renderer-bodies: texture::texture_call_bind_b4 ([vtable+B4] carrier)

// ---------------------------------------------------------------- existing owners (exact copies)
namespace aurora_model { struct MdlNode; }
extern "C" {
// lane-3d-mesh trimesh_skin.h (core / animation / trimesh_skin owners)
Quaternion* __cdecl quaternion_multiply_004a9ac0(Quaternion* out, const Quaternion* a, const Quaternion* b);
void __cdecl gather_bones_00488930(void* part, PointerList* positions, PointerList* orientations,
                                   Vector translation, Quaternion rotation);              // cdecl, add esp,28h
void __cdecl render_flat_0046f900(void* primitive);
std::uint8_t __fastcall aurora_material_has_bump_map_0047b8f0(void* material, void* unused_edx); // AL (lane-3d-model spelling)
// lane-3d-emitter emitter.h (core)
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);                                             // returns out
// lane-3d-model model_instance.hpp (aurora_model::U8 == std::uint8_t)
std::uint8_t __fastcall aurora_trimesh_has_tangent_bases_0043e410(aurora_model::MdlNode* self, void* unused_edx);
}

// ---------------------------------------------------------------- cells owned here (BSS, cold 0)
// Full-image xref (lane-gui-draw/exe-text.asm): referenced only by 0048C650 / 0048D040 and their atexit thunks
// 0073BDB0 / 0073BDD0 / 0073BDF0 / 0073BE10 and EH funclets 00717870.. / 007178C0.. (guard words).
namespace kotor_vp_cells {
VertexProgram* volatile blur_program_00830430 = nullptr;              // 0048CB28 / 0048CFD5
kotor_reconstruction::ArrayMutations blur_positions_00830434{};      // CExoArrayList<Vector> 830434/38/3C
kotor_reconstruction::ArrayMutations blur_orientations_00830440{};   // CExoArrayList<Quaternion> 830440/44/48
volatile U blur_guard_0083044c = 0;                                  // bits 1/2/4 (0048C707/0048C741/0048CAD4)
std::uint32_t* volatile parameters_00830598 = nullptr;               // only read (0048CB11), never stored
kotor_reconstruction::ArrayMutations shell_positions_00830450{};     // 830450/54/58
kotor_reconstruction::ArrayMutations shell_orientations_0083045c{};  // 83045C/60/64
volatile U shell_guard_00830468 = 0;                                 // bits 1/2 (0048D0F6/0048D130)
}

namespace {
using namespace kotor_vp;
using namespace kotor_vp_cells;
using kotor_scene3d::cells;
using kotor_reconstruction::ArrayMutations;
using PartTable = aurora_vt::VTable_00740b68;        // common Part prefix (lane-3d-mesh convention)
using PrimitiveTable = aurora_vt::VTable_00742f74;   // VertexPrimitive layout (14 slots)

// .rdata constants (values from the pinned image)
constexpr U kZero_0073d700 = 0x00000000u;            // 0.0f  (0048C6E3 fcomp)
constexpr U kOne_0073d764 = 0x3F800000u;             // 1.0f  (0048CC04 fsubr)
constexpr U kFacing_00742efc = 0x3F100000u;          // 0.5625f (0048CF02 fmul)
constexpr U kGlCw_0073f2c0 = 0x0900u;                // GL_CW  (0048CFAD)
constexpr U kGlCcw_0073f2c4 = 0x0901u;               // GL_CCW (0048D006)
constexpr U kClientArray_0073f3a8 = 0x8651u;         // 0048D649
constexpr U kClientArray_0073f3b0 = 0x8654u;         // 0048D658

inline const PartTable* part_vt(void* part) { return static_cast<const PartTable*>(field<void*>(part, 0)); }
inline const PrimitiveTable* primitive_vt(void* p) { return static_cast<const PrimitiveTable*>(field<void*>(p, 0)); }
inline Vector* vector_at(U address) { return static_cast<Vector*>(as_object(address)); }
inline Quaternion* quaternion_at(U address) { return static_cast<Quaternion*>(as_object(address)); }

// `mov eax,[0078E5EC]; test; push ...; je NV; call [007BB850] / call [007BB7AC]` (cells reloaded per call).
void program_parameter(U index, float x, float y, float z, float w) {
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0)
        kotor_renderer_gl_procs::glProgramEnvParameter4fARB_007bb850(0x8620u, index, x, y, z, w);
    else
        kotor_renderer_gl_procs::glProgramParameter4fNV_007bb7ac(0x8620u, index, x, y, z, w);
}
void enable_dispatch(U mode, texture::Basic* t0) {                     // call dword ptr [007A47E0]
    kotor_renderer_gl_cells::enable_dispatch_007a47e0(mode, t0, nullptr, nullptr, nullptr);
}

// atexit thunks: `mov eax,[data]; test eax,eax; je ret; push eax; call 006FA390` (0073BDB0..0073BE20).
void __cdecl free_blur_positions_0073bdb0() { if (blur_positions_00830434.data) live_free(as_object(blur_positions_00830434.data)); }
void __cdecl free_blur_orientations_0073bdd0() { if (blur_orientations_00830440.data) live_free(as_object(blur_orientations_00830440.data)); }
void __cdecl free_shell_positions_0073bdf0() { if (shell_positions_00830450.data) live_free(as_object(shell_positions_00830450.data)); }
void __cdecl free_shell_orientations_0073be10() { if (shell_orientations_0083045c.data) live_free(as_object(shell_orientations_0083045c.data)); }

// 00437AF0 Matrix::Matrix: writes the 3x3 identity into the caller's local only (00437AF0..00437B6x). Both callers
// overwrite that local completely (rep movs 9 dwords 0048CA31 / 0048D47B) before any read, so its effect is
// reproduced in place (no new lower).
inline void matrix_identity_00437af0(float* m) {
    for (int i = 0; i < 9; ++i) m[i] = 0.0f;
    m[0] = F(kOne_0073d764); m[4] = F(kOne_0073d764); m[8] = F(kOne_0073d764);
}

struct LocalFrame { Vector translation; Quaternion rotation; void* root; };

// Shared prologue of both bodies (0048C79A..0048C8D6 == 0048D1F0..0048D32F): local -> model frame through the
// parent chain. translation = R^-1(-p) accumulated, rotation = conj(q) composed; root = last Part of the chain.
LocalFrame accumulate_frame(void* part) {
    LocalFrame f;
    Vector negated;                                                    // fchs x3, fstp dword
    negated.x = -field<float>(part, 0x08);
    negated.y = -field<float>(part, 0x0C);
    negated.z = -field<float>(part, 0x10);
    const Quaternion* q = &field<Quaternion>(part, 0x14);
    Quaternion conj;
    quaternion_rotate_004a9bb0(&f.translation, quaternion_negate_w_004a9b50(&conj, q), &negated);   // 004A9B50 / 004A9BB0
    quaternion_negate_w_004a9b50(&f.rotation, q);                                                   // second 004A9B50
    f.root = part;                                                     // esi = ebp (0048C7BE / 0048D21F)
    for (void* p = field<void*>(part, 0x3C); p != nullptr; p = field<void*>(p, 0x3C)) {   // [esi+3C] chain
        f.root = p;
        Vector n;
        n.x = -field<float>(p, 0x08);
        n.y = -field<float>(p, 0x0C);
        n.z = -field<float>(p, 0x10);
        const Quaternion* pq = &field<Quaternion>(p, 0x14);
        Quaternion pc;
        Vector r1, r2;
        quaternion_rotate_004a9bb0(&r1, quaternion_negate_w_004a9b50(&pc, pq), &n);
        const Vector* r = quaternion_rotate_004a9bb0(&r2, &f.rotation, &r1);
        // fld T; fadd [r]; fstp dword (component-wise, wide add rounded once)
        const float x = S(W(f.translation.x) + W(r->x));
        const float y = S(W(f.translation.y) + W(r->y));
        const float z = S(W(f.translation.z) + W(r->z));
        f.translation.x = x; f.translation.y = y; f.translation.z = z;
        Quaternion pc2, product;
        const Quaternion* m = quaternion_multiply_004a9ac0(&product, &f.rotation, quaternion_negate_w_004a9b50(&pc2, pq));
        f.rotation = *m;                                               // 4 raw DWORD moves
    }
    return f;
}

// Per-bone body shared by both loops (0048C96E..0048CAA4 / 0048D3BB..0048D574): compose bone with bind pose,
// return the bone matrix and the composed position. `use_wrapper` selects 0044F820 (blur) vs direct cells (shell).
void upload_bone(void* node, ArrayMutations& positions, ArrayMutations& orientations, I i, I bone, bool use_wrapper) {
    const U vector_offset = static_cast<U>(i) * 12u;                  // ebp += 0Ch
    const U quaternion_offset = static_cast<U>(i) * 16u;              // ebx += 10h
    Vector rotated;
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, quaternion_at(orientations.data + quaternion_offset),
        vector_at(field<U>(node, 0x1C4) + vector_offset));           // [node+1C4]+ebp
    const Vector* current = vector_at(positions.data + vector_offset);
    const float x = S(W(r->x) + W(current->x));
    const float y = S(W(current->y) + W(r->y));
    const float z = S(W(current->z) + W(r->z));
    Vector* target = vector_at(positions.data + vector_offset);      // data reloaded before the stores
    target->x = x; target->y = y; target->z = z;
    Quaternion product;
    Quaternion* orientation = quaternion_at(orientations.data + quaternion_offset);
    const Quaternion* m = quaternion_multiply_004a9ac0(&product, orientation,
        quaternion_at(field<U>(node, 0x1B8) + quaternion_offset));   // [node+1B8]+ebx
    *orientation = *m;
    float scratch[9];
    float matrix[9];
    const float* got = quaternion_get_matrix_0044f470(quaternion_at(orientations.data + quaternion_offset), nullptr, scratch);
    for (int k = 0; k < 9; ++k) matrix[k] = got[k];                    // rep movs 9 dwords
    const Vector* p = vector_at(positions.data + vector_offset);
    U index = static_cast<U>(bone) * 3u + 0x12u;                       // lea esi,[eax+eax*2+12h]
    if (use_wrapper) {
        gl_program_env_parameter_0044f820(index, matrix[0], matrix[3], matrix[6], p->x);       // 0048CA5E
        gl_program_env_parameter_0044f820(index + 1u, matrix[1], matrix[4], matrix[7], p->y);  // 0048CA81
        gl_program_env_parameter_0044f820(index + 2u, matrix[2], matrix[5], matrix[8], p->z);  // 0048CAA4
    } else {
        program_parameter(index, matrix[0], matrix[3], matrix[6], p->x);                       // 0048D4B0 / 0048D4D2
        program_parameter(index + 1u, matrix[1], matrix[4], matrix[7], p->y);                  // 0048D501 / 0048D523
        program_parameter(index + 2u, matrix[2], matrix[5], matrix[8], p->z);                  // 0048D552 / 0048D574
    }
}

// Bone loop (0048C928..0048CACA / 0048D37E..0048D595): count re-read every pass, -1 map entries skipped.
void upload_bones(void* node, ArrayMutations& positions, ArrayMutations& orientations, bool use_wrapper) {
    for (I i = 0; i < field<I>(node, 0x1B4); ++i) {
        const I bone = vp_ftol2(W(static_cast<const float*>(as_object(field<U>(node, 0x1B0)))[i]));  // fld; call 006FAE8C
        if (bone == -1) continue;                                      // cmp eax,-1; je
        upload_bone(node, positions, orientations, i, bone, use_wrapper);
    }
}
} // namespace

// ================================================================ 0048C650 RenderSkinnedVPBlur
extern "C" void __cdecl render_skinned_vp_blur_0048c650(void* primitive) {
    void* const holder = field<void*>(primitive, 8);                                   // 0048C673
    void* const part = as_object(part_vt(holder)->s050(holder, nullptr));               // 0048C676..0048C67E (ebp)
    void* const node = field<void*>(part, 4);                                           // 0048C680 (edi)
    if (field<U16>(node, 0x182) == 0) return;                                           // 0048C685..0048C697 -> 0048D01F
    (void)part_vt(part)->s02c(part, nullptr);                                           // 0048C69D..0048C6A2 (EAX unused)
    void* const owner = field<void*>(part, 0x40);                                       // 0048C6A5
    Vector delta;                                                                       // [esp+1C]
    delta.x = S(W(field<float>(owner, 0x78)) - W(field<float>(owner, 0x94)));          // 0048C6BD..0048C6CB
    delta.y = S(W(field<float>(owner, 0x7C)) - W(field<float>(owner, 0x98)));          // 0048C6B4..0048C6CF
    delta.z = S(W(field<float>(owner, 0x80)) - W(field<float>(owner, 0x9C)));          // 0048C6A8..0048C6D3
    const double magnitude = vector_magnitude_004ab100(&delta);                        // 0048C6D7 (ST0)
    const float magnitude_stored = S(magnitude);                                       // 0048C6DC fst [esp+40h]
    if (magnitude > W(F(kZero_0073d700))) {                                            // 0048C6E3..0048C6EE fcomp; test ah,41h
        primitive_vt(primitive)->s018(primitive, nullptr);                             // 0048C6F4..0048C6F8
        float unused_matrix[9];
        matrix_identity_00437af0(unused_matrix);                                       // 0048C6FB..0048C702
        if ((blur_guard_0083044c & 1u) == 0) {                                         // 0048C707
            blur_guard_0083044c = blur_guard_0083044c | 1u;                            // 0048C710
            blur_orientations_00830440.size = 0;                                       // 0048C71C
            blur_orientations_00830440.capacity = 0;                                   // 0048C722
            blur_orientations_00830440.data = 0;                                       // 0048C728
            std::atexit(free_blur_orientations_0073bdd0);                              // 0048C717..0048C72E
        }
        if ((blur_guard_0083044c & 2u) == 0) {                                         // 0048C741
            blur_guard_0083044c = blur_guard_0083044c | 2u;                            // 0048C74A
            blur_positions_00830434.size = 0;                                          // 0048C756
            blur_positions_00830434.capacity = 0;                                      // 0048C75C
            blur_positions_00830434.data = 0;                                          // 0048C762
            std::atexit(free_blur_positions_0073bdb0);                                 // 0048C751..0048C768
        }
        blur_orientations_00830440.size = 0;                                           // 0048C77C
        blur_positions_00830434.size = 0;                                              // 0048C782
        kotor_renderer_init::glDepthMask_0073d28c(0);                                  // 0048C77B..0048C788
        kotor_renderer_init::glEnable_0073d390(kotor_renderer_gl_cells::vertex_enum_0078d46c);   // 0048C78E..0048C794
        const LocalFrame frame = accumulate_frame(part);                               // 0048C79A..0048C8D6
        gather_bones_00488930(frame.root, &blur_positions_00830434, &blur_orientations_00830440,
            frame.translation, frame.rotation);                                        // 0048C8DC..0048C923
        upload_bones(node, blur_positions_00830434, blur_orientations_00830440, true);  // 0048C928..0048CACA
        if ((blur_guard_0083044c & 4u) == 0) {                                         // 0048CAD4..0048CAE1
            blur_guard_0083044c = blur_guard_0083044c | 4u;                            // 0048CAE3
            void* const memory = live_operator_new(0x201Cu);                           // 0048CAE9..0048CAF9
            VertexProgram* program = nullptr;                                          // 0048CB26 xor eax,eax
            if (memory != nullptr)                                                     // 0048CB05..0048CB0F
                program = (::new (memory) VertexProgram)->Construct_004a27d0(           // 0048CB11..0048CB1F (NV text
                    kotor_vp_data::nv_blur_vp_0079fe50, parameters_00830598);          //  on both routes)
            blur_program_00830430 = program;                                           // 0048CB28
        }
        Vector world_position;                                                         // [esp+58]
        world_position.x = 0.0f; world_position.y = 0.0f; world_position.z = 0.0f;     // 0048CB70..0048CB80
        Quaternion world_orientation;                                                  // [esp+74] (w,x,y,z)
        world_orientation.w = F(kOne_0073d764);                                        // 0048CB68
        world_orientation.x = 0.0f; world_orientation.y = 0.0f; world_orientation.z = 0.0f;   // 0048CB47..0048CB5D
        (void)part_vt(part)->s00c(part, nullptr, word_of(&world_position), word_of(&world_orientation));  // 0048CB38..0048CB88
        void* const camera = *cells.current_camera_007bb4e8;                           // 0048CB8B
        Vector to_camera;                                                              // [esp+28]
        to_camera.x = S(W(field<float>(camera, 0x7C)) - W(world_position.x));         // 0048CBB0..0048CBBD
        to_camera.y = S(W(field<float>(camera, 0x80)) - W(world_position.y));         // 0048CB9F..0048CBC1
        to_camera.z = S(W(field<float>(camera, 0x84)) - W(world_position.z));         // 0048CB90..0048CBC5
        Quaternion inverse;
        Vector camera_local;                                                           // [esp+AC]
        quaternion_rotate_004a9bb0(&camera_local, quaternion_negate_w_004a9b50(&inverse, &world_orientation), &to_camera);  // 0048CBC9..0048CBDA
        // Owner orientation third column (0048CBDF..0048CC3B): q = owner+84 (w,x,y,z).
        const double qw = W(field<float>(owner, 0x84)), qx = W(field<float>(owner, 0x88));
        const double qy = W(field<float>(owner, 0x8C)), qz = W(field<float>(owner, 0x90));
        const double s_xy = qx * qx + qy * qy;                                         // 0048CBF4..0048CC00
        const double a_axis = W(F(kOne_0073d764)) - (s_xy + s_xy);                     // fadd st,st; fsubr [0073D764]
        const double b_part = qz * qy - qx * qw;                                       // 0048CC0E..0048CC19
        const double c_part = qz * qx + qy * qw;                                       // 0048CC1D..0048CC2E
        Vector axis;                                                                   // [esp+28]
        axis.x = S(c_part + c_part);                                                   // 0048CC31..0048CC33
        axis.y = S(b_part + b_part);                                                   // 0048CC1B / 0048CC37
        axis.z = S(a_axis);                                                            // 0048CC3B
        Vector cross;                                                                  // [esp+40]
        vector_cross_004ab210(&cross, &delta, &axis);                                  // 0048CC3F
        {
            Quaternion inv; Vector local;
            const Vector* r = quaternion_rotate_004a9bb0(&local, quaternion_negate_w_004a9b50(&inv, &world_orientation), &delta);  // 0048CC47..0048CC6A
            const Vector copy = *r;
            delta = copy;                                                              // 0048CC6F..0048CC8B (raw)
        }
        {
            Quaternion inv; Vector local;
            const Vector* r = quaternion_rotate_004a9bb0(&local, quaternion_negate_w_004a9b50(&inv, &world_orientation), &cross);  // 0048CC82..0048CCA9
            const Vector copy = *r;
            cross = copy;                                                              // 0048CCB6..0048CCD0 (raw)
        }
        const double frame_time = W(bits_to_float(*cells.frame_time_0078e574));       // 0048CCAE fld [0078E574]
        const float speed = S(W(magnitude_stored) / (frame_time + frame_time));        // fadd st,st; fdivr [esp+48h]; fstp [esp+3C]
        Vector relative;                                                               // [esp+10]
        relative.x = S(W(field<float>(owner, 0x78)) - W(world_position.x));           // 0048CCF4..0048CD01
        relative.y = S(W(field<float>(owner, 0x7C)) - W(world_position.y));           // 0048CCED..0048CD05
        relative.z = S(W(field<float>(owner, 0x80)) - W(world_position.z));           // 0048CCE6..0048CD09
        Vector origin;                                                                 // esi/edi/ebx -> [esp+28]
        {
            Quaternion inv; Vector local;
            const Vector* r = quaternion_rotate_004a9bb0(&local, quaternion_negate_w_004a9b50(&inv, &world_orientation), &relative);  // 0048CD0D..0048CD1E
            origin = *r;                                                               // 0048CD23..0048CD28 / 0048CD48..0048CD50
        }
        program_parameter(0x10u, 0.0f, 2.0f, 3.0f, 1.0f);                             // 0048CD2B..0048CD63
        program_parameter(0x11u, origin.x, origin.y, origin.z, F(0x3E19999Au));       // 0048CD69..0048CD89 (0.15f)
        {
            Vector normal;
            const Vector* n = normalize_004ab180(&normal, &delta);                     // 0048CD8F..0048CD9C
            const Vector copy = *n;
            delta = copy;                                                              // 0048CDA1..0048CDC6 -> [esp+1C]
        }
        program_parameter(0x0Cu, delta.x, delta.y, delta.z, 1.0f);                    // 0048CDB5..0048CDD9
        const U owner_180 = field<U>(owner, 0x180);                                    // 0048CDDF..0048CDF4 [esp+34]
        program_parameter(0x0Eu, speed, F(owner_180), F(0x3C23D70Au), F(0x3F59999Au)); // 0048CDEF..0048CE27 (0.01f, 0.85f)
        program_parameter(0x45u, cross.x, cross.y, cross.z, 0.5f);                    // 0048CE2D..0048CE59
        program_parameter(0x0Du, 0.0f, 0.0f, 0.0f, 0.0f);                             // 0048CE5F..0048CE7F
        // Facing test (0048CE85..0048CF15): a stays wide on ST0, b/c are fstp-dword stores.
        const double a = W(origin.x) - W(camera_local.x);                             // 0048CE85..0048CE89
        const float b = S(W(origin.y) - W(camera_local.y));                           // 0048CE90..0048CE9B [esp+14]
        const float c = S(W(origin.z) - W(camera_local.z));                           // 0048CE9F..0048CEAA [esp+18]
        const double dot = (W(b) * W(delta.y) + W(c) * W(delta.z)) + a * W(delta.x);  // 0048CEAE..0048CEC6
        const double px = W(delta.x) * dot, py = dot * W(delta.y), pz = dot * W(delta.z);   // 0048CEC8..0048CED6
        const double projected = (pz * pz + py * py) + px * px;                       // 0048CEDA..0048CEE8
        const double reference = ((a * a + W(c) * W(c)) + W(b) * W(b)) * W(F(kFacing_00742efc));  // 0048CEEA..0048CF02
        if (reference < projected) {                                                   // fcompp; test ah,5; jp 0048CFBF
            Vector plus;                                                               // [esp+C4]
            plus.x = S(W(delta.x) + a);                                                // 0048CF1B..0048CF29
            plus.y = S(W(b) + W(delta.y));                                             // 0048CF30..0048CF38
            plus.z = S(W(c) + W(delta.z));                                             // 0048CF3F..0048CF47
            Vector minus;                                                              // [esp+BC]
            minus.x = S(a - W(delta.x));                                               // 0048CF4E..0048CF52
            minus.y = S(W(b) - W(delta.y));                                            // 0048CF59..0048CF61
            minus.z = S(W(c) - W(delta.z));                                            // 0048CF68..0048CF70
            const float plus_length = S(vector_magnitude_004ab100(&plus));             // 0048CF77..0048CF7C
            const double minus_length = vector_magnitude_004ab100(&minus);             // 0048CF88
            if (minus_length < W(plus_length)) {                                       // fcomp [esp+40h]; test ah,5; jp
                gl_program_env_parameter_0044f820(0x0Du, 1.0f, 0.0f, 0.0f, 0.0f);     // 0048CF9B..0048CFA8
                kotor_renderer_init::glFrontFace_0073d274(kGlCw_0073f2c0);             // 0048CFAD..0048CFB7
            }
        }
        enable_dispatch(0x22u, field<texture::Basic*>(field<void*>(part, 0x44), 0));  // 0048CFC1..0048CFCF
        blur_program_00830430->Enable_004a24d0(part, 1);                              // 0048CFD5..0048CFE1
        render_flat_0046f900(primitive);                                               // 0048CFE6..0048CFEE
        enable_dispatch(0u, nullptr);                                                  // 0048CFF3..0048CFFD
    }
    kotor_renderer_init::glFrontFace_0073d274(kGlCcw_0073f2c4);                       // 0048D006..0048D00C
    kotor_renderer_init::glDisable_0073d22c(kotor_renderer_gl_cells::vertex_enum_0078d46c);   // 0048D012..0048D019
}

// ================================================================ 0048D040 PartSkin::DrawBumpedOutShellVP
extern "C" void __fastcall part_skin_draw_bumped_out_shell_vp_0048d040(void* self, void* /*unused_edx*/) {
    kotor_renderer_gl_cells::current_stage_007a6898 = 0;                               // 0048D067
    if (aur_multi_texture_available_0045f710() != 0)                                   // 0048D06D..0048D074
        kotor_renderer_gl_procs::glActiveTextureARB_007bb80c(
            kotor_renderer_gl_cells::current_stage_007a6898 + 0x84C0u);               // 0048D076..0048D081
    texture::texture_call_bind_b4(field<texture::Basic*>(field<void*>(self, 0x44), 0x0C));   // 0048D087..0048D08F
    void* const material = field<void*>(self, 0x44);                                   // 0048D095
    if (field<I>(material, 0x2C) > 1)                                                  // 0048D098..0048D09C (signed)
        material_set_blending_mode_0047af70(material, nullptr, 1);                    // 0048D09E..0048D0A0
    void* const holder = field<void*>(self, 0x70);                                     // 0048D0A5
    void* const primitive = as_object(primitive_vt(holder)->s008(holder, nullptr));   // 0048D0A8..0048D0AD [esp+78]
    void* const skin = field<void*>(primitive, 8);                                     // 0048D0AF
    void* const part = as_object(part_vt(skin)->s050(skin, nullptr));                 // 0048D0B2..0048D0BB [esp+80]
    void* const node = field<void*>(part, 4);                                          // 0048D0BD [esp+38]
    if (field<U16>(node, 0x182) == 0) return;                                          // 0048D0C0..0048D0D2 -> 0048D66D
    void* tangent_node = nullptr;                                                      // [esp+30] = 0
    if (part_vt(part)->s02c(part, nullptr) != 0) tangent_node = field<void*>(part, 4); // 0048D0D8..0048D0EB
    primitive_vt(primitive)->s018(primitive, nullptr);                                 // 0048D0EF..0048D0F3
    if ((shell_guard_00830468 & 1u) == 0) {                                            // 0048D0F6
        shell_guard_00830468 = shell_guard_00830468 | 1u;                              // 0048D0FF
        shell_orientations_0083045c.size = 0;                                          // 0048D10B
        shell_orientations_0083045c.capacity = 0;                                      // 0048D111
        shell_orientations_0083045c.data = 0;                                          // 0048D117
        std::atexit(free_shell_orientations_0073be10);                                 // 0048D106..0048D11D
    }
    if ((shell_guard_00830468 & 2u) == 0) {                                            // 0048D130..0048D13D
        shell_guard_00830468 = shell_guard_00830468 | 2u;                              // 0048D13F
        shell_positions_00830450.size = 0;                                             // 0048D14A
        shell_positions_00830450.capacity = 0;                                         // 0048D150
        shell_positions_00830450.data = 0;                                             // 0048D156
        std::atexit(free_shell_positions_0073bdf0);                                    // 0048D145..0048D15C
    }
    shell_orientations_0083045c.size = 0;                                              // 0048D173
    shell_positions_00830450.size = 0;                                                 // 0048D179
    float unused_matrix[9];
    matrix_identity_00437af0(unused_matrix);                                           // 0048D16F..0048D17F
    kotor_renderer_init::glEnable_0073d390(kotor_renderer_gl_cells::vertex_enum_0078d46c);   // 0048D184..0048D18A
    program_parameter(0x10u, 0.0f, 2.0f, 3.0f, 1.0f);                                 // 0048D190..0048D1B7
    void* const scene = *cells.active_scene_007b92a0;                                  // 0048D1BD
    const float lit = field<U>(scene, 0x60) != 0 ? 1.0f : 0.0f;                        // 0048D1C2..0048D1CF [esp+14]
    gl_program_env_parameter_0044f820(0x11u, F(field<U>(scene, 0x64)), F(field<U>(scene, 0x68)), lit, 0.5f);  // 0048D1D7..0048D1EB
    const LocalFrame frame = accumulate_frame(part);                                   // 0048D1F0..0048D32F
    gather_bones_00488930(frame.root, &shell_positions_00830450, &shell_orientations_0083045c,
        frame.translation, frame.rotation);                                            // 0048D335..0048D379
    upload_bones(node, shell_positions_00830450, shell_orientations_0083045c, false); // 0048D37E..0048D595
    const U weight = field<U>(field<void*>(self, 0x40), 0x11C);                        // 0048D5A2..0048D5B4 [esp+14]
    program_parameter(0x0Cu, F(weight), 0.0f, 0.0f, 1.0f);                            // 0048D5AF..0048D5E5
    VertexProgram* program;
    if (aurora_material_has_bump_map_0047b8f0(field<void*>(part, 0x44), nullptr) != 0 &&   // 0048D5EB..0048D5F5
        aurora_trimesh_has_tangent_bases_0043e410(static_cast<aurora_model::MdlNode*>(tangent_node), nullptr) != 0 &&  // 0048D5F7..0048D602
        aur_nv2x_level_shaders_available_0045fbd0())                                   // 0048D604..0048D609
        program = static_cast<VertexProgram*>(renderer_program_cells::output_0083038C);    // 0048D60B
    else
        program = static_cast<VertexProgram*>(renderer_program_cells::output_00830388);    // 0048D613
    program->Enable_004a24d0(part, 0);                                                 // 0048D619..0048D61C
    render_flat_0046f900(primitive);                                                   // 0048D621..0048D626
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0) {                             // 0048D62B..0048D635
        kotor_renderer_gl_procs::glDisableVertexAttribArrayARB_007bb770(1);            // 0048D637..0048D639
        kotor_renderer_gl_procs::glDisableVertexAttribArrayARB_007bb770(4);            // 0048D63F..0048D641
    } else {
        kotor_renderer_init::glDisableClientState_0073d264(kClientArray_0073f3a8);     // 0048D649..0048D656
        kotor_renderer_init::glDisableClientState_0073d264(kClientArray_0073f3b0);     // 0048D658..0048D65F
    }
    kotor_renderer_init::glDisable_0073d22c(kotor_renderer_gl_cells::vertex_enum_0078d46c);   // 0048D661..0048D667
}
