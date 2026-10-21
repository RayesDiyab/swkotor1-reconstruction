// animate.cpp - lane-3d-emitter fork F4: 00494DA0 PartEmitter::AnimateParticles (thiscall, RET 4, float delta).
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1) 00494DA0..00495B10 (lane-local d/00494DA0.txt).
// Stack-local names Lxxx = the original [esp+xxx] offset while EBX is pushed (00494E7B..004958F8);
// Exxx = offset with ESI/EBP/EDI pushed only. D30-FINAL: x87 intermediates are double, float only at
// the original `fstp dword` points (S()). Unwind: none in this function.
#include "4ac9e30747-emitter.h"

// LOCAL-DECLS (merge into emitter.h): none - every callee is declared by emitter.h / scene3d.h.

namespace {
using namespace kotor_emitter;

inline float& fv(void* object, U offset) { return field<float>(object, offset); }
inline U& uv(void* object, U offset) { return field<U>(object, offset); }
inline Vector* list_vec(void* list_owner, U list_offset, I index) {
    // CExoArrayList<Vector> data pointer at +list_offset, element stride 0Ch (00495085 / 004950E1 lea edi,[ebp+ebp*2]; shl 2)
    return reinterpret_cast<Vector*>(field<std::uintptr_t>(list_owner, list_offset) + static_cast<std::uintptr_t>(index) * 12u);
}
inline void copy_words(void* object, U to, U from, U count) {
    for (U k = 0; k < count; ++k) uv(object, to + 4 * k) = uv(object, from + 4 * k);   // raw DWORD moves
}
inline void* gob_position(void* gob, Vector* out) {     // Gob vtbl+64 GetPosition(out) (RET 4); EAX = returned pointer
    return as_object(gob_table(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(out)));
}
inline const float* gob_orientation(void* gob, Quaternion* out) {   // Gob vtbl+68 GetOrientation(out) (RET 4)
    return static_cast<const float*>(as_object(gob_table(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(out))));
}

// 00494F3C..004958F1: point-to-point target block (only reached when [+1E4] target is set and vtbl+84 returns an
// object - LightningEmitter's table holds 00641DB0 self_return there; base/Fountain/Explosion/Single hold return_zero).
void animate_target_block(void* self, void* target) {
    Vector l4c;                                                                     // [esp+4C] target position out
    gob_position(target, &l4c);                                                     // 00494F3C..00494F43 (EAX unused)
    Vector l1c = {0.0f, 0.0f, 0.0f};                                                // 00494F83..00494F93
    float l128[4] = {1.0f, 0.0f, 0.0f, 0.0f};                                       // 00494F57..00494F78 (+128=1.0f)
    part_table(self)->s00c(self, nullptr, word_of(&l1c), word_of(l128));            // 00494F46..00494F9B GetWorld(&L1C, &L128)
    Vector ld0;
    const float* p = static_cast<const float*>(gob_position(target, &ld0));         // 00494F9E..00494FAE
    Vector l10;
    l10.x = S(W(p[0]) - W(l1c.x));                                                   // 00494FC4..00494FCA
    l10.y = S(W(p[1]) - W(l1c.y));                                                   // 00494FBC..00494FCE
    l10.z = S(W(p[2]) - W(l1c.z));                                                   // 00494FB1..00494FD2
    fv(self, 0xD0) = S(vector_magnitude_004ab100(&l10));                             // 00494FD6..00494FDD
    void* const obj = as_object(part_table(self)->s084(self, nullptr));             // 00494FDB..00494FE8 vtbl+84
    if (obj == nullptr) return;                                                     // 00494FF0..00494FF6 -> 004958F1
    fv(self, 0x58) = S(W(fv(self, 0xC4)) * W(fv(self, 0xD0)) + W(F(0x40000000u))); // 00494FFC..00495019 (00741BE0 2.0f)
    Vector l88;
    quaternion_zdir_0043e250(static_cast<const Quaternion*>(at(self, 0x14)), nullptr, &l88); // 00495002..0049501C
    const I n = ftol2(W(fv(self, 0x100)) * W(fv(self, 0xD0)) + W(F(0x3F000000u))); // 00495021..00495033 (0073E9AC 0.5f)
    const I count = n + 2;                                                          // 0049503A lea edi,[ebp+2]
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x210)), nullptr, count); // 0049503D..00495044
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x21C)), nullptr, count); // 00495049..00495050
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x228)), nullptr, count); // 00495055..0049505C
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x234)), nullptr, count); // 00495061..00495068
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x240)), nullptr, count); // 0049506D..00495074
    vector_list_set_size_004213a0(static_cast<PointerList*>(at(obj, 0x24C)), nullptr, count); // 00495079..00495080
    Vector l7c = {0.0f, 0.0f, 0.0f};                                                // 0049508F..004950A2
    *list_vec(obj, 0x228, 0) = l1c;                                                 // 00495085..004950BA
    *list_vec(obj, 0x210, 0) = l1c;                                                 // 004950BD..004950D4
    *list_vec(obj, 0x228, n + 1) = l4c;                                             // 004950D7..004950FB ([edi+ecx+0Ch])
    *list_vec(obj, 0x210, n + 1) = l4c;                                             // 004950FE..00495119
    Vector l40;
    {
        const double s = W(fv(self, 0x110));                                        // 0049511C
        l40.x = S(W(l88.x) * s);                                                     // 00495122..00495131
        l40.y = S(W(l88.y) * s);                                                     // 00495135..0049513E
        l40.z = S(W(l88.z) * s);                                                     // 00495142..0049514B
    }
    *list_vec(obj, 0x24C, 0) = l40;                                                 // 00495129 / 00495151..00495162
    *list_vec(obj, 0x234, 0) = l40;                                                 // 00495165..00495174
    Vector* const l3c_end = list_vec(obj, 0x24C, n + 1);                            // 00495177..00495189
    {
        Vector ld0b;
        const float* p2 = static_cast<const float*>(gob_position(target, &ld0b));   // 0049517D..00495195
        const float l2c_s = fv(self, 0x110);                                        // 0049519F / 004951BF mov [esp+34h],ecx
        l10.z = S(W(p2[2]) - W(l1c.z));                                              // 00495198..004951CB
        l10.y = S(W(p2[1]) - W(l1c.y));
        l10.x = S(W(p2[0]) - W(l1c.x));
        Vector l138;
        const Vector* nrm = normalize_004ab180(&l138, &l10);                         // 004951B0..004951CF
        l40.x = S(W(l2c_s) * W(nrm->x));                                             // 004951D4..004951E8
        l40.y = S(W(l2c_s) * W(nrm->y));                                             // 004951F0..004951F7
        l40.z = S(W(l2c_s) * W(nrm->z));                                             // 004951FB..0049520F
        *l3c_end = l40;                                                             // 00495206..00495217
        *list_vec(obj, 0x234, n + 1) = l40;                                         // 0049521A..0049522D
    }
    if (n >= 1) {                                                                   // 004951DB cmp ebp,1; 00495230 jl 004957F0
        const float ldc = S(static_cast<double>(n + 1));                            // 00495236..00495246 fild/fstp [esp+DCh]
        for (I i = 1; i <= n; ++i) {                                                // L78 = 1 (004951E0); 004957D8..004957E6
            // ---- curve point (cubic Bezier: wpos, wpos + zdir*s, target + targetdir*s, target)
            const double td = static_cast<double>(i) / W(ldc);                       // 00495250..00495254
            float l28 = S(td);                                                       // 0049525B fst [esp+28h]
            if (!x87_less_or_equal(td, 1.0)) l28 = 1.0f;                             // 0049525F..0049526C (test ah,41h; jne)
            const double u = 1.0 - W(l28);                                           // 00495274..0049527E
            const float l2c = S(u);                                                  // 00495293 fst [esp+2Ch]
            Vector* const point = list_vec(obj, 0x228, i);                           // 00495288 / 004952A5 add edi,ebx
            const double u2 = u * W(l2c);                                            // 00495299
            const float l70 = S(u2);                                                 // 004952A7 fst [esp+74h]
            const float l30 = S(u2 * W(l2c));                                        // 004952AB..004952AF
            const float l38 = S(W(l70) * W(l28) * W(F(0x40400000u)));                // 004952B3..004952C1 (00740C84 3.0f)
            const float l34 = S(W(l2c) * W(l28) * W(l28) * W(F(0x40400000u)));       // 004952C5..004952D7
            const double t2 = W(l28) * W(l28);                                       // 004952DB..004952DF
            const float l3c = S(t2);                                                 // 004952E3 fst [esp+40h]
            const double t3 = t2 * W(l28);                                           // 004952E7
            const float lc4 = S(W(l4c.x) * t3);                                      // 004952EB..004952F1
            const float lc8 = S(W(l4c.y) * t3);                                      // 004952F8..004952FE
            const float lcc = S(W(l4c.z) * t3);                                      // 00495305..0049530B
            Quaternion l1bc;
            const float* q = gob_orientation(target, &l1bc);                         // 0049529D..00495314 target vtbl+68
            const double a = (W(q[3]) * W(q[1]) + W(q[2]) * W(q[0])) * 2.0;          // 00495317..00495324
            const double b = (W(q[3]) * W(q[2]) - W(q[1]) * W(q[0])) * 2.0;          // 00495326..00495333
            const float l1a0 = S(1.0 - (W(q[2]) * W(q[2]) + W(q[1]) * W(q[1])) * 2.0); // 00495335..00495357
            const float l68 = fv(self, 0x110);                                      // 0049533B..00495343
            const float l168 = S(a * W(l68));                                        // 00495362..00495368
            const double bs = b * W(l68);                                            // 0049536F
            const double cs = W(l1a0) * W(l68);                                      // 00495373..0049537A
            const float l110 = S(W(l168) + W(l4c.x));                                // 0049537E..00495389
            const float l114 = S(bs + W(l4c.y));                                     // 00495390..00495396
            const double p2z = cs + W(l4c.z);                                        // 0049539D
            const double p2x = W(l110) * W(l34);                                     // 004953A1..004953A8
            const float l108 = S(W(l114) * W(l34));                                  // 004953AC..004953B7
            const float l10c = S(p2z * W(l34));                                      // 004953BE..004953C4
            const double s1 = W(fv(self, 0x110));                                    // 004953CB
            const double zxs = W(l88.x) * s1;                                        // 004953D1..004953D8
            const float l120 = S(W(l88.y) * s1);                                     // 004953DA..004953E3
            const float l124 = S(W(l88.z) * s1);                                     // 004953EA..004953F3
            const double a1x = zxs + W(l1c.x);                                       // 004953FA..004953FE
            const double a1y = W(l120) + W(l1c.y);                                   // 00495400..00495407
            const float l188 = S(W(l124) + W(l1c.z));                                // 0049540B..00495416
            const float l144 = S(a1x * W(l38));                                      // 0049541D..00495423
            const double a1yl = a1y * W(l38);                                        // 0049542A
            const double a1zl = W(l188) * W(l38);                                    // 0049542E..00495435
            const double wxu = W(l1c.x) * W(l30);                                    // 00495439..0049543D
            const double wyu = W(l1c.y) * W(l30);                                    // 00495441..00495445
            const float l164 = S(W(l1c.z) * W(l30));                                 // 00495449..00495451
            const float le0 = S(wxu + W(l144));                                      // 00495458..00495461
            const float le4 = S(wyu + a1yl);                                         // 00495468..0049546A
            const float le8 = S(W(l164) + a1zl);                                     // 00495471..0049547A
            const double sx = p2x + W(le0);                                          // 00495485
            const double sy = W(le4) + W(l108);                                      // 0049548C..00495493
            const float l1b8 = S(W(le8) + W(l10c));                                  // 0049549A..004954A8
            l40.x = S(sx + W(lc4));                                                  // 004954AF..004954B8
            l40.y = S(sy + W(lc8));                                                  // 004954BC..004954C3
            l40.z = S(W(l1b8) + W(lcc));                                             // 004954C7..004954E2
            *point = l40;                                                            // 004954DD..004954F4
            *list_vec(obj, 0x210, i) = l40;                                          // 004954F7..00495520
            // ---- curve tangent (derivative), normalized and scaled by [+110]
            const float l30d = S(W(l70) * W(F(0xC0400000u)));                        // 004954E6..00495501 (00743334 -3.0f)
            const double th3 = W(l28) * W(F(0x40400000u));                           // 00495507..0049550D
            const float l38d = S((1.0 - th3) * W(l2c) * W(F(0x40400000u)));          // 00495517..0049553F
            const float l34d = S((W(F(0x40000000u)) - th3) * W(l28) * W(F(0x40400000u))); // 00495543..00495555 (00741BE0 2.0f)
            const double t23 = W(l3c) * W(F(0x40400000u));                           // 0049555B..0049555F
            const float lec = S(W(l4c.x) * t23);                                     // 00495565..0049556B
            const float lf0 = S(W(l4c.y) * t23);                                     // 00495572..00495578
            const float lf4 = S(W(l4c.z) * t23);                                     // 0049557F..00495585
            Quaternion l1cc;
            const float* r = gob_orientation(target, &l1cc);                         // 00495525..0049558E target vtbl+68
            const double a2 = (W(r[1]) * W(r[3]) + W(r[0]) * W(r[2])) * 2.0;         // 00495591..0049559E
            const double b2 = (W(r[3]) * W(r[2]) - W(r[0]) * W(r[1])) * 2.0;         // 004955A0..004955AD
            const float l17c = S(1.0 - (W(r[2]) * W(r[2]) + W(r[1]) * W(r[1])) * 2.0); // 004955AF..004955C7
            const float l74 = fv(self, 0x110);                                      // 004955D0..004955D8
            const float l1a4 = S(a2 * W(l74));                                       // 004955DC..004955E2
            const double b2s = b2 * W(l74);                                          // 004955E9
            const double c2s = W(l17c) * W(l74);                                     // 004955ED..004955F4
            const float lf8 = S(W(l1a4) + W(l4c.x));                                 // 004955F8..00495603
            const float lfc = S(b2s + W(l4c.y));                                     // 0049560A..00495610
            const double q2z = c2s + W(l4c.z);                                       // 00495617
            const double q2x = W(lf8) * W(l34d);                                     // 0049561B..00495622
            const float la4 = S(W(lfc) * W(l34d));                                   // 00495626..00495631
            const float la8 = S(q2z * W(l34d));                                      // 00495638..0049563E
            const double s2 = W(fv(self, 0x110));                                    // 00495645
            const double zxs2 = W(l88.x) * s2;                                       // 0049564B..00495652
            const float lb0 = S(W(l88.y) * s2);                                      // 00495654..0049565D
            const float lb4 = S(W(l88.z) * s2);                                      // 00495664..0049566D
            const double b1x = zxs2 + W(l1c.x);                                      // 00495674..00495678
            const double b1y = W(lb0) + W(l1c.y);                                    // 0049567A..00495681
            const float l194 = S(W(lb4) + W(l1c.z));                                 // 00495685..00495690
            const float l150 = S(b1x * W(l38d));                                     // 00495697..0049569D
            const double b1yl = b1y * W(l38d);                                       // 004956A4
            const double b1zl = W(l194) * W(l38d);                                   // 004956A8..004956AF
            const double wxv = W(l1c.x) * W(l30d);                                   // 004956B3..004956B7
            const double wyv = W(l1c.y) * W(l30d);                                   // 004956BB..004956BF
            const float l140 = S(W(l1c.z) * W(l30d));                                // 004956C3..004956CB
            const float lb8 = S(wxv + W(l150));                                      // 004956D2..004956DB
            const float lbc = S(wyv + b1yl);                                         // 004956E2..004956E4
            const float lc0 = S(W(l140) + b1zl);                                     // 004956EB..004956F4
            const double dx = q2x + W(lb8);                                          // 004956FF
            const double dy = W(lbc) + W(la4);                                       // 00495706..0049570D
            const float ld8 = S(W(lc0) + W(la8));                                    // 00495714..00495722
            l7c.x = S(dx + W(lec));                                                  // 00495729..0049573A (L10 -> L7C)
            l7c.y = S(dy + W(lf0));                                                  // 0049573E..0049575B
            l7c.z = S(W(ld8) + W(lf4));                                              // 00495749..0049576A
            vector_normalize_004ab130(&l7c, nullptr);                                // 00495771..00495775
            const double s3 = W(fv(self, 0x110));                                    // 0049577A
            Vector l58;
            l58.x = S(W(l7c.x) * s3);                                                // 00495780..00495794
            l58.y = S(W(l7c.y) * s3);                                                // 0049579C..004957A5
            l58.z = S(W(l7c.z) * s3);                                                // 004957A9..004957BB
            *list_vec(obj, 0x24C, i) = l58;                                          // 00495784..004957C3
            *list_vec(obj, 0x234, i) = l58;                                          // 004957C8..004957D5
        }
    }
    // 004957F0..004958EE: [obj+27C] from (target - wpos) / radius; [obj+1F8] = target - wpos; [obj+204] = wpos.
    Vector ld0c;
    const float* p3 = static_cast<const float*>(gob_position(target, &ld0c));       // 004957F0..00495800
    Vector la0;
    la0.x = S(W(p3[0]) - W(l1c.x));                                                  // 00495803..00495817
    la0.y = S(W(p3[1]) - W(l1c.y));                                                  // 0049581F..00495826
    la0.z = S(W(p3[2]) - W(l1c.z));                                                  // 0049582D..0049583C
    Vector lac;
    vector_divide_00437a90(&lac, &la0, fv(self, 0xD0));                              // 00495809..00495843 (divisor = [+D0] DWORD)
    {
        Vector* const out = static_cast<Vector*>(at(obj, 0x27C));                    // 00495867
        const float ny = S(-W(lac.y));                                               // 00495848..00495858 fchs
        const float nx = S(-W(lac.x));                                               // 00495860..00495874 fchs
        out->x = ny;                                                                 // 0049586D
        out->y = lac.z;                                                              // 00495871 (raw DWORD)
        out->z = nx;                                                                 // 0049587C
    }
    Vector ld0d;
    const float* p4 = static_cast<const float*>(gob_position(target, &ld0d));       // 00495883..00495896
    Vector* const d = static_cast<Vector*>(at(obj, 0x1F8));                          // 004958A3
    d->x = S(W(p4[0]) - W(l1c.x));                                                   // 00495899..004958CD
    d->y = S(W(p4[1]) - W(l1c.y));                                                   // 004958B7..004958CF
    d->z = S(W(p4[2]) - W(l1c.z));                                                   // 004958C2..004958DA
    *static_cast<Vector*>(at(obj, 0x204)) = l1c;                                     // 004958A9 / 004958DD..004958EE
}
} // namespace

