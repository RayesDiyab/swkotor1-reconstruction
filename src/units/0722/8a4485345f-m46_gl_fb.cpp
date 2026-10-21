// lane-m46-3dcore fork E: GLRender / texture helpers called by the 3D render core, BSP grass bins, VP lightmap
// paths, construct shadows and the frame-buffer (bloom) area. swkotor.exe GOG K1, image base 00400000; every
// statement cites the original instruction(s) (lane-local d/<addr>.txt, dumpbin /disasm of the pinned image).
//
// Conventions (same as the M45 3d/fb units): opengl32 imports through the single adapter owner
// kotor_renderer_init::<import>_<IAT> (aurora-windows-gl-imports), extension procedures through the typed
// kotor_renderer_gl_procs::<name>_<cell> slots (reloaded at every call, as `call dword ptr [cell]`), renderer cells
// through their owners (renderer_gl_cells.h, framebuffer.h, texture_cells.hpp). .rdata tables are reproduced as
// constexpr arrays with the image words (build/E_pe.py). D30-FINAL: float->int conversions of the original
// `call 006FAE8C` go through the original-ABI port unresolved_ftol2_006fae8c (ST0 -> EDX:EAX); int->float `fild`
// + `fstp dword` is static_cast<float>(int) (round to nearest, identical).
#include <cstdint>
#include <cstring>
#include "056e5913e7-fb_bloom.h"            // lane-3d-fb area header: scene3d.h, renderer_gl_cells/procs, GL adapters, framebuffer.h
#include "16ea3f255d-render_pool.h"         // lane-renderer-bodies: RequestPoolManager (007A68A4, slot +10 GetPool)
#include "708974864c-texture_cells.hpp"     // lane-texture-epoch: max_tex_id_007a46bc, null_texture_007a477c
#include "8839d7f3a5-m46_gl_decls.h"

extern "C" {
// lane-3d-render scene3d.cpp provider (M45 3d-render-scene3d: _aur_multi_texture_available_0045f710).
std::int32_t __cdecl aur_multi_texture_available_0045f710();
}
namespace kotor_renderer_init {
// lane-renderer-bodies extension_loader.h:46 (VBO predicate, zero-arg stdcall == plain RET).
extern std::uint32_t __stdcall VertexBufferAvailable_0045fc90();
}
namespace texture {
extern "C" {
// D21 carrier (naked, defined below): `mov eax,[ecx]; jmp [eax+8]` = the original `mov eax,[ecx]; call [eax+8]`
// of 00420600 with the slot's EAX returned unchanged. The +08 slot is called with no stack arguments.
std::uint32_t __fastcall texture_call_slot_08(void* texture);
}
}

namespace {
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
using U = std::uint32_t;
using I = std::int32_t;

// .rdata 0073F270..0073F28C: blend factor table read as [index*4+0073F270] by 00401674 / 0040167F
// (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO, GL_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_ALPHA,
// GL_DST_ALPHA). The next .rdata word (0073F290) begins the matrix-mode table; every caller passes 0..5.
constexpr U kBlendFactors_0073f270[8] = {0x0302, 0x0303, 0x0001, 0x0000, 0x0300, 0x0306, 0x0305, 0x0304};
// .rdata 0073F2EC..0073F2F4 face table (GL_FRONT, GL_BACK, GL_FRONT_AND_BACK) and 0073F2F8..0073F308 material
// parameter table (GL_SHININESS, GL_AMBIENT, GL_DIFFUSE, GL_EMISSION, GL_SPECULAR), read by 0044F894 / 0044F888.
constexpr U kMaterialFaces_0073f2ec[3] = {0x0404, 0x0405, 0x0408};
constexpr U kMaterialParams_0073f2f8[5] = {0x1601, 0x1200, 0x1201, 0x1600, 0x1202};
// .rdata 0073F334 glBegin primitive table (same words as scene3d.cpp kPrimitiveModes_0073f334).
constexpr U kPrimitiveModes_0073f334[10] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 2};

