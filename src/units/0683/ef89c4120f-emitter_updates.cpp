// lane-3d-emitter emitter_updates.cpp: ExplosionEmitter::Update 00497640 and SingleEmitter::Update 004979C0
// (slot +80 of the lane-3d-model tables 00743500 / 00743588). Not reached by the main-menu emitters (all
// Fountain), authored so the D39 tables link to genuine bodies. Evidence: dumpbin /disasm of swkotor.exe (GOG K1).
// D1 __fastcall transport; D30-FINAL float discipline; unwind maps 007179D6 / 007179F6 cited, not emitted
// (no throwing callee: live_operator_new returns NULL like the original operator new).
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS merged into emitter.h (lane header).

namespace {
using namespace kotor_emitter;
constexpr U kTwoPi_00740c88 = 0x40C90FDBu;      // 6.28319f
constexpr U kMinusTwoPi_00743338 = 0xC0C90FDBu; // -6.28319f
constexpr U kOneBits = 0x3F800000u;
inline PointerList* list_at(void* object, U offset) { return static_cast<PointerList*>(at(object, offset)); }
inline void* item(void* emitter, I index) {
    return as_object(*reinterpret_cast<const volatile U*>(field<U>(emitter, 0x1C8) + static_cast<U>(index) * 4u));
}
// new(78h) ChunkyParticle(emitter, delta) when byte [node+F0] != 0, else new(74h) Particle(emitter, delta).
inline void* new_particle(void* emitter, void* node, U delta_bits) {
    if (field<U8>(node, 0xF0) != 0) {
        void* const block = live_operator_new(0x78);
        return block != nullptr ? chunky_particle_construct_00494cb0(block, nullptr, emitter, delta_bits) : nullptr;
    }
    void* const block = live_operator_new(0x74);
    return block != nullptr ? particle_construct_00494c40(block, nullptr, emitter, delta_bits) : nullptr;
}
// Spin advance and +-2pi wrap shared by both updates (00497761..004977B4 / 00497B12..00497B63).
inline void spin(void* emitter, void* particle, float delta) {
    field<float>(particle, 0x14) = S(W(delta) * W(field<float>(emitter, 0x88)) + W(field<float>(particle, 0x14)));
    const double angle = W(field<float>(particle, 0x14));
    if (angle > W(F(kTwoPi_00740c88))) {                                    // fcomp 2pi; test ah,41h; jne
        field<float>(particle, 0x14) = S(angle - W(F(kTwoPi_00740c88)));
    } else if (angle < W(F(kMinusTwoPi_00743338))) {                        // fcomp -2pi; test ah,5; jp
        field<float>(particle, 0x14) = S(W(field<float>(particle, 0x14)) + W(F(kTwoPi_00740c88)));
    }
}
// Motion-blur fraction (t = min(delta, [+B4]) / [+B4]; ties/unordered take [+B4]).
inline double blur_fraction(void* emitter, float delta) {
    const double limit = W(field<float>(emitter, 0xB4));
    const double numerator = (W(delta) < limit) ? W(delta) : limit;           // fcomp; test ah,5; jp
    return numerator / limit;
}
}

