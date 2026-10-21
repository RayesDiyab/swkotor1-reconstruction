// trimesh_f2.cpp - lane-3d-mesh fork F2: PartTriMesh::Draw and the fixed-function / NV2x TriMesh render paths.
// swkotor.exe (GOG K1, base 00400000). Every statement cites the original instruction(s) it reproduces.
//   0046F900 RenderFlat               0046FA40 RenderLightMapped        0046FF70 RenderEnvironmentMapped
//   004703F0 RenderEMLM               00470860 RenderEMBM               00470D30 PartTriMesh::GetRenderPath
//   00477830 PartTriMesh::Draw
// Object views (no published layout; offsets observed):
//   VertexPrimitive: +04 mesh node (0046F95C), +08 Part (0046FA5D -> slot +2C self_return)
//   Part:            +04 node (00470D37), +40 owner object (0046FFA6), +44 Material (0046FA67), +70 VertexPrimitive
//                    (00477897), +84 float alpha (0046FFDE)
//   Part+40 object:  +C4 u16 (0046FFEA), +D8 float (0046FFFE), +11C float (004779E5), +1A8 texture (0046FFF2)
//   mesh node:       +100 int* counts, +104 count, +118 int* pool indices, +128/+12C/+194 strip, +130 mode,
//                    +160 signed tex-coord index, +184 u8 lightmapped (0046F95F..0046F9F0, 00470D81)
#include "df9dab63cf-trimesh_skin.h"
#include "a7e200e217-model_instance.hpp"      // lane-3d-model: aurora_node_as_trimesh_0043e400 / has_tangent_bases / tree_as_model
#include "1d2ceaf711-texture_controller.hpp"  // lane-texture-epoch: texture_controller_is_linear_0041f250
#include "ccf5cb551c-render_shared_lowers.h"  // lane-renderer-bodies: kotor_renderer_gl::ActivateTextureStage_0041fe80

// ---------------------------------------------------------------- existing owner declarations (exact copies)
extern "C" std::uint32_t __cdecl aur_texture_get_tid_00420bf0(void*);           // lane-upload upload.h (AurTextureGetTID)
namespace kotor_vertex_program { extern std::uint8_t __cdecl ValidateNonRadeon_0045f5e0(); }  // lane-renderer-bodies enable_bindings.h
namespace kotor_renderer_init { std::uint32_t __cdecl CubeMapAvailable_0045f8b0(); }          // lane-upload upload.h
namespace kotor_renderer_gl { std::uint32_t __cdecl BumpyShinyAvailable_0045f680(); }         // lane-renderer-bodies renderer_gl_loader_lowers.cpp

// LOWERS hoisted to trimesh_skin.h (lane merge)

namespace {
using namespace kotor_trimesh;
using namespace kotor_renderer_init;
using kotor_scene3d::cells;
using kotor_vertex_program::VertexProgram;

// .rdata GL constant words (const int globals read by `mov reg,[addr]`; values from the pinned image)
constexpr U kSrcAlpha_0073f270 = 0x302;          // GL_SRC_ALPHA
constexpr U kOneMinusSrcAlpha_0073f274 = 0x303;  // GL_ONE_MINUS_SRC_ALPHA
constexpr U kOne_0073f278 = 0x1;                 // GL_ONE
constexpr U kZero_0073f27c = 0x0;                // GL_ZERO
constexpr U kOneMinusDstAlpha_0073f288 = 0x305;  // GL_ONE_MINUS_DST_ALPHA
constexpr U kTexCoordArray_0073f398 = 0x8078;    // GL_TEXTURE_COORD_ARRAY
constexpr U kNormalArray_0073f39c = 0x8075;      // GL_NORMAL_ARRAY
constexpr U kVertexAttrib_0073f3b8 = 0x865C;     // client array enums (0073F3B8/BC/C0)
constexpr U kVertexAttrib_0073f3bc = 0x865D;
constexpr U kVertexAttrib_0073f3c0 = 0x865E;

inline texture::Basic* basic(Texture* t) { return reinterpret_cast<texture::Basic*>(t); }  // object-pointer view (D24)
inline void enable_dispatch(U op, Texture* a, Texture* b, Texture* c, Texture* d) {
    (*mesh_cells.enable_dispatch_007a47e0)(op, basic(a), basic(b), basic(c), basic(d));   // call [007A47E0]
}
inline void* primitive_part(void* primitive) {                                             // [[prim+8]]+2C
    void* const p = field<void*>(primitive, 8);
    return as_object(part_table(p)->s02c(p, nullptr));
}
inline void* primitive_self(void* primitive) {                                             // [[prim]]+4 (self_return)
    return as_object(primitive_table(primitive)->s004(primitive, nullptr));
}
inline VertexProgram* program_in(void** cell) {   // [M46 compose] MeshCells bss_00827fec..ffc retyped void** (canonical header edit); mov ecx,[cell]
    return static_cast<VertexProgram*>(*cell);
}
inline void count_triangles(I count) {                                                    // imul 55555556h; shr 1Fh; add
    *mesh_cells.triangles_00827fb0 = *mesh_cells.triangles_00827fb0 + count / 3;
}
// Inlined GLRender::ActivateTextureStage (00470258..0047026C / 0047029E..004702BE): 007A6898 = stage, reread.
inline void activate_stage_inline(U stage) {
    *cells.current_stage_007a6898 = stage;
    if (aur_multi_texture_available_0045f710() != 0)
        (*mesh_cells.active_texture_007bb80c)(*cells.current_stage_007a6898 + 0x84C0u);
}
// Environment texture selection shared by 0046FFDC..00470012 and 00470474..004704AB.
inline Texture* env_texture(void* part, void* owner, float& alpha) {
    Texture* environment = nullptr;
    alpha = bits_to_float(field<U>(part, 0x84));                         // mov eax,[part+84]; mov [esp+10h],eax
    if (owner != nullptr) {
        environment = field<Texture*>(owner, 0x1A8);                     // mov esi,[owner+1A8]
        if (field<U16>(owner, 0xC4) != 0)                                // cmp word [owner+C4],0
            alpha = static_cast<float>(static_cast<double>(alpha) *      // fld; fmul [owner+D8]; fstp dword
                                       static_cast<double>(field<float>(owner, 0xD8)));
        if (environment != nullptr) return environment;
    }
    return material_of(part)->textures[5].target;                        // mov esi,[[part+44]+14h]
}
}

