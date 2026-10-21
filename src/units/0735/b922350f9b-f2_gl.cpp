// f2_gl.cpp - lane-m46-3dmodel fork F2: GLRender client-array setters, texture-stage / env-map helpers and the
// VertexPrimitive slot bodies reached by the 3D model / animation / mesh units (M45 newly exposed names).
// swkotor.exe GOG K1 (image base 00400000); listings lane-m46-3dmodel/d/<ADDR>.txt (dumpbin of the pinned exe).
// Every statement cites the instruction(s) it reproduces.
//
// Conventions: D1 (__thiscall -> __fastcall(self, unused EDX, ...)), D21/D24 (vtable calls through the published
// typed tables: lane-3d-model aurora_vt::VTable_00742f74 / VTable_00740b68, lane-texture-epoch texture_vtable),
// D30-FINAL (no float arithmetic in this unit). Borrowed globals are read through the published cell views
// (kotor_scene3d::cells, kotor_trimesh::mesh_cells) or the owners' exact declarations (copied below).
// EH: 00478990 / 00478A40 / 00478AC0 carry SEH frames (states 0/-1 around the base-dtor call); no callee can throw
// (extern "C" bodies, live_free), so the unwind funclets are cited, not emitted (D30-FINAL).
#include "df9dab63cf-trimesh_skin.h"
#include "5d11c14380-model_load.hpp"       // lane-3d-model node casts / pool wrappers (exact owner declarations)
#include "a7e200e217-model_instance.hpp"   // lane-3d-model aurora_trimesh_has_tangent_bases_0043e410

using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using I16 = std::int16_t;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::field; using kotor_scene3d::cells;
using kotor_trimesh::mesh_cells; using kotor_trimesh::part_table; using kotor_trimesh::primitive_table;
using kotor_trimesh::as_object;

// ---------------------------------------------------------------- owners' exact declarations (copies)
namespace kotor_renderer_init {   // lane-renderer-bodies / M45 gl-vertex-availability (decorations ?..@kotor_renderer_init@@YA.XZ)
U32 __cdecl CubeMapAvailable_0045f8b0();
U32 __cdecl BumpMapAvailable_0045fb10();
int __cdecl VertexProgramsAvailable_0045f770();
extern void __stdcall glTexCoordPointer_0073d29c(I32, U32, I32, const void*);
extern void __stdcall glNormalPointer_0073d2a4(U32, I32, const void*);
extern void __stdcall glMatrixMode_0073d228(U32);
extern void __stdcall glMultMatrixf_0073d25c(const float*);
}
namespace kotor_renderer_gl_procs {   // aurora-reviewed/gl-cohort/renderer_gl_procs.h (exact copies)
using PFN_glVertexAttribPointerARB = void (__stdcall*)(std::uint32_t, std::int32_t, std::uint32_t, std::uint8_t, std::int32_t, const void*);
extern PFN_glVertexAttribPointerARB glVertexAttribPointerARB_007bb7d8;
using PFN_glVertexAttribPointerNV = void (__stdcall*)(std::uint32_t, std::int32_t, std::uint32_t, std::int32_t, const void*);
extern PFN_glVertexAttribPointerNV glVertexAttribPointerNV_007bb854;
}
namespace kotor_renderer_gl_cells {   // aurora-reviewed/gl-cohort/renderer_gl_cells.h (exact copy)
extern std::int32_t vertex_buffer_level_0078e500;   // cold 2; compared signed
}
namespace texture_cells {             // texture-epoch/include/texture_cells.hpp (exact copy)
extern unsigned char volatile fill_alpha_007a46d4;
}

