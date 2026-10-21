// f1_part_material.cpp - lane-m46-3dmodel fork F1: PartLight::Mat 004738C0, PartTriMesh::Mat 00473900 and its
// helpers StaticPartNeedsMat 0046F410, DynamicPartCalculateMaterial 0046F490, Material::SetMaterialColor
// 0047B070. Root set = the lane-3d-mesh consumer roots (texture-epoch/include for the texture table view).
// Conventions as f1_part_core.cpp (D1 slot shapes from model_vtables.cpp, D30-FINAL, raw DWORD copies).
#include "6c81d8874d-scene3d.h"
#include "283ae0c931-texture_vtable.hpp"
#include "b22a9fca42-material.h"
#include <cstring>

using kotor_scene3d::U;
using kotor_scene3d::I;
using kotor_scene3d::U8;
using kotor_scene3d::Vector;
using kotor_scene3d::field;

namespace {
inline unsigned char* bp(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
inline void* obj(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void copy3(void* dst, const void* src) { std::memcpy(dst, src, 12); }
template <class T> inline T get(const void* p, U off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template <class T> inline void put(void* p, U off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
inline float f32(double v) { return static_cast<float>(v); }
inline double W(float v) { return static_cast<double>(v); }
inline float raw_float(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
constexpr U kOneBits = 0x3F800000u;
constexpr U kMinusOneBits = 0xBF800000u;
// .rdata GL enum words read by 00473D56..00473DA8 / 0047B098..0047B0A2 (values from the pinned image)
constexpr U kGlFront_0073f2ec = 0x404;
constexpr U kGlAmbient_0073f2fc = 0x1200;
constexpr U kGlDiffuse_0073f300 = 0x1201;
constexpr U kGlEmission_0073f304 = 0x1600;
}  // namespace

namespace kotor_renderer_init {   // M40 aurora-windows-gl-imports
void __stdcall glColor4f_0073d224(float, float, float, float);
void __stdcall glMaterialfv_0073d360(std::uint32_t, std::uint32_t, const float*);
int __cdecl VertexProgramsAvailable_0045f770();   // lane-renderer-bodies gl-vertex-availability (YAHXZ)
}
extern "C" {
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);   // gui-writers owner
Vector* __cdecl vector_multiply_004ab1e0(Vector* out, const Vector* a, const Vector* b);  // F2 (trimesh_skin.h declaration)
// lane-3d-vp owner of the shared .data gate 0078E66C (D54): ?gate_0078e66c@kotor_vp_cells@@3IC
}
namespace kotor_vp_cells { extern volatile unsigned int gate_0078e66c; }
extern "C" {
// 0078E8C0 skin/vertex-program gate (.data cold 1; D31): no storage owner existed in M45 or any lane object
// (MeshCells only views it). Storage now defined once in m46_orphan_cells.cpp (kotor_vp_cells).
}
namespace kotor_vp_cells { extern volatile unsigned int skin_programs_0078e8c0; }   // single owner m46_orphan_cells.cpp (main fix)
extern "C" {
}

// ---------------------------------------------------------------- cells owned here (only 0046F410 /
// 00473900 / 0047B070 reference them in the image; .data values from the pinned image, BSS zero)
extern "C" {
float aurora_unit_color_0078e894[3] = {1.0f, 1.0f, 1.0f};   // .data 3F800000 x3 (0046F44E, 00473B9B)
// 00828074 BSS Vector (zero): single storage owner f2_gl.cpp env_map_origin_00828074 (main integration fix; also read by
// DoEnvMapTransform 0046F7EB/0046F7FC). Declared here, not defined.
extern Vector env_map_origin_00828074;
volatile U aurora_material_alpha_last_0078e8b8 = kMinusOneBits;   // .data BF800000 (-1.0f)
volatile U8 aurora_material_dynamic_last_0078e8bc = 1;      // .data byte 01
volatile U8 aurora_material_fog_0078e8bd = 1;               // .data byte 01
volatile U aurora_material_disabled_00828040 = 0;           // BSS (read 00473A39 only; no writer instruction)
volatile U aurora_material_force_00828048 = 0;              // BSS (read 00473AE9 / 00473C4B only; no writer instruction)
volatile U aurora_material_guard_00828164 = 0;              // function-static guard bits 1/2/4/8/10h/20h
float aurora_material_last_ambient_00828158[3] = {0.0f, 0.0f, 0.0f};
float aurora_material_last_diffuse_0082814c[3] = {0.0f, 0.0f, 0.0f};
float aurora_material_last_emission_00828140[3] = {0.0f, 0.0f, 0.0f};
float aurora_material_ambient_00828134[3] = {0.0f, 0.0f, 0.0f};
float aurora_material_diffuse_00828128[3] = {0.0f, 0.0f, 0.0f};
float aurora_material_emission_0082811c[3] = {0.0f, 0.0f, 0.0f};
volatile U aurora_material_color_scale_0078e390 = kOneBits; // .data 3F800000 (0047B07B only)
}

extern "C" {
U8 __cdecl static_part_needs_mat_0046f410(void* part, void* node, U8* dynamic, U8* changed);
void __cdecl dynamic_part_calculate_material_0046f490(void* gob, float* ambient, float* emission, float* alpha);
U __fastcall material_set_material_color_0047b070(void* material, void* unused_edx, Vector color, float alpha);
U __fastcall part_light_mat_004738c0(void* self, void* unused_edx);
U __fastcall part_tri_mesh_mat_00473900(void* self, void* unused_edx);
}

// 0046F410 StaticPartNeedsMat(part, node, dynamic*, changed*) cdecl, AL result (upper EAX not used by 00473A2D).
U8 __cdecl static_part_needs_mat_0046f410(void* part, void* node, U8* dynamic, U8* changed) {
    if (get<U8>(part, 0x7C) == 0) {                                                     // 0046F415..0046F41A
        void* const m = get<void*>(part, 0x80);                                         // 0046F41C
        if (m != nullptr) *dynamic = static_cast<U8>(get<U>(m, 0x34) != 0);             // 0046F422..0046F432
        if (*dynamic != 0) { *changed = 1; return 1; }                                  // 0046F434..0046F443
    }
    if (aurora_vector_equal_004aa980(bp(node, 0x8C), aurora_unit_color_0078e894) == 0) return 1;   // 0046F444..0046F45E
    if (get<U>(part, 0x84) != kOneBits) return 1;                                       // 0046F460..0046F46A (integer compare)
    if (aurora_vector_equal_004aa980(bp(part, 0x64), &env_map_origin_00828074) == 0) return 1;   // 0046F46C..0046F47F
    if (get<U8>(get<void*>(part, 0x44), 0x40) != 0) return 1;                           // 0046F481..0046F489
    return 0;                                                                           // 0046F48B
}
// 0046F490 DynamicPartCalculateMaterial(gob, ambient*, emission*, alpha*) cdecl.
void __cdecl dynamic_part_calculate_material_0046f490(void* gob, float* ambient, float* emission, float* alpha) {
    if (get<std::uint16_t>(gob, 0xC4) != 0) {                                           // 0046F49D..0046F4A5
        *alpha = f32(W(get<float>(gob, 0xD8)) * W(*alpha));                             // 0046F4A7..0046F4BF
        Vector t;
        copy3(ambient, vector_multiply_004ab1e0(&t, reinterpret_cast<const Vector*>(ambient),
                                                reinterpret_cast<const Vector*>(bp(gob, 0xCC))));   // 0046F4B4..0046F4D8
    }
    if ((get<U8>(get<void*>(gob, 0x58), 0x50) >> 3) & 1)                                // 0046F4DC..0046F4E8
        *alpha = f32(W(get<float>(gob, 0xDC)) * W(*alpha));                             // 0046F4EA..0046F4F2
    if (get<std::uint16_t>(gob, 0xC6) != 0) {                                           // 0046F4F4..0046F4FC
        const float x = f32(W(get<float>(gob, 0xE0)) + W(emission[0]));                 // 0046F502..0046F50A
        const float y = f32(W(get<float>(gob, 0xE4)) + W(emission[1]));                 // 0046F512..0046F51B
        const float z = f32(W(get<float>(gob, 0xE8)) + W(emission[2]));                 // 0046F51F..0046F531
        emission[0] = x; emission[1] = y; emission[2] = z;                              // 0046F52C / 0046F52E / 0046F539
    }
}
// 0047B070 Material::SetMaterialColor(color by value, alpha) thiscall RET 10h: if the texture-0 object's
// slot 24 (IsUsingGlobalAlpha, AL) is set: glMaterialfv(GL_FRONT, GL_DIFFUSE, Homogenize(color, alpha*[0078E390])).
// The texture-0 target is dereferenced unconditionally (as the original 0047B070..0047B074).
U __fastcall material_set_material_color_0047b070(void* material, void*, Vector color, float alpha) {
    font::Texture* const t = static_cast<material_owner::Material*>(material)->textures[0].target;   // 0047B070
    if (texture_vtable::head(t)->common.s24(t, nullptr) == 0) return 0;                 // 0047B072..0047B079 (EAX = AL result, upper bits not reproduced)
    alpha = f32(W(raw_float(aurora_material_color_scale_0078e390)) * W(alpha));         // 0047B07B..0047B08F
    const float* const h = homogenize_004aaab0(&color, &alpha);                         // 0047B081..0047B093
    kotor_renderer_init::glMaterialfv_0073d360(kGlFront_0073f2ec, kGlDiffuse_0073f300, h);   // 0047B098..0047B0A9
    return 0;   // EAX = glMaterialfv's (void; not reproduced)
}

// 004738C0 PartLight::Mat: glColor4f(+4C, +50, +54, 1.0) and publish the colour to 0078E8AC. EAX = [+54] DWORD.
U __fastcall part_light_mat_004738c0(void* self, void*) {
    const U r = get<U>(self, 0x4C), g = get<U>(self, 0x50), b = get<U>(self, 0x54);
    kotor_renderer_init::glColor4f_0073d224(raw_float(r), raw_float(g), raw_float(b), 1.0f);   // 004738C0..004738DD (fld/fstp dword copies)
    copy3(kotor_scene3d::cells.part_draw_vector_0078e8ac, bp(self, 0x4C));               // 004738E3..004738F7
    return b;                                                                           // 004738F4 mov eax,[esi+8]
}

// 00473900 PartTriMesh::Mat: material colour state for one trimesh part. Statics (guard 00828164):
// last ambient/diffuse/emission (init -1.0), current ambient/diffuse/emission (init 0). Fog follows +7D when
// the scene has fog (+60). Gob-attached parts bind texture 0 / blending 0 unless Gob+164; free parts ask
// StaticPartNeedsMat. Colours come from the node (+8C/+98) and part (+64; unit colour when Material+40), with
// the Gob's dynamic scaling; unchanged state (and 0078E330 clear) skips the GL updates unless 00828048.
// EAX: void method (callers ignore EAX; not reproduced).
U __fastcall part_tri_mesh_mat_00473900(void* self, void*) {
    U guard = aurora_material_guard_00828164;                                           // 00473900
    void* const node = get<void*>(self, 4);                                             // 0047390F esi
    if ((guard & 1) == 0) { aurora_material_guard_00828164 = guard = guard | 1;          // 00473908..00473917
        put<U>(aurora_material_last_ambient_00828158, 0, kMinusOneBits); put<U>(aurora_material_last_ambient_00828158, 4, kMinusOneBits); put<U>(aurora_material_last_ambient_00828158, 8, kMinusOneBits); }   // 0047391C..00473930
    if ((guard & 2) == 0) { aurora_material_guard_00828164 = guard = guard | 2;          // 0047393A..00473941
        put<U>(aurora_material_last_diffuse_0082814c, 0, kMinusOneBits); put<U>(aurora_material_last_diffuse_0082814c, 4, kMinusOneBits); put<U>(aurora_material_last_diffuse_0082814c, 8, kMinusOneBits); }   // 00473946..0047395A
    if ((guard & 4) == 0) { aurora_material_guard_00828164 = guard = guard | 4;          // 00473964..0047396B
        put<U>(aurora_material_last_emission_00828140, 0, kMinusOneBits); put<U>(aurora_material_last_emission_00828140, 4, kMinusOneBits); put<U>(aurora_material_last_emission_00828140, 8, kMinusOneBits); }   // 00473970..00473984
    U alpha_bits = get<U>(self, 0x84);                                                  // 0047398E..00473994 [esp+10h]
    U8 dynamic = 0;                                                                     // 004739A0 [esp+0Eh]
    U8 changed;                                                                         // [esp+0Fh]: written only by 0046F410 (never read here)
    void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;                    // 00473998
    if (scene != nullptr && get<U>(scene, 0x60) != 0) {                                 // 0047399E..004739AC
        bool done = false;
        if (get<U8>(self, 0x7D) != 0) {                                                 // 004739AE..004739B3
            kotor_renderer_init::glEnable_0073d390(*kotor_scene3d::cells.gl_fog_0078d450);   // 004739B5..004739BC
            const U8 again = get<U8>(self, 0x7D);                                       // 004739C2
            guard = aurora_material_guard_00828164;                                     // 004739C7
            aurora_material_fog_0078e8bd = 1;                                           // 004739CC
            done = again != 0;                                                          // 004739C5 / 004739D3
        }
        if (!done) {
            kotor_renderer_init::glDisable_0073d22c(*kotor_scene3d::cells.gl_fog_0078d450);  // 004739D5..004739DB
            guard = aurora_material_guard_00828164;                                     // 004739E1
            aurora_material_fog_0078e8bd = 0;                                           // 004739E6
        }
    }
    void* const gob = get<void*>(self, 0x40);                                           // 004739ED
    if (gob != nullptr) {                                                               // 004739F0
        if (get<U8>(gob, 0x164) == 0) {                                                 // 004739F4..004739FC
            material_bind_texture0_0047adf0(get<void*>(self, 0x44), nullptr);           // 004739FE..00473A01
            material_set_blending_mode_0047af70(get<void*>(self, 0x44), nullptr, 0);    // 00473A06..00473A0B
            guard = aurora_material_guard_00828164;                                     // 00473A10
        }
        dynamic = 1;                                                                    // 00473A15
    } else {
        dynamic = static_part_needs_mat_0046f410(self, node, &dynamic, &changed);       // 00473A1C..00473A2D
        guard = aurora_material_guard_00828164;                                         // 00473A31
    }
    (void)changed;
    if (aurora_material_disabled_00828040 != 0) return 0;                               // 00473A39..00473A41
    float* const amb = aurora_material_ambient_00828134;
    float* const dif = aurora_material_diffuse_00828128;
    float* const emi = aurora_material_emission_0082811c;
    if ((guard & 8) == 0) { aurora_material_guard_00828164 = guard = guard | 8; put<U>(amb, 0, 0); put<U>(amb, 4, 0); put<U>(amb, 8, 0); }      // 00473A47..00473A67
    if ((guard & 0x10) == 0) { aurora_material_guard_00828164 = guard = guard | 0x10; put<U>(dif, 0, 0); put<U>(dif, 4, 0); put<U>(dif, 8, 0); } // 00473A71..00473A91
    if ((guard & 0x20) == 0) { aurora_material_guard_00828164 = guard | 0x20; put<U>(emi, 0, 0); put<U>(emi, 4, 0); put<U>(emi, 8, 0); }        // 00473A9B..00473ABB
    kotor_renderer_init::glColor4f_0073d224(0.0f, 0.0f, 0.0f, 1.0f);                    // 00473AC5..00473AD6
    if (dynamic == 0) {                                                                 // 00473AD8..00473ADE
        if (aurora_material_dynamic_last_0078e8bc == 0 && aurora_material_force_00828048 == 0) return 0;   // 00473AE0..00473AF0
        put<U>(amb, 0, kOneBits); put<U>(amb, 4, kOneBits); put<U>(amb, 8, kOneBits);   // 00473AF6..00473B0A
        put<U>(dif, 0, kOneBits); put<U>(dif, 4, kOneBits); put<U>(dif, 8, kOneBits);   // 00473B14..00473B28
        put<U>(emi, 0, 0); put<U>(emi, 4, 0); put<U>(emi, 8, 0);                        // 00473B32..00473B46
    } else {
        copy3(amb, bp(node, 0x8C));                                                     // 00473B55..00473B6F
        copy3(dif, bp(node, 0x98));                                                     // 00473B74..00473B8E
        const void* const src = get<U8>(get<void*>(self, 0x44), 0x40) != 0
                              ? static_cast<const void*>(aurora_unit_color_0078e894) : bp(self, 0x64);   // 00473B93..00473BA2
        copy3(emi, src);                                                                // 00473BA5..00473BB9
        void* const g = get<void*>(self, 0x40);                                         // 00473BBF
        if (g != nullptr) {                                                             // 00473BC2..00473BC4
            float alpha = raw_float(alpha_bits);
            dynamic_part_calculate_material_0046f490(g, amb, emi, &alpha);              // 00473BC6..00473BD6
            std::memcpy(&alpha_bits, &alpha, 4);
        }
    }
    // 00473BDE: compare with the last state when the previous or current part was dynamic
    bool update = true;
    if (aurora_material_dynamic_last_0078e8bc != 0 || dynamic != 0) {                   // 00473BDE..00473BED
        if (aurora_vector_equal_004aa980(aurora_material_last_ambient_00828158, amb) != 0 &&   // 00473BEF..00473C03
            aurora_vector_equal_004aa980(aurora_material_last_diffuse_0082814c, dif) != 0 &&   // 00473C05..00473C19
            aurora_vector_equal_004aa980(aurora_material_last_emission_00828140, emi) != 0 &&  // 00473C1B..00473C2F
            raw_float(aurora_material_alpha_last_0078e8b8) == raw_float(alpha_bits) &&    // 00473C31..00473C40 test ah,44h; jp
            *kotor_scene3d::cells.texture_flag_0078e330 == 0)                            // 00473C42..00473C49
            update = false;
    } else {
        update = false;                                                                 // 00473BED -> 00473C4B
    }
    if (!update && aurora_material_force_00828048 == 0) return 0;                       // 00473C4B..00473C52
    if ((kotor_vp_cells::skin_programs_0078e8c0 != 0 || kotor_vp_cells::gate_0078e66c != 0) &&          // 00473C58..00473C68
        kotor_renderer_init::VertexProgramsAvailable_0045f770() != 0) {                 // 00473C6E..00473C75
        U v[3]; std::memcpy(v, emi, 12);                                                // 00473C83..00473C9C
        void* const sc = *kotor_scene3d::cells.active_scene_007b92a0;                   // 00473C7B
        if (sc != nullptr && get<U>(self, 0x40) != 0) {                                 // 00473C81..00473CA6
            Vector t;
            const Vector* const m = vector_multiply_004ab1e0(&t, reinterpret_cast<const Vector*>(bp(sc, 0x88)),
                                                             reinterpret_cast<const Vector*>(dif));   // 00473CA8..00473CB9
            const float x = f32(W(raw_float(v[0])) + W(m->x));                          // 00473CBE..00473CC7
            const float y = f32(W(raw_float(v[1])) + W(m->y));                          // 00473CCB..00473CD2
            const float z = f32(W(raw_float(v[2])) + W(m->z));                          // 00473CD6..00473CE5
            std::memcpy(&v[0], &x, 4); std::memcpy(&v[1], &y, 4); std::memcpy(&v[2], &z, 4);
        }
        gl_program_env_parameter_0044f820(0x5D, raw_float(v[0]), raw_float(v[1]), raw_float(v[2]), 1.0f);   // 00473CED..00473CF7
        gl_program_env_parameter_0044f820(0x5E, amb[0], amb[1], amb[2], raw_float(alpha_bits));            // 00473CFC..00473D17
        gl_program_env_parameter_0044f820(0x5F, dif[0], dif[1], dif[2], 1.0f);                             // 00473D1C..00473D37
    }
    float w = 1.0f;                                                                     // 00473D49 [esp+14h] = 3F800000
    kotor_renderer_init::glMaterialfv_0073d360(kGlFront_0073f2ec, kGlAmbient_0073f2fc,
        homogenize_004aaab0(reinterpret_cast<const Vector*>(dif), &w));                 // 00473D3F..00473D6D
    float alpha = raw_float(alpha_bits);
    kotor_renderer_init::glMaterialfv_0073d360(kGlFront_0073f2ec, kGlDiffuse_0073f300,
        homogenize_004aaab0(reinterpret_cast<const Vector*>(amb), &alpha));             // 00473D6F..00473D8F
    w = 1.0f;                                                                           // 00473D9B
    kotor_renderer_init::glMaterialfv_0073d360(kGlFront_0073f2ec, kGlEmission_0073f304,
        homogenize_004aaab0(reinterpret_cast<const Vector*>(emi), &w));                 // 00473D91..00473DB9
    if (get<U>(self, 0x40) != 0) {                                                      // 00473DBB..00473DC0
        Vector c; std::memcpy(&c, amb, 12);                                             // 00473DC6..00473DE4 (by value)
        material_set_material_color_0047b070(get<void*>(self, 0x44), nullptr, c, raw_float(alpha_bits));   // 00473DC2..00473DE7
        kotor_renderer_init::glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                // 00473DEC..00473E0D
    } else {
        kotor_renderer_init::glColor4f_0073d224(0.0f, 0.0f, 0.0f, 1.0f);                // 00473E02..00473E0D
    }
    aurora_material_dynamic_last_0078e8bc = dynamic;                                    // 00473E0F..00473E1E
    copy3(aurora_material_last_ambient_00828158, amb);                                  // 00473E13..00473E40
    copy3(aurora_material_last_diffuse_0082814c, dif);                                  // 00473E30..00473E62
    copy3(aurora_material_last_emission_00828140, emi);                                 // 00473E52..00473E7D
    aurora_material_alpha_last_0078e8b8 = alpha_bits;                                   // 00473E74..00473E83
    *kotor_scene3d::cells.texture_flag_0078e330 = 0;                                    // 00473E89
    return 0;
}