extern "C" {
// ================================================================ 0046F900 RenderFlat (cdecl, RET)
void __cdecl render_flat_0046f900(void* primitive) {
    if (*mesh_cells.gate_0078e614 == 0) return;                                          // 0046F900..0046F907
    if (gate_0078e61c == 0) {                                                            // 0046F90D..0046F91D
        kotor_renderer_init::glDisable_0073d22c(0x0B50);                                 // 0046F91F
        glDisableClientState_0073d264(0x8076);                                           // 0046F92A
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                                      // 0046F931..0046F945
    }
    *mesh_cells.draws_00827fb4 = *mesh_cells.draws_00827fb4 + 1;                         // 0046F94B..0046F956
    void* const node = field<void*>(primitive, 4);                                       // 0046F95C
    const I count = field<I>(node, 0x104);                                               // 0046F967 (cached in EBP)
    if (field<I>(node, 0x160) < 0) glDisableClientState_0073d264(kTexCoordArray_0073f398); // 0046F95F..0046F976
    if (count != 0 && count > 0) {                                                       // 0046F978..0046F980
        for (I i = 0; i < count; ++i) {                                                  // 0046F990..0046F9D4
            const I elements = field<I*>(node, 0x100)[i];                               // 0046F999
            gl_render_draw_elements_00425ab0(field<U>(node, 0x130), elements, 0,         // 0046F990..0046F9AD
                                             field<I*>(node, 0x118)[i]);
            count_triangles(elements);                                                   // 0046F9B2..0046F9CE
        }
    }
    if (field<U>(node, 0x130) == 4 && field<U>(node, 0x12C) != 0)                        // 0046F9D7..0046F9E8
        gl_render_draw_elements_00425ab0(3, field<I>(node, 0x12C), field<U>(node, 0x194), field<I>(node, 0x128)); // 0046F9EA..0046F9FB
    if (field<I>(node, 0x160) < 0) glEnableClientState_0073d260(kTexCoordArray_0073f398); // 0046FA03..0046FA19
    if (gate_0078e61c == 0) {                                                            // 0046FA1B..0046FA22
        kotor_renderer_init::glEnable_0073d390(0x0B50);                                  // 0046FA24
        glEnableClientState_0073d260(0x8076);                                            // 0046FA2F
    }
}

// ================================================================ 0046FA40 RenderLightMapped (cdecl, RET)
void __cdecl render_light_mapped_0046fa40(void* primitive) {
    if (gate_0078e620 == 0) return;                                                      // 0046FA40..0046FA47
    if (*cells.active_scene_007b92a0 == nullptr) { render_flat_0046f900(primitive); return; } // 0046FA49..0046FA52 (tail jmp)
    void* const part = primitive_part(primitive);                                        // 0046FA59..0046FA65
    if (aur_texture_is_null_00420600(material_of(part)->textures[0].target, nullptr) != 0) { // 0046FA67..0046FA73
        render_flat_0046f900(primitive);                                                 // 0046FA75
        return;
    }
    glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);                // 0046FA81..0046FA8F
    gl_render_activate_texture_stage_00425250(1);                                 // 0046FA95
    glEnableClientState_0073d260(kTexCoordArray_0073f398);                               // 0046FA9C..0046FAA5
    gl_render_activate_texture_stage_00425250(0);                                 // 0046FAAB
    Material* const material = material_of(part);                                        // 0046FAB2
    enable_dispatch(0x1A, material->textures[0].target, material->textures[1].target, nullptr, nullptr); // 0046FAB5..0046FAC2
    render_flat_0046f900(primitive);                                                     // 0046FAC9
    enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);                           // 0046FACE..0046FAD8
    gl_render_activate_texture_stage_00425250(1);                                 // 0046FAE0
    glDisableClientState_0073d264(kTexCoordArray_0073f398);                              // 0046FAE5..0046FAEE
    gl_render_activate_texture_stage_00425250(0);                                 // 0046FAF6..0046FAFE (arg cell = 0; tail jmp)
}

