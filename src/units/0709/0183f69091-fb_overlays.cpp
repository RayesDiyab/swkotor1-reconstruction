// fb_overlays.cpp - lane-3d-fb (fork C): framebuffer-modification overlay passes of swkotor.exe.
//   00432780 RenderOverlayFilmNoiseATI      00432AD0 RenderOverlayFilmNoise
//   00432D70 RenderForceDistortionATI       004331D0 RenderForceDistortion
//   00433970 RenderAccumulationBlur (ATI)   00433DA0 RenderAccumulationBlur (NV)
//   004340F0 RenderOverlayOverbrighten (NV) 00434240 RenderOverlayOverbrighten (ATI)
//   004343D0 RenderSaturationAdjustmentATI  00434890 RenderSaturationAdjustment
// None executes on the first main-menu frame (callers only inside 00435BD0 / 00436070 / 004361F0 / 00437520,
// themselves gated by Scene+D4; see gates.json). All are __cdecl with plain RET.
// Evidence: lane-local disassembly d/<addr>.txt; the instruction address is cited on every statement.
// GL: kotor_renderer_init adapters (one per original IAT cell; the original's `mov reg,[IAT]; call reg` reuse is
// the same import each time). Extension procs: kotor_renderer_gl_procs slots, reloaded per call.
// D30-FINAL: x87 intermediates are double; float only at the original `fstp dword` stores.
#include "056e5913e7-fb_bloom.h"
#include "283ae0c931-texture_vtable.hpp"   // lane-texture-epoch typed texture tables (+48 / +B4 slots)