extern "C" {
// lane-3d-model model_vtables.hpp tables (published, D39) - stored by the destructors below
// (declared there: aurora_vtable_vertex_flat_00742f74 / skinned_00742fac / dangly_00742fe4 / anim_0074301c)
// anim3d.h:244..246 (exact copies)
void __cdecl gl_render_set_vertex_buffer_00425900(U stride, U offset, U pool, I count);
void __cdecl gl_render_set_normal_buffer_004259e0(U stride, U offset, U pool, I count);
void __cdecl gl_render_set_tex_coord_buffer_00425a40(U stride, U offset, U pool, I count);
// NEW names (GLRender family, authored here because the VertexPrimitive bodies call them)
void __cdecl gl_render_set_color_buffer_00425970(U stride, U offset, U pool, I count);
void __cdecl gl_render_set_interleaved_buffer_00425520(U stride_unused, U flags, U pool, I count_unused, void* node, U offset);
// model_vtables.cpp spellings (exact) of the slot bodies authored here
U __fastcall vertex_primitive_anim_deleting_destructor_00478b30(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_dangly_deleting_destructor_00478aa0(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_flat_deleting_destructor_004789f0(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_skinned_deleting_destructor_00478a20(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_flat_reconnect_arrays_var_00478c20(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_skinned_reconnect_arrays_var_00478f50(void* self, void* unused_edx, U);
U __fastcall vertex_primitive_flat_setup_arrays_00479020(void* self, void* unused_edx);
U __fastcall vertex_primitive_setup_vertex_arrays_standard_004790e0(void* self, void* unused_edx);
// NEW names: complete-object destructor bodies (non-deleting) called by the deleting destructors
void __fastcall vertex_primitive_flat_destruct_00478990(void* self, void* unused_edx);
void __fastcall vertex_primitive_skinned_destruct_00478a10(void* self, void* unused_edx);
void __fastcall vertex_primitive_dangly_destruct_00478a40(void* self, void* unused_edx);
void __fastcall vertex_primitive_anim_destruct_00478ac0(void* self, void* unused_edx);
}

// ---------------------------------------------------------------- storage owned by these bodies
extern "C" {
// AurOnePassEnvMapAvailable cache (.data 0078E544 = FFFFFFFFh in the pinned image; only 0045FBF0 reads/writes it)
std::int32_t one_pass_env_map_cache_0078e544 = -1;
// BSS Vector pushed as the translation of the env-map texture matrix (no writer anywhere: zero; also read by
// StaticPartNeedsMat 0046F410 - not authored, it borrows this definition)
Vector env_map_origin_00828074 = {0.0f, 0.0f, 0.0f};
}

namespace {
constexpr U kFloat_1406 = 0x1406u;           // GL_FLOAT (immediates 00425930 etc.)
constexpr U kUnsignedByte_1401 = 0x1401u;    // GL_UNSIGNED_BYTE (004259A0)
constexpr U kShort_1402 = 0x1402u;           // GL_SHORT (00478FD2 / 00479000)
constexpr U kArrayBuffer_8892 = 0x8892u;     // GL_ARRAY_BUFFER_ARB (0042591A)
constexpr U kTexture0_84c0 = 0x84C0u;        // GL_TEXTURE0_ARB (0042525E lea +84C0h)
constexpr U kMaxGeneralCombiners_0073f26c = 0x854Du;  // .rdata 0073F26C (glGetIntegerv pname)
constexpr U kModelview_0073f298 = 0x1700u;   // .rdata 0073F298 GL_MODELVIEW
constexpr U kTexture_0073f29c = 0x1702u;     // .rdata 0073F29C GL_TEXTURE
constexpr U kTexCoordArray_0073f398 = 0x8078u; // .rdata 0073F398
constexpr U kColorArray_0073f3a0 = 0x8076u;  // .rdata 0073F3A0

namespace gl = kotor_renderer_init;
const void* as_pointer(U word) { return reinterpret_cast<const void*>(static_cast<std::uintptr_t>(word)); }
U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }

// Shared shape of 00425900 / 00425970 / 004259E0 / 00425A40: VBO -> bind pool buffer and pass the offset,
// else pass pool base + offset. The pool cell is re-read after VertexBufferAvailable (00425907 mov ecx,[007A68A4]).
template <class Emit> void set_buffer(U offset, U pool_index, Emit emit) {
    const U vbo = gl::VertexBufferAvailable_0045fc90();                            // call 0045FC90
    kotor_renderer_gl::RequestPoolManager* const pool = *mesh_cells.pool_007a68a4; // mov ecx,[007A68A4]
    if (vbo != 0) {
        void* const buffer = pool->GetPool(static_cast<std::int32_t>(pool_index));  // call [eax+10h]
        (*mesh_cells.bind_buffer_007bb7b4)(kArrayBuffer_8892, word_of(buffer));     // push 8892h; call [007BB7B4]
        emit(as_pointer(offset));                                                  // pointer = offset
        return;
    }
    void* const base = pool->GetPool(static_cast<std::int32_t>(pool_index));        // call [edx+10h]
    emit(as_pointer(word_of(base) + offset));                                      // add eax,ecx
}

// MultiTextureAvailable-gated client texture select used by 00425520 / 00478C20 (inline copies of 00425250's body).
void client_stage(U stage) {
    if (aur_multi_texture_available_0045f710() != 0)                               // call 0045F710; test; je
        (*cells.client_active_texture_007bb6b0)(kTexture0_84c0 + stage);           // push 84C0h+n; call [007BB6B0]
    *cells.current_stage_007a6898 = stage;                                         // mov [007A6898],n
}
// ARB (programs 0078E5EC) else NV (0078E8C0 && VertexProgramsAvailable) attribute pointer (00425704..0042573C pattern)
void attrib_pointer(U index, I size, U type, I stride, U pointer) {
    if (*cells.programs_0078e5ec != 0) {
        kotor_renderer_gl_procs::glVertexAttribPointerARB_007bb7d8(index, size, type, 0, stride, as_pointer(pointer));
    } else if (*mesh_cells.skin_programs_0078e8c0 != 0 && gl::VertexProgramsAvailable_0045f770() != 0) {
        kotor_renderer_gl_procs::glVertexAttribPointerNV_007bb854(index, size, type, stride, as_pointer(pointer));
    }
}
void* part_of(void* primitive) { return field<void*>(primitive, 8); }                        // VertexPrimitive+8
void* part_as_trimesh(void* part) { return as_object(part_table(part)->s02c(part, nullptr)); } // part vtbl+2C
} // namespace

extern "C" {
// ================================================================ GLRender client-array setters (cdecl)
// 00425900 GLRender::SetVertexBuffer(stride, offset, pool, count): glVertexPointer(3, GL_FLOAT, stride, p).
void __cdecl gl_render_set_vertex_buffer_00425900(U stride, U offset, U pool, I) {
    set_buffer(offset, pool, [stride](const void* p) {
        gl::glVertexPointer_0073d2a8(3, kFloat_1406, static_cast<I>(stride), p); }); // 0042592D..00425936 / 00425951..0042595A
}
// 00425970 GLRender::SetColorBuffer: glColorPointer(4, GL_UNSIGNED_BYTE, stride, p).
void __cdecl gl_render_set_color_buffer_00425970(U stride, U offset, U pool, I) {
    set_buffer(offset, pool, [stride](const void* p) {
        gl::glColorPointer_0073d2a0(4, kUnsignedByte_1401, static_cast<I>(stride), p); }); // 0042599D..004259A6 / 004259C1..004259CA
}
// 004259E0 GLRender::SetNormalBuffer: glNormalPointer(GL_FLOAT, stride, p).
void __cdecl gl_render_set_normal_buffer_004259e0(U stride, U offset, U pool, I) {
    set_buffer(offset, pool, [stride](const void* p) {
        gl::glNormalPointer_0073d2a4(kFloat_1406, static_cast<I>(stride), p); });   // 00425A0D..00425A14 / 00425A2F..00425A36
}
// 00425A40 GLRender::SetTexCoordBuffer: glTexCoordPointer(2, GL_FLOAT, stride, p).
void __cdecl gl_render_set_tex_coord_buffer_00425a40(U stride, U offset, U pool, I) {
    set_buffer(offset, pool, [stride](const void* p) {
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, static_cast<I>(stride), p); }); // 00425A6D..00425A76 / 00425A91..00425A9A
}

// 00425250 GLRender::ActivateTextureStage (cdecl(stage)): client texture select + stage cell.
// D54: this spelling is the adopted one; lane-3d-bsp's gl_render_activate_client_texture_stage_00425250 is a join.
// [M45 compose] 00425250 body removed: composer single-body pick lane-m46-3dcore gl_render_activate_client_texture_stage_00425250 (GL helper owner; D54 spelling pick pending) (definition removed; declaration kept)
void __cdecl gl_render_activate_texture_stage_00425250(U stage);

// 00425520 GLRender::SetInterleavedBuffer (cdecl, 6 args; arg1 and arg4 unused by the body).
// node = MdlNode (stride +14C, offsets +154 pos / +158 normal / +15C colour / +160..+16C uv0..3 / +170 tangent basis).
void __cdecl gl_render_set_interleaved_buffer_00425520(U, U flags, U pool_index, I, void* node, U offset) {
    const U vbo = gl::VertexBufferAvailable_0045fc90();                            // 00425524
    kotor_renderer_gl::RequestPoolManager* const pool = *mesh_cells.pool_007a68a4; // 0042552D
    U base;
    if (vbo != 0) {                                                                // 00425533..00425538
        void* const buffer = pool->GetPool(static_cast<std::int32_t>(pool_index)); // 0042553A
        (*mesh_cells.bind_buffer_007bb7b4)(kArrayBuffer_8892, word_of(buffer));    // 0042553D..00425543
        base = offset;                                                             // 00425549
    } else {
        base = word_of(pool->GetPool(static_cast<std::int32_t>(pool_index))) + offset; // 0042554F..00425554
    }
    const U8 bl = static_cast<U8>(flags);                                          // 00425558 mov bl,[esp+18h]
    if (bl & 1u) gl::glVertexPointer_0073d2a8(3, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x154) + base)); // 0042555C..0042557C
    if (bl & 0x20u) gl::glNormalPointer_0073d2a4(kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x158) + base)); // 00425582..0042559C
    if (bl & 0x40u) gl::glColorPointer_0073d2a0(4, kUnsignedByte_1401, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x15C) + base)); // 004255A2..004255BE
    if (bl & 2u) {                                                                 // 004255C4..004255CD
        client_stage(0);                                                           // 004255CF..004255E3
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x160) + base)); // 004255ED..00425604
    }
    if (bl & 4u) {
        client_stage(1);                                                           // 0042560B..0042561F
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x164) + base)); // 00425629..00425640
    }
    if (bl & 8u) {
        client_stage(2);                                                           // 00425647..0042565B
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x168) + base)); // 00425665..0042567C
    }
    if (bl & 0x10u) {
        client_stage(3);                                                           // 00425683..00425697
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x16C) + base)); // 004256A1..004256B8
    }
    client_stage(0);                                                               // 004256BA..004256CE
    if (gl::VertexProgramsAvailable_0045f770() == 0) return;                       // 004256D8..004256DF
    if (bl & 0x80u) {                                                              // 004256E5..004256E7 test bl,bl; jns
        attrib_pointer(0xC, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base);        // 004256ED..0042573C
        attrib_pointer(0xD, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base + 0xC);  // 0042573C..0042578D
        attrib_pointer(0xE, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base + 0x18); // 0042578D..004257DE
        (*mesh_cells.enable_vertex_attrib_007bb79c)(0xC);                          // 004257DE..004257E0
        (*mesh_cells.enable_vertex_attrib_007bb79c)(0xD);                          // 004257E6..004257E8
        (*mesh_cells.enable_vertex_attrib_007bb79c)(0xE);                          // 004257EE..004257F0
    } else {
        (*mesh_cells.disable_vertex_attrib_007bb770)(0xC);                         // 004257F8..004257FA
        (*mesh_cells.disable_vertex_attrib_007bb770)(0xD);                         // 00425800..00425802
        (*mesh_cells.disable_vertex_attrib_007bb770)(0xE);                         // 00425808..0042580A
    }
    aurora_model::MdlNode* const as_node = static_cast<aurora_model::MdlNode*>(node);
    if (aurora_node_as_skin_0043e3f0(as_node, nullptr) == nullptr) {               // 00425810..00425819
        (*mesh_cells.disable_vertex_attrib_007bb770)(1);                           // 004258E0..004258E2
        (*mesh_cells.disable_vertex_attrib_007bb770)(4);                           // 004258E8..004258EA
        return;
    }
    void* const skin = aurora_node_as_skin_0043e3f0(as_node, nullptr);             // 0042581F..00425821
    attrib_pointer(1, 4, kFloat_1406, field<I>(node, 0x14C), field<U>(skin, 0x1A8) + base);           // 00425826..00425875
    void* const skin_again = aurora_node_as_skin_0043e3f0(as_node, nullptr);       // 00425875..00425877
    attrib_pointer(4, 4, kFloat_1406, field<I>(node, 0x14C), field<U>(skin_again, 0x1AC) + base);     // 0042587C..004258CB
    (*mesh_cells.enable_vertex_attrib_007bb79c)(1);                                // 004258CB..004258CD
    (*mesh_cells.enable_vertex_attrib_007bb79c)(4);                                // 004258D3..004258D5
}

