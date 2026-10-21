// fb_modifications.cpp - lane-3d-fb (fork C): framebuffer-modification drivers of swkotor.exe.
//   00435BD0 FrameBufferModificationsATI     00436070 GLRender::FrameBufferModificationsATI2
//   004361F0 GLRender::FrameBufferModifications2   00437520 GLRender::FrameBufferModifications
// Not executed on the first main-menu frame: RenderSinglePass calls 00437520 / 004361F0 only when Scene+D4 != 0
// (0045194F / 0045197A); the GUI scene keeps +D4 = 0 (gates.json). All are __cdecl(U mask), plain RET.
// Evidence: lane-local disassembly d/<addr>.txt; instruction addresses cited per statement.
#include "056e5913e7-fb_bloom.h"

namespace {
using namespace kotor_fb;
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
using kotor_scene3d::cells;

constexpr U kProjection = 0x1701, kModelview = 0x1700;
constexpr U kTexture2D = 0x0DE1, kTextureRectangle = 0x84F5, kTexture0 = 0x84C0;

inline double fild(U value) { return static_cast<double>(static_cast<I>(value)); }
inline U window_width() { return *kotor_renderer_init::framebuffer.width_0078d1d4; }      // [0078D1D4]
inline U window_height() { return *kotor_renderer_init::framebuffer.height_0078d1d8; }    // [0078D1D8]
inline U power_width() { return *kotor_renderer_init::framebuffer.power_width_007a6890; }   // [007A6890]
inline U power_height() { return *kotor_renderer_init::framebuffer.power_height_007a6894; } // [007A6894]
inline U scene_texture() { return *kotor_renderer_init::framebuffer.root_texture_007a687c; } // [007A687C]

// glMatrixMode(PROJECTION); glPushMatrix; glLoadIdentity; glOrtho(0,w,0,h,-1,1); glMatrixMode(MODELVIEW);
// glPushMatrix; glLoadIdentity  (pushes 3FF00000/0, BFF00000/0, fild height, 0/0, fild width, 0/0)
void push_ortho() {
    glMatrixMode_0073d228(kProjection);
    glPushMatrix_0073d300();
    glLoadIdentity_0073d230();
    glOrtho_0073d234(0.0, fild(window_width()), 0.0, fild(window_height()), -1.0, 1.0);
    glMatrixMode_0073d228(kModelview);
    glPushMatrix_0073d300();
    glLoadIdentity_0073d230();
}
// glCopyTexSubImage2D(target, 0, 0, 0, 0, 0, width, height) (pushes height, width, 0 x5, target)
inline void copy_frame(U target, U width, U height) {
    glCopyTexSubImage2D_0073d308(target, 0, 0, 0, 0, 0, static_cast<I>(width), static_cast<I>(height));
}
// mov [007A6898],0; call 0045F710; test; je; push [007A6898]+84C0h; call [007BB80C]
void select_stage0() {
    kotor_renderer_gl_cells::current_stage_007a6898 = 0;
    if (aur_multi_texture_available_0045f710() != 0)
        procs::glActiveTextureARB_007bb80c(kotor_renderer_gl_cells::current_stage_007a6898 + kTexture0);
}
// Rectangle-texture parameters (WRAP_S/T = CLAMP 2900h, MIN/MAG = LINEAR 2601h)
void clamp_linear(U target) {
    glTexParameteri_0073d23c(target, 0x2802, 0x2900);
    glTexParameteri_0073d23c(target, 0x2803, 0x2900);
    glTexParameteri_0073d23c(target, 0x2801, 0x2601);
    glTexParameteri_0073d23c(target, 0x2800, 0x2601);
}
inline bool mask_bit(U mask, U bit) { return (static_cast<U8>(mask) & bit) != 0; }   // test byte ptr [esp+14h],bit
} // namespace

