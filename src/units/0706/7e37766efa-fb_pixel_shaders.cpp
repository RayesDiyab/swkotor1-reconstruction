// fb_pixel_shaders.cpp - lane-3d-fb fork A: the two GLRender::FrameBufferModificationPixelShaders setups.
//   004293F0 (ATI_fragment_shader path: texture-unit setup, glBindFragmentShaderATI, glSetFragmentShaderConstantATI)
//   004299C0 (NV_register_combiners path: texture-unit setup, combiner inputs/outputs/final combiner)
// Both are __cdecl (mode, t0, t1, t2, t3), plain RET, no return value used by any caller (callers add esp,14h).
// First-menu-frame status: NOT EXECUTED (every caller is inside the Scene+D4 / soft-shadow gated passes; gates.json).
//
// Dispatch (both functions, identical shape): mode > 32h -> 256h/64h/128h/512h compare chain; mode == 32h direct;
// otherwise mode-1 indexed through a byte table (0042999C / 0042B350, 22 bytes, read from the image:
// 00 01 05 02 05 05 05 03 05*13 04) into a 6-entry jump table (00429984 / 0042B338), entry 5 = the plain
// epilogue (0042997F / 0042B332). So the accepted modes are exactly 1,2,4,8,16h,32h,64h,128h,256h,512h; any other
// mode (including 0, via the unsigned `dec`/`ja`) returns without a GL call.
//
// Case bodies are straight-line push/call sequences with no conditional code (verified: the only instruction kinds
// inside them are push/mov/call/jmp/pop/ret). They were transcribed mechanically by forks/gen_ps.py, which simulates the
// argument pushes, the [esp+n] argument loads (adjusted for the 3 / 4 callee-saved pushes and pending pushes), the
// `mov reg,[IAT]` / `call reg` register calls, and follows the compiler's tail-merging jmps into shared sequences
// (those tails are duplicated inline here; same calls in the same order). Every statement cites the address of its
// first argument push and its call instruction.
// GL imports: single adapter owner kotor_renderer_init (lane-renderer-bodies windows_gl_imports.cpp / M40).
// Extension slots: kotor_renderer_gl_procs typed cells, re-read at every call like `call dword ptr ds:[slot]`.
// Pushed addresses 0078E110 + 16*i are rows of fb_shader_constants_0078e110 (lane-3d-fb fb_cells.cpp).
#include "056e5913e7-fb_bloom.h"

namespace procs = kotor_renderer_gl_procs;