namespace {
using namespace kotor_fb;
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
using kotor_scene3d::Vector;

constexpr U kProjection = 0x1701, kModelview = 0x1700;
constexpr U kTexture2D = 0x0DE1, kTextureRectangle = 0x84F5;
constexpr U kTexture0 = 0x84C0, kTexture1 = 0x84C1, kTexture2 = 0x84C2;
constexpr U kFloatOne = 0x3F800000u;       // pushed immediate 3F800000h
constexpr U kFloat7p5 = 0x40F00000u;       // stored immediate 40F00000h (7.5f)

// fild dword: the original loads the window/power cells as signed 32-bit integers.
inline double fild(U value) { return static_cast<double>(static_cast<I>(value)); }
// fild + `test reg,reg; jge; fadd [0073D770]` unsigned fix-up (0073D770 = 4294967296.0f).
inline double fild_unsigned(U value) {
    double result = static_cast<double>(static_cast<I>(value));
    if (static_cast<I>(value) < 0) result += static_cast<double>(f(kFloat2p32_0073d770));
    return result;
}
inline U window_width() { return *kotor_renderer_init::framebuffer.width_0078d1d4; }     // [0078D1D4]
inline U window_height() { return *kotor_renderer_init::framebuffer.height_0078d1d8; }   // [0078D1D8]
inline U power_width() { return *kotor_renderer_init::framebuffer.power_width_007a6890; }   // [007A6890]
inline U power_height() { return *kotor_renderer_init::framebuffer.power_height_007a6894; } // [007A6894]

// glOrtho(0, width, 0, height, -1, 1): pushes 3FF00000/0 (far 1.0), BFF00000/0 (near -1.0), fild height (top),
// 0/0 (bottom), fild width (right), 0/0 (left).
inline void ortho_window() {
    glOrtho_0073d234(0.0, fild(window_width()), 0.0, fild(window_height()), -1.0, 1.0);
}
// glMatrixMode(PROJECTION); glPushMatrix; glLoadIdentity; glOrtho; glMatrixMode(MODELVIEW); glPushMatrix; glLoadIdentity
void push_ortho() {
    glMatrixMode_0073d228(kProjection);
    glPushMatrix_0073d300();
    glLoadIdentity_0073d230();
    ortho_window();
    glMatrixMode_0073d228(kModelview);
    glPushMatrix_0073d300();
    glLoadIdentity_0073d230();
}
// glMatrixMode(PROJECTION); glPopMatrix; glMatrixMode(MODELVIEW); glPopMatrix
void pop_matrices() {
    glMatrixMode_0073d228(kProjection);
    glPopMatrix_0073d2fc();
    glMatrixMode_0073d228(kModelview);
    glPopMatrix_0073d2fc();
}
// GLRender::ActivateTextureStage inlined form: mov [007A6898],n; call 0045F710; test eax,eax; je;
// mov reg,[007A6898]; add reg,84C0h; push reg; call [007BB80C]
void select_stage(U stage) {
    kotor_renderer_gl_cells::current_stage_007a6898 = stage;
    if (aur_multi_texture_available_0045f710() != 0)
        procs::glActiveTextureARB_007bb80c(kotor_renderer_gl_cells::current_stage_007a6898 + kTexture0);
}
// Four glTexParameteri: WRAP_S/WRAP_T = CLAMP (2900h), MIN/MAG = LINEAR (2601h), in this order.
void clamp_linear(U target) {
    glTexParameteri_0073d23c(target, 0x2802, 0x2900);
    glTexParameteri_0073d23c(target, 0x2803, 0x2900);
    glTexParameteri_0073d23c(target, 0x2801, 0x2601);
    glTexParameteri_0073d23c(target, 0x2800, 0x2601);
}
// glTexParameteri WRAP_S / WRAP_T = REPEAT (2901h)
void repeat_st(U target) {
    glTexParameteri_0073d23c(target, 0x2802, 0x2901);
    glTexParameteri_0073d23c(target, 0x2803, 0x2901);
}
inline font::Texture* texture_object(void* object) { return static_cast<font::Texture*>(object); }
// mov ecx,<texture>; mov edx,[ecx]; call [edx+0B4h]  (Bind; live table reloaded)
inline void texture_bind_b4(void* object) {
    font::Texture* t = texture_object(object);
    texture_vtable::bind_tail(t)->sb4(t, nullptr);
}
// mov ecx,<texture>; mov eax,[ecx]; call [eax+48h]  (EAX result unused by every caller here)
inline void texture_slot_48(void* object) {
    font::Texture* t = texture_object(object);
    texture_vtable::head(t)->common.s48(t, nullptr);
}
inline bool first_or_last(U corner) { return corner == 0 || corner == 3; }   // test esi,esi; je / cmp esi,3; je
inline bool first_or_second(U corner) { return corner == 0 || corner == 1; } // test esi,esi; je / cmp esi,1; je
// Saturation matrix build shared by 004343D0 / 00434890 (identical x87 sequence, different stack slots).
struct SaturationRows { Vector first, second, third; };
SaturationRows saturation_rows() {
    const double s = static_cast<double>(fb_saturation_param_007a6884);
    const double a = static_cast<double>(f(kFloat1_0073d764)) - s;               // fld [0073D764]; fsub [007A6884]
    const double blue = static_cast<double>(f(kLumaB_0073f4f8)) * a;             // fld [0073F4F8]; fmul st,st(1)
    const float diag_blue = static_cast<float>(s + blue);                        // fld [007A6884]; fadd st,st(1); fstp
    const float blue_f = static_cast<float>(blue);                               // fst
    const double green = a * static_cast<double>(f(kLumaG_0073f4f4));            // fld st(1); fmul [0073F4F4]
    const float diag_green = static_cast<float>(green + s);                      // fld st(1); fadd [007A6884]; fstp
    const float green_f = static_cast<float>(green);                             // fxch; fstp
    const double red = a * static_cast<double>(f(kLumaR_0073f4f0));              // fxch st(2); fmul [0073F4F0]
    const float red_f = static_cast<float>(red);                                 // fst / fst (two copies)
    const float diag_red = static_cast<float>(red + s);                          // fadd [007A6884]; fstp
    // x87 stack now [blue, green] (unrounded)
    const double p24 = static_cast<double>(fb_saturation_param_0078db24);
    const double p28 = static_cast<double>(fb_saturation_param_0078db28);
    const double p2c = static_cast<double>(fb_saturation_param_0078db2c);
    SaturationRows r;
    r.first.x = static_cast<float>(p24 * diag_blue);                             // fld [0078DB24]; fmul diag_blue; fstp
    r.first.y = static_cast<float>(p24 * green);                                 // fld [0078DB24]; fmul st,st(2) (wide green); fstp
    r.first.z = static_cast<float>(p24 * red_f);                                 // fld [0078DB24]; fmul red_f; fstp
    r.second.x = static_cast<float>(p28 * blue_f);                               // fld [0078DB28]; fmul blue_f
    r.second.y = static_cast<float>(p28 * diag_green);                           // fmul diag_green
    r.second.z = static_cast<float>(p28 * red_f);                                // fmul red_f (second copy)
    const double third_x = p2c * blue;                                           // fld [0078DB2C]; fmul st,st(1) (wide blue); stays on stack
    r.third.x = static_cast<float>(third_x);                                     // fst dword
    r.third.y = static_cast<float>(p2c * green_f);                               // fmul green_f
    r.third.z = static_cast<float>(p2c * diag_red);                              // fmul diag_red
    const float one = f(kFloat1_0073d764);
    // fcomp [0073D764]; fnstsw; test ah,41h; jne -> store 1.0 only when value > 1.0
    if (r.first.x > one) r.first.x = one;
    if (r.first.y > one) r.first.y = one;
    if (r.first.z > one) r.first.z = one;
    if (r.second.x > one) r.second.x = one;
    if (r.second.y > one) r.second.y = one;
    if (r.second.z > one) r.second.z = one;
    if (third_x > static_cast<double>(one)) r.third.x = one;                     // compares the wide x87 value
    if (r.third.y > one) r.third.y = one;
    // Original quirk: the ninth test reloads the (already clamped) third.x slot and clamps third.z.
    if (r.third.x > one) r.third.z = one;
    return r;
}
} // namespace