// ================================================================ 0046FF70 RenderEnvironmentMapped (cdecl 3 args, RET)
// arg2 is not read; arg3 is tested as a byte (0047003B mov al,[esp+2Ch]).
void __cdecl render_environment_mapped_0046ff70(void* primitive, U, U ati_flag) {
    if (gate_0078e640 == 0) return;                                                      // 0046FF70..0046FF7A
    if (*mesh_cells.gate_0078e614 == 0) return;                                          // 0046FF80..0046FF87
    void* const part = primitive_part(primitive);                                        // 0046FF8F..0046FF9D
    void* const owner = field<void*>(part, 0x40);                                        // 0046FFA6
    if (gate_0078e63c == 0 || aur_texture_env_map_disabled_004204d0() != 0) {            // 0046FF9F..0046FFB2
        Texture* const t0 = material_of(part)->textures[0].target;                       // 0046FFB4..0046FFB7
        const I format = texture_head(t0)->common.s8c(t0, nullptr);                      // 0046FFB9..0046FFBB
        if (format != 7 && format != 8) { render_flat_0046f900(primitive); return; }     // 0046FFC1..0046FFDB
    }
    float alpha;
    Texture* const environment = env_texture(part, owner, alpha);                        // 0046FFDC..00470012
    if (gate_0078e638 != 0 && aur_one_pass_env_map_available_0045fbf0() != 0) {         // 00470012..00470026
        kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);          // 0047002C..00470039
        Texture* const t0 = material_of(part)->textures[0].target;
        if (static_cast<std::uint8_t>(ati_flag) != 0 && kotor_renderer_init::ATIValidate_0045f640())   // 0047003B..0047004A
            enable_dispatch(0x14, t0, environment, nullptr, nullptr);                    // 0047004C..00470068
        else
            enable_dispatch(0x13, t0, environment, nullptr, nullptr);                    // 0047005B..00470068
        do_env_map_transform_0046f780(environment, 1);                                   // 00470071..00470074
        float color[3] = {1.0f, 1.0f, 1.0f};                                             // 00470083..00470093 (locals +14..+1C)
        (*mesh_cells.constant0_dispatch_007a6820)(color, alpha);                         // 00470079..0047009B
        render_flat_0046f900(primitive);                                                 // 004700A1
        enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);                       // 004700A7..004700B1
        undo_env_map_transform_0046f870(environment, 1);                                 // 004700B7..004700BA
        kotor_renderer_gl::ActivateTextureStage_0041fe80(1);                             // 004700BF
        if (kotor_renderer_init::CubeMapAvailable_0045f8b0() != 0)                       // 004700C9..004700D0
            kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texture_cube_map_0078d478); // 004700D2..004700D8
        kotor_renderer_gl::ActivateTextureStage_0041fe80(0);                             // 004700DA
        kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);           // 004700E1..004700EB
        return;
    }
    // multi-pass (004700F9)
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);              // 004700F9..00470100
    if (bss_00827fd4 != 0) {                                                             // 00470106..00470113
        glBlendFunc_0073d38c(kOne_0073f278, kZero_0073f27c);                             // 00470119..00470126
        render_flat_0046f900(primitive);                                                 // 00470128..0047012D
        glBlendFunc_0073d38c(kOneMinusDstAlpha_0073f288, kOne_0073f278);                 // 00470132..00470142
        do_env_map_transform_0046f780(environment, 0);                                   // 00470144..00470147
        texture_vtable::bind_tail(environment)->sb4(environment, nullptr);               // 0047014C..00470153
        if (texture_head(environment)->common.s0c(environment, nullptr) != 0) {          // 00470159..00470168 (AL)
            glTexGeni_0073d314(0x2000, 0x2500, 0x8512);                                  // 0047016A..00470179
            glTexGeni_0073d314(0x2001, 0x2500, 0x8512);                                  // 0047017B..0047018A
            glTexGeni_0073d314(0x2002, 0x2500, 0x8512);                                  // 0047018C..0047019B
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_s_0078d460);    // 0047019D..004701AA
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_t_0078d464);    // 004701AC..004701B3
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_r_0078d468);    // 004701B5..004701F5
        } else {
            glTexGeni_0073d314(0x2000, 0x2500, 0x2402);                                  // 004701BD..004701CC
            glTexGeni_0073d314(0x2001, 0x2500, 0x2402);                                  // 004701CE..004701DD
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_s_0078d460);    // 004701DF..004701EC
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_t_0078d464);    // 004701EE..004701F5
        }
        undo_env_map_transform_0046f870(environment, 0);                                 // 004701F7..004701FA
        render_flat_0046f900(primitive);                                                 // 004701FF..00470204
        if (field<void*>(part, 0x40) == nullptr)                                         // 00470209..00470211
            material_bind_texture0_0047adf0(material_of(part), nullptr);                // 00470213..00470216
    } else {                                                                             // 004702CC
        glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);            // 004702CC..004702DA
        do_env_map_transform_0046f780(environment, 0);                                   // 004702DC..004702DF
        texture_vtable::bind_tail(environment)->sb4(environment, nullptr);               // 004702E4..004702EB
        if (texture_head(environment)->common.s0c(environment, nullptr) != 0) {          // 004702F1..00470300
            glTexGeni_0073d314(0x2000, 0x2500, 0x8512);                                  // 00470302..00470311
            glTexGeni_0073d314(0x2001, 0x2500, 0x8512);                                  // 00470313..00470322
            glTexGeni_0073d314(0x2002, 0x2500, 0x8512);                                  // 00470324..00470333
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_s_0078d460);    // 00470335..00470341
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_t_0078d464);    // 00470343..0047034A
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_r_0078d468);    // 0047034C..0047038C
        } else {
            glTexGeni_0073d314(0x2000, 0x2500, 0x2402);                                  // 00470355..00470364
            glTexGeni_0073d314(0x2001, 0x2500, 0x2402);                                  // 00470366..00470375
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_s_0078d460);    // 00470377..00470383
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.gl_texgen_t_0078d464);    // 00470385..0047038C
        }
        render_flat_0046f900(primitive);                                                 // 0047038E..00470393
        undo_env_map_transform_0046f870(environment, 0);                                 // 00470398..0047039B
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_s_0078d460);       // 004703A0..004703AF
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_t_0078d464);       // 004703B1..004703B8
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_r_0078d468);       // 004703BA..004703C1
        glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);            // 004703C3..004703D0
        material_bind_texture0_0047adf0(material_of(part), nullptr);                     // 004703D2..004703D5
        render_flat_0046f900(primitive);                                                 // 004703DA..004703DF; jmp 00470221
    }
    // common tail 00470221 (the first branch enters at 0047021B, which only loads ESI = glDisable)
    if (kotor_renderer_init::CubeMapAvailable_0045f8b0() != 0)                           // 00470221..00470228
        gl_render_bind_texture_00420440(0x0C, 0);                                        // 0047022A..0047022E
    glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);                // 00470236..00470243
    kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);               // 00470245..0047024C
    activate_stage_inline(1);                                                            // 0047024E..0047026C
    kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_s_0078d460);           // 00470272..00470279
    kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_t_0078d464);           // 0047027B..00470282
    kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texgen_r_0078d468);           // 00470284..0047028A
    if (kotor_renderer_init::CubeMapAvailable_0045f8b0() != 0)                           // 0047028C..00470293
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texture_cube_map_0078d478); // 00470295..0047029C
    activate_stage_inline(0);                                                            // 0047029E..004702BE
}

