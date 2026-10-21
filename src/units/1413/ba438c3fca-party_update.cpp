// Lane W4 (party fork): 00637050 CSWParty::Update and the CSWPartyFollowDataArray / follow-point helpers.
// Transcribed from dis/00637050.asm, dis/00634DA0.asm, dis/00634E00.asm, dis/00634CC0.asm, dis/00634D40.asm,
// dis/00634A70.asm, dis/006350F0.asm, dis/006359C0.asm, dis/00636840.asm, dis/006346F0.asm.
// x87 policy (w4_native.hpp): FPU-stack values are `double` (PC53), every `fstp dword` an explicit float store, in
// the original evaluation order; compares use the exact fnstsw/test/jcc predicate (jcc_* helpers).
#include "7b616bc567-party.hpp"

using namespace w4;

namespace w4 {
volatile U32 follow_formation_override_007a2414 = 0xFFFFFFFFu;   // [007A2414] .data, never written by the original
}

namespace {
const float k_deg2rad_007415f8 = 0.0174532924f;   // 3C8EFA35
const float k_half_0073e9ac = 0.5f, k_one_0073d764 = 1.0f, k_zero_0073d700 = 0.0f;
const float k_hundred_00741838 = 100.0f, k_49_00747c3c = 49.0f, k_five_default = 5.0f;   // 40A00000 at 00634A8D
const double k_25_007514e0 = 25.0;
const char k_retreat_007514e8[] = "k_hen_retreat";
const char k_exit5m_007514d0[] = "k_hen_exit5m";
const char k_enter5m_007514c0[] = "k_hen_enter5m";
const char k_primary_00746f28[] = "PrimaryRange";
const char k_secondary_00746f38[] = "SecondaryRange";
const char k_bad_path_00751480[] = "PathFollowData requesting bad data position %d";
void* rules_float_table() { return lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0xC4); }  // [[[007A3A28]+B8h]+C4h]
U8* at(void* base, U32 offset) { return static_cast<U8*>(base) + offset; }
W4Vector load_vector(const void* p) { W4Vector v; std::memcpy(&v, p, 12); return v; }
void store_vector(void* p, const W4Vector& v) { std::memcpy(p, &v, 12); }
// Squared length in the original's evaluation order ((a*a + b*b) + c*c); a = first operand loaded last (ST0).
double sum_squares(double a, double b, double c) { return (a * a + b * b) + c * c; }
}

// 00634CC0 CSWPartyFollowDataArray::GetPathData(int) (thiscall, RET 4).
extern "C" void* __fastcall kotor_fastcall_00634cc0(void* follow, void*, int index) {
    if (index < 0 || index >= 100) {                                                         // 00634CDD..00634CE7
        U32 message[2];                                                                      // [esp+8] CExoString
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(message), nullptr); // 00634CE9..00634CED
        frame_LogFormat_005e5680(message, k_bad_path_00751480, index);                       // 00634CF2..00634D05
        frame_LogStringDtor_005e5c20(message, nullptr);                                      // 00634D0D..00634D19
    }
    return at(follow, static_cast<U32>(index) * 0x28u);                                      // 00634D1E..00634D21
}

