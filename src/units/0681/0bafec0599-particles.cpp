// particles.cpp - lane-3d-emitter fork F5: FountainEmitter::Update 00496E40 and the Particle / ChunkyParticle
// lifetime it drives (ctor/dtor/vtables, chkParticleLife, chkRandom, MoveFunc).
// Evidence: dumpbin /disasm of the pinned GOG swkotor.exe; instruction addresses cited per behaviour.
// D30-FINAL: x87 intermediates are double (W), float only at the original `fstp dword` points (S).
// EH: unwind maps 007179AC (00496E40) and 007178F8 (00494CB0 / 0048DC30) are cited, not emitted (operator new
// returns NULL in the original; no throwing callee on these paths).
#include "4ac9e30747-emitter.h"

using namespace kotor_emitter;

// LOCAL-DECLS merged into emitter.h (lane header).

namespace {
constexpr U kOne = 0x3F800000u;               // 1.0f immediate (00494C90, 00496E75)
constexpr U kTwoPi_00740c88 = 0x40C90FDBu;    // .rdata 6.283185f
constexpr U kMinusTwoPi_00743338 = 0xC0C90FDBu; // .rdata -6.283185f

inline PointerList* list_at(void* object, U offset) { return static_cast<PointerList*>(at(object, offset)); }
inline void** list_data(PointerList* list) { return reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data)); }
// Inline CExoArrayList<T*>::Add (e.g. 004971AD..004971E0 / 00494BBC..00494BF9): grow to capacity*2 (8 when 0)
// through 005E03C0 when size == capacity, then data[size++] = value.
inline void list_add(PointerList* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity) {
        menu_list_growth::allocate_005e03c0(list, nullptr, capacity != 0 ? static_cast<U>(capacity + capacity) : 8u);
    }
    list_data(list)[list->size] = value;
    list->size = list->size + 1;
}
// new(78h) ChunkyParticle(emitter, delta) when the node names a chunk model (byte [node+F0] != 0), else
// new(74h) Particle(emitter, delta) (00497116..00497193 / 00497506..00497587); NULL when operator new fails.
inline void* new_particle(void* emitter, void* node, U delta_bits) {
    if (field<U8>(node, 0xF0) != 0) {
        void* const block = live_operator_new(0x78);
        return block != nullptr ? chunky_particle_construct_00494cb0(block, nullptr, emitter, delta_bits) : nullptr;
    }
    void* const block = live_operator_new(0x74);
    return block != nullptr ? particle_construct_00494c40(block, nullptr, emitter, delta_bits) : nullptr;
}

// Lane-private typed bodies for the shared one-instruction slots (D24).
U __fastcall particle_slot_return_zero_0063e7f0(void*, void*) { return 0; }            // 0063E7F0: xor eax,eax; ret
U __fastcall particle_slot_self_return_00641db0(void* self, void*) { return word_of(self); } // 00641DB0: mov eax,ecx; ret
} // namespace