inline void* handle(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline const void* offset_pointer(U value) { return reinterpret_cast<const void*>(static_cast<std::uintptr_t>(value)); }
inline U field_word(const void* object, U offset) {
    U value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}

// __ftol2 006FAE8C applied to a float loaded by `fld dword` (0042B3F0 / 0042B3F9 / 0042B64F / 0042B658).
I ftol2_float(float value) {
    I converted;
    __asm { fld value }
    __asm { call unresolved_ftol2_006fae8c }
    __asm { mov converted, eax }
    return converted;
}

// GLRender::ActivateTextureStage (client arrays) as inlined in 00425280/004253D0/00426230: optional
// glClientActiveTextureARB(0x84C0 + stage) then the store of 007A6898 (order differs per site; see callers).
inline void client_active_if_multitexture(U unit) {
    if (aur_multi_texture_available_0045f710() != 0)                   // call 0045F710; test eax,eax; je
        procs::glClientActiveTextureARB_007bb6b0(unit);                // push 84C0h/84C1h; call [007BB6B0]
}

// Shared body of 00425280 / 004253D0 (instruction-identical except which node word, +164h or +160h, feeds
// stage 0 and which feeds stage 1).
void set_tex_coords(U pool_index, const void* node, U offset, U stage0_word, U stage1_word) {
    const bool vbo = VertexBufferAvailable_0045fc90() != 0;            // 00425283..00425288
    kotor_renderer_gl::RequestPoolManager* const pool = kotor_renderer_gl_cells::pool_007a68a4;   // 0042528A mov ecx,[007A68A4] (after the call)
    U base;
    if (vbo) {                                                         // 00425290 je
        void* const buffer = pool->GetPool(static_cast<I>(pool_index));   // 00425296..0042529D call [eax+10h]
        procs::glBindBufferARB_007bb7b4(0x8892, word(buffer));         // 004252A0..004252A6 GL_ARRAY_BUFFER_ARB
        base = offset;                                                 // 004252C4 edi = [esp+24h] (offset only)
    } else {
        void* const buffer = pool->GetPool(static_cast<I>(pool_index));   // 00425324..0042532B
        base = word(buffer) + offset;                                  // 00425332..00425334 edi = eax + offset
    }
    client_active_if_multitexture(0x84C0);                             // 004252AC..004252BA / 00425336..00425344
    kotor_renderer_gl_cells::current_stage_007a6898 = 0;               // 004252CE / 00425354
    glTexCoordPointer_0073d29c(2, 0x1406, static_cast<I>(field_word(node, 0x14C)),
        offset_pointer(field_word(node, stage0_word) + base));         // 004252D8..004252EF / 0042535E..00425375
    client_active_if_multitexture(0x84C1);                             // 004252F1..004252FF / 00425377..00425385
    kotor_renderer_gl_cells::current_stage_007a6898 = 1;               // 00425305 / 0042538B
    glTexCoordPointer_0073d29c(2, 0x1406, static_cast<I>(field_word(node, 0x14C)),
        offset_pointer(field_word(node, stage1_word) + base));         // 0042530F..0042531F jmp 004253A5 / 00425395..004253AC
    client_active_if_multitexture(0x84C0);                             // 004253AE..004253BC
    kotor_renderer_gl_cells::current_stage_007a6898 = 0;               // 004253C4
}

// Shared body of the 4-vertex quad loops of 0042B370 / 0042B5D0 / 004337A0 (esi = 0..3): selector words
// first = (v == 0 || v == 3), second = !(v == 0 || v == 1) (test esi / cmp esi,3 / cmp esi,1).
inline bool quad_first(I v) { return v == 0 || v == 3; }
inline bool quad_second(I v) { return !(v == 0 || v == 1); }

// Ortho/viewport prologue shared by both MipmapFilter variants after their make-current call
// (0042B388..0042B432 ATI / 0042B5E7..0042B691 NV): blend on, glBlendFunc(SRC_ALPHA, ONE), projection ortho
// (0,width,0,height,-1,1), modelview identity, viewport (0,0,ftol(width),ftol(height)) with height converted
// first, depth test off, clear colour 0 and colour buffer clear.
void mipmap_prologue(float width, float height) {
    glEnable_0073d390(0x0BE2);                                         // 0042B38E..0042B393 / 0042B5ED..0042B5F2
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042B395..0042B39C / 0042B5F4..0042B5FB
    glMatrixMode_0073d228(0x1701);                                     // 0042B3A8..0042B3AD / 0042B607..0042B60C
    glLoadIdentity_0073d230();                                         // 0042B3B5 / 0042B614
    glOrtho_0073d234(0.0, static_cast<double>(width), 0.0, static_cast<double>(height), -1.0, 1.0); // 0042B3B7..0042B3E1
    glMatrixMode_0073d228(0x1700);                                     // 0042B3E7..0042B3EC / 0042B646..0042B64B
    glLoadIdentity_0073d230();                                         // 0042B3EE / 0042B64D
    const I h = ftol2_float(height);                                   // 0042B3F0..0042B3F4 / 0042B64F..0042B653
    const I w = ftol2_float(width);                                    // 0042B3F9..0042B3FE / 0042B658..0042B65D
    glViewport_0073d2bc(0, 0, w, h);                                   // 0042B403..0042B408 / 0042B662..0042B667
    glDisable_0073d22c(0x0B71);                                        // 0042B40E..0042B413 / 0042B66D..0042B672
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042B419..0042B421 / 0042B678..0042B680
    glClear_0073d21c(0x4000);                                          // 0042B427..0042B42C / 0042B686..0042B68B
}

// Texture unit 0 binding of the pbuffer image (0042B43D..0042B4AE / 0042B69C..0042B70D).
void mipmap_bind(void* pbuffer, U texture) {
    glEnable_0073d390(0x0DE1);                                         // 0042B43D..0042B442 / 0042B69C..0042B6A1
    glBindTexture_0073d240(0x0DE1, texture);                           // 0042B444..0042B454 / 0042B6A3..0042B6B3
    procs::wglBindTexImageARB_007bb7b8(pbuffer, 0x2083);               // 0042B456..0042B460 / 0042B6B5..0042B6BF
    glTexParameteri_0073d23c(0x0DE1, 0x2802, 0x2900);                  // 0042B46C..0042B47B WRAP_S CLAMP
    glTexParameteri_0073d23c(0x0DE1, 0x2803, 0x2900);                  // 0042B47D..0042B48C WRAP_T CLAMP
    glTexParameteri_0073d23c(0x0DE1, 0x2801, 0x2601);                  // 0042B48E..0042B49D MIN LINEAR
    glTexParameteri_0073d23c(0x0DE1, 0x2800, 0x2601);                  // 0042B49F..0042B4AE MAG LINEAR
}
} // namespace

