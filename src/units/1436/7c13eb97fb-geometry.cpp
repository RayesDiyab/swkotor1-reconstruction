// lane-world-los geometry: vector / segment / polygon tests reached from the world line-of-sight closure
// (CSWCollisionMesh::NoNonWalkPolys 005972B0, CSWRoomSurfaceMesh::CheckAABBNode 00580920).
//
// x87 policy (D30-FINAL): double intermediates; float only where the original stores a dword (fstp/fst dword).
// Compare patterns (fcom/fcomp + fnstsw + test ah):
//   "test ah,5  / jp"  taken iff !(a <  b)     "test ah,5  / jnp" taken iff a < b
//   "test ah,41h/ jne" taken iff !(a >  b)     "test ah,41h/ je"  taken iff a > b
//   "test ah,41h/ jp"  taken iff !(a <= b)     "test ah,41h/ jnp" taken iff a <= b
//   "test ah,44h/ jp"  taken iff !(a == b)     "test ah,44h/ jnp" taken iff a == b
//   "test ah,1  / jne" taken iff !(a >= b)
// (C0/C2/C3 all set when unordered, so every NaN case follows from the C++ relational operator shown.)
#include <cmath>
#include "4bdde5ee56-world_los.h"

namespace kotor_world_los {
// Function-static Vector of 004AB9D0 (.bss 00831F94) + guard word (.bss 00831FA0, bit 0). Only 004AB9D0
// references these addresses.
U guard_00831fa0 = 0;
Vector polyhit_point_00831f94 = {0.0f, 0.0f, 0.0f};
// Function-static Vector of 004ABF00 (.bss 00831FA4) + guard word (.bss 00831FB0, bit 0). Only 004ABF00
// references 00831FB0 (004ACCDF.. uses a different guard, 00831FCC, for its own static at 00831FB4).
U guard_00831fb0 = 0;
Vector distance_axis_00831fa4 = {0.0f, 0.0f, 0.0f};
}

using namespace kotor_world_los;

namespace {
// .rdata 007440FC float = 322BCC77h (1e-8f), compared by 004ABACE fcom dword
inline float eps_007440fc() { return bits_to_float(0x322BCC77u); }
constexpr float k_four_0073f404 = 4.0f;    // .rdata 0073F404 = 40800000h
constexpr double k_one_0073ede8 = 1.0;     // .rdata qword 0073EDE8 = 3FF0000000000000h
constexpr double k_zero_0073edf8 = 0.0;    // .rdata qword 0073EDF8 = 0
}

