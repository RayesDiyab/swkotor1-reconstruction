// fb_bloom_conv.cpp - lane-3d-fb (fork B): GLRender::BloomFilter (ATI 0042B810 / NV 0042BF80) and
// GLRender::RenderOverlayConvolution (ATI 0042C6E0 / NV 0042CC60). NOT executed on the first main-menu frame
// (reachable only from FrameBufferModifications* / FinishSoftShadows*, gated by Scene+D4 != 0; gates.json).
// Every statement cites the original instruction(s) (lane-local d/<addr>.txt).
#include <cstdint>
#include "056e5913e7-fb_bloom.h"

// ---------------------------------------------------------------- local prototypes (cdecl, plain RET)
extern "C" {
// 0042B810 / 0042BF80: call sites 0042C9E6..0042CA03, 0042D73C..0042D757, 0042CED8..0042CEF5, 0042DE84..0042DE9F
// (8 DWORDs, add esp,20h): dc, context, width, height (float, 256.0/512.0 immediates), pbuffer, texture, size (float), flag.
void __cdecl gl_bloom_filter_ati_0042b810(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size, kotor_fb::U flag);
void __cdecl gl_bloom_filter_0042bf80(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size, kotor_fb::U flag);
// core (lane-3d-render SUBLANES "core"), unauthored: GLRender::MipmapFilter ATI 0042B370 / NV 0042B5D0.
// Call sites 0042C94F..0042C9C5 / 0042CE41..0042CEB7: 7 DWORDs each (3 calls, add esp,54h): dc, context,
// width, height (float), pbuffer, texture, size (float).
void __cdecl gl_mipmap_filter_ati_0042b370(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size);
void __cdecl gl_mipmap_filter_0042b5d0(void* dc, void* context, float width, float height, void* pbuffer,
    kotor_fb::U texture, float size);
}

