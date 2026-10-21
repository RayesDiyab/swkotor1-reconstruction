// Lane "dialogue-start", fork B2, part 4: CSWBehaviorCameraDialog::GetCameraParameters 006BB7B0 and
// GetSideUnobstructed 006BC600, transcribed from the original swkotor.exe (read-only).
// The frame of GetCameraParameters is modelled as the original [esp+X] slots (q[] indexed by the offset from the
// post-prologue esp, arguments at +11Ch..+140h) so the original slot reuse is kept exactly; FPU stack values that
// the original keeps on the x87 stack are double locals (PC_53), every `fst/fstp dword` is a float rounding.
#include "1289b1b830-claude_dialogue_camera.hpp"

using namespace dlg;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned size); // Allocate
}

extern "C" {
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);
void __fastcall int_list_add_00444fe0(void* list, void*, U value);
void* __fastcall gob_get_property_0x68_0063c070(void* gob, void*);
void* __fastcall aurora_hit_info_construct_004578f0(void* hit, void*, U a, U b, U c);
void __fastcall aurora_hit_info_clear_output_00456a60(void* hit, void*);
U __fastcall kotor_fastcall_00445080(void* list, void*, void* item);
}

namespace {
constexpr U kHookHeight = 0x3dcccccdu;  // [0073E798] 0.1f
constexpr U kOneHalf = 0x3fc00000u;     // [0073F414] 1.5f
constexpr U kHalf = 0x3f000000u;        // [0073E9AC] 0.5f
constexpr U kFifth = 0x3e4ccccdu;       // [0073E46C] 0.2f
constexpr U kTenth = 0x3dcccccdu;       // [0073E798] 0.1f
constexpr U kEyeDrop = 0x3d23d70au;     // [00741C40] 0.04f
constexpr U kOffset15 = 0x3e19999au;    // [0074A604] 0.15f
constexpr U kNinety = 0x42b40000u;      // [0073E40C] 90.0f

// The inlined CExoArrayList<int>::Add of a CHitInfo ignore list (data +8, size +C, allocated +10 of the hit info)
void hit_list_add_inline(unsigned char* hit, U value) {
    if (rd(hit, 0xc) == rd(hit, 0x10)) {
        const U allocated = rd(hit, 0x10);
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(hit + 8), nullptr,
                                            allocated ? allocated + allocated : 0x10u);
    }
    wr(rp(hit, 8), rd(hit, 0xc) * 4, value);
    wr(hit, 0xc, rd(hit, 0xc) + 1);
}

void hit_ignore_participant(unsigned char* hit, U id) {
    void* object = kotor_fastcall_005ed580(client(), nullptr, id);
    if (!object || !vcall<void*>(object, 0xc)) return;
    if (!gob_get_property_0x68_0063c070(vcall<void*>(object, 0xc), nullptr)) return;
    void* p = gob_get_property_0x68_0063c070(vcall<void*>(object, 0xc), nullptr);
    const U item = vcall<U>(p, 0x8, 0xfeu);
    if (item) int_list_add_00444fe0(hit + 8, nullptr, item);
}

// The inlined ~CHitInfo (006BC04D..006BC08B / 006BCE45..006BCE93): release the +38 dependency, free the list data
void hit_destroy_inline(unsigned char* hit) {
    if (rd(hit, 0x38)) (void)kotor_fastcall_00445080(at(rp(hit, 0x38), 4), nullptr, hit + 0x38);
    if (rd(hit, 8)) live_free(rp(hit, 8));
}

V3 load3(const U* q, U off) { return V3{q[off / 4], q[off / 4 + 1], q[off / 4 + 2]}; }
}  // namespace

