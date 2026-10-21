// lane-window: renderer GL resource Shutdown 00432180 (cdecl, one word `release`, plain RET),
// called by CAurInternalGL::ShutDown 0044EBAA. Evidence: dumpbin /disasm swkotor.exe
// 00432180..0043271A. Every cell/procedure is the single lane-renderer-bodies object
// (renderer_gl_cells / renderer_gl_framebuffer_cells / renderer_gl_loader_cells / procs);
// declarations below repeat their exact types. 007A68E4 has no other owner (BSS 0, only
// this body references it) and is owned here.
#include "37d96ffda3-render_pool.h"           // lane-renderer-bodies: RequestPoolManager, pool_007a68a4
#include <cstdint>

namespace kotor_renderer_init {
using U32 = std::uint32_t; using I32 = std::int32_t;
extern I32 __stdcall wglMakeCurrent_0073d2b0(void*, void*);         // M41 aurora-windows-gl-imports
extern I32 __stdcall wglDeleteContext_0073d30c(void*);              // M41
extern I32 __stdcall ReleaseDC_0073d404(void*, void*);              // M41
extern void __stdcall glDeleteTextures_0073d248(I32, const U32*);   // lane-renderer-bodies windows_gl_imports.cpp
U32 __cdecl ATIFrameBufferAvailable_0045f860();                     // lane-renderer-bodies capabilities
}
namespace kotor_soft_shadow { std::uint32_t __cdecl ATISoftAvailable_0045f7e0(); }
namespace kotor_renderer_gl_procs {
extern void (__stdcall* glDeleteProgramsARB_007bb724)(std::int32_t, const std::uint32_t*);
extern std::int32_t (__stdcall* wglReleasePbufferDCARB_007bb578)(void*, void*);
extern std::int32_t (__stdcall* wglDestroyPbufferARB_007bb598)(void*);
extern std::int32_t (__stdcall* wglMakeContextCurrentARB_007bb818)(void*, void*, void*);
extern void (__stdcall* glDeleteFragmentShaderATI_007bb550)(std::uint32_t);
}
namespace kotor_renderer_gl_cells {
using U32 = std::uint32_t;
extern U32 main_context_007a6854, main_dc_007a47e4;
extern U32 scene_texture_007a687c, scratch_texture_007a68f8;
extern void* pbuffer1_007a68e8; extern void* dc1_007a68ec; extern void* context1_007a68f0; extern U32 texture1_007a68f4;
extern void* pbuffer2_007a68fc; extern void* dc2_007a6900; extern void* context2_007a6904; extern U32 texture2_007a6908;
extern U32 framebuffer_textures_007a47c4[6];
extern void* framebuffer_dcs_007a47ec[6];
extern void* framebuffer_contexts_007a4804[6];
extern void* framebuffer_pbuffers_007a6824[6];
extern std::uint8_t framebuffer_active_007a6860;
extern U32 bloom_ramp_007a6864, bloom_ramp_007a6868, bloom_ramp_007a686c;
extern U32 ramp_007a6870, ramp_007a6874, ramp_007a6878;
extern U32 bloom_texture_007a68c0;
extern void* bloom_pbuffer_007a68c4; extern void* bloom_dc_007a68c8; extern void* bloom_context_007a68cc;
extern U32 framebuffer_program_007a68d4;
extern U32 ati_shader_007a692c, ati_shader_007a6924, ati_shader_007a6920, ati_shader_007a691c;
extern U32 ati_shader_007a6918, ati_shader_007a690c, ati_shader_007a6910, ati_shader_007a6914;
}
struct HWND__;
namespace kotor_menu_window_cells { extern HWND__* volatile render_007a39d8; }   // M41 window-shared-cells.obj
namespace kotor_aurora_window {
std::uint32_t program_007a68e4 = 0;     // BSS; deleted at 0043233B..00432350, no writer in the image
}
extern "C" void __cdecl render_shutdown_00432180(std::uint32_t release);