extern "C" {

// 00497640 ExplosionEmitter::Update(delta), RET 4.
U __fastcall explosion_emitter_update_00497640(void* self, void*, U delta_bits) {
    const float delta = F(delta_bits);
    I spawn = ftol2(W(field<float>(self, 0x58)));                           // 0049765D..00497666
    void* const node = field<void*>(self, 0x4);                             // 00497668..0049766B
    U scale_bits = kOneBits;                                                // 00497674
    if (field<void*>(self, 0x40) != nullptr)                                // 0049766F..0049767C
        scale_bits = field<U>(field<void*>(self, 0x40), 0x1B4);             // 0049767E..00497684
    part_emitter_chk_particle_life_00494b30(self, nullptr, delta);          // 00497688..0049768F
    const U motion_blur = emitter_render_motion_blur_00798b8c;              // 00497694
    I index = field<I>(self, 0x1CC);                                        // 004976A0
    float t = 0.0f;  // D2-style: [esp+10] is written only in motion-blur mode (004976CB) and read only there.
    if (field<U>(self, 0x114) == motion_blur)                               // 0049769A..004976A6
        t = S(blur_fraction(self, delta));                                  // 004976A8..004976CB
    if (index > 0) {                                                        // 004976CF..004976D1
        do {
            const U mode = field<U>(self, 0x114);                           // 004976D7
            --index;                                                        // 004976DD
            if (mode == motion_blur) {                                      // 004976DE..004976E0
                void* const particle = item(self, index);                   // 004976E2..004976F2
                float* const previous = &field<float>(particle, 0x60);      // 004976F5
                const double u = 1.0 - W(t);                                // 004976E8..004976EE
                const float uz = S(u * W(previous[2]));                     // 004976F8..004976FD
                const float uy = S(u * W(previous[1]));                     // 00497701..00497706
                const float ux = S(u * W(previous[0]));                     // 0049770A..0049770C
                const double tz = W(t) * W(field<float>(particle, 0x10));   // 00497710..00497714
                const float ty = S(W(t) * W(field<float>(particle, 0x0C))); // 00497717..0049771E
                const float tx = S(W(t) * W(field<float>(particle, 0x08))); // 00497722..00497729
                const double z = tz + W(uz);                                // 0049772D
                const double y = W(ty) + W(uy);                             // 00497731..00497735
                previous[0] = S(W(tx) + W(ux));                             // 00497739..00497749
                previous[1] = S(y);                                         // 0049774B..00497753
                previous[2] = S(z);                                         // 00497756..0049775E
            }
            spin(self, item(self, index), delta);                           // 00497761..004977B4
        } while (index > 0);                                                // 004977B7..004977B9
    }
    part_emitter_move_func_00494d40(self, nullptr, delta);                  // 004977BF..004977C2
    if (spawn != 0) {                                                       // 004977C7..004977C9
        if (field<U8>(self, 0x1F6) == 0) {                                  // 004977CF..004977D7
            field<float>(self, 0xF4) = 0.0f;                                // 00497996..004979A0
            field<U8>(self, 0x1F6) = 0;                                     // 004979A7
            return 0;   // EAX is not defined by the original here (callers through vtbl+80 ignore it)
        }
        if (W(field<float>(self, 0xF4)) < W(F(kOneBits))) {                 // 004977DD..004977EE (test ah,5; jp)
            if (W(field<float>(node, 0x54)) != 0.0) {                       // 004977F8..00497806 (test ah,44h; jnp)
                float position[3] = {0.0f, 0.0f, 0.0f};                     // 00497816..00497826
                float orientation[4] = {F(kOneBits), 0.0f, 0.0f, 0.0f};     // 0049782E..00497846
                part_table(self)->s00c(self, nullptr, word_of(position), word_of(orientation)); // 00497808..0049784E
                void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;        // 00497874
                Vector at_world;
                std::memcpy(&at_world, position, sizeof(at_world));                     // 0049785D..00497879
                wind_manager_add_point_source_wind_004a6510(field<void*>(scene, 0x84), nullptr, at_world,
                    field<U>(node, 0x54), field<U>(node, 0x58), F(kOneBits));        // 00497851..00497882
            }
            for (; spawn > 0; ) {                                           // 00497887..00497889
                void* particle;
                U second;
                if (field<I>(self, 0x1D8) > 0) {                            // 00497895..0049789D
                    const I last = field<I>(self, 0x1D8) - 1;
                    particle = as_object(*reinterpret_cast<const volatile U*>(field<U>(self, 0x1D4) + static_cast<U>(last) * 4u)); // 0049789F..004978A5
                    kotor_renderer_gl::deleteAt_006857a0(static_cast<kotor_reconstruction::ArrayMutations*>(at(self, 0x1D4)), nullptr, last); // 004978A9..004978B1
                    second = kOneBits;                                      // 004978BA
                } else {
                    particle = new_particle(self, node, delta_bits);        // 004978C2..00497922
                    second = scale_bits;                                    // 00497924..0049792C
                }
                // A failed allocation leaves particle == NULL and the original dereferences it at 00497938.
                particle_table(particle)->s08_initialize(particle, nullptr, delta_bits, second); // 00497938..0049793C
                PointerList* const list = list_at(self, 0x1C8);             // 0049788F
                if (list->size == list->capacity) {                         // 0049793F..00497945
                    const I capacity = list->capacity;
                    menu_list_growth::allocate_005e03c0(list, nullptr, capacity != 0 ? static_cast<U>(capacity + capacity) : 8u); // 00497947..00497957
                }
                *reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(list->data) + static_cast<U>(list->size) * 4u) = word_of(particle); // 0049795C..00497962
                list->size = list->size + 1;                                // 00497965
                part_emitter_chk_random_0048dcf0(self, nullptr, item(self, field<I>(self, 0x1CC) - 1)); // 00497968..00497978
                if (--spawn == 0) break;                                    // 0049797D..0049797E
            }
        }
    }
    field<float>(self, 0xF4) = (field<U8>(self, 0x1F6) != 0) ? F(kOneBits) : 0.0f;   // 00497984..004979A0
    field<U8>(self, 0x1F6) = 0;                                             // 004979A7
    return 0;   // EAX after the epilogue is not a defined value; callers ignore it
}