namespace {
using namespace kotor_fb;
using namespace kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
using kotor_scene3d::cells;

// Handle words (007A47E4 / 007A6854 are U32 cells) passed where the WGL procedure takes a handle pointer.
inline void* handle(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }

// __ftol2 006FAE8C on a float loaded by `fld dword` (D30-FINAL: original CRT port, ST0 -> EDX:EAX; EAX used).
I ftol2_float(float value) {
    I converted;
    __asm { fld value }
    __asm { call unresolved_ftol2_006fae8c }
    __asm { mov converted, eax }
    return converted;
}

// Four glTexParameteri calls on GL_TEXTURE_2D (esi = [0073D23C]): WRAP_S/T = CLAMP, MIN/MAG = LINEAR.
void clamp_linear(U target) {
    glTexParameteri_0073d23c(target, 0x2802, 0x2900);
    glTexParameteri_0073d23c(target, 0x2803, 0x2900);
    glTexParameteri_0073d23c(target, 0x2801, 0x2601);
    glTexParameteri_0073d23c(target, 0x2800, 0x2601);
}

// Kernel element read by `[eax*4+table]` with eax = k + i (k = 0,4,..,24; i = 0..3), i.e. indices 0..27.
// Indices 25..27 address the 12 pad bytes after each 25-float kernel (0078E294..0078E29F and 0078E304..0078E30F),
// which are zero in the image and have no writer instruction; header declares [25] (see B.json header issue).
float kernel_at(const float* kernel, I index) { return kernel[index]; }   // tables are float[28] incl. original zero pad (fb_cells.cpp)

// Shared 5x5-tap quad loop of both BloomFilter variants (ATI 0042BAFE..0042BEC2, NV 0042C24C..0042C610; the two
// bodies are instruction-identical except for the constant upload and the kernel table).
template<class Upload>
void bloom_quads(const float* kernel, float width, float height, float size, U flag, Upload upload) {
    I row = 0;                                                         // L18 = 0 (0042BAEA / 0042C238)
    I col = 0;                                                         // L20 = 0 (0042BAEE / 0042C23C)
    const float inv = static_cast<float>(f(kFloat1_0073d764) / static_cast<double>(size)); // 0042BAF4..0042BB04 fld 1.0; fdiv size; fstp L10
    for (I k = 0; k < 25; k += 4) {                                    // L24 0042BAFE; 0042BEB0..0042BEC2 add 4; cmp 19h; jl
        for (I i = 0; i < 4; ++i) {                                    // esi 0042BB12 / 0042BBA8..0042BBAC
            const I index = i + k;                                     // 0042BB1A lea eax,[esi+edi]
            float c[4];
            if (flag != 0) {                                           // 0042BB14..0042BB1D
                const float v = static_cast<float>(static_cast<double>(fb_bloom_param_0078db04) *
                    static_cast<double>(kernel_at(kernel, index)));  // 0042BB1F fld [0078DB04]; fmul [eax*4+kernel]
                c[0] = v; c[1] = v; c[2] = v; c[3] = v;                // 0042BB2C/30/34 fst; 0042BB64 fstp
            } else {
                c[0] = kernel_at(kernel, index);                       // 0042BB3A / 0042BB41
                c[1] = kernel_at(kernel, index);                       // 0042BB45 / 0042BB4C
                c[2] = kernel_at(kernel, index);                       // 0042BB50 / 0042BB57
                c[3] = kernel_at(kernel, index);                       // 0042BB5B / 0042BB64
            }
            upload(i, c);                                              // 0042BB62..0042BBA2 (ATI) / 0042C2B0..0042C304 (NV)
        }
        glBegin_0073d2d0(7);                                           // 0042BBB2 GL_QUADS
        const float rowf = static_cast<float>(row);                    // 0042BBBA fild L18; 0042BBC4 fstp L28
        const float colf = static_cast<float>(col);                    // 0042BBCD fild L20; 0042BBD3 fstp L2C
        const I col1 = col + 1;                                        // 0042BBBE lea edi,[ebx+1]
        const I col2 = col + 2;                                        // 0042BBD7 add ebx,2
        const I wrap1 = col1 % 5;                                      // 0042BBC1..0042BBDE idiv -> L38
        const I wrap2 = col2 % 5;                                      // 0042BBDA..0042BBEC -> L3C
        const I wrap3 = (col + 3) % 5;                                 // 0042BBE5..0042BBF3 -> L40
        for (I v = 0; v < 4; ++v) {                                    // esi 0042BBDC / 0042BE88..0042BE8C
            glColor4f_0073d224(f(kFloat1_0073d764), f(kFloat1_0073d764), f(kFloat1_0073d764),
                fb_bloom_param_0078daf8);                              // 0042BC00..0042BC16 (alpha = raw [0078DAF8])
            const bool add_first = (v == 0 || v == 3);                 // test esi / cmp esi,3
            const bool add_second = !(v == 0 || v == 1);               // test esi / cmp esi,1
            // unit 0 (0042BC1C..0042BC8A)
            procs::glActiveTextureARB_007bb80c(0x84C0);
            float t0 = static_cast<float>(((add_first ? static_cast<double>(rowf) + static_cast<double>(size)
                : static_cast<double>(rowf)) - static_cast<double>(fb_bloom_param_0078db00)) * static_cast<double>(inv)); // 0042BC27..0042BC4A -> L1C
            float s0 = static_cast<float>(((add_second ? static_cast<double>(colf) + static_cast<double>(size)
                : static_cast<double>(colf)) - static_cast<double>(fb_bloom_param_0078db00)) * static_cast<double>(inv)); // 0042BC4E..0042BC7B -> L14
            procs::glMultiTexCoord2fARB_007bb6e8(0x84C0, s0, t0);      // 0042BC77..0042BC8A
            // units 1..3 (0042BC90..0042BE2F): row advances when the column offset wraps past 4
            const I tap_col[3] = {wrap1, wrap2, wrap3};
            const I tap_raw[3] = {col1, col2, col + 3};                // cmp edi,4 / cmp ebx,4 / [esp+20h]+3 (NV: ebp)
            for (I u = 0; u < 3; ++u) {
                const I tap_row = tap_raw[u] > 4 ? row + 1 : row;      // 0042BC90..0042BCA4 jle -> L14 (int)
                procs::glActiveTextureARB_007bb80c(0x84C1 + static_cast<U>(u)); // 0042BCAC..0042BCB5
                const float t = static_cast<float>(((add_first ? static_cast<double>(tap_col[u]) + static_cast<double>(size)
                    : static_cast<double>(tap_col[u])) - static_cast<double>(fb_bloom_param_0078db00)) * static_cast<double>(inv)); // 0042BCBB..0042BCDE fild L1C -> L1C
                const float s = static_cast<float>(((add_second ? static_cast<double>(tap_row) + static_cast<double>(size)
                    : static_cast<double>(tap_row)) - static_cast<double>(fb_bloom_param_0078db00)) * static_cast<double>(inv)); // 0042BCE2..0042BD06 fild L14 -> L14
                procs::glMultiTexCoord2fARB_007bb6e8(0x84C1 + static_cast<U>(u), s, t); // 0042BCFD..0042BD14
            }
            procs::glActiveTextureARB_007bb80c(0x84C0);                // 0042BE35..0042BE3A
            const float y = add_first ? height : 0.0f;                 // 0042BE40..0042BE57 -> L30
            const float x = add_second ? width : 0.0f;                 // 0042BE5B..0042BE6E -> L34
            glVertex3f_0073d2c8(x, y, 0.0f);                           // 0042BE76..0042BE82
        }
        glEnd_0073d2c4();                                              // 0042BE92
        const I next = col + 4;                                        // 0042BE98 add eax,4
        if (next > 4) ++row;                                           // 0042BE9F..0042BEA4 inc L18
        col = next % 5;                                                // 0042BEA8..0042BEBE idiv -> L20
    }
}

void bind_pbuffer_unit(U unit, U texture, void* pbuffer) {
    procs::glActiveTextureARB_007bb80c(unit);                          // e.g. 0042B8E4..0042B8E9
    glEnable_0073d390(0x0DE1);                                         // 0042B8EF..0042B8F4 (edi = [0073D390])
    glBindTexture_0073d240(0x0DE1, texture);                           // 0042B900..0042B906 (ebp = [0073D240])
    procs::wglBindTexImageARB_007bb7b8(pbuffer, 0x2083);               // 0042B908..0042B912 WGL_FRONT_LEFT_ARB
    clamp_linear(0x0DE1);                                              // 0042B918..0042B960
}

// Window-sized ortho (0042CA32..0042CA60 pattern): fild height, fild width -> doubles.
void ortho_window(U width_bits, U height_bits) {
    glOrtho_0073d234(0.0, static_cast<double>(static_cast<I>(width_bits)), 0.0,
        static_cast<double>(static_cast<I>(height_bits)), -1.0, 1.0);
}
} // namespace