// ================================================================ 004703F0 RenderEMLM (cdecl, RET)
void __cdecl render_emlm_004703f0(void* primitive) {
    if (gate_0078e644 == 0) return;                                                      // 004703F0..004703FA
    if (gate_0078e640 == 0) return;                                                      // 00470400..00470407
    if (*mesh_cells.gate_0078e614 == 0) return;                                          // 0047040D..00470414
    if (!aur_nv2x_level_shaders_available_0045fbd0()) return;                            // 0047041A..00470421
    void* const part = primitive_part(primitive);                                        // 00470429..00470433
    void* const owner = field<void*>(part, 0x40);                                        // 0047043F
    if (gate_0078e63c == 0 || aur_texture_env_map_disabled_004204d0() != 0) {            // 00470438..0047044B
        Texture* const t0 = material_of(part)->textures[0].target;                       // 0047044D..00470450
        const I format = texture_head(t0)->common.s8c(t0, nullptr);                      // 00470452..00470454
        if (format != 7 && format != 8) { render_flat_0046f900(primitive); return; }     // 0047045A..00470473
    }
    float alpha;
    Texture* const environment = env_texture(part, owner, alpha);                        // 00470474..004704AB
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);              // 004704AB..004704B8
    gl_render_activate_texture_stage_00425250(1);                                 // 004704BA..004704BC
    glEnableClientState_0073d260(kTexCoordArray_0073f398);                               // 004704C1..004704CA
    gl_render_activate_texture_stage_00425250(0);                                 // 004704D0..004704D2
    Material* const material = material_of(part);                                        // 004704D7
    enable_dispatch(0x16, material->textures[0].target, environment, material->textures[1].target, nullptr); // 004704DA..004704E6
    do_env_map_transform_0046f780(environment, 2);                                       // 004704EC..004704EF
    float color[3] = {1.0f, 1.0f, 1.0f};                                                 // 004704FE..0047050E
    (*mesh_cells.constant0_dispatch_007a6820)(color, alpha);                             // 004704F4..00470516
    render_flat_0046f900(primitive);                                                     // 0047051C..0047051D
    undo_env_map_transform_0046f870(environment, 2);                                     // 00470522..00470525
    enable_dispatch(0x25, nullptr, nullptr, nullptr, nullptr);                           // 0047052A..00470534
    gl_render_activate_texture_stage_00425250(1);                                 // 0047053D..0047053F
    glDisableClientState_0073d264(kTexCoordArray_0073f398);                              // 00470544..0047054E
    gl_render_activate_texture_stage_00425250(0);                                 // 00470554..00470556
    kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);               // 0047055B..00470564
    kotor_renderer_gl::ActivateTextureStage_0041fe80(1);                                 // 0047056A..0047056C
    kotor_renderer_init::glDisable_0073d22c(*mesh_cells.gl_texture_cube_map_0078d478);   // 00470571..0047057B
    kotor_renderer_gl::ActivateTextureStage_0041fe80(0);                                 // 0047057D..0047057F
}

