// render_linked.cpp - lane-3d-emitter: 00495B20 PartEmitter::RenderLinked (render mode "Linked").
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1) 00495B20..00496E38; instruction addresses cited per behaviour.
// Stack locals are named after their [esp+X] offset in the original frame (after the four register pushes).
// D30-FINAL: values the original keeps on the x87 stack are double; float only at `fst/fstp dword` points.
// EH: the original registers unwind state 00717969 around the static-pool init and the local CExoArrayList
// (state 1 from 00495D8D); no callee here throws (live_operator_new returns null), so the cleanup is not emitted.
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS (merge into emitter.h): none beyond emitter.h / scene3d.h.

namespace {
using namespace kotor_emitter;
using kotor_scene3d::cells;

// .rdata constants read through memory operands (values from the pinned image).
constexpr U kGlSrcAlpha_0073f270 = 0x302;           // GL_SRC_ALPHA
constexpr U kGlOneMinusSrcAlpha_0073f274 = 0x303;   // GL_ONE_MINUS_SRC_ALPHA
constexpr U kGlOne_0073f278 = 1;                    // GL_ONE
constexpr double kZero_0073d700 = 0.0;
constexpr double kOne_0073d764 = 1.0;
constexpr double kHalf_0073e9ac = 0.5;
constexpr double k255_0073ede4 = 255.0;
constexpr double kEpsilon_00743308 = 1e-05;         // qword 3EE4F8B588E368F1
constexpr U kOneBits = 0x3F800000u;
constexpr U kHundredth = 0x3C23D70Au;               // 0.01f (00495F76)
constexpr U k255Bits = 0x437F0000u;                 // 255.0f (0049614D integer compare)

inline float fl(void* object, U offset) { return field<float>(object, offset); }
inline double wf(void* object, U offset) { return W(field<float>(object, offset)); }
// 0049650C..004966D1 clamp idiom: `fcomp 0 / test ah,5 / jp` then `fcomp 1 / test ah,41h / jne`.
inline void clamp01(float& v) {
    if (W(v) < kZero_0073d700) v = 0.0f;
    else if (W(v) > kOne_0073d764) v = F(kOneBits);
}
inline void* particle_at(void* emitter, I index) {
    return static_cast<void* const*>(as_object(field<U>(emitter, 0x1C8)))[index];   // [ebp+1C8][i*4]
}
}