extern "C" {
// ---------------------------------------------------------------- vtable data 007432D8 / 007432E4
const ParticleVTable particle_vtable_007432d8 = {
    &particle_deleting_destructor_0048de30,       // +00 0048DE30
    &particle_slot_return_zero_0063e7f0,          // +04 0063E7F0
    &particle_initialize_004921d0,                // +08 004921D0
};
const ParticleVTable chunky_particle_vtable_007432e4 = {
    &chunky_particle_deleting_destructor_0048e190, // +00 0048E190
    &particle_slot_self_return_00641db0,           // +04 00641DB0
    &chunky_particle_initialize_00492400,          // +08 00492400
};

// ---------------------------------------------------------------- 00494C40 Particle::Particle(emitter, a2), RET 8
void* __fastcall particle_construct_00494c40(void* self, void*, void* emitter, U delta_bits) {
    field<const void*>(self, 0x00) = &particle_vtable_007432d8;                 // 00494C49
    field<U>(self, 0x08) = 0; field<U>(self, 0x0C) = 0; field<U>(self, 0x10) = 0; // 00494C4F..00494C55
    field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0; field<U>(self, 0x24) = 0; // 00494C58..00494C5E
    field<U>(self, 0x38) = 0; field<U>(self, 0x34) = 0; field<U>(self, 0x30) = 0; // 00494C61..00494C67
    field<U>(self, 0x2C) = kOne;                                                // 00494C6A
    field<U>(self, 0x48) = 0; field<U>(self, 0x4C) = 0; field<U>(self, 0x50) = 0; // 00494C71..00494C77
    field<U>(self, 0x54) = 0; field<U>(self, 0x58) = 0; field<U>(self, 0x5C) = 0; // 00494C7A..00494C80
    field<U>(self, 0x60) = 0; field<U>(self, 0x64) = 0; field<U>(self, 0x68) = 0; // 00494C83..00494C89
    field<void*>(self, 0x04) = emitter;                                         // 00494C98
    particle_initialize_004921d0(self, nullptr, delta_bits, kOne);              // 00494C90..00494C9B (direct, non-virtual)
    particle_count_0083046c = particle_count_0083046c + 1;                     // 00494CA0 inc dword [0083046C]
    return self;                                                                // 00494CA6
}

// ---------------------------------------------------------------- 00494CB0 ChunkyParticle::ChunkyParticle(emitter, a2), RET 8
void* __fastcall chunky_particle_construct_00494cb0(void* self, void*, void* emitter, U delta_bits) {
    particle_construct_00494c40(self, nullptr, emitter, delta_bits);           // 00494CD0..00494CD8
    field<const void*>(self, 0x00) = &chunky_particle_vtable_007432e4;          // 00494CE7 (EH state 0 = ~Particle, cited)
    if (emitter != nullptr) {                                                   // 00494CDD / 00494CED
        void* const node = field<void*>(emitter, 0x04);                         // 00494CEF
        // NewCAurObject(node+F0 chunk name, "" (.rdata 0073D71C, empty string), 0, 0) (00494CF2..00494D02)
        aurora_model::Gob* const gob = aurora_new_aur_object_00449cc0(static_cast<const char*>(at(node, 0xF0)), "", nullptr, nullptr);
        field<void*>(self, 0x74) = gob;                                         // 00494D0C
        if (gob != nullptr) {                                                   // 00494D0F
            void* const scene = *kotor_scene3d::cells.active_scene_007b92a0;    // 00494D11 [007B92A0]
            gob_table(gob)->attach_to_scene(gob, nullptr, static_cast<aurora_model::Scene*>(scene)); // 00494D17..00494D1C vtbl+4C
        }
    }
    return self;                                                                // 00494D24
}

// ---------------------------------------------------------------- destructors
// 0048D860 Particle::~Particle
void __fastcall particle_destruct_0048d860(void* self, void*) {
    field<const void*>(self, 0x00) = &particle_vtable_007432d8;                 // 0048D860
    particle_count_0083046c = particle_count_0083046c - 1;                     // 0048D866
}
// 0048DE30 Particle deleting destructor (vtbl+0), RET 4
U __fastcall particle_deleting_destructor_0048de30(void* self, void*, U flags) {
    field<const void*>(self, 0x00) = &particle_vtable_007432d8;                 // 0048DE37
    particle_count_0083046c = particle_count_0083046c - 1;                     // 0048DE3D..0048DE46
    if ((flags & 1u) != 0) live_free(self);                                     // 0048DE44 test al,1 / 0048DE4F
    return word_of(self);                                                       // 0048DE57
}
// 0048DC30 ChunkyParticle::~ChunkyParticle
void __fastcall chunky_particle_destruct_0048dc30(void* self, void*) {
    field<const void*>(self, 0x00) = &chunky_particle_vtable_007432e4;          // 0048DC4D
    void* const gob = field<void*>(self, 0x74);                                 // 0048DC53
    if (gob != nullptr) {                                                       // 0048DC56
        gob_table(gob)->s000_gob_destruct_gob(as_gob(gob), nullptr, 1);         // 0048DC62..0048DC66 vtbl+0 (push 1)
        field<void*>(self, 0x74) = nullptr;                                     // 0048DC68
    }
    field<const void*>(self, 0x00) = &particle_vtable_007432d8;                 // 0048DC73 (inlined ~Particle)
    particle_count_0083046c = particle_count_0083046c - 1;                     // 0048DC79
}
// 0048E190 ChunkyParticle deleting destructor (vtbl+0), RET 4
U __fastcall chunky_particle_deleting_destructor_0048e190(void* self, void*, U flags) {
    chunky_particle_destruct_0048dc30(self, nullptr);                          // 0048E193
    if ((flags & 1u) != 0) live_free(self);                                     // 0048E198..0048E1A0
    return word_of(self);                                                       // 0048E1A8
}

// ---------------------------------------------------------------- 0048DCF0 PartEmitter::chkRandom(particle), RET 4
void __fastcall part_emitter_chk_random_0048dcf0(void* self, void*, void* particle) {
    void* const node = field<void*>(self, 0x04);                                // 0048DCF4
    if (((field<U>(node, 0x12C) >> 5) & 1u) == 0) return;                       // 0048DCF7..0048DD03 (Random flag)
    U n = static_cast<U>(ftol2(W(field<float>(self, 0x6C)) - W(field<float>(self, 0x70)))) + 1u; // 0048DD05..0048DD13
    if (static_cast<I>(n) < 0) n = 0u - n;                                      // 0048DD14 jns / 0048DD16 neg
    if (n == 0) return;                                                         // 0048DD18..0048DD1A
    const I r = crt_rand_006fc447() % static_cast<I>(n);                        // 0048DD1C..0048DD22 cdq; idiv
    field<I>(particle, 0x3C) = ftol2(static_cast<double>(r) + W(field<float>(self, 0x70))); // 0048DD24..0048DD38
}

// ---------------------------------------------------------------- 00494B30 PartEmitter::chkParticleLife(delta), RET 4
void __fastcall part_emitter_chk_particle_life_00494b30(void* self, void*, float delta) {
    const bool chunky = field<U8>(field<void*>(self, 0x04), 0xF0) != 0;         // 00494B35..00494B47
    for (I index = field<I>(self, 0x1CC) - 1; index >= 0; --index) {            // 00494B4C..00494B53 / 00494C2D..00494C2E
        PointerList* const live = list_at(self, 0x1C8);
        const double lifetime = W(field<float>(self, 0x78));                    // 00494B60
        void* const particle = list_data(live)[index];                          // 00494B63 / 00494B6F
        if (!x87_less(lifetime, 0.0) &&                                         // 00494B69..00494B77 (test ah,1; jne)
            !x87_less(W(field<float>(particle, 0x70)), lifetime)) {             // 00494B7D..00494B88
            // expired: remove at index (inline shift, 00494B8E..00494BBA)
            live->size = live->size - 1;
            for (I k = index; k < live->size; ++k) list_data(live)[k] = list_data(live)[k + 1];
            list_add(list_at(self, 0x1D4), particle);                           // 00494BBC..00494BF9 dead-particle pool
            if (chunky) {                                                       // 00494BF2..00494BFC
                void* const gob = field<void*>(particle, 0x74);                 // 00494BFE
                if (gob != nullptr) gob_table(gob)->attach_to_scene(as_gob(gob), nullptr, nullptr); // 00494C05..00494C09 vtbl+4C (push 0)
            }
            continue;
        }
        const double age = W(delta) + W(field<float>(particle, 0x70));          // 00494C0E..00494C12
        field<float>(particle, 0x70) = S(age);                                  // 00494C15 fst
        if (age < 0.0) {                                                        // 00494C18..00494C23 (test ah,5; jp)
            field<U>(particle, 0x70) = 0;                                       // 00494C27
            field<U>(particle, 0x40) = 0;                                       // 00494C2A
        }
    }
}

// ---------------------------------------------------------------- 00494D40 PartEmitter::MoveFunc(delta), RET 4
void __fastcall part_emitter_move_func_00494d40(void* self, void*, float delta) {
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 0x04), nullptr); // 00494D43..00494D46
    if ((field<U8>(node, 0x12C) & 1u) != 0) {                                  // 00494D4B (P2P)
        if (field<void*>(self, 0x1E4) == nullptr) return;                       // 00494D54..00494D5C
        if (((field<U>(node, 0x12C) >> 1) & 1u) != 0)                           // 00494D5E..00494D68 (P2P_Sel)
            part_emitter_move_particle_p2p_bezier_00492ea0(self, nullptr, delta);  // 00494D71
        else
            part_emitter_move_particle_p2p_grav_00493ae0(self, nullptr, delta);    // 00494D81
        return;
    }
    part_emitter_move_particle_standard_00492450(self, nullptr, delta);         // 00494D91
}

