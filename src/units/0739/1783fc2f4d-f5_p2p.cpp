// f5_p2p.cpp - lane-m46-3dmodel fork F5: the two point-to-point particle movers of swkotor.exe (GOG K1, base 00400000)
//   00492EA0 PartEmitter::moveParticleP2PBezier(dt)  thiscall RET 4 (MoveFunc 00494D40 -> 00494D71 when P2P and P2P_Sel)
//   00493AE0 PartEmitter::moveParticleP2PGrav(dt)    thiscall RET 4 (MoveFunc 00494D40 -> 00494D81 when P2P, P2P_Sel clear)
// Consumer: M45 stage 3d/emitter/particles.cpp (MoveFunc) through the emitter.h declarations (D1 fastcall, nullptr EDX).
// Evidence: dumpbin /disasm of the pinned swkotor.exe (lane-sound/full.asm; lane-m46-3dmodel/d/00492EA0.txt, d/00493AE0.txt);
// .rdata constants read from the pinned image (tools/pe.py). Instruction addresses are cited per behaviour.
// Stack-frame names: [L..] (bezier) and [F..] (grav) = the original's esp-relative locals with every in-flight push removed.
// Float discipline (D30-FINAL): x87 stack values are double; float (S()) only at the original `fstp dword` stores; the
// original CRT transports 006FC1F0 (pow) and 006FAE8C (_ftol2) are called through D21 inline asm (precedent:
// stage font-txi/image_math.cpp, aurora-window/aurora_window_slots.cpp). Randomness: crt_rand_006fc447 (D52).
// Neither mover is reached by the first main-menu frame (all six menu emitters have node flag bit0 P2P clear).
#include "4ac9e30747-emitter.h"

// Original x87 transports (M45 owners: x87-pow-entry _genuine_x87_pow_006fc1f0, renderer-x87-ftol-alias
// _genuine_x87_ftol_006fae8c); consumed only from inline asm below (same declarations as the stage owners' consumers).
extern "C" {
void __cdecl genuine_x87_ftol_006fae8c();
void __cdecl genuine_x87_pow_006fc1f0();
// 004AB360 segmentproject(out, a, b, point): cdecl, caller pops 10h (004944E8 / 0049451D), returns out in EAX.
// No provider in M45 or any lane object (callee_new_name).
kotor_scene3d::Vector* __cdecl segment_project_004ab360(kotor_scene3d::Vector* out, const kotor_scene3d::Vector* a,
                                                       const kotor_scene3d::Vector* b, const kotor_scene3d::Vector* point);
}