extern "C" {

// 0042B810 GLRender::BloomFilter (ATI fragment shaders), cdecl 8 args, ends with tail jmp [0073D2F8] glFlush.
void __cdecl gl_bloom_filter_ati_0042b810(void* dc, void* context, float width, float height, void* pbuffer,
    U texture, float size, U flag) {
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0);   // 0042B817..0042B821
    procs::wglMakeContextCurrentARB_007bb818(dc, dc, context);         // 0042B826..0042B834
    glEnable_0073d390(0x0BE2);                                         // 0042B83A..0042B845 GL_BLEND
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042B847..0042B84E
    glMatrixMode_0073d228(0x1701);                                     // 0042B854..0042B85F
    glLoadIdentity_0073d230();                                         // 0042B861..0042B867
    glOrtho_0073d234(0.0, static_cast<double>(width), 0.0, static_cast<double>(height), -1.0, 1.0); // 0042B869..0042B893
    glMatrixMode_0073d228(0x1700);                                     // 0042B899..0042B89E
    glLoadIdentity_0073d230();                                         // 0042B8A0
    {
        const I h = ftol2_float(height);                               // 0042B8A2..0042B8A6
        const I w = ftol2_float(width);                                // 0042B8AB..0042B8B0
        glViewport_0073d2bc(0, 0, w, h);                               // 0042B8B5..0042B8BA
    }
    glDisable_0073d22c(0x0B71);                                        // 0042B8C0..0042B8C5 GL_DEPTH_TEST
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042B8CB..0042B8D3
    glClear_0073d21c(0x4000);                                          // 0042B8D9..0042B8DE
    bind_pbuffer_unit(0x84C0, texture, pbuffer);                       // 0042B8E4..0042B960
    bind_pbuffer_unit(0x84C1, texture, pbuffer);                       // 0042B962..0042B9CE
    bind_pbuffer_unit(0x84C2, texture, pbuffer);                       // 0042B9D0..0042BA3C
    bind_pbuffer_unit(0x84C3, texture, pbuffer);                       // 0042BA3E..0042BAAA
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042BAAC..0042BAB1
    if (flag != 0) {                                                   // 0042BAB7..0042BABF
        glEnable_0073d390(0x8920);                                     // 0042BAC1..0042BAC6 GL_FRAGMENT_SHADER_ATI
        procs::glBindFragmentShaderATI_007bb6cc(*kotor_soft_shadow::bindings.shader_007a6928); // 0042BAC8..0042BACF
    } else {
        gl_frame_buffer_modification_pixel_shaders_ati_004293f0(8, 0, 0, 0, 0); // 0042BAD7..0042BAE2
    }
    glEnable_0073d390(0x0BE2);                                         // 0042BAE5..0042BAF2
    bloom_quads(fb_bloom_kernel_0078e230, width, height, size, flag,
        [](I i, const float* c) {                                      // 0042BB62..0042BBA2
            procs::glSetFragmentShaderConstantATI_007bb5d8(0x8941 + static_cast<U>(i), c); // GL_CON_1..4_ATI
        });
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042BEC8..0042BECD
    procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);            // 0042BED3..0042BEDD
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042BEE3..0042BEEA
    for (U unit = 0x84C1; unit <= 0x84C3; ++unit) {                    // 0042BEEC..0042BF65 (unrolled x3)
        procs::glActiveTextureARB_007bb80c(unit);
        glDisable_0073d22c(0x0DE1);
        procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);
        glBindTexture_0073d240(0x0DE1, 0);
    }
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042BF67..0042BF6C
    glFlush_0073d2f8();                                                // 0042BF79 jmp [0073D2F8]
}