// ================================================================ 00470860 RenderEMBM (cdecl, RET)
void __cdecl render_embm_00470860(void* primitive) {
    if (gate_0078e65c == 0) return;                                                      // 00470860..0047086A
    if (*cells.active_scene_007b92a0 == nullptr) { render_flat_0046f900(primitive); return; } // 00470870..00470889
    void* const part = primitive_part(primitive);                                        // 0047088B..00470897
    void* const part_node = field<void*>(part, 4);                                       // 004708A7 (EBP)
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);                    // 004708A2..004708AB
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);              // 004708AD..004708B4
    std::uint8_t lightmapped = 0;                                                        // 004708C5 [esp+12h]
    if (field<std::uint8_t>(field<void*>(primitive, 4), 0x184) != 0 &&                  // 004708BA..004708CA
        *mesh_cells.gate_0078e608 != 0) {                                               // 004708CC..004708D3
        Texture* const t1 = material_of(part)->textures[1].target;                       // 004708D5..004708D8
        lightmapped = 1;                                                                 // 004708DD
        if (t1 != nullptr && aur_texture_get_tid_00420bf0(t1) == 0) lightmapped = 0;     // 004708E2..004708F1 (AL = 0)
    }
    std::uint8_t fogged = 0;                                                             // 00470A1A [esp+13h]
    {
        void* const scene = *cells.active_scene_007b92a0;                               // 004708F5
        if (field<U>(scene, 0x60) != 0) {                                                // 004708FB..00470900
            aurora_model::MaxTree* const model =
                aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(part_node, 8), nullptr); // 00470906..00470909
            if (field<std::uint8_t>(model, 0x53) != 0) {                                 // 0047090E..00470913
                fogged = 1;                                                              // 00470920
                kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);         // 00470919..00470925
            }
        }
    }
    glBlendFunc_0073d38c(kOne_0073f278, kZero_0073f27c);                                 // 0047092B..00470938
    Material* const material = material_of(part);                                        // 0047093E
    if (static_cast<double>(material->water_alpha) == 1.0) {                             // 00470941..00470955 (test ah,44h; jp: NaN/!= skip)
        if (aur_texture_is_null_00420600(material->textures[0].target, nullptr) == 0 && lightmapped != 0) { // 0047095B..00470970
            if (*mesh_cells.gate_0078e650 != 0) {                                        // 00470976..0047097D
                gl_render_activate_texture_stage_00425250(1);                     // 00470983..00470985
                glEnableClientState_0073d260(kTexCoordArray_0073f398);                   // 0047098A..00470994
                gl_render_activate_texture_stage_00425250(0);                     // 0047099A..0047099C
                enable_dispatch(0x1A, material_of(part)->textures[0].target,             // 004709A1..004709B1
                                material_of(part)->textures[1].target, nullptr, nullptr);
                kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c); // 004709B7..004709C0
                program_in(mesh_cells.bss_00827fec)->Enable_004a24d0(part, 1);           // 004709C2..004709CB
                gl_program_env_parameter_0044f820(0x0F, 1.0f, 1.0f, 1.0f, 0.0f);         // 004709D0..004709E3
                render_flat_0046f900(primitive);                                         // 004709E8..004709E9
                enable_dispatch(0x20, nullptr, nullptr, nullptr, nullptr);               // 004709EE..004709F8
                gl_render_activate_texture_stage_00425250(1);                     // 004709FE..00470A00
                glDisableClientState_0073d264(kTexCoordArray_0073f398);                  // 00470A05..00470A0F
                gl_render_activate_texture_stage_00425250(0);                     // 00470A11..00470A13
            }
        } else if (*mesh_cells.gate_0078e650 != 0) {                                     // 00470A24..00470A2B
            program_in(mesh_cells.bss_00827ff4)->Enable_004a24d0(part, 1);               // 00470A2D..00470A36
            kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c); // 00470A3B..00470A42
            enable_dispatch(0, nullptr, nullptr, nullptr, nullptr);                      // 00470A44..00470A4E
            material_bind_texture0_0047adf0(material_of(part), nullptr);                 // 00470A54..00470A5A
            render_flat_0046f900(primitive);                                             // 00470A5F..00470A60
        }
    }
    // 00470A68
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);              // 00470A68..00470A6E
    glPushAttrib_0073d268(0x6040);                                                       // 00470A74..00470A79
    kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c);         // 00470A7F..00470A86
    if (gate_0078e658 != 0) {                                                            // 00470A88..00470A8F
        if (*cells.programs_0078e5ec != 0) {                                             // 00470A95..00470A9C
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0C);                           // 00470A9E..00470AA0
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0D);                           // 00470AA6..00470AA8
            (*mesh_cells.enable_vertex_attrib_007bb79c)(0x0E);                           // 00470AAE..00470AB0
        } else {
            glEnableClientState_0073d260(kVertexAttrib_0073f3b8);                        // 00470AB8..00470ABF
            glEnableClientState_0073d260(kVertexAttrib_0073f3bc);                        // 00470AC5..00470ACB
            glEnableClientState_0073d260(kVertexAttrib_0073f3c0);                        // 00470AD1..00470AD8
        }
        glDisableClientState_0073d264(kNormalArray_0073f39c);                            // 00470ADE..00470AE5
        program_in(lightmapped != 0 ? mesh_cells.bss_00827ffc : mesh_cells.bss_00827ff8) // 00470AE7..00470AF8
            ->Enable_004a24d0(part, 0);                                                  // 00470AF3..00470AFE
        gl_program_env_parameter_0044f820(0x0F, 1.0f, 0.0f, 1.0f, 0.0f);                 // 00470B03..00470B13
        enable_dispatch(0x1D, material_of(part)->textures[4].target, nullptr, nullptr,   // 00470B18..00470B29
                        material_of(part)->textures[5].target);
        enable_dispatch(0x18, nullptr, nullptr, nullptr, nullptr);                       // 00470B2F..00470B39
        if (static_cast<double>(material_of(part)->water_alpha) < 1.0) {                 // 00470B3F..00470B53 (test ah,5; jp)
            glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);        // 00470B55..00470B63
            const float alpha = bits_to_float(field<U>(material_of(part), 0x48));       // 00470B69..00470B6F raw DWORD
            float color[3] = {1.0f, 1.0f, 1.0f};                                         // 00470B7B..00470B8B
            (*mesh_cells.constant0_dispatch_007a6820)(color, alpha);                     // 00470B71..00470B93
        } else {
            glBlendFunc_0073d38c(kOneMinusDstAlpha_0073f288, kOne_0073f278);             // 00470B9E..00470BAC
        }
        if (bss_00827fe8 != 0) glBlendFunc_0073d38c(kOne_0073f278, kZero_0073f27c);      // 00470BB2..00470BC8
        render_flat_0046f900(primitive);                                                 // 00470BCE..00470BCF
        enable_dispatch(0, nullptr, nullptr, nullptr, nullptr);                          // 00470BD4..00470BDE
        enable_dispatch(0x0D, nullptr, nullptr, nullptr, nullptr);                       // 00470BE4..00470BEE
        enable_dispatch(0x1C, nullptr, nullptr, nullptr, nullptr);                       // 00470BF4..00470BFE
        if (*cells.programs_0078e5ec != 0) {                                             // 00470C04..00470C0E
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0C);                          // 00470C10..00470C12
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0D);                          // 00470C18..00470C1A
            (*mesh_cells.disable_vertex_attrib_007bb770)(0x0E);                          // 00470C20..00470C22
        } else {
            glDisableClientState_0073d264(kVertexAttrib_0073f3b8);                       // 00470C2A..00470C31
            glDisableClientState_0073d264(kVertexAttrib_0073f3bc);                       // 00470C33..00470C39
            glDisableClientState_0073d264(kVertexAttrib_0073f3c0);                       // 00470C3B..00470C42
        }
        glEnableClientState_0073d260(kNormalArray_0073f39c);                             // 00470C44..00470C4B
        gl_program_env_parameter_0044f820(0x0F, 1.0f, 1.0f, 1.0f, 0.0f);                 // 00470C51..00470C64
    }
    if (fogged != 0) {                                                                   // 00470C6C..00470C72
        kotor_renderer_init::glEnable_0073d390(*cells.gl_fog_0078d450);                  // 00470C74..00470C7A
        glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);            // 00470C7C..00470C8A
        enable_dispatch(2, material_of(part)->textures[0].target, nullptr, nullptr, nullptr); // 00470C90..00470C9E
        kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c);     // 00470CA4..00470CAE
        program_in(mesh_cells.bss_00827ff0)->Enable_004a24d0(part, 0);                   // 00470CB0..00470CB9
        render_flat_0046f900(primitive);                                                 // 00470CBE..00470CBF
    }
    glPopAttrib_0073d26c();                                                              // 00470CC7
    gl_program_env_parameter_0044f820(0x0F, 1.0f, 1.0f, 1.0f, 0.0f);                     // 00470CCD..00470CE0
    kotor_renderer_init::glDisable_0073d22c(*mesh_cells.vertex_program_0078d46c);        // 00470CE5..00470CEE
    enable_dispatch(0, nullptr, nullptr, nullptr, nullptr);                              // 00470CF4..00470CFE
    kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);               // 00470D04..00470D0E
    glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);                // 00470D10..00470D1D
}