extern "C" {

// 00432780 GLRender::RenderOverlayFilmNoiseATI(texture, flag)
void __cdecl gl_render_overlay_film_noise_ati_00432780(U texture, U flag) {
    push_ortho();                                                             // 0043278D..004327E1
    glBlendFunc_0073d38c(1, 0);                                               // 004327E3..004327E7
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 004327ED..004327F2
    glEnable_0073d390(kTexture2D);                                            // 004327FE..00432803
    glBindTexture_0073d240(kTexture2D, texture);                              // 00432805..00432815
    clamp_linear(kTexture2D);                                                 // 0043281D..0043285F
    select_stage(1);                                                          // 00432861..00432881
    glEnable_0073d390(kTexture2D);                                            // 00432887..0043288C
    texture_bind_b4(fb_film_noise_texture_007a68d0);                          // 0043288E..00432896
    repeat_st(kTexture2D);                                                    // 0043289C..004328BC
    glEnable_0073d390(0x8920);                                                // 004328BE..004328C3 GL_FRAGMENT_SHADER_ATI
    procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6920); // 004328C5..004328CB
    const float u = static_cast<float>(fild(window_width()) / fild_unsigned(power_width()));   // 004328D1..004328F7
    const float v = static_cast<float>(fild(window_height()) / fild_unsigned(power_height())); // 004328FB..00432913
    glBegin_0073d2d0(7);                                                      // 00432911..00432917 GL_QUADS
    for (U corner = 0; corner < 4; ++corner) {                                // 00432923 / 00432A4C..00432A50
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00432930..00432944
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00432946..0043294B
        const float t0 = first_or_last(corner) ? v : 0.0f;                   // 00432951..00432968 [esp+14]
        const float s0 = first_or_second(corner) ? 0.0f : u;                 // 0043296C..0043297F [esp+18]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00432987..00432996
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 0043299C..004329A1
        const float t1 = first_or_last(corner) ? 0.0f : f(kFloat7p5);        // 004329A7..004329B8 [esp+1C]
        float s1 = 0.0f;                                                      // 004329C9 / 004329D9 [esp+10]
        if (first_or_second(corner) && static_cast<U8>(flag) == 0)            // 004329C0..004329E1 test al,al ([esp+38] byte)
            s1 = f(kFloat7p5);                                                // 004329E3
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, s1, t1);              // 004329EB..004329FA
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00432A00..00432A19
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00432A1D..00432A32
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00432A3A..00432A46
    }
    glEnd_0073d2c4();                                                         // 00432A56
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 00432A5C..00432A6B
    select_stage(0);                                                          // 00432A6E..00432A8E
    glDisable_0073d22c(kTexture2D);                                           // 00432A94..00432A99
    glBindTexture_0073d240(kTexture2D, 0);                                    // 00432A9F..00432AA6
    glFlush_0073d2f8();                                                       // 00432AA8
    glMatrixMode_0073d228(kProjection);                                       // 00432AAE..00432AB3
    glPopMatrix_0073d2fc();                                                   // 00432AB5..00432ABB
    glMatrixMode_0073d228(kModelview);                                        // 00432ABD..00432AC2
    glPopMatrix_0073d2fc();                                                   // 00432AC4
}                                                                             // 00432ACD ret

// 00432AD0 GLRender::RenderOverlayFilmNoise(texture, flag) (NV, rectangle texture)
void __cdecl gl_render_overlay_film_noise_00432ad0(U texture, U flag) {
    glBlendFunc_0073d38c(1, 0);                                               // 00432AD7..00432ADB
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00432AE1..00432AE6
    glEnable_0073d390(kTextureRectangle);                                     // 00432AEC..00432AF7
    glBindTexture_0073d240(kTextureRectangle, texture);                       // 00432AF9..00432B03
    clamp_linear(kTextureRectangle);                                          // 00432B09..00432B51
    select_stage(1);                                                          // 00432B53..00432B73
    glEnable_0073d390(kTexture2D);                                            // 00432B79..00432B7E
    texture_bind_b4(fb_film_noise_texture_007a68d0);                          // 00432B80..00432B88
    repeat_st(kTexture2D);                                                    // 00432B8E..00432BAE
    gl_frame_buffer_modification_pixel_shaders_004299c0(0x32, 0, 0, 0, 0);    // 00432BB0..00432BBF
    glBegin_0073d2d0(7);                                                      // 00432BC2..00432BC4
    const U8 flag_byte = static_cast<U8>(flag);                               // 00432BCA mov bl,[esp+30h]
    for (U corner = 0; corner < 4; ++corner) {                                // 00432BDA / 00432CF8..00432CFC
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00432BE0..00432BF4
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00432BF6..00432BFB
        const float t0 = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00432C01..00432C1A
        const float s0 = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00432C1E..00432C33
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00432C3B..00432C4A
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 00432C50..00432C55
        const float t1 = first_or_last(corner) ? 0.0f : f(kFloat7p5);        // 00432C5B..00432C6C
        float s1 = 0.0f;                                                      // 00432C7D / 00432C89
        if (first_or_second(corner) && flag_byte == 0) s1 = f(kFloat7p5);     // 00432C74..00432C93 test bl,bl
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, s1, t1);              // 00432C9B..00432CAA
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00432CB0..00432CC9
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00432CCD..00432CE2
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00432CEA..00432CF6
    }
    glEnd_0073d2c4();                                                         // 00432D02
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);       // 00432D08..00432D17
    select_stage(0);                                                          // 00432D1A..00432D3C
    glDisable_0073d22c(kTextureRectangle);                                    // 00432D42..00432D47
    glBindTexture_0073d240(kTextureRectangle, 0);                             // 00432D4D..00432D54
    glFlush_0073d2f8();                                                       // 00432D5D jmp [0073D2F8] (tail)
}

