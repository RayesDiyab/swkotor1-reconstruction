// render_standard.cpp - lane-3d-emitter (fork F1): PartEmitter::RenderStandard 0048F040 and its private helpers
// 0048DC90 (frame scale), 0048DD50 (next frame), 0048DDA0 ResizeEmitterPool, 0048E460 updateParticleFrame.
// Evidence: dumpbin /disasm of the pinned GOG swkotor.exe (d/0048F040.txt, d/0048DC90.txt, d/0048DD50.txt,
// d/0048DDA0.txt, d/0048E460.txt); instruction addresses cited per behaviour.
// D30-FINAL: W() = value kept on the x87 stack (double), S() = original `fstp dword` rounding point.
// x87 compare decoding (fcomp A,B; fnstsw ax): C0 = A<B or unordered, C3 = A==B or unordered, C2 = unordered.
//   test ah,5  + jp  -> taken unless "A<B ordered"          => !less()
//   test ah,1  + jne -> taken when C0                        => c0()
//   test ah,41 + jne -> taken when A<=B or unordered         => le_or_unordered()
//   test ah,41 + jp  -> taken when A>B or unordered          => gt_or_unordered()
//   test ah,41 + jnp -> taken when A<B or A==B (ordered)     => le_ordered()
//   test ah,44 + jnp -> taken when A==B (ordered)            => eq()
// Unwind: the static-pool initialisation (state 0, handler 0071791E) is cited, not emitted (operator new returns
// NULL; D30-FINAL unwind rule - no throwing callee in this unit).
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS merged into emitter.h (lane header).

namespace {
using namespace kotor_emitter;
using kotor_scene3d::cells;

inline bool less(double a, double b) { return a < b; }
inline bool c0(double a, double b) { return a < b || a != a || b != b; }
inline bool le_or_unordered(double a, double b) { return !(a > b); }
inline bool gt_or_unordered(double a, double b) { return !(a <= b); }
inline bool le_ordered(double a, double b) { return a <= b; }
inline bool eq(double a, double b) { return a == b; }

inline float fl(void* o, U off) { return field<float>(o, off); }
inline U raw(void* o, U off) { return field<U>(o, off); }

// .rdata constants read through memory operands (values from the pinned image).
constexpr double kZero_0073d700 = 0.0;          // 00000000h
constexpr double kOne_0073d764 = 1.0;           // 3F800000h
constexpr double kHalf_0073e9ac = 0.5;          // 3F000000h
constexpr double k255_0073ede4 = 255.0;         // 437F0000h
constexpr double kQuarter_0073edf0 = 0.25;      // 3E800000h
constexpr U kSrcAlpha_0073f270 = 0x302;         // GL_SRC_ALPHA
constexpr U kOneMinusSrcAlpha_0073f274 = 0x303; // GL_ONE_MINUS_SRC_ALPHA
constexpr U kOne_0073f278 = 1;                  // GL_ONE
constexpr U kFill_0073f2e8 = 0x1B02;            // GL_FILL
constexpr U kFrontAndBack_0073f2f4 = 0x408;     // GL_FRONT_AND_BACK

// Clamp sequence 0048FA80..0048FB57 (and 0048FB6E..0048FC45): below 0 -> 0; above 1 -> 1.
inline void clamp01(float& v) {
    if (less(W(v), kZero_0073d700)) v = 0.0f;                      // test ah,5 / jp (not taken) -> mov 0
    else if (!le_or_unordered(W(v), kOne_0073d764)) v = 1.0f;      // test ah,41h / jne (not taken) -> mov 3F800000h
}
}