namespace {
using namespace kotor_emitter;

// .rdata constants (pinned image)
constexpr U kZero_0073d700 = 0x00000000;          // 0.0f
constexpr U kOne_0073d764 = 0x3F800000;           // 1.0f
constexpr U kHundredth_0073dfec = 0x3C23D70A;     // 0.01f
constexpr U kFiveThousandth_0073dff4 = 0x3BA3D70A;// 0.005f
constexpr U kHalf_0073e9ac = 0x3F000000;          // 0.5f
constexpr U kThree_00740c84 = 0x40400000;         // 3.0f
constexpr U kMinusOne_00740cbc = 0xBF800000;      // -1.0f
constexpr U kFifty_00740cc0 = 0x42480000;         // 50.0f
constexpr U kMinusTwo_007432f0 = 0xC0000000;      // -2.0f
constexpr U kGravity_007432f4 = 0x411CF5C3;       // 9.81f
constexpr U kOffset_00743320 = 0x3CA3D70A;        // 0.02f

inline float& fl(void* object, U offset) { return field<float>(object, offset); }
inline void copy_words(void* to, const void* from, U count) { std::memcpy(to, from, count * 4u); }   // raw DWORD moves
inline void zero3(Vector* v) { field<U>(v, 0) = 0; field<U>(v, 4) = 0; field<U>(v, 8) = 0; }
inline const kotor_scene3d::SceneVTable* scene_table(void* scene) {
    return *static_cast<const kotor_scene3d::SceneVTable* const volatile*>(scene);
}
inline void* active_scene() { return *kotor_scene3d::cells.active_scene_007b92a0; }   // [007B92A0] (each read cited)
inline const Vector* as_vector(U word) { return static_cast<const Vector*>(as_object(word)); }

// call 006FAE8C (_ftol2) on the x87 value: chop to the 64-bit integer, EAX = low dword.
__declspec(noinline) I x87_ftol_006fae8c(double value) {
    I result;
    __asm {
        fld qword ptr [value]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}
// call 006FC1F0 (pow) with ST1 = base, ST0 = exponent (both loaded from float operands by the original); ST0 = result.
__declspec(noinline) double x87_pow_006fc1f0(double base, double exponent) {
    double result;
    __asm {
        fld qword ptr [base]
        fld qword ptr [exponent]
        call genuine_x87_pow_006fc1f0
        fstp qword ptr [result]
    }
    return result;
}

// One random offset component (both movers, three times each):
//   rand (006FC447); n = _ftol2(emitter[+FC] * 50.0f); rem = rand % n (cdq/idiv, n == 0 traps as in the original);
//   odd rand -> +rem, even rand -> -rem (test al,1 / neg edx); fild; * 0.01f; fstp dword.
inline float random_component(void* emitter) {
    const I r = crt_rand_006fc447();
    const I n = x87_ftol_006fae8c(W(fl(emitter, 0xFC)) * W(F(kFifty_00740cc0)));
    I rem = r % n;
    if ((r & 1) == 0) rem = -rem;
    return S(static_cast<double>(rem) * W(F(kHundredth_0073dfec)));
}

// pa = Rotate(emitter[+1A4] * ~emitter[+1B4], pos - emitter[+170]) + emitter[+164]; velocity rotated the same way.
// (identical sequence in both movers: bezier 004930DE..004931C7, grav 00493D07..00493DE5)
void inherit_local(void* emitter, Vector* pos, Vector* velocity, Vector* pa) {
    Vector local;
    local.x = S(W(pos->x) - W(fl(emitter, 0x170)));
    local.y = S(W(pos->y) - W(fl(emitter, 0x174)));
    local.z = S(W(pos->z) - W(fl(emitter, 0x178)));
    Quaternion inverse, relative; Vector rotated;
    quaternion_negate_w_004a9b50(&inverse, &field<Quaternion>(emitter, 0x1B4));
    quaternion_multiply_004a9ac0(&relative, &field<Quaternion>(emitter, 0x1A4), &inverse);
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, &relative, &local);
    copy_words(pa, r, 3);
    Quaternion inverse2, relative2; Vector rotated2;
    quaternion_negate_w_004a9b50(&inverse2, &field<Quaternion>(emitter, 0x1B4));
    quaternion_multiply_004a9ac0(&relative2, &field<Quaternion>(emitter, 0x1A4), &inverse2);
    r = quaternion_rotate_004a9bb0(&rotated2, &relative2, velocity);
    copy_words(velocity, r, 3);
    vector_add_assign_004a9740(pa, nullptr, &field<Vector>(emitter, 0x164));
}

// P = pa + dt * velocity (bezier 004931DF..00493243, grav 004941A8..00494216)
void advance(const Vector* pa, const Vector* v, double t, Vector* P) {
    const double ax = t * W(v->x);
    const double ay = t * W(v->y);
    const float az = S(t * W(v->z));
    P->x = S(ax + W(pa->x));
    P->y = S(ay + W(pa->y));
    P->z = S(W(az) + W(pa->z));
}

// position = P + speed * direction * dt (bezier 004935A6..0049363B, grav 0049456D..00494607)
void integrate(void* p, const Vector* P, double t) {
    Vector* const pos = &field<Vector>(p, 0x08);
    const double s = fl(p, 0x18);
    const double sdx = s * W(fl(p, 0x1C));
    const float sdy = S(s * W(fl(p, 0x20)));
    const float sdz = S(s * W(fl(p, 0x24)));
    const double x = sdx * t;
    const double y = W(sdy) * t;
    const float z = S(W(sdz) * t);
    pos->x = S(x + W(P->x));
    pos->y = S(y + W(P->y));
    pos->z = S(W(z) + W(P->z));
}

// decay: speed = (1 - dt) * speed; fst; negative -> * -1.0f (fcom 0.0f / test ah,5 / jp: C0 only stores the flip)
void decay(void* p, double one_minus_dt) {
    const double decayed = one_minus_dt * W(fl(p, 0x18));
    fl(p, 0x18) = S(decayed);
    if (decayed < W(F(kZero_0073d700))) fl(p, 0x18) = S(decayed * W(F(kMinusOne_00740cbc)));
}

// Bounce part shared by both movers once HitCheck reports a hit (bezier 00493821..00493AA8, grav 00494803..00494A89).
// offset_constant = the bezier's 0.005f (0073DFF4) - identical in both.
void bounce_hit(void* emitter, void* node, void* p) {
    Vector* const pos = &field<Vector>(p, 0x08);
    Vector* const v = &field<Vector>(p, 0x54);
    void* info = field<void*>(emitter, 0x1E8);
    const double nx2 = W(fl(info, 0x40)) * W(F(kMinusTwo_007432f0));
    const double ny2 = W(fl(info, 0x44)) * W(F(kMinusTwo_007432f0));
    const float nz2 = S(W(fl(info, 0x48)) * W(F(kMinusTwo_007432f0)));
    const float dot = S((W(v->z) * W(fl(info, 0x48)) + W(v->y) * W(fl(info, 0x44))) + W(fl(info, 0x40)) * W(v->x));
    const float tx = S(nx2 * W(dot));
    const double ty = ny2 * W(dot);
    const double tz = W(nz2) * W(dot);
    const float rx = S(W(tx) + W(v->x));
    const float ry = S(ty + W(v->y));
    const double rz = tz + W(v->z);
    const double k = fl(emitter, 0x5C);
    v->x = S(W(rx) * k);
    v->y = S(W(ry) * k);
    v->z = S(rz * k);
    info = field<void*>(emitter, 0x1E8);
    Vector lift;
    lift.x = S(W(fl(info, 0x40)) * W(F(kFiveThousandth_0073dff4)));
    lift.y = S(W(fl(info, 0x44)) * W(F(kFiveThousandth_0073dff4)));
    lift.z = S(W(fl(info, 0x48)) * W(F(kFiveThousandth_0073dff4)));
    vector_add_assign_004a9740(pos, nullptr, &lift);
    const double vz = fl(p, 0x5C), vy = fl(p, 0x58), vx = fl(p, 0x54);
    const double magnitude2 = (vx * vx + vy * vy) + vz * vz;
    if (magnitude2 < W(F(kHalf_0073e9ac))) {
        field<U8>(p, 0x6C) = 1;
        Vector to; copy_words(&to, at(field<void*>(emitter, 0x1E8), 0x40), 3);
        Vector from; field<U>(&from, 0) = 0; field<U>(&from, 4) = 0; field<U>(&from, 8) = kOne_0073d764;
        Quaternion arc;
        const Quaternion* const r = rotation_arc_004ab630(&arc, from, to);
        copy_words(at(p, 0x2C), r, 4);
        info = field<void*>(emitter, 0x1E8);
        Vector offset;
        offset.x = S(W(fl(info, 0x40)) * W(F(kOffset_00743320)));
        offset.y = S(W(fl(info, 0x44)) * W(F(kOffset_00743320)));
        offset.z = S(W(fl(info, 0x48)) * W(F(kOffset_00743320)));
        vector_add_assign_004a9740(pos, nullptr, &offset);
        copy_words(at(p, 0x48), at(field<void*>(emitter, 0x1E8), 0x40), 3);
    }
    if (field<U8>(node, 0xF0) != 0) {
        const float push_scale = fl(emitter, 0x90);
        info = field<void*>(emitter, 0x1E8);
        Vector push;
        push.x = S(W(push_scale) * W(fl(info, 0x40)));
        push.y = S(W(push_scale) * W(fl(info, 0x44)));
        push.z = S(W(push_scale) * W(fl(info, 0x48)));
        vector_add_assign_004a9740(pos, nullptr, &push);
    }
}

// Bounce prelude shared by both movers (bezier 0049368B..00493819, grav 0049465D..004947FD): returns the HitCheck result.
I bounce_check(void* emitter, void* node, void* p, const Vector* old, Vector* pa, Vector* P) {
    Vector* const pos = &field<Vector>(p, 0x08);
    copy_words(pa, pos, 3);
    if (field<U8>(node, 0xF0) != 0) {
        PointerList& ignore = field<PointerList>(field<void*>(emitter, 0x1E8), 8);
        if (ignore.data != 0) {
            live_free(as_object(ignore.data));
            ignore.data = 0;
        }
        ignore.capacity = 0; ignore.size = 0;
        void* const chunk = field<void*>(p, 0x74);
        if (chunk != nullptr) {
            PointerList& list = field<PointerList>(field<void*>(emitter, 0x1E8), 8);
            if (list.size == list.capacity) {
                const I capacity = list.capacity;
                menu_list_growth::allocate_005e03c0(&list, nullptr,
                    static_cast<menu_list_growth::Word>(capacity != 0 ? capacity + capacity : 8));
            }
            reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(list.data))[list.size] = word_of(chunk);
            list.size = list.size + 1;
        }
        const float scale = fl(emitter, 0x90);
        const double dx = W(pos->x) - W(old->x);
        const double dy = W(pos->y) - W(old->y);
        const float dz = S(W(pos->z) - W(old->z));
        const float sx = S(dx * W(scale));
        const double sy = dy * W(scale);
        const double sz = W(dz) * W(scale);
        P->x = S(W(sx) + W(pos->x));
        P->y = S(sy + W(pos->y));
        P->z = S(sz + W(pos->z));
    } else {
        copy_words(P, pos, 3);
    }
    void* const info = field<void*>(emitter, 0x1E8);
    const kotor_scene3d::SceneVTable* const table = scene_table(active_scene());   // first [007B92A0] read (vtable)
    void* const scene = active_scene();                                            // second [007B92A0] read (ECX)
    return table->s038_scene_hit_check(scene, nullptr, *old, *P, pos, info);
}
} // namespace