namespace {
using namespace kotor_renderer_gl_cells;
using namespace kotor_renderer_gl_procs;
using kotor_renderer_init::U32;
void* as_handle(U32 w) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
void* main_dc() { return as_handle(main_dc_007a47e4); }
void* main_ctx() { return as_handle(main_context_007a6854); }
void delete_texture(U32& cell) {                                    // cmp [cell],0; push cell; push 1; call edi; mov [cell],0
    if (cell) { kotor_renderer_init::glDeleteTextures_0073d248(1, &cell); cell = 0; }
}
}

extern "C" void __cdecl render_shutdown_00432180(std::uint32_t release) {
    using namespace kotor_renderer_init;
    if (kotor_renderer_gl::RequestPoolManager* pool = pool_007a68a4)  // 00432180..0043218F
        pool->BackUpPoolManager();                                    // call [eax+28h]
    if (dc1_007a68ec) {                                               // 00432192..004321A1
        if (kotor_soft_shadow::ATISoftAvailable_0045f7e0() != 0) {    // 004321A7..004321B3
            (void)wglMakeContextCurrentARB_007bb818(dc1_007a68ec, dc1_007a68ec, context1_007a68f0); // 004321B9..004321C2
            glDeleteTextures_0073d248(1, &texture1_007a68f4);         // 004321C8..004321CF
            (void)wglMakeContextCurrentARB_007bb818(main_dc(), main_dc(), main_ctx()); // 004321D1..004321DF
            (void)wglReleasePbufferDCARB_007bb578(pbuffer1_007a68e8, dc1_007a68ec);   // 004321E5..004321F2
            (void)wglDestroyPbufferARB_007bb598(pbuffer1_007a68e8);   // 004321F8..004321FF
            void* dc2 = dc2_007a6900; void* ctx2 = context2_007a6904; // 00432205..00432210 (pushed first)
            dc1_007a68ec = nullptr; context1_007a68f0 = nullptr; pbuffer1_007a68e8 = nullptr; texture1_007a68f4 = 0; // 00432212..00432224
            (void)wglMakeContextCurrentARB_007bb818(dc2, dc2, ctx2);  // 0043222A
            glDeleteTextures_0073d248(1, &texture2_007a6908);         // 00432230..00432237
            (void)wglMakeContextCurrentARB_007bb818(main_dc(), main_dc(), main_ctx()); // 00432239..00432247
            (void)wglReleasePbufferDCARB_007bb578(pbuffer2_007a68fc, dc2_007a6900);   // 0043224D..0043225A
            (void)wglDestroyPbufferARB_007bb598(pbuffer2_007a68fc);   // 00432260..00432267
            dc2_007a6900 = nullptr; context2_007a6904 = nullptr; pbuffer2_007a68fc = nullptr; texture2_007a6908 = 0; // 00432274..00432286
            glDeleteTextures_0073d248(1, &scratch_texture_007a68f8);  // 0043226D..0043228C
            scratch_texture_007a68f8 = 0;                             // 0043228E
        } else {
            (void)wglMakeCurrent_0073d2b0(dc1_007a68ec, context1_007a68f0); // 00432433..00432441
            glDeleteTextures_0073d248(1, &texture1_007a68f4);         // 00432443..0043244A
            (void)wglMakeCurrent_0073d2b0(main_dc(), main_ctx());     // 0043244C..0043245A
            (void)wglDeleteContext_0073d30c(context1_007a68f0);       // 0043245C..00432468
            (void)wglReleasePbufferDCARB_007bb578(pbuffer1_007a68e8, dc1_007a68ec);   // 0043246A..00432478
            (void)wglDestroyPbufferARB_007bb598(pbuffer1_007a68e8);   // 0043247E..00432484
            void* dc2 = dc2_007a6900; void* ctx2 = context2_007a6904; // 0043248A..00432497
            dc1_007a68ec = nullptr; context1_007a68f0 = nullptr; pbuffer1_007a68e8 = nullptr; texture1_007a68f4 = 0; // 00432498..004324AA
            (void)wglMakeCurrent_0073d2b0(dc2, ctx2);                 // 004324B0
            glDeleteTextures_0073d248(1, &texture2_007a6908);         // 004324B2..004324B9
            (void)wglMakeCurrent_0073d2b0(main_dc(), main_ctx());     // 004324BB..004324C8
            (void)wglDeleteContext_0073d30c(context2_007a6904);       // 004324CA..004324D1
            (void)wglReleasePbufferDCARB_007bb578(pbuffer2_007a68fc, dc2_007a6900);   // 004324D3..004324E0
            (void)wglDestroyPbufferARB_007bb598(pbuffer2_007a68fc);   // 004324E6..004324ED
            dc2_007a6900 = nullptr; context2_007a6904 = nullptr; pbuffer2_007a68fc = nullptr; texture2_007a6908 = 0; // 004324FA..0043250C
            glDeleteTextures_0073d248(1, &scratch_texture_007a68f8);  // 004324F3..00432512
            scratch_texture_007a68f8 = 0;                             // 00432514
        }
    }
    delete_texture(scene_texture_007a687c);                           // 0043229A..004322AB
    delete_texture(ramp_007a6870);                                    // 004322B1..004322C2
    delete_texture(ramp_007a6874);                                    // 004322C8..004322D9
    delete_texture(ramp_007a6878);                                    // 004322DF..004322F0
    delete_texture(bloom_ramp_007a6864);                              // 004322F6..00432307
    delete_texture(bloom_ramp_007a6868);                              // 0043230D..0043231E
    delete_texture(bloom_ramp_007a686c);                              // 00432324..00432335
    if (kotor_aurora_window::program_007a68e4) {                      // 0043233B..00432350
        glDeleteProgramsARB_007bb724(1, &kotor_aurora_window::program_007a68e4);
        kotor_aurora_window::program_007a68e4 = 0;
    }
    if (framebuffer_active_007a6860 == 1) {                           // 00432356..0043235D
        if (ATIFrameBufferAvailable_0045f860() != 0) {                // 00432363..0043236C
            for (unsigned i = 0; i < 6; ++i) {                        // 00432372..004323E2
                (void)wglMakeContextCurrentARB_007bb818(framebuffer_dcs_007a47ec[i], framebuffer_dcs_007a47ec[i], framebuffer_contexts_007a4804[i]);
                glDeleteTextures_0073d248(1, &framebuffer_textures_007a47c4[i]);
                (void)wglMakeContextCurrentARB_007bb818(main_dc(), main_dc(), main_ctx());
                (void)wglReleasePbufferDCARB_007bb578(framebuffer_pbuffers_007a6824[i], framebuffer_dcs_007a47ec[i]);
                (void)wglDestroyPbufferARB_007bb598(framebuffer_pbuffers_007a6824[i]);
                framebuffer_dcs_007a47ec[i] = nullptr; framebuffer_contexts_007a4804[i] = nullptr;
                framebuffer_pbuffers_007a6824[i] = nullptr; framebuffer_textures_007a47c4[i] = 0;
            }
            // 004323E4..0043242E then 0043260B: bloom released unconditionally on this path.
            (void)wglMakeContextCurrentARB_007bb818(bloom_dc_007a68c8, bloom_dc_007a68c8, bloom_context_007a68cc);
            glDeleteTextures_0073d248(1, &bloom_texture_007a68c0);
            (void)wglMakeContextCurrentARB_007bb818(main_dc(), main_dc(), main_ctx());
            (void)wglReleasePbufferDCARB_007bb578(bloom_pbuffer_007a68c4, bloom_dc_007a68c8);
            (void)wglDestroyPbufferARB_007bb598(bloom_pbuffer_007a68c4);
            bloom_dc_007a68c8 = nullptr; bloom_context_007a68cc = nullptr; bloom_pbuffer_007a68c4 = nullptr; bloom_texture_007a68c0 = 0; // 00432611..00432623
        } else {
            for (unsigned i = 0; i < 6; ++i) {                        // 0043251F..004325A8
                (void)wglMakeCurrent_0073d2b0(framebuffer_dcs_007a47ec[i], framebuffer_contexts_007a4804[i]);
                glDeleteTextures_0073d248(1, &framebuffer_textures_007a47c4[i]);
                (void)wglMakeCurrent_0073d2b0(main_dc(), main_ctx());
                (void)wglDeleteContext_0073d30c(framebuffer_contexts_007a4804[i]);
                (void)wglReleasePbufferDCARB_007bb578(framebuffer_pbuffers_007a6824[i], framebuffer_dcs_007a47ec[i]);
                (void)wglDestroyPbufferARB_007bb598(framebuffer_pbuffers_007a6824[i]);
                framebuffer_dcs_007a47ec[i] = nullptr; framebuffer_contexts_007a4804[i] = nullptr;
                framebuffer_pbuffers_007a6824[i] = nullptr; framebuffer_textures_007a47c4[i] = 0;
            }
            if (bloom_pbuffer_007a68c4) {                             // 004325AE..004325B4
                (void)wglMakeCurrent_0073d2b0(bloom_dc_007a68c8, bloom_context_007a68cc); // 004325B6..004325CA
                glDeleteTextures_0073d248(1, &bloom_texture_007a68c0);   // 004325CC..004325D3
                (void)wglMakeCurrent_0073d2b0(main_dc(), main_ctx());   // 004325D5..004325E2
                (void)wglDeleteContext_0073d30c(bloom_context_007a68cc); // 004325E4..004325EB
                (void)wglReleasePbufferDCARB_007bb578(bloom_pbuffer_007a68c4, bloom_dc_007a68c8); // 004325F1..004325FE
                (void)wglDestroyPbufferARB_007bb598(bloom_pbuffer_007a68c4); // 00432604..0043260B
                bloom_dc_007a68c8 = nullptr; bloom_context_007a68cc = nullptr; bloom_pbuffer_007a68c4 = nullptr; bloom_texture_007a68c0 = 0;
            }
        }
        framebuffer_active_007a6860 = 0;                              // 00432629
    }
    if (framebuffer_program_007a68d4) {                               // 0043262F..00432647
        glDeleteProgramsARB_007bb724(1, &framebuffer_program_007a68d4);
        framebuffer_program_007a68d4 = 0;
    }
    if (ati_shader_007a690c) {                                        // 0043264D..00432654
        glDeleteFragmentShaderATI_007bb550(ati_shader_007a690c);      // 0043265B
        const U32 s6910 = ati_shader_007a6910; ati_shader_007a690c = 0; glDeleteFragmentShaderATI_007bb550(s6910); // 00432661..0043266D
        const U32 s6914 = ati_shader_007a6914; ati_shader_007a6910 = 0; glDeleteFragmentShaderATI_007bb550(s6914); // 00432673..00432680
        const U32 s6918 = ati_shader_007a6918; ati_shader_007a6914 = 0; glDeleteFragmentShaderATI_007bb550(s6918); // 00432686..00432693
        const U32 s691c = ati_shader_007a691c; ati_shader_007a6918 = 0; glDeleteFragmentShaderATI_007bb550(s691c); // 00432699..004326A5
        const U32 s6920 = ati_shader_007a6920; ati_shader_007a691c = 0; glDeleteFragmentShaderATI_007bb550(s6920); // 004326AB..004326B8
        const U32 s6924 = ati_shader_007a6924; ati_shader_007a6920 = 0; glDeleteFragmentShaderATI_007bb550(s6924); // 004326BE..004326CB
        const U32 s692c = ati_shader_007a692c; ati_shader_007a6924 = 0; glDeleteFragmentShaderATI_007bb550(s692c); // 004326D1..004326DD
        ati_shader_007a692c = 0;                                      // 004326E3
    }
    (void)ReleaseDC_0073d404(kotor_menu_window_cells::render_007a39d8, main_dc()); // 004326E9..004326F7
    if (release) {                                                    // 004326FD..00432701
        if (kotor_renderer_gl::RequestPoolManager* pool = pool_007a68a4) delete pool; // 0043270D..00432711 [vtbl+0](1)
        pool_007a68a4 = nullptr;                                      // 00432713
    }
}