// ================================================================ 00470D30 PartTriMesh::GetRenderPath (thiscall, RET)
I __fastcall part_tri_mesh_get_render_path_00470d30(void* part, void*) {
    aurora_model::MdlNode* const node =
        aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(part, 4), nullptr); // 00470D37..00470D43
    const std::uint8_t bump = aurora_material_has_bump_map_0047b8f0(material_of(part), nullptr); // 00470D40..00470D4D [esp+10h]
    const std::uint8_t linear = texture_controller_is_linear_0041f250(material_of(part), nullptr); // 00470D51..00470D59 [esp+13h] (ECX = Material)
    const std::uint8_t bumpy_shiny = material_has_bumpy_shiny_map_0047b0c0(material_of(part), nullptr); // 00470D5D..00470D64 BL
    const std::uint8_t tangents = aurora_trimesh_has_tangent_bases_0043e410(node, nullptr); // 00470D66..00470D6E [esp+0Eh]
    std::uint8_t env = material_has_env_map_0047b0e0(material_of(part), nullptr);        // 00470D72..00470D7D [esp+0Fh]
    Material* const material = material_of(part);                                        // 00470D77
    const double water_alpha = material->water_alpha;                                    // 00470D7A fld [ecx+48h]
    std::uint8_t lightmapped = field<std::uint8_t>(node, 0x184);                         // 00470D81..00470D8D [esp+11h]
    const std::uint8_t translucent = water_alpha < 1.0 ? 1 : 0;                          // 00470D87..00470D9D (test ah,5; jnp: only C0)
    Texture* const t1 = material->textures[1].target;                                    // 00470DA2
    if (t1 != nullptr && aur_texture_get_tid_00420bf0(t1) == 0) lightmapped = 0;         // 00470DA5..00470DB6
    if (env == 0) {                                                                      // 00470DBA..00470DC0
        void* const owner = field<void*>(part, 0x40);                                    // 00470DC2
        if (owner != nullptr && field<void*>(owner, 0x1A8) != nullptr) env = 1;          // 00470DC5..00470DD3
    }
    I path = kotor_vertex_program::ValidateNonRadeon_0045f5e0() != 0 ? 0 : 1;            // 00470DD8..00470DE3
    std::uint8_t lm = bumpy_shiny;                                                       // BL
    if (bumpy_shiny != 0 && bump != 0 && tangents != 0 && *mesh_cells.gate_0078e66c != 0) { // 00470DE8..00470E03
        if (aur_nv2x_level_shaders_available_0045fbd0()) return 2;                       // 00470E05..00470E1B
        if (lightmapped == 0) goto L_F4E;                                                // 00470E1C..00470E22
        if (aur_nv2x_level_shaders_available_0045fbd0()) goto L_F19;                     // 00470E28..00470E2F
        if (translucent == 0) goto L_F2E;                                                // 00470E35..00470E3B
        path = 10; goto L_FC3;                                                           // 00470E41..00470E46
    }
    lm = lightmapped;                                                                    // 00470E4B
    if (lm != 0) {                                                                       // 00470E4F..00470E51
        if (bump == 0) goto L_ECB;                                                       // 00470E53..00470E59
        if (tangents != 0 && aur_nv2x_level_shaders_available_0045fbd0() &&             // 00470E5B..00470E6A
            *mesh_cells.gate_0078e5fc != 0 && *mesh_cells.gate_0078e608 != 0)            // 00470E6C..00470E7C
            return field<U>(material_of(part), 0x44) != 0 ? 3 : 4;                       // 00470E7E..00470E93 (neg/sbb/add 4)
    }
    if (bump != 0 && tangents != 0 && *mesh_cells.gate_0078e5fc != 0 &&                  // 00470E94..00470EAB
        aur_nv2x_level_shaders_available_0045fbd0())                                     // 00470EAD..00470EB4
        return get_ati_fragment_shaders_disabled_0045f5a0() != 0 ? 5 : 6;                // 00470EB6..00470ECA (neg/sbb/add 6)