namespace texture {
extern "C" __declspec(naked) std::uint32_t __fastcall texture_call_slot_08(void*) {
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 8]
    }
}
}

extern "C" {

// Original cold value .data 0078D400 = 01 (image word 00000001). Byte cell: writers 004204B2 (0) / 004204C2 (1) /
// 00423650 (0); readers mov al,[0078D400] at 00423268.. (texture ctor 00423150 area; lane-texture-epoch
// texture::providers.default_flag is the consumer view and is currently null - handoff).
volatile std::uint8_t aur_texture_mode_dynamic_0078d400 = 1;

// 00401670 _glBlendFunc(source_index, destination_index), cdecl, plain RET (0040168E).
void __cdecl gl_blend_func_00401670(U source_index, U destination_index) {
    glBlendFunc_0073d38c(kBlendFactors_0073f270[source_index],         // 0040167B..0040167F edx = [esp+4]
        kBlendFactors_0073f270[destination_index]);                    // 00401670..00401674 eax = [esp+8]; 00401688 call [0073D38C]
}

// 0041FEB0 AurTextureGetMaxTexID: mov eax,[007A46BC]; ret.
I __cdecl aur_texture_get_max_tex_id_0041feb0() {
    return static_cast<I>(texture_cells::max_tex_id_007a46bc);         // 0041FEB0
}

// 004204B0 AurTextureSetModeStatic: mov byte ptr [0078D400],0; ret.
void __cdecl aur_texture_set_mode_static_004204b0() {
    aur_texture_mode_dynamic_0078d400 = 0;                             // 004204B0
}

// 004204C0 AurTextureSetModeDynamic: mov byte ptr [0078D400],1; ret.
void __cdecl aur_texture_set_mode_dynamic_004204c0() {
    aur_texture_mode_dynamic_0078d400 = 1;                             // 004204C0
}

// 00420600 CAurTexture::IsNull (__thiscall, RET 0): AL = ([vtable+8]() == [007A477C]); upper EAX bits are the
// slot result in the original (sete al only) - consumers test AL only (std::uint8_t).
std::uint8_t __fastcall aur_texture_is_null_00420600(void* texture, void*) {
    const U reference = texture::texture_call_slot_08(texture);        // 00420600..00420602 call [eax+8]
    return reference == word(texture_cells::null_texture_007a477c.target) ? 1u : 0u;   // 00420605..0042060B cmp; sete al
}

// 00425250 GLRender::ActivateTextureStage(stage) (client arrays), cdecl.
void __cdecl gl_render_activate_client_texture_stage_00425250(U stage) {
    if (aur_multi_texture_available_0045f710() != 0) {                 // 00425250..00425257
        procs::glClientActiveTextureARB_007bb6b0(stage + 0x84C0);      // 0042525A..00425265 lea eax,[esi+84C0h]
        kotor_renderer_gl_cells::current_stage_007a6898 = stage;       // 0042526B
        return;
    }
    kotor_renderer_gl_cells::current_stage_007a6898 = stage;           // 00425273..00425277
}

// 00425280 GLRender::SetLightmapTexCoordToStage0(word_14c, word_150, pool, count, node, offset), cdecl.
// Only pool ([esp+18h]), node ([esp+20h]) and offset ([esp+24h]) are read.
void __cdecl gl_render_set_lightmap_tex_coord_to_stage0_00425280(U, U, U pool_index, I, void* node, U offset) {
    set_tex_coords(pool_index, node, offset, 0x164, 0x160);            // stage 0 <- node+164h (004252D8), stage 1 <- +160h (0042530F)
}

// 004253D0 GLRender::SetDecalTexCoordToStage0: same body with +160h on stage 0 and +164h on stage 1.
void __cdecl gl_render_set_decal_tex_coord_to_stage0_004253d0(U, U, U pool_index, I, void* node, U offset) {
    set_tex_coords(pool_index, node, offset, 0x160, 0x164);            // 00425428 (+160h) / 0042545F (+164h)
}

// 00426230 GLRender::DrawLightmappedGrass(vertices, position_stride, attributes, attribute_stride, first, quads).
// The third argument is not read. Normals start after quads*4 positions of 12 bytes (lea ecx,[ebx+ebx*2]; shl 4).
void __cdecl gl_render_draw_lightmapped_grass_00426230(const void* vertices, U position_stride, const void*,
    I attribute_stride, I first, I quads) {
    const bool vbo = VertexBufferAvailable_0045fc90() != 0;            // 00426234..0042623B
    if (vbo) procs::glBindBufferARB_007bb7b4(0x8892, 0);               // 00426241..00426248
    glVertexPointer_0073d2a8(3, 0x1406, static_cast<I>(position_stride), vertices);   // 0042624E..0042625F / 00426307..00426318
    const unsigned char* const attributes = static_cast<const unsigned char*>(vertices) +
        static_cast<U>(quads) * 0x30u;                                 // 00426265..00426273 / 0042631E..0042632C
    glNormalPointer_0073d2a4(0x1406, attribute_stride, attributes);   // 00426275..0042627C
    glTexCoordPointer_0073d29c(2, 0x1406, attribute_stride, attributes + 0x0C);   // 00426282..00426294
    client_active_if_multitexture(0x84C1);                             // 00426296..004262A4
    kotor_renderer_gl_cells::current_stage_007a6898 = 1;               // 004262B6 (before the call at 004262C0)
    glTexCoordPointer_0073d29c(2, 0x1406, attribute_stride, attributes + 0x14);   // 004262AA..004262C0
    client_active_if_multitexture(0x84C0);                             // 004262C2..004262D0
    kotor_renderer_gl_cells::current_stage_007a6898 = 0;               // 004262E5 (before the call at 004262EF)
    glDrawArrays_0073d2ac(7, first, quads * 4);        // 004262D6..004262EF GL_QUADS, count = quads*4
    if (vbo) procs::glBindBufferARB_007bb7b4(0x8892, 0);               // 004262F5..004262FC (VBO path only)
}

// 0042B370 GLRender::MipmapFilter (ATI fragment shaders), cdecl 7 args (size is not read); tail jmp glFlush.
void __cdecl gl_mipmap_filter_ati_0042b370(void* dc, void* context, float width, float height, void* pbuffer,
    U texture, float) {
    procs::wglMakeContextCurrentARB_007bb818(dc, dc, context);         // 0042B370..0042B382 push context, dc, dc
    mipmap_prologue(width, height);                                    // 0042B388..0042B42C
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042B432..0042B437
    mipmap_bind(pbuffer, texture);                                     // 0042B43D..0042B4AE
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(4, 0, 0, 0, 0);   // 0042B4B0..0042B4BF
    glEnable_0073d390(0x0BE2);                                         // 0042B4C2..0042B4C7
    glBegin_0073d2d0(7);                                               // 0042B4C9..0042B4CB GL_QUADS
    for (I v = 0; v < 4; ++v) {                                        // 0042B4D7 / 0042B585..0042B589
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042B4E0..0042B4F4
        const float t = quad_first(v) ? 1.0f : 0.0f;                   // 0042B4F6..0042B507 -> [esp+10h]
        const float s = quad_second(v) ? 1.0f : 0.0f;                  // 0042B50F..0042B520 -> [esp+14h]
        procs::glMultiTexCoord2fARB_007bb6e8(0x84C0, s, t);            // 0042B528..0042B537
        const float y = quad_first(v) ? height : 0.0f;                 // 0042B53D..0042B554 -> [esp+18h]
        const float x = quad_second(v) ? width : 0.0f;                 // 0042B558..0042B56B -> [esp+1Ch]
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042B573..0042B57F
    }
    glEnd_0073d2c4();                                                  // 0042B58F
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042B595..0042B59A
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042B5A0..0042B5A7
    procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);            // 0042B5A9..0042B5AF
    glFlush_0073d2f8();                                                // 0042B5BC jmp [0073D2F8]
}

