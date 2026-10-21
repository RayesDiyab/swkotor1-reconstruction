// fb_soft_shadows.cpp - lane-3d-fb (fork B): GLRender::SetUpSoftShadows 0042D1C0, FinishSoftShadowsATI 0042D250,
// FinishSoftShadows 0042D9A0 (+ the unlabeled ATI copy block 0042D140 that 0042D1C9 tail-jumps into).
// NOT executed on the first main-menu frame: the only caller Scene::RenderShadows 00451A50 (00451B21 / 00451C04) is
// reached only when Scene+D4 != 0 (00451746), and Scene+D4 is 0 for the GUI scene (gates.json).
// Soft-shadow cells are borrowed through lane-renderer-bodies' kotor_soft_shadow::bindings descriptor.
#include <cstdint>
#include "056e5913e7-fb_bloom.h"

extern "C" {
// Identical local prototypes to fb_bloom_conv.cpp (definitions there).
void __cdecl gl_bloom_filter_ati_0042b810(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size, kotor_fb::U flag);
void __cdecl gl_bloom_filter_0042bf80(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size, kotor_fb::U flag);
}

namespace {
using namespace kotor_fb;
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
using kotor_scene3d::cells;

inline void* handle(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline I width_i() { return static_cast<I>(*kotor_soft_shadow::bindings.width_0078d1d4); }   // fild / mov [0078D1D4]
inline I height_i() { return static_cast<I>(*kotor_soft_shadow::bindings.height_0078d1d8); } // fild / mov [0078D1D8]

// glOrtho(0, width, 0, height, -1, 1) with fild height then fild width (e.g. 0042D34C..0042D37A).
void ortho_window() {
    const double top = static_cast<double>(height_i());
    const double right = static_cast<double>(width_i());
    glOrtho_0073d234(0.0, right, 0.0, top, -1.0, 1.0);
}

// 0042D140 (unlabeled block between 0042CC60 and 0042D1C0; target of 0042D1C9 jmp): ATI scratch copy.
void soft_shadow_copy_ati_0042d140() {
    const auto& b = kotor_soft_shadow::bindings;
    glEnable_0073d390(0x0DE1);                                         // 0042D141..0042D146
    glBindTexture_0073d240(0x0DE1, *b.scratch_007a68f8);               // 0042D14C..0042D15D
    glCopyTexSubImage2D_0073d308(0x0DE1, 0, 0, 0, 0, 0, static_cast<I>(*b.power_width_007a6890),
        static_cast<I>(*b.power_height_007a6894));                     // 0042D15F..0042D17C
    glDisable_0073d22c(0x0DE1);                                        // 0042D182..0042D187
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042D18D..0042D194
    glFlush_0073d2f8();                                                // 0042D196
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042D19C..0042D1A4
    glClear_0073d21c(0x4000);                                          // 0042D1AA..0042D1AF
}                                                                      // 0042D1B6 ret
} // namespace

extern "C" {

// 0042D1C0 GLRender::SetUpSoftShadows, cdecl no args.
void __cdecl gl_set_up_soft_shadows_0042d1c0() {
    const auto& b = kotor_soft_shadow::bindings;
    if (kotor_soft_shadow::ATISoftAvailable_0045f7e0() != 0) {         // 0042D1C0..0042D1C7
        soft_shadow_copy_ati_0042d140();                               // 0042D1C9 jmp 0042D140 (tail)
        return;
    }
    glEnable_0073d390(0x84F5);                                         // 0042D1CF..0042D1D4 GL_TEXTURE_RECTANGLE_NV
    glBindTexture_0073d240(0x84F5, *b.scratch_007a68f8);               // 0042D1DA..0042D1EB
    glCopyTexSubImage2D_0073d308(0x84F5, 0, 0, 0, 0, 0, width_i(), height_i()); // 0042D1ED..0042D20A
    glDisable_0073d22c(0x84F5);                                        // 0042D210..0042D215
    glBindTexture_0073d240(0x84F5, 0);                                 // 0042D21B..0042D222
    glFlush_0073d2f8();                                                // 0042D224
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042D22A..0042D232
    glClear_0073d21c(0x4000);                                          // 0042D238..0042D23D
}                                                                      // 0042D244 ret

// 0042D250 GLRender::FinishSoftShadowsATI, cdecl no args (EBP frame, esp aligned to 8 at 0042D253).
void __cdecl gl_finish_soft_shadows_ati_0042d250() {
    const auto& b = kotor_soft_shadow::bindings;
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                 // 0042D259..0042D269 (ebp = [0073D22C])
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 0042D26B..0042D278 (esi = [0073D390])
    glDisable_0073d22c(0x0B90);                                        // 0042D27A..0042D27F GL_STENCIL_TEST
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042D281..0042D286
    glEnable_0073d390(0x0DE1);                                         // 0042D28C..0042D291
    glBindTexture_0073d240(0x0DE1, *b.scene_007a687c);                 // 0042D293..0042D2A5 (edi = [0073D240])
    glCopyTexSubImage2D_0073d308(0x0DE1, 0, 0, 0, 0, 0, static_cast<I>(*b.power_width_007a6890),
        static_cast<I>(*b.power_height_007a6894));                     // 0042D2A7..0042D2C3
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042D2C9..0042D2D0
    glDisable_0073d22c(0x0DE1);                                        // 0042D2D2..0042D2D7
    glFlush_0073d2f8();                                                // 0042D2D9
    glClear_0073d21c(0x4000);                                          // 0042D2DF..0042D2E4
    procs::wglMakeContextCurrentARB_007bb818(*b.dc1_007a68ec, *b.dc1_007a68ec, *b.context1_007a68f0); // 0042D2EA..0042D2F8
    glDrawBuffer_0073d304(0x0404);                                     // 0042D2FE..0042D303
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042D309..0042D30E
    glEnable_0073d390(0x0DE1);                                         // 0042D314..0042D319
    glBindTexture_0073d240(0x0DE1, *b.scene_007a687c);                 // 0042D31B..0042D326
    glDisable_0073d22c(0x0B50);                                        // 0042D328..0042D32D GL_LIGHTING
    glMatrixMode_0073d228(0x1701);                                     // 0042D32F..0042D33A (esi = [0073D228])
    glPushMatrix_0073d300();                                           // 0042D33C..0042D342 (edi = [0073D300])
    glLoadIdentity_0073d230();                                         // 0042D344..0042D34A (ebx = [0073D230])
    ortho_window();                                                    // 0042D34C..0042D37A
    glMatrixMode_0073d228(0x1700);                                     // 0042D380..0042D385
    glPushMatrix_0073d300();                                           // 0042D387
    glLoadIdentity_0073d230();                                         // 0042D389
    glDisable_0073d22c(0x0B71);                                        // 0042D38B..0042D390
    glDepthMask_0073d28c(0);                                           // 0042D392..0042D394
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042D39A..0042D3A8
    const float scale_s = static_cast<float>(static_cast<double>(width_i()) /
        static_cast<double>(*b.power_width_007a6890));                 // 0042D3AE..0042D3D4 (unsigned fixup 0073D770) -> L28
    const float scale_t = static_cast<float>(static_cast<double>(height_i()) /
        static_cast<double>(*b.power_height_007a6894));                // 0042D3D8..0042D3F0 -> L24
    glBegin_0073d2d0(7);                                               // 0042D3EE..0042D3F4
    for (I v = 0; v < 4; ++v) {                                        // ebp 0042D3FA / 0042D4AC..0042D4B0
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042D400..0042D414
        const float t = (v == 0 || v == 3) ? scale_t : 0.0f;          // 0042D41A..0042D431 -> L14
        const float s = (v == 0 || v == 1) ? 0.0f : scale_s;          // 0042D435..0042D448 -> L18
        glTexCoord2f_0073d2cc(s, t);                                   // 0042D450..0042D45A
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042D460..0042D479 -> L1C
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042D47D..0042D492 -> L20
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042D49A..0042D4A6
    }
    glEnd_0073d2c4();                                                  // 0042D4B6
    glMatrixMode_0073d228(0x1701);                                     // 0042D4BC..0042D4C1
    glPopMatrix_0073d2fc();                                            // 0042D4C3..0042D4C9 (ebp = [0073D2FC])
    glMatrixMode_0073d228(0x1700);                                     // 0042D4CB..0042D4D0
    glPopMatrix_0073d2fc();                                            // 0042D4D2
    {
        void* const main_dc = handle(*b.main_dc_007a47e4);             // 0042D4DA
        procs::wglMakeContextCurrentARB_007bb818(main_dc, main_dc, handle(*b.main_context_007a6854)); // 0042D4D4..0042D4E1
    }
    glDrawBuffer_0073d304(0x0405);                                     // 0042D4E7..0042D4EC
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042D4F2..0042D4F7
    glEnable_0073d390(0x0DE1);                                         // 0042D4FD..0042D502
    glBindTexture_0073d240(0x0DE1, *b.scratch_007a68f8);               // 0042D508..0042D514
    glDisable_0073d22c(0x0B50);                                        // 0042D51A..0042D51F
    glMatrixMode_0073d228(0x1701);                                     // 0042D525..0042D52A
    glPushMatrix_0073d300();                                           // 0042D52C
    glLoadIdentity_0073d230();                                         // 0042D52E
    ortho_window();                                                    // 0042D530..0042D55E
    glMatrixMode_0073d228(0x1700);                                     // 0042D564..0042D569
    glPushMatrix_0073d300();                                           // 0042D56B
    glLoadIdentity_0073d230();                                         // 0042D56D
    glViewport_0073d2bc(0, 0, width_i(), height_i());                  // 0042D56F..0042D580
    glDisable_0073d22c(0x0B71);                                        // 0042D586..0042D58B
    glDepthMask_0073d28c(0);                                           // 0042D591..0042D593
    glBegin_0073d2d0(7);                                               // 0042D599..0042D59B
    for (I v = 0; v < 4; ++v) {                                        // [esp+14h] 0042D5A1 / 0042D664..0042D670
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042D5B0..0042D5C4
        const float t = (v == 0 || v == 3) ? scale_t : 0.0f;          // 0042D5CA..0042D5E5 (L24) -> L20
        const float s = (v == 0 || v == 1) ? 0.0f : scale_s;          // 0042D5E9..0042D5FC (L28) -> L1C
        glTexCoord2f_0073d2cc(s, t);                                   // 0042D604..0042D60E
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042D614..0042D631 -> L18
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042D635..0042D64A -> L2C
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042D652..0042D65E
    }
    glEnd_0073d2c4();                                                  // 0042D676
    glMatrixMode_0073d228(0x1701);                                     // 0042D67C..0042D681
    glPopMatrix_0073d2fc();                                            // 0042D683
    glMatrixMode_0073d228(0x1700);                                     // 0042D685..0042D68A
    glPopMatrix_0073d2fc();                                            // 0042D68C
    glPushAttrib_0073d268(0x6040);                                     // 0042D68E..0042D693
    procs::wglMakeContextCurrentARB_007bb818(*b.dc2_007a6900, *b.dc2_007a6900, *b.context2_007a6904); // 0042D699..0042D6A7
    glDrawBuffer_0073d304(0x0404);                                     // 0042D6AD..0042D6B2
    glMatrixMode_0073d228(0x1701);                                     // 0042D6B8..0042D6BD
    glPushMatrix_0073d300();                                           // 0042D6BF
    glLoadIdentity_0073d230();                                         // 0042D6C1
    ortho_window();                                                    // 0042D6C3..0042D6F1
    glMatrixMode_0073d228(0x1700);                                     // 0042D6F7..0042D6FC
    glPushMatrix_0073d300();                                           // 0042D6FE
    glLoadIdentity_0073d230();                                         // 0042D700
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042D702..0042D710
    glEnable_0073d390(0x0BE2);                                         // 0042D716..0042D71B
    glBlendFunc_0073d38c(1, 0);                                        // 0042D721..0042D725 GL_ONE, GL_ZERO
    gl_bloom_filter_ati_0042b810(*b.dc2_007a6900, *b.context2_007a6904, 512.0f, 512.0f, *b.buffer1_007a68e8,
        *b.texture1_007a68f4, 512.0f, 1);                              // 0042D72B..0042D757
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0); // 0042D75C..0042D76B (add esp,34h)
    glMatrixMode_0073d228(0x1701);                                     // 0042D76E..0042D773
    glPopMatrix_0073d2fc();                                            // 0042D775
    glMatrixMode_0073d228(0x1700);                                     // 0042D777..0042D77C
    glPopMatrix_0073d2fc();                                            // 0042D77E
    glFlush_0073d2f8();                                                // 0042D780
    {
        void* const main_dc = handle(*b.main_dc_007a47e4);             // 0042D78C
        procs::wglMakeContextCurrentARB_007bb818(main_dc, main_dc, handle(*b.main_context_007a6854)); // 0042D786..0042D793
    }
    glDrawBuffer_0073d304(0x0405);                                     // 0042D799..0042D79E
    glEnable_0073d390(0x0BE2);                                         // 0042D7A4..0042D7A9
    glBlendFunc_0073d38c(0x0302, 0x0303);                              // 0042D7AF..0042D7B9
    glMatrixMode_0073d228(0x1701);                                     // 0042D7BF..0042D7C4
    glPushMatrix_0073d300();                                           // 0042D7C6
    glLoadIdentity_0073d230();                                         // 0042D7C8
    ortho_window();                                                    // 0042D7CA..0042D7F8
    glMatrixMode_0073d228(0x1700);                                     // 0042D7FE..0042D803
    glPushMatrix_0073d300();                                           // 0042D805
    glLoadIdentity_0073d230();                                         // 0042D807
    glViewport_0073d2bc(0, 0, width_i(), height_i());                  // 0042D809..0042D81B
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042D821..0042D826
    glEnable_0073d390(0x0DE1);                                         // 0042D82C..0042D837 (ebx = [0073D390])
    glBindTexture_0073d240(0x0DE1, *b.texture2_007a6908);              // 0042D839..0042D844
    procs::wglBindTexImageARB_007bb7b8(*b.buffer2_007a68fc, 0x2083);   // 0042D84A..0042D856
    glBegin_0073d2d0(7);                                               // 0042D85C..0042D85E
    for (I v = 0; v < 4; ++v) {                                        // edi 0042D864 / 0042D918..0042D91C
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042D870..0042D884
        const float t = (v == 0 || v == 3) ? 1.0f : 0.0f;              // 0042D88A..0042D89B -> L2C
        const float s = (v == 0 || v == 1) ? 0.0f : 1.0f;              // 0042D8A3..0042D8B4 -> L28
        glTexCoord2f_0073d2cc(s, t);                                   // 0042D8BC..0042D8C6
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042D8CC..0042D8E5 -> L24
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042D8E9..0042D8FE -> L20
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042D906..0042D912
    }
    glEnd_0073d2c4();                                                  // 0042D922
    procs::wglReleaseTexImageARB_007bb6d0(*b.buffer2_007a68fc, 0x2083); // 0042D928..0042D933
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042D939..0042D940
    glDisable_0073d22c(0x0DE1);                                        // 0042D946..0042D951 (edi = [0073D22C])
    glPopAttrib_0073d26c();                                            // 0042D953
    glMatrixMode_0073d228(0x1701);                                     // 0042D959..0042D95E
    glPopMatrix_0073d2fc();                                            // 0042D960
    glMatrixMode_0073d228(0x1700);                                     // 0042D962..0042D967
    glPopMatrix_0073d2fc();                                            // 0042D969
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                  // 0042D96B..0042D972
    glDisable_0073d22c(0x0B90);                                        // 0042D974..0042D979
    glDepthMask_0073d28c(1);                                           // 0042D97B..0042D97D
    glStencilMask_0073d278(0xFF);                                      // 0042D983..0042D988
    glDisable_0073d22c(0x8920);                                        // 0042D98E..0042D993 GL_FRAGMENT_SHADER_ATI
}                                                                      // 0042D99C ret

// 0042D9A0 GLRender::FinishSoftShadows (NV path), cdecl no args (EBP frame, esp aligned to 8 at 0042D9A3).
void __cdecl gl_finish_soft_shadows_0042d9a0() {
    if (kotor_soft_shadow::ATISoftAvailable_0045f7e0() != 0) {         // 0042D9AD..0042D9B4
        gl_finish_soft_shadows_ati_0042d250();                         // 0042D9B6
        return;                                                        // 0042D9BB..0042D9C2
    }
    const auto& b = kotor_soft_shadow::bindings;
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042D9C3..0042D9C8
    glEnable_0073d390(0x84F5);                                         // 0042D9CE..0042D9D9 (edi = [0073D390])
    glBindTexture_0073d240(0x84F5, *b.scene_007a687c);                 // 0042D9DB..0042D9EC (esi = [0073D240])
    glCopyTexSubImage2D_0073d308(0x84F5, 0, 0, 0, 0, 0, width_i(), height_i()); // 0042D9EE..0042DA0B
    glBindTexture_0073d240(0x84F5, 0);                                 // 0042DA11..0042DA18
    glDisable_0073d22c(0x84F5);                                        // 0042DA1A..0042DA25 (ebx = [0073D22C])
    glFlush_0073d2f8();                                                // 0042DA27
    wglMakeCurrent_0073d2b0(*b.dc2_007a6900, *b.context2_007a6904);    // 0042DA2D..0042DA3A
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042DA40..0042DA45
    glEnable_0073d390(0x84F5);                                         // 0042DA4B..0042DA50
    glBindTexture_0073d240(0x84F5, *b.scene_007a687c);                 // 0042DA52..0042DA5E
    glDisable_0073d22c(0x0B50);                                        // 0042DA60..0042DA65
    glDisable_0073d22c(0x0B71);                                        // 0042DA67..0042DA6C
    glDisable_0073d22c(0x0B90);                                        // 0042DA6E..0042DA73
    glMatrixMode_0073d228(0x1701);                                     // 0042DA75..0042DA80 (esi = [0073D228])
    glPushMatrix_0073d300();                                           // 0042DA82
    glLoadIdentity_0073d230();                                         // 0042DA88..0042DA8E (ebp = [0073D230])
    ortho_window();                                                    // 0042DA90..0042DABE
    glMatrixMode_0073d228(0x1700);                                     // 0042DAC4..0042DAC9
    glPushMatrix_0073d300();                                           // 0042DACB
    glLoadIdentity_0073d230();                                         // 0042DAD1
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042DAD3..0042DAE1
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042DAE7..0042DAEF
    glClear_0073d21c(0x4000);                                          // 0042DAF5..0042DAFA
    glBegin_0073d2d0(7);                                               // 0042DB00..0042DB02
    for (I v = 0; v < 4; ++v) {                                        // [esp+14h] 0042DB08 / 0042DBC8..0042DBD4
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042DB10..0042DB24
        const float t = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042DB2A..0042DB47 -> L18
        const float s = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042DB4B..0042DB60 -> L1C
        glTexCoord2f_0073d2cc(s, t);                                   // 0042DB68..0042DB72
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042DB78..0042DB95 -> L20
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042DB99..0042DBAE -> L24
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042DBB6..0042DBC2
    }
    glEnd_0073d2c4();                                                  // 0042DBDA
    glMatrixMode_0073d228(0x1701);                                     // 0042DBE0..0042DBE5
    glPopMatrix_0073d2fc();                                            // 0042DBE7
    glMatrixMode_0073d228(0x1700);                                     // 0042DBED..0042DBF2
    glPopMatrix_0073d2fc();                                            // 0042DBF4
    wglMakeCurrent_0073d2b0(handle(*b.main_dc_007a47e4), handle(*b.main_context_007a6854)); // 0042DBFA..0042DC08
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042DC0E..0042DC13
    glEnable_0073d390(0x84F5);                                         // 0042DC19..0042DC1E
    glBindTexture_0073d240(0x84F5, *b.scratch_007a68f8);               // 0042DC20..0042DC2B
    glDisable_0073d22c(0x0B50);                                        // 0042DC31..0042DC36
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 0042DC38..0042DC3F
    glBlendFunc_0073d38c(1, 0);                                        // 0042DC41..0042DC45
    glDisable_0073d22c(0x0B90);                                        // 0042DC4B..0042DC50
    glMatrixMode_0073d228(0x1701);                                     // 0042DC52..0042DC57
    glPushMatrix_0073d300();                                           // 0042DC59
    glLoadIdentity_0073d230();                                         // 0042DC5F
    ortho_window();                                                    // 0042DC61..0042DC8F
    glMatrixMode_0073d228(0x1700);                                     // 0042DC95..0042DC9A
    glPushMatrix_0073d300();                                           // 0042DC9C
    glLoadIdentity_0073d230();                                         // 0042DCA2
    glViewport_0073d2bc(0, 0, width_i(), height_i());                  // 0042DCA4..0042DCB5
    glDisable_0073d22c(0x0B71);                                        // 0042DCBB..0042DCC0
    glDepthMask_0073d28c(0);                                           // 0042DCC2..0042DCC4
    glBegin_0073d2d0(7);                                               // 0042DCCA..0042DCCC
    for (I v = 0; v < 4; ++v) {                                        // [esp+14h] 0042DCD2 / 0042DD98..0042DDA4
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042DCE0..0042DCF4
        const float t = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042DCFA..0042DD17 -> L24
        const float s = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042DD1B..0042DD30 -> L20
        glTexCoord2f_0073d2cc(s, t);                                   // 0042DD38..0042DD42
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042DD48..0042DD65 -> L1C
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042DD69..0042DD7E -> L18
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042DD86..0042DD92
    }
    glEnd_0073d2c4();                                                  // 0042DDAA
    glEnable_0073d390(0x0B71);                                         // 0042DDB0..0042DDB5
    glDepthMask_0073d28c(1);                                           // 0042DDB7..0042DDB9
    glBindTexture_0073d240(0x84F5, 0);                                 // 0042DDBF..0042DDC6
    glDisable_0073d22c(0x84F5);                                        // 0042DDCC..0042DDD1
    glDepthMask_0073d28c(0);                                           // 0042DDD3..0042DDD5
    glPushAttrib_0073d268(0x6040);                                     // 0042DDDB..0042DDE0
    wglMakeCurrent_0073d2b0(*b.dc1_007a68ec, *b.context1_007a68f0);    // 0042DDE6..0042DDF3
    glMatrixMode_0073d228(0x1701);                                     // 0042DDF9..0042DDFE
    glLoadIdentity_0073d230();                                         // 0042DE00 (no push in this block)
    ortho_window();                                                    // 0042DE02..0042DE30
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042DE36..0042DE44
    glMatrixMode_0073d228(0x1700);                                     // 0042DE4A..0042DE4F
    glLoadIdentity_0073d230();                                         // 0042DE51
    glEnable_0073d390(0x0BE2);                                         // 0042DE53..0042DE58
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042DE5A..0042DE62
    glClear_0073d21c(0x4000);                                          // 0042DE68..0042DE6D
    gl_bloom_filter_0042bf80(*b.dc1_007a68ec, *b.context1_007a68f0, 512.0f, 512.0f, *b.buffer2_007a68fc,
        *b.texture2_007a6908, 512.0f, 1);                              // 0042DE73..0042DE9F
    wglMakeCurrent_0073d2b0(handle(*b.main_dc_007a47e4), handle(*b.main_context_007a6854)); // 0042DEA4..0042DEB4
    glEnable_0073d390(0x0BE2);                                         // 0042DEBA..0042DEBF
    glBlendFunc_0073d38c(0x0302, 0x0303);                              // 0042DEC1..0042DECB
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042DED1..0042DED6
    glEnable_0073d390(0x0DE1);                                         // 0042DEDC..0042DEE1
    glBindTexture_0073d240(0x0DE1, *b.texture1_007a68f4);              // 0042DEE3..0042DEEF
    procs::wglBindTexImageARB_007bb7b8(*b.buffer1_007a68e8, 0x2083);   // 0042DEF5..0042DF01
    glBegin_0073d2d0(7);                                               // 0042DF07..0042DF09
    for (I v = 0; v < 4; ++v) {                                        // ebp 0042DF0F / 0042DFB9..0042DFBD
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042DF11..0042DF25
        const float t = (v == 0 || v == 3) ? 1.0f : 0.0f;              // 0042DF2B..0042DF3C -> L24
        const float s = (v == 0 || v == 1) ? 0.0f : 1.0f;              // 0042DF44..0042DF55 -> L20
        glTexCoord2f_0073d2cc(s, t);                                   // 0042DF5D..0042DF67
        const float y = (v == 0 || v == 3) ? static_cast<float>(height_i()) : 0.0f; // 0042DF6D..0042DF86 -> L1C
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(width_i()); // 0042DF8A..0042DF9F -> L18
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042DFA7..0042DFB3
    }
    glEnd_0073d2c4();                                                  // 0042DFC3
    procs::wglReleaseTexImageARB_007bb6d0(*b.buffer1_007a68e8, 0x2083); // 0042DFC9..0042DFD5
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042DFDB..0042DFE2
    glDisable_0073d22c(0x0DE1);                                        // 0042DFE8..0042DFED
    glPopAttrib_0073d26c();                                            // 0042DFEF
    glMatrixMode_0073d228(0x1701);                                     // 0042DFF5..0042DFFA
    glPopMatrix_0073d2fc();                                            // 0042DFFC..0042E002
    glMatrixMode_0073d228(0x1700);                                     // 0042E004..0042E009
    glPopMatrix_0073d2fc();                                            // 0042E00B
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                  // 0042E00D..0042E014
    glDisable_0073d22c(0x0B90);                                        // 0042E016..0042E01B
    glDepthMask_0073d28c(1);                                           // 0042E01D..0042E01F
    glStencilMask_0073d278(0xFF);                                      // 0042E025..0042E02A
}                                                                      // 0042E037 ret

} // extern "C"