// 00432D70 GLRender::RenderForceDistortionATI(texture)
void __cdecl gl_render_force_distortion_ati_00432d70(U texture) {
    push_ortho();                                                             // 00432D7D..00432DD1
    glViewport_0073d2bc(0, 0, static_cast<I>(window_width()), static_cast<I>(window_height())); // 00432DD3..00432DE4
    glEnable_0073d390(0x8620);                                                // 00432DEA..00432DF5 GL_VERTEX_PROGRAM_ARB
    procs::glProgramEnvParameter4fARB_007bb850(0x8620, 0x0F, 0.0f, f(kFloatOne), 0.0f, 0.0f); // 00432DF7..00432E09
    procs::glBindProgramARB_007bb788(0x8620, *kotor_renderer_init::framebuffer.program_007a68d4); // 00432E0F..00432E1B
    texture_slot_48(*kotor_scene3d::cells.distortion_texture_007a68d8);       // 00432E21..00432E29
    glEnable_0073d390(0x0BE2);                                                // 00432E2C..00432E31 GL_BLEND
    glBlendFunc_0073d38c(1, 0);                                               // 00432E33..00432E37
    select_stage(0);                                                          // 00432E3D..00432E5D
    glEnable_0073d390(kTexture2D);                                            // 00432E63..00432E68
    glBindTexture_0073d240(kTexture2D, texture);                              // 00432E6A..00432E7A
    texture_bind_b4(*kotor_scene3d::cells.distortion_texture_007a68d8);       // 00432E7C..00432E84
    clamp_linear(kTexture2D);                                                 // 00432E8A..00432ED2
    procs::glActiveTextureARB_007bb80c(kTexture1);                            // 00432ED4..00432ED9
    glEnable_0073d390(kTexture2D);                                            // 00432EDF..00432EE4
    glBindTexture_0073d240(kTexture2D, 0);                                    // 00432EE6..00432EED
    procs::glActiveTextureARB_007bb80c(kTexture2);                            // 00432EEF..00432EF4
    glEnable_0073d390(kTexture2D);                                            // 00432EFA..00432EFF
    glBindTexture_0073d240(kTexture2D, texture);                              // 00432F01..00432F07
    clamp_linear(kTexture2D);                                                 // 00432F09..00432F4B
    glEnable_0073d390(0x8920);                                                // 00432F4D..00432F52
    procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6924); // 00432F54..00432F5A
    glEnable_0073d390(*kotor_scene3d::cells.gl_stencil_test_0078d470);        // 00432F60..00432F67
    glStencilFunc_0073d298(0x0201, 0, 1);                                     // 00432F69..00432F72 GL_LESS
    glStencilOp_0073d270(0x1E00, 0x1E00, 0x1E00);                             // 00432F78..00432F87 GL_KEEP x3
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00432F8D..00432F92
    const float u = static_cast<float>(fild(window_width()) / fild_unsigned(power_width()));   // 00432F98..00432FBD [esp+1C]
    const float v = static_cast<float>(fild(window_height()) / fild_unsigned(power_height())); // 00432FC1..00432FD9 [esp+18]
    glBegin_0073d2d0(7);                                                      // 00432FD7..00432FDD
    const float v_tenth = static_cast<float>(static_cast<double>(v) * static_cast<double>(f(kFloat0p1_0073e798))); // 00432FE3..00432FFD edi
    const float u_tenth = static_cast<float>(static_cast<double>(u) * static_cast<double>(f(kFloat0p1_0073e798))); // 00432FF9..0043300B ebx
    for (U corner = 0; corner < 4; ++corner) {                                // 00432FF3 / 00433132..00433136
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00433010..00433024
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00433026..0043302B
        const float t0 = first_or_last(corner) ? 0.0f : f(kFloatOne);        // 00433031..00433042 [esp+10]
        const float s0 = first_or_second(corner) ? 0.0f : f(kFloatOne);      // 0043304A..0043305B [esp+14]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00433063..00433072
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 00433078..0043307D
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, u_tenth, v_tenth);    // 00433083..0043308A (push edi; push ebx)
        procs::glActiveTextureARB_007bb80c(kTexture2);                        // 00433090..00433095
        const float t2 = first_or_last(corner) ? v : 0.0f;                   // 0043309B..004330B2 [esp+20]
        const float s2 = first_or_second(corner) ? 0.0f : u;                 // 004330B6..004330C9 [esp+24]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture2, s2, t2);              // 004330D1..004330E0
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 004330E6..004330FF
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00433103..00433118
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00433120..0043312C
    }
    glEnd_0073d2c4();                                                         // 0043313C
    glDisable_0073d22c(*kotor_scene3d::cells.gl_stencil_test_0078d470);       // 00433142..0043314E
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 00433150..0043315F
    glDisable_0073d22c(0x8620);                                               // 00433162..00433167
    procs::glBindProgramARB_007bb788(0x8620, 0);                              // 00433169..00433170
    select_stage(0);                                                          // 00433176..00433196
    glFlush_0073d2f8();                                                       // 0043319C
    pop_matrices();                                                           // 004331A2..004331BE
}                                                                             // 004331C7 ret