// 0042B5D0 GLRender::MipmapFilter (NV register combiners), cdecl 7 args (size is not read); tail jmp glFlush.
void __cdecl gl_mipmap_filter_0042b5d0(void* dc, void* context, float width, float height, void* pbuffer,
    U texture, float) {
    wglMakeCurrent_0073d2b0(dc, context);                              // 0042B5D0..0042B5E1
    mipmap_prologue(width, height);                                    // 0042B5E7..0042B68B
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042B691..0042B696
    mipmap_bind(pbuffer, texture);                                     // 0042B69C..0042B70D
    gl_frame_buffer_modification_pixel_shaders_004299c0(4, 0, 0, 0, 0);   // 0042B70F..0042B71E
    glBegin_0073d2d0(7);                                               // 0042B721..0042B723
    for (I v = 0; v < 4; ++v) {                                        // 0042B72F / 0042B7D1..0042B7D5
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042B731..0042B745
        const float t = quad_first(v) ? 1.0f : 0.0f;                   // 0042B747..0042B758 -> [esp+10h]
        const float s = quad_second(v) ? 1.0f : 0.0f;                  // 0042B760..0042B771 -> [esp+14h]
        glTexCoord2f_0073d2cc(s, t);                                   // 0042B779..0042B783
        const float y = quad_first(v) ? height : 0.0f;                 // 0042B789..0042B7A0 -> [esp+18h]
        const float x = quad_second(v) ? width : 0.0f;                 // 0042B7A4..0042B7B7 -> [esp+1Ch]
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042B7BF..0042B7CB
    }
    glEnd_0073d2c4();                                                  // 0042B7DB
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042B7E1..0042B7E6
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042B7EC..0042B7F3
    procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);            // 0042B7F5..0042B7FB
    glFlush_0073d2f8();                                                // 0042B808 jmp [0073D2F8]
}