extern "C" {

// ================================================================ 00492EA0 PartEmitter::moveParticleP2PBezier(dt), RET 4
void __fastcall part_emitter_move_particle_p2p_bezier_00492ea0(void* emitter, void*, float dt) {
    void* const node = field<void*>(emitter, 4);                               // 00492EB2 edi; [L54] 00492F13
    Vector pa; zero3(&pa);                                                     // 00492EB5..00492EC5 [L2C..L34]
    Vector target; zero3(&target);                                             // 00492ECD..00492EDD [L48..L50]
    Vector P; zero3(&P);                                                       // 00492EE5..00492EF5 [L20..L28]
    const U inheriting = (field<U>(node, 0x12C) >> 6) & 1u;                    // 00492EFD..00492F17 (ebx, [L8C])
    {
        void* const gob = field<void*>(emitter, 0x1E4);                        // 00492EAB
        float position[3];                                                     // [L160]
        const U r = gob_table(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(position)); // 00492F03..00492F1E (+64)
        copy_words(&target, as_object(r), 3);                                  // 00492F27..00492F3D
    }
    if (!(fl(emitter, 0xFC) == F(kZero_0073d700))) {                           // 00492F21..00492F46 fcomp / test ah,44h / jnp (equal skips; NaN randomises)
        Vector rnd; zero3(&rnd);                                               // 00492F4C..00492F5C [L38..L40]
        rnd.x = random_component(emitter);                                     // 00492F64..00492FB3
        rnd.y = random_component(emitter);                                     // 00492FB7..00493006
        rnd.z = random_component(emitter);                                     // 0049300A..00493062
        vector_add_assign_004a9740(&target, nullptr, &rnd);                    // 00493059..00493066
    }
    const bool bouncing = ((field<U>(node, 0x12C) >> 4) & 1u) != 0;           // 0049306B..00493076 [L1F]
    if (field<I>(emitter, 0x1CC) <= 0) return;                                 // 0049307A..0049308A
    const float one_minus_dt = S(W(F(kOne_0073d764)) - W(dt));                 // 00493090..0049309D [LCC]
    const double t = W(dt);                                                    // [esp+278h]
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                         // [L10]; 00493AAD..00493ABE
        void* const p = as_object(reinterpret_cast<const volatile U*>(
            static_cast<std::uintptr_t>(field<U>(emitter, 0x1C8)))[i]);       // 004930B2..004930BC (ebp)
        Vector* const pos = &field<Vector>(p, 0x08);                           // 004930BF (edi)
        Vector old; copy_words(&old, pos, 3);                                  // 004930C2..004930D4 [L68..L70]
        Vector* const v = &field<Vector>(p, 0x54);                             // ebx = p+54 (00493162 / 004931E6)
        if (inheriting != 0) inherit_local(emitter, pos, v, &pa);              // 004930D8..004931C7
        else copy_words(&pa, pos, 3);                                          // 004931C9..004931DB
        advance(&pa, v, t, &P);                                                // 004931DF..00493251 [L20..L28]
        if (((field<U>(node, 0x12C) >> 2) & 1u) != 0) {                       // 00493222..00493260 (AffectedByWind; node from [L54])
            const kotor_scene3d::SceneVTable* const table = scene_table(active_scene()); // 00493269..0049326F
            void* const scene = active_scene();                                // 00493293
            Vector out;                                                        // [L208]
            const U r = table->s0d0_scene_get_wind_vector(scene, nullptr, word_of(&out),
                field<U>(&pa, 0), field<U>(&pa, 4), field<U>(&pa, 8), float_to_bits(dt)); // 00493262..00493299 (+D0)
            vector_add_assign_004a9740(&P, nullptr, as_vector(r));             // 0049329F..004932A4
        }
        double tt = W(fl(p, 0x70)) / W(fl(emitter, 0x78));                     // 004932A9..004932AC
        if (tt > W(F(kOne_0073d764))) tt = W(F(kOne_0073d764));                // 004932AF..004932BE (test ah,41h; jne keeps <, =, unordered)
        const double u = W(F(kOne_0073d764)) - tt;                             // 004932C4..004932D0
        const double uu = u * u;                                               // 004932DC..004932E0
        const float u3 = S(uu * u);                                            // 004932E2..004932E6 [L18]
        const float c1 = S(uu * tt * W(F(kThree_00740c84)));                   // 004932EA..004932F2 [L44]
        const float c2 = S(u * tt * tt * W(F(kThree_00740c84)));               // 004932F6..00493300 [L58]
        const float t3 = S(tt * tt * tt);                                      // 00493304..0049330A [L14]
        Vector tt3;                                                            // [L74..L7C]
        tt3.x = S(W(target.x) * W(t3));                                        // 00493310..00493318
        tt3.y = S(W(target.y) * W(t3));                                        // 0049331C..00493324
        tt3.z = S(W(target.z) * W(t3));                                        // 00493328..00493330
        void* const gob = field<void*>(emitter, 0x1E4);                        // 004932CA (vtable 004932D2)
        float orientation[4];                                                  // [L244]
        const float* const q = static_cast<const float*>(as_object(
            gob_table(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(orientation)))); // 004932D4..00493337 (+68)
        const double q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];
        const float strength = fl(emitter, 0x84);                              // 00493340..00493348 [L5C]
        const double ax = (q1 * q3 + q0 * q2) * 2.0;                           // 0049333A..00493351
        const double ay = (q3 * q2 - q1 * q0) * 2.0;                           // 00493353..00493360
        const float az = S(W(F(kOne_0073d764)) - (q2 * q2 + q1 * q1) * 2.0);   // 00493362..0049337A [L180]
        const float sx = S(ax * W(strength));                                  // 00493385..0049338B [L184]
        const double sy = ay * W(strength);                                    // 00493392
        const double sz = W(az) * W(strength);                                 // 00493396..0049339D
        const float c2x = S(W(sx) + W(target.x));                              // 004933A1..004933AC [L100]
        const float c2y = S(sy + W(target.y));                                 // 004933B3..004933B9 [L104]
        const double c2z = sz + W(target.z);                                   // 004933C0
        const double x2 = W(c2x) * W(c2);                                      // 004933C4..004933CB
        const float y2 = S(W(c2y) * W(c2));                                    // 004933CF..004933DA [LE0]
        const float z2 = S(c2z * W(c2));                                       // 004933E1..004933E7 [LE4]
        const double s = fl(emitter, 0x80);                                    // 004933EE
        const double c1x = s * W(fl(emitter, 0x140));                          // 004933F4..004933F6
        const float c1yy = S(s * W(fl(emitter, 0x144)));                       // 004933FC..00493404 [LF8]
        const float c1zz = S(s * W(fl(emitter, 0x148)));                       // 0049340B..00493413 [LFC]
        const double c1xw = c1x + W(fl(emitter, 0x164));                       // 0049341A
        const double c1yw = W(c1yy) + W(fl(emitter, 0x168));                   // 00493420..00493427
        const float c1zw = S(W(c1zz) + W(fl(emitter, 0x16C)));                 // 0049342D..0049343A [L1EC]
        const float k1x = S(c1xw * W(c1));                                     // 00493441..00493447 [L1C0]
        const double k1y = c1yw * W(c1);                                       // 0049344E
        const double k1z = W(c1zw) * W(c1);                                    // 00493452..00493459
        const double u3x = W(u3) * W(fl(emitter, 0x164));                      // 0049345D..00493461
        const double u3y = W(u3) * W(fl(emitter, 0x168));                      // 00493467..0049346B
        const float u3z = S(W(u3) * W(fl(emitter, 0x16C)));                    // 00493471..0049347B [L174]
        const float b0x = S(u3x + W(k1x));                                     // 00493482..0049348B [L9C]
        const float b0y = S(u3y + k1y);                                        // 00493492..00493494 [LA0]
        const float b0z = S(W(u3z) + k1z);                                     // 0049349B..004934A4 [LA4]
        const double bx = x2 + W(b0x);                                         // 004934AF
        const double by = W(b0y) + W(y2);                                      // 004934B6..004934BD
        const float bz = S(W(b0z) + W(z2));                                    // 004934C4..004934D2 [L1E0]
        Vector bez;                                                            // [L80..L88]
        bez.x = S(bx + W(tt3.x));                                              // 004934D9..004934DF
        bez.y = S(by + W(tt3.y));                                              // 004934E6..004934EA
        bez.z = S(W(bz) + W(tt3.z));                                           // 004934F1..004934FC
        const double w = x87_pow_006fc1f0(W(t3), W(fl(emitter, 0x60)));        // 00493503..0049350A pow(t^3, emitter[+60])
        const double keep = W(F(kOne_0073d764)) - w;                           // 0049350F..00493515
        const float px = S(W(P.x) * keep);                                     // 00493517..0049351D [L118]
        const float py = S(W(P.y) * keep);                                     // 00493524..0049352A [L11C]
        const double pz = W(P.z) * keep;                                       // 00493531
        const double bxw = W(bez.x) * w;                                       // 00493535..0049353C
        const float byw = S(W(bez.y) * w);                                     // 0049353E..00493547 [L14C]
        const float bzw = S(W(bez.z) * w);                                     // 0049354E..00493557 [L150]
        P.x = S(bxw + W(px));                                                  // 0049355E..0049356D
        P.y = S(W(byw) + W(py));                                               // 00493575..00493594
        P.z = S(W(bzw) + pz);                                                  // 00493587..004935A2
        integrate(p, &P, t);                                                   // 004935A6..0049363B
        fl(p, 0x5C) = S(W(fl(p, 0x5C)) - t * W(fl(emitter, 0x7C)) * W(F(kGravity_007432f4))); // 00493634..0049364A
        decay(p, W(one_minus_dt));                                             // 0049364D..00493672 ([LCC] float)
        aurora_hit_info_clear_output_00456a60(field<void*>(emitter, 0x1E8), nullptr); // 00493674..0049367A (every particle)
        if (!bouncing) continue;                                               // 0049367F..00493685
        if (bounce_check(emitter, node, p, &old, &pa, &P) == 0) continue;     // 0049368B..0049381B
        bounce_hit(emitter, node, p);                                          // 00493821..00493AA8
    }
}