extern "C" {
// 00494DA0 PartEmitter::AnimateParticles(float delta), thiscall RET 4.
void __fastcall part_emitter_animate_particles_00494da0(void* self, void*, float delta) {
    void* const gob = field<void*>(self, 0x40);                                     // 00494DA9
    if (gob != nullptr) {                                                           // 00494DAC
        void* const fader = field<void*>(gob, 0x170);                               // 00494DB0
        if (fader != nullptr && W(fv(fader, 0x94)) < 1.0) return;                 // 00494DBA..00494DCB (test ah,5; jnp: ordered <) -> 00495B09
    }
    void* const node = aurora_node_as_emitter_0043e3c0(field<void*>(self, 4), nullptr); // 00494DD1..00494DDB (EBP; E60)
    Vector e90 = {0.0f, 0.0f, 0.0f};                                                // 00494DEC..00494E02
    if (((uv(node, 0x12C) >> 8) & 1u) != 0 && gob != nullptr && field<U8>(self, 0x1F7) != 0) { // 00494DDD..00494E1E
        e90.x = S(W(fv(gob, 0x78)) - W(fv(self, 0x158)));                           // 00494E20..00494E34
        e90.y = S(W(fv(gob, 0x7C)) - W(fv(self, 0x15C)));                           // 00494E31..00494E55
        e90.z = S(W(fv(gob, 0x80)) - W(fv(self, 0x160)));                           // 00494E45..00494E64
    }
    if (field<U8>(self, 0x1F7) == 0) {                                              // 00494E6B..00494E73
        part_table(self)->s00c(self, nullptr, word_of(at(self, 0x164)), word_of(at(self, 0x1A4))); // 00494E79..00494E8C GetWorld
        void* const g = field<void*>(self, 0x40);                                   // 00494E8F
        if (g != nullptr) {                                                         // 00494E92
            const U flags = uv(node, 0x12C);                                        // 00494E96
            if (((flags >> 10) & 1u) != 0 || ((flags >> 8) & 1u) != 0) {            // 00494E9C..00494EAE
                uv(self, 0x158) = uv(g, 0x78); uv(self, 0x15C) = uv(g, 0x7C); uv(self, 0x160) = uv(g, 0x80); // 00494EB0..00494EC6
                uv(self, 0x194) = uv(g, 0x84); uv(self, 0x198) = uv(g, 0x88);       // 00494EC9..00494EDB
                uv(self, 0x19C) = uv(g, 0x8C); uv(self, 0x1A0) = uv(g, 0x90);       // 00494EDE..00494EE7
            }
        }
        if (((uv(node, 0x12C) >> 10) & 1u) != 0) {                                  // 00494EEE..00494EFA
            copy_words(self, 0x158, 0x164, 3);                                      // 00494EFC..00494F0F
            copy_words(self, 0x194, 0x1A4, 4);                                      // 00494F12..00494F2B
        }
        void* const target = field<void*>(self, 0x1E4);                             // 00494F2E
        if (target != nullptr) animate_target_block(self, target);                  // 00494F34..004958EE
        field<U8>(self, 0x1F7) = 1;                                                 // 004958F1
    }
    if (((uv(node, 0x12C) >> 8) & 1u) == 0) {                                       // 004958F9..00495904
        copy_words(self, 0x170, 0x164, 3);                                          // 00495906..0049591F
        copy_words(self, 0x1B4, 0x1A4, 4);                                          // 00495922..00495941
    }
    if (((uv(node, 0x12C) >> 8) & 1u) != 0) {                                       // 00495944..0049594F
        fv(self, 0x170) = S(W(e90.x) + W(fv(self, 0x164)));                         // 00495951..00495964
        fv(self, 0x174) = S(W(e90.y) + W(fv(self, 0x168)));                         // 0049596C..00495979
        fv(self, 0x178) = S(W(e90.z) + W(fv(self, 0x16C)));                         // 0049597D..00495999
        copy_words(self, 0x1B4, 0x1A4, 4);                                          // 004959A4..004959BD
    }
    part_table(self)->s00c(self, nullptr, word_of(at(self, 0x164)), word_of(at(self, 0x1A4))); // 004959C0..004959D2 GetWorld
    const float* const q = static_cast<const float*>(at(self, 0x1A4));             // 004959C2 (EDI)
    const double q0 = W(q[0]), q1 = W(q[1]), q2 = W(q[2]), q3 = W(q[3]);
    fv(self, 0x128) = S(1.0 - (q2 * q2 + q3 * q3) * 2.0);                           // 004959D5..004959F3
    fv(self, 0x12C) = S((q0 * q3 + q2 * q1) * 2.0);                                 // 004959FF..00495A0E
    fv(self, 0x130) = S((q3 * q1 - q0 * q2) * 2.0);                                 // 00495A12..00495A30
    fv(self, 0x134) = S((q2 * q1 - q0 * q3) * 2.0);                                 // 00495A3B..00495A4A
    fv(self, 0x138) = S(1.0 - (q3 * q3 + q1 * q1) * 2.0);                           // 00495A52..00495A6A
    fv(self, 0x13C) = S((q0 * q1 + q2 * q3) * 2.0);                                 // 00495A76..00495A8A
    fv(self, 0x140) = S((q3 * q1 + q2 * q0) * 2.0);                                 // 00495A95..00495AAA
    fv(self, 0x144) = S((q3 * q2 - q0 * q1) * 2.0);                                 // 00495AB2..00495AC1
    fv(self, 0x148) = S(1.0 - (q2 * q2 + q1 * q1) * 2.0);                           // 00495AC5..00495AE8
    part_table(self)->s080(self, nullptr, float_to_bits(delta));                    // 00495AF5..00495B01 vtbl+80 Update(delta)
}
}