namespace {
// Shared body of 00634DA0 / 00634D40 (identical except setge / setle at 00634DF6 / 00634D96).
void member_path_positions(void* follow, I32* first, I32* second) {
    void* party = lp(follow, 0xFA0);                                                         // ...DA0 / ...D40
    I32 a = -1, b = -1;
    if (lw(party, 0xAC) != 0x7F000000u) a = li(party, 0xD0);                                 // DA7..DBC
    if (lw(party, 0x134) != 0x7F000000u) b = li(party, 0x158);                               // DC2..DCE
    const I32 head = li(follow, 0xFA4);                                                      // DD4
    if (head > a && a != -1) a += 100;                                                       // DDA..DE3
    if (head > b && b != -1) b += 100;                                                       // DE6..DEF
    *first = a; *second = b;
}
}
// 00634DA0 CSWPartyFollowDataArray::FindFurthestPartyMemeber_Current() (thiscall, RET 0).
extern "C" int __fastcall kotor_fastcall_00634da0(void* follow, void*) {
    I32 a, b; member_path_positions(follow, &a, &b);
    return (a >= b ? 1 : 0) + 1;                                                             // 00634DF2..00634DFB
}
// 00634D40 CSWPartyFollowDataArray::FindClosestPartyMemeber_Current() (thiscall, RET 0).
extern "C" int __fastcall kotor_fastcall_00634d40(void* follow, void*) {
    I32 a, b; member_path_positions(follow, &a, &b);
    return (a <= b ? 1 : 0) + 1;                                                             // 00634D92..00634D9B
}

// 00634E00 CSWPartyFollowDataArray::PointOnLineDistanceFromLeader(int index, int squared) (thiscall, RET 8;
// result in ST0, consumed unrounded by the caller -> double).
extern "C" double __fastcall kotor_fastcall_00634e00(void* follow, void*, int index, int squared) {
    void* party = lp(follow, 0xFA0);                                                         // 00634E03
    void* leader = li(party, 0) > 0 ? pt(frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24))) : nullptr; // 00634E09..00634E22
    const U8* point = at(party, static_cast<U32>(index) * 0x88u + 0x3C);                     // 00634E24..00634E34
    const double dx = static_cast<double>(lf(point, 0)) - lf(leader, 0x24);                  // 00634E34..00634E38
    const double dy = static_cast<double>(lf(point, 4)) - lf(leader, 0x28);                  // 00634E3B..00634E3F
    const double dz = static_cast<double>(lf(point, 8)) - lf(leader, 0x2C);                  // 00634E42..00634E46



    const double s = (dz * dz + dx * dx) + dy * dy;                                          // 00634E49..00634E62

    return squared != 0 ? s : x87_fsqrt(s);                                                  // 00634E59..00634E6C
}

// 00634A70 CSWParty::GetFollowRange() (thiscall, RET 0; float returned in ST0 via fld dword).
extern "C" double __fastcall kotor_fastcall_00634a70(void*, void*) {
    float range = k_five_default;                                                            // 00634A8D [esp]
    U32 formation = lw(lane_config_required::GetPartyTable_004aee70(server_app(), nullptr), 0xE8); // 00634A88..00634AA6
    if (follow_formation_override_007a2414 != 0xFFFFFFFFu) formation = follow_formation_override_007a2414; // 00634A9D..00634AAE
    if (formation == 0 || formation == 1) {                                                  // 00634AB0..00634AB6
        U32 column[2];                                                                       // CExoString
        frame_LogStringCtor_005e5a90(column, nullptr, k_primary_00746f28);                   // 00634AFD / 00634ABC
        c2da_current_get_float_00413430(rules_float_table(), nullptr, formation == 0 ? 0x0E : 0x0F, column, &range); // 00634B0B..00634B33 / 00634ACA..00634AF2
        frame_LogStringDtor_005e5c20(column, nullptr);                                       // 00634B38..00634B44
    }
    return range;                                                                            // 00634B4D fld dword [esp]
}