// 0042BF80 GLRender::BloomFilter (NV register combiners), cdecl 8 args, tail jmp [0073D2F8] glFlush.
void __cdecl gl_bloom_filter_0042bf80(void* dc, void* context, float width, float height, void* pbuffer,
    U texture, float size, U flag) {
    wglMakeCurrent_0073d2b0(dc, context);                              // 0042BF80..0042BF91
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042BF97..0042BF9E
    glMatrixMode_0073d228(0x1701);                                     // 0042BFA4..0042BFAF
    glLoadIdentity_0073d230();                                         // 0042BFB1..0042BFB7
    glOrtho_0073d234(0.0, static_cast<double>(width), 0.0, static_cast<double>(height), -1.0, 1.0); // 0042BFB9..0042BFE3
    glMatrixMode_0073d228(0x1700);                                     // 0042BFE9..0042BFEE
    glLoadIdentity_0073d230();                                         // 0042BFF0
    {
        const I h = ftol2_float(height);                               // 0042BFF2..0042BFF6
        const I w = ftol2_float(width);                                // 0042BFFB..0042C000
        glViewport_0073d2bc(0, 0, w, h);                               // 0042C005..0042C00A
    }
    glDisable_0073d22c(0x0B71);                                        // 0042C010..0042C015
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042C01B..0042C023
    glClear_0073d21c(0x4000);                                          // 0042C029..0042C02E
    bind_pbuffer_unit(0x84C0, texture, pbuffer);                       // 0042C034..0042C0B6
    bind_pbuffer_unit(0x84C1, texture, pbuffer);                       // 0042C0B8..0042C124
    bind_pbuffer_unit(0x84C2, texture, pbuffer);                       // 0042C126..0042C192
    bind_pbuffer_unit(0x84C3, texture, pbuffer);                       // 0042C194..0042C200
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042C202..0042C207
    glEnable_0073d390(0x8535);                                         // 0042C20D..0042C212 GL_REGISTER_COMBINERS_NV
    gl_frame_buffer_modification_pixel_shaders_004299c0(flag != 0 ? 0x256u : 8u, 0, 0, 0, 0); // 0042C214..0042C22B
    glEnable_0073d390(0x0BE2);                                         // 0042C233..0042C240
    bloom_quads(fb_bloom_kernel_0078e2a0, width, height, size, flag,
        [](I i, const float* c) {                                      // 0042C2B0..0042C304 glCombinerStageParameterfvNV
            static const U stage[4] = {0x8550, 0x8550, 0x8551, 0x8552}; // GL_COMBINER0/0/1/2_NV
            static const U pname[4] = {0x852A, 0x852B, 0x852A, 0x852A}; // GL_CONSTANT_COLOR0/1_NV
            procs::glCombinerStageParameterfvNV_007bb6f8(stage[i], pname[i], c);
        });
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042C616..0042C61B
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042C621..0042C62E
    procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);            // 0042C630..0042C63A
    for (U unit = 0x84C1; unit <= 0x84C3; ++unit) {                    // 0042C640..0042C6B5 (unrolled x3)
        procs::glActiveTextureARB_007bb80c(unit);
        glDisable_0073d22c(0x0DE1);
        glBindTexture_0073d240(0x0DE1, 0);
        procs::wglReleaseTexImageARB_007bb6d0(pbuffer, 0x2083);
    }
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042C6BB..0042C6C0
    glFlush_0073d2f8();                                                // 0042C6CD jmp [0073D2F8]
}