// ================================================================ texture / env-map helpers (cdecl)
// 004204D0 AurTextureEnvMapDisabled: AL = byte [007A46D4] (owner texture_cells::fill_alpha_007a46d4).
std::uint8_t __cdecl aur_texture_env_map_disabled_004204d0() {
    return texture_cells::fill_alpha_007a46d4;                                      // 004204D0 mov al,[007A46D4]
}

// 0045FBF0 AurOnePassEnvMapAvailable: cached in 0078E544 (-1 = not computed).
U __cdecl aur_one_pass_env_map_available_0045fbf0() {
    const std::int32_t cached = one_pass_env_map_cache_0078e544;                    // 0045FBF0
    if (cached != -1) return static_cast<U>(cached);                               // 0045FBF5..0045FBFB -> 0045FC8A
    const std::int32_t level = kotor_renderer_gl_cells::vertex_buffer_level_0078e500; // 0045FC01
    if (level < 1) { one_pass_env_map_cache_0078e544 = 0; return 0; }              // 0045FC07..0045FC0A -> 0045FC83
    const U caps = *cells.capabilities_007bb85c;                                   // 0045FC0C
    const U mask = *cells.mask_0078e4d0;                                           // 0045FC12
    const U control = *cells.control_0078e4fc;                                     // 0045FC18
    const U all = ((caps & mask) == mask) ? 1u : 0u;                               // 0045FC1E..0045FC28 (and/sub/neg/sbb/inc)
    if ((all & control) != 0 && level == 1) {                                      // 0045FC29..0045FC30
        one_pass_env_map_cache_0078e544 = 0; return 0;                             // 0045FC83..0045FC8D
    }
    if (kotor_renderer_init::BumpMapAvailable_0045fb10() == 0) {                   // 0045FC32..0045FC3E
        one_pass_env_map_cache_0078e544 = 0; return 0;
    }
    if ((all & control) != 0) {                                                    // 0045FC40..0045FC4B (recomputed, same value)
        one_pass_env_map_cache_0078e544 = 1; return 1;                             // 0045FC75..0045FC82
    }
    kotor_renderer_init::glGetIntegerv_0073d244(kMaxGeneralCombiners_0073f26c, &one_pass_env_map_cache_0078e544); // 0045FC4D..0045FC58
    const std::int32_t combiners = one_pass_env_map_cache_0078e544;                 // 0045FC5E
    const U result = combiners >= 2 ? 1u : 0u;                                     // 0045FC64..0045FC69 (signed setge)
    one_pass_env_map_cache_0078e544 = static_cast<std::int32_t>(result);            // 0045FC6F
    return result;
}