extern "C" void __fastcall part_emitter_render_linked_00495b20(void* emitter, void*) {
    // 00495B2E..00495B70: function-static vertex pool, guard bit 0 of 008304B4; new(5DC0h) = 1000 * 18h.
    if ((emitter_linked_pool_guard_008304b4 & 1u) == 0) {
        emitter_linked_pool_guard_008304b4 = emitter_linked_pool_guard_008304b4 | 1u;   // 00495B51
        emitter_linked_pool_008304b0 = live_operator_new(0x5DC0);                     // 00495B57..00495B6B
    }
    // 00495B7B..00495B9D: owner fade object; fade < 1.0 -> nothing drawn.
    void* const gob = field<void*>(emitter, 0x40);
    if (gob != nullptr) {
        void* const fade = field<void*>(gob, 0x170);
        if (fade != nullptr && wf(fade, 0x94) < kOne_0073d764) return;               // 00495B8C..00495B9D (jnp)
    }
    // 00495BA3..00495D80: camera orientation (w,x,y,z) = camera[+88..+94] -> rotation rows.
    void* const camera = *cells.current_camera_007bb4e8;
    const double qw = wf(camera, 0x88), qx = wf(camera, 0x8C), qy = wf(camera, 0x90), qz = wf(camera, 0x94);
    // Quaternion locals [80..8C] and [D8..E4] start as identity (00495BA8..00495BF5).
    Quaternion q80 = {F(kOneBits), 0.0f, 0.0f, 0.0f};
    Quaternion qd8 = {F(kOneBits), 0.0f, 0.0f, 0.0f};
    U fog_was_enabled_90 = 0;                                                             // 00495C0C
    const double t1 = (qx * qz - qy * qw) * 2.0;                                          // 00495C00..00495C21
    const float c68 = S(t1);                                                              // 00495C23 fst
    const double t2 = (qw * qz + qy * qx) * 2.0;                                          // 00495C27..00495C41
    const float c6C = S(t2);                                                              // 00495C43 fst
    const float cAC = S(1.0 - (qy * qy + qz * qz) * 2.0);                                 // 00495C47..00495C65
    const float cB0 = S(t2), cB4 = S(t1);                                                 // 00495C70 / 00495C77
    const double t3 = (qz * qy + qw * qx) * 2.0;                                          // 00495C7E..00495C98
    const float c7C = S(t3);                                                              // 00495C9A fst
    const double r1 = 1.0 - (qx * qx + qz * qz) * 2.0;                                    // 00495C9E..00495CB6
    const float c70 = S(r1);                                                              // 00495CBC fst
    const float cC4 = S((qy * qx - qz * qw) * 2.0);                                       // 00495CC4..00495CE0
    const float cC8 = S(r1), cCC = S(t3);                                                 // 00495CE7 / 00495CEE
    const double r2 = 1.0 - (qx * qx + qy * qy) * 2.0;                                    // 00495CF5..00495D0D
    U c10 = field<U>(emitter, 0x68);                                                      // 00495D13..00495D16 (raw dword)
    PointerList list_b8;                                                                  // CExoArrayList<Vector> at [B8]
    list_b8.size = 0; list_b8.capacity = 0; list_b8.data = 0;                             // 00495D1A..00495D2C
    const double t5 = (qz * qy - qw * qx) * 2.0;                                          // 00495D33..00495D4D
    const double t6 = (qw * qy + qz * qx) * 2.0;                                          // 00495D4F..00495D70
    void* const node = field<void*>(emitter, 4);                                          // 00495D67 -> [58]
    const float cE8 = S(t6), cEC = S(t5), cF0 = S(r2);                                    // 00495D72..00495D80
    U c5C = kOneBits;                                                                     // 00495D94
    if (field<I>(emitter, 0x1CC) == 0) return;                                            // 00495D87..00495D9C (list data still 0)

    // 00495DA2..00495E37: particle positions, offset by the vtbl+84 object's per-particle vectors (+270).
    void* const offsets = as_object(part_table(emitter)->s084(emitter, nullptr));         // 00495DA2..00495DB5 -> [3C]
    const I count0 = field<I>(emitter, 0x1CC);
    if (count0 == 0) list_b8.capacity = 0;                                                // 00495DBB
    else vector_list_allocate_00421310(&list_b8, nullptr, count0);                        // 00495DC4..00495DCC
    list_b8.size = count0;                                                                // 00495DD7
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                                    // 00495DD1..00495E37 (bound re-read)
        Vector* const slot = reinterpret_cast<Vector*>(static_cast<std::uintptr_t>(list_b8.data + static_cast<U>(i) * 0xCu));
        std::memcpy(slot, at(particle_at(emitter, i), 8), 12);                            // 00495DF0..00495E11 particle[+8]
        if (offsets != nullptr) {                                                         // 00495E08..00495E14
            const U extra = field<U>(offsets, 0x270) + static_cast<U>(i) * 0xCu;          // 00495E18..00495E1E
            vector_add_assign_004a9740(slot, nullptr, static_cast<const Vector*>(as_object(extra))); // 00495E20..00495E23
        }
    }
    if (emitter_debug_spheres_00830498 != 0) part_emitter_render_bounding_sphere_0048ef10(emitter, nullptr); // 00495E39..00495E44

    // 00495E49..00495EA3: grow the static pool when 4*count exceeds the capacity (unsigned compare, jbe).
    {
        const U count = field<U>(emitter, 0x1CC);
        const U capacity = static_cast<U>(emitter_linked_pool_capacity_00798bac);         // 00495E4F
        if (count * 4u > capacity) {
            void* const old_pool = emitter_linked_pool_008304b0;                          // 00495E60
            emitter_linked_pool_capacity_00798bac = static_cast<I>(count * 8u);           // 00495E70
            void* const new_pool = live_operator_new(count * 8u * 0x18u);                 // 00495E66..00495E75
            std::memcpy(new_pool, old_pool, capacity * 0x18u);                            // 00495E7A..00495E95 rep movsd/movsb
            live_free(old_pool);                                                          // 00495E94..00495E97
            emitter_linked_pool_008304b0 = new_pool;                                      // 00495EA3
        }
    }
    // 00495EA9..00495EC0: owner alpha scale gob[+1B4] when its bits differ from 1.0f.
    if (gob != nullptr && field<U>(gob, 0x1B4) != kOneBits) c5C = field<U>(gob, 0x1B4);
    // 00495EC4..00495EE5: frame rate [10] = 1/[ebp+68] when > 0, else 0.
    if (W(F(c10)) > kZero_0073d700) c10 = float_to_bits(S(kOne_0073d764 / W(F(c10))));
    else c10 = 0;
    void* const material = field<void*>(emitter, 0x1E0);
    material_bind_texture0_0047adf0(material, nullptr);                                   // 00495EED..00495EF3
    material_set_blending_mode_0047af70(field<void*>(emitter, 0x1E0), nullptr, 0);         // 00495EF8..00495F00
    std::memcpy(&qd8, at(camera, 0x88), 16);                                              // 00495F05..00495F3C
    // 00495F2F..00495F76: per-particle colour step [64].
    float c64;
    if (static_cast<I>(field<std::int16_t>(emitter, 0x1F4)) == static_cast<I>(emitter_render_mode_00798b80))
        c64 = S(kOne_0073d764 / static_cast<double>(field<I>(emitter, 0x1CC)));           // 00495F45..00495F51
    else if (wf(emitter, 0x78) > kZero_0073d700)
        c64 = S(kOne_0073d764 / wf(emitter, 0x78));                                       // 00495F67..00495F70
    else c64 = F(kHundredth);                                                             // 00495F76
    // 00495F7E..00495FA0: texture grid columns [74] = node[+64], rows [3C] = node[+68]; 0 -> 1.
    I c74 = field<I>(node, 0x64); if (c74 == 0) c74 = 1;
    I c3C = field<I>(node, 0x68); if (c3C == 0) c3C = 1;
    // 00495FA8..00496039: GL state.
    kotor_renderer_init::glDepthMask_0073d28c(0);                                         // 00495FA8
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_lighting_0078d45c);                 // 00495FB0..00495FBD
    kotor_renderer_init::glEnable_0073d390(*cells.gl_blend_0078d448);                     // 00495FBF..00495FCC
    kotor_renderer_init::glDisable_0073d22c(*cells.gl_alpha_test_0078d44c);               // 00495FCE..00495FD4
    const U blend = field<U>(emitter, 0x4C);
    if (blend == 0) {
        kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274); // 00495FE3..00495FF0
    } else if (blend == 1) {
        kotor_renderer_init::glDisable_0073d22c(*cells.gl_blend_0078d448);                // 00495FF9..00496000
        kotor_renderer_init::glEnable_0073d390(*cells.gl_alpha_test_0078d44c);            // 00496002..00496009
    } else {
        kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOne_0073f278); // 0049600D..0049601A
        fog_was_enabled_90 = kotor_renderer_init::glIsEnabled_0073d37c(*cells.gl_fog_0078d450); // 0049601C..00496032 movzx
        kotor_renderer_init::glDisable_0073d22c(*cells.gl_fog_0078d450);                  // 00496028..00496039
    }
    part_emitter_update_particle_frame_0048e460(emitter, nullptr, F(c10));                // 0049603B..00496042
    const float c78 = S(kOne_0073d764 / static_cast<double>(c74));                        // 00496047..00496078
    emitter_particles_drawn_00830474 = emitter_particles_drawn_00830474 + field<I>(emitter, 0x1CC); // 0049604B..0049605F
    U cD4 = kOneBits;                                                                     // 0049606D
    const float c58 = S(kOne_0073d764 / static_cast<double>(c3C));                        // 0049607C..00496086
    // 00496065..004960BC: fade from [ebp+1EC] (or gob[+170]): (emitter[+16C] - x[+30]) > 5.0 -> [D4] = x[+50].
    void* fader = field<void*>(emitter, 0x1EC);
    if (fader == nullptr && gob != nullptr) fader = field<void*>(gob, 0x170);
    if (fader != nullptr && (wf(emitter, 0x16C) - wf(fader, 0x30)) > W(F(emitter_frame_threshold_00798b94)))
        cD4 = field<U>(fader, 0x50);
    U cD0 = kOneBits;                                                                     // 004960C8
    if (gob != nullptr) cD0 = field<U>(gob, 0xD8);                                        // 004960D5..004960DB

    void* pool = emitter_linked_pool_008304b0;
    U k60 = 0;                                                                            // [60] / edi: byte offset of quad i
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i, k60 += 0x60) {                       // 004960E2..00496D93
        const float* const A = reinterpret_cast<const float*>(
            static_cast<std::uintptr_t>(list_b8.data + 0x10u + static_cast<U>(i) * 0xCu)); // [44] = data+10h (+0Ch per i)
        float c18, c1C, c20, c28, c14, c24;
        if (!(wf(emitter, 0x78) < kZero_0073d700)) {                                       // 00496110..0049611E
            double t;
            if (static_cast<I>(field<std::int16_t>(emitter, 0x1F4)) == static_cast<I>(emitter_render_mode_00798b80))
                t = static_cast<double>(i) * W(c64);                                       // 00496133..00496137
            else t = W(c64) * wf(particle_at(emitter, i), 0x70);                          // 0049613D..0049614A
            if (field<U>(emitter, 0xDC) == k255Bits) {                                     // 0049614D..00496157
                const double a = t * wf(emitter, 0x184);                                  // 0049615D..0049615F
                const float c34 = S(t * wf(emitter, 0x180));                              // 00496165..0049616D
                const float c1AC = S(t * wf(emitter, 0x17C));                             // 00496171..00496179
                const double u = kOne_0073d764 - t;                                        // 00496180..00496186
                const float c2C = S(u);                                                   // 00496188
                const float c38 = S(u * wf(emitter, 0x190));                              // 0049618C..00496194
                const float c54 = S(u * wf(emitter, 0x18C));                              // 00496198..004961A0
                const float c194 = S(u * wf(emitter, 0x188));                             // 004961A4..004961AA
                c20 = S(a + W(c38));                                                      // 004961B1, 004961EF..004961FD
                c1C = S(W(c54) + W(c34));                                                 // 004961B5..004961B9, 004961DD..004961EB
                c18 = S(W(c194) + W(c1AC));                                               // 004961BD..004961D9
                c28 = S(W(c2C) * wf(emitter, 0x54) + t * wf(emitter, 0x50));              // 004961FD..0049620F
                c14 = S((W(c2C) * wf(emitter, 0x90) + t * wf(emitter, 0x94)) * kHalf_0073e9ac); // 00496213..0049622D
                if (wf(emitter, 0x98) == 0.0 && wf(emitter, 0x9C) == 0.0) c24 = c14;     // 00496231..0049625D
                else c24 = S((t * wf(emitter, 0x9C) + W(c2C) * wf(emitter, 0x98)) * kHalf_0073e9ac); // 00496266..0049627E
            } else if (!(t < wf(emitter, 0xE4))) {                                         // 00496287..00496292
                std::memcpy(&c18, at(emitter, 0x17C), 4); std::memcpy(&c1C, at(emitter, 0x180), 4);
                std::memcpy(&c20, at(emitter, 0x184), 4);                                 // 00496294..004962BD
                c28 = fl(emitter, 0x50);                                                  // 004962B8..004962CA (raw dword)
                c14 = fl(emitter, 0x94);                                                  // 004962C4..004962CE (raw dword, not halved)
                if (wf(emitter, 0x9C) == 0.0) c24 = c14;                                  // 0049629E..004962D2 -> 0049649D
                else c24 = fl(emitter, 0x9C);                                             // 004962D8 -> 00496508
            } else if (!(t < wf(emitter, 0xE0))) {                                         // 004962E3..004962EE
                const float f = S((t - wf(emitter, 0xE0)) / (wf(emitter, 0xE4) - wf(emitter, 0xE0))); // 004962F4..00496314
                Vector out;
                const Vector* const r = vector_interpolate_0046eea0(&out, static_cast<const Vector*>(at(emitter, 0x11C)),
                                                             static_cast<const Vector*>(at(emitter, 0x17C)), f); // 0049631C..00496327
                c18 = r->x; c1C = r->y; c20 = r->z;                                       // 00496336..00496358
                const double om = kOne_0073d764 - W(f);                                   // 0049632C..00496332
                c28 = S(om * wf(emitter, 0xD8) + W(f) * wf(emitter, 0x50));               // 0049633E..0049635E
                c14 = S((om * wf(emitter, 0xE8) + W(f) * wf(emitter, 0x94)) * kHalf_0073e9ac); // 00496362..0049637C
                if (wf(emitter, 0xEC) == 0.0 && wf(emitter, 0x9C) == 0.0) c24 = c14;     // 00496380..004963AC
                else c24 = S((W(f) * wf(emitter, 0x9C) + om * wf(emitter, 0xEC)) * kHalf_0073e9ac); // 004963B5..004963CF
            } else if (!(t < wf(emitter, 0xDC))) {                                         // 004963D8..004963E3
                const float f = S((t - wf(emitter, 0xDC)) / (wf(emitter, 0xE0) - wf(emitter, 0xDC))); // 004963E9..00496409
                Vector out;
                const Vector* const r = vector_interpolate_0046eea0(&out, static_cast<const Vector*>(at(emitter, 0x188)),
                                                             static_cast<const Vector*>(at(emitter, 0x11C)), f); // 00496411..0049641C
                c18 = r->x; c1C = r->y; c20 = r->z;                                       // 0049642B..0049644D
                const double om = kOne_0073d764 - W(f);                                   // 00496421..00496427
                c28 = S(om * wf(emitter, 0x54) + W(f) * wf(emitter, 0xD8));               // 00496433..00496453
                c14 = S((om * wf(emitter, 0x90) + W(f) * wf(emitter, 0xE8)) * kHalf_0073e9ac); // 00496457..00496471
                if (wf(emitter, 0x98) == 0.0 && wf(emitter, 0xEC) == 0.0) c24 = c14;     // 00496475..0049649D
                else c24 = S((W(f) * wf(emitter, 0xEC) + om * wf(emitter, 0x98)) * kHalf_0073e9ac); // 004964A7..004964B3 -> 00496270
            } else {                                                                       // 004964B8
                std::memcpy(&c18, at(emitter, 0x188), 4); std::memcpy(&c1C, at(emitter, 0x18C), 4);
                std::memcpy(&c20, at(emitter, 0x190), 4);                                 // 004964BA..004964EE
                c28 = fl(emitter, 0x54);                                                  // 004964D8..004964DB
                c14 = fl(emitter, 0x90);                                                  // 004964E8..004964F2
                if (wf(emitter, 0x98) == 0.0) c24 = c14;                                  // 004964C2..004964F6 -> 00496504
                else c24 = fl(emitter, 0x98);                                             // 004964F8..004964FE
            }
            clamp01(c18); clamp01(c1C); clamp01(c20); clamp01(c28);                       // 0049650C..004965E3
        } else {                                                                           // 004965E8: lifetime < 0
            std::memcpy(&c18, at(emitter, 0x188), 4); std::memcpy(&c1C, at(emitter, 0x18C), 4);
            std::memcpy(&c20, at(emitter, 0x190), 4); c28 = fl(emitter, 0x54);            // 004965E8..0049660F
            clamp01(c18); clamp01(c1C); clamp01(c20); clamp01(c28);                       // 004965FA..004966D1
            c14 = S(wf(emitter, 0x90) * kHalf_0073e9ac);                                  // 004966D9..004966E5
            if (wf(emitter, 0x98) == 0.0 && wf(emitter, 0xEC) == 0.0 && wf(emitter, 0x9C) == 0.0) c24 = c14; // 004966E9..00496726
            else c24 = S(wf(emitter, 0x98) * kHalf_0073e9ac);                             // 0049672C..00496738
        }
        // 0049673C..004967E8: q80 = Quaternion(axis row 2, particle[+14]) * camera orientation (result not read later).
        {
            void* const particle = particle_at(emitter, i);
            const Vector axis = {cE8, cEC, cF0};
            Quaternion q1d0;
            const Quaternion* const q = quaternion_construct_004ac960(&q1d0, nullptr, axis, fl(particle, 0x14)); // 00496745..00496772
            q80 = *q;                                                                     // 00496777..004967A6
            Quaternion q1e0;
            const Quaternion* const m = quaternion_multiply_004a9ac0(&q1e0, &q80, &qd8);  // 00496797..004967B6
            q80 = *m;                                                                     // 004967BB..004967E1
        }
        // 004967E8..00496817: texture cell: row = ftol2(frame * [78]), col = frame - row * [74].
        const I frame = field<I>(particle_at(emitter, i), 0x3C);                          // 004967DB..004967EB
        const I row = ftol2(static_cast<double>(frame) * W(c78));                         // 004967F2..004967FD
        I col = static_cast<I>(static_cast<U>(frame) - static_cast<U>(row) * static_cast<U>(c74)); // 00496810..00496817
        if (W(F(c5C)) != kOne_0073d764) {                                                 // 00496802..00496820 (jnp: equal skips)
            c14 = S(W(c14) * W(F(c5C)));                                                  // 00496822..0049682A
            c24 = S(W(c24) * W(F(c5C)));                                                  // 0049682E..00496836
        }
        pool = emitter_linked_pool_008304b0;                                              // 00496840
        if (i >= field<I>(emitter, 0x1CC) - 1) continue;                                  // 0049683A..00496849
        unsigned char* const quad = static_cast<unsigned char*>(pool) + k60;
        float* const v = reinterpret_cast<float*>(quad);
        // 0049684F..004969F8: segment direction projected on camera rows 0/1 -> strip side vectors.
        const double dz = W(A[1]) - W(A[-2]);                                             // 00496853..00496856
        const double dy = W(A[0]) - W(A[-3]);                                             // 00496859..0049685B
        const double dx = W(A[-1]) - W(A[-4]);                                            // 0049685E..00496861
        float s0 = S(W(cAC) * dx + dy * W(c6C) + dz * W(c68));                            // 00496864..0049687D
        const double s1 = dx * W(cC4) + dy * W(c70) + dz * W(c7C);                        // 00496881..00496896
        const double len = std::sqrt(s1 * s1 + W(s0) * W(s0));                            // 00496898..004968A6
        float c94, c98, c9C, cA0, cA4, cA8;
        if (len > kEpsilon_00743308) {                                                     // 004968A8..004968B3
            const double inv = kOne_0073d764 / len;                                        // 004968B9
            s0 = S(W(s0) * inv);                                                          // 004968BF..004968C5
            const double s1n = s1 * inv;                                                  // 004968C9
            const double a = W(c7C) * W(s0);                                              // 004968CB..004968CF
            const float c34 = S(W(c70) * W(s0));                                          // 004968D3..004968DB
            const float c158 = S(W(cC4) * W(s0));                                         // 004968DF..004968EA
            const double b = W(c68) * s1n;                                                // 004968F1..004968F5
            const float c2C = S(W(c6C) * s1n);                                            // 004968F7..004968FD
            const float c170 = S(W(cAC) * s1n);                                           // 00496901..0049690A
            c9C = S(b - a);                                                               // 00496911, 00496953..00496965
            c98 = S(W(c2C) - W(c34));                                                     // 00496913..00496917, 0049693E..0049694C
            c94 = S(W(c170) - W(c158));                                                   // 0049691B..00496937
            const float c2Cb = S(W(c7C) * s1n);                                           // 00496961..0049696E
            const float c40 = S(W(c70) * s1n);                                            // 00496972..00496978
            const float c140 = S(W(cC4) * s1n);                                           // 0049697C..00496985
            const double m = W(c68) * W(s0);                                              // 0049698E..00496992
            const float c34b = S(W(c6C) * W(s0));                                         // 00496996..0049699E
            const float c17C = S(W(cAC) * W(s0));                                         // 004969A2..004969AD
            cA8 = S(m + W(c2Cb));                                                         // 004969B4, 004969EA..004969F1
            cA4 = S(W(c34b) + W(c40));                                                    // 004969B8..004969BC, 004969DC..004969E3
            cA0 = S(W(c17C) + W(c140));                                                   // 004969C0..004969D5
        } else {                                                                           // 004969FA..00496A36: camera rows 0 / 1
            c94 = cAC; c98 = cB0; c9C = cB4;
            cA0 = cC4; cA4 = cC8; cA8 = cCC;
        }
        // 00496A3D..00496AA4: half extents.
        const double w2 = W(c14) + W(c14);                                                // 00496A3D..00496A48
        const float u10 = S(W(c9C) * w2);                                                 // 00496A51..00496A61
        const float u40 = S(W(c98) * w2);                                                 // 00496A65..00496A6E
        const double P = W(c94) * w2;                                                     // 00496A72 (kept on the x87 stack)
        const double h = W(c24) * kHalf_0073e9ac;                                         // 00496A79..00496A7D
        const float h34 = S(W(cA8) * h);                                                  // 00496A83..00496A8C
        const float h2C = S(W(cA4) * h);                                                  // 00496A90..00496A99
        const float h134 = S(W(cA0) * h);                                                 // 00496A9D..00496AA4
        // 00496AAB..00496B18: packed colour (a<<24 | b<<16 | g<<8 | r), written to the four vertices.
        U rgba = static_cast<U>(ftol2(W(F(cD0)) * W(F(cD4)) * W(c28) * k255_0073ede4));   // 00496AAB..00496AC3
        rgba <<= 8; rgba |= static_cast<U>(ftol2(W(c20) * k255_0073ede4));               // 00496AC8..00496AE8
        rgba <<= 8; rgba |= static_cast<U>(ftol2(W(c1C) * k255_0073ede4));               // 00496ADC..00496AFC
        rgba <<= 8; rgba |= static_cast<U>(ftol2(W(c18) * k255_0073ede4));               // 00496AF0..00496B04
        field<U>(quad, 0x0C) = rgba; field<U>(quad, 0x24) = rgba;                         // 00496B0C..00496B10
        field<U>(quad, 0x3C) = rgba; field<U>(quad, 0x54) = rgba;                         // 00496B14..00496B18
        // vertex 0: shared with the previous quad's vertex 1, else cur - side - width.
        float x0, y0, z0;
        if (static_cast<I>(k60) > 0) {                                                     // 00496B1C..00496B32
            const float* const prev = reinterpret_cast<const float*>(quad - 0x48);
            x0 = prev[0]; y0 = prev[1]; z0 = prev[2];
        } else {                                                                           // 00496B37..00496BA9
            const float c30 = S(W(u40) + W(A[-3]));
            const float c1A0 = S(P + W(A[-4]));
            z0 = S((W(u10) + W(A[-2])) - W(h34));
            y0 = S(W(c30) - W(h2C));
            x0 = S(W(c1A0) - W(h134));
        }
        v[0] = x0; v[1] = y0; v[2] = z0;                                                  // 00496BAD..00496BCD
        const double u1w = static_cast<double>(col + 1) * W(c78);                         // 00496BD2..00496BE0
        v[4] = S(u1w);                                                                    // 00496BE4 fst
        const float c100 = S(kOne_0073d764 - static_cast<double>(row + 1) * W(c58));      // 00496BE8..00496BF6
        v[5] = c100;                                                                      // 00496BFD
        // vertex 1: next + side + width.
        {
            const float c30 = S(W(u40) + W(A[0]));                                        // 00496C08..00496C0E
            const float c188 = S(P + W(A[-1]));                                           // 00496C12..00496C17
            const double zz = W(u10) + W(A[1]);                                           // 00496C01..00496C05
            v[6] = S(W(c188) + W(h134));                                                  // 00496C2A..00496C38
            v[7] = S(W(c30) + W(h2C));                                                    // 00496C22..00496C3C
            v[8] = S(zz + W(h34));                                                        // 00496C1E, 00496C40
        }
        v[10] = S(u1w);                                                                   // 00496C44
        const double v1w = kOne_0073d764 - static_cast<double>(row) * W(c58);             // 00496C48..00496C50
        v[11] = S(v1w);                                                                   // 00496C56 fst
        // vertex 2: next - side + width.
        {
            const double zz = W(A[1]) - W(u10);                                           // 00496C5A..00496C5D
            const float c30 = S(W(A[0]) - W(u40));                                        // 00496C61..00496C67
            const float c164 = S(W(A[-1]) - P);                                           // 00496C6B..00496C70
            v[12] = S(W(c164) + W(h134));                                                 // 00496C83..00496C91
            v[13] = S(W(c30) + W(h2C));                                                   // 00496C7B..00496C95
            v[14] = S(zz + W(h34));                                                       // 00496C77, 00496C99
        }
        const float c38 = S(static_cast<double>(col) * W(c78));                           // 00496C9D..00496CA5
        v[16] = c38;                                                                      // 00496CA9
        v[17] = S(v1w);                                                                   // 00496CAD
        // vertex 3: shared with the previous quad's vertex 2, else cur - side - width (other side).
        float x3, y3, z3;
        if (static_cast<I>(k60) > 0) {                                                     // 00496CB1..00496CC9
            const float* const prev = reinterpret_cast<const float*>(quad - 0x30);
            x3 = prev[0]; y3 = prev[1]; z3 = prev[2];
        } else {                                                                           // 00496CCE..00496D3E
            const double zz = W(A[-2]) - W(u10);
            const float c30 = S(W(A[-3]) - W(u40));
            const float c14C = S(W(A[-4]) - P);
            z3 = S(zz - W(h34));
            y3 = S(W(c30) - W(h2C));
            x3 = S(W(c14C) - W(h134));
        }
        v[18] = x3; v[19] = y3; v[20] = z3;                                               // 00496D42..00496D56
        v[22] = c38;                                                                      // 00496D5A..00496D5E
        v[23] = c100;                                                                     // 00496D62..00496D69
    }
    // 00496DA1..00496DFF: draw and restore.
    *emitter_cells.triangles_00827fb0 = *emitter_cells.triangles_00827fb0 + field<I>(emitter, 0x1CC); // 00496DA1..00496DA7
    kotor_cdecl_00431cc0(static_cast<float*>(emitter_linked_pool_008304b0),
                                           static_cast<I>(static_cast<U>(field<I>(emitter, 0x1CC)) * 4u - 4u)); // 00496DAD..00496DBC
    kotor_renderer_init::glDepthMask_0073d28c(1);                                         // 00496DC4
    kotor_renderer_init::glEnable_0073d390(*cells.gl_lighting_0078d45c);                  // 00496DCC..00496DD8
    kotor_renderer_init::glBlendFunc_0073d38c(kGlSrcAlpha_0073f270, kGlOneMinusSrcAlpha_0073f274); // 00496DDA..00496DE8
    if (fog_was_enabled_90 != 0) kotor_renderer_init::glEnable_0073d390(*cells.gl_fog_0078d450); // 00496DEE..00496DFF
    if (list_b8.data != 0) live_free(as_object(list_b8.data));                            // 00496E01..00496E18
    (void)pool;
}