extern "C" {

// 00423BF0 vectorComponentMult(out, v, scale): out = float(scale * v) (products z,y,x formed before stores), EAX=out.
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float scale) {
    auto* const resultPointer = out;
    // fld scalar / fmul m32 is intentional: loading v first changes NaN payloads.
    // The original keeps z,y,x on the x87 stack until all inputs have been read.
    __asm {
        mov eax, v
        fld scale
        fmul dword ptr [eax+8]
        fld scale
        fmul dword ptr [eax+4]
        fld scale
        fmul dword ptr [eax]
        mov eax, resultPointer
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
    return out;
}

// 004AAB50 "IntersectCircle2d" (actually 3-D): segment a->b against the sphere (center, radius).
// D = float(b - a); A = float(D.D); B = 2*(f.D) with f = a - center (wide); disc = B*float(B) - (f.f - r*r)*A*4.
// Returns the number of roots t in [0,1] written: disc == 0 -> one root into out1; disc > 0 -> first root
// (sq - B)/2A into out1, second (-B - sq)/2A into out2 (or out1 if the first was rejected). flag (arg7) unused.
I __cdecl intersect_circle_2d_004aab50(const Vector* center, float radius, const Vector* a, const Vector* b,
                                       Vector* out1, Vector* out2, bool flag) {
    (void)flag;                                                  // [esp+44h] never read
    Vector d;                                                    // [esp+4..0C]
    d.x = static_cast<float>(static_cast<double>(b->x) - a->x);  // 004AAB57..004AAB60
    d.y = static_cast<float>(static_cast<double>(b->y) - a->y);  // 004AAB64..004AAB6A
    d.z = static_cast<float>(static_cast<double>(b->z) - a->z);  // 004AAB6E..004AAB78
    const double fx = static_cast<double>(a->x) - center->x;     // 004AAB7C..004AAB7E
    const double fy = static_cast<double>(a->y) - center->y;     // 004AAB80..004AAB83
    const double fz = static_cast<double>(a->z) - center->z;     // 004AAB86..004AAB89
    const float a_f = static_cast<float>(static_cast<double>(d.x) * d.x + static_cast<double>(d.z) * d.z +
                                         static_cast<double>(d.y) * d.y);           // 004AAB8C..004AABA8 (over arg3 slot)
    const double s = fx * d.x + fz * d.z + fy * d.y;                                 // 004AABAC..004AABC0
    const double bw = s + s;                                                         // 004AABC2 fadd st0,st0
    const float b_f = static_cast<float>(bw);                                        // 004AABC4 fst (over arg4 slot)
    const double c0 = fz * fz + fy * fy + fx * fx;                                   // 004AABCC..004AABDA
    const double disc = bw * b_f - (c0 - static_cast<double>(radius) * radius) * a_f * k_four_0073f404; // 004AABC8..004AABF0
    const float disc_f = static_cast<float>(disc);                                   // 004AABF2 (over radius slot)

    if (disc_f == k_zero_0073d700) {                             // 004AABFC..004AAC0B (jp taken iff !=)
        const double t = -(b_f / (static_cast<double>(a_f) + a_f));                  // 004AAC11..004AAC1B
        const float t_f = static_cast<float>(t);                                     // 004AAC1D fst
        if (!(t >= k_zero_0073d700)) return 0;                   // 004AAC21..004AAC2C (wide compare)
        if (!(t_f <= k_one_0073d764)) return 0;                  // 004AAC32..004AAC41
        const double x = static_cast<double>(d.x) * t_f;         // 004AAC47..004AAC4F
        const double y = static_cast<double>(d.y) * t_f;         // 004AAC53..004AAC57
        const float z = static_cast<float>(static_cast<double>(d.z) * t_f);          // 004AAC5B..004AAC63
        out1->x = static_cast<float>(x + a->x);                  // 004AAC69..004AAC85
        out1->y = static_cast<float>(y + a->y);                  // 004AAC73..004AAC87
        out1->z = static_cast<float>(static_cast<double>(z) + a->z);                 // 004AAC7A..004AAC93
        return 1;                                                // 004AAC96
    }
    if (!(disc_f > k_zero_0073d700)) return 0;                   // 004AAC9F..004AACAE
    const double sq = std::sqrt(static_cast<double>(disc_f));    // 004AACB4..004AACB9
    const double two_a = static_cast<double>(a_f) + a_f;         // 004AACC5..004AACC9
    I roots = 2;                                                 // 004AACC0 mov ebx,2
    const double t1 = (sq - b_f) / two_a;                        // 004AACCB..004AACD1
    const float t1_f = static_cast<float>(t1);                   // 004AACD3 fst
    if (!(t1 >= k_zero_0073d700)) {                              // 004AACD7..004AACE2 (wide)
        roots = 1;                                               // 004AAD43
    } else if (!(t1_f <= k_one_0073d764)) {                      // 004AACE4..004AACF3
        roots = 1;
    } else {
        const double x = static_cast<double>(d.x) * t1_f;        // 004AACF5..004AACFB
        const double y = static_cast<double>(d.y) * t1_f;        // 004AACFF..004AAD03
        const float z = static_cast<float>(static_cast<double>(d.z) * t1_f);         // 004AAD07..004AAD0F
        Vector r;                                                // [esp+18h..20h]
        r.x = static_cast<float>(x + a->x);                      // 004AAD13..004AAD17
        r.y = static_cast<float>(y + a->y);                      // 004AAD1F..004AAD22
        r.z = static_cast<float>(static_cast<double>(z) + a->z); // 004AAD26..004AAD36
        copy_vector(out1, &r);                                   // 004AAD31..004AAD3E dword copies
    }
    const double t2 = (-static_cast<double>(b_f) - sq) / two_a; // 004AAD48..004AAD50
    const float t2_f = static_cast<float>(t2);                   // 004AAD52 fstp dword
    if (!(t2_f >= k_zero_0073d700)) return roots - 1;            // 004AAD5A..004AAD69 -> 004AADDA dec ebx
    if (!(t2_f <= k_one_0073d764)) return roots - 1;             // 004AAD6B..004AAD7A
    Vector scaled;                                               // [esp+1Ch]
    Vector sum;                                                  // [esp+10h]
    Vector* m = vector_component_mult_00423bf0(&scaled, &d, t2_f);   // 004AAD7C..004AAD8B
    const Vector* p = vector_sum_00423b90(&sum, a, m);              // 004AAD90..004AAD97
    copy_vector(roots == 2 ? out2 : out1, p);                    // 004AAD9F..004AADCE
    return roots;                                                // 004AADB9 / 004AADD2
}

// 004AB9D0 polyhit(points, count, start, end, hit_out, normal_out): polygon normal by summed edge cross products,
// segment/plane crossing (start on the front side, end not in front, different signed distances), then an
// inside test against every edge. EAX 1/0; outputs written only on success.
I __cdecl polyhit_004ab9d0(const Vector* points, I count, const Vector* start, const Vector* end,
                           Vector* hit_out, Vector* normal_out) {
    Vector sum = {0.0f, 0.0f, 0.0f};                             // 004AB9E4..004AB9F4 [esp+24h]
    for (I left = count - 2, i = 2; left > 0; --left, ++i) {     // 004AB9D9..004AB9FC, 004ABA87..004ABABE
        const Vector* prev = &points[(i - 1) % count];           // 004ABA10..004ABA1E idiv
        const Vector* next = &points[i % count];                 // 004ABA21..004ABA31 idiv
        const Vector* base = &points[i - 2];                     // esi = points + 8 + 12*(i-2)
        Vector e2, e1, r;
        e2.x = static_cast<float>(static_cast<double>(next->x) - prev->x);   // 004ABA2E..004ABA3A
        e2.y = static_cast<float>(static_cast<double>(next->y) - prev->y);   // 004ABA3E..004ABA44
        e2.z = static_cast<float>(static_cast<double>(next->z) - prev->z);   // 004ABA48..004ABA53
        e1.x = static_cast<float>(static_cast<double>(prev->x) - base->x);   // 004ABA57..004ABA5C
        e1.y = static_cast<float>(static_cast<double>(prev->y) - base->y);   // 004ABA60..004ABA66
        e1.z = static_cast<float>(static_cast<double>(prev->z) - base->z);   // 004ABA6A..004ABA75
        const Vector* c = vector_cross_004ab210(&r, &e1, &e2);   // 004ABA79
        sum.x = static_cast<float>(static_cast<double>(sum.x) + c->x);       // 004ABA7E..004ABA8C
        sum.y = static_cast<float>(static_cast<double>(sum.y) + c->y);       // 004ABA90..004ABA97
        sum.z = static_cast<float>(static_cast<double>(sum.z) + c->z);       // 004ABA9B..004ABAB2
    }
    const double mag = vector_magnitude_004ab100(&sum);          // 004ABAC4..004ABAC9
    if (mag < eps_007440fc()) return 0;                          // 004ABACE..004ABADC -> 004ABDB7
    const double inv = static_cast<double>(k_one_0073d764) / mag;               // 004ABAE2
    Vector n;                                                    // [esp+18h..20h]
    n.x = static_cast<float>(sum.x * inv);                       // 004ABAEF..004ABAF5
    n.y = static_cast<float>(sum.y * inv);                       // 004ABAF9..004ABAFF
    n.z = static_cast<float>(sum.z * inv);                       // 004ABB03..004ABB09
    const double plane_d = -(static_cast<double>(n.z) * points->z + static_cast<double>(n.y) * points->y +
                             static_cast<double>(n.x) * points->x);                // 004ABB0F..004ABB27
    const double s_wide = (static_cast<double>(n.z) * start->z + static_cast<double>(n.y) * start->y +
                           static_cast<double>(n.x) * start->x) + plane_d;        // 004ABB29..004ABB41
    const float s_start = static_cast<float>(s_wide);            // 004ABB43 fst [esp+10h]
    if (s_wide < k_zero_0073d700) return 0;                      // 004ABB47..004ABB52 (wide compare)
    const float s_end = static_cast<float>((static_cast<double>(n.z) * end->z + static_cast<double>(n.y) * end->y +
                                            static_cast<double>(n.x) * end->x) + plane_d);   // 004ABB5F..004ABB79
    if (s_end > k_zero_0073d700) return 0;                       // 004ABB7F..004ABB8E
    if (s_start == s_end) return 0;                              // 004ABB94..004ABBA1
    if ((guard_00831fa0 & 1u) == 0) {                            // 004ABBA7..004ABBAE
        guard_00831fa0 |= 1u;                                    // 004ABBB0
        polyhit_point_00831f94.x = 0.0f;                         // 004ABBB7
        polyhit_point_00831f94.y = 0.0f;                         // 004ABBC1
        polyhit_point_00831f94.z = 0.0f;                         // 004ABBCB
    }
    const double t = s_start / (static_cast<double>(s_start) - s_end);         // 004ABBD5..004ABBDF
    const double ex = t * end->x;                                              // 004ABBE3..004ABBE5 (kept wide)
    const float ey = static_cast<float>(t * end->y);                           // 004ABBE7..004ABBEC
    const float ez = static_cast<float>(t * end->z);                           // 004ABBF0..004ABBF5
    const double u = static_cast<double>(k_one_0073d764) - t;                  // 004ABBF9..004ABBFF
    const float sx = static_cast<float>(u * start->x);                         // 004ABC01..004ABC05
    const float sy = static_cast<float>(u * start->y);                         // 004ABC09..004ABC0E
    const float sz = static_cast<float>(u * start->z);                         // 004ABC12..004ABC15
    Vector hit;                                                  // [esp+24h..2Ch]
    hit.x = static_cast<float>(sx + ex);                         // 004ABC19..004ABC1F
    polyhit_point_00831f94.x = hit.x;                            // 004ABC23..004ABC27 (dword copy)
    hit.y = static_cast<float>(static_cast<double>(sy) + ey);    // 004ABC33..004ABC3B
    polyhit_point_00831f94.y = hit.y;                            // 004ABC43..004ABC4B
    hit.z = static_cast<float>(static_cast<double>(sz) + ez);    // 004ABC3F..004ABC51
    polyhit_point_00831f94.z = hit.z;                            // 004ABC55..004ABC59
    for (I i = 0; i < count; ++i) {                              // 004ABC5E..004ABC6A, 004ABD52..004ABD59
        Vector p, q, to_hit, edge, c_out;
        copy_vector(&p, &points[i]);                             // 004ABC70..004ABC95 [esp+30h]
        to_hit.x = static_cast<float>(static_cast<double>(hit.x) - p.x);      // 004ABC72..004ABC8A
        to_hit.y = static_cast<float>(static_cast<double>(hit.y) - p.y);      // 004ABC91..004ABCA7
        to_hit.z = static_cast<float>(static_cast<double>(hit.z) - p.z);      // 004ABCAB..004ABCB3
        copy_vector(&q, &points[(i + 1) % count]);               // 004ABC8E..004ABCD5 idiv, [esp+3Ch]
        edge.x = static_cast<float>(static_cast<double>(q.x) - p.x);          // 004ABCC9..004ABCDD
        edge.y = static_cast<float>(static_cast<double>(q.y) - p.y);          // 004ABCE2..004ABCF4
        edge.z = static_cast<float>(static_cast<double>(q.z) - p.z);          // 004ABCF8..004ABD00
        const Vector* cr = vector_cross_004ab210(&c_out, &edge, &to_hit);     // 004ABD04
        Vector c;                                                // [esp+48h..50h]
        copy_vector(&c, cr);                                     // 004ABD09..004ABD21
        const double dot = static_cast<double>(c.z) * n.z + static_cast<double>(c.y) * n.y +
                           static_cast<double>(c.x) * n.x;       // 004ABD19..004ABD3A
        if (!(dot >= k_zero_0073edf8)) return 0;                 // 004ABD3C..004ABD5F (eax=0 ends the loop)
    }
    if (normal_out != nullptr) copy_vector(normal_out, &n);      // 004ABD6B..004ABD87
    if (hit_out != nullptr) copy_vector(hit_out, &polyhit_point_00831f94);     // 004ABD8A..004ABDAF (reads the static)
    return 1;
}

// 004ABF00 DistanceBetweenLines(p1, dir1, p2, dir2, closest1_out, closest2_out): axis S = normalize(dir1 x dir2)
// held in a function static (00831FA4, guard 00831FB0); distance = |(-S.p1) - (-S.p2)| stored as float over the
// arg1 slot (004ABFD1) and returned from that float (004AC0F6). Each non-null out gets the intersection of the
// other line with the plane through this line containing S.
double __cdecl distance_between_lines_004abf00(const Vector* p1, const Vector* p2, const Vector* p3,
                                               const Vector* p4, Vector* p5, Vector* p6) {
    if ((guard_00831fb0 & 1u) == 0) {                            // 004ABF00..004ABF10
        guard_00831fb0 |= 1u;                                    // 004ABF12
        distance_axis_00831fa4.x = 0.0f;                         // 004ABF18
        distance_axis_00831fa4.y = 0.0f;                         // 004ABF22
        distance_axis_00831fa4.z = 0.0f;                         // 004ABF2C
    }
    Vector& axis = distance_axis_00831fa4;
    Vector tmp_a, tmp_n, tmp_c;                                  // [esp+1Ch] / [esp+10h] / [esp+28h] (post-push frame)
    copy_vector(&axis, vector_cross_004ab210(&tmp_a, p2, p4));   // 004ABF42..004ABF6A
    vector_normalize_004ab130(&axis, nullptr);                   // 004ABF65..004ABF6F (lane-3d-model)
    const double d1 = -(static_cast<double>(axis.x) * p1->x + static_cast<double>(axis.z) * p1->z +
                        static_cast<double>(axis.y) * p1->y);    // 004ABF74..004ABF9A
    const double d2 = -(static_cast<double>(axis.z) * p3->z + static_cast<double>(axis.y) * p3->y +
                        static_cast<double>(axis.x) * p3->x);    // 004ABF9C..004ABFBA
    double dist = d1 - d2;                                       // 004ABFBC fsubp
    if (dist < k_zero_0073d700) dist = -dist;                    // 004ABFBE..004ABFCB
    const float dist_f = static_cast<float>(dist);               // 004ABFD1 fstp [arg1 slot]
    if (p5 != nullptr) {                                         // 004ABFCD..004ABFD7
        const Vector* c = vector_cross_004ab210(&tmp_a, p4, &axis);           // 004ABFDD..004ABFE8
        normalize_004ab180(&tmp_n, c);                                        // 004ABFED..004ABFF3
        const float plane_d = static_cast<float>(-(static_cast<double>(tmp_n.y) * p3->y +
            static_cast<double>(tmp_n.z) * p3->z + static_cast<double>(tmp_n.x) * p3->x));   // 004ABFF8..004AC020
        tmp_a.x = static_cast<float>(static_cast<double>(p1->x) + p2->x);   // 004AC024..004AC030
        tmp_a.y = static_cast<float>(static_cast<double>(p2->y) + p1->y);   // 004AC034..004AC03A
        tmp_a.z = static_cast<float>(static_cast<double>(p2->z) + p1->z);   // 004AC03E..004AC044
        const Vector* hit = plane_line_intersection_004ab250(&tmp_c, &tmp_n, plane_d, p1, &tmp_a); // 004AC048
        copy_vector(p5, hit);                                    // 004AC04D..004AC061
    }
    if (p6 != nullptr) {                                         // 004AC064..004AC06A
        const Vector* c = vector_cross_004ab210(&tmp_c, p2, &axis);           // 004AC070..004AC07B
        normalize_004ab180(&tmp_n, c);                                        // 004AC080..004AC086
        const float plane_d = static_cast<float>(-(static_cast<double>(tmp_n.x) * p1->x +
            static_cast<double>(tmp_n.y) * p1->y + static_cast<double>(tmp_n.z) * p1->z));   // 004AC08B..004AC0B3
        tmp_a.x = static_cast<float>(static_cast<double>(p3->x) + p4->x);   // 004AC0B7..004AC0C2
        tmp_a.y = static_cast<float>(static_cast<double>(p4->y) + p3->y);   // 004AC0C6..004AC0CC
        tmp_a.z = static_cast<float>(static_cast<double>(p4->z) + p3->z);   // 004AC0D0..004AC0D6
        const Vector* hit = plane_line_intersection_004ab250(&tmp_c, &tmp_n, plane_d, p3, &tmp_a); // 004AC0DA
        copy_vector(p6, hit);                                    // 004AC0DF..004AC0F3
    }
    return dist_f;                                               // 004AC0F6 fld dword [arg1 slot]
}

// 004AC340 IntersectLineSegments2d(p1, p2, p3, p4, out): XY segment intersection with z interpolated on p3->p4.
// Transcribed block-for-block (labels = original addresses). Result = flag byte kept in the arg1 slot
// (004AC3D7 / 004AC472 / 004AC5BF / 004AC86C), returned via setne; L665 clears it first.
I __cdecl intersect_line_segments_2d_004ac340(const Vector* p1, const Vector* p2, const Vector* p3,
                                              const Vector* p4, Vector* out) {
    unsigned char flag = 0;
    float y_slot = 0.0f;     // [arg2 slot] (004AC406 / 004AC553); every read is preceded by its store on that path
    float m1_f = 0.0f;       // [esp+14h] reused (004AC67D); read only on the L67D path after its store
    double m1 = 0.0;
    double abs_a = 0.0, abs_b = 0.0, t = 0.0;

    const double dy1 = static_cast<double>(p2->y) - p1->y;       // 004AC347..004AC350
    const float dy1_f = static_cast<float>(dy1);                 // 004AC35C fst
    const double dx1 = static_cast<double>(p2->x) - p1->x;       // 004AC360..004AC362
    const float dx1_f = static_cast<float>(dx1);                 // 004AC365 fst
    const float len1_f = static_cast<float>(std::sqrt(dy1 * dy1 + dx1 * dx1));  // 004AC369..004AC375
    const double dy2 = static_cast<double>(p4->y) - p3->y;       // 004AC37D..004AC380
    const float dy2_f = static_cast<float>(dy2);                 // 004AC383 fst (arg4 slot)
    const double dx2 = static_cast<double>(p4->x) - p3->x;       // 004AC387..004AC389
    const float dx2_f = static_cast<float>(dx2);                 // 004AC38B fst (arg3 slot)
    const double dot = (static_cast<double>(dx2_f) * dx1_f + static_cast<double>(dy2_f) * dy1_f) / len1_f; // 004AC38F..004AC3A1
    const double len2 = std::sqrt(dx2 * dx2 + dy2 * dy2);        // 004AC3A5..004AC3AF
    if (dot / len2 == k_one_0073ede8) return 0;                  // 004AC3B5..004AC3D0 (parallel, same direction)

    flag = 0;                                                    // 004AC3D7
    if (!(p1->x == p2->x)) goto L526;                            // 004AC3D1..004AC3E1
    {   // line 1 vertical
        const double m = static_cast<double>(dy2_f) / dx2_f;     // 004AC3E7..004AC3EE
        std::memcpy(&out->x, &p1->x, 4);                         // 004AC3EB..004AC3F6 dword move
        y_slot = static_cast<float>((static_cast<double>(p3->y) - m * p3->x) + m * p1->x);  // 004AC3F8..004AC406
        std::memcpy(&out->y, &y_slot, 4);                        // 004AC40A..004AC40E
    }
    if (!(p4->y >= p3->y)) goto L43A;                            // 004AC411..004AC41C
    if (!(y_slot >= p3->y)) goto L43A;                           // 004AC41E..004AC42A
    if (y_slot <= p4->y) goto L46F;                              // 004AC42C..004AC438
L43A:
    if (!(p3->y >= p4->y)) goto L943;                            // 004AC43A..004AC445
    if (!(y_slot >= p4->y)) goto L943;                           // 004AC44B..004AC457
    if (!(y_slot <= p3->y)) goto L943;                           // 004AC45D..004AC469
L46F:
    flag = 1;                                                    // 004AC472
    if (!(p2->y >= p1->y)) goto L4A5;                            // 004AC46F..004AC47F
    if (!(y_slot >= p1->y)) goto L665;                           // 004AC481..004AC48D
    if (!(y_slot <= p2->y)) goto L665;                           // 004AC493..004AC49F
L4A5:
    if (!(p1->y >= p2->y)) goto L4D6;                            // 004AC4A5..004AC4B0
    if (!(y_slot >= p2->y)) goto L665;                           // 004AC4B2..004AC4BE
    if (!(y_slot <= p1->y)) goto L665;                           // 004AC4C4..004AC4D0
L4D6:
    if (!(p4->x >= p3->x)) goto L4FF;                            // 004AC4D6..004AC4DF
    if (!(out->x >= p3->x)) goto L665;                           // 004AC4E1..004AC4EA
    if (!(out->x <= p4->x)) goto L665;                           // 004AC4F0..004AC4F9
L4FF:
    if (!(p3->x >= p4->x)) goto L8E6;                            // 004AC4FF..004AC508
    if (!(out->x >= p4->x)) goto L665;                           // 004AC50E..004AC517
    if (out->x <= p3->x) goto L8E6;                              // 004AC51D..004AC521 -> 004AC65A..004AC65F
    goto L665;

L526:
    m1 = static_cast<double>(dy1_f) / dx1_f;                     // 004AC52A..004AC52E
    if (!(p3->x == p4->x)) goto L67D;                            // 004AC526..004AC537
    {   // line 2 vertical
        std::memcpy(&out->x, &p3->x, 4);                         // 004AC53D..004AC545
        y_slot = static_cast<float>((static_cast<double>(p1->y) - m1 * p1->x) + m1 * p3->x);  // 004AC53F..004AC553
        std::memcpy(&out->y, &y_slot, 4);                        // 004AC557..004AC55B
    }
    if (!(p2->y >= p1->y)) goto L587;                            // 004AC55E..004AC569
    if (!(y_slot >= p1->y)) goto L587;                           // 004AC56B..004AC577
    if (y_slot <= p2->y) goto L5BC;                              // 004AC579..004AC585
L587:
    if (!(p1->y >= p2->y)) goto L943;                            // 004AC587..004AC592
    if (!(y_slot >= p2->y)) goto L943;                           // 004AC598..004AC5A4
    if (!(y_slot <= p1->y)) goto L943;                           // 004AC5AA..004AC5B6
L5BC:
    flag = 1;                                                    // 004AC5BF
    if (!(p4->y >= p3->y)) goto L5EE;                            // 004AC5BC..004AC5CC
    if (!(y_slot >= p3->y)) goto L665;                           // 004AC5CE..004AC5DA
    if (!(y_slot <= p4->y)) goto L665;                           // 004AC5E0..004AC5EC
L5EE:
    if (!(p3->y >= p4->y)) goto L617;                            // 004AC5EE..004AC5F9
    if (!(y_slot >= p4->y)) goto L665;                           // 004AC5FB..004AC607
    if (!(y_slot <= p3->y)) goto L665;                           // 004AC609..004AC615
L617:
    if (!(p2->x >= p1->x)) goto L63A;                            // 004AC617..004AC621
    if (!(out->x >= p1->x)) goto L665;                           // 004AC623..004AC62D
    if (!(out->x <= p2->x)) goto L665;                           // 004AC62F..004AC638
L63A:
    if (!(p1->x >= p2->x)) goto L8E6;                            // 004AC63A..004AC644
    if (!(out->x >= p2->x)) goto L665;                           // 004AC64A..004AC653
    if (out->x <= p1->x) goto L8E6;                              // 004AC655..004AC65F
    goto L665;

L67D:
    {   // general case
        m1_f = static_cast<float>(m1);                           // 004AC67D fstp [esp+14h]
        const double m2 = static_cast<double>(dy2_f) / dx2_f;    // 004AC681..004AC685
        const float m2_f = static_cast<float>(m2);               // 004AC689 fst (arg4 slot)
        if (m2 - m1_f == k_zero_0073edf8) return 0;              // 004AC68D..004AC6A7
        Vector a, b, c, d, n;                                    // [esp+1Ch] [esp+28h] [esp+40h] [esp+34h] [esp+4Ch]
        copy_vector(&a, p1);                                     // 004AC6A8..004AC6BC
        copy_vector(&b, p2);                                     // 004AC6BA..004AC6D6
        n.x = static_cast<float>(static_cast<double>(b.y) - a.y);              // 004AC6CE..004AC6E1
        copy_vector(&c, p3);                                     // 004AC6DA..004AC6F8
        const double nyw = static_cast<double>(a.x) - b.x;      // 004AC6E8..004AC6EC
        copy_vector(&d, p4);                                     // 004AC6FC..004AC713
        n.y = static_cast<float>(nyw);                           // 004AC720
        c.z = 0.0f;                                              // 004AC725 (dword 0)
        d.z = 0.0f;                                              // 004AC72D
        n.z = 0.0f;                                              // 004AC735
        vector_divide_00437a90(&b, &n, len1_f);                  // 004AC70A..004AC73D (out = b copy)
        const float plane_d = static_cast<float>((static_cast<double>(b.z) * k_zero_0073d700 -
            static_cast<double>(b.y) * a.y) - static_cast<double>(b.x) * a.x);   // 004AC742..004AC772
        const Vector* hit = plane_line_intersection_004ab250(&n, &b, plane_d, &c, &d);   // 004AC77A..004AC77D
        copy_vector(out, hit);                                   // 004AC789..004AC7AC
        double am2 = m2_f;                                       // 004AC798 fld
        if (m2_f < k_zero_0073d700) am2 = -am2;                  // 004AC782..004AC7B9
        if (!(am2 > k_one_0073d764)) goto L819;                  // 004AC7BB..004AC7C6
    }
    if (!(p4->y >= p3->y)) goto L7EF;                            // 004AC7C8..004AC7D3
    if (!(out->y >= p3->y)) goto L7EF;                           // 004AC7D5..004AC7E0
    if (out->y <= p4->y) goto L867;                              // 004AC7E2..004AC7ED
L7EF:
    if (!(p3->y >= p4->y)) goto L943;                            // 004AC7EF..004AC7FA
    if (!(out->y >= p4->y)) goto L943;                           // 004AC800..004AC80B
    if (!(out->y <= p3->y)) goto L943;                           // 004AC811..004AC817 -> 004AC85C..004AC861
    goto L867;
L819:
    if (!(p4->x >= p3->x)) goto L83A;                            // 004AC819..004AC822
    if (!(out->x >= p3->x)) goto L83A;                           // 004AC824..004AC82D
    if (out->x <= p4->x) goto L867;                              // 004AC82F..004AC838
L83A:
    if (!(p3->x >= p4->x)) goto L943;                            // 004AC83A..004AC843
    if (!(out->x >= p4->x)) goto L943;                           // 004AC849..004AC852
    if (!(out->x <= p3->x)) goto L943;                           // 004AC858..004AC861
L867:
    flag = 1;                                                    // 004AC86C
    if (!(aurora_fabs_00423b70(m1_f) > k_one_0073d764)) goto L617;   // 004AC867..004AC888
    if (!(p2->y >= p1->y)) goto L8BD;                            // 004AC88E..004AC899
    if (!(out->y >= p1->y)) goto L665;                           // 004AC89B..004AC8A6
    if (!(out->y <= p2->y)) goto L665;                           // 004AC8AC..004AC8B7
L8BD:
    if (!(p1->y >= p2->y)) goto L8E6;                            // 004AC8BD..004AC8C8
    if (!(out->y >= p2->y)) goto L665;                           // 004AC8CA..004AC8D5
    if (out->y <= p1->y) goto L8E6;                              // 004AC8DB..004AC8E1 -> 004AC65A..004AC65F
    goto L665;

L8E6:
    abs_a = static_cast<double>(p3->x) - p4->x;                  // 004AC8E6..004AC8E8
    if (abs_a < k_zero_0073d700) abs_a = -abs_a;                 // 004AC8EA..004AC8F7
    abs_b = static_cast<double>(p3->y) - p4->y;                  // 004AC8F9..004AC8FC
    if (abs_b < k_zero_0073d700) abs_b = -abs_b;                 // 004AC8FF..004AC90C
    if (!(abs_a >= abs_b))                                       // 004AC90E..004AC919
        t = (static_cast<double>(out->y) - p3->y) / (static_cast<double>(p4->y) - p3->y);   // 004AC925..004AC931
    else
        t = (static_cast<double>(out->x) - p3->x) / (static_cast<double>(p4->x) - p3->x);   // 004AC91B..004AC931
    out->z = static_cast<float>((static_cast<double>(p4->z) - p3->z) * t + p3->z);          // 004AC933..004AC93E
L943:
    return flag != 0 ? 1 : 0;                                    // 004AC943..004AC955 setne
L665:
    flag = 0;                                                    // 004AC666
    return flag != 0 ? 1 : 0;                                    // 004AC66B..004AC67C
}

} // extern "C"