extern "C" {

// 006BB7B0 GetCameraParameters(type, side, Vector a, Vector b, Vector* position, Quaternion* orientation) (RET 28h)
void __fastcall kotor_fastcall_006bb7b0(void* self, void*, U type, U side, V3 a, V3 b, void* out_position,
                                        void* out_orientation) {
    U q[0x150 / 4] = {};                                                                   // [esp+00h..14Ch]
    auto F = [&q](U off) -> U& { return q[off / 4]; };
    auto L = [&q](U off) -> double { return fl(q[off / 4]); };
    auto S = [&q](U off, double v) { q[off / 4] = fs(v); };
    F(0x11c) = type; F(0x120) = side;
    F(0x124) = a.x; F(0x128) = a.y; F(0x12c) = a.z;
    F(0x130) = b.x; F(0x134) = b.y; F(0x138) = b.z;
    void* const edi = out_position;                                                        // [esp+13Ch]
    F(0x40) = 0; F(0x44) = 0; F(0x48) = 0;                                                 // 006BB7E1..006BB7F1
    if (aurora_vector_equal_004aa980(&F(0x124), &F(0x130))) {                              // 006BB7CF..006BB808
        wr(self, 0x34, 1);                                                                 // 006BB811
        const double z = L(0x12c) + fl(kHookHeight);                                       // 006BB80A..006BB814
        const double y = L(0x128) + fl(kOneHalf);                                          // 006BB81A..006BB821
        const double x = L(0x124) + fl(kOneHalf);                                          // 006BB827..006BB82E
        S(0x1c, x); F(0x130) = F(0x1c);                                                    // 006BB834..006BB83C
        S(0x20, y); F(0x134) = F(0x20);                                                    // 006BB843..006BB84B
        S(0x24, z); F(0x138) = F(0x24);                                                    // 006BB852..006BB85A
    }
    // 006BB861: per-type framing constants
    if (type == 2) {
        F(0x54) = 0x3e99999au; F(0x2c) = 0x3f4ccccdu; F(0x64) = 0xbdcccccdu; F(0x68) = 0xbd4ccccdu; // 0.3 0.8 -0.1 -0.05
        F(0x4c) = 0x41f00000u; F(0x50) = 0; F(0x6c) = 0;                                   // 30.0
    } else {
        F(0x68) = 0; F(0x64) = 0;                                                          // 006BB870..006BB878
        if (type == 3) {
            F(0x54) = 0x3f000000u; F(0x2c) = 0x3fc00000u; F(0x4c) = 0x42b40000u;           // 0.5 1.5 90.0
            F(0x50) = 0xbe4ccccdu; F(0x6c) = 0x3e99999au;                                  // -0.2 0.3
        } else {
            F(0x54) = 0x3f800000u; F(0x2c) = 0x3f000000u;                                  // 1.0 0.5
            F(0x4c) = 0x41f00000u; F(0x50) = 0; F(0x6c) = 0;                               // 30.0
        }
    }
    if (side == 2) S(0x4c, -L(0x4c));                                                      // 006BB8F6..006BB908 fchs
    double carry;                                                                          // value left on ST0 at the joins
    if (rd(self, 0x38) != 0) {                                                             // 006BB90C..006BB911
        F(0x3c) = rd(self, 0x14);                                                          // 006BB919..006BB91C
        F(0x28) = rd(self, 0x18);                                                          // 006BB920..006BB923
        if (type != 1) {
            S(0x10, L(0x130) - L(0x124));                                                  // 006BB92D..006BB964 (B - A)
            S(0x14, L(0x134) - L(0x128));
            S(0x18, L(0x138) - L(0x12c));
            const double mag = vector_magnitude_004ab100(&F(0x10));                        // 006BB968
            const double k64 = L(0x64) * mag;                                              // 006BB96D..006BB974
            const double k68 = mag * L(0x68);                                              // 006BB976..006BB978
            S(0x50, mag * L(0x50) + fl(rd(self, 0x20)));                                   // 006BB97C..006BB985
            S(0x68, mag * L(0x6c) + fl(rd(self, 0x1c)));                                   // 006BB989..006BB992
            S(0x138, (L(0x138) + L(0x28)) + k68);                                          // 006BB996..006BB9A3
            S(0x12c, (L(0x12c) + L(0x3c)) + k64);                                          // 006BB9AC..006BB9B9
            const double dz = L(0x12c) - L(0x138);                                         // 006BB9C2..006BB9C9
            F(0x28) = fs(dz);                                                              // 006BB9D0 fst
            const double dy = L(0x128) - L(0x134);                                         // 006BB9D4..006BB9DB
            const double dx = L(0x124) - L(0x130);                                         // 006BB9E4..006BB9EB
            F(0x64) = fs(dx);                                                              // 006BB9F2 fst
            S(0x28, L(0x28) * L(0x54));                                                    // 006BB9F6..006BBA06
            F(0x10) = F(0x64);                                                             // 006BB9FA..006BBA02
            S(0x3c, dy * L(0x54));                                                         // 006BBA0A..006BBA10
            const double dxk = dx * L(0x54);                                               // 006BBA14
            const double mz = L(0x28) + L(0x138);                                          // 006BBA18..006BBA1C
            const double my = L(0x134) + L(0x3c);                                          // 006BBA23..006BBA2A
            const double mx = dxk + L(0x130);                                              // 006BBA2E..006BBA30
            S(0x1c, mx); F(0x30) = F(0x1c);                                                // 006BBA37..006BBA41
            S(0x20, my); F(0x34) = F(0x20);                                                // 006BBA45..006BBA4D
            S(0x24, mz); F(0x38) = F(0x24);                                                // 006BBA51..006BBA5D
            S(0x38, mz + L(0x50));                                                         // 006BBA59..006BBA65
            S(0x28, dz); F(0x18) = F(0x28);                                                // 006BBA6B..006BBA77
            S(0x14, dy);                                                                   // 006BBA73
            Q4 yaw_q;                                                                      // [esp+70h]
            const void* rq = yaw_pitch_roll_004acac0(&yaw_q, bitsf(F(0x4c)), 0.0f, 0.0f);  // 006BBA7B..006BBA8D
            const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(&F(0x1c), rq, &F(0x10))); // ..006BBA9B
            F(0x40) = r->x; F(0x44) = r->y; F(0x48) = r->z;                                // 006BBAA0..006BBABD
            if (type == 2) {                                                               // 006BBABA..006BBAC1
                const V3* n = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BBAC7..006BBAD1
                const double nzk = fl(n->z) * fl(kOffset15);                               // 006BBAD6..006BBAD9
                const double nyk = fl(n->y) * fl(kOffset15);                               // 006BBAE2..006BBAE5
                S(0x10, fl(n->x) * fl(kOffset15));                                         // 006BBAEB..006BBAF5
                const double rzs = L(0x48) * L(0x2c);                                      // 006BBAF9..006BBAFD
                S(0x28, L(0x44) * L(0x2c));                                                // 006BBB01..006BBB09
                S(0x58, L(0x40) * L(0x2c));                                                // 006BBB0D..006BBB15
                S(0x3c, rzs + nzk);                                                        // 006BBB19..006BBB1B
                S(0x28, L(0x28) + nyk);                                                    // 006BBB1F..006BBB25
                S(0x58, L(0x58) + L(0x10));                                                // 006BBB2D..006BBB35
                const double z = L(0x38) - L(0x3c);                                        // 006BBB39..006BBB3D
                const double y = L(0x34) - L(0x28);                                        // 006BBB41..006BBB45
                const double x = L(0x30) - L(0x58);                                        // 006BBB49..006BBB4D
                S(0x1c, x); wr(edi, 0, F(0x1c));                                           // 006BBB51..006BBB59
                S(0x20, y); wr(edi, 4, F(0x20));                                           // 006BBB5B..006BBB63
                S(0x24, z);                                                                // 006BBB66
                carry = L(0x68);                                                           // 006BBB6E
                wr(edi, 8, F(0x24));                                                       // 006BBB72
            } else if (type == 3) {                                                        // 006BBB7A..006BBB7D
                const double rzs = L(0x48) * L(0x2c);                                      // 006BBB7F..006BBB85
                S(0x28, L(0x44) * L(0x2c));                                                // 006BBB89..006BBB91
                S(0x58, L(0x40) * L(0x2c));                                                // 006BBB95..006BBB9D
                const double z = L(0x38) - rzs;                                            // 006BBBA1 fsubr
                const double y = L(0x34) - L(0x28);                                        // 006BBBA5..006BBBA9
                const double x = L(0x30) - L(0x58);                                        // 006BBBAD..006BBBB1
                S(0x1c, x); wr(edi, 0, F(0x1c));
                S(0x20, y); wr(edi, 4, F(0x20));
                S(0x24, z); wr(edi, 8, F(0x24));                                           // 006BBBCA..006BBBD2
                carry = L(0x68);                                                           // 006BBBD5
            } else {
                carry = L(0x68);                                                           // 006BBBD5
            }
        } else {
            // 006BBBDE: type 1
            F(0x4c) = side == 2 ? 0xc1f00000u : 0x41f00000u;                               // -30.0 / 30.0
            S(0x12c, L(0x12c) + L(0x3c));                                                  // 006BBBF3..006BBC08
            S(0x138, L(0x138) + L(0x3c));                                                  // 006BBC0F..006BBC1A (the +14 height)
            S(0x10, L(0x124) - L(0x130));                                                  // 006BBC21..006BBC53 (A - B)
            S(0x14, L(0x128) - L(0x134));
            S(0x18, L(0x12c) - L(0x138));
            const V3* n = static_cast<const V3*>(normalize_004ab180(&F(0x1c), &F(0x10)));  // 006BBC57
            const double nzk = fl(n->z) * fl(kFifth);                                      // 006BBC5C..006BBC62
            S(0x28, fl(n->y) * fl(kFifth));                                                // 006BBC6C..006BBC7A
            S(0x10, fl(n->x) * fl(kFifth));                                                // 006BBC7E..006BBC86
            const double pz = L(0x12c) - nzk;                                              // 006BBC8A fsubr
            const double py = L(0x128) - L(0x28);                                          // 006BBC91..006BBC98
            const double px = L(0x124) - L(0x10);                                          // 006BBC9C..006BBCA3
            S(0x1c, px); F(0x30) = F(0x1c);                                                // 006BBCA7..006BBCAF
            S(0x20, py); F(0x34) = F(0x20);                                                // 006BBCB3..006BBCBB
            S(0x24, pz); F(0x38) = F(0x24);                                                // 006BBCBF..006BBCD1
            S(0x38, (pz - fl(kEyeDrop)) + fl(rd(self, 0x20)));                             // 006BBCC7..006BBCE1
            S(0x10, L(0x124) - L(0x130));                                                  // 006BBCE5..006BBD17 (A - B)
            S(0x14, L(0x128) - L(0x134));
            S(0x18, L(0x12c) - L(0x138));
            Q4 yaw_q;
            const void* rq = yaw_pitch_roll_004acac0(&yaw_q, bitsf(F(0x4c)), 0.0f, 0.0f);  // 006BBD1B
            const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(&F(0x1c), rq, &F(0x10))); // 006BBD29
            F(0x40) = r->x; F(0x44) = r->y; F(0x48) = r->z;                                // 006BBD2E..006BBD48
            const V3* n1 = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BBD4C
            F(0x40) = n1->x; F(0x44) = n1->y; F(0x48) = n1->z;                             // 006BBD51..006BBD6B
            const V3* n2 = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BBD6F
            const double nz2 = fl(n2->z) * fl(kHalf);                                      // 006BBD74..006BBD77
            S(0x28, fl(n2->y) * fl(kHalf));                                                // 006BBD84..006BBD90
            S(0x58, fl(n2->x) * fl(kHalf));                                                // 006BBD94..006BBD9E
            const double z = L(0x38) - nz2;                                                // 006BBDA2 fsubr
            const double y = L(0x34) - L(0x28);
            const double x = L(0x30) - L(0x58);
            S(0x1c, x); wr(edi, 0, F(0x1c));
            S(0x20, y); wr(edi, 4, F(0x20));
            S(0x24, z); wr(edi, 8, F(0x24));                                               // 006BBDB6..006BBDD3
            carry = fl(rd(self, 0x1c));                                                    // 006BBDD6
        }
        wr(edi, 8, fs(carry + fl(rd(edi, 8))));                                            // 006BBDD9..006BBDDE
        if (rd(self, 0x34) != 0) {                                                         // 006BBDE1..006BBDE6
            alignas(4) unsigned char hit[0x64];                                            // [esp+80h]
            F(0x1c) = 0; F(0x20) = 0; F(0x24) = 0;                                         // 006BBDFA..006BBE02
            (void)aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xbfffffffu, 0);    // 006BBDEC..006BBE06
            wr(hit, 0, 0);                                                                 // 006BBE20
            hit_list_add_inline(hit, rd(self, 0x64));                                      // 006BBE0B..006BBE57
            hit_list_add_inline(hit, rd(self, 0x68));                                      // 006BBE5E..006BBEAA
            hit_ignore_participant(hit, rd(self, 0x5c));                                   // 006BBEA6..006BBF08
            hit_ignore_participant(hit, rd(self, 0x60));                                   // 006BBF0D..006BBF67
            const V3 to = *static_cast<const V3*>(edi);
            const V3 from = load3(q, 0x30);                                                // (ebp, [esp+34h], [esp+38h])
            if (vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, from, to, &F(0x1c), hit)) { // 006BBF6C..006BBFB9
                S(0x10, L(0x1c) - L(0x30));                                                // 006BBFBF..006BBFE9 (hit - from)
                S(0x14, L(0x20) - L(0x34));
                S(0x18, L(0x24) - L(0x38));
                const V3* n = static_cast<const V3*>(normalize_004ab180(&F(0x58), &F(0x10))); // 006BBFED
                const double nz = fl(n->z) * fl(kTenth);                                   // 006BBFF2..006BBFF7
                S(0x28, fl(n->y) * fl(kTenth));                                            // 006BC000..006BC009
                S(0x58, fl(n->x) * fl(kTenth));                                            // 006BC00D..006BC015
                const double z = L(0x24) - nz;                                             // 006BC019 fsubr
                const double y = L(0x20) - L(0x28);
                const double x = L(0x1c) - L(0x58);
                S(0x10, x); wr(edi, 0, F(0x10));
                S(0x14, y); wr(edi, 4, F(0x14));
                S(0x18, z); wr(edi, 8, F(0x18));                                           // 006BC02D..006BC04A
            }
            hit_destroy_inline(hit);                                                       // 006BC04D..006BC08B
        }
    } else {
        // 006BC090: no camera hooks
        if (type != 1 && rd(self, 0x34) != 1) {                                            // 006BC090..006BC09B
            S(0x10, L(0x130) - L(0x124));                                                  // 006BC0A1..006BC0D8 (B - A)
            S(0x14, L(0x134) - L(0x128));
            S(0x18, L(0x138) - L(0x12c));
            const double mag = vector_magnitude_004ab100(&F(0x10));                        // 006BC0DC
            S(0x3c, L(0x6c) * mag + fl(rd(self, 0x1c)));                                   // 006BC0E1..006BC0ED
            S(0x138, L(0x68) * mag + L(0x138));                                            // 006BC0F1..006BC0FE
            S(0x12c, L(0x64) * mag + L(0x12c));                                            // 006BC105..006BC112
            const double dx = L(0x124) - L(0x130);                                         // 006BC119..006BC120
            F(0x10) = fs(dx);                                                              // 006BC127 fst
            const double dy = L(0x128) - L(0x134);                                         // 006BC12B..006BC132
            const double dz = L(0x12c) - L(0x138);                                         // 006BC13B..006BC142
            F(0x28) = fs(dz);                                                              // 006BC149 fst
            S(0x10, L(0x10) * L(0x54));                                                    // 006BC14D..006BC155
            S(0x14, dy * L(0x54));                                                         // 006BC159..006BC15F
            const double dzk = dz * L(0x54);                                               // 006BC163
            S(0x1c, L(0x10) + L(0x130)); F(0x30) = F(0x1c);                                // 006BC167..006BC17E
            const double my = L(0x14) + L(0x134);                                          // 006BC17A..006BC182
            F(0x18) = F(0x28);                                                             // 006BC189..006BC18D
            S(0x20, my); F(0x34) = F(0x20);                                                // 006BC191..006BC1A0
            const double mz = dzk + L(0x138);                                              // 006BC199
            S(0x24, mz); F(0x38) = F(0x24);                                                // 006BC1A9..006BC1B7
            S(0x38, (mag * L(0x50) + fl(rd(self, 0x20))) + L(0x24));                       // 006BC1B1..006BC1CB
            S(0x10, dx);                                                                   // 006BC1CF
            S(0x14, dy);                                                                   // 006BC1D3
            Q4 yaw_q;
            const void* rq = yaw_pitch_roll_004acac0(&yaw_q, bitsf(F(0x4c)), 0.0f, 0.0f);  // 006BC1BB..006BC1DF
            const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(&F(0x1c), rq, &F(0x10))); // 006BC1ED
            F(0x40) = r->x; F(0x44) = r->y; F(0x48) = r->z;                                // 006BC1F2..006BC20F
            if (type == 2) {
                const V3* n = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BC219..006BC223
                const double nxk = fl(n->x) * fl(kOffset15);                               // 006BC228..006BC22A
                const double nyk = fl(n->y) * fl(kOffset15);                               // 006BC232..006BC238
                S(0x60, fl(n->z) * fl(kOffset15));                                         // 006BC23E..006BC247
                const double rxs = L(0x40) * L(0x2c);                                      // 006BC24B..006BC24F
                S(0x14, L(0x44) * L(0x2c));                                                // 006BC253..006BC25B
                S(0x18, L(0x48) * L(0x2c));                                                // 006BC25F..006BC267
                S(0x1c, rxs + nxk);                                                        // 006BC26B..006BC26D
                const double ysum = L(0x14) + nyk;                                         // 006BC271..006BC277
                const double zsum = L(0x18) + L(0x60);                                     // 006BC27B..006BC27F
                S(0x10, L(0x30) - L(0x1c)); wr(edi, 0, F(0x10));                           // 006BC283..006BC297
                S(0x14, L(0x34) - ysum); wr(edi, 4, F(0x14));                              // 006BC293..006BC2A9
                S(0x18, L(0x38) - zsum); wr(edi, 8, F(0x18));                              // 006BC29F..006BC2B6
            } else if (type == 3) {
                const double rxs = L(0x40) * L(0x2c);                                      // 006BC2CC..006BC2D2
                const double rys = L(0x44) * L(0x2c);                                      // 006BC2D6..006BC2DA
                S(0x60, L(0x48) * L(0x2c));                                                // 006BC2DE..006BC2E6
                S(0x10, L(0x30) - rxs); wr(edi, 0, F(0x10));                               // 006BC2EA..006BC2FC
                S(0x14, L(0x34) - rys); wr(edi, 4, F(0x14));                               // 006BC2F8..006BC30A
                S(0x18, L(0x38) - L(0x60)); wr(edi, 8, F(0x18));                           // 006BC30F..006BC31F
            }
            wr(edi, 8, fs(L(0x3c) + fl(rd(edi, 8))));                                      // 006BC2BB/006BC322..006BC545
        } else {
            // 006BC32E: type 1 or the camera already obstructed
            F(0x4c) = side == 2 ? 0xc1f00000u : 0x41f00000u;                               // -30.0 / 30.0
            S(0x10, L(0x124) - L(0x130));                                                  // 006BC343..006BC361
            F(0x138) = F(0x12c);                                                           // 006BC34A / 006BC36D (b.z = a.z)
            F(0x18) = 0;                                                                   // 006BC37B
            S(0x14, L(0x128) - L(0x134));                                                  // 006BC366..006BC383
            const V3* n = static_cast<const V3*>(normalize_004ab180(&F(0x1c), &F(0x10)));  // 006BC387
            const double nxk = fl(n->x) * fl(kFifth);                                      // 006BC38C..006BC38E
            const double nyk = fl(n->y) * fl(kFifth);                                      // 006BC397..006BC39A
            S(0x18, fl(n->z) * fl(kFifth));                                                // 006BC3A0..006BC3A9
            S(0x1c, L(0x124) - nxk); F(0x30) = F(0x1c);                                    // 006BC3AD..006BC3C5
            S(0x20, L(0x128) - nyk); F(0x34) = F(0x20);                                    // 006BC3BE..006BC3DC
            S(0x24, L(0x12c) - L(0x18));                                                   // 006BC3E6..006BC3F4
            F(0x38) = F(0x24);                                                             // 006BC3FC..006BC406
            S(0x38, (L(0x24) - fl(kEyeDrop)) + fl(rd(self, 0x20)));                        // 006BC3F8..006BC415
            S(0x10, L(0x124) - L(0x130));                                                  // 006BC419..006BC44B (A - B)
            S(0x14, L(0x128) - L(0x134));
            S(0x18, L(0x12c) - L(0x138));
            Q4 yaw_q;
            const void* rq = yaw_pitch_roll_004acac0(&yaw_q, bitsf(F(0x4c)), 0.0f, 0.0f);  // 006BC44F
            const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(&F(0x1c), rq, &F(0x10))); // 006BC45D
            F(0x40) = r->x; F(0x44) = r->y; F(0x48) = r->z;                                // 006BC462..006BC472
            const V3* n1 = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BC476..006BC480
            F(0x40) = n1->x; F(0x44) = n1->y; F(0x48) = n1->z;                             // 006BC48B..006BC49B
            const double range = rd(self, 0x34) != 0 ? 0.0 : fl(rd(self, 0x24));           // 006BC485..006BC4AB
            S(0x50, fl(kFifth) * range);                                                   // 006BC4AE..006BC4C0
            S(0x38, L(0x38) - L(0x50));                                                    // 006BC4C4..006BC4CC
            S(0x3c, range + fl(kHalf));                                                    // 006BC4D0..006BC4D6
            const V3* n2 = static_cast<const V3*>(normalize_004ab180(&F(0x10), &F(0x40))); // 006BC4DA
            const double sx = L(0x3c) * fl(n2->x);                                         // 006BC4DF..006BC4E3
            const double sy = L(0x3c) * fl(n2->y);                                         // 006BC4EC..006BC4F3
            S(0x60, L(0x3c) * fl(n2->z));                                                  // 006BC4F6..006BC4FF
            S(0x10, L(0x30) - sx); wr(edi, 0, F(0x10));                                    // 006BC503..006BC515
            S(0x14, L(0x34) - sy); wr(edi, 4, F(0x14));                                    // 006BC511..006BC523
            S(0x18, L(0x38) - L(0x60)); wr(edi, 8, F(0x18));                               // 006BC528..006BC538
            wr(edi, 8, fs((fl(rd(self, 0x1c)) + fl(rd(edi, 8))) + L(0x50)));               // 006BC53B..006BC545
        }
    }
    // 006BC548: orientation looking from the camera position at the framed point
    S(0x58, L(0x30) - fl(rd(edi, 0)));                                                     // 006BC548..006BC55C
    S(0x5c, L(0x34) - fl(rd(edi, 4)));                                                     // 006BC561..006BC568
    S(0x60, L(0x38) - fl(rd(edi, 8)));                                                     // 006BC56C..006BC573
    const void* look = normalize_004ab180(&F(0xe4), &F(0x58));                             // 006BC577
    const float pitch = static_cast<float>(kotor_m15a_pitch_004aa130(look) + fl(kNinety)); // 006BC57C..006BC58F
    const void* heading = normalize_004ab180(&F(0xf0), &F(0x40));                          // 006BC592..006BC59B
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(heading));                // 006BC5A0..006BC5A6
    const Q4* result = static_cast<const Q4*>(yaw_pitch_roll_004acac0(&F(0xfc), yaw, pitch, 0.0f)); // 006BC5AD..006BC5B5
    std::memcpy(out_orientation, result, 16);                                              // 006BC5BA..006BC5D9
}

