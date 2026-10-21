// fb_emitter_motion_blur.cpp - lane-3d-fb (framebuffer_bloom area): 00490820 PartEmitter::RenderMotionBlurred.
// Evidence: dumpbin /disasm swkotor.exe 00490820..00491A42 (lane-local d/00490820.txt); addresses cited per statement.
// First menu frame: NOT executed (PartEmitter::RenderFunc 0049CA3B selects it only for render mode [00798B8C] = 9
// "Motion_Blur"; the menu models contain only "Normal" / "Billboard_to_World_Z", gates.json).
//
// Locals are named after their original frame offset ([esp+X] with no pending pushes, after the four register
// pushes at 00490840..00490847). D30-FINAL: values the original keeps on the x87 stack are double, float only at
// the original `fst/fstp dword` points; _ftol2 goes through the original-ABI boundary unresolved_ftol2_006fae8c.
// EH: the original registers unwind state 0 (handler 0071793E -> Unwind@00717930 clears the guard) only around the
// static-pool `operator new` (00490857..00490870). live_operator_new returns null instead of throwing and no other
// callee throws, so the cleanup is cited, not emitted (D30-FINAL).
//
// Reused declarations: lane-3d-emitter emitter.h (PartEmitter family, emitter cells, core/animation lowers,
// typed tables), lane-3d-render scene3d.h (cells, Material lowers, M41 allocator), lane-texture-epoch
// texture_controller.hpp (sine/cosine LUT, exact copies below), fb_bloom.h (00827FB0 storage owner).
#include "056e5913e7-fb_bloom.h"
#include "4ac9e30747-emitter.h"
#include <cstring>

extern "C" {
// lane-texture-epoch include/texture_controller.hpp (exact declarations; ST0 float result)
float __cdecl cosine_lut_004aadf0(float);
float __cdecl sine_lut_004aae30(float);

// ---------------------------------------------------------------- cells owned by this unit
// 00798BA8: .data E8 03 00 00 (1000); the only writer is 00490A47 (this function), the only reader 00490A2C.
volatile kotor_scene3d::I emitter_motion_blur_pool_capacity_00798ba8 = 1000;
// 008304A8 / 008304AC: function-static vertex pool of 00490820 and its static-init guard (bit 0). BSS.
// Writers: 0049086B / 00490A54 (pool), 00490851 (guard; Unwind@00717930 is the only other writer).
void* volatile emitter_motion_blur_pool_008304a8 = nullptr;
volatile kotor_scene3d::U emitter_motion_blur_pool_guard_008304ac = 0;
}

namespace {
using namespace kotor_emitter;
using kotor_scene3d::cells;
using kotor_renderer_init::glEnable_0073d390;
using kotor_renderer_init::glDisable_0073d22c;

// .rdata constants read through memory operands (pinned image values).
constexpr U kGlSrcAlpha_0073f270 = 0x302;
constexpr U kGlOneMinusSrcAlpha_0073f274 = 0x303;
constexpr U kGlOne_0073f278 = 1;
constexpr double kZero_0073d700 = 0.0;
constexpr double kOne_0073d764 = 1.0;
constexpr double kHalf_0073e9ac = 0.5;
constexpr double k255_0073ede4 = 255.0;
constexpr double kEpsilon_00743308 = 1e-05;   // qword F1 68 E3 88 B5 F8 E4 3E
constexpr U kOneBits = 0x3F800000u;
constexpr U kHundredthBits = 0x3C23D70Au;     // 0.01f immediate at 00490B93
constexpr U k255Bits = 0x437F0000u;           // 255.0f immediate at 00490D4B (integer compare)

inline double wf(void* object, U offset) { return W(field<float>(object, offset)); }
inline float bitsf(void* object, U offset) { return bits_to_float(field<U>(object, offset)); }   // raw DWORD copy

// Clamp idiom (004910BF..0049118E / 004911AD..00491284):
// `fcomp 0 / test ah,5 / jp` (store 0 iff v < 0), else `fcomp 1 / test ah,41h / jne` (store 1.0 iff v > 1).
inline void clamp01(float& v) {
    if (W(v) < kZero_0073d700) v = 0.0f;
    else if (W(v) > kOne_0073d764) v = bits_to_float(kOneBits);
}

// _ftol2 006FAE8C through the existing original-ABI boundary (ST0 in, EDX:EAX out, ST0 popped); EAX kept.
// Same mechanism as lane-renderer-bodies framebuffer.cpp (D21 inline asm).
I ftol2_006fae8c(double value) {
    I result_low;
    __asm {
        fld value
        call unresolved_ftol2_006fae8c
        mov result_low, eax
    }
    return result_low;
}

inline void* particle_at(void* emitter, I index) {
    return static_cast<void* const*>(field<void*>(emitter, 0x1C8))[index];          // [[ebp+1C8]+eax*4-4]
}
} // namespace