// ================================================================ 00493AE0 PartEmitter::moveParticleP2PGrav(dt), RET 4
void __fastcall part_emitter_move_particle_p2p_grav_00493ae0(void* emitter, void*, float dt) {
    void* const node = field<void*>(emitter, 4);                               // 00493AEB ebp; [F28] 00493B8F
    Vector pa; zero3(&pa);                                                     // 00493AF4..00493B04 [F14..F1C]
    Vector target; zero3(&target);                                             // 00493B0C..00493B1C [F54..F5C]
    Vector P; zero3(&P);                                                       // 00493B24..00493B34 [F30..F38]
    Vector D; zero3(&D);                                                       // 00493B3C..00493B4C [F48..F50]
    Vector accel; zero3(&accel);                                               // 00493B54..00493B6A [F90..F98]
    const bool inheriting = ((field<U>(node, 0x12C) >> 6) & 1u) != 0;         // 00493B75..00493B88 [F2F]
    {
        void* const gob = field<void*>(emitter, 0x1E4);                        // 00493AEE
        float position[3];                                                     // [F19C]
        const U r = gob_table(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(position)); // 00493B8C..00493B93 (+64)
        copy_words(&target, as_object(r), 3);                                  // 00493B9C..00493BB2
    }
    if (!(fl(emitter, 0xFC) == F(kZero_0073d700))) {                           // 00493B96..00493BBB (jnp: equal skips)
        Vector rnd; zero3(&rnd);                                               // 00493BC1..00493BD1 [F3C..F44]
        rnd.x = random_component(emitter);                                     // 00493BD9..00493C17
        rnd.y = random_component(emitter);                                     // 00493C1B..00493C59
        rnd.z = random_component(emitter);                                     // 00493C5D..00493CA4
        vector_add_assign_004a9740(&target, nullptr, &rnd);                    // 00493C9B..00493CA8
    }
    const bool bouncing = ((field<U>(node, 0x12C) >> 4) & 1u) != 0;           // 00493CAD..00493CB8 [F27]
    const double t = W(dt);                                                    // [esp+354h]
    for (I i = 0; i < field<I>(emitter, 0x1CC); ++i) {                         // 00493CBC..00493CCC / 00494A8E..00494AA3 [F10]
        void* const p = as_object(reinterpret_cast<const volatile U*>(
            static_cast<std::uintptr_t>(field<U>(emitter, 0x1C8)))[i]);       // 00493CD2..00493CDC (ebx)
        Vector* const pos = &field<Vector>(p, 0x08);                           // 00493CDF
        Vector old; copy_words(&old, pos, 3);                                  // 00493CE2..00493CFA [F78..F80]
        Vector* const v = &field<Vector>(p, 0x54);
        if (inheriting) inherit_local(emitter, pos, v, &pa);                   // 00493CF4..00493DEE
        else copy_words(&pa, pos, 3);                                          // 00493DF0..00493E00
        U flags = field<U>(node, 0x12C);                                       // 00493E04
        if (((flags >> 8) & 1u) != 0 && ((flags >> 6) & 1u) == 0 && field<void*>(emitter, 0x40) != nullptr) { // 00493E0A..00493E28
            Vector d;                                                          // [F178..F180]
            d.x = S(W(pa.x) - W(fl(emitter, 0x158)));                          // 00493E2E..00493E48
            d.y = S(W(pa.y) - W(fl(emitter, 0x15C)));                          // 00493E56..00493E5F
            d.z = S(W(pa.z) - W(fl(emitter, 0x160)));                          // 00493E66..00493E6D
            Quaternion inverse; Vector r1, r2;
            quaternion_negate_w_004a9b50(&inverse, &field<Quaternion>(emitter, 0x194)); // 00493E74
            const Vector* r = quaternion_rotate_004a9bb0(&r1, &inverse, &d);  // 00493E7C..00493E85
            r = quaternion_rotate_004a9bb0(&r2, &field<Quaternion>(field<void*>(emitter, 0x40), 0x84), r); // 00493E8A..00493E9D
            copy_words(&pa, r, 3);                                             // 00493EA2..00493EB2
            void* const gob = field<void*>(emitter, 0x40);                     // 00493EB6
            Vector o;                                                          // [F10C..F114]
            o.z = S(W(fl(gob, 0x80)) - W(fl(emitter, 0x160)));                 // 00493EB9..00493EC2
            o.y = S(W(fl(gob, 0x7C)) - W(fl(emitter, 0x15C)));                 // 00493EC8..00493ED2
            o.x = S(W(fl(gob, 0x78)) - W(fl(emitter, 0x158)));                 // 00493ED6..00493EDC
            vector_add_assign_004a9740(&pa, nullptr, &o);                      // 00493EDE..00493EF3
            vector_add_assign_004a9740(&field<Vector>(p, 0x60), nullptr, &o);  // 00493EF8..00493F03
            vector_add_assign_004a9740(&pa, nullptr, &field<Vector>(emitter, 0x158)); // 00493F08..00493F0D
            Quaternion inverse2; Vector r3, r4;
            quaternion_negate_w_004a9b50(&inverse2, &field<Quaternion>(emitter, 0x194)); // 00493F12..00493F1F
            r = quaternion_rotate_004a9bb0(&r3, &inverse2, v);                 // 00493F27..00493F30
            r = quaternion_rotate_004a9bb0(&r4, &field<Quaternion>(field<void*>(emitter, 0x40), 0x84), r); // 00493F35..00493F48
            copy_words(v, r, 3);                                               // 00493F4D..00493F61
        }
        flags = field<U>(node, 0x12C);                                         // 00493F64
        if (((flags >> 10) & 1u) != 0 && field<void*>(emitter, 0x40) != nullptr) { // 00493F6A..00493F7B (InheritingPart)
            Vector d;                                                          // [F148..F150]
            d.x = S(W(pa.x) - W(fl(emitter, 0x158)));                          // 00493F81..00493F9C
            d.y = S(W(pa.y) - W(fl(emitter, 0x15C)));                          // 00493FA4..00493FB3
            d.z = S(W(pa.z) - W(fl(emitter, 0x160)));                          // 00493FBA..00493FC1
            Quaternion inverse; Vector r5, r6;
            quaternion_negate_w_004a9b50(&inverse, &field<Quaternion>(emitter, 0x194)); // 00493FC8
            const Vector* r = quaternion_rotate_004a9bb0(&r5, &inverse, &d);  // 00493FD0..00493FD9
            r = quaternion_rotate_004a9bb0(&r6, &field<Quaternion>(field<void*>(emitter, 0x40), 0x84), r); // 00493FDE..00493FF1
            Vector o;                                                          // [F118..F120]
            o.x = S(W(fl(emitter, 0x164)) - W(fl(emitter, 0x158)));            // 00493FF6..0049400F
            o.y = S(W(fl(emitter, 0x168)) - W(fl(emitter, 0x15C)));            // 00494016..0049402E
            o.z = S(W(fl(emitter, 0x16C)) - W(fl(emitter, 0x160)));            // 00494035..00494042
            copy_words(&pa, r, 3);                                             // 00493FFC..0049403B
            vector_add_assign_004a9740(&pa, nullptr, &o);                      // 00494029..00494049
            Vector* const anchor = &field<Vector>(p, 0x60);                    // 00494055 (edi)
            vector_add_assign_004a9740(anchor, nullptr, &o);                   // 0049404E..0049405B
            void* gob = field<void*>(emitter, 0x40);                           // 00494062
            {
                const float ax = S(W(anchor->x) - W(fl(gob, 0x78)));           // 00494060..0049406C
                const float ay = S(W(anchor->y) - W(fl(gob, 0x7C)));           // 0049407A..00494080
                const float az = S(W(anchor->z) - W(fl(gob, 0x80)));           // 00494087..0049409B
                anchor->x = ax; anchor->y = ay; anchor->z = az;                // 00494096..004940A9
            }
            Quaternion inverse2; Vector r7, r8;
            quaternion_negate_w_004a9b50(&inverse2, &field<Quaternion>(emitter, 0x194)); // 004940AC..004940BB
            r = quaternion_rotate_004a9bb0(&r7, &inverse2, anchor);            // 004940C3..004940CC
            r = quaternion_rotate_004a9bb0(&r8, &field<Quaternion>(field<void*>(emitter, 0x40), 0x84), r); // 004940D1..004940E4
            copy_words(anchor, r, 3);                                          // 004940E9..004940F8
            gob = field<void*>(emitter, 0x40);                                 // 004940FD
            {
                const float ax = S(W(anchor->x) + W(fl(gob, 0x78)));           // 004940FB..0049410A
                const float ay = S(W(anchor->y) + W(fl(gob, 0x7C)));           // 00494118..0049411E
                const float az = S(W(anchor->z) + W(fl(gob, 0x80)));           // 00494125..0049413B
                anchor->x = ax; anchor->y = ay; anchor->z = az;                // 00494132..00494149
            }
            vector_add_assign_004a9740(&pa, nullptr, &field<Vector>(emitter, 0x158)); // 00494109 / 00494137..0049414C
            Quaternion inverse3; Vector r9, r10;
            quaternion_negate_w_004a9b50(&inverse3, &field<Quaternion>(emitter, 0x194)); // 00494151..00494164
            r = quaternion_rotate_004a9bb0(&r9, &inverse3, v);                 // 0049416C..00494175
            r = quaternion_rotate_004a9bb0(&r10, &field<Quaternion>(field<void*>(emitter, 0x40), 0x84), r); // 0049417A..0049418C
            copy_words(v, r, 3);                                               // 00494191..004941A5
        }
        advance(&pa, v, t, &P);                                                // 004941A8..00494216 [F30..F38]
        if (((field<U>(node, 0x12C) >> 2) & 1u) != 0) {                       // 0049421A..00494225 (AffectedByWind)
            void* const scene = active_scene();                                // 00494232 (single read: vtable and ECX)
            Vector out;                                                        // [F25C]
            const U r = scene_table(scene)->s0d0_scene_get_wind_vector(scene, nullptr, word_of(&out),
                field<U>(&pa, 0), field<U>(&pa, 4), field<U>(&pa, 8), float_to_bits(dt)); // 00494227..00494258 (+D0)
            vector_add_assign_004a9740(&P, nullptr, as_vector(r));             // 0049425E..00494263
        }
        D.x = S(W(target.x) - W(pa.x));                                        // 00494268..00494274 [F60]->[F48]
        D.y = S(W(target.y) - W(pa.y));                                        // 00494278..00494284 [F64]->[F4C]
        D.z = S(W(target.z) - W(pa.z));                                        // 00494288..00494298 [F68]->[F50]
        const float radius2 = S(W(fl(emitter, 0xA4)) * W(fl(emitter, 0xA4))); // 0049429C..004942AE [F20]
        const double dist2 = (W(D.z) * W(D.z) + W(D.y) * W(D.y)) + W(D.x) * W(D.x); // 004942B4..004942CE
        if (dist2 < W(radius2) || dist2 == W(radius2)) {                       // 004942D0..004942D9 fcomp / test ah,41h / jnp
            field<U>(p, 0x70) = field<U>(emitter, 0x78);                       // 00494AC3..00494AC6
            return;                                                            // 00494AC9..00494AD3 (leaves the whole mover; no tail copy)
        }
        const double w = x87_pow_006fc1f0(W(fl(emitter, 0x64)), t);            // 004942DF..004942E9 pow(emitter[+64], dt)
        {
            const float vx = S(w * W(v->x));                                   // 004942EE..004942F4
            const float vy = S(w * W(v->y));                                   // 00494302..00494307
            const float vz = S(w * W(v->z));                                   // 00494315..0049431D
            v->x = vx; v->y = vy; v->z = vz;                                   // 00494318..0049432B
        }
        const float pull = fl(emitter, 0x74);                                  // 0049432E..0049433E [F74]
        Vector unit;
        const Vector* const n = normalize_004ab180(&unit, &D);                 // 00494331..00494342
        const float acc_x = S(W(pull) * W(n->x) * t);                          // 00494347..0049436E [F84]->[F90]
        const float acc_y = S(W(pull) * W(n->y) * t);                          // 00494350..00494392 [F88]->[F94]
        const float pull_z = S(W(pull) * W(n->z));                             // 00494357..0049435E [F1EC]
        const float acc_z = S(W(pull_z) * t);                                  // 00494399..004943B9 [F8C]->[F98]
        accel.x = acc_x; accel.y = acc_y; accel.z = acc_z;
        Vector step;                                                           // [F190..F198]
        step.x = S(W(acc_x) * t);                                              // 004943C0..004943DC
        step.y = S(W(acc_y) * t);                                              // 004943E3..004943F1
        step.z = S(W(acc_z) * t);                                              // 004943F8..00494406
        vector_add_assign_004a9740(&P, nullptr, &step);                        // 0049438A / 004943B5..0049440D
        vector_add_assign_004a9740(v, nullptr, &accel);                        // 00494412..0049441C
        D.x = S(W(P.x) - W(pa.x));                                             // 00494421..00494429 [FDC]->[F48]
        D.y = S(W(P.y) - W(pa.y));                                             // 00494430..00494438 [FE0]->[F4C]
        D.z = S(W(P.z) - W(pa.z));                                             // 0049443F..00494464 [FE4]->[F50]
        {
            Vector zero; zero3(&zero);                                         // 0049447C..00494492 [F16C..F174]
            if (aurora_vector_equal_004aa980(&D, &zero) == 0) {                // 00494472..004944A7
                Vector heading;
                const Vector* const h = normalize_004ab180(&heading, &D);      // 004944A9..004944B6
                copy_words(at(p, 0x48), h, 3);                                 // 004944BB..004944CE
            }
        }
        Vector projection;                                                     // [F22C]
        const Vector* const c = segment_project_004ab360(&projection, &pa, &P, &target); // 004944D1..004944E8
        Vector e;                                                              // [F3C..F44] (copied to [F48..F50])
        {
            const float cx = c->x, cy = c->y, cz = c->z;                       // 004944F1..00494502
            e.x = S(W(target.x) - W(cx));                                      // 004944ED..00494505
            e.y = S(W(target.y) - W(cy));                                      // 00494509..00494520
            e.z = S(W(target.z) - W(cz));                                      // 00494524..00494534
        }
        copy_words(&D, &e, 3);                                                 // 00494519 / 00494530 / 00494544
        const double near2 = (W(e.z) * W(e.z) + W(e.y) * W(e.y)) + W(e.x) * W(e.x); // 00494538..0049455A
        if (near2 < W(radius2) || near2 == W(radius2))                         // 0049455C..00494565 (jp skips on > / unordered)
            field<U>(p, 0x70) = field<U>(emitter, 0x78);                       // 00494567..0049456A
        integrate(p, &P, t);                                                   // 0049456D..00494607
        fl(p, 0x5C) = S(W(fl(p, 0x5C)) - t * W(fl(emitter, 0x7C)) * W(F(kGravity_007432f4))); // 00494600..00494616
        decay(p, W(F(kOne_0073d764)) - t);                                     // 00494619..00494644 (1 - dt kept on the x87 stack)
        aurora_hit_info_clear_output_00456a60(field<void*>(emitter, 0x1E8), nullptr); // 00494646..0049464C (every particle)
        if (!bouncing) continue;                                               // 00494651..00494657
        if (bounce_check(emitter, node, p, &old, &pa, &P) == 0) continue;     // 0049465D..004947FD
        bounce_hit(emitter, node, p);                                          // 00494803..00494A89
    }
    void* const gob = field<void*>(emitter, 0x40);                             // 00494AA9
    if (gob != nullptr && ((field<U>(node, 0x12C) >> 8) & 1u) != 0) {         // 00494AAC..00494ABC
        copy_words(at(emitter, 0x158), at(gob, 0x78), 3);                      // 00494ABE / 00494AEA..00494B02
        copy_words(at(emitter, 0x194), at(gob, 0x84), 4);                      // 00494AFD..00494B1E
        return;
    }
    if (((field<U>(node, 0x12C) >> 10) & 1u) != 0) {                           // 00494AD6..00494AE2
        copy_words(at(emitter, 0x158), at(emitter, 0x164), 3);                 // 00494AE4..00494B02
        // Original quirk: EAX still holds emitter[+40] (00494AA9) and the orientation is copied from [EAX+84h] - the
        // attached Gob's orientation, not emitter[+1A4]. With emitter[+40] == NULL the original reads 00000084 and
        // faults; the same address arithmetic is reproduced (REVIEW: original fault preserved, not guarded).
        const void* const source = as_object(word_of(gob) + 0x84u);            // 00494AFD add eax,84h
        copy_words(at(emitter, 0x194), source, 4);                             // 00494B05..00494B1E
    }
}

} // extern "C"