// 004331D0 GLRender::RenderForceDistortion(texture) (NV)
void __cdecl gl_render_force_distortion_004331d0(U texture) {
    glEnable_0073d390(0x8620);                                                // 004331D6..004331E1
    procs::glProgramEnvParameter4fARB_007bb850(0x8620, 0x0F, 0.0f, f(kFloatOne), 0.0f, 0.0f); // 004331E3..004331F5
    procs::glBindProgramARB_007bb788(0x8620, *kotor_renderer_init::framebuffer.program_007a68d4); // 004331FB..00433206
    texture_slot_48(*kotor_scene3d::cells.distortion_texture_007a68d8);       // 0043320C..00433214
    glBlendFunc_0073d38c(1, 0);                                               // 00433217..0043321B
    select_stage(0);                                                          // 00433221..0043323F
    glEnable_0073d390(kTexture2D);                                            // 00433245..0043324A
    texture_bind_b4(*kotor_scene3d::cells.distortion_texture_007a68d8);       // 0043324C..00433254
    clamp_linear(kTexture2D);                                                 // 0043325A..004332A2
    select_stage(1);                                                          // 004332A4..004332C2
    glEnable_0073d390(kTexture2D);                                            // 004332C8..004332CD
    glBindTexture_0073d240(kTexture2D, 0);                                    // 004332CF..004332DC
    select_stage(2);                                                          // 004332DE..004332FE
    glBindTexture_0073d240(kTextureRectangle, texture);                       // 00433304..0043330E
    clamp_linear(kTextureRectangle);                                          // 00433310..00433352
    gl_frame_buffer_modification_pixel_shaders_004299c0(0x64, 0, 0, 0, 0);    // 00433354..00433368
    glEnable_0073d390(*kotor_scene3d::cells.gl_stencil_test_0078d470);        // 00433363..0043336C
    glStencilFunc_0073d298(0x0201, 0, 1);                                     // 0043336E..00433377
    glStencilOp_0073d270(0x1E00, 0x1E00, 0x1E00);                             // 0043337D..0043338C
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00433392..00433397
    glDisable_0073d22c(0x0B50);                                               // 0043339D..004333A8 GL_LIGHTING
    glBegin_0073d2d0(7);                                                      // 004333AA..004333AC
    for (U corner = 0; corner < 4; ++corner) {                                // 004333B8 / 00433506..0043350A
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 004333C0..004333D4
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 004333D6..004333DB
        const float t0 = first_or_last(corner) ? 0.0f : f(kFloatOne);        // 004333E1..004333F2 [esp+0C]
        const float s0 = first_or_second(corner) ? 0.0f : f(kFloatOne);      // 004333FA..0043340B [esp+10]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00433413..00433422
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 00433428..0043342D
        const float t1 = static_cast<float>(fild(window_height()) * static_cast<double>(f(kFloat0p1_0073e798))); // 00433433..00433442
        const float s1 = static_cast<float>(fild(window_width()) * static_cast<double>(f(kFloat0p1_0073e798)));  // 00433446..00433452
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, s1, t1);              // 00433455..0043345A
        procs::glActiveTextureARB_007bb80c(kTexture2);                        // 00433460..00433465
        const float t2 = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 0043346B..00433484
        const float s2 = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00433488..0043349D
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture2, s2, t2);              // 004334A5..004334B4
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 004334BA..004334D3
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 004334D7..004334EC
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 004334F4..00433500
    }
    glEnd_0073d2c4();                                                         // 00433510
    glDisable_0073d22c(*kotor_scene3d::cells.gl_stencil_test_0078d470);       // 00433516..0043351D
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);       // 0043351F..0043352E
    glDisable_0073d22c(0x8620);                                               // 00433531..00433536
    procs::glBindProgramARB_007bb788(0x8620, 0);                              // 00433538..0043353F
    select_stage(0);                                                          // 00433545..00433568
    glFlush_0073d2f8();                                                       // 00433571 jmp [0073D2F8] (tail)
}