extern "C" {

// 00435BD0 FrameBufferModificationsATI(mask)
void __cdecl gl_frame_buffer_modifications_ati_00435bd0(U mask) {
    glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                        // 00435BD0..00435BE0
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                        // 00435BE2..00435BE9
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                          // 00435BEB..00435BF2
    glDepthMask_0073d28c(0);                                                  // 00435BF4..00435BF6
    glDisable_0073d22c(*cells.gl_blend_0078d448);                             // 00435BFC..00435C02
    push_ortho();                                                             // 00435C04..00435C5E
    // Speed blur (accumulation) and frame-to-texture
    if (fb_effect_gate_0078d984 != 0) {                                       // 00435C60..00435C79
        if (fb_speed_blur_007a6880 != 0 && mask_bit(mask, 0x10) &&            // 00435C7F..00435C8D
            *kotor_renderer_init::framebuffer.bloom_active_0078d990 == 0 &&   // 00435C8F..00435C96
            fb_ati_frame_rendered_007a698c == 1) {                            // 00435C98 cmp byte,1
            glEnable_0073d390(kTexture2D);                                    // 00435CA1..00435CA6
            glBindTexture_0073d240(kTexture2D, scene_texture());              // 00435CA8..00435CB4
            copy_frame(kTexture2D, power_width(), power_height());            // 00435CB6..00435CD2
            glDisable_0073d22c(kTexture2D);                                   // 00435CD4..00435CD9
            glBindTexture_0073d240(kTexture2D, 0);                            // 00435CDB..00435CE2
            glFlush_0073d2f8();                                               // 00435CE4
            gl_render_accumulation_blur_ati_00433970(scene_texture());        // 00435CEA..00435CF6
        }
        if (fb_effect_gate_0078d984 != 0 && fb_speed_blur_007a6880 != 0 && mask_bit(mask, 0x10)) { // 00435CF9..00435D18
            glEnable_0073d390(kTexture2D);                                    // 00435D1E..00435D23
            glBindTexture_0073d240(kTexture2D, scene_texture());              // 00435D25..00435D31
            copy_frame(kTexture2D, power_width(), power_height());            // 00435D33..00435D4F
            glDisable_0073d22c(kTexture2D);                                   // 00435D51..00435D56
            glBindTexture_0073d240(kTexture2D, 0);                            // 00435D58..00435D5F
            glFlush_0073d2f8();                                               // 00435D61
            glDisable_0073d22c(kTexture2D);                                   // 00435D67..00435D6C
            glBindTexture_0073d240(kTexture2D, 0);                            // 00435D6E..00435D75
            select_stage0();                                                  // 00435D77..00435D97
            gl_render_frame_to_texture_ati_00433580(scene_texture());         // 00435D9D..00435DA8
            fb_ati_frame_rendered_007a698c = 1;                               // 00435DAB
            goto overbrighten;                                                // 00435DB2 jmp 00435DBB
        }
    }
    fb_ati_frame_rendered_007a698c = 0;                                       // 00435DB4
overbrighten:
    if (fb_word_007a68bc != 0 && mask_bit(mask, 0x01)) {                      // 00435DBB..00435DC9
        glEnable_0073d390(kTexture2D);                                        // 00435DCB..00435DD0
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 00435DD2..00435DDE
        copy_frame(kTexture2D, power_width(), power_height());                // 00435DE0..00435DFC
        glDisable_0073d22c(kTexture2D);                                       // 00435DFE..00435E03
        glBindTexture_0073d240(kTexture2D, 0);                                // 00435E05..00435E0C
        glFlush_0073d2f8();                                                   // 00435E0E
        gl_render_overlay_overbrighten_ati_00434240(scene_texture());         // 00435E14..00435E20
    }
    if (fb_effect_gate_0078d974 != 0 && mask_bit(mask, 0x02)) {               // 00435E23..00435E31
        glEnable_0073d390(kTexture2D);                                        // 00435E33..00435E38
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 00435E3A..00435E46
        copy_frame(kTexture2D, power_width(), power_height());                // 00435E48..00435E64
        glDisable_0073d22c(kTexture2D);                                       // 00435E66..00435E6B
        glBindTexture_0073d240(kTexture2D, 0);                                // 00435E6D..00435E74
        glFlush_0073d2f8();                                                   // 00435E76
        gl_render_overlay_convolution_ati_0042c6e0(scene_texture());          // 00435E7C..00435E88
    }
    if (fb_effect_gate_0078d980 != 0 && mask_bit(mask, 0x20)) {               // 00435E8B..00435E99
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00435E9B..00435EA0
        glEnable_0073d390(kTexture2D);                                        // 00435EA6..00435EAB
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 00435EAD..00435EB8
        copy_frame(kTexture2D, power_width(), power_height());                // 00435EBA..00435ED7
        glBindTexture_0073d240(kTexture2D, 0);                                // 00435ED9..00435EE0
        glDisable_0073d22c(kTexture2D);                                       // 00435EE2..00435EE7
        glFlush_0073d2f8();                                                   // 00435EE9..00435EEF
        gl_render_force_distortion_ati_00432d70(scene_texture());             // 00435EF1..00435EFC
    }
    // Scan noise (film noise flag 1). The texture word is loaded once; a null word skips straight to the next
    // block with ECX still holding it (00435F28 je 00435F8A), which is the same value the next block tests.
    void* film_noise = fb_film_noise_texture_007a68d0;                        // 00435F20 / 00435F84
    if (fb_effect_gate_0078d978 != 0 && fb_scan_noise_007a6882 != 0 && mask_bit(mask, 0x04) && // 00435F07..00435F1E
        film_noise != nullptr && aur_texture_is_null_00420600(film_noise, nullptr) == 0) {     // 00435F26..00435F31
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00435F33..00435F38
        glEnable_0073d390(kTexture2D);                                        // 00435F3E..00435F43
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 00435F45..00435F51
        copy_frame(kTexture2D, power_width(), power_height());                // 00435F53..00435F6F
        glFlush_0073d2f8();                                                   // 00435F71
        gl_render_overlay_film_noise_ati_00432780(scene_texture(), 1);        // 00435F73..00435F81
    }
    if (film_noise != nullptr) film_noise = fb_film_noise_texture_007a68d0;   // 00435F84 reload (non-null path only)
    if (fb_effect_gate_0078d988 != 0 && fb_film_noise_flag_007a6883 != 0 && mask_bit(mask, 0x04) && // 00435F8A..00435FA1
        film_noise != nullptr && aur_texture_is_null_00420600(film_noise, nullptr) == 0) {     // 00435FA3..00435FAE
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 00435FB0..00435FB5
        glEnable_0073d390(kTexture2D);                                        // 00435FBB..00435FC0
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 00435FC2..00435FCE
        copy_frame(kTexture2D, power_width(), power_height());                // 00435FD0..00435FEC
        glFlush_0073d2f8();                                                   // 00435FEE
        gl_render_overlay_film_noise_ati_00432780(scene_texture(), 0);        // 00435FF0..00435FFE
    }
    glBlendFunc_0073d38c(kBlendSrcAlpha_0073f270, kBlendOneMinusSrcAlpha_0073f274); // 00436001..0043600B (immediates 302h/303h)
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                         // 00436011..00436017
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                         // 00436019..00436020
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                           // 00436022..00436029
    glDepthMask_0073d28c(1);                                                  // 0043602B..0043602D
    glMatrixMode_0073d228(kModelview);                                        // 00436033..0043603E
    glPopMatrix_0073d2fc();                                                   // 00436040..00436046
    glMatrixMode_0073d228(kProjection);                                       // 00436048..0043604D
    glPopMatrix_0073d2fc();                                                   // 0043604F
    glMatrixMode_0073d228(kModelview);                                        // 00436051..00436056
    *kotor_renderer_init::framebuffer.bloom_active_0078d990 = 0;              // 0043605B mov byte [0078D990],0
}                                                                             // 00436063 ret

// 00436070 GLRender::FrameBufferModificationsATI2(mask) - saturation pass (ATI)
void __cdecl gl_frame_buffer_modifications_ati2_00436070(U mask) {
    if (fb_effect_gate_0078d97c == 0 || fb_saturation_007a6881 == 0 || !mask_bit(mask, 0x08)) // 00436070..0043608F
        return;                                                               // 004361E2 ret
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                        // 00436095..004360A4
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                          // 004360A6..004360AD
    glDepthMask_0073d28c(0);                                                  // 004360AF..004360B1
    glEnable_0073d390(*cells.gl_blend_0078d448);                              // 004360B7..004360C4
    push_ortho();                                                             // 004360C6..00436120
    if (fb_effect_gate_0078d97c != 0 && fb_saturation_007a6881 != 0) {        // 00436126..00436136
        glEnable_0073d390(kTexture2D);                                        // 00436138..0043613D
        glBindTexture_0073d240(kTexture2D, scene_texture());                  // 0043613F..0043614A
        copy_frame(kTexture2D, power_width(), power_height());                // 00436150..0043616D
        glDisable_0073d22c(kTexture2D);                                       // 00436173..00436178
        glBindTexture_0073d240(kTexture2D, 0);                                // 0043617A..00436181
        gl_render_saturation_adjustment_ati_004343d0(scene_texture());        // 00436187..00436192
    }
    glBlendFunc_0073d38c(kBlendSrcAlpha_0073f270, kBlendOneMinusSrcAlpha_0073f274); // 00436195..0043619F
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                         // 004361A5..004361AC
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                         // 004361AE..004361B5
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                           // 004361B7..004361BD
    glDepthMask_0073d28c(1);                                                  // 004361BF..004361C1
    glMatrixMode_0073d228(kProjection);                                       // 004361C7..004361CC
    glPopMatrix_0073d2fc();                                                   // 004361CE..004361D4
    glMatrixMode_0073d228(kModelview);                                        // 004361D6..004361DB
    glPopMatrix_0073d2fc();                                                   // 004361DD
}                                                                             // 004361E2 ret

// 004361F0 GLRender::FrameBufferModifications2(mask) - saturation pass (NV) / ATI2 dispatch
void __cdecl gl_frame_buffer_modifications2_004361f0(U mask) {
    const auto& fb = kotor_renderer_init::framebuffer;
    if (*fb.gate_0078d98c == 0 || *fb.active_007a6860 == 0 || *fb.enabled_0078daf4 == 0) return; // 004361F0..00436211
    if (FrameBufferAvailable_0045f820() == 0) {                               // 00436217..0043621E
        if (ATIFrameBufferAvailable_0045f860() == 0) return;                  // 00436220..00436227
        gl_frame_buffer_modifications_ati2_00436070(mask);                    // 0043622D jmp 00436070 (tail, same argument)
        return;
    }
    if (fb_effect_gate_0078d97c == 0 || fb_saturation_007a6881 == 0 || !mask_bit(mask, 0x08)) return; // 00436232..00436251
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                        // 00436257..00436267
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                          // 00436269..00436270
    glDepthMask_0073d28c(0);                                                  // 00436272..00436274
    glEnable_0073d390(*cells.gl_blend_0078d448);                              // 0043627A..00436286
    push_ortho();                                                             // 00436288..004362E0
    if (fb_effect_gate_0078d97c != 0 && fb_saturation_007a6881 != 0) {        // 004362E6..004362F6
        glEnable_0073d390(kTextureRectangle);                                 // 004362F8..004362FD
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 004362FF..0043630B
        copy_frame(kTextureRectangle, window_width(), window_height());       // 00436311..0043632D
        gl_render_saturation_adjustment_00434890(scene_texture());            // 00436333..0043633F
    }
    gl_blend_func_00401670(0, 1);                                             // 00436342..0043634B (push 1; push 0)
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                         // 0043634B..00436355
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                         // 00436357..0043635D
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                           // 0043635F..00436366
    glDepthMask_0073d28c(1);                                                  // 00436368..0043636A
    glMatrixMode_0073d228(kProjection);                                       // 00436370..00436375
    glPopMatrix_0073d2fc();                                                   // 00436377..0043637D
    glMatrixMode_0073d228(kModelview);                                        // 0043637F..00436384
    glPopMatrix_0073d2fc();                                                   // 00436386
}                                                                             // 0043638B ret

// 00437520 GLRender::FrameBufferModifications(mask) (NV) / ATI dispatch
void __cdecl gl_frame_buffer_modifications_00437520(U mask) {
    const auto& fb = kotor_renderer_init::framebuffer;
    glDisable_0073d22c(*cells.gl_fog_0078d450);                               // 00437520..0043752D (before every gate)
    if (*fb.gate_0078d98c == 0 || *fb.active_007a6860 == 0 || *fb.enabled_0078daf4 == 0) return; // 0043752F..00437550
    if (FrameBufferAvailable_0045f820() == 0) {                               // 00437556..0043755D
        if (ATIFrameBufferAvailable_0045f860() == 0) return;                  // 0043755F..00437566
        gl_frame_buffer_modifications_ati_00435bd0(mask);                     // 0043756C pop esi; 0043756D jmp 00435BD0 (tail)
        return;
    }
    push_ortho();                                                             // 0043757B..004375CF
    glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                        // 004375D1..004375D8
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                        // 004375DA..004375E0
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                          // 004375E2..004375E9
    glDepthMask_0073d28c(0);                                                  // 004375EB..004375ED
    glDisable_0073d22c(*cells.gl_blend_0078d448);                             // 004375F3..004375FA
    if (fb_effect_gate_0078d984 != 0) {                                       // 004375FC..00437615
        if (fb_speed_blur_007a6880 != 0 && mask_bit(mask, 0x10) &&            // 0043761B..00437629
            *fb.bloom_active_0078d990 == 0 && fb_frame_rendered_007a698d == 1) { // 0043762B..0043763B
            glEnable_0073d390(kTextureRectangle);                             // 0043763D..00437642
            glBindTexture_0073d240(kTextureRectangle, scene_texture());       // 00437644..0043764F
            copy_frame(kTextureRectangle, window_width(), window_height());   // 00437651..0043766E
            glBindTexture_0073d240(kTextureRectangle, 0);                     // 00437670..00437677
            glDisable_0073d22c(kTextureRectangle);                            // 00437679..0043767E
            glFlush_0073d2f8();                                               // 00437680
            gl_render_accumulation_blur_00433da0(scene_texture());            // 00437686..00437691
        }
        if (fb_effect_gate_0078d984 != 0 && fb_speed_blur_007a6880 != 0 && mask_bit(mask, 0x10)) { // 00437694..004376AF
            procs::glActiveTextureARB_007bb80c(kTexture0);                    // 004376B1..004376B6
            glEnable_0073d390(kTextureRectangle);                             // 004376BC..004376C1
            glBindTexture_0073d240(kTextureRectangle, scene_texture());       // 004376C3..004376CF
            copy_frame(kTextureRectangle, window_width(), window_height());   // 004376D1..004376ED
            glBindTexture_0073d240(kTextureRectangle, 0);                     // 004376EF..004376F6
            glDisable_0073d22c(kTextureRectangle);                            // 004376F8..004376FD
            glFlush_0073d2f8();                                               // 004376FF
            kotor_renderer_gl::ActivateTextureStage_0041fe80(0);              // 00437705..00437707
            gl_render_frame_to_texture_004337a0(scene_texture());             // 0043770C..00437718 (add esp,8 pops both)
            fb_frame_rendered_007a698d = 1;                                   // 0043771B
            goto overbrighten;                                                // 00437722 jmp 0043772B
        }
    }
    fb_frame_rendered_007a698d = 0;                                           // 00437724
overbrighten:
    if (fb_word_007a68bc != 0 && mask_bit(mask, 0x01)) {                      // 0043772B..00437739
        glEnable_0073d390(kTextureRectangle);                                 // 0043773B..00437740
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 00437742..0043774E
        copy_frame(kTextureRectangle, window_width(), window_height());       // 00437750..0043776C
        glDisable_0073d22c(kTextureRectangle);                                // 0043776E..00437773
        glBindTexture_0073d240(kTextureRectangle, 0);                         // 00437775..0043777C
        glFlush_0073d2f8();                                                   // 0043777E
        gl_render_overlay_overbrighten_004340f0(scene_texture());             // 00437784..00437790
    }
    if (fb_effect_gate_0078d974 != 0 && mask_bit(mask, 0x02)) {               // 00437793..004377A1
        glEnable_0073d390(kTextureRectangle);                                 // 004377A3..004377A8
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 004377AA..004377B5
        copy_frame(kTextureRectangle, window_width(), window_height());       // 004377B7..004377D4
        glDisable_0073d22c(kTextureRectangle);                                // 004377D6..004377DB
        glBindTexture_0073d240(kTextureRectangle, 0);                         // 004377DD..004377E4
        glFlush_0073d2f8();                                                   // 004377E6
        gl_render_overlay_convolution_0042cc60(scene_texture());              // 004377EC..004377F7
    }
    if (fb_effect_gate_0078d980 != 0 && mask_bit(mask, 0x20)) {               // 004377FA..00437808
        procs::glActiveTextureARB_007bb80c(kTexture0);                        // 0043780A..0043780F
        glEnable_0073d390(kTextureRectangle);                                 // 00437815..0043781A
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 0043781C..00437828
        copy_frame(kTextureRectangle, window_width(), window_height());       // 0043782A..00437846
        glBindTexture_0073d240(kTextureRectangle, 0);                         // 00437848..0043784F
        glDisable_0073d22c(kTextureRectangle);                                // 00437851..00437856
        gl_render_force_distortion_004331d0(scene_texture());                 // 00437858..00437864 (no glFlush here)
    }
    void* film_noise = fb_film_noise_texture_007a68d0;                        // 00437892 / 00437935
    if (fb_effect_gate_0078d978 != 0 && fb_scan_noise_007a6882 != 0 && mask_bit(mask, 0x04) && // 00437867..0043788C
        film_noise != nullptr && aur_texture_is_null_00420600(film_noise, nullptr) == 0) {     // 00437898..004378A7
        glEnable_0073d390(kTextureRectangle);                                 // 004378AD..004378B2
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 004378B4..004378C0
        copy_frame(kTextureRectangle, window_width(), window_height());       // 004378C2..004378DE
        clamp_linear(kTextureRectangle);                                      // 004378E0..00437922
        gl_render_overlay_film_noise_00432ad0(scene_texture(), 1);            // 00437924..00437932
    }
    if (film_noise != nullptr) film_noise = fb_film_noise_texture_007a68d0;   // 00437935 reload (0043789A je 0043793B keeps ECX = 0)
    if (fb_effect_gate_0078d988 != 0 && fb_film_noise_flag_007a6883 != 0 && mask_bit(mask, 0x04) && // 0043793B..0043795A
        film_noise != nullptr && aur_texture_is_null_00420600(film_noise, nullptr) == 0) {     // 00437960..0043796F
        glEnable_0073d390(kTextureRectangle);                                 // 00437975..0043797A
        glBindTexture_0073d240(kTextureRectangle, scene_texture());           // 0043797C..00437987
        copy_frame(kTextureRectangle, window_width(), window_height());       // 00437989..004379A6
        clamp_linear(kTextureRectangle);                                      // 004379A8..004379EA
        gl_render_overlay_film_noise_00432ad0(scene_texture(), 0);            // 004379EC..004379F9
    }
    glBlendFunc_0073d38c(kBlendSrcAlpha_0073f270, kBlendOneMinusSrcAlpha_0073f274); // 004379FC..00437A06 (immediates 302h/303h)
    glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                         // 00437A0C..00437A13
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                         // 00437A15..00437A1C
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                           // 00437A1E..00437A24
    glDepthMask_0073d28c(1);                                                  // 00437A26..00437A28
    *fb.bloom_active_0078d990 = 0;                                            // 00437A39 mov byte [0078D990],0
    glMatrixMode_0073d228(kProjection);                                       // 00437A2E..00437A40
    glPopMatrix_0073d2fc();                                                   // 00437A42..00437A48
    glMatrixMode_0073d228(kModelview);                                        // 00437A4A..00437A4F
    glPopMatrix_0073d2fc();                                                   // 00437A51
}                                                                             // 00437A57 ret

} // extern "C"
