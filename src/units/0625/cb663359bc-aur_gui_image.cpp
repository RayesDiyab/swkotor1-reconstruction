// lane-gui-render: CAurGUIImageInternal (vtable 74183C) draw entries, CAurGUIImage::FlushBuffer,
// the quad renderers 004599B0 (+14) / 0045B290 (+20) and the Material GUI helpers 0047AF00 /
// 0047B000 (D49: lane-gui-render owns the 2D pixel path). Every statement cites the original
// instruction(s) it reproduces (GOG swkotor.exe 9c10e045...91435).
//
// Float discipline (D30-FINAL): the original runs x87 at PC_53 / round-to-nearest, so every wide
// intermediate is a `double`; `float` appears only where the original stores with FSTP m32.
// FRNDINT/FISTP is reproduced as x87 FRNDINT/FISTP (GR-4). _ftol 006FAE8C is the M40 port
// (unresolved_ftol2_006fae8c, world-timer-x87-conversion.obj). A raw DWORD that the original moves
// through FLD m32 / FSTP m32 (packed colour, fill rotation) goes through x87_move(), which keeps the
// exact x87 effect (signalling NaN patterns are quietened).
//
// D2-style deviations (documented): the original reads uninitialised stack bytes when
// (flags & 0x0C) == 0x0C (00459A53 / 0045B3A9 fall-through) or when the rotation index is > 3
// (0045B3E0 ja); those locals are value-initialised to 0 here.
#include "881508e8f6-gui_render.h"
#include "6ce637f12f-gui_cells.h"
#include "5fa24cf659-texture_vtable.hpp"   // lane-texture-epoch E1 typed texture tables, material_owner::Material,
                                // texture_make_current_00421ee0 (scheduler.h)
#include "2069aa4ef1-renderer_gl_procs.h"
#include <cstring>
#include <intrin.h>

using gui_render::I32;
using gui_render::Word;

namespace kotor_renderer_init {
// Single GL adapter owner (declarations identical to lane-renderer-bodies renderer_gl_imports_gen.h;
// glEnable/glDisable are the M40 aurora-windows-gl-imports.obj definitions).
void __stdcall glDisable_0073d22c(std::uint32_t);
void __stdcall glEnable_0073d390(std::uint32_t);
extern void __stdcall glColor4f_0073d224(float, float, float, float);
extern void __stdcall glLoadIdentity_0073d230();
extern void __stdcall glAlphaFunc_0073d238(std::uint32_t, float);
extern void __stdcall glTranslatef_0073d284(float, float, float);
extern void __stdcall glRotatef_0073d288(float, float, float, float);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glTexCoord2f_0073d2cc(float, float);
extern void __stdcall glBegin_0073d2d0(std::uint32_t);
extern void __stdcall glColor4ubv_0073d2e8(const std::uint8_t*);
extern void __stdcall glPopMatrix_0073d2fc();
extern void __stdcall glPushMatrix_0073d300();
extern void __stdcall glVertex4f_0073d324(float, float, float, float);
extern void __stdcall glScalef_0073d374(float, float, float);
extern void __stdcall glBlendFunc_0073d38c(std::uint32_t, std::uint32_t);
}

extern "C" {
// lane-3d-mesh trimesh_f1.cpp (SUBLANES "trimesh_skin" owner; declaration as lane-3d-render scene3d.h).
void __fastcall material_bind_texture0_0047adf0(void* material, void*);
// lane-upload upload.h (exact copy).
std::uint32_t __cdecl aur_texture_get_tid_00420bf0(void*);
// lane-gui-draw writers (task-6 M30 r5 vector_equal.cpp): cdecl, full EAX 0/1.
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// M40 world-timer-x87-conversion.obj: original _ftol, argument ST0, result EDX:EAX.
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
// This lane (aur_gui_quad.cpp): GLRender quad renderers.
void __cdecl kotor_cdecl_00431eb0(float* buffer, I32 count);
void __cdecl kotor_cdecl_00431cc0(float* buffer, I32 count);
}