L_ECB:
    if (linear != 0 && tangents == 0 && *mesh_cells.gate_0078e5fc != 0 &&                // 00470ECB..00470EE2
        aur_nv2x_level_shaders_available_0045fbd0())                                     // 00470EE4..00470EEB
        return 15;                                                                       // 00470EED..00470EFA
    if (lm != 0) {                                                                       // 00470EFB..00470EFD
        if (env == 0) goto L_F55;                                                        // 00470EFF..00470F05
        if (*mesh_cells.gate_0078e608 == 0) goto L_F46;                                  // 00470F07..00470F0E
        if (!aur_nv2x_level_shaders_available_0045fbd0()) goto L_F2E;                    // 00470F10..00470F17
    L_F19:
        return kotor_vertex_program::ValidateNonRadeon_0045f5e0() != 0 ? 7 : 8;          // 00470F19..00470F2D
    L_F2E:
        return aur_ati_fragment_shaders_bump_map_available_0045fb90() != 0 ? 9 : 14;     // 00470F2E..00470F45 (and ~4; add 0Eh)
    }
L_F46:
    if (env != 0) {                                                                      // 00470F46..00470F4C
    L_F4E:
        path = 10; goto L_FC3;                                                           // 00470F4E..00470F53
    }
L_F55:
    if (lm != 0 && *mesh_cells.gate_0078e608 != 0) {                                     // 00470F55..00470F60
        if (aur_nv2x_level_shaders_available_0045fbd0())                                 // 00470F62..00470F69
            return kotor_vertex_program::ValidateNonRadeon_0045f5e0() != 0 ? 12 : 13;    // 00470F6B..00470F7F
        return 14;                                                                       // 00470F80..00470F8D
    }
    if (bump != 0 && tangents != 0 && *mesh_cells.gate_0078e5fc != 0 &&                  // 00470F8E..00470FA5
        !aur_nv2x_level_shaders_available_0045fbd0())                                    // 00470FA7..00470FAE
        return 6;                                                                        // 00470FB0..00470FBD
    if (path != 10) return path;                                                         // 00470FBE..00470FC1 -> 00470FD1
L_FC3:
    if (kotor_vertex_program::ValidateNonRadeon_0045f5e0() == 0) path = 11;              // 00470FC3..00470FCC
    return path;                                                                         // 00470FD1..00470FD9
}

