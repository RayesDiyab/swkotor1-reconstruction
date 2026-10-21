// W5 t3: CSWSArea::ShrinkLineByCreatures 004BF650 (every frame: W4's camera collision 004B221B). Transcribed from the
// original (read-only). Export name is the one W4's camera_collision.cpp imports.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned int); } // CExoArrayList::Allocate

extern "C" {
unsigned __fastcall recovered_area_index_00506f20(void* area, void*, unsigned* out, unsigned x_bits); // GetFirstObjectIndiceByX (1065)
I __cdecl intersect_circle_2d_004aab50(const Vector* center, float radius, const Vector* a, const Vector* b,
                                       Vector* out1, Vector* out2, bool flag);                        // IntersectCircle2d (0528)
U __fastcall kotor_fastcall_004bf650(void* area, void*, const Vector* from, const Vector* to, Vector* out);
}

namespace {
const float k_box_margin_00740c84 = 3.0f;            // .rdata 00740C84
const float k_step_0073d708 = 0.001f;                // .rdata 0073D708
const float k_z_bias_0073dfec = 0.01f;               // .rdata 0073DFEC
struct PtrList { void** data; I count; I alloc; };   // CExoArrayList<CSWSCreature*> [esp+40]
// x87 sequences 004BFB99..BEB / 004BFBFE..C44 / 004BFCCE..D1C / 004BFD41..8D: dz stays in ST, dy and dx are also
// stored as floats: (dxf*dxf + dy*dyf) + dzf*dz
inline double dist2_mixed(double dx, double dy, double dz) {
    const float dxf = static_cast<float>(dx), dyf = static_cast<float>(dy), dzf = static_cast<float>(dz);
    return (static_cast<double>(dxf) * dxf + dy * dyf) + static_cast<double>(dzf) * dz;
}
}  // namespace