// 006BC600 GetSideUnobstructed() -> 1 or 2 (RET 0): counts the obstructed tests of both line-of-action sides
U __fastcall kotor_fastcall_006bc600(void* self, void*) {
    V3 hook{0, 0, 0};                                                                      // [esp+50h]
    Q4 orientation{0x3f800000u, 0, 0, 0};                                                  // [esp+5Ch]
    V3 a, b;                                                                               // [esp+10h], [esp+1Ch]
    (void)vcall<void*>(rp(self, 0x64), 0x64, &a);                                          // 006BC620..006BC663
    (void)vcall<void*>(rp(self, 0x68), 0x64, &b);                                          // 006BC666..006BC670
    a.z = fs(fl(a.z) + fl(kHookHeight));                                                   // 006BC673..006BC684
    V3 mid;                                                                                // [esp+44h]..4Ch
    {
        const double bz = fl(b.z) + fl(kHookHeight);                                       // 006BC688..006BC68C
        b.z = fs(bz);                                                                      // 006BC692 fst
        const double dz = bz - fl(a.z);                                                    // 006BC696
        const double dy = fl(b.y) - fl(a.y);                                               // 006BC69A..006BC69E
        const double dx = fl(b.x) - fl(a.x);                                               // 006BC6A2..006BC6A6
        U t44 = fs(dx);                                                                    // 006BC6AA
        U t34 = fs(dz * fl(kHalf));                                                        // 006BC6AE..006BC6B6
        const double hy = dy * fl(kHalf);                                                  // 006BC6BA
        t44 = fs(fl(t44) * fl(kHalf));                                                     // 006BC6C0..006BC6CA
        t34 = fs(fl(t34) + fl(a.z));                                                       // 006BC6CE..006BC6D6
        const double my = hy + fl(a.y);                                                    // 006BC6DA
        mid.x = fs(fl(t44) + fl(a.x));                                                     // 006BC6DE..006BC6E6
        mid.y = fs(my);                                                                    // 006BC6EA
        mid.z = fs(((fl(rd(self, 0x14)) - fl(rd(self, 0x18))) * fl(kHalf) + fl(rd(self, 0x14))) + fl(t34)); // ..006BC701
    }
    if (rd(self, 0x38) == 0) {                                                             // 006BC67D..006BC705
        a.z = fs(fl(a.z) + fl(rd(self, 0x14)));                                            // 006BC70B..006BC71E
        b.z = fs(fl(b.z) + fl(rd(self, 0x18)));                                            // 006BC723..006BC72F
        if (vcall<U>(rp(self, 0x64), 0x98, "CAMERAHOOK", &hook, &orientation)) a = hook;   // 006BC733..006BC753
        if (vcall<U>(rp(self, 0x68), 0x98, "CAMERAHOOK", &hook, &orientation)) b = hook;   // 006BC757..006BC789
    }
    V3 position{0, 0, 0};                                                                  // [esp+28h]..30h
    V3 hit_out{0, 0, 0};                                                                   // [esp+38h]..40h
    alignas(4) unsigned char hit[0x64];                                                    // [esp+6Ch]
    (void)aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xbfffffffu, 0);            // 006BC78D..006BC7C8
    wr(hit, 0, 0);                                                                         // 006BC7DF
    hit_list_add_inline(hit, rd(self, 0x64));                                              // 006BC7CD..006BC80B
    hit_list_add_inline(hit, rd(self, 0x68));                                              // 006BC80F..006BC84B
    hit_ignore_participant(hit, rd(self, 0x5c));                                           // 006BC841..006BC89E
    hit_ignore_participant(hit, rd(self, 0x60));                                           // 006BC8A3..006BC8FA
    auto test = [&](const V3& from) -> U {
        const U r = vcall<U>(vcall<void*>(rp(self, 0x10), 0x104), 0x38, from, position, &hit_out, hit);
        aurora_hit_info_clear_output_00456a60(hit, nullptr);
        return r;
    };
    // side 2 (edi)
    kotor_fastcall_006bb7b0(self, nullptr, 3, 2, a, b, &position, &orientation);           // 006BC8FF..006BC941
    I side2 = static_cast<I>(test(mid));                                                   // 006BC946..006BC99A
    if (rd(self, 0x38) == 0) {                                                             // 006BC99F..006BC9A4
        side2 += static_cast<I>(test(a));                                                  // 006BC9AA..006BC9FC
        side2 += static_cast<I>(test(b));                                                  // 006BCA01..006BCA4D
    }
    kotor_fastcall_006bb7b0(self, nullptr, 2, 2, a, b, &position, &orientation);           // 006BCA52..006BCA94
    side2 += static_cast<I>(test(a));                                                      // 006BCA99..006BCAED
    kotor_fastcall_006bb7b0(self, nullptr, 2, 1, b, a, &position, &orientation);           // 006BCAF2..006BCB34
    side2 += static_cast<I>(test(b));                                                      // 006BCB39..006BCB8D
    // side 1 (ebx)
    kotor_fastcall_006bb7b0(self, nullptr, 3, 1, a, b, &position, &orientation);           // 006BCB92..006BCBD4
    I side1 = static_cast<I>(test(mid));                                                   // 006BCBD9..006BCC2D
    if (rd(self, 0x38) == 0) {                                                             // 006BCC32..006BCC37
        side1 += static_cast<I>(test(a));                                                  // 006BCC3D..006BCC93
        side1 += static_cast<I>(test(b));                                                  // 006BCC98..006BCCE8
    }
    kotor_fastcall_006bb7b0(self, nullptr, 2, 1, a, b, &position, &orientation);           // 006BCCED..006BCD2F
    side1 += static_cast<I>(test(a));                                                      // 006BCD34..006BCD88
    kotor_fastcall_006bb7b0(self, nullptr, 2, 2, b, a, &position, &orientation);           // 006BCD8D..006BCDCF
    side1 += static_cast<I>(test(b));                                                      // 006BCDD4..006BCE2A
    hit_destroy_inline(hit);                                                               // 006BCE31..006BCE93
    return side2 > side1 ? 1u : 2u;                                                        // 006BCE2F..006BCE96 (jle)
}

}  // extern "C"