namespace kotor_gui_render_cells {
// 007B94D0 aurGUIBuffer float[2048] (.data tail, loader zero; next object noGUIRender 007BB4D0).
float gui_buffer_007b94d0[2048];
// 007BB4D4 guiBufferSizeUsed (float count; loader zero).
volatile I32 gui_buffer_used_007bb4d4;
// 007BB4D8 CAurGUIImage::guiTID (loader zero): written 0045B335, read 0045B2D5.
volatile std::uint32_t gui_tid_007bb4d8;
// 007BB4DC CAurGUIImage::material (loader zero).
void* volatile gui_material_007bb4dc;
// 007BB4E0 CAurGUIImage::isVTC BYTE (loader zero).
volatile std::uint8_t gui_is_vtc_007bb4e0;
// 00798A8C .data DWORD 1 (file bytes 01 00 00 00), no writer: alpha-test gate read only by
// 0047AFB8 (Material::SetBlendingMode, lane-3d-mesh) and 0047B023 (SetGUIBlendingMode).
volatile std::uint32_t alpha_test_gate_00798a8c = 1u;
// 00798A90 .data float 0.35f (file bytes 33 33 B3 3E), no writer: alpha reference read only by
// 0047AFD3 and 0047B03C (pushed as a raw DWORD).
volatile std::uint32_t alpha_test_ref_00798a90 = 0x3EB33333u;
}