extern "C" {

// ---------------------------------------------------------------- 0048DDA0 ResizeEmitterPool
// Original register convention: ECX = old vertex count, EAX = new vertex count, [esp+4] = old buffer (caller
// cleans, plain RET), EAX = new buffer. REVIEW: exported here as __fastcall(ECX old, EDX new, stack old buffer)
// (EAX->EDX transport, callee cleans 4 bytes); C++ callers (0048F1FC here, 00490A4C framebuffer_bloom) use it so.
void* __fastcall resize_emitter_pool_0048dda0(I old_count, I new_count, void* old_buffer) {
    void* const buffer = live_operator_new(static_cast<U>(new_count) * 24u);           // 0048DDA7..0048DDB1 (lea*3, shl 3)
    const U bytes = static_cast<U>(old_count) * 24u;                                    // 0048DDB6..0048DDBC
    if (bytes != 0) std::memcpy(buffer, old_buffer, bytes);                             // 0048DDBE..0048DDCF rep movsd/movsb
    live_free(old_buffer);                                                              // 0048DDCE..0048DDD1
    return buffer;                                                                      // 0048DDDC
}

// ---------------------------------------------------------------- 0048DC90 (frame scale, ST0)
double __fastcall part_emitter_frame_scale_0048dc90(void* emitter, void*) {
    void* source = field<void*>(emitter, 0x1EC);                                        // 0048DC90
    if (source == nullptr) {                                                            // 0048DC9C
        void* const gob = field<void*>(emitter, 0x40);                                  // 0048DCA0
        if (gob == nullptr) return kOne_0073d764;                                       // 0048DCA3..0048DCA5
        source = field<void*>(gob, 0x170);                                              // 0048DCA7
        if (source == nullptr) return kOne_0073d764;                                    // 0048DCAD..0048DCB5
    }
    const double age = W(fl(emitter, 0x16C)) - W(fl(source, 0x30));                     // 0048DCB7..0048DCBD
    if (le_or_unordered(age, W(bits_to_float(emitter_frame_threshold_00798b94))))       // 0048DCC0..0048DCCB (5.0f)
        return kOne_0073d764;
    return W(fl(source, 0x50));                                                         // 0048DCCD..0048DCCF
}

// ---------------------------------------------------------------- 0048DD50 (next frame, RET 4)
I __fastcall part_emitter_next_frame_0048dd50(void* emitter, void*, I frame) {
    if (eq(W(fl(emitter, 0x70)), W(fl(emitter, 0x6C)))) return -1;                     // 0048DD50..0048DD5B -> 0048DD99
    if (less(W(fl(emitter, 0x6C)), W(fl(emitter, 0x70)))) {                             // 0048DD5D..0048DD6C
        frame = static_cast<I>(static_cast<U>(frame) + 1u);                             // 0048DD6E
        if (gt_or_unordered(static_cast<double>(frame), W(fl(emitter, 0x70)))) return -1; // 0048DD73..0048DD7F
        return frame;                                                                   // 0048DD81
    }
    frame = static_cast<I>(static_cast<U>(frame) - 1u);                                 // 0048DD86
    if (!c0(static_cast<double>(frame), W(fl(emitter, 0x70)))) return frame;            // 0048DD8B..0048DD97
    return -1;                                                                          // 0048DD99
}

// ---------------------------------------------------------------- 0048E460 PartEmitter::updateParticleFrame (RET 4)
void __fastcall part_emitter_update_particle_frame_0048e460(void* emitter, void*, float delta) {
    const float range = S(W(fl(emitter, 0x6C)) - W(fl(emitter, 0x70)));                // 0048E46D..0048E47B
    I span = fistp_nearest(W(range));                                                   // 0048E488..0048E492 frndint/fistp
    if (span < 0) span = static_cast<I>(0u - static_cast<U>(span));                     // 0048E498..0048E49C
    span = static_cast<I>(static_cast<U>(span) + 1u);                                   // 0048E4A2
    void* const node = field<void*>(emitter, 0x4);                                      // 0048E4A7
    const U flags = field<U>(node, 0x12C);                                              // 0048E4AA
    const bool random_order = ((flags >> 12) & 1u) != 0;                                // 0048E4B0..0048E4BA ([esp+13h])
    const bool random = ((flags >> 5) & 1u) != 0;                                       // 0048E4BF..0048E4C6 (bl)
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                                  // 0048E4C8..0048E4D2 / 0048E6E0..0048E6E9
        void* const p = as_object(field<U>(as_object(field<U>(emitter, 0x1C8)), static_cast<U>(i) * 4u)); // 0048E4E2..0048E4E8
        float age = S(W(fl(p, 0x70)) - W(random ? fl(p, 0x40) : fl(p, 0x44)));         // 0048E4EB..0048E4F8
        if (eq(W(delta), kZero_0073d700)) continue;                                     // 0048E4FC..0048E50A
        if (c0(W(age), W(delta))) continue;                                             // 0048E510..0048E51C
        const I last = fistp_nearest(W(fl(emitter, 0x70)));                             // 0048E522..0048E53B ([esp+20h])
        const I first = fistp_nearest(W(fl(emitter, 0x6C)));                            // 0048E541..0048E55A ([esp+24h])
        if (eq(W(fl(emitter, 0x70)), W(fl(emitter, 0x6C)))) continue;                   // 0048E560..0048E56B
        if (random_order) {
            const I r = crt_rand_006fc447();                                            // 0048E579
            field<I>(p, 0x3C) = r % span;                                               // 0048E57E..0048E583 cdq/idiv
        } else {
            // 0048E5B6..0048E5C6: the fcomp of [+6C] vs [+70] precedes the fdiv; its C0/C2/C3 are read by the
            // fnstsw after FDIV (architecturally "undefined" after FDIV, preserved by x87 implementations).
            const bool ascending = less(W(fl(emitter, 0x6C)), W(fl(emitter, 0x70)));
            const double ratio = W(age) / W(delta);                                     // 0048E5BF..0048E5C3
            if (ascending) {
                age = S(-std::floor(ratio));                                            // 0048E5D4..0048E5DF floor, fchs
                const I n = fistp_nearest(W(age));                                      // 0048E5EA..0048E5F4
                field<I>(p, 0x3C) = static_cast<I>(static_cast<U>(field<I>(p, 0x3C)) + static_cast<U>(n)); // 0048E5F6..0048E601
                if (random) {
                    field<float>(p, 0x44) = S(static_cast<double>(n) * W(delta) + W(fl(p, 0x44))); // 0048E608..0048E612
                    if (less(static_cast<double>(field<I>(p, 0x3C)), W(fl(emitter, 0x6C)))) { // 0048E619..0048E624
                        do {
                            const U step = static_cast<U>(last) - static_cast<U>(first) + 1u; // 0048E632..0048E636
                            field<I>(p, 0x3C) = static_cast<I>(static_cast<U>(field<I>(p, 0x3C)) + step); // 0048E637
                        } while (less(static_cast<double>(field<I>(p, 0x3C)), W(fl(emitter, 0x6C)))); // 0048E63A..0048E649
                    }
                } else if (last > field<I>(p, 0x3C)) {                                  // 0048E650..0048E656
                    field<I>(p, 0x3C) = last;                                           // 0048E658..0048E65A
                }
            } else {
                age = S(std::floor(ratio));                                             // 0048E662..0048E667
                const I n = fistp_nearest(W(age));                                      // 0048E676..0048E680
                field<I>(p, 0x3C) = static_cast<I>(static_cast<U>(field<I>(p, 0x3C)) + static_cast<U>(n)); // 0048E682..0048E68D
                if (random) {
                    field<float>(p, 0x44) = S(static_cast<double>(n) * W(delta) + W(fl(p, 0x44))); // 0048E692..0048E6A3
                    while (field<I>(p, 0x3C) > first) {                                 // 0048E69A / 0048E6A6 / 0048E6B8..0048E6BB
                        const U step = static_cast<U>(last) - static_cast<U>(first) - 1u; // 0048E6B0..0048E6B4
                        field<I>(p, 0x3C) = static_cast<I>(static_cast<U>(field<I>(p, 0x3C)) + step); // 0048E6B5
                    }
                } else if (first < field<I>(p, 0x3C)) {                                 // 0048E6C2..0048E6C8
                    field<I>(p, 0x3C) = first;                                          // 0048E6CA..0048E6CC
                }
            }
        }
        if (!random) field<float>(p, 0x44) = S(static_cast<double>(field<I>(p, 0x3C)) * W(delta)); // 0048E586..0048E590
        if (c0(W(fl(emitter, 0x78)), kZero_0073d700)) {                                 // 0048E593..0048E5A5
            field<U>(p, 0x40) = 0; field<U>(p, 0x70) = 0;                               // 0048E6D4..0048E6D9
        } else {
            field<U>(p, 0x40) = field<U>(p, 0x70);                                      // 0048E5AB..0048E5AE
        }
    }
}