// 0042C6E0 GLRender::RenderOverlayConvolution (ATI), cdecl 1 arg (texture), plain RET.
void __cdecl gl_render_overlay_convolution_ati_0042c6e0(U texture) {
    const auto& fb = kotor_renderer_init::framebuffer;
    if (fb_convolution_gate_0078db20 == 0) {                           // 0042C6E0..0042C6EA
        glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                 // 0042C6EC..0042C6F4
        glClear_0073d21c(0x4000);                                      // 0042C6FA..0042C6FF
    }
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042C709..0042C716 (edi = [0073D38C])
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 0042C718..0042C71E
    void* const dc3 = fb.dc_007a47ec[3];                               // 0042C72A [007A47F8]
    void* const context3 = fb.contexts_007a4804[3];                    // 0042C724 [007A4810]
    const U main_context = *fb.main_context_007a6854;                  // 0042C72F -> L28 (0042C73E)
    void* const main_dc = handle(*fb.main_dc_007a47e4);                // 0042C735 esi
    procs::wglMakeContextCurrentARB_007bb818(dc3, dc3, context3);      // 0042C73B..0042C742
    glDrawBuffer_0073d304(0x0404);                                     // 0042C748..0042C74D GL_FRONT
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042C753..0042C75A
    glMatrixMode_0073d228(0x1701);                                     // 0042C75C..0042C767
    glLoadIdentity_0073d230();                                         // 0042C769..0042C76F
    glOrtho_0073d234(0.0, 512.0, 0.0, 512.0, -1.0, 1.0);               // 0042C771..0042C795 (40800000:00000000 = 512.0)
    glMatrixMode_0073d228(0x1700);                                     // 0042C79B..0042C7A0
    glLoadIdentity_0073d230();                                         // 0042C7A2
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042C7A4..0042C7B2
    glDisable_0073d22c(0x0B71);                                        // 0042C7B8..0042C7C3
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042C7C5..0042C7CD
    glClear_0073d21c(0x4100);                                          // 0042C7D3..0042C7D8
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 0042C7DE..0042C7E4
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(1, texture, *fb.ramps[0], *fb.ramps[1], *fb.ramps[2]); // 0042C7E6..0042C801
    const float scale_s = static_cast<float>(static_cast<double>(static_cast<I>(*fb.width_0078d1d4)) /
        static_cast<double>(*fb.power_width_007a6890));                // 0042C806..0042C82E (unsigned fixup 0073D770) -> L24
    const float scale_t = static_cast<float>(static_cast<double>(static_cast<I>(*fb.height_0078d1d8)) /
        static_cast<double>(*fb.power_height_007a6894));               // 0042C832..0042C84A -> L20
    glBegin_0073d2d0(7);                                               // 0042C848..0042C84E
    for (I v = 0; v < 4; ++v) {                                        // ebp 0042C854 / 0042C909..0042C90D
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042C860..0042C874
        const float t = (v == 0 || v == 3) ? scale_t : 0.0f;           // 0042C87A..0042C891 -> L10
        const float s = (v == 0 || v == 1) ? 0.0f : scale_s;           // 0042C895..0042C8A8 -> L14
        procs::glMultiTexCoord2fARB_007bb6e8(0x84C0, s, t);            // 0042C8B0..0042C8BF
        const float y = (v == 0 || v == 3) ? 512.0f : 0.0f;            // 0042C8C5..0042C8D6 -> L18 (44000000)
        const float x = (v == 0 || v == 1) ? 0.0f : 512.0f;            // 0042C8DE..0042C8EF -> L1C
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042C8F7..0042C903
    }
    glEnd_0073d2c4();                                                  // 0042C913
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0); // 0042C919..0042C928
    glFlush_0073d2f8();                                                // 0042C92B
    procs::wglMakeContextCurrentARB_007bb818(main_dc, main_dc, handle(main_context)); // 0042C931..0042C938
    gl_mipmap_filter_ati_0042b370(fb.dc_007a47ec[2], fb.contexts_007a4804[2], 256.0f, 256.0f,
        fb.pbuffers_007a6824[3], fb.textures_007a47c4[3], 512.0f);     // 0042C93E..0042C968
    gl_mipmap_filter_ati_0042b370(fb.dc_007a47ec[1], fb.contexts_007a4804[1], 128.0f, 128.0f,
        fb.pbuffers_007a6824[2], fb.textures_007a47c4[2], 256.0f);     // 0042C96D..0042C997
    gl_mipmap_filter_ati_0042b370(fb.dc_007a47ec[0], fb.contexts_007a4804[0], 64.0f, 64.0f,
        fb.pbuffers_007a6824[1], fb.textures_007a47c4[1], 128.0f);     // 0042C99C..0042C9C5
    {
        const U index = fb_word_007a6938;                              // 0042C9CA
        gl_bloom_filter_ati_0042b810(fb.dc_007a47ec[5], fb.contexts_007a4804[5], 256.0f, 256.0f,
            fb.pbuffers_007a6824[index], fb.textures_007a47c4[index], fb_bloom_param_0078db0c, 0); // 0042C9CF..0042CA03
    }
    procs::wglMakeContextCurrentARB_007bb818(main_dc, main_dc, handle(main_context)); // 0042CA0B..0042CA0E
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042CA14..0042CA1B
    glMatrixMode_0073d228(0x1701);                                     // 0042CA21..0042CA26
    glPushMatrix_0073d300();                                           // 0042CA28..0042CA2E
    glLoadIdentity_0073d230();                                         // 0042CA30
    ortho_window(*fb.width_0078d1d4, *fb.height_0078d1d8);             // 0042CA32..0042CA60
    glMatrixMode_0073d228(0x1700);                                     // 0042CA66..0042CA6B
    glPushMatrix_0073d300();                                           // 0042CA6D
    glLoadIdentity_0073d230();                                         // 0042CA6F
    glViewport_0073d2bc(0, 0, static_cast<I>(*fb.width_0078d1d4), static_cast<I>(*fb.height_0078d1d8)); // 0042CA71..0042CA82
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042CA88..0042CA8D
    glEnable_0073d390(0x0DE1);                                         // 0042CA93..0042CA98
    glBindTexture_0073d240(0x0DE1, fb.textures_007a47c4[fb_bloom_count_0078db08]); // 0042CA9E..0042CAB7
    procs::wglBindTexImageARB_007bb7b8(fb.pbuffers_007a6824[fb_bloom_count_0078db08], 0x2083); // 0042CAB9..0042CACC
    clamp_linear(0x0DE1);                                              // 0042CAD2..0042CB1A
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(4, 0, 0, 0, 0); // 0042CB1C..0042CB2B
    glDrawBuffer_0073d304(0x0405);                                     // 0042CB2E..0042CB33 GL_BACK
    glBegin_0073d2d0(7);                                               // 0042CB39..0042CB3B
    for (I v = 0; v < 4; ++v) {                                        // esi 0042CB47 / 0042CBF4..0042CBF8
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042CB50..0042CB64
        const float t = (v == 0 || v == 3) ? 1.0f : 0.0f;              // 0042CB6A..0042CB7B -> L1C
        const float s = (v == 0 || v == 1) ? 0.0f : 1.0f;              // 0042CB83..0042CB94 -> L18
        glTexCoord2f_0073d2cc(s, t);                                   // 0042CB9C..0042CBA6 (ebp = [0073D2CC])
        const float y = (v == 0 || v == 3) ? static_cast<float>(static_cast<I>(*fb.height_0078d1d8)) : 0.0f; // 0042CBA8..0042CBC1 -> L14
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(static_cast<I>(*fb.width_0078d1d4)); // 0042CBC5..0042CBDA -> L10
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042CBE2..0042CBEE
    }
    glEnd_0073d2c4();                                                  // 0042CBFE
    glMatrixMode_0073d228(0x1701);                                     // 0042CC04..0042CC09
    glPopMatrix_0073d2fc();                                            // 0042CC0B..0042CC11
    glMatrixMode_0073d228(0x1700);                                     // 0042CC13..0042CC18
    glPopMatrix_0073d2fc();                                            // 0042CC1A
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042CC1C..0042CC23 (edi = [0073D240])
    procs::wglReleaseTexImageARB_007bb6d0(fb.pbuffers_007a6824[fb_bloom_count_0078db08], 0x2083); // 0042CC25..0042CC38
    gl_frame_buffer_modification_pixel_shaders_ati_004293f0(2, 0, 0, 0, 0); // 0042CC3E..0042CC4D
}                                                                      // 0042CC57 ret