extern "C" {

// 004293F0 GLRender::FrameBufferModificationPixelShaders (ATI fragment shaders)
void __cdecl gl_frame_buffer_modification_pixel_shaders_ati_004293f0(U mode, U t0, U t1, U t2, U t3) {
    // 004293F0 mov eax,[esp+4]; 004293F7..004293F9 push ebx/esi/edi; dispatch 004293F4..00429417
    switch (mode) {
    case 0x1: {   // table 00429984[0] (index byte 0042999C[0]=0) -> 0042946A
        procs::glActiveTextureARB_007bb80c(0x84C0); // 0042946A..0042946F call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 0042947B..00429480 call edi=[0073D390] (loaded 00429475)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t0); // 0042948C..00429492 call ebx=[0073D240] (loaded 00429486)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 0042949A..004294A9 call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 004294AB..004294BA call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 004294BC..004294CB call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 004294CD..004294DC call esi=[0073D23C] (loaded 00429494)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 004294DE..004294E3 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 004294E9..004294EE call edi=[0073D390] (loaded 00429475)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t1); // 004294F4..004294FA call ebx=[0073D240] (loaded 00429486)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 004294FC..0042950B call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 0042950D..0042951C call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 0042951E..0042952D call esi=[0073D23C] (loaded 00429494)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 0042952F..00429534 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 0042953A..0042953F call edi=[0073D390] (loaded 00429475)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t2); // 00429545..0042954B call ebx=[0073D240] (loaded 00429486)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 0042954D..0042955C call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 0042955E..0042956D call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 0042956F..0042957E call esi=[0073D23C] (loaded 00429494)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 00429580..00429585 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 0042958B..00429590 call edi=[0073D390] (loaded 00429475)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t3); // 00429596..0042959C call ebx=[0073D240] (loaded 00429486)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 0042959E..004295AD call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 004295AF..004295BE call esi=[0073D23C] (loaded 00429494)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 004295C0..004295CF call esi=[0073D23C] (loaded 00429494)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 004295D1..004295D6 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x8920); // 004295DC..004295E1 call edi=[0073D390] (loaded 00429475)
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a690c); // 004295E8..004295E9 call [007BB6CC]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8941, fb_shader_constants_0078e110[5]); // 004295EF..004295F9 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, fb_shader_constants_0078e110[4]); // 004295FF..00429609 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8943, fb_shader_constants_0078e110[3]); // 0042960F..00429619 call [007BB5D8]
        return; // 00429622 ret
    }
    case 0x2: {   // table 00429984[1] (0042999C[1]=1) -> 0042965B
        kotor_renderer_init::glDisable_0073d22c(0x8920); // 00429661..00429666 call esi=[0073D22C] (loaded 0042965B)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 00429668..0042966D call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 00429673..00429678 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 00429680..00429687 call edi=[0073D240] (loaded 0042967A)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 00429689..0042968E call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE0); // 00429694..00429699 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 0042969B..004296A0 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0x8513); // 004296A2..004296A7 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, 0); // 004296A9..004296B0 call edi=[0073D240] (loaded 0042967A)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 004296B2..004296B9 call edi=[0073D240] (loaded 0042967A)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 004296BB..004296C0 call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 004296C6..004296CB call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0xDE0); // 004296CD..004296D2 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0x8513); // 004296D4..004296D9 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, 0); // 004296DB..004296E2 call edi=[0073D240] (loaded 0042967A)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 004296E4..004296EB call edi=[0073D240] (loaded 0042967A)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 004296ED..004296F2 call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 004296F8..004296FD call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0xDE0); // 004296FF..00429704 call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glDisable_0073d22c(0x8513); // 00429706..0042970B call esi=[0073D22C] (loaded 0042965B)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 0042970D..00429714 call edi=[0073D240] (loaded 0042967A)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, 0); // 00429716..0042971D call edi=[0073D240] (loaded 0042967A)
        kotor_renderer_init::glBindTexture_0073d240(0x8513, 0); // 0042971F..00429726 call edi=[0073D240] (loaded 0042967A)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 00429728..0042972D call [007BB80C]
        return; // 00429736 ret
    }
    case 0x4: {   // table 00429984[2] (0042999C[3]=2) -> 00429623
        kotor_renderer_init::glEnable_0073d390(0x8920); // 00429623..00429628 call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6910); // 00429634..00429635 call [007BB6CC]
        return; // 0042963E ret
    }
    case 0x8: {   // table 00429984[3] (0042999C[7]=3) -> 0042963F
        kotor_renderer_init::glEnable_0073d390(0x8920); // 0042963F..00429644 call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6914); // 00429650..00429651 call [007BB6CC]
        return; // 0042965A ret
    }
    case 0x16: {   // table 00429984[4] (0042999C[0x15]=4) -> 0042941E
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8944, fb_shader_constants_0078e110[8]); // 0042941E..00429428 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8945, fb_shader_constants_0078e110[7]); // 0042942E..00429438 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8946, fb_shader_constants_0078e110[6]); // 0042943E..00429448 call [007BB5D8]
        kotor_renderer_init::glEnable_0073d390(0x8920); // 0042944E..00429453 call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6918); // 0042945F..00429460 call [007BB6CC]
        return; // 00429469 ret
    }
    case 0x32: {   // 00429400 je -> 00429737
        kotor_renderer_init::glEnable_0073d390(0x8920); // 00429737..0042973C call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6920); // 00429747..00429748 call [007BB6CC]
        return; // 00429751 ret
    }
    case 0x64: {   // 0042975E je -> 00429787
        kotor_renderer_init::glEnable_0073d390(0x8920); // 00429787..0042978C call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a6924); // 00429798..00429799 call [007BB6CC]
        return; // 004297A2 ret
    }
    case 0x128: {   // 00429760 cmp 128h / 00429765 jne -> 0042976B
        kotor_renderer_init::glEnable_0073d390(0x8920); // 0042976B..00429770 call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a691c); // 0042977C..0042977D call [007BB6CC]
        return; // 00429786 ret
    }
    case 0x256: {   // 00429759 je -> 004297A3
        kotor_renderer_init::glEnable_0073d390(0x8920); // 004297A3..004297A8 call [0073D390]
        procs::glBindFragmentShaderATI_007bb6cc(kotor_renderer_gl_cells::soft_shader_007a6928); // 004297B3..004297B4 call [007BB6CC]
        return; // 004297BD ret
    }
    case 0x512: {   // 004297BE cmp 512h / 004297C3 jne -> 004297C9
        procs::glActiveTextureARB_007bb80c(0x84C0); // 004297C9..004297CE call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 004297DA..004297DF call edi=[0073D390] (loaded 004297D4)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t0); // 004297EB..004297F1 call ebx=[0073D240] (loaded 004297E5)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 004297F9..00429808 call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 0042980A..00429819 call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 0042981B..0042982A call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 0042982C..0042983B call esi=[0073D23C] (loaded 004297F3)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 0042983D..00429842 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 00429848..0042984D call edi=[0073D390] (loaded 004297D4)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t1); // 00429853..00429859 call ebx=[0073D240] (loaded 004297E5)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 0042985B..0042986A call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 0042986C..0042987B call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 0042987D..0042988C call esi=[0073D23C] (loaded 004297F3)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 0042988E..00429893 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 00429899..0042989E call edi=[0073D390] (loaded 004297D4)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t2); // 004298A4..004298AA call ebx=[0073D240] (loaded 004297E5)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 004298AC..004298BB call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 004298BD..004298CC call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 004298CE..004298DD call esi=[0073D23C] (loaded 004297F3)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 004298DF..004298E4 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0xDE0); // 004298EA..004298EF call edi=[0073D390] (loaded 004297D4)
        kotor_renderer_init::glBindTexture_0073d240(0xDE0, t3); // 004298F5..004298FB call ebx=[0073D240] (loaded 004297E5)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2802, 0x2900); // 004298FD..0042990C call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2801, 0x2601); // 0042990E..0042991D call esi=[0073D23C] (loaded 004297F3)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE0, 0x2800, 0x2601); // 0042991F..0042992E call esi=[0073D23C] (loaded 004297F3)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 00429930..00429935 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x8920); // 0042993B..00429940 call edi=[0073D390] (loaded 004297D4)
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_renderer_init::ati_setup.shader_007a692c); // 00429948..00429949 call [007BB6CC]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8941, fb_shader_constants_0078e110[2]); // 0042994F..00429959 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8942, fb_shader_constants_0078e110[1]); // 0042995F..00429969 call [007BB5D8]
        procs::glSetFragmentShaderConstantATI_007bb5d8(0x8943, fb_shader_constants_0078e110[0]); // 0042996F..00429979 call [007BB5D8]
        return; // 00429982 ret
    }
    default:
        return;   // 0042997F..00429982 (pop edi/esi/ebx; ret)
    }
}