// 00433970 GLRender::RenderAccumulationBlur (ATI). The texture argument is not read by the body.
void __cdecl gl_render_accumulation_blur_ati_00433970(U /*texture: never read*/) {
    glMatrixMode_0073d228(kProjection);                                       // 00433983..00433988
    glLoadIdentity_0073d230();                                                // 0043398A..00433990
    ortho_window();                                                           // 00433992..004339C0
    glMatrixMode_0073d228(kModelview);                                        // 004339C6..004339CB
    glLoadIdentity_0073d230();                                                // 004339CD
    glViewport_0073d2bc(0, 0, static_cast<I>(window_width()), static_cast<I>(window_height())); // 004339CF..004339E0
    glDisable_0073d22c(0x0B71);                                               // 004339E6..004339F1 GL_DEPTH_TEST
    glBlendFunc_0073d38c(1, 0);                                               // 004339F3..004339F7
    const float ratio = fb_speed_blur_ratio_0078d43c;                         // 004339FD mov edx,[0078D43C]
    Vector blend{ratio, ratio, ratio};                                        // 00433A0C..00433A14 [esp+30..38]
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00433A07..00433A18
    glEnable_0073d390(kTexture2D);                                            // 00433A1E..00433A29
    glBindTexture_0073d240(kTexture2D, *kotor_renderer_init::framebuffer.bloom_texture_007a68c0); // 00433A2B..00433A3D
    procs::wglBindTexImageARB_007bb7b8(*kotor_renderer_init::framebuffer.bloom_pbuffer_007a68c4, 0x2083); // 00433A3F..00433A4A WGL_FRONT_LEFT_ARB
    clamp_linear(kTexture2D);                                                 // 00433A50..00433A98
    procs::glActiveTextureARB_007bb80c(kTexture1);                            // 00433A9A..00433A9F
    glEnable_0073d390(kTexture2D);                                            // 00433AA5..00433AAA
    glBindTexture_0073d240(kTexture2D, *kotor_renderer_init::framebuffer.root_texture_007a687c); // 00433AAC..00433AB8
    clamp_linear(kTexture2D);                                                 // 00433ABA..00433AFC
    glEnable_0073d390(0x8920);                                                // 00433AFE..00433B03
    float w = f(kFloatOne);                                                   // 00433B0F [esp+24] = 1.0
    procs::glSetFragmentShaderConstantATI_007bb5d8(0x8941, homogenize_004aaab0(&blend, &w)); // 00433B05..00433B25 GL_CON_1_ATI
    Vector inverse;
    inverse.x = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.x)); // 00433B2B..00433B3E
    w = 0.0f;                                                                 // 00433B49
    inverse.y = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.y)); // 00433B43..00433B55
    inverse.z = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.z)); // 00433B59..00433B63
    procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, homogenize_004aaab0(&inverse, &w)); // 00433B67..00433B75 GL_CON_2_ATI
    glEnable_0073d390(0x8920);                                                // 00433B7B..00433B80
    procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a691c); // 00433B82..00433B88
    glDisable_0073d22c(0x0B50);                                               // 00433B8E..00433B93
    const float u = static_cast<float>(fild(window_width()) / fild_unsigned(power_width()));   // 00433B95..00433BBB [esp+2C]
    const float v = static_cast<float>(fild(window_height()) / fild_unsigned(power_height())); // 00433BBF..00433BD7 [esp+28]
    glBegin_0073d2d0(7);                                                      // 00433BD5..00433BDB
    for (U corner = 0; corner < 4; ++corner) {                                // 00433BE7 / 00433CF7..00433CFB
        glColor4f_0073d224(f(kFloatOne), 0.0f, f(kFloatOne), f(kFloatOne));   // 00433BF0..00433C01
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00433C03..00433C08
        const float t0 = first_or_last(corner) ? f(kFloatOne) : 0.0f;        // 00433C0E..00433C1F [esp+10]
        const float s0 = first_or_second(corner) ? 0.0f : f(kFloatOne);      // 00433C27..00433C38 [esp+14]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00433C40..00433C4F
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 00433C55..00433C5A
        const float t1 = first_or_last(corner) ? v : 0.0f;                   // 00433C60..00433C77 [esp+18]
        const float s1 = first_or_second(corner) ? 0.0f : u;                 // 00433C7B..00433C8E [esp+1C]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, s1, t1);              // 00433C96..00433CA5
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00433CAB..00433CC4
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00433CC8..00433CDD
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00433CE5..00433CF1
    }
    glEnd_0073d2c4();                                                         // 00433D01
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00433D07..00433D0C
    procs::wglReleaseTexImageARB_007bb6d0(*kotor_renderer_init::framebuffer.bloom_pbuffer_007a68c4, 0x2083); // 00433D12..00433D1E
    glBindTexture_0073d240(kTexture2D, 0);                                    // 00433D24..00433D2B
    glDisable_0073d22c(kTexture2D);                                           // 00433D2D..00433D32
    select_stage(0);                                                          // 00433D34..00433D52
    procs::glActiveTextureARB_007bb80c(kTexture1);                            // 00433D58..00433D5D
    glBindTexture_0073d240(kTexture2D, 0);                                    // 00433D63..00433D6A
    glDisable_0073d22c(kTexture2D);                                           // 00433D6C..00433D71
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00433D73..00433D78
    glDisable_0073d22c(0x8920);                                               // 00433D7E..00433D83
    glFlush_0073d2f8();                                                       // 00433D85
}                                                                             // 00433D92 ret

// 00433DA0 GLRender::RenderAccumulationBlur (NV). The texture argument is not read by the body.
void __cdecl gl_render_accumulation_blur_00433da0(U /*texture: never read*/) {
    glDisable_0073d22c(0x0B71);                                               // 00433DAD..00433DB2
    glBlendFunc_0073d38c(1, 0);                                               // 00433DB4..00433DB8
    const float ratio = fb_speed_blur_ratio_0078d43c;                         // 00433DBE mov eax,[0078D43C]
    Vector blend{ratio, ratio, ratio};                                        // 00433DCC..00433DD4 [esp+28..30]
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 00433DC7..00433DD8
    glEnable_0073d390(kTexture2D);                                            // 00433DDE..00433DE9
    glBindTexture_0073d240(kTexture2D, *kotor_renderer_init::framebuffer.bloom_texture_007a68c0); // 00433DEB..00433DFC
    procs::wglBindTexImageARB_007bb7b8(*kotor_renderer_init::framebuffer.bloom_pbuffer_007a68c4, 0x2083); // 00433DFE..00433E0A
    clamp_linear(kTexture2D);                                                 // 00433E10..00433E58
    procs::glActiveTextureARB_007bb80c(kTexture1);                            // 00433E5A..00433E5F
    glEnable_0073d390(kTextureRectangle);                                     // 00433E65..00433E6A
    glBindTexture_0073d240(kTextureRectangle, *kotor_renderer_init::framebuffer.root_texture_007a687c); // 00433E6C..00433E78
    clamp_linear(kTextureRectangle);                                          // 00433E7A..00433EBC
    gl_frame_buffer_modification_pixel_shaders_004299c0(0x128, 0, 0, 0, 0);   // 00433EBE..00433ECB (args popped at 00433EE7)
    float w = f(kFloatOne);                                                   // 00433ED4 [esp+38] (= [B+24])
    procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852A, homogenize_004aaab0(&blend, &w)); // 00433ED0..00433EF5 COMBINER0 / CONSTANT_COLOR0
    Vector inverse;
    inverse.x = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.x)); // 00433EFB..00433F0E
    w = 0.0f;                                                                 // 00433F19
    inverse.y = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.y)); // 00433F13..00433F25
    inverse.z = static_cast<float>(static_cast<double>(f(kFloat1_0073d764)) - static_cast<double>(blend.z)); // 00433F29..00433F33
    procs::glCombinerStageParameterfvNV_007bb6f8(0x8551, 0x852B, homogenize_004aaab0(&inverse, &w)); // 00433F37..00433F4A COMBINER1 / CONSTANT_COLOR1
    glDisable_0073d22c(0x0B50);                                               // 00433F50..00433F55
    glBegin_0073d2d0(7);                                                      // 00433F57..00433F59
    for (U corner = 0; corner < 4; ++corner) {                                // 00433F65 / 0043407B..0043407F
        glColor4f_0073d224(f(kFloatOne), 0.0f, f(kFloatOne), f(kFloatOne));   // 00433F70..00433F81
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00433F83..00433F88
        const float t0 = first_or_last(corner) ? f(kFloatOne) : 0.0f;        // 00433F8E..00433F9F [esp+10]
        const float s0 = first_or_second(corner) ? 0.0f : f(kFloatOne);      // 00433FA7..00433FB8 [esp+14]
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture0, s0, t0);              // 00433FC0..00433FCF
        procs::glActiveTextureARB_007bb80c(kTexture1);                        // 00433FD5..00433FDA
        const float t1 = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00433FE0..00433FF9
        const float s1 = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00433FFD..00434012
        procs::glMultiTexCoord2fARB_007bb6e8(kTexture1, s1, t1);              // 0043401A..00434029
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 0043402F..00434048
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 0043404C..00434061
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00434069..00434075
    }
    glEnd_0073d2c4();                                                         // 00434085
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);       // 0043408B..0043409A
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 0043409D..004340A2
    glBindTexture_0073d240(kTexture2D, 0);                                    // 004340A8..004340AF
    procs::wglReleaseTexImageARB_007bb6d0(*kotor_renderer_init::framebuffer.bloom_pbuffer_007a68c4, 0x2083); // 004340B1..004340BD
    glDisable_0073d22c(kTexture2D);                                           // 004340C3..004340C8
    procs::glActiveTextureARB_007bb80c(kTexture1);                            // 004340CA..004340CF
    glDisable_0073d22c(kTextureRectangle);                                    // 004340D5..004340DA
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 004340DC..004340E1
}                                                                             // 004340EE ret