// 0046F780 DoEnvMapTransform(texture, stage) cdecl: for cube-map textures load the camera orientation into the
// texture matrix of `stage`, then restore GL_MODELVIEW and stage 0.
void __cdecl do_env_map_transform_0046f780(void* texture, U stage) {
    if (kotor_renderer_init::CubeMapAvailable_0045f8b0() == 0) return;             // 0046F783..0046F78A
    font::Texture* const t = static_cast<font::Texture*>(texture);
    if (texture_vtable::head(t)->common.s0c(t, nullptr) == 0) return;              // 0046F790..0046F79B call [eax+0Ch]; test al,al
    *cells.current_stage_007a6898 = stage;                                         // 0046F7A1..0046F7A5
    if (aur_multi_texture_available_0045f710() != 0)                               // 0046F7AB..0046F7B2
        (*mesh_cells.active_texture_007bb80c)(*cells.current_stage_007a6898 + kTexture0_84c0); // 0046F7B4..0046F7C1
    gl::glMatrixMode_0073d228(kTexture_0073f29c);                                  // 0046F7C7..0046F7D4
    gl::glPushMatrix_0073d300();                                                   // 0046F7D6
    float matrix[16];                                                              // [esp+14h] (after push esi)
    Quaternion identity;                                                           // [esp+4h] (written only on the no-camera path)
    const Quaternion* orientation;
    void* const camera = *cells.current_camera_007bb4e8;                           // 0046F7DC
    if (camera != nullptr) {                                                       // 0046F7E1..0046F7E3
        orientation = static_cast<const Quaternion*>(kotor_scene3d::at(camera, 0x88)); // 0046F7E5 add eax,88h
    } else {
        identity.z = 0.0f; identity.y = 0.0f; identity.x = 0.0f;                   // 0046F805..0046F815
        identity.w = 1.0f;                                                         // 0046F81D mov [..],3F800000h
        orientation = &identity;                                                   // 0046F7F7 lea edx,[esp+4]
    }
    make_matrix4x4_004aa9c0(matrix, &env_map_origin_00828074, orientation);         // 0046F7EA..0046F826 push q; push 828074h; push m
    gl::glMultMatrixf_0073d25c(matrix);                                            // 0046F82E..0046F833
    gl::glMatrixMode_0073d228(kModelview_0073f298);                                // 0046F839..0046F840
    *cells.current_stage_007a6898 = 0;                                             // 0046F842
    if (aur_multi_texture_available_0045f710() != 0)                               // 0046F84C..0046F854
        (*mesh_cells.active_texture_007bb80c)(*cells.current_stage_007a6898 + kTexture0_84c0); // 0046F856..0046F861
}