// 004299C0 GLRender::FrameBufferModificationPixelShaders (NV register combiners)
void __cdecl gl_frame_buffer_modification_pixel_shaders_004299c0(U mode, U t0, U t1, U t2, U t3) {
    // 004299C0 mov eax,[esp+4]; 004299C7..004299CA push ebx/ebp/esi/edi; dispatch 004299C4..004299E8
    switch (mode) {
    case 0x1: {   // table 0042B338[0] (index byte 0042B350[0]=0) -> 00429D29
        procs::glActiveTextureARB_007bb80c(0x84C0); // 00429D29..00429D2E call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 00429D3A..00429D3F call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glEnable_0073d390(0x84F5); // 00429D41..00429D46 call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glBindTexture_0073d240(0x84F5, t0); // 00429D52..00429D58 call ebp=[0073D240] (loaded 00429D4C)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x84F5); // 00429D60..00429D6F call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2802, 0x2900); // 00429D77..00429D86 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2803, 0x2900); // 00429D88..00429D97 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2801, 0x2601); // 00429D99..00429DA8 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2800, 0x2601); // 00429DAA..00429DB9 call esi=[0073D23C] (loaded 00429D71)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 00429DBB..00429DC0 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 00429DC6..00429DCB call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 00429DCD..00429DD2 call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t1); // 00429DD8..00429DDE call ebp=[0073D240] (loaded 00429D4C)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 00429DE0..00429DEF call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86E9); // 00429DF1..00429E00 call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 00429E02..00429E11 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 00429E13..00429E22 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 00429E24..00429E33 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 00429E35..00429E44 call esi=[0073D23C] (loaded 00429D71)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 00429E46..00429E4B call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 00429E51..00429E56 call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 00429E58..00429E5D call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t2); // 00429E63..00429E69 call ebp=[0073D240] (loaded 00429D4C)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 00429E6B..00429E7A call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86EA); // 00429E7C..00429E8B call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 00429E8D..00429E9C call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 00429E9E..00429EAD call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 00429EAF..00429EBE call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 00429EC0..00429ECF call esi=[0073D23C] (loaded 00429D71)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 00429ED1..00429ED6 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 00429EDC..00429EE1 call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 00429EE3..00429EE8 call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t3); // 00429EEE..00429EF4 call ebp=[0073D240] (loaded 00429D4C)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86EA); // 00429EF6..00429F05 call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 00429F07..00429F16 call ebx=[0073D2F4] (loaded 00429D5A)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 00429F18..00429F27 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 00429F29..00429F38 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 00429F3A..00429F49 call esi=[0073D23C] (loaded 00429D71)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 00429F4B..00429F5A call esi=[0073D23C] (loaded 00429D71)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 00429F5C..00429F61 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x8522); // 00429F67..00429F6C call edi=[0073D390] (loaded 00429D34)
        kotor_renderer_init::glEnable_0073d390(0x8535); // 00429F6E..00429F73 call edi=[0073D390] (loaded 00429D34)
        procs::glCombinerParameteriNV_007bb664(0x854E, 2); // 00429F75..00429F7C call [007BB664]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852A, fb_shader_constants_0078e110[14]); // 00429F82..00429F91 call [007BB6F8]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852B, fb_shader_constants_0078e110[13]); // 00429F97..00429FA6 call [007BB6F8]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C1, 0x8536, 0x1907); // 00429FAC..00429FCA call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 00429FD0..00429FEE call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0x84C2, 0x8536, 0x1907); // 00429FF4..0042A012 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0x852B, 0x8536, 0x1907); // 0042A018..0042A036 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042A03C..0042A05F call [007BB59C]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8551, 0x852A, fb_shader_constants_0078e110[12]); // 0042A065..0042A074 call [007BB6F8]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x84C3, 0x8536, 0x1907); // 0042A07A..0042A098 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042A09E..0042A0BC call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x852F, 0x8536, 0x1907); // 0042A0C2..0042A0E0 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0, 0x8537, 0x1907); // 0042A0E6..0042A101 call [007BB738]
        // 0042A12A jmp 0042A89F (compiler tail-merged sequence, continued inline)
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042A107..0042A89F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A8A5..0042A8B6 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042A8BC..0042A8D0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A8D6..0042A8EA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A8F0..0042A901 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0, 0x8537, 0x1906); // 0042A907..0042A918 call [007BB654]
        return; // 0042A922 ret
    }
    case 0x2: {   // table 0042B338[1] (0042B350[1]=1) -> 0042A368
        kotor_renderer_init::glDisable_0073d22c(0x8522); // 0042A36E..0042A373 call esi=[0073D22C] (loaded 0042A368)
        kotor_renderer_init::glDisable_0073d22c(0x8535); // 0042A375..0042A37A call esi=[0073D22C] (loaded 0042A368)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 0042A37C..0042A381 call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0x84F5); // 0042A387..0042A38C call esi=[0073D22C] (loaded 0042A368)
        kotor_renderer_init::glBindTexture_0073d240(0x84F5, 0); // 0042A394..0042A39B call edi=[0073D240] (loaded 0042A38E)
        kotor_renderer_init::glDisable_0073d22c(0x86DE); // 0042A39D..0042A3A2 call esi=[0073D22C] (loaded 0042A368)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 0042A3A4..0042A3A9 call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 0042A3AF..0042A3B4 call esi=[0073D22C] (loaded 0042A368)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 0042A3B6..0042A3BD call edi=[0073D240] (loaded 0042A38E)
        kotor_renderer_init::glDisable_0073d22c(0x86DE); // 0042A3BF..0042A3C4 call esi=[0073D22C] (loaded 0042A368)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 0042A3C6..0042A3CB call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 0042A3D1..0042A3D6 call esi=[0073D22C] (loaded 0042A368)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 0042A3D8..0042A3DF call edi=[0073D240] (loaded 0042A38E)
        kotor_renderer_init::glDisable_0073d22c(0x86DE); // 0042A3E1..0042A3E6 call esi=[0073D22C] (loaded 0042A368)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 0042A3E8..0042A3ED call [007BB80C]
        kotor_renderer_init::glDisable_0073d22c(0xDE1); // 0042A3F3..0042A3F8 call esi=[0073D22C] (loaded 0042A368)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, 0); // 0042A3FA..0042A401 call edi=[0073D240] (loaded 0042A38E)
        kotor_renderer_init::glDisable_0073d22c(0x86DE); // 0042A403..0042A408 call esi=[0073D22C] (loaded 0042A368)
        procs::glActiveTextureARB_007bb80c(0x84C0); // 0042A40A..0042A40F call [007BB80C]
        return; // 0042A419 ret
    }
    case 0x4: {   // table 0042B338[2] (0042B350[3]=2) -> 0042A12F
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A135..0042A13A call esi=[0073D390] (loaded 0042A12F)
        kotor_renderer_init::glEnable_0073d390(0x8535); // 0042A13C..0042A141 call esi=[0073D390] (loaded 0042A12F)
        procs::glCombinerParameteriNV_007bb664(0x854E, 1); // 0042A143..0042A14A call [007BB664]
        // 0042A15F jmp 0042A804 (compiler tail-merged sequence, continued inline)
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 0042A150..0042A813 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042A819..0042A834 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0, 0x8536, 0x1907); // 0042A83A..0042A855 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0, 0x8536, 0x1907); // 0042A85B..0042A876 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x852E, 0x8530, 0x8530, 0, 0, 0, 0, 0); // 0042A87C..0042A89F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A8A5..0042A8B6 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042A8BC..0042A8D0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A8D6..0042A8EA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A8F0..0042A901 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0, 0x8537, 0x1906); // 0042A907..0042A918 call [007BB654]
        return; // 0042A922 ret
    }
    case 0x8: {   // table 0042B338[3] (0042B350[7]=3) -> 0042A164
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A164..0042A169 call [0073D390]
        procs::glCombinerParameteriNV_007bb664(0x854E, 3); // 0042A16F..0042A176 call [007BB664]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 0042A17C..0042A19A call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042A1A0..0042A1BE call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0x84C1, 0x8536, 0x1907); // 0042A1C4..0042A1E2 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0x852B, 0x8536, 0x1907); // 0042A1E8..0042A206 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042A20C..0042A22F call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x852E, 0x8536, 0x1907); // 0042A235..0042A253 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042A259..0042A274 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x84C2, 0x8536, 0x1907); // 0042A27A..0042A298 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0x852A, 0x8536, 0x1907); // 0042A29E..0042A2BC call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042A2C2..0042A2E5 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8523, 0x852E, 0x8536, 0x1907); // 0042A2EB..0042A309 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042A30F..0042A32A call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8525, 0x84C3, 0x8536, 0x1907); // 0042A330..0042A34E call [007BB738]
        // 0042A363 jmp 0042B278 (compiler tail-merged sequence, continued inline)
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8526, 0x852A, 0x8536, 0x1907); // 0042A354..0042B287 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8552, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042B28D..0042B2B0 call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042B2B6..0042B2C7 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042B2CD..0042B2E1 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042B2E7..0042B2FB call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042B301..0042B312 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0x8577, 0x8536, 0x1906); // 0042B318..0042B32C call [007BB654]
        return; // 0042B336 ret
    }
    case 0x16: {   // table 0042B338[4] (0042B350[0x15]=4) -> 004299EF
        kotor_renderer_init::glEnable_0073d390(0x8522); // 004299F5..004299FA call esi=[0073D390] (loaded 004299EF)
        kotor_renderer_init::glEnable_0073d390(0x8535); // 004299FC..00429A01 call esi=[0073D390] (loaded 004299EF)
        procs::glCombinerParameteriNV_007bb664(0x854E, 4); // 00429A03..00429A0A call [007BB664]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 00429A10..00429A2E call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 00429A34..00429A52 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0x84C0, 0x8536, 0x1907); // 00429A58..00429A76 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0x852B, 0x8536, 0x1907); // 00429A7C..00429A9A call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x852E, 0x852F, 0x8530, 0, 0, 1, 1, 0); // 00429AA0..00429AC3 call [007BB59C]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8551, 0x852A, fb_shader_constants_0078e110[17]); // 00429AC9..00429AD8 call [007BB6F8]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8551, 0x852B, fb_shader_constants_0078e110[16]); // 00429ADE..00429AED call [007BB6F8]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x852E, 0x8536, 0x1907); // 00429AF3..00429B11 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 00429B17..00429B35 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x852F, 0x8536, 0x1907); // 00429B3B..00429B59 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0x852B, 0x8536, 0x1907); // 00429B5F..00429B7D call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 00429B83..00429BA6 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 00429BAC..00429BCA call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 00429BD0..00429BEE call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8525, 0, 0x8536, 0x1907); // 00429BF4..00429C0F call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8526, 0, 0x8536, 0x1907); // 00429C15..00429C30 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8552, 0x1907, 0x852F, 0x8530, 0x8530, 0, 0, 1, 0, 0); // 00429C36..00429C59 call [007BB59C]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8553, 0x852A, fb_shader_constants_0078e110[15]); // 00429C5F..00429C6E call [007BB6F8]
        procs::glCombinerInputNV_007bb738(0x8553, 0x1907, 0x8523, 0x852E, 0x8536, 0x1907); // 00429C74..00429C92 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8553, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 00429C98..00429CB3 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8553, 0x1907, 0x8525, 0x852F, 0x8536, 0x1907); // 00429CB9..00429CD7 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8553, 0x1907, 0x8526, 0x852A, 0x8536, 0x1907); // 00429CDD..00429CFB call [007BB738]
        // 00429D24 jmp 0042A89F (compiler tail-merged sequence, continued inline)
        procs::glCombinerOutputNV_007bb59c(0x8553, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 00429D01..0042A89F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A8A5..0042A8B6 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042A8BC..0042A8D0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A8D6..0042A8EA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A8F0..0042A901 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0, 0x8537, 0x1906); // 0042A907..0042A918 call [007BB654]
        return; // 0042A922 ret
    }
    case 0x32: {   // 004299D1 je -> 0042A41A
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A420..0042A425 call esi=[0073D390] (loaded 0042A41A)
        procs::glCombinerParameteriNV_007bb664(0x854E, 1); // 0042A427..0042A42E call [007BB664]
        kotor_renderer_init::glEnable_0073d390(0x8535); // 0042A434..0042A439 call esi=[0073D390] (loaded 0042A41A)
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 0042A43B..0042A459 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042A45F..0042A47A call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0x84C1, 0x8536, 0x1907); // 0042A480..0042A49E call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0, 0x8537, 0x1907); // 0042A4A4..0042A4BF call [007BB738]
        // 0042A4DE jmp 0042A895 (compiler tail-merged sequence, continued inline)
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042A4C5..0042A89F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A8A5..0042A8B6 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042A8BC..0042A8D0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A8D6..0042A8EA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A8F0..0042A901 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0, 0x8537, 0x1906); // 0042A907..0042A918 call [007BB654]
        return; // 0042A922 ret
    }
    case 0x64: {   // 0042A4F7 je -> 0042A713
        procs::glActiveTextureARB_007bb80c(0x84C0); // 0042A713..0042A718 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042A724..0042A729 call esi=[0073D390] (loaded 0042A71E)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 0042A72B..0042A730 call esi=[0073D390] (loaded 0042A71E)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0xDE1); // 0042A738..0042A747 call edi=[0073D2F4] (loaded 0042A732)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 0042A749..0042A74E call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042A754..0042A759 call esi=[0073D390] (loaded 0042A71E)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 0042A75B..0042A76A call edi=[0073D2F4] (loaded 0042A732)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86EC); // 0042A76C..0042A77B call edi=[0073D2F4] (loaded 0042A732)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86D9, 0x8538); // 0042A77D..0042A78C call edi=[0073D2F4] (loaded 0042A732)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 0042A78E..0042A793 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042A799..0042A79E call esi=[0073D390] (loaded 0042A71E)
        kotor_renderer_init::glEnable_0073d390(0x84F5); // 0042A7A0..0042A7A5 call esi=[0073D390] (loaded 0042A71E)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 0042A7A7..0042A7B6 call edi=[0073D2F4] (loaded 0042A732)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x864E); // 0042A7B8..0042A7C7 call edi=[0073D2F4] (loaded 0042A732)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86D9, 0x8538); // 0042A7C9..0042A7D8 call edi=[0073D2F4] (loaded 0042A732)
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A7DA..0042A7DF call esi=[0073D390] (loaded 0042A71E)
        procs::glCombinerParameteriNV_007bb664(0x854E, 1); // 0042A7E1..0042A7E8 call [007BB664]
        kotor_renderer_init::glEnable_0073d390(0x8535); // 0042A7EE..0042A7F3 call esi=[0073D390] (loaded 0042A71E)
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C2, 0x8536, 0x1907); // 0042A7F5..0042A813 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042A819..0042A834 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0, 0x8536, 0x1907); // 0042A83A..0042A855 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0, 0x8536, 0x1907); // 0042A85B..0042A876 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x852E, 0x8530, 0x8530, 0, 0, 0, 0, 0); // 0042A87C..0042A89F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A8A5..0042A8B6 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042A8BC..0042A8D0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A8D6..0042A8EA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A8F0..0042A901 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0, 0x8537, 0x1906); // 0042A907..0042A918 call [007BB654]
        return; // 0042A922 ret
    }
    case 0x128: {   // 0042A4FD cmp 128h / 0042A502 jne -> 0042A508
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A50E..0042A513 call esi=[0073D390] (loaded 0042A508)
        procs::glCombinerParameteriNV_007bb664(0x854E, 2); // 0042A515..0042A51C call [007BB664]
        kotor_renderer_init::glEnable_0073d390(0x8535); // 0042A522..0042A527 call esi=[0073D390] (loaded 0042A508)
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 0042A529..0042A547 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042A54D..0042A56B call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0, 0x8536, 0x1907); // 0042A571..0042A58C call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0, 0x8536, 0x1907); // 0042A592..0042A5AD call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x852E, 0x8530, 0x8530, 0, 0, 0, 0, 0); // 0042A5B3..0042A5D6 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x84C1, 0x8536, 0x1907); // 0042A5DC..0042A5FA call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0x852B, 0x8536, 0x1907); // 0042A600..0042A61E call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x852E, 0x8536, 0x1907); // 0042A624..0042A642 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0, 0x8537, 0x1907); // 0042A648..0042A663 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042A669..0042A68C call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042A692..0042A6A3 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852F, 0x8536, 0x1907); // 0042A6A9..0042A6BD call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042A6C3..0042A6D7 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042A6DD..0042A6EE call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0x852A, 0x8536, 0x1906); // 0042A6F4..0042A708 call [007BB654]
        return; // 0042A712 ret
    }
    case 0x256: {   // 0042A4EE je -> 0042A923
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042A923..0042A928 call [0073D390]
        procs::glCombinerParameteriNV_007bb664(0x854E, 3); // 0042A92E..0042A935 call [007BB664]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C0, 0x8536, 0x1907); // 0042A93B..0042A959 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042A95F..0042A97D call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0x84C1, 0x8536, 0x1907); // 0042A983..0042A9A1 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0x852B, 0x8536, 0x1907); // 0042A9A7..0042A9C5 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1906, 0x8523, 0x84C0, 0x8536, 0x1906); // 0042A9CB..0042A9E9 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1906, 0x8524, 0x852A, 0x8536, 0x1906); // 0042A9EF..0042AA0D call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1906, 0x8525, 0x84C1, 0x8536, 0x1906); // 0042AA13..0042AA31 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1906, 0x8526, 0x852B, 0x8536, 0x1906); // 0042AA37..0042AA55 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042AA5B..0042AA7E call [007BB59C]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1906, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042AA84..0042AAA7 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x852F, 0x8536, 0x1907); // 0042AAAD..0042AACB call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042AAD1..0042AAEC call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x84C2, 0x8536, 0x1907); // 0042AAF2..0042AB10 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0x852A, 0x8536, 0x1907); // 0042AB16..0042AB34 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1906, 0x8523, 0x852F, 0x8536, 0x1906); // 0042AB3A..0042AB58 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1906, 0x8524, 0, 0x8537, 0x1906); // 0042AB5E..0042AB79 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1906, 0x8525, 0x84C2, 0x8536, 0x1906); // 0042AB7F..0042AB9D call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1906, 0x8526, 0x852A, 0x8536, 0x1906); // 0042ABA3..0042ABC1 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042ABC7..0042ABEA call [007BB59C]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1906, 0x8530, 0x8530, 0x852F, 0, 0, 0, 0, 0); // 0042ABF0..0042AC13 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8523, 0x852F, 0x8536, 0x1907); // 0042AC19..0042AC37 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8524, 0, 0x8537, 0x1907); // 0042AC3D..0042AC58 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8525, 0x84C3, 0x8536, 0x1907); // 0042AC5E..0042AC7C call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8526, 0x852A, 0x8536, 0x1907); // 0042AC82..0042ACA0 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1906, 0x8523, 0x852F, 0x8536, 0x1906); // 0042ACA6..0042ACC4 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1906, 0x8524, 0, 0x8537, 0x1906); // 0042ACCA..0042ACE5 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1906, 0x8525, 0x84C3, 0x8536, 0x1906); // 0042ACEB..0042AD09 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1906, 0x8526, 0x852A, 0x8536, 0x1906); // 0042AD0F..0042AD2D call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8552, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042AD33..0042AD56 call [007BB59C]
        procs::glCombinerOutputNV_007bb59c(0x8552, 0x1906, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042AD5C..0042AD7F call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042AD85..0042AD96 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042AD9C..0042ADB0 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042ADB6..0042ADCA call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042ADD0..0042ADE1 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0x852E, 0x8536, 0x1906); // 0042ADE7..0042ADFB call [007BB654]
        return; // 0042AE05 ret
    }
    case 0x512: {   // 0042AE06 cmp 512h / 0042AE0B jne -> 0042AE11
        procs::glActiveTextureARB_007bb80c(0x84C0); // 0042AE11..0042AE16 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x84F5); // 0042AE22..0042AE27 call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glBindTexture_0073d240(0x84F5, t0); // 0042AE33..0042AE39 call ebp=[0073D240] (loaded 0042AE2D)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2802, 0x2900); // 0042AE41..0042AE50 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2803, 0x2900); // 0042AE52..0042AE61 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2801, 0x2601); // 0042AE63..0042AE72 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0x84F5, 0x2800, 0x2601); // 0042AE74..0042AE83 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x84F5); // 0042AE8B..0042AE9A call ebx=[0073D2F4] (loaded 0042AE85)
        procs::glActiveTextureARB_007bb80c(0x84C1); // 0042AE9C..0042AEA1 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042AEA7..0042AEAC call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 0042AEAE..0042AEB3 call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t1); // 0042AEB9..0042AEBF call ebp=[0073D240] (loaded 0042AE2D)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 0042AEC1..0042AED0 call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86E9); // 0042AED2..0042AEE1 call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 0042AEE3..0042AEF2 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 0042AEF4..0042AF03 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 0042AF05..0042AF14 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 0042AF16..0042AF25 call esi=[0073D23C] (loaded 0042AE3B)
        procs::glActiveTextureARB_007bb80c(0x84C2); // 0042AF27..0042AF2C call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042AF32..0042AF37 call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 0042AF39..0042AF3E call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t2); // 0042AF44..0042AF4A call ebp=[0073D240] (loaded 0042AE2D)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 0042AF4C..0042AF5B call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86EA); // 0042AF5D..0042AF6C call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 0042AF6E..0042AF7D call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 0042AF7F..0042AF8E call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 0042AF90..0042AF9F call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 0042AFA1..0042AFB0 call esi=[0073D23C] (loaded 0042AE3B)
        procs::glActiveTextureARB_007bb80c(0x84C3); // 0042AFB2..0042AFB7 call [007BB80C]
        kotor_renderer_init::glEnable_0073d390(0x86DE); // 0042AFBD..0042AFC2 call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glEnable_0073d390(0xDE1); // 0042AFC4..0042AFC9 call edi=[0073D390] (loaded 0042AE1C)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1, t3); // 0042AFCF..0042AFD5 call ebp=[0073D240] (loaded 0042AE2D)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86E4, 0x84C0); // 0042AFD7..0042AFE6 call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DE, 0x86DF, 0x86EA); // 0042AFE8..0042AFF7 call ebx=[0073D2F4] (loaded 0042AE85)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2802, 0x2900); // 0042AFF9..0042B008 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2803, 0x2900); // 0042B00A..0042B019 call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2801, 0x2601); // 0042B01B..0042B02A call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glTexParameteri_0073d23c(0xDE1, 0x2800, 0x2601); // 0042B02C..0042B03B call esi=[0073D23C] (loaded 0042AE3B)
        kotor_renderer_init::glEnable_0073d390(0x8522); // 0042B03D..0042B042 call edi=[0073D390] (loaded 0042AE1C)
        procs::glCombinerParameteriNV_007bb664(0x854E, 3); // 0042B044..0042B04B call [007BB664]
        kotor_renderer_init::glEnable_0073d390(0x8535); // 0042B051..0042B056 call edi=[0073D390] (loaded 0042AE1C)
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8550, 0x852A, fb_shader_constants_0078e110[11]); // 0042B058..0042B067 call [007BB6F8]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8551, 0x852A, fb_shader_constants_0078e110[10]); // 0042B06D..0042B07C call [007BB6F8]
        procs::glCombinerStageParameterfvNV_007bb6f8(0x8552, 0x852A, fb_shader_constants_0078e110[9]); // 0042B082..0042B091 call [007BB6F8]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8523, 0x84C1, 0x8536, 0x1907); // 0042B097..0042B0B5 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042B0BB..0042B0D9 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8525, 0, 0x8536, 0x1907); // 0042B0DF..0042B0FA call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8550, 0x1907, 0x8526, 0, 0x8536, 0x1907); // 0042B100..0042B11B call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8550, 0x1907, 0x852E, 0x8530, 0x8530, 0, 0, 0, 0, 0); // 0042B121..0042B144 call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8523, 0x84C2, 0x8536, 0x1907); // 0042B14A..0042B168 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042B16E..0042B18C call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8525, 0x852E, 0x8536, 0x1907); // 0042B192..0042B1B0 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8551, 0x1907, 0x8526, 0, 0x8537, 0x1907); // 0042B1B6..0042B1D1 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8551, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042B1D7..0042B1FA call [007BB59C]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8523, 0x84C3, 0x8536, 0x1907); // 0042B200..0042B21E call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8524, 0x852A, 0x8536, 0x1907); // 0042B224..0042B242 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8525, 0x852E, 0x8536, 0x1907); // 0042B248..0042B266 call [007BB738]
        procs::glCombinerInputNV_007bb738(0x8552, 0x1907, 0x8526, 0, 0x8537, 0x1907); // 0042B26C..0042B287 call [007BB738]
        procs::glCombinerOutputNV_007bb59c(0x8552, 0x1907, 0x8530, 0x8530, 0x852E, 0, 0, 0, 0, 0); // 0042B28D..0042B2B0 call [007BB59C]
        procs::glFinalCombinerInputNV_007bb654(0x8523, 0, 0x8537, 0x1906); // 0042B2B6..0042B2C7 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8524, 0x852E, 0x8536, 0x1907); // 0042B2CD..0042B2E1 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8525, 0xB60, 0x8536, 0x1907); // 0042B2E7..0042B2FB call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8526, 0, 0x8536, 0x1907); // 0042B301..0042B312 call [007BB654]
        procs::glFinalCombinerInputNV_007bb654(0x8529, 0x8577, 0x8536, 0x1906); // 0042B318..0042B32C call [007BB654]
        return; // 0042B336 ret
    }
    default:
        return;   // 0042B332..0042B336 (pop edi/esi/ebp/ebx; ret)
    }
}

} // extern "C"