// 00431F70 GLRender::RenderObjectsVCUP(mode, vertices, count): 0x10-byte V(12)/C(4 ubyte) vertices.
void __cdecl gl_render_objects_vcup_00431f70(U mode, void* vertices, I count) {
    if (VertexBufferAvailable_0045fc90() != 0) {                       // 00431F71..00431F78
        procs::glBindBufferARB_007bb7b4(0x8892, 0);                    // 00431F7A..00431F81
        glEnableClientState_0073d260(0x8074);                          // 00431F8D..00431F92 GL_VERTEX_ARRAY
        glEnableClientState_0073d260(0x8076);                          // 00431F94..00431F99 GL_COLOR_ARRAY
        glDisableClientState_0073d264(0x8078);                         // 00431FA1..00431FA6 GL_TEXTURE_COORD_ARRAY
        glDisableClientState_0073d264(0x8075);                         // 00431FA8..00431FAD GL_NORMAL_ARRAY
        unsigned char* const vertex = static_cast<unsigned char*>(vertices);
        glColorPointer_0073d2a0(4, 0x1401, 0x10, vertex + 0x0C);       // 00431FAF..00431FC0
        glVertexPointer_0073d2a8(3, 0x1406, 0x10, vertex);             // 00431FC6..00431FD0
        glDrawArrays_0073d2ac(kPrimitiveModes_0073f334[mode], 0, count);   // 00431FD6..00431FE9
        return;
    }
    glBegin_0073d2d0(kPrimitiveModes_0073f334[mode]);                  // 00431FF1..00431FFE
    const unsigned char* vertex = static_cast<const unsigned char*>(vertices);   // 0043200C esi = [esp+10h]
    for (I n = count; n > 0; --n, vertex += 0x10) {                    // 00432004..0043200A test edi,edi; jle / 0043202C dec; jne
        glColor4ubv_0073d2e8(vertex + 0x0C);                           // 00432020..00432024
        glVertex3fv_0073d2e4(reinterpret_cast<const float*>(vertex));  // 00432026..00432027
    }
    glEnd_0073d2c4();                                                  // 00432033 jmp [0073D2C4]
}