// 0046F870 UndoEnvMapTransform(texture, stage) cdecl: pop the texture matrix pushed by 0046F780.
void __cdecl undo_env_map_transform_0046f870(void* texture, U stage) {
    if (kotor_renderer_init::CubeMapAvailable_0045f8b0() == 0) return;             // 0046F870..0046F877
    font::Texture* const t = static_cast<font::Texture*>(texture);
    if (texture_vtable::head(t)->common.s0c(t, nullptr) == 0) return;              // 0046F879..0046F884
    *cells.current_stage_007a6898 = stage;                                         // 0046F886..0046F88A
    if (aur_multi_texture_available_0045f710() != 0)                               // 0046F890..0046F897
        (*mesh_cells.active_texture_007bb80c)(*cells.current_stage_007a6898 + kTexture0_84c0); // 0046F899..0046F8A6
    gl::glMatrixMode_0073d228(kTexture_0073f29c);                                  // 0046F8AC..0046F8B9
    gl::glPopMatrix_0073d2fc();                                                    // 0046F8BB
    gl::glMatrixMode_0073d228(kModelview_0073f298);                                // 0046F8C1..0046F8C8
    *cells.current_stage_007a6898 = 0;                                             // 0046F8CA
    if (aur_multi_texture_available_0045f710() != 0)                               // 0046F8D4..0046F8DC
        (*mesh_cells.active_texture_007bb80c)(*cells.current_stage_007a6898 + kTexture0_84c0); // 0046F8DE..0046F8EB
}