// 0042CC60 GLRender::RenderOverlayConvolution (NV), cdecl 1 arg (texture), plain RET.
void __cdecl gl_render_overlay_convolution_0042cc60(U texture) {
    const auto& fb = kotor_renderer_init::framebuffer;
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042CC65..0042CC74 (ebp = [0073D38C])
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 0042CC76..0042CC7C
    wglMakeCurrent_0073d2b0(fb.dc_007a47ec[3], fb.contexts_007a4804[3]); // 0042CC82..0042CC90
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042CC96..0042CC9D
    glMatrixMode_0073d228(0x1701);                                     // 0042CC9F..0042CCAA
    glLoadIdentity_0073d230();                                         // 0042CCAC..0042CCB2
    glOrtho_0073d234(0.0, 512.0, 0.0, 512.0, -1.0, 1.0);               // 0042CCB4..0042CCD8
    glMatrixMode_0073d228(0x1700);                                     // 0042CCDE..0042CCE3
    glLoadIdentity_0073d230();                                         // 0042CCE5
    glViewport_0073d2bc(0, 0, 0x200, 0x200);                           // 0042CCE7..0042CCF5
    glDisable_0073d22c(0x0B71);                                        // 0042CCFB..0042CD06 (ebx = [0073D22C])
    glClearColor_0073d220(0.0f, 0.0f, 0.0f, 0.0f);                     // 0042CD08..0042CD10
    glClear_0073d21c(0x4000);                                          // 0042CD16..0042CD1B
    glDisable_0073d22c(0x0B71);                                        // 0042CD21..0042CD26
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                   // 0042CD28..0042CD2E
    gl_frame_buffer_modification_pixel_shaders_004299c0(1, texture, *fb.ramps[0], *fb.ramps[1], *fb.ramps[2]); // 0042CD30..0042CD4B
    glBegin_0073d2d0(7);                                               // 0042CD53..0042CD55
    for (I v = 0; v < 4; ++v) {                                        // ebx 0042CD5B / 0042CE08..0042CE0C
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042CD60..0042CD74
        const float t = (v == 0 || v == 3) ? static_cast<float>(static_cast<I>(*fb.height_0078d1d8)) : 0.0f; // 0042CD7A..0042CD93 -> L10
        const float s = (v == 0 || v == 1) ? 0.0f : static_cast<float>(static_cast<I>(*fb.width_0078d1d4)); // 0042CD97..0042CDAC -> L14
        glTexCoord2f_0073d2cc(s, t);                                   // 0042CDB4..0042CDBE
        const float y = (v == 0 || v == 3) ? 512.0f : 0.0f;            // 0042CDC4..0042CDD5 -> L18
        const float x = (v == 0 || v == 1) ? 0.0f : 512.0f;            // 0042CDDD..0042CDEE -> L1C
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042CDF6..0042CE02
    }
    glEnd_0073d2c4();                                                  // 0042CE12
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0); // 0042CE18..0042CE27
    glFlush_0073d2f8();                                                // 0042CE2A
    gl_mipmap_filter_0042b5d0(fb.dc_007a47ec[2], fb.contexts_007a4804[2], 256.0f, 256.0f,
        fb.pbuffers_007a6824[3], fb.textures_007a47c4[3], 512.0f);     // 0042CE30..0042CE59
    gl_mipmap_filter_0042b5d0(fb.dc_007a47ec[1], fb.contexts_007a4804[1], 128.0f, 128.0f,
        fb.pbuffers_007a6824[2], fb.textures_007a47c4[2], 256.0f);     // 0042CE5E..0042CE88
    gl_mipmap_filter_0042b5d0(fb.dc_007a47ec[0], fb.contexts_007a4804[0], 64.0f, 64.0f,
        fb.pbuffers_007a6824[1], fb.textures_007a47c4[1], 128.0f);     // 0042CE8D..0042CEB7
    {
        const U index = fb_word_007a6938;                              // 0042CEBC
        gl_bloom_filter_0042bf80(fb.dc_007a47ec[5], fb.contexts_007a4804[5], 256.0f, 256.0f,
            fb.pbuffers_007a6824[index], fb.textures_007a47c4[index], fb_bloom_param_0078db0c, 0); // 0042CEC1..0042CEF5
    }
    wglMakeCurrent_0073d2b0(handle(*fb.main_dc_007a47e4), handle(*fb.main_context_007a6854)); // 0042CEFA..0042CF0A
    glBlendFunc_0073d38c(0x0302, 1);                                   // 0042CF10..0042CF17
    glMatrixMode_0073d228(0x1701);                                     // 0042CF19..0042CF1E
    glPushMatrix_0073d300();                                           // 0042CF20..0042CF26
    glLoadIdentity_0073d230();                                         // 0042CF28
    ortho_window(*fb.width_0078d1d4, *fb.height_0078d1d8);             // 0042CF2A..0042CF58
    glMatrixMode_0073d228(0x1700);                                     // 0042CF5E..0042CF63
    glPushMatrix_0073d300();                                           // 0042CF65
    glLoadIdentity_0073d230();                                         // 0042CF67
    glViewport_0073d2bc(0, 0, static_cast<I>(*fb.width_0078d1d4), static_cast<I>(*fb.height_0078d1d8)); // 0042CF69..0042CF7A
    procs::glActiveTextureARB_007bb80c(0x84C0);                        // 0042CF80..0042CF85
    glEnable_0073d390(0x0DE1);                                         // 0042CF8B..0042CF90
    glBindTexture_0073d240(0x0DE1, fb.textures_007a47c4[fb_bloom_count_0078db08]); // 0042CF96..0042CFAF
    procs::wglBindTexImageARB_007bb7b8(fb.pbuffers_007a6824[fb_bloom_count_0078db08], 0x2083); // 0042CFB1..0042CFC3
    clamp_linear(0x0DE1);                                              // 0042CFC9..0042D011
    gl_frame_buffer_modification_pixel_shaders_004299c0(4, 0, 0, 0, 0); // 0042D013..0042D022
    glBegin_0073d2d0(7);                                               // 0042D025..0042D027
    for (I v = 0; v < 4; ++v) {                                        // esi 0042D02D / 0042D0D8..0042D0DC
        glColor4f_0073d224(1.0f, 1.0f, 1.0f, 1.0f);                    // 0042D030..0042D044
        const float t = (v == 0 || v == 3) ? 1.0f : 0.0f;              // 0042D04A..0042D05B -> L1C
        const float s = (v == 0 || v == 1) ? 0.0f : 1.0f;              // 0042D063..0042D074 -> L18
        glTexCoord2f_0073d2cc(s, t);                                   // 0042D07C..0042D086
        const float y = (v == 0 || v == 3) ? static_cast<float>(static_cast<I>(*fb.height_0078d1d8)) : 0.0f; // 0042D08C..0042D0A5 -> L14
        const float x = (v == 0 || v == 1) ? 0.0f : static_cast<float>(static_cast<I>(*fb.width_0078d1d4)); // 0042D0A9..0042D0BE -> L10
        glVertex3f_0073d2c8(x, y, 0.0f);                               // 0042D0C6..0042D0D2
    }
    glEnd_0073d2c4();                                                  // 0042D0E2
    glMatrixMode_0073d228(0x1701);                                     // 0042D0E8..0042D0ED
    glPopMatrix_0073d2fc();                                            // 0042D0EF..0042D0F5
    glMatrixMode_0073d228(0x1700);                                     // 0042D0F7..0042D0FC
    glPopMatrix_0073d2fc();                                            // 0042D0FE
    glBindTexture_0073d240(0x0DE1, 0);                                 // 0042D100..0042D107
    procs::wglReleaseTexImageARB_007bb6d0(fb.pbuffers_007a6824[fb_bloom_count_0078db08], 0x2083); // 0042D109..0042D11B
    gl_frame_buffer_modification_pixel_shaders_004299c0(2, 0, 0, 0, 0); // 0042D121..0042D130
}                                                                      // 0042D13A ret

} // extern "C"