// 006350F0 CSWPartyFollowDataArray::GetPointBackFromStart(float, Vector*, int*, int) (thiscall, RET 10h).
extern "C" int __fastcall kotor_fastcall_006350f0(void* follow, void*, float distance, W4Vector* out, U32* out_index, int member) {
    I32 index = li(follow, 0xFA4) - 1;                                                       // 006350F7..006350FD
    if (index < 0) index += 100;                                                             // 006350FE..00635100
    if (li(follow, 0xFA8) < 3) {                                                             // 00635103..0063510A
        *out_index = 0;                                                                      // 00635114
        *out = W4Vector{0.0f, 0.0f, 0.0f};                                                   // 0063511A..00635144
        return 0;
    }
    W4Vector point = load_vector(kotor_fastcall_00634cc0(follow, nullptr, index));          // 00635150..00635176
    I32 remaining = li(follow, 0xFA8);                                                       // 00635168 ebx
    I32 stop = -1;                                                                           // 0063517A [esp+10h]
    if (member != 0) stop = li(lp(follow, 0xFA0), static_cast<U32>(member) * 0x88u + 0x48); // 00635182..00635198
    float left = distance;                                                                   // 0063519E..006351A2 (arg slot)
    *out_index = static_cast<U32>(index);                                                    // 006351A6..006351AA
    if (remaining < 0) goto fail;                                                            // 0063519C..006351AC
    for (;;) {
        if (jcc_jne(left, k_zero_0073d700, 0x41)) {                                          // 006351B2..006351C1
            if (remaining >= 0) return 1;                                                    // 00635300..00635302 -> 00635280
            goto fail;
        }
        if (member != 0 && index == stop) {                                                  // 006351C7..006351CF
            *out_index = static_cast<U32>(index);                                            // 0063526A..0063526E
            *out = point;                                                                    // 00635270..0063527D
            return 1;
        }
        *out_index = static_cast<U32>(index);                                                // 006351D5..006351D9
        if (--index < 0) index += 100;                                                       // 006351DB..006351DE
        --remaining;                                                                         // 006351E4
        const U8* next = static_cast<const U8*>(kotor_fastcall_00634cc0(follow, nullptr, index)); // 006351E1..006351E5
        const double dz = static_cast<double>(lf(next, 8)) - point.z;                        // 006351EA..006351ED
        const double dy = static_cast<double>(lf(next, 4)) - point.y;                        // 006351F1..006351F4
        const double dx = static_cast<double>(lf(next, 0)) - point.x;                        // 006351F8..006351FA
        W4Vector step;                                                                       // [esp+20h]
        step.x = static_cast<float>(dx);                                                     // 006351FE
        step.y = static_cast<float>(dy);                                                     // 00635202 fst
        step.z = static_cast<float>(dz);                                                     // 00635206..00635208



        const double length2 = (static_cast<double>(step.x) * step.x + dy * dy) + dz * dz;   // 0063520C..0063521E

        const double reach2 = static_cast<double>(left) * left;                              // 00635224..00635228
        if (!jcc_jne(length2, reach2, 0x01)) {                                               // 0063522C..00635235 (je when C0 clear)
            W4Vector unit;
            const W4Vector* n = vector_normalized_004ab180(&unit, &step);                    // 0063528F..0063529B
            const double sx = static_cast<double>(left) * n->x;                              // 006352A0..006352A4
            const double sy = static_cast<double>(left) * n->y;                              // 006352AA..006352B1
            const float sz = static_cast<float>(static_cast<double>(left) * n->z);           // 006352B5..006352BF
            out->x = static_cast<float>(sx + point.x);                                       // 006352C3..006352D1
            out->y = static_cast<float>(sy + point.y);                                       // 006352D3..006352E7
            out->z = static_cast<float>(static_cast<double>(sz) + point.z);                  // 006352DB..006352F7
            return 1;
        }
        left = static_cast<float>(static_cast<double>(left) - x87_fsqrt(length2));           // 00635237..00635240
        point = load_vector(kotor_fastcall_00634cc0(follow, nullptr, index));                // 00635239..0063525B
        if (remaining < 0) break;                                                            // 00635249..0063525F
    }
fail:
    *out = W4Vector{0.0f, 0.0f, 0.0f};                                                       // 00635308..00635338
    return 0;
}