// ================================================================ VertexPrimitive family
// Layout (observed): +0 vtable, +4 MdlNode, +8 Part, +14 byte (cleared after SetupArrays), +18 pool handle
// (Flat), +1C/+20 pool handles (Anim) or +1C heap block (Dangly).
//
// 00478990 VertexPrimitiveFlat::~VertexPrimitiveFlat (complete-object body, RET 0).
// DEVIATION (D24 table gate, labeled): the final store `mov [esi],742F48h` (004789D7) installs the abstract
// VertexPrimitive table 00742F48, which no lane publishes as a typed table; it is not reproduced. It is
// observable only through a virtual call on an already-destroyed object (every caller is a deleting destructor
// or a derived destructor that frees/ends the object next). Gate: lane-3d-model to publish 00742F48.
void __fastcall vertex_primitive_flat_destruct_00478990(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_vertex_flat_00742f74;             // 004789AD mov [esi],742F74h
    const U pool = field<U>(self, 0x18);                                           // 004789B3
    if (pool != 0xFFFFFFFFu) {                                                     // 004789B6..004789C1
        aurora_pool_delete_00423f50(pool);                                         // 004789C3..004789C9
        field<U>(self, 0x18) = 0xFFFFFFFFu;                                        // 004789CC
    }
}
// 00478A10 VertexPrimitiveSkinned::~VertexPrimitiveSkinned (complete-object body): table store, tail jump 00478990.
void __fastcall vertex_primitive_skinned_destruct_00478a10(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_vertex_skinned_00742fac;          // 00478A10
    vertex_primitive_flat_destruct_00478990(self, nullptr);                        // 00478A16 jmp 00478990
}
// 00478A40 VertexPrimitiveDangly::~VertexPrimitiveDangly: free(+1C) unconditionally, then the Flat body.
void __fastcall vertex_primitive_dangly_destruct_00478a40(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_vertex_dangly_00742fe4;           // 00478A5D
    live_free(field<void*>(self, 0x1C));                                           // 00478A63..00478A6F (006FA390)
    vertex_primitive_flat_destruct_00478990(self, nullptr);                        // 00478A77..00478A81
}
// 00478AC0 VertexPrimitiveAnim::~VertexPrimitiveAnim: delete both pools (no -1 test), then the Flat body.
void __fastcall vertex_primitive_anim_destruct_00478ac0(void* self, void*) {
    field<const void*>(self, 0) = &aurora_vtable_vertex_anim_0074301c;             // 00478ADD
    aurora_pool_delete_00423f50(field<U>(self, 0x1C));                             // 00478AE3..00478AEF
    field<U>(self, 0x1C) = 0xFFFFFFFFu;                                            // 00478AF8
    aurora_pool_delete_00423f50(field<U>(self, 0x20));                             // 00478AF4..00478AFF
    field<U>(self, 0x20) = 0xFFFFFFFFu;                                            // 00478B09
    vertex_primitive_flat_destruct_00478990(self, nullptr);                        // 00478B07..00478B18
}

// Deleting destructors (vtable +00, thiscall RET 4): body; if (flags & 1) free(self); EAX = self.
U __fastcall vertex_primitive_flat_deleting_destructor_004789f0(void* self, void*, U flags) {
    vertex_primitive_flat_destruct_00478990(self, nullptr);                        // 004789F3
    if (flags & 1u) live_free(self);                                               // 004789F8..00478A00 (006FA390)
    return word_of(self);                                                          // 00478A08 mov eax,esi
}
U __fastcall vertex_primitive_skinned_deleting_destructor_00478a20(void* self, void*, U flags) {
    vertex_primitive_skinned_destruct_00478a10(self, nullptr);                     // 00478A23
    if (flags & 1u) live_free(self);                                               // 00478A28..00478A30
    return word_of(self);                                                          // 00478A38
}
U __fastcall vertex_primitive_dangly_deleting_destructor_00478aa0(void* self, void*, U flags) {
    vertex_primitive_dangly_destruct_00478a40(self, nullptr);                      // 00478AA3
    if (flags & 1u) live_free(self);                                               // 00478AA8..00478AB0
    return word_of(self);                                                          // 00478AB8
}
U __fastcall vertex_primitive_anim_deleting_destructor_00478b30(void* self, void*, U flags) {
    vertex_primitive_anim_destruct_00478ac0(self, nullptr);                        // 00478B33
    if (flags & 1u) live_free(self);                                               // 00478B38..00478B40
    return word_of(self);                                                          // 00478B48
}