// 004BF650 ShrinkLineByCreatures(from*, to*, out*): `out` (starting at `to`) is pulled back along the line to just
// outside the circle (pathfind +10) of creatures in the line's x/y box (+-3); creatures whose personal space (pathfind
// +4) contains `from` are ignored. Passes repeat while one changed something. EAX 1 when out moved. RET 0Ch.
extern "C" U __fastcall kotor_fastcall_004bf650(void* area, void*, const Vector* from, const Vector* to, Vector* out) {
    PtrList list{nullptr, 0, 0};                                                       // 004BF673..7B
    const float min_x = static_cast<float>(static_cast<double>(from->x < to->x ? from->x : to->x) - k_box_margin_00740c84); // [14]
    const float max_x = static_cast<float>(static_cast<double>(from->x > to->x ? from->x : to->x) + k_box_margin_00740c84); // [1C]
    const float min_y = static_cast<float>(static_cast<double>(from->y < to->y ? from->y : to->y) - k_box_margin_00740c84); // [10]
    const float max_y = static_cast<float>(static_cast<double>(from->y > to->y ? from->y : to->y) + k_box_margin_00740c84); // [30]
    I last = at<I>(area, 0x194) - 1;                                                   // 004BF6FE..0B
    unsigned found = 0;                                                                // [esp+64]
    if (recovered_area_index_00506f20(area, nullptr, &found, bits(max_x)) != 0) last = static_cast<I>(found); // 004BF717..2C
    I first = 0;                                                                       // EBX
    if (recovered_area_index_00506f20(area, nullptr, &found, bits(min_x)) != 0) first = static_cast<I>(found); // 004BF730..45
    for (I i = first; i <= last; ++i) {                                                // 004BF749..4D / 004BF846..4D
        void* cr = kotor_fastcall_004ae770(server_app(), nullptr, at<U*>(area, 0x190)[i]);   // 004BF760..73
        if (cr == nullptr) continue;
        const Vector p = at<Vector>(cr, 0x90);                                         // [34]
        const float dy = static_cast<float>(static_cast<double>(p.y) - from->y);       // [1C]
        const float dx = static_cast<float>(static_cast<double>(p.x) - from->x);       // [74]
        if (p.y < min_y || p.y != p.y) continue;                                       // 004BF7CA..D7 (test ah,1)
        if (!(p.y <= max_y)) continue;                                                 // 004BF7D9..E6 (test ah,41h; jp)
        const float r = at<float>(at<void*>(cr, 0x340), 4);                            // 004BF7E8..F1
        const double d2 = static_cast<double>(dx) * dx + static_cast<double>(dy) * dy; // 004BF7F4..804
        const double r2 = static_cast<double>(r) * r;                                  // 004BF806..08
        if (!(r2 < d2)) continue;                                                      // 004BF80A..13 (test ah,5; jp)
        if (list.count == list.alloc)
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&list), nullptr,
                                                static_cast<unsigned>(list.alloc != 0 ? list.alloc + list.alloc : 0x10)); // 004BF815..31
        list.data[list.count++] = cr;                                                  // 004BF836..42
    }
    copy_vector(out, to);                                                              // 004BF853..72
    if (list.count == 0) { live_free(list.data); return 0; }                           // 004BF875..97
    U moved = 0;                                                                       // [1C]
    U changed;                                                                         // [18]
    do {
        changed = 0;                                                                   // 004BF8AC
        for (I i = 0; i < list.count; ++i) {                                           // 004BF8C0..DD0
            void* cr = list.data[i];
            const Vector c0 = at<Vector>(cr, 0x90);                                    // [B4]
            const Vector o = *out;                                                     // [A8]
            const float radius = at<float>(at<void*>(cr, 0x340), 0x10);                // [30]
            {
                const double ddy = static_cast<double>(c0.y) - o.y, ddx = static_cast<double>(c0.x) - o.x;
                const double s = ddx * ddx + ddy * ddy;                                // 004BF933..3B
                const double rr = static_cast<double>(radius) * radius;                // 004BF93D..41
                if (!(rr > s)) continue;                                               // 004BF945..50 (test ah,41h; jne)
            }
            Vector a = *from; a.z = 0.0f;                                              // [24] (z cleared at 004BF9E0)
            Vector b = *out;  b.z = 0.0f;                                              // [58] (z cleared at 004BF9E8)
            Vector d;                                                                  // [C0]
            d.x = static_cast<float>(static_cast<double>(b.x) - a.x);
            d.y = static_cast<float>(static_cast<double>(b.y) - a.y);
            d.z = 0.0f;                                                                // 004BF9F0
            Vector n;                                                                  // [108]
            const Vector* nn = normalize_004ab180(&n, &d);                             // 004BF9FB
            const float ez = static_cast<float>(static_cast<double>(nn->z) * k_step_0073d708);   // [10]
            const float ey = static_cast<float>(static_cast<double>(nn->y) * k_step_0073d708);   // [14]
            const float ex = static_cast<float>(static_cast<double>(nn->x) * k_step_0073d708);   // [74]
            Vector center = at<Vector>(cr, 0x90); center.z = 0.0f;                     // [9C] (z cleared at 004BFA77)
            Vector i0{0, 0, 0}, i1{0, 0, 0};                                           // [4C], [34]
            const I roots = intersect_circle_2d_004aab50(&center, radius, &a, &b, &i0, &i1, true);   // 004BFA6E..82
            double t;
            if (roots == 2) {                                                          // 004BFA8A
                const double az = static_cast<double>(from->z) - i0.z, ay = static_cast<double>(from->y) - i0.y,
                             ax = static_cast<double>(from->x) - i0.x;
                const double bz = static_cast<double>(from->z) - i1.z, by = static_cast<double>(from->y) - i1.y;
                const float bxf = static_cast<float>(static_cast<double>(from->x) - i1.x);   // [CC]
                const double s0 = (ax * ax + ay * ay) + az * az;                       // 004BFAC2..D0
                const double s1 = (static_cast<double>(bxf) * bxf + by * by) + bz * bz;// 004BFAD2..EA
                const Vector& pick = (s1 > s0) ? i0 : i1;                              // 004BFAEC..FD (test ah,41h; jne)
                out->x = static_cast<float>(static_cast<double>(pick.x) - ex);         // 004BFAFF..48 / 004BFB4D..96
                out->y = static_cast<float>(static_cast<double>(pick.y) - ey);
                out->z = static_cast<float>(static_cast<double>(pick.z) - ez);
                moved = 1; changed = 1;                                                // 004BFB9D..A5
                const double tt = dist2_mixed(static_cast<double>(a.x) - b.x, static_cast<double>(a.y) - b.y,
                                              static_cast<double>(a.z) - b.z);         // 004BFB99..EB
                if (tt == 0.0) continue;                                               // 004BFBED..F8 (fcom; test ah,44h; jnp)
                const double u = dist2_mixed(static_cast<double>(a.x) - out->x, static_cast<double>(a.y) - out->y,
                                             static_cast<double>(a.z) - out->z);       // 004BFBFE..44
                t = u / tt;                                                            // 004BFC46 fdivrp
            } else if (roots == 1) {                                                   // 004BFC69..6B
                if (!(out->x == i0.x) || !(out->y == i0.y)) {                          // 004BFC71..8A (test ah,44h)
                    out->x = static_cast<float>(static_cast<double>(i0.x) - ex);       // 004BFC8C..CB
                    out->y = static_cast<float>(static_cast<double>(i0.y) - ey);
                    out->z = static_cast<float>(static_cast<double>(i0.z) - ez);
                    moved = 1; changed = 1;
                }
                const float tt = static_cast<float>(dist2_mixed(static_cast<double>(a.x) - b.x, static_cast<double>(a.y) - b.y,
                                                                static_cast<double>(a.z) - b.z));   // 004BFCCE..20 [10]
                if (changed == 0) continue;                                            // 004BFD26 (EAX = [18] read at 004BFCD2)
                if (tt == 0.0f) continue;                                              // 004BFD2C..3B
                const double u = dist2_mixed(static_cast<double>(a.x) - out->x, static_cast<double>(a.y) - out->y,
                                             static_cast<double>(a.z) - out->z);       // 004BFD41..8D
                t = u / static_cast<double>(tt);                                       // 004BFD8F fdiv dword
            } else {
                continue;                                                              // 004BFC6B
            }
            // 004BFD93..C5: z interpolated between the endpoints' min and max z
            const double lo = from->z < to->z ? from->z : to->z;                       // 004BFD93..AE / 004BFC4D
            const double hi = from->z > to->z ? from->z : to->z;                       // 004BFC50..64 / 004BFDB3
            out->z = static_cast<float>(((hi - lo) * t - k_z_bias_0073dfec) + lo);     // 004BFDB6..C2
        }
    } while (changed == 1);                                                            // 004BFDD6..DB
    live_free(list.data);                                                              // 004BFDE1..F1
    return moved;                                                                      // 004BFDF6
}