extern "C" void __fastcall part_emitter_render_motion_blurred_00490820(void* emitter, void*) {
    // ---- 0049082E..00490870: function-static vertex pool, guard bit 0 of 008304AC; new(5DC0h) = 1000 * 18h.
    if ((emitter_motion_blur_pool_guard_008304ac & 1u) == 0) {                          // 0049082E / 00490845 / 0049084F
        emitter_motion_blur_pool_guard_008304ac = emitter_motion_blur_pool_guard_008304ac | 1u; // 00490851
        emitter_motion_blur_pool_008304a8 = live_operator_new(0x5DC0);                // 00490857..0049086B (EH state 0 -> -1)
    }
    // ---- 0049087B..0049089D: owner Gob fade object; fade < 1.0 -> nothing drawn (jnp after test ah,5).
    {
        void* const gob = field<void*>(emitter, 0x40);                                // 0049087B
        if (gob != nullptr) {
            void* const fade = field<void*>(gob, 0x170);                                // 00490882
            if (fade != nullptr && wf(fade, 0x94) < kOne_0073d764) return;            // 0049088C..0049089D -> 00491A2A
        }
    }
    // ---- 004908A3..00490A0D: camera orientation [+88..+94] -> view rows (locals 64/68/6C, 40/44/48).
    void* const camera = *cells.current_camera_007bb4e8;                               // 004908A3
    Vector vc8 = {0.0f, 0.0f, 0.0f};                                                   // 004908A8..004908BE ([C8],[CC],[D0])
    Quaternion q9c = {bits_to_float(kOneBits), 0.0f, 0.0f, 0.0f};                     // 004908C9..004908EA ([9C]=1.0, [A0..A8]=0)
    Quaternion qd4 = {bits_to_float(kOneBits), 0.0f, 0.0f, 0.0f};                     // 004908F5..00490916 ([D4]=1.0, [D8..E0]=0)
    U fog_was_enabled_bc = 0;                                                          // 00490932
    float scale_80 = bits_to_float(kOneBits);                                          // 0049093B
    const double ca = wf(camera, 0x88), cb = wf(camera, 0x8C), cc = wf(camera, 0x90), cd = wf(camera, 0x94);
    const float r64 = S(kOne_0073d764 - (cc * cc + cd * cd) * 2.0);                   // 00490921..00490954
    const float r68 = S((ca * cd + cc * cb) * 2.0);                                    // 0049095C..00490978
    const float r6c = S((cd * cb - ca * cc) * 2.0);                                    // 0049097C..00490998
    const float r40 = S((cc * cb - ca * cd) * 2.0);                                    // 0049099C..004909B8
    const float r44 = S(kOne_0073d764 - (cd * cd + cb * cb) * 2.0);                   // 004909BC..004909DA
    void* const node = field<void*>(emitter, 0x04);                                    // 0049092D esi = [ebp+4]
    float delta_10 = bitsf(emitter, 0x68);                                             // 004909FA..004909FD raw DWORD
    const float r48 = S((cc * cd + ca * cb) * 2.0);                                    // 004909E2..00490A0D
    if (field<U>(emitter, 0x1CC) == 0) return;                                         // 00490A01..00490A11 -> 00491A2A

    // ---- 00490A17..00490A21: debug bounding sphere.
    if (emitter_debug_spheres_00830498 != 0)                                           // 00490A17
        part_emitter_render_bounding_sphere_0048ef10(emitter, nullptr);                // 00490A1F..00490A21
    // ---- 00490A26..00490A54: grow the static pool when 4*count exceeds the capacity (unsigned, jbe).
    {
        const U count = field<U>(emitter, 0x1CC);                                      // 00490A26
        const U capacity = static_cast<U>(emitter_motion_blur_pool_capacity_00798ba8); // 00490A2C
        if (count * 4u > capacity) {                                                   // 00490A32..00490A3B
            void* const old_pool = emitter_motion_blur_pool_008304a8;                  // 00490A3D
            emitter_motion_blur_pool_capacity_00798ba8 = static_cast<I>(count * 8u);   // 00490A43..00490A47
            // 00490A4C call ResizeEmitterPool 0048DDA0: register ABI (EAX = new count 00490A43, ECX = old count
            // 00490A2C, [esp+4] = old pool), not expressible as a C/C++ call; its body is reproduced in place:
            void* const new_pool = live_operator_new(count * 8u * 0x18u);             // 0048DDA7..0048DDB1
            std::memcpy(new_pool, old_pool, capacity * 0x18u);                        // 0048DDB6..0048DDCF rep movsd/movsb
            live_free(old_pool);                                                       // 0048DDCE..0048DDD1 (006FA390)
            emitter_motion_blur_pool_008304a8 = new_pool;                              // 0048DDDC / 00490A54
        }
    }
    // ---- 00490A59..00490A70: owner Gob scale (raw DWORD compare with 1.0f).
    {
        void* const gob = field<void*>(emitter, 0x40);                                // 00490A59
        if (gob != nullptr && field<U>(gob, 0x1B4) != kOneBits)                       // 00490A60..00490A6C
            scale_80 = bitsf(gob, 0x1B4);                                              // 00490A6E..00490A70
    }
    // ---- 00490A77..00490A98: reciprocal of [ebp+68] when > 0 (test ah,41h / jne), else 0.
    if (W(delta_10) > kZero_0073d700) delta_10 = S(kOne_0073d764 / W(delta_10));      // 00490A77..00490A92
    else delta_10 = 0.0f;                                                              // 00490A98
    material_bind_texture0_0047adf0(field<void*>(emitter, 0x1E0), nullptr);          // 00490AA0..00490AA6
    material_set_blending_mode_0047af70(field<void*>(emitter, 0x1E0), nullptr, 0);    // 00490AAB..00490AB2 (push ebx = 0)
    // ---- 00490AB7..00490B53: camera forward column [C8..D0] and camera quaternion copy [D4..E0].
    {
        void* const cam = *cells.current_camera_007bb4e8;                              // 00490AB7
        const double a = wf(cam, 0x88), b = wf(cam, 0x8C), c = wf(cam, 0x90), d = wf(cam, 0x94);
        const float l4c = S((a * c + d * b) * 2.0);                                    // 00490ABC..00490AD4
        const float l50 = S((d * c - a * b) * 2.0);                                    // 00490ADC..00490AEB
        const float l54 = S(kOne_0073d764 - (c * c + b * b) * 2.0);                   // 00490AEF..00490B19
        vc8.x = l4c; vc8.y = l50; vc8.z = l54;                                         // 00490AD8/00490AF9, 00490AF2/00490B02, 00490B1D/00490B21
        qd4.w = bitsf(cam, 0x88); qd4.x = bitsf(cam, 0x8C);                            // 00490B2A..00490B38 raw DWORD copies
        qd4.y = bitsf(cam, 0x90); qd4.z = bitsf(cam, 0x94);                            // 00490B3F..00490B53
    }
    // ---- 00490B42..00490B93: per-particle phase scale [3C].
    float scale_3c;
    if (static_cast<U>(static_cast<I>(field<std::int16_t>(emitter, 0x1F4))) == emitter_render_mode_00798b80) // 00490B42..00490B60 movsx
        scale_3c = S(kOne_0073d764 / static_cast<double>(field<I>(emitter, 0x1CC)));  // 00490B62..00490B6E (fild; fdivr)
    else if (wf(emitter, 0x78) > kZero_0073d700)                                       // 00490B74..00490B82 (test ah,41h / jne)
        scale_3c = S(kOne_0073d764 / wf(emitter, 0x78));                               // 00490B84..00490B8D
    else
        scale_3c = bits_to_float(kHundredthBits);                                      // 00490B93
    // ---- 00490B9B..00490BBB: texture grid (columns [94], rows [2C]); 0 -> 1.
    I columns_94 = field<I>(node, 0x64);                                               // 00490B9B..00490BA0
    if (columns_94 == 0) columns_94 = 1;                                               // 00490BA7..00490BA9
    I rows_2c = field<I>(node, 0x68);                                                  // 00490BB0..00490BB5
    if (rows_2c == 0) rows_2c = 1;                                                     // 00490BB9..00490BBB
    // ---- 00490BBF..00490C57: GL state.
    kotor_renderer_init::glDepthMask_0073d28c(0);                                      // 00490BBF..00490BC0
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                                   // 00490BC6..00490BD3
    glEnable_0073d390(*cells.gl_blend_0078d448);                                       // 00490BD5..00490BE1
    glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                                 // 00490BE3..00490BEA
    glDisable_0073d22c(*cells.gl_cull_face_0078d454);                                  // 00490BEC..00490BF3
    {
        const U blend = field<U>(emitter, 0x4C);                                       // 00490BF5
        if (blend == 0) {                                                              // 00490BF8 / 00490C00
            kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274); // 00490C02..00490C0F
        } else if (blend == 1) {                                                       // 00490C13..00490C16
            glDisable_0073d22c(*cells.gl_blend_0078d448);                              // 00490C18..00490C1E
            glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                          // 00490C20..00490C27
        } else {
            kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOne_0073f278); // 00490C2B..00490C38
            fog_was_enabled_bc = kotor_renderer_init::glIsEnabled_0073d37c(*cells.gl_fog_0078d450); // 00490C3A..00490C50 movzx
            glDisable_0073d22c(*cells.gl_fog_0078d450);                                // 00490C4A..00490C57
        }
    }
    part_emitter_update_particle_frame_0048e460(emitter, nullptr, delta_10);         // 00490C59..00490C60 (raw DWORD pushed)
    // ---- 00490C65..00490CC4: texture-cell sizes, statistics, frame scale, owner alpha.
    const double inv_columns = kOne_0073d764 / static_cast<double>(columns_94);       // 00490C65..00490C78 (fild; fdivr)
    emitter_particles_drawn_00830474 = static_cast<I>(static_cast<U>(emitter_particles_drawn_00830474)
        + field<U>(emitter, 0x1CC));                                                   // 00490C6C..00490C82
    const float cell_u_84 = S(inv_columns);                                            // 00490C88
    const float cell_v_c4 = S(kOne_0073d764 / static_cast<double>(rows_2c));         // 00490C8F..00490C99
    const float frame_scale_c0 = S(part_emitter_frame_scale_0048dc90(emitter, nullptr)); // 00490CA0..00490CA5 (ST0)
    float owner_alpha_98 = bits_to_float(kOneBits);                                    // 00490CB1
    {
        void* const gob = field<void*>(emitter, 0x40);                                // 00490CAC
        if (gob != nullptr) owner_alpha_98 = bitsf(gob, 0xD8);                         // 00490CBE..00490CC4
    }

    // ---- 00490CCB..004919B7: one quad (4 vertices x 18h) per particle, last particle first.
    const U count = field<U>(emitter, 0x1CC);                                          // 00490CCB
    if (count != 0) {                                                                  // 00490CD1..00490CD3
        U quad_7c = count * 0x60u;                                                     // 00490CD9..00490CDF (edi and [7C])
        I remaining_2c = static_cast<I>(count);                                        // eax at the first 00490CF0
        do {
            void* const particle = particle_at(emitter, remaining_2c - 1);             // 00490CF3 / 00490D03
            remaining_2c = remaining_2c - 1;                                           // 00490D07..00490D08
            quad_7c = quad_7c - 0x60u;                                                 // 00490D0E..00490D17 (edx/edi -= 60h)
            float t_10, alpha_14, size_y_18, size_x_1c, red_20, green_24, blue_28;
            if (wf(emitter, 0x78) >= kZero_0073d700) {                                 // 00490CF0..00490D1B (test ah,1 / jne 0049119B)
                // 00490D21..00490D47: life fraction
                double t;
                if (static_cast<U>(static_cast<I>(field<std::int16_t>(emitter, 0x1F4))) == emitter_render_mode_00798b80) // 00490D21..00490D2E
                    t = static_cast<double>(remaining_2c) * W(scale_3c);               // 00490D30..00490D34
                else
                    t = W(scale_3c) * wf(particle, 0x70);                              // 00490D3A..00490D3E
                t_10 = S(t);                                                           // 00490D47
                if (field<U>(emitter, 0xDC) == k255Bits) {                             // 00490D41..00490D50 (two-key: start -> end)
                    Vector out_1c0;
                    const Vector* const rgb = vector_interpolate_0046eea0(&out_1c0, &field<Vector>(emitter, 0x188),
                        &field<Vector>(emitter, 0x17C), t_10);                         // 00490D56..00490D71
                    const double u = kOne_0073d764 - W(t_10);                          // 00490D76..00490D7C
                    blue_28 = rgb->z;                                                  // 00490D8C / 00490D91
                    red_20 = rgb->x; green_24 = rgb->y;                                // 00490D80 / 00490D86 / 00490D9B / 00490D9F
                    alpha_14 = S(W(t_10) * wf(emitter, 0x50) + u * wf(emitter, 0x54)); // 00490D82..00490DA5
                    size_x_1c = S((W(t_10) * wf(emitter, 0x94) + u * wf(emitter, 0x90)) * kHalf_0073e9ac); // 00490DA9..00490DC3
                    if (wf(emitter, 0x98) == kZero_0073d700 && wf(emitter, 0x9C) == kZero_0073d700) // 00490DC7..00490DEB (test ah,44h / jp)
                        size_y_18 = size_x_1c;                                         // 00490DED..00490DF3
                    else
                        size_y_18 = S((W(t_10) * wf(emitter, 0x9C) + u * wf(emitter, 0x98)) * kHalf_0073e9ac); // 00490DFC..00490E16
                } else if (W(t_10) >= wf(emitter, 0xE4)) {                             // 00490E1F..00490E2E (past the end key)
                    red_20 = bitsf(emitter, 0x17C); green_24 = bitsf(emitter, 0x180); blue_28 = bitsf(emitter, 0x184); // 00490E36..00490E64
                    alpha_14 = bitsf(emitter, 0x50);                                   // 00490E4E..00490E51
                    size_x_1c = bitsf(emitter, 0x94);                                  // 00490E5E / 00490E68
                    if (wf(emitter, 0x9C) == kZero_0073d700) size_y_18 = size_x_1c;    // 00490E30 / 00490E5B..00490E6C -> 00491043
                    else size_y_18 = bitsf(emitter, 0x9C);                             // 00490E72 / 004910BB
                } else if (W(t_10) >= wf(emitter, 0xE0)) {                             // 00490E7D..00490E90 (middle -> end)
                    t_10 = S((W(t_10) - wf(emitter, 0xE0)) / (wf(emitter, 0xE4) - wf(emitter, 0xE0))); // 00490E96..00490EB6
                    Vector out_1cc;
                    const Vector* const rgb = vector_interpolate_0046eea0(&out_1cc, &field<Vector>(emitter, 0x11C),
                        &field<Vector>(emitter, 0x17C), t_10);                         // 00490EBA..00490EC9
                    const double u = kOne_0073d764 - W(t_10);                          // 00490ECE..00490ED4
                    red_20 = rgb->x;                                                   // 00490ED8 / 00490EDD
                    alpha_14 = S(u * wf(emitter, 0xD8) + W(t_10) * wf(emitter, 0x50)); // 00490EE1..00490F00
                    green_24 = rgb->y; blue_28 = rgb->z;                               // 00490EDA/00490EF3, 00490EE3/00490EFA
                    size_x_1c = S((u * wf(emitter, 0xE8) + W(t_10) * wf(emitter, 0x94)) * kHalf_0073e9ac); // 00490F04..00490F1E
                    if (wf(emitter, 0xEC) == kZero_0073d700 && wf(emitter, 0x9C) == kZero_0073d700) // 00490F22..00490F46
                        size_y_18 = size_x_1c;                                         // 00490F48..00490F4E
                    else
                        size_y_18 = S((W(t_10) * wf(emitter, 0x9C) + u * wf(emitter, 0xEC)) * kHalf_0073e9ac); // 00490F57..00490F71
                } else if (W(t_10) >= wf(emitter, 0xDC)) {                             // 00490F7A..00490F85 (start -> middle)
                    t_10 = S((W(t_10) - wf(emitter, 0xDC)) / (wf(emitter, 0xE0) - wf(emitter, 0xDC))); // 00490F8B..00490FAF
                    Vector out_1b4;
                    const Vector* const rgb = vector_interpolate_0046eea0(&out_1b4, &field<Vector>(emitter, 0x188),
                        &field<Vector>(emitter, 0x11C), t_10);                         // 00490FB3..00490FC2
                    const double u = kOne_0073d764 - W(t_10);                          // 00490FC7..00490FCD
                    red_20 = rgb->x;                                                   // 00490FD1 / 00490FD6
                    alpha_14 = S(u * wf(emitter, 0x54) + W(t_10) * wf(emitter, 0xD8)); // 00490FDA..00490FF9
                    green_24 = rgb->y; blue_28 = rgb->z;                               // 00490FD3/00490FE9, 00490FDC/00490FF3
                    size_x_1c = S((u * wf(emitter, 0x90) + W(t_10) * wf(emitter, 0xE8)) * kHalf_0073e9ac); // 00490FFD..00491017
                    if (wf(emitter, 0x98) == kZero_0073d700 && wf(emitter, 0xEC) == kZero_0073d700) // 0049101B..0049103F
                        size_y_18 = size_x_1c;                                         // 00491041..00491047
                    else
                        size_y_18 = S((W(t_10) * wf(emitter, 0xEC) + u * wf(emitter, 0x98)) * kHalf_0073e9ac); // 0049104D..00491067
                } else {                                                               // 0049106D (before the start key)
                    red_20 = bitsf(emitter, 0x188); green_24 = bitsf(emitter, 0x18C); blue_28 = bitsf(emitter, 0x190); // 00491073..004910A1
                    size_x_1c = bitsf(emitter, 0x90);                                  // 0049108B..00491091
                    alpha_14 = bitsf(emitter, 0x54);                                   // 0049109E / 004910A5
                    if (wf(emitter, 0x98) == kZero_0073d700) size_y_18 = size_x_1c;    // 0049106D / 0049109B..004910A9 -> 004910B7
                    else size_y_18 = bitsf(emitter, 0x98);                             // 004910AB..004910B1
                }
                clamp01(red_20);                                                       // 004910BF..004910EB
                clamp01(green_24);                                                     // 004910F3..0049111F
                clamp01(blue_28);                                                      // 00491127..00491153
                clamp01(alpha_14);                                                     // 0049115B..0049118E
            } else {                                                                   // 0049119B (negative / NaN [ebp+78])
                red_20 = bitsf(emitter, 0x188); blue_28 = bitsf(emitter, 0x190);      // 0049119B..004911A9 / 004911BA
                green_24 = bitsf(emitter, 0x18C);                                      // 004911A3 / 004911BE
                alpha_14 = bitsf(emitter, 0x54);                                       // 004911B7 / 004911C2
                clamp01(red_20);                                                       // 004911AD..004911E8
                clamp01(green_24);                                                     // 004911F0..0049121C
                clamp01(blue_28);                                                      // 00491224..00491250
                clamp01(alpha_14);                                                     // 00491258..00491284
                size_x_1c = S(wf(emitter, 0x90) * kHalf_0073e9ac);                    // 0049128C..00491298
                if (wf(emitter, 0x98) == kZero_0073d700 && wf(emitter, 0x9C) == kZero_0073d700) // 0049129C..004912C0
                    size_y_18 = size_x_1c;                                             // 004912C2..004912C6
                else
                    size_y_18 = S(wf(emitter, 0x98) * kHalf_0073e9ac);                // 004912CC..004912D8
            }

            // ---- 004912DC..004913E4: particle spin quaternion about the camera forward axis (q9c = spin * camera).
            const float sine_10 = sine_lut_004aae30(S(wf(particle, 0x14) * kHalf_0073e9ac)); // 004912DC..004912F5
            Vector n_1a8;
            const Vector* const axis = normalize_004ab180(&n_1a8, &vc8);              // 004912EE..00491302
            const float s124 = S(W(sine_10) * W(axis->x));                             // 00491307..00491310
            const float s128 = S(W(sine_10) * W(axis->y));                             // 00491317..0049131E
            const float s34 = S(W(sine_10) * W(axis->z));                              // 00491325..0049132C
            const float cosine_fc = cosine_lut_004aadf0(S(wf(particle, 0x14) * kHalf_0073e9ac)); // 00491330..00491348
            q9c.w = cosine_fc; q9c.x = s124; q9c.y = s128; q9c.z = s34;                // 00491341..004913AF (via [FC..108])
            {
                Quaternion q1d8;
                const Quaternion* const spun = quaternion_multiply_004a9ac0(&q1d8, &q9c, &qd4); // 00491399..004913BF
                q9c = *spun;                                                           // 004913C4..004913E4 (result not read afterwards)
            }
            // ---- 004913EB..00491440: texture cell (row [34], column [B8]); tail position [88..90].
            const I frame_index = field<I>(particle, 0x3C);                            // 004913EB..004913EE
            const I row_34 = ftol2_006fae8c(static_cast<double>(frame_index) * W(cell_u_84)); // 004913F2..00491408
            const I column_b8 = static_cast<I>(static_cast<U>(frame_index)
                - static_cast<U>(row_34) * static_cast<U>(columns_94));                // 0049140C..00491414 / 00491440
            const float tail_88 = bitsf(particle, 0x60), tail_8c = bitsf(particle, 0x64), tail_90 = bitsf(particle, 0x68); // 00491416..00491436
            // ---- 00491405..00491498: particle-to-tail delta in view space.
            const double dx = wf(particle, 0x08) - W(tail_88);                         // 00491405 / 0049142F
            const double dy = wf(particle, 0x0C) - W(tail_8c);                         // 0049143D / 00491447
            const double dz = wf(particle, 0x10) - W(tail_90);                         // 0049144E / 00491451
            const float a_10 = S((W(r6c) * dz + dy * W(r68)) + dx * W(r64));          // 00491458..0049146E
            const double b = (dz * W(r48) + dy * W(r44)) + dx * W(r40);                // 00491472..00491484
            const float b_30 = S(b);                                                   // 00491486 fst (b stays wide)
            double length = std::sqrt(b * W(b_30) + W(a_10) * W(a_10));                // 0049148A..00491498
            // ---- 0049149A..00491651: right [AC..B4] and up [4C..54] axes of the streak quad.
            float right_ac, right_b0, right_b4, up_4c, up_50, up_54;
            if (!(length == kOne_0073d764) && length > kEpsilon_00743308) {           // 0049149A..004914B6
                const double inv = kOne_0073d764 / length;                             // 004914BC..004914C2
                const double A = W(a_10) * inv;                                        // 004914C4..004914C8
                const double B = inv * W(b_30);                                        // 004914CA..004914CC
                const double r40a = W(r40) * A;                                        // 004914D0..004914D4
                const double r44a = W(r44) * A;                                        // 004914D6..004914DA
                const float f180 = S(W(r48) * A);                                      // 004914DC..004914E2
                const double r64b = W(r64) * B;                                        // 004914E9..004914ED
                const float f14c = S(W(r68) * B);                                      // 004914EF..004914F5
                const float f150 = S(W(r6c) * B);                                      // 004914FC..00491502
                right_ac = S(r64b - r40a);                                             // 00491509..00491520 ([E4] -> [AC])
                right_b0 = S(W(f14c) - r44a);                                          // 00491519..00491539 ([E8] -> [B0])
                right_b4 = S(W(f150) - W(f180));                                       // 00491542..00491564 ([EC] -> [B4])
                const float ff0 = S(W(r40) * B);                                       // 00491557..0049156B
                const float ff4 = S(W(r44) * B);                                       // 00491572..00491578
                const float ff8 = S(W(r48) * B);                                       // 0049157F..00491585
                const float f160 = S(W(r64) * A);                                      // 0049158E..00491594
                const float f164 = S(W(r68) * A);                                      // 0049159B..004915A1
                const double r6ca = W(r6c) * A;                                        // 004915A8..004915B0
                up_4c = S(W(f160) + W(ff0));                                           // 004915B2..004915D5 ([10C] -> [4C])
                up_50 = S(W(f164) + W(ff4));                                           // 004915CE..00491603 ([110] -> [50])
                up_54 = S(r6ca + W(ff8));                                              // 004915E7..00491607 ([114] -> [54])
            } else {                                                                   // 0049160D
                right_b4 = r6c; right_ac = r64; right_b0 = r68;                        // 0049160D..0049163E
                up_54 = r48; up_4c = r40; up_50 = r44;                                 // 00491626..0049164D
                if (length == kZero_0073d700) length = kOne_0073d764;                  // 00491611..00491655 (test ah,44h / jp)
            }
            // ---- 0049165B..004916F7: half extents.
            double stretch = (W(size_x_1c) / length);                                  // 0049165B fdivr
            stretch = stretch + stretch;                                               // 0049165F
            if (!(stretch < kOne_0073d764)) stretch = kOne_0073d764;                  // 00491661..00491670
            double width = W(size_x_1c);                                               // 00491683
            if (!(W(scale_80) == kOne_0073d764)) {                                     // 00491676..0049168C (test ah,44h / jnp)
                width = W(size_x_1c) * W(scale_80);                                    // 0049168E
                size_y_18 = S(W(size_y_18) * W(scale_80));                             // 00491695..004916A0
            }
            width = width + width;                                                     // 004916A4
            const float side_58 = S(W(right_ac) * width);                              // 004916A6..004916AF
            const float side_5c = S(W(right_b0) * width);                              // 004916B3..004916BC
            const float side_60 = S(W(right_b4) * width);                              // 004916C0..004916C9
            const double half_height = W(size_y_18) * kHalf_0073e9ac;                  // 004916CF..004916D3
            const float up_70 = S(W(up_4c) * half_height);                             // 004916D9..004916DF
            const float up_74 = S(W(up_50) * half_height);                             // 004916E3..004916E9
            const float up_78 = S(W(up_54) * half_height);                             // 004916ED..004916F3
            // ---- 004916F9..004917C2: packed colours (A<<24|B<<16|G<<8|R); tail end alpha also scaled by stretch.
            const I blue_38 = ftol2_006fae8c(W(blue_28) * k255_0073ede4);              // 004916F9..00491714
            const I green_30 = ftol2_006fae8c(W(green_24) * k255_0073ede4);            // 00491708..00491727
            const I red_10 = ftol2_006fae8c(W(red_20) * k255_0073ede4);                // 0049171D..0049173B
            const I alpha_head = ftol2_006fae8c(((W(owner_alpha_98) * W(alpha_14)) * W(frame_scale_c0)) * k255_0073ede4); // 00491730..0049174C
            unsigned char* const quad = static_cast<unsigned char*>(emitter_motion_blur_pool_008304a8) + quad_7c; // 00491780 esi; [7C]/edi
            {
                U packed = static_cast<U>(alpha_head) << 8;                            // 00491762
                packed = (packed | static_cast<U>(blue_38)) << 8;                     // 00491769..0049176F
                packed = (packed | static_cast<U>(green_30)) << 8;                    // 00491779..0049177B
                packed = packed | static_cast<U>(red_10);                             // 0049177E
                field<U>(quad, 0x0C) = packed;                                         // 0049178C
                field<U>(quad, 0x24) = packed;                                         // 00491790
            }
            {
                const I alpha_tail = ftol2_006fae8c((((W(owner_alpha_98) * stretch) * W(alpha_14))
                    * W(frame_scale_c0)) * k255_0073ede4);                             // 00491751..00491794
                U packed = static_cast<U>(alpha_tail) << 8;                            // 004917A3
                packed = (packed | static_cast<U>(blue_38)) << 8;                     // 004917A6..004917AC
                packed = (packed | static_cast<U>(green_30)) << 8;                    // 004917AF..004917B5
                packed = packed | static_cast<U>(red_10);                             // 004917B8
                field<U>(quad, 0x3C) = packed;                                         // 004917BE
                field<U>(quad, 0x54) = packed;                                         // 004917C2
            }
            // ---- 0049179F..00491844: vertex 0 = particle + side + up; uv (column+1, row).
            {
                const double x = (W(side_58) + wf(particle, 0x08)) + W(up_70);         // 0049179F / 004917C6 / 004917F1
                const float f158 = S(W(side_5c) + wf(particle, 0x0C));                 // 004917D0..004917DC
                const float f15c = S(W(side_60) + wf(particle, 0x10));                 // 004917E3..004917EA
                const double y = W(f158) + W(up_74);                                   // 004917F5..004917FC
                const float f18c = S(W(f15c) + W(up_78));                              // 00491800..0049180B
                field<float>(quad, 0x00) = S(x);                                       // 00491814
                field<float>(quad, 0x04) = S(y);                                       // 00491817
                field<float>(quad, 0x08) = f18c;                                       // 0049181B..00491822
            }
            const I column_plus_one = static_cast<I>(static_cast<U>(column_b8) + 1u);  // 004917C9..004917D8 ([38])
            const double u0 = static_cast<double>(column_plus_one) * W(cell_u_84);    // 00491826..0049182A
            field<float>(quad, 0x10) = S(u0);                                          // 00491831 fst
            const double v0 = kOne_0073d764 - static_cast<double>(row_34) * W(cell_v_c4); // 00491835..00491840
            field<float>(quad, 0x14) = S(v0);                                          // 00491846 fst
            // ---- 0049184A..004918B7: vertex 1 = particle - side + up; uv (column, row).
            {
                const double x = (wf(particle, 0x08) - W(side_58)) + W(up_70);        // 0049184A..0049184D / 0049186D
                const float f134 = S(wf(particle, 0x0C) - W(side_5c));                 // 00491851..00491858
                const float f138 = S(wf(particle, 0x10) - W(side_60));                 // 0049185F..00491866
                const double y = W(f134) + W(up_74);                                   // 00491871..00491878
                const float f1a4 = S(W(f138) + W(up_78));                              // 0049187C..00491887
                field<float>(quad, 0x18) = S(x);                                       // 00491890
                field<float>(quad, 0x1C) = S(y);                                       // 00491894
                field<float>(quad, 0x20) = f1a4;                                       // 00491898..0049189F
            }
            const double u1 = static_cast<double>(column_b8) * W(cell_u_84);          // 004918A3..004918AA
            field<float>(quad, 0x28) = S(u1);                                          // 004918B1 fst
            field<float>(quad, 0x2C) = S(v0);                                          // 004918B5..004918B7
            // ---- 004918BB..00491944: vertex 2 = tail - side - up; uv (column, row+1).
            {
                const double x = (W(tail_88) - W(side_58)) - W(up_70);                 // 004918BB..004918C2 / 004918EA
                const float f140 = S(W(tail_8c) - W(side_5c));                         // 004918C6..004918D1
                const float f144 = S(W(tail_90) - W(side_60));                         // 004918D8..004918E3
                const double y = W(f140) - W(up_74);                                   // 004918EE..004918F5
                const float f198 = S(W(f144) - W(up_78));                              // 004918F9..00491904
                field<float>(quad, 0x30) = S(x);                                       // 0049190D
                field<float>(quad, 0x34) = S(y);                                       // 00491911
                field<float>(quad, 0x38) = f198;                                       // 00491915..00491920
            }
            field<float>(quad, 0x40) = S(u1);                                          // 00491929
            const I row_plus_one = static_cast<I>(static_cast<U>(row_34) + 1u);        // 0049191C..00491925 ([38])
            const double v1 = kOne_0073d764 - static_cast<double>(row_plus_one) * W(cell_v_c4); // 00491933..0049193E
            field<float>(quad, 0x44) = S(v1);                                          // 00491944 fst
            // ---- 00491948..004919B3: vertex 3 = tail + side - up; uv (column+1, row+1).
            {
                const double x = (W(side_58) + W(tail_88)) - W(up_70);                 // 00491948..0049194C / 00491977
                const float f11c = S(W(side_5c) + W(tail_8c));                         // 00491953..0049195E
                const float f120 = S(W(side_60) + W(tail_90));                         // 00491965..00491970
                const double y = W(f11c) - W(up_74);                                   // 0049197B..00491982
                const float f174 = S(W(f120) - W(up_78));                              // 00491986..00491991
                field<float>(quad, 0x48) = S(x);                                       // 0049199A
                field<float>(quad, 0x4C) = S(y);                                       // 0049199E
                field<float>(quad, 0x50) = f174;                                       // 004919A2..004919A9
            }
            field<float>(quad, 0x58) = S(u0);                                          // 004919AD..004919AF
            field<float>(quad, 0x5C) = S(v1);                                          // 004919B3
        } while (remaining_2c != 0);                                                   // 0049192D..00491931 / 004919B7
    }
    // ---- 004919BF..00491A28: submit and restore GL state.
    void* const pool = emitter_motion_blur_pool_008304a8;                              // 004919BF (or esi from 00491780)
    aurora_triangle_count_00827fb0 = static_cast<I>(static_cast<U>(aurora_triangle_count_00827fb0)
        + field<U>(emitter, 0x1CC));                                                   // 004919C5..004919CB
    kotor_cdecl_00431cc0(static_cast<float*>(pool), static_cast<I>(field<U>(emitter, 0x1CC) << 2)); // 004919D1..004919E1
    kotor_renderer_init::glDepthMask_0073d28c(1);                                      // 004919E4..004919E6
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                                    // 004919EC..004919F9
    kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274); // 004919FB..00491A08
    glEnable_0073d390(*cells.gl_cull_face_0078d454);                                   // 00491A0E..00491A15
    if (fog_was_enabled_bc != 0) glEnable_0073d390(*cells.gl_fog_0078d450);            // 00491A17..00491A28
}                                                                                      // 00491A2A..00491A42 ret