// ---------------------------------------------------------------- 00496E40 FountainEmitter::Update(delta) (vtbl+80), RET 4
// Return: EAX is not consumed by the only caller (AnimateParticles 00495B01, void tail); 0 is returned.
U __fastcall fountain_emitter_update_00496e40(void* self, void*, U delta_bits) {
    const float delta = F(delta_bits);
    const float rate = field<float>(self, 0x58);                                // 00496E5F/00496E66 entry copy [E+4]
    void* const node = field<void*>(self, 0x04);                                // 00496E63
    U scale_bits = kOne;                                                        // 00496E75
    void* const owner = field<void*>(self, 0x40);                               // 00496E6A
    if (owner != nullptr) scale_bits = field<U>(owner, 0x1B4);                  // 00496E7F..00496E85
    part_emitter_chk_particle_life_00494b30(self, nullptr, delta);             // 00496E89..00496E91

    const U motion_blur = emitter_render_motion_blur_00798b8c;                  // 00496E9A (loaded once)
    // D2-style: [esp+14h] (t) has no store unless the mode is Motion_Blur, and it is read only in that mode.
    float t = 0.0f;
    if (field<U>(self, 0x114) == motion_blur) {                                 // 00496EA0
        const double blur_time = W(field<float>(self, 0xB4));
        const double clipped = (W(delta) < blur_time) ? W(delta) : blur_time;   // 00496EA8..00496ECB (test ah,5; jp)
        t = S(clipped / blur_time);                                             // 00496ECB..00496ED1
    }
    for (I index = field<I>(self, 0x1CC); index > 0;) {                         // 00496ED5..00496EDD / 00496FC3..00496FC9
        void* const particle = list_data(list_at(self, 0x1C8))[index - 1];      // 00496EF6..00496EFC
        --index;                                                                // 00496F00
        if (field<U>(self, 0x114) == motion_blur) {                             // 00496EF0 / 00496F01
            const double a = 1.0 - W(t);                                        // 00496F05..00496F0B
            const float a68 = S(a * W(field<float>(particle, 0x68)));           // 00496F0F..00496F14
            const float a64 = S(a * W(field<float>(particle, 0x64)));           // 00496F18..00496F1D
            const float a60 = S(a * W(field<float>(particle, 0x60)));           // 00496F21..00496F24
            const double t10 = W(t) * W(field<float>(particle, 0x10));          // 00496F28..00496F2C (kept wide)
            const float t0c = S(W(t) * W(field<float>(particle, 0x0C)));        // 00496F2F..00496F36
            const float t08 = S(W(t) * W(field<float>(particle, 0x08)));        // 00496F3A..00496F41
            const double z = t10 + W(a68);                                      // 00496F45
            const double y = W(t0c) + W(a64);                                   // 00496F49..00496F4D
            field<float>(particle, 0x60) = S(W(t08) + W(a60));                  // 00496F51..00496F61
            field<float>(particle, 0x64) = S(y);                                // 00496F64..00496F6C
            field<float>(particle, 0x68) = S(z);                                // 00496F6F..00496F77
        }
        const double spin = W(delta) * W(field<float>(self, 0x88)) + W(field<float>(particle, 0x14)); // 00496F7E..00496F8B
        field<float>(particle, 0x14) = S(spin);                                 // 00496F94 fst
        if (spin > W(F(kTwoPi_00740c88))) {                                     // 00496F8E..00496F9C (test ah,41h; jne)
            field<float>(particle, 0x14) = S(spin - W(F(kTwoPi_00740c88)));     // 00496F9E..00496FA4
        } else if (spin < W(F(kMinusTwoPi_00743338))) {                         // 00496FA9..00496FB4 (test ah,5; jp)
            field<float>(particle, 0x14) = S(spin + W(F(kTwoPi_00740c88)));     // 00496FB6..00496FBC
        }
    }
    part_emitter_move_func_00494d40(self, nullptr, delta);                      // 00496FCF..00496FD7
    if (ftol2(W(rate)) <= 0) return 0;                                          // 00496FDC..00496FE7
    const float inverse_rate = S(1.0 / W(rate));                                // 00496FED..00496FFD [esp+30h]

    if (field<I>(node, 0x6C) != 0) {                                            // 00496FF3..00497001
        // ---- spawn along the segment travelled since the last frame
        const double dz = W(field<float>(self, 0x16C)) - W(field<float>(self, 0x178)); // 0049700B..00497011
        const double dy = W(field<float>(self, 0x168)) - W(field<float>(self, 0x174)); // 00497017..0049701D
        const float dx = S(W(field<float>(self, 0x164)) - W(field<float>(self, 0x170))); // 00497023..0049702F
        const float vz = S(dz + W(field<float>(self, 0x154)));                  // 00497035..0049703B
        const float vy = S(dy + W(field<float>(self, 0x150)));                  // 0049703F..00497045
        const float vx = S(W(dx) + W(field<float>(self, 0x14C)));               // 00497049..00497053
        const double length = std::sqrt(W(vy) * W(vy) + W(vz) * W(vz) + W(vx) * W(vx)); // 00497057..00497073
        const float length_f = S(length);                                       // 00497075 fst [esp+18h]
        const double inverse = (length > 0.0) ? 1.0 / W(length_f) : 0.0;         // 00497079..00497098 (test ah,41h; jne)
        const float nz = S(W(vz) * inverse);                                    // 00497098..0049709E
        const float ny = S(W(vy) * inverse);                                    // 004970A2..004970A8
        const float nx = S(inverse * W(vx));                                    // 004970AC..004970B0
        const I count = ftol2(W(length_f) * W(rate));                           // 004970B4..004970C5
        for (I spawned = 0; spawned < count; ++spawned) {                       // 004970C1..004970C9 / 0049737F..00497386
            PointerList* const dead = list_at(self, 0x1D4);
            void* particle;
            I last = dead->size;                                                // 004970D5
            if (last > 0) {                                                     // 004970DB..004970DD
                particle = list_data(dead)[last - 1];                           // 004970DF..004970E5
                --last;                                                         // 004970F2
                dead->size = dead->size - 1;                                    // 004970EF..004970F8
                for (I k = last; k < dead->size; ++k) list_data(dead)[k] = list_data(dead)[k + 1]; // 004970FB..00497112
            } else {
                particle = new_particle(self, node, delta_bits);                // 00497116..00497193 (EH states 0/1 cited)
            }
            particle_table(particle)->s08_initialize(particle, nullptr, delta_bits, scale_bits); // 00497195..004971A6 vtbl+8
            list_add(list_at(self, 0x1C8), particle);                           // 004971A9..004971E0
            void* const newest = list_data(list_at(self, 0x1C8))[field<I>(self, 0x1CC) - 1]; // 004971E7..004971F3
            part_emitter_chk_random_0048dcf0(self, nullptr, newest);            // 004971F7..0049723A (inlined chkRandom)
            if (field<float>(self, 0xAC) == 0.0f && field<float>(self, 0xB0) == 0.0f) { // 00497241..00497269 (test ah,44h; jp)
                const I live_count = field<I>(self, 0x1CC);                     // 0049726F
                if (live_count > 1) {                                           // 00497275..00497278
                    void* const p = list_data(list_at(self, 0x1C8))[live_count - 1]; // 00497289..0049728F
                    const double f = static_cast<double>(spawned + 1) * W(inverse_rate); // 0049727E..00497293
                    const double xf = W(nx) * f;                                // 0049729A..0049729E
                    const float yf = S(f * W(ny));                              // 004972A0..004972A6
                    const float zf = S(f * W(nz));                              // 004972AA..004972B0
                    const double px = W(field<float>(self, 0x170)) - W(field<float>(self, 0x14C)); // 004972B4..004972BA
                    const float py = S(W(field<float>(self, 0x174)) - W(field<float>(self, 0x150))); // 004972C0..004972CC
                    const float pz = S(W(field<float>(self, 0x178)) - W(field<float>(self, 0x154))); // 004972D0..004972DC
                    field<float>(p, 0x08) = S(px + xf);                         // 004972E0..004972EA
                    const float y = S(W(py) + W(yf));                           // 004972EE..004972F6
                    const float z = S(W(pz) + W(zf));                           // 004972FA..00497306
                    field<float>(p, 0x0C) = y;                                  // 00497375
                    field<float>(p, 0x10) = z;                                  // 00497378
                } else {
                    void* const p = list_data(list_at(self, 0x1C8))[0];         // 00497314..0049731E (element 0)
                    const double lx = W(nx) * W(length_f);                      // 00497310..0049731A
                    const double ly = W(ny) * W(length_f);                      // 00497320..00497327
                    const float lz = S(W(nz) * W(length_f));                    // 0049732B..00497333
                    const float x = S(W(field<float>(self, 0x164)) - lx);       // 0049733A..00497342
                    const float y = S(W(field<float>(self, 0x168)) - ly);       // 0049734A..00497352
                    const float z = S(W(field<float>(self, 0x16C)) - W(lz));    // 0049735E..0049736D
                    field<float>(p, 0x08) = x;                                  // 00497364
                    field<float>(p, 0x0C) = y;                                  // 00497375
                    field<float>(p, 0x10) = z;                                  // 00497378
                }
            }
        }
        const double f = static_cast<double>(count) * W(inverse_rate);          // 0049738C..00497396
        const double xf = W(nx) * f;                                            // 0049739A..0049739E
        const float yf = S(f * W(ny));                                          // 004973A0..004973A6
        const float zf = S(f * W(nz));                                          // 004973AA..004973B0
        field<float>(self, 0x14C) = S(W(vx) - xf);                              // 004973B7..004973C5
        field<float>(self, 0x150) = S(W(vy) - W(yf));                           // 004973C9..004973E4
        field<float>(self, 0x154) = S(W(vz) - W(zf));                           // 004973D5..004973EF
        return 0;
    }

    // ---- time-based birth (node[+6C] == 0)
    field<float>(self, 0xCC) = S(W(delta) + W(field<float>(self, 0xCC)));       // 004973F7..00497408
    if (x87_less(W(field<float>(self, 0xCC)), W(inverse_rate))) return 0;       // 0049740E..00497421 (test ah,1; jne)
    float birth = rate;                                                         // [esp+14h] (entry copy of +58)
    const I spread = fistp_nearest(W(field<float>(self, 0xF0)));               // 00497427..00497443 frndint/fistp
    if (spread > 0) {                                                           // 00497445..0049744B
        if (crt_rand_006fc447() % 2 != 0) {                                     // 0049744D..0049745E (and 80000001h idiom)
            const I r = crt_rand_006fc447() % spread;                           // 00497460..00497466
            birth = S(static_cast<double>(r) + W(birth));                       // 0049746E..00497472 / 0049748E
        } else {
            const I r = crt_rand_006fc447() % spread;                           // 00497478..0049747E
            birth = S(W(birth) - static_cast<double>(r));                       // 00497486..0049748E (fsubr)
        }
        if (birth < 0.0f) birth = 0.0f;                                         // 00497492..004974A3 (test ah,5; jp)
    }
    const I modulus = ftol2(W(birth)) + 1;                                      // 004974AB..004974C4 (lea ebx,[eax+1])
    I due = ftol2(W(birth) * W(field<float>(self, 0xCC))) % modulus;            // 004974B6..004974CD
    for (I spawned = 0; spawned < due;) {                                       // 004974CF..004974D1 / 00497608..0049760A
        PointerList* const dead = list_at(self, 0x1D4);
        void* particle;
        const I last = dead->size;                                              // 004974E0
        if (last > 0) {                                                         // 004974E6..004974E8
            particle = list_data(dead)[last - 1];                               // 004974EA..004974F0
            kotor_renderer_gl::deleteAt_006857a0(dead, nullptr, last - 1);      // 004974F4..004974FC
        } else {
            particle = new_particle(self, node, delta_bits);                    // 00497506..00497587 (EH states 2/3 cited)
        }
        particle_table(particle)->s08_initialize(particle, nullptr, delta_bits, scale_bits); // 00497589..0049759A vtbl+8
        list_add(list_at(self, 0x1C8), particle);                               // 0049759D..004975D4
        void* const newest = list_data(list_at(self, 0x1C8))[field<I>(self, 0x1CC) - 1]; // 004975D7..004975E7
        part_emitter_chk_random_0048dcf0(self, nullptr, newest);                // 004975EB..004975EC
        ++spawned;                                                              // 004975FF
        due = ftol2(W(birth) * W(field<float>(self, 0xCC))) % modulus;          // 004975F1..00497606
    }
    field<U>(self, 0xCC) = 0;                                                   // 00497610
    return 0;
}
} // extern "C"