// ================================================================ 00477830 PartTriMesh::Draw (vtable +64, thiscall RET 4)
// EAX on return (slot type U): [part+40] (0047784A exit), 0 (0046EE40 exit) or the slot +94 result; callers discard it.
U __fastcall part_tri_mesh_draw_00477830(void* self, void*, U) {
    if (gate_0078e68c == 0) {                                                            // 00477830..0047783A
        if (part_table(self)->s04c(self, nullptr) == 0) {                                // 0047783C..00477843
            const U owner = field<U>(self, 0x40);                                        // 00477845
            if (owner != 0) return owner;                                                // 00477848..0047784A -> 00477B1C
        }
    }
    if (part_tri_mesh_visible_0046ee40(self) == 0) return 0;                             // 00477850..0047785B
    aurora_model::MdlNode* const node =
        aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(self, 4), nullptr); // 00477861..00477865
    (void)aurora_material_has_bump_map_0047b8f0(material_of(self), nullptr);             // 0047786A..0047786F (result unused)
    (void)material_has_bumpy_shiny_map_0047b0c0(material_of(self), nullptr);             // 00477874..00477877 (result unused)
    (void)aurora_trimesh_has_tangent_bases_0043e410(node, nullptr);                      // 0047787C..0047787E (result unused)
    part_table(self)->s090(self, nullptr);                                               // 00477883..00477887 TransformModelviewMatrix
    part_table(self)->s088(self, nullptr);                                               // 0047788D..00477891 TransformTextureMatrix
    {
        void* const vertices = field<void*>(self, 0x70);                                 // 00477897
        primitive_table(vertices)->s018(vertices, nullptr);                              // 0047789A..0047789C SetupVertexArrays
    }
    switch (part_tri_mesh_get_render_path_00470d30(self, nullptr)) {                     // 0047789F..004778AF jump table 00477B20
    case 0:  render_flat_0046f900(primitive_self(field<void*>(self, 0x70))); break;                       // 004779BC
    case 1:  render_flat_vp_00470fe0(field<void*>(self, 0x70), self); break;                              // 0047799E
    case 2:  render_embm_00470860(primitive_self(field<void*>(self, 0x70))); break;                       // 004778B6
    case 3:  render_light_mapped_bump_specular_00475450(primitive_self(field<void*>(self, 0x70))); break; // 004778C9
    case 4:  render_light_mapped_bump_diffuse_00474ea0(primitive_self(field<void*>(self, 0x70))); break;  // 004778DC
    case 5:  render_bump_mapped_00476690(primitive_self(field<void*>(self, 0x70))); break;                // 00477902
    case 6:  render_bumpmap_fallback_004713d0(field<void*>(self, 0x70)); break;                           // 00477915
    case 7:  render_emlm_004703f0(primitive_self(field<void*>(self, 0x70))); break;                       // 00477923
    case 8:  render_emlm_vp_004712b0(field<void*>(self, 0x70), self); break;                              // 00477980
    case 9:  render_nv1x_emlm_00470590(primitive_self(field<void*>(self, 0x70))); break;                  // 00477936
    case 10: render_environment_mapped_0046ff70(primitive_self(field<void*>(self, 0x70)), 0, 0); break;   // 00477949
    case 11: render_environment_mapped_vp_00471180(field<void*>(self, 0x70), self); break;                // 004779AD
    case 12: render_light_mapped_0046fa40(primitive_self(field<void*>(self, 0x70))); break;               // 00477970
    case 13: render_lightmapped_vp_004710b0(field<void*>(self, 0x70), self); break;                       // 0047798F
    case 14: render_light_mapped_no_shaders_0046fb10(primitive_self(field<void*>(self, 0x70))); break;    // 00477960
    case 15: render_normal_mapped_00477050(primitive_self(field<void*>(self, 0x70))); break;              // 004778EF
    default: break;                                                                      // 004778A6..004778A9 (ja 004779CD)
    }
    void* const owner = field<void*>(self, 0x40);                                        // 004779DA
    if (kotor_renderer_gl::BumpyShinyAvailable_0045f680() != 0 && owner != nullptr &&    // 004779CD..004779DF
        !(static_cast<double>(field<float>(owner, 0x11C)) == 0.0) &&                     // 004779E5..004779F6 (test ah,44h; jnp: NaN draws)
        *cells.active_scene_007b92a0 != nullptr) {                                       // 004779FC..00477A03
        *mesh_cells.draws_00827fb4 = *mesh_cells.draws_00827fb4 + 1;                     // 00477A09 inc dword
        void* const mesh = field<void*>(primitive_self(field<void*>(self, 0x70)), 4);    // 00477A0F..00477A1D (EDI)
        kotor_renderer_init::glEnable_0073d390(*mesh_cells.vertex_program_0078d46c);     // 00477A17..00477A21
        gl_program_env_parameter_0044f820(0x0C, bits_to_float(field<U>(field<void*>(self, 0x40), 0x11C)), // 00477A27..00477A3C raw
                                          0.0f, 0.0f, 1.0f);
        static_cast<VertexProgram*>(*mesh_cells.vertex_program_00828014)->Enable_004a24d0(self, 0); // 00477A41..00477A4D
        {
            Texture* const t3 = material_of(self)->textures[3].target;                   // 00477A52..00477A55
            texture_vtable::bind_tail(t3)->sb4(t3, nullptr);                             // 00477A58..00477A5A
        }
        if (field<I>(material_of(self), 0x2C) > 1)                                       // 00477A60..00477A67 (states.size)
            material_set_blending_mode_0047af70(material_of(self), nullptr, 1);          // 00477A69..00477A6B
        for (I i = 0; i < field<I>(mesh, 0x104); ++i) {                                  // 00477A70..00477ACA (bound reread)
            const I elements = field<I*>(mesh, 0x100)[i];                               // 00477A89..00477A8F
            gl_render_draw_elements_00425ab0(field<U>(mesh, 0x130), elements, 0,         // 00477A80..00477A9D
                                             field<I*>(mesh, 0x118)[i]);
            count_triangles(elements);                                                   // 00477AA2..00477ABB
        }
        if (field<U>(mesh, 0x130) == 4 && field<U>(mesh, 0x12C) != 0)                    // 00477ACD..00477ADF
            gl_render_draw_elements_00425ab0(3, field<I>(mesh, 0x12C), field<U>(mesh, 0x194), field<I>(mesh, 0x128)); // 00477AE1..00477AF2
        kotor_renderer_init::glDisable_0073d22c(*mesh_cells.vertex_program_0078d46c);    // 00477AFA..00477B01
    }
    part_table(self)->s08c(self, nullptr);                                               // 00477B07..00477B0B RestoreTextureMatrix
    return part_table(self)->s094(self, nullptr);                                        // 00477B11..00477B15 RestoreModelviewMatrix
}
}