// ---------------------------------------------------------------- 0048F040 PartEmitter::RenderStandard
void __fastcall part_emitter_render_standard_0048f040(void* self, void*) {
    using namespace kotor_renderer_init;
    if ((emitter_standard_pool_guard_008304a4 & 1u) == 0) {                            // 0048F05B..0048F06C (byte test, bit 0)
        emitter_standard_pool_guard_008304a4 = emitter_standard_pool_guard_008304a4 | 1u; // 0048F06E
        emitter_standard_pool_008304a0 = live_operator_new(0x5DC0);                     // 0048F075..0048F08D (1000 vertices x 18h)
    }
    {
        void* const gob = field<void*>(self, 0x40);                                     // 0048F09D
        if (gob != nullptr) {
            void* const fade = field<void*>(gob, 0x170);                                // 0048F0A4
            if (fade != nullptr && less(W(fl(fade, 0x94)), kOne_0073d764)) return;      // 0048F0AE..0048F0BF -> 004906DE
        }
    }
    Vector dir{0.0f, 0.0f, 0.0f};                                                       // [esp+70h] 0048F0C5..0048F0D5
    Vector right{0.0f, 0.0f, 0.0f};                                                     // [esp+3Ch] 0048F0DD..0048F0ED
    void* const node = field<void*>(self, 0x4);                                         // 0048F0F5
    const bool splat = ((field<U>(node, 0x12C) >> 9) & 1u) != 0;                        // 0048F0F8..0048F103 ([esp+4Bh])
    Quaternion spin{1.0f, 0.0f, 0.0f, 0.0f};                                            // [esp+4Ch] 0048F107..0048F11F
    Quaternion orient{1.0f, 0.0f, 0.0f, 0.0f};                                          // [esp+8Ch] 0048F127..0048F148
    float inv_fps = fl(self, 0x68);                                                     // [esp+6Ch] 0048F153..0048F156
    Vector world_z{0.0f, 0.0f, 0.0f};                                                   // [esp+BCh] 0048F162..0048F178
    U fog_saved = 0;                                                                    // [esp+CCh] 0048F18A
    float scale = 1.0f;                                                                 // [esp+9Ch] 0048F191
    I pass = 0;                                                                         // [esp+B8h] 0048F19C
    const I passes = field<U8>(node, 0x10A) != 0 ? 5 : 1;                               // [esp+C8h] 0048F15A..0048F1B0
    if (emitter_debug_spheres_00830498 != 0) part_emitter_render_bounding_sphere_0048ef10(self, nullptr); // 0048F1BB..0048F1C3
    const I count = field<I>(self, 0x1CC);                                              // 0048F1CC
    if (count == 0) return;                                                             // 0048F1D2..0048F1D4
    if (static_cast<U>(count) * 4u > static_cast<U>(emitter_standard_pool_capacity_00798ba4)) { // 0048F1DA..0048F1E9 (jbe)
        const I old_capacity = emitter_standard_pool_capacity_00798ba4;                 // 0048F1EB
        const I new_capacity = static_cast<I>(static_cast<U>(count) * 8u);              // 0048F1F3
        emitter_standard_pool_capacity_00798ba4 = new_capacity;                         // 0048F1F7
        emitter_standard_pool_008304a0 = resize_emitter_pool_0048dda0(old_capacity, new_capacity, emitter_standard_pool_008304a0); // 0048F1ED..0048F208
    }
    {
        void* const gob = field<void*>(self, 0x40);                                     // 0048F20D
        if (gob != nullptr && field<U>(gob, 0x1B4) != 0x3F800000u) scale = bits_to_float(field<U>(gob, 0x1B4)); // 0048F214..0048F224
    }
    if (le_or_unordered(W(inv_fps), kZero_0073d700)) inv_fps = 0.0f;                    // 0048F22B..0048F23A / 0048F24C
    else inv_fps = S(kOne_0073d764 / W(inv_fps));                                       // 0048F23C..0048F246
    material_bind_texture0_0047adf0(field<void*>(self, 0x1E0), nullptr);                // 0048F254..0048F25A
    material_set_blending_mode_0047af70(field<void*>(self, 0x1E0), nullptr, 0);         // 0048F25F..0048F26A
    world_z = Vector{0.0f, 0.0f, 1.0f};                                                 // 0048F27F..0048F2A5
    const U mode = field<U>(self, 0x114);                                               // 0048F279
    if (mode == emitter_render_normal_00798b6c) {                                       // 0048F273 / 0048F29C
        Quaternion* const view = static_cast<Quaternion*>(at(*cells.current_camera_007bb4e8, 0x88)); // 0048F2B2..0048F2B8
        Vector z;
        dir = *quaternion_zdir_0043e250(view, nullptr, &z);                             // 0048F2BE..0048F2DD
        orient = *view;                                                                 // 0048F2E1..0048F301
    } else if (mode == emitter_render_billboard_world_z_00798b74) {                     // 0048F30D
        dir = world_z;                                                                  // 0048F327..0048F33C ([esp+70h..78h])
        Quaternion q;
        orient = *quaternion_construct_004ac960(&q, nullptr, world_z, 0.0f);            // 0048F315..0048F36F (angle push ebx = 0)
    } else if (mode == emitter_render_billboard_local_z_00798b70) {                     // 0048F37B
        Quaternion* const local = static_cast<Quaternion*>(at(self, 0x1A4));            // 0048F38B
        Vector z;
        dir = *quaternion_zdir_0043e250(local, nullptr, &z);                            // 0048F391..0048F3A8
        orient = *local;                                                                // 0048F3AC..0048F3CC
    } else {
        Quaternion* const view = static_cast<Quaternion*>(at(*cells.current_camera_007bb4e8, 0x88)); // 0048F3D8..0048F3DE
        Vector z;
        dir = *quaternion_zdir_0043e250(view, nullptr, &z);                             // 0048F3E4..0048F3FB
        orient = *view;                                                                 // 0048F3FF..0048F42E
        Vector y;
        const Vector* const yd = quaternion_ydir_0044f420(&orient, nullptr, &y);        // 0048F415..0048F435
        if (less((W(yd->y) + W(yd->x)) * kZero_0073d700 + W(yd->z), kZero_0073d700))   // 0048F43A..0048F453
            world_z.z = bits_to_float(0xBF800000u);                                     // 0048F455
    }
    float inv_life;                                                                     // [esp+D0h]
    if (le_or_unordered(W(fl(self, 0x78)), kZero_0073d700)) inv_life = bits_to_float(0x3C23D70Au); // 0048F460..0048F46E / 0048F482
    else inv_life = S(kOne_0073d764 / W(fl(self, 0x78)));                               // 0048F470..0048F479
    I grid_x = field<I>(node, 0x64);                                                    // [esp+80h] 0048F48D
    if (grid_x == 0) grid_x = 1;                                                        // 0048F499..0048F49B
    I grid_y = field<I>(node, 0x68);                                                    // [esp+84h] 0048F4A6
    if (grid_y == 0) grid_y = 1;                                                        // 0048F4B2..0048F4B4
    glDepthMask_0073d28c(0);                                                            // 0048F4BF..0048F4C0
    glDisable_0073d22c(*cells.gl_lighting_0078d45c);                                    // 0048F4C6..0048F4D3
    glEnable_0073d390(*cells.gl_blend_0078d448);                                        // 0048F4D5..0048F4DB
    glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);                                  // 0048F4E1..0048F4E8
    if (field<U>(field<void*>(self, 0x4), 0x100) != 0) {                                // 0048F4EA..0048F4F7
        glPolygonMode_0073d378(kFrontAndBack_0073f2f4, kFill_0073f2e8);                 // 0048F4F9..0048F507
        glDisable_0073d22c(*cells.gl_cull_face_0078d454);                               // 0048F50D..0048F513
    }
    const U blend = field<U>(self, 0x4C);                                               // 0048F519
    if (blend == 0) {
        glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);          // 0048F526..0048F533
    } else if (blend == 1) {
        glDisable_0073d22c(*cells.gl_blend_0078d448);                                   // 0048F53C..0048F543
        glEnable_0073d390(*cells.gl_alpha_test_0078d44c);                               // 0048F545..0048F54C
    } else {
        glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOne_0073f278);                        // 0048F554..0048F561
        fog_saved = glIsEnabled_0073d37c(*cells.gl_fog_0078d450);                       // 0048F563..0048F57A (movzx)
        glDisable_0073d22c(*cells.gl_fog_0078d450);                                     // 0048F570..0048F581
    }
    part_emitter_update_particle_frame_0048e460(self, nullptr, inv_fps);                // 0048F583..0048F58C
    const float inv_grid_x = S(kOne_0073d764 / static_cast<double>(grid_x));            // [esp+7Ch] 0048F591..0048F5B6
    emitter_particles_drawn_00830474 = static_cast<I>(static_cast<U>(emitter_particles_drawn_00830474) +
                                                      static_cast<U>(field<I>(self, 0x1CC))); // 0048F598..0048F5B0
    const float inv_grid_y = S(kOne_0073d764 / static_cast<double>(grid_y));            // [esp+D8h] 0048F5BA..0048F5C7
    glPushMatrix_0073d300();                                                            // 0048F5CE
    const float frame_scale = S(part_emitter_frame_scale_0048dc90(self, nullptr));      // [esp+128h] 0048F5D8..0048F5DF
    float gob_alpha = 1.0f;                                                             // [esp+D4h] 0048F5EB
    if (field<void*>(self, 0x40) != nullptr) gob_alpha = bits_to_float(field<U>(field<void*>(self, 0x40), 0xD8)); // 0048F5E6..0048F5FE

    I frame1 = 0;   // [esp+20h] D2: written by pass 1 for every particle before pass 3 reads it (never read uninitialised)
    I frame2 = 0;   // [esp+38h] D2: written by pass 2 for every particle before pass 4 reads it (never read uninitialised)
    for (; pass < passes; ) {                                                           // 0048F605..0048F60E / 0049068E..00490698
        I remaining = field<I>(self, 0x1CC);                                            // [esp+84h] 0048F621..0048F629
        while (remaining != 0) {                                                        // 0048F630 / 004905FF..0049060C
            remaining = static_cast<I>(static_cast<U>(remaining) - 1u);                 // 0048F685
            const I index = field<I>(self, 0x1CC) - remaining;                          // 0048F673..0048F686
            void* const p = as_object(field<U>(as_object(field<U>(self, 0x1C8)), static_cast<U>(index) * 4u - 4u)); // 0048F67F..0048F688
            float r, g, b, a, size_x, size_y;                                           // [24][28][2C][30] [14] [1C]
            if (!c0(W(fl(self, 0x78)), kZero_0073d700)) {                               // 0048F670..0048F698
                const float t = S(W(inv_life) * W(fl(p, 0x70)));                        // [10] 0048F69E..0048F6B3
                if (field<U>(self, 0xDC) == 0x437F0000u) {                              // 0048F6A5..0048F6BB (255.0f: no middle key)
                    const double a0 = W(t) * W(fl(self, 0x17C));                        // 0048F6C1
                    const double a1 = W(t) * W(fl(self, 0x180));                        // 0048F6C7..0048F6CB
                    const float a2 = S(W(t) * W(fl(self, 0x184)));                      // 0048F6D1..0048F6DB
                    const double om = kOne_0073d764 - W(t);                             // 0048F6E2..0048F6E8
                    const float om34 = S(om);                                           // [34] 0048F6EC fst
                    const double b0 = om * W(fl(self, 0x188));                          // 0048F6F0..0048F6F2
                    const float b1 = S(om * W(fl(self, 0x18C)));                        // 0048F6F8..0048F700
                    const float b2 = S(om * W(fl(self, 0x190)));                        // 0048F707..0048F70F
                    r = S(b0 + a0);                                                     // 0048F716..0048F718
                    g = S(W(b1) + a1);                                                  // 0048F726..0048F733
                    b = S(W(b2) + W(a2));                                               // 0048F749..0048F757
                    a = S(W(om34) * W(fl(self, 0x54)) + W(t) * W(fl(self, 0x50)));      // 0048F75E..0048F779
                    size_x = S((W(om34) * W(fl(self, 0x90)) + W(t) * W(fl(self, 0x94))) * kHalf_0073e9ac); // 0048F77D..0048F799
                    if (!eq(W(fl(self, 0x98)), kZero_0073d700) || !eq(W(fl(self, 0x9C)), kZero_0073d700)) // 0048F79D..0048F7C1
                        size_y = S((W(t) * W(fl(self, 0x9C)) + W(om34) * W(fl(self, 0x98))) * kHalf_0073e9ac); // 0048F7C7..0048F7E3
                    else size_y = size_x;                                               // 0048FA78..0048FA7C
                } else if (!c0(W(t), W(fl(self, 0xE4)))) {                              // 0048F7EC..0048F7F7 (t >= key3)
                    size_x = S(W(fl(self, 0x94)) * kHalf_0073e9ac);                     // 0048F7F9..0048F813
                    r = bits_to_float(raw(self, 0x17C)); g = bits_to_float(raw(self, 0x180)); b = bits_to_float(raw(self, 0x184)); // 0048F7FF..0048F82E
                    a = bits_to_float(raw(self, 0x50));                                 // 0048F82B..0048F832
                    if (eq(W(fl(self, 0x9C)), kZero_0073d700)) size_y = size_x;          // 0048F81B..0048F83B -> 0048FA78
                    else size_y = S(W(fl(self, 0x9C)) * kHalf_0073e9ac);                // 0048F841..0048F84D
                } else if (!c0(W(t), W(fl(self, 0xE0)))) {                              // 0048F856..0048F869 (key2 <= t < key3)
                    const float t2 = S((W(t) - W(fl(self, 0xE0))) / (W(fl(self, 0xE4)) - W(fl(self, 0xE0)))); // 0048F86F..0048F88F
                    Vector c;
                    const Vector* const mixed = vector_interpolate_0046eea0(&c, static_cast<const Vector*>(at(self, 0x11C)),
                                                                      static_cast<const Vector*>(at(self, 0x17C)), t2); // 0048F893..0048F8A2
                    r = mixed->x; g = mixed->y; b = mixed->z;                           // 0048F8B1..0048F8D3 (raw dword copies)
                    const double om = kOne_0073d764 - W(t2);                            // 0048F8A7..0048F8AD
                    a = S(om * W(fl(self, 0xD8)) + W(t2) * W(fl(self, 0x50)));          // 0048F8BA..0048F8D9
                    size_x = S((om * W(fl(self, 0xE8)) + W(t2) * W(fl(self, 0x94))) * kHalf_0073e9ac); // 0048F8DD..0048F8F7
                    if (!eq(W(fl(self, 0xEC)), kZero_0073d700) || !eq(W(fl(self, 0x9C)), kZero_0073d700)) // 0048F8FB..0048F91F
                        size_y = S((W(t2) * W(fl(self, 0x9C)) + om * W(fl(self, 0xEC))) * kHalf_0073e9ac); // 0048F925..0048F93F
                    else size_y = size_x;                                               // 0048FA76..0048FA7C
                } else if (!c0(W(t), W(fl(self, 0xDC)))) {                              // 0048F948..0048F953 (key1 <= t < key2)
                    const float t3 = S((W(t) - W(fl(self, 0xDC))) / (W(fl(self, 0xE0)) - W(fl(self, 0xDC)))); // 0048F959..0048F97D
                    Vector c;
                    const Vector* const mixed = vector_interpolate_0046eea0(&c, static_cast<const Vector*>(at(self, 0x188)),
                                                                      static_cast<const Vector*>(at(self, 0x11C)), t3); // 0048F981..0048F990
                    r = mixed->x; g = mixed->y; b = mixed->z;                           // 0048F99F..0048F9C1
                    const double om = kOne_0073d764 - W(t3);                            // 0048F995..0048F99B
                    a = S(om * W(fl(self, 0x54)) + W(t3) * W(fl(self, 0xD8)));          // 0048F9A8..0048F9C7
                    size_x = S((om * W(fl(self, 0x90)) + W(t3) * W(fl(self, 0xE8))) * kHalf_0073e9ac); // 0048F9CB..0048F9E5
                    if (!eq(W(fl(self, 0x98)), kZero_0073d700) || !eq(W(fl(self, 0xEC)), kZero_0073d700)) // 0048F9E9..0048FA0D
                        size_y = S((W(t3) * W(fl(self, 0xEC)) + om * W(fl(self, 0x98))) * kHalf_0073e9ac); // 0048FA0F..0048FA1B -> 0048F7D5..0048F7E3
                    else size_y = size_x;                                               // 0048FA76..0048FA7C
                } else {                                                                // 0048FA20 (t < key1)
                    size_x = S(W(fl(self, 0x90)) * kHalf_0073e9ac);                     // 0048FA20..0048FA3A
                    r = bits_to_float(raw(self, 0x188)); g = bits_to_float(raw(self, 0x18C)); b = bits_to_float(raw(self, 0x190)); // 0048FA26..0048FA55
                    a = bits_to_float(raw(self, 0x54));                                 // 0048FA52..0048FA59
                    if (eq(W(fl(self, 0x98)), kZero_0073d700)) size_y = size_x;          // 0048FA42..0048FA62 -> 0048FA78
                    else size_y = S(W(fl(self, 0x98)) * kHalf_0073e9ac);                // 0048FA64..0048FA70
                }
                clamp01(r); clamp01(g); clamp01(b); clamp01(a);                         // 0048FA80..0048FB57
            } else {                                                                    // 0048FB5C (lifetime < 0)
                r = bits_to_float(raw(self, 0x188)); g = bits_to_float(raw(self, 0x18C)); b = bits_to_float(raw(self, 0x190)); // 0048FB5C..0048FB7F
                a = bits_to_float(raw(self, 0x54));                                     // 0048FB78..0048FB83
                clamp01(r); clamp01(g); clamp01(b); clamp01(a);                         // 0048FB6E..0048FC45
                size_x = S(W(fl(self, 0x90)) * kHalf_0073e9ac);                         // 0048FC4D..0048FC59
                if (!eq(W(fl(self, 0x98)), kZero_0073d700) || !eq(W(fl(self, 0xEC)), kZero_0073d700) ||
                    !eq(W(fl(self, 0x9C)), kZero_0073d700))                             // 0048FC5D..0048FC94
                    size_y = S(W(fl(self, 0x98)) * kHalf_0073e9ac);                     // 0048FCA0..0048FCAC
                else size_y = size_x;                                                   // 0048FC96..0048FC9A
            }
            if (splat) {                                                                // 0048FCB0..0048FCB6
                if (field<U8>(p, 0x6C) != 0) {                                          // 0048FCB8..0048FCBD
                    size_x = bits_to_float(raw(self, 0x94));                            // 0048FCC5..0048FCD1
                    if (eq(W(fl(self, 0x9C)), kZero_0073d700)) size_y = size_x;          // 0048FCBF..0048FCDA -> 0048FD11
                    else size_y = bits_to_float(raw(self, 0x9C));                       // 0048FCDC..0048FCE2
                } else {
                    size_x = bits_to_float(raw(self, 0x90));                            // 0048FCEE..0048FCFA
                    if (eq(W(fl(self, 0x98)), kZero_0073d700)) size_y = size_x;          // 0048FCE8..0048FD03 -> 0048FD11
                    else size_y = bits_to_float(raw(self, 0x98));                       // 0048FD05..0048FD0B
                }
            }
            const Quaternion* rhs;
            if (field<U8>(p, 0x6C) != 0) {                                              // 0048FD19..0048FD1E
                Quaternion q;
                spin = *quaternion_construct_004ac960(&q, nullptr, field<Vector>(p, 0x48), bits_to_float(raw(p, 0x14))); // 0048FD20..0048FD5F
                rhs = static_cast<const Quaternion*>(at(p, 0x2C));                      // 0048FD63
            } else {
                Quaternion q;
                spin = *quaternion_construct_004ac960(&q, nullptr, dir, bits_to_float(raw(p, 0x14))); // 0048FD6F..0048FDB2
                rhs = &orient;                                                          // 0048FDB6
            }
            {
                Quaternion product;
                spin = *quaternion_multiply_004a9ac0(&product, &spin, rhs);             // 0048FDC4..0048FDFC
            }
            if (!eq(W(scale), kOne_0073d764)) {                                         // 0048FDD0..0048FE00
                size_x = S(W(size_x) * W(scale));                                       // 0048FE02..0048FE0D
                size_y = S(W(size_y) * W(scale));                                       // 0048FE11..0048FE1C
            }
            // Atlas cell selection per frame-blend pass (0048FE20..004900D3).
            I column = 0, row = 0;                                                      // edi / ebp 0048FE2B..0048FE2D ([10] / [34] = 0)
            float weight = 0.0f;                                                        // [esp+88h] 0048FE32
            const double f70 = W(fl(self, 0x70)), f6c = W(fl(self, 0x6C));
            bool select = false;
            I frame = 0;
            if (pass == 1) {                                                            // 0048FE2F..0048FE45
                I fr = field<I>(p, 0x3C);                                               // 0048FE4E
                if (eq(f70, f6c)) frame1 = -1;                                          // 0048FE4B..0048FE59 -> 00490041
                else {
                    bool ok;
                    if (less(f6c, f70)) {                                               // 0048FE5F..0048FE6A
                        fr = static_cast<I>(static_cast<U>(fr) - 1u);                   // 0048FE6C
                        ok = !c0(static_cast<double>(fr), W(fl(self, 0x6C)));           // 0048FE71..0048FE7D
                    } else {
                        fr = static_cast<I>(static_cast<U>(fr) + 1u);                   // 0048FE85
                        ok = !gt_or_unordered(static_cast<double>(fr), W(fl(self, 0x6C))); // 0048FE8A..0048FE96
                    }
                    frame1 = ok ? fr : -1;                                              // 0048FE9C..0048FEA0 / 00490041
                    if (ok && fr >= 0) {                                                // 0048FEA4
                        weight = S((W(fl(p, 0x70)) - W(fl(p, 0x44))) / W(inv_fps) * kHalf_0073e9ac); // 0048FEAA..0048FEBA
                        select = true; frame = fr;
                    }
                }
            } else if (pass == 2) {                                                     // 0048FEEB..0048FEEE
                I fr = field<I>(p, 0x3C);                                               // 0048FEF7
                if (eq(f70, f6c)) frame2 = -1;                                          // 0048FEF4..0048FF02 -> 0048FF92
                else {
                    bool ok;
                    if (less(f6c, f70)) {                                               // 0048FF08..0048FF13
                        fr = static_cast<I>(static_cast<U>(fr) + 1u);                   // 0048FF15
                        ok = !gt_or_unordered(static_cast<double>(fr), W(fl(self, 0x70))); // 0048FF1A..0048FF26
                    } else {
                        fr = static_cast<I>(static_cast<U>(fr) - 1u);                   // 0048FF7F
                        ok = !c0(static_cast<double>(fr), W(fl(self, 0x70)));           // 0048FF84..0048FF90
                    }
                    frame2 = ok ? fr : -1;                                              // 0048FF28..0048FF2C / 0048FF92
                    if (ok && fr >= 0) {                                                // 0048FF30
                        weight = S(kOne_0073d764 - (W(fl(p, 0x70)) - W(fl(p, 0x44))) / W(inv_fps) * kHalf_0073e9ac); // 0048FF36..0048FF4C
                        select = true; frame = fr;
                    }
                }
            } else if (pass == 3) {                                                     // 0048FF9F..0048FFA2
                if (eq(f70, f6c)) frame1 = -1;                                          // 0048FFA8..0048FFB3 -> 00490041
                else {
                    I fr = frame1;                                                      // 0048FFBC
                    bool ok;
                    if (less(f6c, f70)) {                                               // 0048FFB9..0048FFC8
                        fr = static_cast<I>(static_cast<U>(fr) - 1u);                   // 0048FFCA
                        ok = !c0(static_cast<double>(fr), W(fl(self, 0x6C)));           // 0048FFCF..0048FFDB
                    } else {
                        fr = static_cast<I>(static_cast<U>(fr) + 1u);                   // 0049002E
                        ok = le_ordered(static_cast<double>(fr), W(fl(self, 0x6C)));    // 00490033..0049003F
                    }
                    frame1 = ok ? fr : -1;                                              // 0048FFDD..0048FFE1 / 00490041
                    if (ok && fr >= 0) {                                                // 0048FFE5
                        weight = S((W(fl(p, 0x70)) - W(fl(p, 0x44))) / W(inv_fps) * kQuarter_0073edf0); // 0048FFEB..0048FFFB
                        select = true; frame = fr;
                    }
                }
            } else if (pass == 4) {                                                     // 0049004E..00490051
                frame2 = part_emitter_next_frame_0048dd50(self, nullptr, frame2);       // 00490053..00490061
                if (frame2 >= 0) {                                                      // 00490065
                    weight = S((kOne_0073d764 - (W(fl(p, 0x70)) - W(fl(p, 0x44))) / W(inv_fps)) * kQuarter_0073edf0); // 00490067..0049007D -> 0048FF4C
                    select = true; frame = frame2;
                }
            } else {                                                                    // 00490082 (pass 0)
                const I fr = field<I>(p, 0x3C);                                         // 00490082..00490085
                row = ftol2(static_cast<double>(fr) * W(inv_grid_x));                   // 00490089..00490096
                column = static_cast<I>(static_cast<U>(fr) - static_cast<U>(row) * static_cast<U>(grid_x)); // 004900A5..004900AF
                weight = field<U8>(node, 0x10A) != 0 ? 0.5f : 1.0f;                     // 0049009F..004900C8 (node via [esp+118h])
            }
            if (select) {                                                               // 0048FEC1..0048FEE2 / 0048FF53..0048FF76 / 00490002..00490025
                row = ftol2(static_cast<double>(frame) * W(inv_grid_x));
                column = static_cast<I>(static_cast<U>(frame) - static_cast<U>(row) * static_cast<U>(grid_x));
            }
            // Particle axes (004900D3..004902D5).
            Vector up;                                                                  // [esp+A0h]
            if (field<U>(self, 0x114) == emitter_render_aligned_particle_dir_00798b88) { // 004900D3..004900DF
                const double w = W(fl(p, 0x2C)), x = W(fl(p, 0x30)), y = W(fl(p, 0x34)), z = W(fl(p, 0x38));
                right.x = S(kOne_0073d764 - (y * y + z * z) * 2.0);                     // 004900E5..004900FD
                right.y = S((w * z + y * x) * 2.0);                                     // 0049010F..0049011F
                right.z = S((z * x - w * y) * 2.0);                                     // 00490126..00490145
                up.x = S((z * x + y * w) * 2.0);                                        // 00490157..00490167
                up.y = S((z * y - x * w) * 2.0);                                        // 00490175..0049018C
                up.z = S(kOne_0073d764 - (y * y + x * x) * 2.0);                        // 00490193..004901B9
            } else {
                const double w = W(spin.w), x = W(spin.x), y = W(spin.y), z = W(spin.z);
                right.x = S(kOne_0073d764 - (y * y + z * z) * 2.0);                     // 004901D7..004901F1
                right.y = S((x * y + z * w) * 2.0);                                     // 004901FF..00490217
                right.z = S((x * z - y * w) * 2.0);                                     // 0049021E..00490242
                if (field<U>(self, 0x114) == emitter_render_aligned_world_z_00798b7c) { // 00490235..0049025C
                    up = world_z;                                                       // 0049025E..00490281
                } else {
                    Vector c;
                    const Vector* const crossed = splat
                        ? vector_cross_004ab210(&c, static_cast<const Vector*>(at(p, 0x48)), &right) // 00490292..004902A3
                        : vector_cross_004ab210(&c, &dir, &right);                      // 004902A5..004902B7
                    up = *crossed;                                                      // 004902BC..004902D5
                }
            }
            right.x = S(W(right.x) * W(size_x));                                        // 004902DC..004902F6
            right.y = S(W(right.y) * W(size_x));                                        // 004902F2..00490314
            right.z = S(W(right.z) * W(size_x));                                        // 00490305..00490331
            const Vector up_scaled{S(W(up.x) * W(size_y)), S(W(up.y) * W(size_y)), S(W(up.z) * W(size_y))}; // [esp+ACh] 0049031F..00490359
            const I red = fistp_nearest(W(S(W(r) * k255_0073ede4)));                    // [60] 00490360..0049037B
            const I green = fistp_nearest(W(S(W(g) * k255_0073ede4)));                  // [68] 0049037D..00490398
            const I blue = fistp_nearest(W(S(W(b) * k255_0073ede4)));                   // [5C] 0049039A..004903B5
            const I alpha = fistp_nearest(W(S(W(gob_alpha) * W(weight) * W(a) * W(frame_scale) * k255_0073ede4))); // [64] 004903B7..004903E7
            const U packed = ((((static_cast<U>(alpha) << 8) | static_cast<U>(blue)) << 8 | static_cast<U>(green)) << 8) | static_cast<U>(red); // 004903E9..0049040D
            unsigned char* const v = static_cast<unsigned char*>(emitter_standard_pool_008304a0) + static_cast<U>(remaining) * 96u; // 00490403..0049041A
            for (U k = 0; k < 4; ++k) field<U>(v, 0x0C + k * 0x18) = packed;            // 0049041C..0049042A
            const I row1 = static_cast<I>(static_cast<U>(row) + 1u);                    // 0049042F inc ebp
            const I column1 = static_cast<I>(static_cast<U>(column) + 1u);              // 00490434 inc edi
            const double px = W(fl(p, 0x8)), py = W(fl(p, 0xC)), pz = W(fl(p, 0x10));
            // vertex 0: p - right - up
            {
                const double x0 = px - W(right.x);                                      // 0049042C..00490430
                const float y0 = S(py - W(right.y));                                    // 00490435..0049043C
                const float z0 = S(pz - W(right.z));                                    // 00490443..0049044A
                field<float>(v, 0x00) = S(x0 - W(up_scaled.x));                         // 00490451 / 00490484
                field<float>(v, 0x04) = S(W(y0) - W(up_scaled.y));                      // 00490458..0049045F / 00490486
                field<float>(v, 0x08) = S(W(z0) - W(up_scaled.z));                      // 00490466..00490489
            }
            const double u0 = static_cast<double>(column) * W(inv_grid_x);              // 0049048C..00490494
            field<float>(v, 0x10) = S(u0);                                              // 00490498 fst
            const double v1 = kOne_0073d764 - static_cast<double>(row1) * W(inv_grid_y); // 0049049B..004904AA
            field<float>(v, 0x14) = S(v1);                                              // 004904B0 fst
            // vertex 1: right + p - up
            {
                const double x1 = W(right.x) + px;                                      // 004904B3..004904B7
                const float y1 = S(W(right.y) + py);                                    // 004904BA..004904C1
                const float z1 = S(W(right.z) + pz);                                    // 004904C8..004904CF
                field<float>(v, 0x18) = S(x1 - W(up_scaled.x));                         // 004904D6 / 00490509
                field<float>(v, 0x1C) = S(W(y1) - W(up_scaled.y));                      // 004904DD..004904E4 / 0049050C
                field<float>(v, 0x20) = S(W(z1) - W(up_scaled.z));                      // 004904EB..0049050F
            }
            const double u1 = static_cast<double>(column1) * W(inv_grid_x);             // 00490512..00490516
            field<float>(v, 0x28) = S(u1);                                              // 0049051A fst
            field<float>(v, 0x2C) = S(v1);                                              // 0049051D..0049051F
            // vertex 2: right + p + up
            {
                const double x2 = W(right.x) + px;                                      // 00490522..00490526
                const float y2 = S(W(right.y) + py);                                    // 00490529..00490530
                const float z2 = S(W(right.z) + pz);                                    // 00490537..0049053E
                field<float>(v, 0x30) = S(x2 + W(up_scaled.x));                         // 00490545 / 00490571
                field<float>(v, 0x34) = S(W(y2) + W(up_scaled.y));                      // 0049054C..00490553 / 0049057B
                field<float>(v, 0x38) = S(W(z2) + W(up_scaled.z));                      // 0049055A..0049057E
            }
            field<float>(v, 0x40) = S(u1);                                              // 00490581
            const double v0 = kOne_0073d764 - static_cast<double>(row) * W(inv_grid_y); // 00490584..0049058F ([34] = row)
            field<float>(v, 0x44) = S(v0);                                              // 00490595 fst
            // vertex 3: p - right + up
            {
                const double x3 = px - W(right.x);                                      // 00490598..0049059B
                const float y3 = S(py - W(right.y));                                    // 0049059F..004905A6
                const float z3 = S(pz - W(right.z));                                    // 004905AD..004905B4
                field<float>(v, 0x48) = S(x3 + W(up_scaled.x));                         // 004905BB / 004905EE
                field<float>(v, 0x4C) = S(W(y3) + W(up_scaled.y));                      // 004905C2..004905C9 / 004905F1
                field<float>(v, 0x50) = S(W(z3) + W(up_scaled.z));                      // 004905D0..004905F4
            }
            field<float>(v, 0x58) = S(u0);                                              // 004905F7..004905F9
            field<float>(v, 0x5C) = S(v0);                                              // 004905FC
        }
        if (pass > 0) {                                                                 // 00490625..00490627
            glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOne_0073f278);                    // 00490629..00490636
            fog_saved = glIsEnabled_0073d37c(*cells.gl_fog_0078d450);                   // 00490638..0049064F
            glDisable_0073d22c(*cells.gl_fog_0078d450);                                 // 00490645..00490656
        }
        *emitter_cells.triangles_00827fb0 = static_cast<I>(static_cast<U>(*emitter_cells.triangles_00827fb0) +
                                                           static_cast<U>(field<I>(self, 0x1CC))); // 0049065C..0049066F
        kotor_cdecl_00431cc0(static_cast<float*>(emitter_standard_pool_008304a0),
                                               static_cast<I>(static_cast<U>(field<I>(self, 0x1CC)) * 4u)); // 00490674..0049067F
        pass = pass + 1;                                                                // 0049068E..00490691
    }
    glPopMatrix_0073d2fc();                                                             // 0049069E
    glDepthMask_0073d28c(1);                                                            // 004906A4..004906A6
    glEnable_0073d390(*cells.gl_lighting_0078d45c);                                     // 004906AC..004906B9
    glBlendFunc_0073d38c(kSrcAlpha_0073f270, kOneMinusSrcAlpha_0073f274);              // 004906BB..004906C8
    if (fog_saved != 0) glEnable_0073d390(*cells.gl_fog_0078d450);                      // 004906CA..004906DC
}

} // extern "C"