// 004979C0 SingleEmitter::Update(delta), RET 4.
U __fastcall single_emitter_update_004979c0(void* self, void*, U delta_bits) {
    const float delta = F(delta_bits);
    void* const node = field<void*>(self, 0x4);                             // 004979DF
    const I spawn = ftol2(W(field<float>(self, 0x58)));                     // 004979DB..004979E2
    const I count = field<I>(self, 0x1CC);                                  // 004979E9
    if (spawn == 0) {                                                       // 004979E7..004979EF
        if (count == 0) return 0;                                           // 00497C26..00497C28
        void* const first = item(self, 0);                                  // 00497C2A..00497C36
        kotor_renderer_gl::deleteAt_006857a0(static_cast<kotor_reconstruction::ArrayMutations*>(at(self, 0x1C8)), nullptr, 0); // 00497C30..00497C3A
        if (first != nullptr) particle_table(first)->s00_deleting_destructor(first, nullptr, 1); // 00497C3F..00497C49
        return 0;
    }
    if (count == 0) {                                                       // 004979F5..004979F7
        void* const particle = new_particle(self, node, delta_bits);        // 004979FD..00497A59
        int_list_add_00444fe0(list_at(self, 0x1C8), nullptr, word_of(particle)); // 00497A5B..00497A6C
        part_emitter_chk_random_0048dcf0(self, nullptr, item(self, 0));     // 00497A71..00497A78
        return 0;
    }
    void* particle = item(self, 0);                                         // 00497A90..00497A9A
    field<float>(particle, 0x70) = S(W(delta) + W(field<float>(particle, 0x70))); // 00497A96..00497A9F
    if (W(field<float>(self, 0x78)) > 0.0) {                                // 00497AA2..00497AB0 (test ah,41h; jne)
        particle = item(self, 0);                                           // 00497AB2..00497AB8
        if (W(field<float>(particle, 0x70)) > W(field<float>(self, 0x78)) && // 00497ABA..00497AC5
            field<U>(node, 0x104) != 0)                                     // 00497AC7..00497ACF
            field<U>(particle, 0x70) = 0;                                   // 00497AD1
    } else {
        particle = item(self, 0);                                           // 00497ADA..00497AE0
        if (W(field<float>(particle, 0x70)) < 0.0) {                        // 00497AE2..00497AF0 (test ah,5; jp)
            if (W(field<float>(self, 0x78)) > 0.0) field<U>(particle, 0x70) = 0; // 00497AF2..00497B02
            else field<U>(particle, 0x70) = delta_bits;                     // 00497B0B..00497B0F
        }
    }
    spin(self, item(self, 0), delta);                                       // 00497B12..00497B63
    if (field<U>(self, 0x114) == emitter_render_motion_blur_00798b8c) {     // 00497B66..00497B72
        const double t = blur_fraction(self, delta);                        // 00497B78..00497B95
        void* const first = item(self, 0);                                  // 00497B9B..00497BA1
        float* const previous = &field<float>(first, 0x60);                 // 00497BA3
        const double u = 1.0 - t;                                           // 00497BA6..00497BAC
        const float ux = S(u * W(previous[0]));                             // 00497BAE..00497BB2
        const float uy = S(u * W(previous[1]));                             // 00497BB6..00497BBB
        const double uz = u * W(previous[2]);                               // 00497BBF
        const double tx = t * W(field<float>(first, 0x08));                 // 00497BC2..00497BC4
        const double ty = t * W(field<float>(first, 0x0C));                 // 00497BC7..00497BC9
        const float tz = S(t * W(field<float>(first, 0x10)));               // 00497BCC..00497BD1
        previous[0] = S(tx + W(ux));                                        // 00497BD5..00497BE3
        previous[1] = S(ty + W(uy));                                        // 00497BE1..00497BF7
        previous[2] = S(W(tz) + uz);                                        // 00497BED..00497C04
    }
    part_emitter_move_func_00494d40(self, nullptr, delta);                  // 00497C07..00497C0E
    return 0;   // EAX after MoveFunc is not a defined value; callers through vtbl+80 ignore it
}

} // extern "C"