namespace {
using material_owner::Material;
using Texture = font::Texture;

void* P(const void* p, unsigned off) {
    void* v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
Word W(const void* p, unsigned off) {
    Word v;
    _ReadWriteBarrier();
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4);
    _ReadWriteBarrier();
    return v;
}
float F(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
Word B(float f) { Word w; std::memcpy(&w, &f, 4); return w; }
Material* material_of(void* image) { return static_cast<Material*>(P(image, 4)); }   // [this+4]
Texture* texture_of(void* image) { return material_of(image)->textures[0].target; }  // [[this+4]]
// CExoArrayList<int> element 0 ([list.data]); object-pointer view of the stored address (D24).
I32 first_word(const kotor_reconstruction::ArrayMutations& list) {
    I32 v;
    std::memcpy(&v, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(list.data)), 4);
    return v;
}
I32 ftol_006fae8c(double value) {
    I32 result;
    __asm {
        fld qword ptr value
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
// FLD m32 / FSTP m32 of a raw DWORD (0045B69D/0045B6A1 etc.; 00416D76/00416D89 pattern).
Word x87_move(Word bits) {
    Word result;
    __asm {
        fld dword ptr bits
        fstp dword ptr result
    }
    return result;
}
// Viewport alpha product 1.0 * alpha[0..index] (0045A1BB..0045A1DA, 00459D7B..00459D9A,
// 00459DAD..00459DCA, 00459F2F..00459F4C, 0045B5F8..0045B61A): double chain at PC_53.
double alpha_product() {
    const short index = kotor_renderer_cells::viewport_index_007b9460;
    double product = static_cast<double>(kotor_renderer_cells::one_0073d764);
    for (int i = 0; i <= index; ++i)
        product *= static_cast<double>(kotor_renderer_cells::viewport_alphas_007b9248[i]);
    return product;
}
// FLD m32; FMUL 255.0 (0073EDE4); FSTP m32; FLD; FRNDINT; FISTP m32.
I32 channel(double wide) {
    const float stored = static_cast<float>(wide * static_cast<double>(kotor_renderer_cells::channel_0073ede4));
    // GR-4: the original FLD m32; FRNDINT; FISTP m32 is kept as those three instructions (rounding by the
    // live x87 RC, nearest per D30-FINAL; out of range / NaN -> integer indefinite 0x80000000).
    I32 result;
    __asm {
        fld dword ptr stored
        frndint
        fistp dword ptr result
    }
    return result;
}
// SHL/OR packing (00459F70..00459F8D, 0045B63E..0045B65F): no masking of out-of-range channels.
Word pack(I32 r, I32 g, I32 b, I32 a) {
    Word v = static_cast<Word>(a);
    v = (v << 8) | static_cast<Word>(b);
    v = (v << 8) | static_cast<Word>(g);
    v = (v << 8) | static_cast<Word>(r);
    return v;
}
void current_viewport(I32& width, I32& height) {   // MOVSX words [idx*10+007B946C/007B946E]
    const short index = kotor_renderer_cells::viewport_index_007b9460;
    const unsigned base = static_cast<unsigned>(static_cast<int>(index)) * 5u;
    std::int16_t w, h;
    std::memcpy(&w, &kotor_renderer_cells::viewport_frame_words_007b9468[base + 2], 2);
    std::memcpy(&h, &kotor_renderer_cells::viewport_frame_words_007b9468[base + 3], 2);
    width = w;
    height = h;
}
// FILD/FDIV/FSTP m32 (0045982A..0045986F / 00459954..00459999).
Word normalize(I32 value, I32 extent) {
    return B(static_cast<float>(static_cast<double>(value) / static_cast<double>(extent)));
}
using NormalizedDrawSlot = void (__fastcall*)(void* self, void* unused_edx, Word x, Word y, Word w,
                                              Word h, Word flags, Word angle_bits, const void* color,
                                              Word alpha_bits);
void normalized_call(void* self, unsigned offset, I32 x, I32 y, I32 w, I32 h, Word flags,
                     Word angle_bits, const void* color, Word alpha_bits, I32 vw, I32 vh) {
    const Word nh = normalize(h, vh);   // original evaluation order h, w, y, x
    const Word nw = normalize(w, vw);
    const Word ny = normalize(y, vh);
    const Word nx = normalize(x, vw);
    NormalizedDrawSlot fn;
    std::memcpy(&fn, static_cast<const unsigned char*>(P(self, 0)) + offset, sizeof fn);
    fn(self, nullptr, nx, ny, nw, nh, flags, angle_bits, color, alpha_bits);
}
// Corner order bytes from flags & 0x0C (00459A05..00459A53 / 0045B356..0045B3A4).
void corner_order(Word flags, std::uint8_t order[4]) {
    switch (flags & 0x0Cu) {
    case 0x0: order[0] = 0; order[1] = 1; order[2] = 2; order[3] = 3; break;
    case 0x4: order[0] = 1; order[1] = 0; order[2] = 3; order[3] = 2; break;
    case 0x8: order[0] = 3; order[1] = 2; order[2] = 1; order[3] = 0; break;
    default: break;   // 0x0C: original leaves the bytes uninitialised (D2: zero here)
    }
}
// Quarter-turn permutation writes, in the original store order.
void quarter_turn(const std::uint8_t order[4], std::uint8_t perm[4], int turn) {
    switch (turn) {
    case 0: perm[0] = order[0]; perm[1] = order[1]; perm[2] = order[2]; perm[3] = order[3]; break;
    case 1: perm[order[0]] = 1; perm[order[1]] = 2; perm[order[2]] = 3; perm[order[3]] = 0; break;
    case 2: perm[order[0]] = 2; perm[order[1]] = 3; perm[order[2]] = 0; perm[order[3]] = 1; break;
    case 3: perm[order[0]] = 3; perm[order[1]] = 0; perm[order[2]] = 1; perm[order[3]] = 2; break;
    default: break;   // 0045B3E0 ja: uninitialised in the original (D2: zero here)
    }
}
// Negative angle + 360.0 (00459A57..00459A72 / 0045B3A9..0045B3C4: TEST AH,5; JP -> only if a < 0).
float wrap_angle(float angle) {
    if (angle < 0.0f)
        return static_cast<float>(static_cast<double>(angle) + 360.0);
    return angle;
}
void buffer_put(I32 index, Word bits) {   // stores into aurGUIBuffer
    std::memcpy(&kotor_gui_render_cells::gui_buffer_007b94d0[index], &bits, 4);
}
constexpr Word kOneBits = 0x3F800000u;   // MOV EDX,3F800000h / MOV EBX,3F800000h
}

extern "C" {

// 0047AF00 Material::GetTextureTID (thiscall, RET): push [ecx] (texture slot 0); call 00420BF0.
std::uint32_t __fastcall kotor_fastcall_0047af00(void* material, void*) {
    return aur_texture_get_tid_00420bf0(static_cast<Material*>(material)->textures[0].target);
}

// 0047B000 Material::SetGUIBlendingMode (thiscall, RET).
void __fastcall kotor_fastcall_0047b000(void* material, void*) {
    // .rdata 0073F270 blend-factor table, 8 words (same table and domain as 0047AF70; lane-3d-mesh copy).
    static constexpr std::uint32_t kBlendFactor_0073f270[8] = {0x302u, 0x303u, 0x1u, 0x0u,
                                                                0x300u, 0x306u, 0x305u, 0x304u};
    constexpr std::uint32_t kGreater_0073f2b0 = 0x204u;   // .rdata GL_GREATER
    Material* const m = static_cast<Material*>(material);
    const std::uint32_t dst = kBlendFactor_0073f270[first_word(m->modes)];    // 0047B003..0047B008 ([esi+34h])
    const std::uint32_t src = kBlendFactor_0073f270[first_word(m->states)];   // 0047B00F..0047B015 ([esi+28h])
    kotor_renderer_init::glBlendFunc_0073d38c(src, dst);                      // 0047B01D
    if (kotor_gui_render_cells::alpha_test_gate_00798a8c == 0) return;        // 0047B023..0047B02A
    if (first_word(m->states) == 2 && first_word(m->modes) == 3) {            // 0047B02C..0047B03A
        kotor_renderer_init::glAlphaFunc_0073d238(kGreater_0073f2b0,
                                                  F(kotor_gui_render_cells::alpha_test_ref_00798a90));   // 0047B03C..0047B049
        return;
    }
    kotor_renderer_init::glAlphaFunc_0073d238(kGreater_0073f2b0, F(0u));      // 0047B051..0047B05A (push 0)
}

// 0045A090 CAurGUIImageInternal::GetImageHeight: tail-jump texture vtbl+4C.
I32 __fastcall kotor_fastcall_0045a090(void* self, void*) {
    Texture* const t = texture_of(self);
    return texture_vtable::head(t)->common.s4c(t, nullptr);
}

// 0045A0A0 CAurGUIImageInternal::GetImageWidth: tail-jump texture vtbl+48.
I32 __fastcall kotor_fastcall_0045a0a0(void* self, void*) {
    Texture* const t = texture_of(self);
    return texture_vtable::head(t)->common.s48(t, nullptr);
}

// 00459920 CAurGUIImageInternal::Draw(int x,y,w,h, flags, angle, color*, alpha), RET 20h.
void __fastcall kotor_fastcall_00459920(void* self, void*, I32 x, I32 y, I32 w, I32 h, Word flags,
                                        Word angle_bits, const void* color, Word alpha_bits) {
    if (kotor_renderer_cells::busy_007bb4d0 != 0) return;                     // 00459921..00459928
    I32 vw, vh;
    current_viewport(vw, vh);                                                 // 0045992A..0045993D
    kotor_renderer_cells::reset_007a689c = 0;                                 // 00459948
    normalized_call(self, 0x14, x, y, w, h, flags, angle_bits, color, alpha_bits, vw, vh);   // 004599A2
}

// 00459800 CAurGUIImageInternal::DrawBuffered(int x,y,w,h, flags, angle, color*, alpha), RET 20h.
void __fastcall kotor_fastcall_00459800(void* self, void*, I32 x, I32 y, I32 w, I32 h, Word flags,
                                        Word angle_bits, const void* color, Word alpha_bits) {
    if (kotor_renderer_cells::busy_007bb4d0 != 0) return;                     // 00459801..00459808
    I32 vw, vh;
    current_viewport(vw, vh);                                                 // 0045980A..0045981D
    normalized_call(self, 0x20, x, y, w, h, flags, angle_bits, color, alpha_bits, vw, vh);   // 00459878
}

// 0045A170 CAurGUIImage::FlushBuffer(float alpha), cdecl, RET.
void __cdecl kotor_cdecl_0045a170(Word alpha_bits) {
    using namespace kotor_gui_render_cells;
    if (gui_buffer_used_007bb4d4 > 0) {                                       // 0045A170..0045A177
        kotor_renderer_init::glDisable_0073d22c(kotor_renderer_cells::disable_0078d45c);   // 0045A17D..0045A183
        kotor_renderer_cells::reset_007a689c = 0;                             // 0045A189
        texture_scheduler::texture_make_current_00421ee0();                                      // 0045A193 (EAX unused)
        if (gui_material_007bb4dc != nullptr) {                               // 0045A198..0045A1A0
            material_bind_texture0_0047adf0(gui_material_007bb4dc, nullptr);  // 0045A1A2
            kotor_fastcall_0047b000(gui_material_007bb4dc, nullptr);          // 0045A1A7..0045A1AD
        }
        if (gui_is_vtc_007bb4e0 == 0) {                                       // 0045A1B2..0045A1B9
            const float alpha = static_cast<float>(alpha_product() * static_cast<double>(F(alpha_bits)));   // 0045A1BB..0045A1E1
            kotor_renderer_init::glColor4f_0073d224(1.0f, 1.0f, 1.0f, alpha); // 0045A1E4..0045A1F3
            kotor_cdecl_00431eb0(gui_buffer_007b94d0, gui_buffer_used_007bb4d4 / 5);    // 0045A1F9..0045A215
        } else {
            kotor_cdecl_00431cc0(gui_buffer_007b94d0, gui_buffer_used_007bb4d4 / 6);    // 0045A21C..0045A236
        }
        kotor_renderer_init::glEnable_0073d390(kotor_renderer_cells::disable_0078d45c);    // 0045A23B..0045A245
    }
    gui_buffer_used_007bb4d4 = 0;                                             // 0045A24B
}

// 004599B0 CAurGUIImageInternal::Draw on normalized coordinates (vtable +14), RET 20h.
// Immediate-mode textured quad; renders every visible GUI fill.
void __fastcall kotor_fastcall_004599b0(void* self, void*, Word x_bits, Word y_bits, Word w_bits,
                                        Word h_bits, Word flags, Word angle_bits, const void* color,
                                        Word alpha_bits) {
    namespace gl = kotor_renderer_init;
    if (kotor_renderer_cells::busy_007bb4d0 != 0) return;                     // 004599B3..004599C1
    {   // 004599C7..004599F1: four texture metric calls, results discarded.
        Texture* t = texture_of(self);
        texture_vtable::head(t)->common.s58(t, nullptr);                      // 004599D3 GetNumX
        texture_vtable::head(t)->common.s48(t, nullptr);                      // 004599DA GetWidth
        t = texture_of(self);
        texture_vtable::head(t)->common.s5c(t, nullptr);                      // 004599EA GetNumY
        texture_vtable::head(t)->common.s4c(t, nullptr);                      // 004599F1 GetHeight
    }
    texture_scheduler::texture_make_current_00421ee0();                                          // 004599F4
    material_bind_texture0_0047adf0(material_of(self), nullptr);              // 004599F9..00459A00
    std::uint8_t order[4] = {0, 0, 0, 0};
    corner_order(flags, order);                                               // 00459A05..00459A53
    std::uint8_t perm[4] = {0, 0, 0, 0};
    const float x = F(x_bits), y = F(y_bits), w = F(w_bits), h = F(h_bits);
    const float angle = wrap_angle(F(angle_bits));                            // 00459A57..00459A72
    gl::glPushMatrix_0073d300();                                              // 00459A76
    if (angle == 0.0f) {                                                      // 00459A7C..00459A8B
        quarter_turn(order, perm, 0);
    } else if (angle == 90.0f) {                                              // 00459AAE (0073E40C)
        quarter_turn(order, perm, 1);
    } else if (angle == 180.0f) {                                             // 00459AEA (0073E408)
        quarter_turn(order, perm, 2);
    } else if (angle == 270.0f) {                                             // 00459B26 (0073E404)
        quarter_turn(order, perm, 3);
    } else {                                                                  // 00459B62..00459CDC
        quarter_turn(order, perm, 0);
        const double half_w = static_cast<double>(w) * 0.5;                   // 00459B62..00459B6A (0073E9AC)
        const float tx = static_cast<float>(-static_cast<double>(x) - half_w); // 00459B74..00459B90 -> [esp+24h]
        const float ty = static_cast<float>(half_w - (1.0 - static_cast<double>(y)));   // 00459B94..00459BA0 -> [esp+10h]
        gl::glLoadIdentity_0073d230();                                        // 00459BA6
        gl::glTranslatef_0073d284(-tx, -ty, 0.0f);                            // 00459BAC..00459BC9
        gl::glRotatef_0073d288(angle, 0.0f, 0.0f, 1.0f);                      // 00459BCB..00459BD7
        const I32 degrees = ftol_006fae8c(static_cast<double>(angle));        // 00459BE3..00459BEF
        const double t = static_cast<double>(degrees % 180) *
                         static_cast<double>(0x1.6c16c2p-7f);                 // 00459BF4..00459C04 (00741864)
        const double s = (t > 1.0) ? (1.0 - t) : 0.0;                         // 00459C0A..00459C21
        const float c = static_cast<float>(s + t);                            // 00459C2E..00459C3C [esp+1Ch]
        I32 vw, vh;
        current_viewport(vw, vh);                                             // 00459C27..00459C40
        float q = 1.0f;                                                       // 00459BE7 [esp+20h]
        double k = 1.0;                                                       // 00459BDD
        if (static_cast<std::int16_t>(vh) < static_cast<std::int16_t>(vw)) {  // 00459C47..00459C4C
            q = static_cast<float>(static_cast<double>(vh) / static_cast<double>(vw));   // 00459C4E..00459C66
            k = 1.0 / static_cast<double>(q);                                 // 00459C6A..00459C70
        } else if (vh > vw) {                                                 // 00459C76
            k = static_cast<double>(vw) / static_cast<double>(vh);            // 00459C7A..00459C8C
            q = static_cast<float>(1.0 / k);                                  // 00459C90..00459C98
        }
        const double one_minus_c = 1.0 - static_cast<double>(c);              // 00459C9C..00459CA7
        const float sy = static_cast<float>(static_cast<double>(c) * static_cast<double>(q) + one_minus_c);   // 00459CAE..00459CB8
        const float sx = static_cast<float>(static_cast<double>(c) * k + one_minus_c);                       // 00459CBC..00459CC4
        gl::glScalef_0073d374(sx, sy, 1.0f);                                  // 00459CA2..00459CCB
        gl::glTranslatef_0073d284(tx, ty, 0.0f);                              // 00459CD1..00459CDC
    }
    gl::glDisable_0073d22c(kotor_renderer_cells::disable_0078d45c);           // 00459CDE..00459CE5
    kotor_fastcall_0047b000(material_of(self), nullptr);                      // 00459CEB..00459CF2
    const std::uint32_t quads = kotor_renderer_cells::quads_0073f34c;         // 00459CFC
    float tc[4][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
    tc[perm[0]][0] = 0.0f; tc[perm[0]][1] = 1.0f;                             // 00459CF7..00459D0E
    tc[perm[1]][0] = 0.0f; tc[perm[1]][1] = 0.0f;                             // 00459D12..00459D1E
    tc[perm[2]][0] = 1.0f; tc[perm[2]][1] = 0.0f;                             // 00459D22..00459D2E
    tc[perm[3]][0] = 1.0f; tc[perm[3]][1] = 1.0f;                             // 00459D32..00459D3F
    gl::glBegin_0073d2d0(quads);                                              // 00459D43
    const Word none[3] = {0xBF800000u, 0xBF800000u, 0xBF800000u};             // 00459D53..00459D63
    if (aurora_vector_equal_004aa980(color, none) != 0) {                     // 00459D6B..00459D75
        const float a = static_cast<float>(alpha_product() * static_cast<double>(F(alpha_bits)));   // 00459D7B..00459DA1
        gl::glColor4f_0073d224(1.0f, 1.0f, 1.0f, a);                          // 00459DA4..00459DA7
        const I32 alpha_byte = ftol_006fae8c(alpha_product() * static_cast<double>(F(alpha_bits)) *
                                             static_cast<double>(kotor_renderer_cells::channel_0073ede4));   // 00459DAD..00459DD6
        const Word packed = (static_cast<Word>(alpha_byte) << 24) | 0x00FFFFFFu;   // 00459DDB..00459DE2
        gl::glColor4ubv_0073d2e8(reinterpret_cast<const std::uint8_t*>(&packed)); // 00459DE7..00459DEC
    } else {
        const float* rgb = static_cast<const float*>(color);
        const I32 r = channel(static_cast<double>(rgb[0]));                   // 00459EC1..00459EE3
        const I32 g = channel(static_cast<double>(rgb[1]));                   // 00459EE5..00459F08
        const I32 b = channel(static_cast<double>(rgb[2]));                   // 00459F0A..00459F2D
        const I32 a = channel(alpha_product() * static_cast<double>(F(alpha_bits)));   // 00459F2F..00459F6E
        const Word packed = pack(r, g, b, a);                                 // 00459F70..00459F94
        gl::glColor4ubv_0073d2e8(reinterpret_cast<const std::uint8_t*>(&packed)); // 00459F98
    }
    const float top = static_cast<float>(1.0 - static_cast<double>(y));                                   // 1 - y
    const float bottom = static_cast<float>(1.0 - (static_cast<double>(y) + static_cast<double>(h)));     // 1 - (y + h)
    const float right = static_cast<float>(static_cast<double>(x) + static_cast<double>(w));              // x + w
    gl::glTexCoord2f_0073d2cc(tc[0][0], tc[0][1]);                            // 00459DF2..00459E02
    gl::glVertex4f_0073d324(x, top, 0.0f, 1.0f);                              // 00459E04..00459E24
    gl::glTexCoord2f_0073d2cc(tc[1][0], tc[1][1]);                            // 00459E26..00459E30
    gl::glVertex4f_0073d324(x, bottom, 0.0f, 1.0f);                           // 00459E32..00459E50
    gl::glTexCoord2f_0073d2cc(tc[2][0], tc[2][1]);                            // 00459E52..00459E5C
    gl::glVertex4f_0073d324(right, bottom, 0.0f, 1.0f);                       // 00459E5E..00459E88
    gl::glTexCoord2f_0073d2cc(tc[3][0], tc[3][1]);                            // 00459E8A..00459E94
    gl::glVertex4f_0073d324(right, top, 0.0f, 1.0f);                          // 00459E96..0045A068
    gl::glEnd_0073d2c4();                                                     // 0045A06A
    gl::glPopMatrix_0073d2fc();                                               // 0045A070
    gl::glEnable_0073d390(kotor_renderer_cells::disable_0078d45c);            // 0045A076..0045A07D
}

// 0045B290 CAurGUIImageInternal::DrawBuffered on normalized coordinates (vtable +20), RET 20h.
// Appends one quad (VT: 4 x {x,y,0,u,v}; VTC: 4 x {x,y,0,rgba,u,v}) to aurGUIBuffer, flushing first
// when the texture, vertex format or alpha changes or the buffer is full.
void __fastcall kotor_fastcall_0045b290(void* self, void*, Word x_bits, Word y_bits, Word w_bits,
                                        Word h_bits, Word flags, Word angle_bits, const void* color,
                                        Word alpha_bits) {
    using namespace kotor_gui_render_cells;
    if (kotor_renderer_cells::busy_007bb4d0 != 0) return;                     // 0045B293..0045B2A1
    const std::uint8_t colored =
        (W(color, 0) == 0xBF800000u && W(color, 4) == 0xBF800000u && W(color, 8) == 0xBF800000u)
            ? 0 : 1;                                                          // 0045B2A7..0045B2C5 (bit compares)
    const std::uint32_t tid = kotor_fastcall_0047af00(material_of(self), nullptr);   // 0045B2CA..0045B2D3
    bool flush = true;
    if (tid == gui_tid_007bb4d8 && colored == gui_is_vtc_007bb4e0 &&
        F(alpha_bits) == F(gui_image_alpha_0078e43c)) {                       // 0045B2D5..0045B2F8
        const I32 used = gui_buffer_used_007bb4d4;                            // 0045B2FC
        flush = colored != 0 ? !(used + 0x18 < 0x800) : !(used + 0x14 < 0x800);   // 0045B302..0045B319
    }
    if (flush) {                                                              // 0045B31B..0045B351
        kotor_cdecl_0045a170(gui_image_alpha_0078e43c);
        gui_tid_007bb4d8 = tid;
        gui_material_007bb4dc = material_of(self);
        gui_is_vtc_007bb4e0 = colored;
        gui_image_alpha_0078e43c = alpha_bits;
    }
    std::uint8_t order[4] = {0, 0, 0, 0};
    corner_order(flags, order);                                               // 0045B356..0045B3A4
    const float angle = wrap_angle(F(angle_bits));                            // 0045B3A9..0045B3C4
    const I32 turn = ftol_006fae8c((static_cast<double>(angle) + 45.0) *
                                   static_cast<double>(0x1.6c16c2p-7f));      // 0045B3C8..0045B3D8 (00741874, 00741864)
    std::uint8_t perm[4] = {0, 0, 0, 0};
    quarter_turn(order, perm, static_cast<std::uint32_t>(turn) <= 3u ? turn : -1);   // 0045B3DD..0045B483
    const float x = F(x_bits), y = F(y_bits), w = F(w_bits), h = F(h_bits);
    const Word top = B(static_cast<float>(1.0 - static_cast<double>(y)));
    const Word bottom = B(static_cast<float>(1.0 - (static_cast<double>(y) + static_cast<double>(h))));
    const Word left = x87_move(x_bits);                                       // FLD/FSTP m32 of x (0045B492/0045B4C7/0045B507, 0045B642/0045B6AE)
    const Word right = B(static_cast<float>(static_cast<double>(x) + static_cast<double>(w)));
    if (colored == 0) {                                                       // 0045B488..0045B48C
        const I32 base = gui_buffer_used_007bb4d4;                            // esi
        buffer_put(base + 0, left);                                           // 0045B49B
        buffer_put(base + 1, top);                                            // 0045B4B9
        buffer_put(base + 2, 0);                                              // 0045B4C0
        buffer_put(base + 5 * perm[0] + 3, 0);                                // 0045B4CB
        buffer_put(base + 5 * perm[0] + 4, kOneBits);                         // 0045B4D1
        buffer_put(base + 5, left);                                           // 0045B4DC
        buffer_put(base + 6, bottom);                                         // 0045B4F9
        buffer_put(base + 7, 0);                                              // 0045B500
        buffer_put(base + 5 * perm[1] + 3, 0);                                // 0045B50B
        buffer_put(base + 5 * perm[1] + 4, 0);                                // 0045B515
        buffer_put(base + 10, right);                                         // 0045B523
        buffer_put(base + 11, bottom);                                        // 0045B531
        buffer_put(base + 12, 0);                                             // 0045B538
        buffer_put(base + 5 * perm[2] + 3, kOneBits);                         // 0045B53F
        buffer_put(base + 5 * perm[2] + 4, 0);                                // 0045B545
        buffer_put(base + 15, right);                                         // 0045B550
        buffer_put(base + 16, top);                                           // 0045B55A
        buffer_put(base + 17, 0);                                             // 0045B561
        gui_buffer_used_007bb4d4 = base + 0x14;                               // 0045B56D..0045B570
        buffer_put(base + 5 * perm[3] + 3, kOneBits);                         // 0045B577
        buffer_put(base + 5 * perm[3] + 4, kOneBits);                         // 0045B57D
        return;
    }
    const float* rgb = static_cast<const float*>(color);
    const I32 r = channel(static_cast<double>(rgb[0]));                       // 0045B58A..0045B5AC
    const I32 g = channel(static_cast<double>(rgb[1]));                       // 0045B5AE..0045B5D1
    const I32 b = channel(static_cast<double>(rgb[2]));                       // 0045B5D3..0045B5F6
    const I32 a = channel(alpha_product() * static_cast<double>(F(alpha_bits)));   // 0045B5F8..0045B63C
    const Word packed = pack(r, g, b, a);                                     // 0045B63E..0045B666
    const I32 base = gui_buffer_used_007bb4d4;                                // 0045B66A
    buffer_put(base + 0, left);                                               // 0045B675
    buffer_put(base + 1, top);                                                // 0045B68F
    buffer_put(base + 2, 0);                                                  // 0045B696
    buffer_put(base + 3, x87_move(packed));                                   // 0045B69D..0045B6A1
    buffer_put(base + 6 * perm[0] + 4, 0);                                    // 0045B6A8
    buffer_put(base + 6 * perm[0] + 5, kOneBits);                             // 0045B6B2
    buffer_put(base + 6, left);                                               // 0045B6AE..0045B6B8
    buffer_put(base + 7, bottom);                                             // 0045B6DB
    buffer_put(base + 8, 0);                                                  // 0045B6E2
    buffer_put(base + 9, x87_move(packed));                                   // 0045B6E9..0045B6ED
    buffer_put(base + 6 * perm[1] + 4, 0);                                    // 0045B6F4
    buffer_put(base + 6 * perm[1] + 5, 0);                                    // 0045B6FE
    buffer_put(base + 12, right);                                             // 0045B713
    buffer_put(base + 13, bottom);                                            // 0045B71F
    buffer_put(base + 14, 0);                                                 // 0045B726
    buffer_put(base + 15, x87_move(packed));                                  // 0045B72D..0045B731
    buffer_put(base + 6 * perm[2] + 4, kOneBits);                             // 0045B738
    buffer_put(base + 6 * perm[2] + 5, 0);                                    // 0045B73E
    buffer_put(base + 18, right);                                             // 0045B749
    buffer_put(base + 19, top);                                               // 0045B753
    buffer_put(base + 20, 0);                                                 // 0045B75A
    buffer_put(base + 21, x87_move(packed));                                  // 0045B761..0045B768
    buffer_put(base + 6 * perm[3] + 4, kOneBits);                             // 0045B775
    buffer_put(base + 6 * perm[3] + 5, kOneBits);                             // 0045B77B
    gui_buffer_used_007bb4d4 = base + 0x18;                                   // 0045B772..0045B781
}

}   // extern "C"