// 006359C0 CSWParty::RecaulateFollowPoint() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_006359c0(void* party, void*) {
    W4Vector point{0.0f, 0.0f, 0.0f};                                                        // [esp+0Ch] 006359D5..006359E5
    U32 index;                                                                               // [esp+8] (always written by the callee)
    void* follow = at(party, 0x2DC);                                                         // 006359ED
    float d = static_cast<float>(kotor_fastcall_00634a70(party, nullptr) - lf(party, 0x1C0)); // 006359F3..00635A01
    if (kotor_fastcall_006350f0(follow, nullptr, d, &point, &index, 1) != 0) {               // 00635A04..00635A0B
        store_vector(at(party, 0xD4), point);                                                // 00635A0D..00635A28
        sw(party, 0xE0, index);                                                              // 00635A2B
    }
    d = static_cast<float>(kotor_fastcall_00634a70(party, nullptr) + lf(party, 0x1C0));      // 00635A3F..00635A4D
    if (kotor_fastcall_006350f0(follow, nullptr, d, &point, &index, 2) != 0) {               // 00635A50..00635A57
        store_vector(at(party, 0x15C), point);                                               // 00635A59..00635A74
        sw(party, 0x168, index);                                                             // 00635A77
    }
}

// 006346F0 CSWParty::InvertFormation() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_006346f0(void* party, void*) {
    sf(party, 0xE4, static_cast<float>(-static_cast<double>(lf(party, 0xE4))));              // 006346F4..00634717 fchs
    sf(party, 0x16C, static_cast<float>(-static_cast<double>(lf(party, 0x16C))));            // 0063471F..00634741 fchs
    sb(party, 0x1C4, lb(party, 0x1C4) == 0 ? 1 : 0);                                         // 00634749..00634755
}

// 00636840 CSWParty::TestAndResetPath() (thiscall, RET 0).
extern "C" int __fastcall kotor_fastcall_00636840(void* party, void*) {
    I32 head = li(party, 0x1280) - 1;                                                        // 00636846..0063684C
    if (head < 0) head += 100;                                                               // 0063684F..00636851
    void* follow = at(party, 0x2DC);                                                         // 00636854
    const float degrees = lf(kotor_fastcall_00634cc0(follow, nullptr, head), 0x0C);         // 0063685A..00636862
    const W4Vector forward{0.0f, 1.0f, 0.0f};                                                // [esp+1Ch] 00636871..00636881
    const W4Vector axis{0.0f, 0.0f, 1.0f};                                                   // [esp+10h] 00636889..00636899
    const float angle = static_cast<float>(deg_to_rad_004a96d0(degrees));                   // 006368A1..006368A6
    U8 rotation[16];                                                                         // [esp+34h]
    void* q = quaternion_from_axis_angle_004ac960(rotation, nullptr, axis, angle);          // 006368A9..006368C6
    W4Vector facing;                                                                         // [esp+28h]
    quaternion_rotate_004a9bb0(&facing, q, &forward);                                        // 006368CB..006368D1
    const int closest = kotor_fastcall_00634d40(follow, nullptr);                            // 006368D9..006368DB
    const W4Vector m = load_vector(at(party, static_cast<U32>(closest) * 0x88u + 0x70));     // 006368E0..006368FE
    const double dot = (static_cast<double>(m.z) * facing.z + static_cast<double>(m.y) * facing.y) +
                       static_cast<double>(m.x) * facing.x;                                  // 006368FA..00636918
    if (jcc_jp(dot, k_zero_0073d700, 0x05)) return 0;                                        // 0063691A..00636925 -> 006369B9
    kotor_fastcall_006359c0(party, nullptr);                                                 // 0063692B..0063692D
    for (I32 i = 1; i <= 2; ++i) {                                                           // 00636932..006369A4
        U8* state = at(party, 0xB0 + static_cast<U32>(i - 1) * 0x88u);                       // esi
        if (i < 0 || i >= li(party, 0)) continue;                                            // 00636940..00636946
        if (frame_R4_005ed5b0(client_app(), nullptr, lw(state - 4, 0)) == 0) continue; // 00636948..0063695C
        const I32 s = li(state, 0);                                                          // 0063695E
        if (s == -1 || s == 0 || s == 5 || s == 6 || s == 7) continue;                       // 00636960..00636976
        sw(state, 0x20, lw(state, 0x30));                                                    // 00636978..0063697E
        std::memmove(state + 0x14, state + 0x24, 12);                                        // 0063697B..00636991
        sw(state, 0, 4);                                                                     // 00636994
    }
    kotor_fastcall_006346f0(party, nullptr);                                                 // 006369A6..006369A8
    return 1;
}