// 004340F0 GLRender::RenderOverlayOverbrighten (NV)
void __cdecl gl_render_overlay_overbrighten_004340f0(U texture) {
    const auto& fb = kotor_renderer_init::framebuffer;
    glBlendFunc_0073d38c(1, 0);                                               // 004340F7..004340FB
    gl_frame_buffer_modification_pixel_shaders_004299c0(0x512, texture,
        *fb.bloom_ramps[0], *fb.bloom_ramps[1], *fb.bloom_ramps[2]);          // 00434101..00434124 (007A6864/68/6C)
    glBegin_0073d2d0(7);                                                      // 00434127..00434129
    for (U corner = 0; corner < 4; ++corner) {                                // 00434141 / 004341F9..004341FD (jb)
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00434150..00434164
        const float t = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00434166..0043417F
        const float s = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00434183..00434198
        glTexCoord2f_0073d2cc(s, t);                                          // 004341A0..004341AA
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 004341AC..004341C5
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 004341C9..004341DE
        glVertex4f_0073d324(x, y, 0.0f, f(kFloatOne));                        // 004341E6..004341F7
    }
    glEnd_0073d2c4();                                                         // 00434203
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);       // 00434209..00434218
    glBindTexture_0073d240(kTextureRectangle, 0);                             // 0043421B..00434222
    glDisable_0073d22c(kTextureRectangle);                                    // 00434228..0043422D
}                                                                             // 0043423A ret

// 00434240 GLRender::RenderOverlayOverbrighten (ATI)
void __cdecl gl_render_overlay_overbrighten_ati_00434240(U texture) {
    const auto& fb = kotor_renderer_init::framebuffer;
    glBlendFunc_0073d38c(1, 0);                                               // 00434247..0043424B
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(0x512, texture,
        *fb.bloom_ramps[0], *fb.bloom_ramps[1], *fb.bloom_ramps[2]);          // 00434251..0043426F
    const float u = static_cast<float>(fild(window_width()) / fild_unsigned(power_width()));   // 00434274..0043429D [esp+24]
    const float v = static_cast<float>(fild(window_height()) / fild_unsigned(power_height())); // 004342A1..004342B9 [esp+20]
    glBegin_0073d2d0(7);                                                      // 004342B7..004342BD
    for (U corner = 0; corner < 4; ++corner) {                                // 004342D5 / 00434385..00434389 (jb)
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 004342E0..004342F4
        const float t = first_or_last(corner) ? v : 0.0f;                    // 004342F6..0043430D [esp+10]
        const float s = first_or_second(corner) ? 0.0f : u;                  // 00434311..00434324 [esp+14]
        glTexCoord2f_0073d2cc(s, t);                                          // 0043432C..00434336
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00434338..00434351
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00434355..0043436A
        glVertex4f_0073d324(x, y, 0.0f, f(kFloatOne));                        // 00434372..00434383
    }
    glEnd_0073d2c4();                                                         // 0043438F
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 00434395..004343A4
    glBindTexture_0073d240(kTexture2D, 0);                                    // 004343A7..004343AE
    glDisable_0073d22c(kTexture2D);                                           // 004343B4..004343B9
}                                                                             // 004343C6 ret