// 004790E0 VertexPrimitive::SetupVertexArraysStandard (vtable +18, RET 0). EAX = the +1C slot's EAX.
U __fastcall vertex_primitive_setup_vertex_arrays_standard_004790e0(void* self, void*) {
    void* const part = part_of(self);                                              // 004790E3
    void* const trimesh = part_as_trimesh(part);                                   // 004790E6..004790E8 call [eax+2Ch]
    if (field<U8>(trimesh, 0x7C) == 0) {                                           // 004790EB..004790F0
        gl::glDisableClientState_0073d264(kColorArray_0073f3a0);                   // 004790F2..004790F9
    } else if (*cells.black_fog_008305e0 != 0) {                                   // 0047910C..00479113
        gl::glDisableClientState_0073d264(kColorArray_0073f3a0);                   // 00479115..0047911C
    } else {
        gl::glEnableClientState_0073d260(kColorArray_0073f3a0);                    // 0047912F..00479135
    }
    const U result = primitive_table(self)->s01c(self, nullptr);                   // 004790FF..00479103 / 00479122..00479126 / 0047913B..0047913F
    field<U8>(self, 0x14) = 0;                                                     // 00479106 / 00479129 / 00479142
    return result;
}

// 00479020 VertexPrimitiveFlat::SetupArrays (vtable +1C, RET 0).
// EAX on return: the last callee's EAX (SetVertexBuffer / SetInterleavedBuffer, both `void` here); returned as 0
// (documented: the +1C result reaches only 004790E0, whose callers discard it).
U __fastcall vertex_primitive_flat_setup_arrays_00479020(void* self, void*) {
    const U extra = primitive_table(self)->s00c(self, nullptr);                    // 00479025..00479028 call [eax+0Ch] -> ebp
    (void)primitive_table(self)->s010(self, nullptr);                              // 0047902B..00479031 call [edx+10h] (EAX unused)
    void* const part = part_of(self);                                              // 00479034
    void* const trimesh = part_as_trimesh(part);                                   // 00479037..00479039 call [eax+2Ch]
    void* const node = field<void*>(self, 4);                                      // 00479040 edi = [esi+4]
    if (field<U8>(trimesh, 0x7C) == 1)                                             // 0047903C..00479043
        gl_render_set_color_buffer_00425970(4, 0, field<U>(trimesh, 0x74),         // 00479045..00479056
            static_cast<I>(field<I16>(node, 0x180)) + 1);
    const U offset = field<U>(node, 0x194);                                        // 00479061 / 00479072
    const I count = static_cast<I>(field<I16>(node, 0x180)) + 1;                   // 0047906B..00479076
    void* const mdl_node = field<void*>(self, 4);                                  // 0047905E ecx = [esi+4]
    aurora_model::MaxTree* const model = aurora_tree_as_model_0043e1c0(
        field<aurora_model::MaxTree*>(mdl_node, 8), nullptr);                      // 00479067..00479077
    void* const node_again = field<void*>(self, 4);                                // 00479086
    gl_render_set_interleaved_buffer_00425520(field<U>(node_again, 0x14C), field<U>(node_again, 0x150), // 0047908D..0047909B
        field<U>(model, 0xAC), count, node, offset);
    if (extra != 0) {                                                              // 004790A3..004790A6
        void* const e = as_object(extra);
        gl_render_set_tex_coord_buffer_00425a40(8, 0, field<U>(e, 0x20),           // 004790A8..004790B8
            static_cast<I>(field<I16>(node, 0x180)));
        gl_render_set_vertex_buffer_00425900(0xC, 0, field<U>(e, 0x1C),            // 004790BD..004790D0
            static_cast<I>(field<I16>(field<void*>(self, 4), 0x180)));
    }
    return 0;
}