// 00637050 CSWParty::Update(float delta) (thiscall, RET 4). Replaces the menu-domain trap of the same symbol.
extern "C" void __fastcall frame_R4_00637050(void* party, void*, float delta) {
    void* const last_cell = at(party, 0x2C0);                                                // ebx
    W4Vector last = load_vector(last_cell);                                                  // [esp+40h] 00637073..0063708E
    void* leader = nullptr;                                                                  // [esp+18h]
    if (li(party, 0) > 0) {                                                                  // 00637087..00637092
        leader = pt(frame_R4_005ed5b0(client_app(), nullptr, lw(party, 0x24)));              // 00637098..006370AC
        if (leader != nullptr) {
            const U8* orient = at(leader, 0x30);                                             // esi
            const float angle = static_cast<float>(kotor_m15a_yaw_004aa0f0(reinterpret_cast<const W4Vector*>(orient)) *
                                                   static_cast<double>(k_deg2rad_007415f8)); // 006370B9..006370D0
            const W4Vector axis = load_vector(&kotor_world_los::up_007a19b0);                // 006370C5..006370E6
            U8 rotation[16];                                                                 // [esp+60h]
            quaternion_from_axis_angle_004ac960(rotation, nullptr, axis, angle);             // 006370E2..006370E9
            const U8* pos = at(leader, 0x24);                                                // ecx
            const double dz = static_cast<double>(lf(party, 0x2B8)) - lf(pos, 8);            // 006370EE..006370F8
            const double dy = static_cast<double>(lf(party, 0x2B4)) - lf(pos, 4);            // 006370FE..0063710A
            const double dx = static_cast<double>(lf(party, 0x2B0)) - lf(pos, 0);            // 0063710D..0063710F
            const double d2 = sum_squares(dx, dy, dz);                                       // 00637111..00637125
            const double half = static_cast<double>(lf(party, 0x1C0)) * k_half_0073e9ac;     // 00637127..0063712D
            const double hz = half * lf(orient, 8);                                          // 00637133..00637135
            const float hy = static_cast<float>(half * lf(orient, 4));                       // 00637138..0063713D [esp+28h]
            const float hx = static_cast<float>(half * lf(orient, 0));                       // 00637141..00637145 [esp+34h]
            const double cz = static_cast<double>(lf(pos, 8)) - hz;                          // 00637149 fsubr



            const float cy = static_cast<float>(static_cast<double>(lf(pos, 4)) - hy);       // 0063714C..00637153 [esp+14h]

            const float cx = static_cast<float>(static_cast<double>(lf(pos, 0)) - hx);       // 00637157..00637165 [esp+34h]
            const float czf = static_cast<float>(cz);                                        // 00637169 fst [esp+3Ch]
            const double ez = static_cast<double>(lf(last_cell, 8)) - cz;                    // 0063716D..00637170
            const double ey = static_cast<double>(lf(last_cell, 4)) - cy;                    // 00637172..00637175
            const double ex = static_cast<double>(lf(last_cell, 0)) - cx;                    // 00637179..0063717B
            if (!jcc_jne(sum_squares(ex, ey, ez), k_hundred_00741838, 0x01)) {               // 0063717F..006371A0
                const W4Vector behind{cx, cy, czf};
                store_vector(last_cell, behind);                                             // 006371A2..006371BB
                last = behind;                                                               // 006371B3..006371C2
                std::memcpy(at(party, 0x2CC), rotation, 16);                                 // 006371C6..006371E4
            }
            if (!jcc_jne(d2, k_one_0073d764, 0x01)) {                                        // 006371E7..006371F4
                const double wz = cz - last.z;                                               // 006371FA
                const double wy = static_cast<double>(cy) - last.y;                          // 00637200..00637206
                const double wx = static_cast<double>(cx) - last.x;                          // 0063720C..00637212
                std::memcpy(at(party, 0x2B0), pos, 12);                                      // 0063720A..0063723D
                const W4Vector w{static_cast<float>(wx), static_cast<float>(wy), static_cast<float>(wz)}; // 0063721F..00637231
                const float dist = static_cast<float>(x87_fsqrt(d2));                        // 0063723B..0063724A
                W4Vector unit;
                const W4Vector* n = vector_normalized_004ab180(&unit, &w);                   // 00637240..0063724E
                const double sz = static_cast<double>(dist) * n->z;                          // 00637253..00637257
                const float sy = static_cast<float>(static_cast<double>(dist) * n->y);       // 0063725D..00637264
                const float sx = static_cast<float>(static_cast<double>(dist) * n->x);       // 00637268..0063726E
                const double nz = sz + last.z;                                               // 00637272
                const double ny = static_cast<double>(last.y) + sy;                          // 00637276..0063727A
                const float nx = static_cast<float>(static_cast<double>(sx) + last.x);       // 0063727E..00637286
                sf(party, 0x2C0, nx);                                                        // 0063728A..0063728E
                sf(party, 0x2C4, static_cast<float>(ny));                                    // 00637294..0063729C
                sf(party, 0x2C8, static_cast<float>(nz));                                    // 006372A6..006372BA
                std::memcpy(at(party, 0x2CC), rotation, 16);                                 // 006372A2..006372CE
            }
        }
    }
    // ---- 006372E1..00637352: keep the furthest follower on the path ----
    void* follow = at(party, 0x2DC);                                                         // ebx
    const int furthest = kotor_fastcall_00634da0(follow, nullptr);                           // 006372E7..006372E9
    if (furthest >= 0 && furthest < li(party, 0)) {                                          // 006372F0..006372F6
        if (frame_R4_005ed5b0(client_app(), nullptr, lw(party, static_cast<U32>(furthest) * 0x88u + 0x24)) != 0 && // 006372F8..00637315
            lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0xB4) == 0) {                   // 00637317..0063732C
            if (!jcc_jne(kotor_fastcall_00634e00(follow, nullptr, furthest, 1), k_49_00747c3c, 0x01) && // 0063732E..00637343
                kotor_fastcall_00636840(party, nullptr) == 0)                                 // 00637345..0063734E
                kotor_fastcall_006359c0(party, nullptr);                                     // 00637350..00637352
        }
    }
    // ---- 00637357..00637622: henchman retreat / 5 m scripts ----
    sf(party, 0x2BC, static_cast<float>(static_cast<double>(delta) + lf(party, 0x2BC)));    // 00637357..00637369
    U32 retreat[2];                                                                          // [esp+4Ch]
    frame_LogStringCtor_005e5a90(retreat, nullptr, k_retreat_007514e8);                      // 0063735A..0063736F
    U32 column[2];                                                                           // [esp+28h]
    // Deviation (documented, t1 review): [esp+10h]/[esp+30h] are uninitialised in the original; they only stay
    // unwritten if GetFLOATEntry fails (missing row). W4 starts both from 0.0f to avoid reading indeterminate values.
    float primary = 0.0f, secondary = 0.0f;                                                  // [esp+10h], [esp+30h]
    frame_LogStringCtor_005e5a90(column, nullptr, k_primary_00746f28);                       // 00637376..00637386
    c2da_current_get_float_00413430(rules_float_table(), nullptr, 0x10, column, &primary);  // 0063738B..006373B3
    frame_LogStringDtor_005e5c20(column, nullptr);                                           // 006373B8..006373C1
    const float primary2 = static_cast<float>(static_cast<double>(primary) * primary);       // 006373C6..006373D7
    frame_LogStringCtor_005e5a90(column, nullptr, k_secondary_00746f38);                     // 006373CE..006373DB
    c2da_current_get_float_00413430(rules_float_table(), nullptr, 0x10, column, &secondary); // 006373E0..00637408
    frame_LogStringDtor_005e5c20(column, nullptr);                                           // 0063740D..00637416
    U32 retreated = 0;                                                                       // [esp+28h]
    if (li(party, 0) > 1) {                                                                  // 0063741B..0063742A
        for (I32 i = 1; i < li(party, 0); ++i) {                                             // [esp+1Ch] 00637422..006375E8
            const U32 client_id = lw(party, static_cast<U32>(i) * 0x88u + 0x24);             // [esp+14h] -> [eax]
            const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, client_id);  // 00637440..00637455
            void* server_creature = kotor_fastcall_004ae770(server_app(), nullptr, server_id); // 00637457..00637465
            if (server_creature == nullptr) continue;                                        // 00637467..00637469
            void* henchman = lp(server_creature, 0x4C0);                                     // 0063746F
            if (henchman == nullptr) continue;                                               // 00637475..00637477
            const U32 inside = lw(henchman, 0x38);                                           // 0063747D [esp+24h]
            void* member = (i >= 0 && i < li(party, 0))
                               ? pt(frame_R4_005ed5b0(client_app(), nullptr, client_id))
                               : nullptr;                                                    // 00637480..006374A7
            const double dz = static_cast<double>(lf(leader, 0x2C)) - lf(member, 0x2C);      // 006374A9..006374B0
            const double dy = static_cast<double>(lf(leader, 0x28)) - lf(member, 0x28);      // 006374B3..006374B6
            const double dx = static_cast<double>(lf(leader, 0x24)) - lf(member, 0x24);      // 006374B9..006374BC
            const float distance2 = static_cast<float>(sum_squares(dx, dy, dz));             // 006374BF..006374CF [esp+20h]
            const unsigned flag = server_id != 0x7F000000u ? 1u : 0u;
            if (!jcc_jne(lf(party, 0x2BC), secondary, 0x01) &&                               // 006374D9..006374E8
                !jcc_jne(distance2, primary2, 0x01)) {                                       // 006374EA..006374F7
                retreated = 1;                                                               // 0063750E
                frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, retreat, server_id, flag); // 006374F9..00637519
            }
            if (jcc_jnp(distance2, k_25_007514e0, 0x41)) {                                   // 0063751E..00637531 (inside 5 m)
                if (inside != 0) continue;                                                   // 0063757E..00637580
                U32 script[2];                                                               // [esp+40h]
                frame_LogStringCtor_005e5a90(script, nullptr, k_enter5m_007514c0);           // 00637582..0063758B
                frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script, server_id, flag); // 00637590..006375AD
                sw(lp(server_creature, 0x4C0), 0x38, 1);                                     // 006375B2..006375B8
                frame_LogStringDtor_005e5c20(script, nullptr);                               // 006375BF..006375C8
            } else {
                if (inside == 0) continue;                                                   // 00637533..00637535
                U32 script[2];                                                               // [esp+34h]
                frame_LogStringCtor_005e5a90(script, nullptr, k_exit5m_007514d0);            // 0063753B..00637544
                frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, script, server_id, flag); // 00637549..00637566
                sw(lp(server_creature, 0x4C0), 0x38, 0);                                     // 0063756B..00637571
                frame_LogStringDtor_005e5c20(script, nullptr);                               // 00637578..006375C8
            }
        }
        if (retreated != 0) sw(party, 0x2BC, 0);                                             // 006375EE..006375F6
    }
    frame_LogStringDtor_005e5c20(retreat, nullptr);                                          // 00637600..0063760C
}