// 004343D0 GLRender::RenderSaturationAdjustmentATI(texture)
void __cdecl gl_render_saturation_adjustment_ati_004343d0(U texture) {
    push_ortho();                                                             // 004343DD..00434431
    glEnable_0073d390(0x0BE2);                                                // 00434433..0043443E GL_BLEND
    glBlendFunc_0073d38c(1, 0);                                               // 00434440..00434444
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 0043444A..0043444F
    glEnable_0073d390(kTexture2D);                                            // 00434455..0043445A
    glBindTexture_0073d240(kTexture2D, texture);                              // 0043445C..0043446C
    clamp_linear(kTexture2D);                                                 // 0043446E..004344B6
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(0x16, 0, 0, 0, 0); // 004344B8..004344C2
    SaturationRows rows = saturation_rows();                                  // 004344C7..004346BF ([esp+34]/[28]/[40] rows)
    float w = 0.0f;                                                           // 004346D1 [esp+1C] = 0
    procs::glSetFragmentShaderConstantATI_007bb5d8(0x8941, homogenize_004aaab0(&rows.first, &w));  // 004346C7..004346E7
    w = 0.0f;                                                                 // 004346F7
    procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, homogenize_004aaab0(&rows.second, &w)); // 004346ED..0043470D
    w = 0.0f;                                                                 // 0043471D
    procs::glSetFragmentShaderConstantATI_007bb5d8(0x8943, homogenize_004aaab0(&rows.third, &w));  // 00434713..00434733
    glBegin_0073d2d0(7);                                                      // 00434739..0043473B
    const float u = static_cast<float>(fild(window_width()) / fild_unsigned(power_width()));   // 00434741..00434767 [esp+24]
    const float v = static_cast<float>(fild(window_height()) / fild_unsigned(power_height())); // 0043476B..00434789 [esp+20]
    for (U corner = 0; corner < 4; ++corner) {                                // 00434787 / 00434838..0043483C
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00434790..004347A4
        const float t = first_or_last(corner) ? v : 0.0f;                    // 004347A6..004347BD [esp+10]
        const float s = first_or_second(corner) ? 0.0f : u;                  // 004347C1..004347D4 [esp+14]
        glTexCoord2f_0073d2cc(s, t);                                          // 004347DC..004347E6
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 004347EC..00434805
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00434809..0043481E
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00434826..00434832
    }
    glEnd_0073d2c4();                                                         // 00434842
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 00434848..00434857
    glBindTexture_0073d240(kTexture2D, 0);                                    // 0043485A..00434861
    glDisable_0073d22c(kTexture2D);                                           // 00434863..00434868
    pop_matrices();                                                           // 0043486E..00434884
}                                                                             // 0043488D ret

// 00434890 GLRender::RenderSaturationAdjustment(texture) (NV)
void __cdecl gl_render_saturation_adjustment_00434890(U texture) {
    glBlendFunc_0073d38c(1, 0);                                               // 00434897..0043489B
    glDisable_0073d22c(0x0BE2);                                               // 004348A1..004348A6 GL_BLEND
    procs::glActiveTextureARB_007bb80c(kTexture0);                            // 004348AC..004348B1
    glEnable_0073d390(kTextureRectangle);                                     // 004348B7..004348BC
    glBindTexture_0073d240(kTextureRectangle, texture);                       // 004348C2..004348CC
    clamp_linear(kTextureRectangle);                                          // 004348D2..0043491A
    gl_frame_buffer_modification_pixel_shaders_004299c0(0x16, 0, 0, 0, 0);    // 0043491C..00434926
    SaturationRows rows = saturation_rows();                                  // 0043492B..00434B23 ([esp+2C]/[20]/[38] rows)
    float w = 0.0f;                                                           // 00434B35 [esp+1C] = 0
    procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852A, homogenize_004aaab0(&rows.first, &w));  // 00434B2B..00434B50
    w = 0.0f;                                                                 // 00434B60
    procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852B, homogenize_004aaab0(&rows.second, &w)); // 00434B56..00434B7B
    w = 0.0f;                                                                 // 00434B8B
    procs::glCombinerStageParameterfvNV_007bb6f8(0x8552, 0x852A, homogenize_004aaab0(&rows.third, &w));  // 00434B81..00434BA6
    glBegin_0073d2d0(7);                                                      // 00434BAC..00434BAE
    for (U corner = 0; corner < 4; ++corner) {                                // 00434BC6 / 00434C74..00434C78
        glColor4f_0073d224(f(kFloatOne), f(kFloatOne), f(kFloatOne), f(kFloatOne)); // 00434BD0..00434BE4
        const float t = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00434BE6..00434BFF
        const float s = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00434C03..00434C18
        glTexCoord2f_0073d2cc(s, t);                                          // 00434C20..00434C2A
        const float y = first_or_last(corner) ? static_cast<float>(fild(window_height())) : 0.0f; // 00434C2C..00434C45
        const float x = first_or_second(corner) ? 0.0f : static_cast<float>(fild(window_width())); // 00434C49..00434C5E
        glVertex3f_0073d2c8(x, y, 0.0f);                                      // 00434C66..00434C72
    }
    glEnd_0073d2c4();                                                         // 00434C7E
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0);       // 00434C84..00434C93
    glBindTexture_0073d240(kTextureRectangle, 0);                             // 00434C96..00434C9D
    glDisable_0073d22c(kTextureRectangle);                                    // 00434CA3..00434CA8
}                                                                             // 00434CB5 ret

} // extern "C"