// 00478C20 VertexPrimitiveFlat::ReconnectArraysVAR(base) (vtable +24, RET 4): set every client array from the
// VAR block at `base`; returns base(+colours) + stride * vertex_count.
U __fastcall vertex_primitive_flat_reconnect_arrays_var_00478c20(void* self, void*, U base_arg) {
    const I count = static_cast<I>(field<I16>(field<void*>(self, 4), 0x180));      // 00478C27..00478C31 ([esp+0Ch])
    void* const part = part_of(self);                                              // 00478C35
    void* const trimesh = part_as_trimesh(part);                                   // 00478C38..00478C3C
    void* const node = aurora_node_as_trimesh_0043e400(field<aurora_model::MdlNode*>(trimesh, 4), nullptr); // 00478C41..00478C48 -> esi
    U base;
    if (field<U8>(trimesh, 0x7C) != 0) {                                           // 00478C4F..00478C54
        base = base_arg;                                                           // 00478C56
        gl::glColorPointer_0073d2a0(4, kUnsignedByte_1401, 0, as_pointer(base));   // 00478C5A..00478C64
        base += static_cast<U>(count) * 4u;                                        // 00478C6A..00478C6E lea ebx,[ebx+eax*4]
        if (*cells.black_fog_008305e0 != 0)                                        // 00478C71..00478C78
            gl::glDisableClientState_0073d264(kColorArray_0073f3a0);               // 00478C7A..00478C81
        else
            gl::glEnableClientState_0073d260(kColorArray_0073f3a0);                // 00478C89..00478C90
    } else {
        gl::glDisableClientState_0073d264(kColorArray_0073f3a0);                   // 00478C98..00478C9E
        base = base_arg;                                                           // 00478CA4
    }
    // switch (textures - 1) via table 00478F34: {0 -> 00478D5A, 1 -> 00478D23, 2 -> 00478CEC, 3 -> 00478CC9}
    const I textures = static_cast<I>(field<I16>(field<void*>(self, 4), 0x182));   // 00478CA8..00478CAB
    const U selector = static_cast<U>(textures - 1);                               // 00478CB2 dec eax
    if (selector <= 3u) {                                                          // 00478CB3..00478CB6 cmp eax,3; ja
        if (selector == 3u) {                                                      // 00478CC9
            gl_render_activate_texture_stage_00425250(3);                          // 00478CC9..00478CCB
            gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x16C) + base)); // 00478CD0..00478CEA
        }
        if (selector >= 2u) {                                                      // 00478CEC
            if (aur_multi_texture_available_0045f710() != 0) (*cells.client_active_texture_007bb6b0)(0x84C2u); // 00478CEC..00478CFA
            *cells.current_stage_007a6898 = 2;                                     // 00478D00
            gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x168) + base)); // 00478D0A..00478D21
        }
        if (selector >= 1u) {                                                      // 00478D23
            if (aur_multi_texture_available_0045f710() != 0) (*cells.client_active_texture_007bb6b0)(0x84C1u); // 00478D23..00478D31
            *cells.current_stage_007a6898 = 1;                                     // 00478D37
            gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x164) + base)); // 00478D41..00478D58
        }
        if (aur_multi_texture_available_0045f710() != 0) (*cells.client_active_texture_007bb6b0)(0x84C0u); // 00478D5A..00478D68
        *cells.current_stage_007a6898 = 0;                                         // 00478D6E
        gl::glTexCoordPointer_0073d29c(2, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x160) + base)); // 00478D78..00478D8F
    }
    if (field<I16>(field<void*>(self, 4), 0x182) < 1)                              // 00478D95..00478DA0
        gl::glDisableClientState_0073d264(kTexCoordArray_0073f398);                // 00478DA2..00478DA9
    else
        gl::glEnableClientState_0073d260(kTexCoordArray_0073f398);                 // 00478DB1..00478DB7
    gl::glNormalPointer_0073d2a4(kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x158) + base)); // 00478DBD..00478DD2
    gl::glVertexPointer_0073d2a8(3, kFloat_1406, field<I>(node, 0x14C), as_pointer(field<U>(node, 0x154) + base)); // 00478DD8..00478DEF
    if (aurora_material_has_bump_map_0047b8f0(field<void*>(trimesh, 0x44), nullptr) != 0 &&   // 00478DF5..00478DFF
        aurora_trimesh_has_tangent_bases_0043e410(field<aurora_model::MdlNode*>(self, 4), nullptr) != 0 && // 00478E05..00478E0F (ecx = [ebp+4])
        kotor_renderer_init::BumpMapAvailable_0045fb10() != 0) {                   // 00478E15..00478E1C
        attrib_pointer(0xC, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base);        // 00478E22..00478E71
        attrib_pointer(0xD, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base + 0xC);  // 00478E71..00478EC2
        attrib_pointer(0xE, 3, kFloat_1406, field<I>(node, 0x14C), field<U>(node, 0x170) + base + 0x18); // 00478EC2..00478F13
        gl_render_activate_texture_stage_00425250(0);                              // 00478F13..00478F15
    }
    return static_cast<U>(field<I>(node, 0x14C)) * static_cast<U>(count) + base;  // 00478F1D..00478F2B imul; add
}

// 00478F50 VertexPrimitiveSkinned::ReconnectArraysVAR(base) (vtable +24, RET 4): Flat arrays, then the bone
// weight (attrib 1, 4 floats) and bone index (attrib 4, 4 shorts) streams.
U __fastcall vertex_primitive_skinned_reconnect_arrays_var_00478f50(void* self, void*, U base_arg) {
    U p = vertex_primitive_flat_reconnect_arrays_var_00478c20(self, nullptr, base_arg); // 00478F50..00478F5E
    if (kotor_renderer_init::VertexProgramsAvailable_0045f770() == 0) return p;    // 00478F60..00478F6D
    const I count = static_cast<I>(field<I16>(field<void*>(self, 4), 0x180));      // 00478F77..00478F7A
    attrib_pointer(1, 4, kFloat_1406, 0, p);                                       // 00478F70..00478FB7
    p += static_cast<U>(count) << 4;                                               // 00478FC2..00478FC7
    attrib_pointer(4, 4, kShort_1402, 0, p);                                       // 00478FBD..00479007
    return p + static_cast<U>(count) * 4u;                                         // 00478FE1 / 0047900D lea eax,[esi+edi*4]
}
}