// 004337A0 GLRender::RenderFrameToTexture(texture) (NV path): 128x128 rectangle-texture quad into the bloom
// pbuffer context 007A68C8/007A68CC, then back to the main context 007A47E4/007A6854.
void __cdecl gl_render_frame_to_texture_004337a0(U texture) {
    const kotor_renderer_init::FrameBufferBindings& fb = kotor_renderer_init::framebuffer;
    wglMakeCurrent_0073d2b0(*fb.bloom_dc_007a68c8, *fb.bloom_context_007a68cc);   // 004337A0..004337BA
    glMatrixMode_0073d228(0x1701);                                     // 004337BC..004337C7
    glLoadIdentity_0073d230();                                         // 004337C9..004337CF
    glOrtho_0073d234(0.0, 128.0, 0.0, 128.0, -1.0, 1.0);               // 004337D1..004337F5 (40600000:0 = 128.0)
    glViewport_0073d2bc(0, 0, 0x80, 0x80);                             // 004337FB..00433809
    glMatrixMode_0073d228(0x1700);                                     // 0043380F..00433814
    glLoadIdentity_0073d230();                                         // 00433816
    glDisable_0073d22c(0x0B71);                                        // 00433818..0043381D GL_DEPTH_TEST
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 00433823..00433828
    glEnable_0073d390(0x84F5);                                         // 0043382E..00433839 GL_TEXTURE_RECTANGLE_NV
    glBindTexture_0073d240(0x84F5, texture);                           // 0043383B..00433845
    gl_frame_buffer_modification_pixel_shaders_004299c0(4, 0, 0, 0, 0);   // 0043384B..0043385A
    glEnable_0073d390(0x0BE2);                                         // 0043385D..00433862 GL_BLEND
    glBlendFunc_0073d38c(1, 0);                                        // 00433864..00433868 GL_ONE, GL_ZERO
    glBegin_0073d2d0(7);                                               // 0043386E..00433870 GL_QUADS
    for (I v = 0; v < 4; ++v) {                                        // 00433882 / 00433930..00433934
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 00433890..004338A4
        const float t = quad_first(v)                                  // 004338A6..004338BF -> [esp+10h]
            ? static_cast<float>(static_cast<I>(*fb.height_0078d1d8)) : 0.0f;   // fild [0078D1D8]; fstp dword
        const float s = quad_second(v)                                 // 004338C3..004338D8 -> [esp+14h]
            ? static_cast<float>(static_cast<I>(*fb.width_0078d1d4)) : 0.0f;    // fild [0078D1D4]; fstp dword
        glTexCoord2f_0073d2cc(s, t);                                   // 004338E0..004338EA
        const float y = quad_first(v) ? 128.0f : 0.0f;                 // 004338F0..00433901 -> [esp+18h] (43000000h)
        const float x = quad_second(v) ? 128.0f : 0.0f;                // 00433909..0043391A -> [esp+1Ch]
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 00433922..0043392E
    }
    glEnd_0073d2c4();                                                  // 0043393A
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);   // 00433940..0043394F
    glFlush_0073d2f8();                                                // 00433952
    wglMakeCurrent_0073d2b0(handle(*fb.main_dc_007a47e4), handle(*fb.main_context_007a6854));   // 00433958..00433966
}

// 0044F880 glMaterialfv wrapper(face_index, pname_index, params), cdecl.
void __cdecl gl_material_fv_0044f880(U face_index, U pname_index, const float* params) {
    glMaterialfv_0073d360(kMaterialFaces_0073f2ec[face_index],         // 0044F890..0044F894 [eax*4+0073F2ECh]
        kMaterialParams_0073f2f8[pname_index], params);                // 0044F884..0044F888 [ecx*4+0073F2F8h]; 0044F89D
}

} // extern "C"
