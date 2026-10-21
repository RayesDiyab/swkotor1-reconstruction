// W5 pathfind-walk lane, fork A: CAvoidCreature (the "walk around a blocking creature" planner used by
// CSWSCreature::WalkUpdateLocationDistance 00516630). Transcribed from the original swkotor.exe (read-only);
// addresses in the comments are the original instructions.
//   CAvoidCreature layout (from the bodies below): +0 side flag (0 = right / clockwise, else left),
//   +4..+4B six hex corner points (Vector), +4C/+50 entry path-pair index / entry hex edge, +54 entry point,
//   +60/+64 exit path-pair index / exit hex edge, +68 exit point. Size 0x74.
// Conventions: thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; ST0 returns as double; live_operator_new /
// live_free only.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"
#include "4f622be2f2-w5_area_direct_line.hpp"

using namespace w5;

namespace frame_control { extern unsigned message_manager_007a3a00; }   // [007A3A00] CVirtualMachine* [0456]

extern "C" {
// ---------------- existing providers (c05 map), exact C-linkage names ----------------
I __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U id, U flag);     // GetReputation [W1 b7]
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U object, U flag);           // CVirtualMachine::RunScript
void __stdcall vector_constructor_iterator_00401130(void* array, U size, U count, void* ctor); // `eh vector ctor iterator' [0526]
void* __fastcall vector_default_construct_00409e70(void* v, void*);                            // Vector::Vector (zero) [0526]

// ---------------- W5-owned storage (no other owner in the c05 map) ----------------
// [00832884] .bss counter: only written by PlotPathAroundCreature (inc at 005D0860), never read in the image.
U w5_cell_avoid_plot_count_00832884 = 0;

// ---------------- reconstructed in this file ----------------
void* __fastcall kotor_fastcall_00514ab0(void* self, void*);
void __fastcall kotor_fastcall_0050e270(void* self, void*);
U __fastcall kotor_fastcall_005cecc0(void* self, void*, I first, I last, const Vector* segment, I segment_count, void* pf);
void __fastcall kotor_fastcall_005cf1f0(void* self, void*, const Vector* center, const Vector* direction, float radius);
Vector* __fastcall kotor_fastcall_005cf650(void* self, void*, I* count);
Vector* __fastcall kotor_fastcall_005cf7a0(void* self, void*, I* count);
U __fastcall kotor_fastcall_005cf8d0(void* self, void*, U id, const Vector* from, const Vector* to, U* blocker);
I __fastcall kotor_fastcall_005cfae0(void* self, void*, const Vector* p);
U __fastcall kotor_fastcall_005cfc70(void* self, void*, const Vector* p);
U __fastcall kotor_fastcall_005d0160(void* self, void*, void* pf);
Vector* __fastcall kotor_fastcall_005d0690(void* self, void*, I* count);
U __fastcall kotor_fastcall_005d0770(void* self, void*, U id, const Vector* path, I count, U* blocker);
I __fastcall kotor_fastcall_005d0840(void* self, void*, void* creature, void* blocker);
}

namespace {
const float k_tolerance_0073d708 = 0.001f;        // .rdata 0073D708
const float k_zero_0073d700 = 0.0f;               // .rdata 0073D700
const float k_debug_lift_0073e46c = 0.2f;         // .rdata 0073E46C
const U k_cos30_0074c454 = 0x3f5db3d0u;           // .rdata 0074C454 (0.866025)
const U k_flt_max_0074c468 = 0x7f7fffffu;         // .rdata 0074C468
const U k_minus_60deg = 0xbf860a92u;              // immediate at 005CF243 (-1.0471976 rad)
const I k_path_failed_0074c45c = 0;               // .rdata 0074C45C (the caller's compare value; 0 = failure)
// [007A2274] .data float 0.2: read only by PlotPathAroundCreature (005D0985), no writer in the image.
const float k_hex_margin_007a2274 = 0.2f;
// [00832880] .bss debug flag: read only by SideClearOfObstructions (005CF9E8), no writer in the image -> 0.
const U k_debug_flag_00832880 = 0;
const char k_def_pathfail01_0074c46c[] = "k_def_pathfail01";   // .rdata 0074C46C

inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline Vector* hex(void* self, I i) { return reinterpret_cast<Vector*>(bytes(self) + 4 + 12 * i); }
inline Vector* vat(void* p, U off) { return reinterpret_cast<Vector*>(bytes(p) + off); }
inline Vector vec(float x, float y, float z) { Vector v; v.x = x; v.y = y; v.z = z; return v; }
inline void* area_cells() {                                  // GetModule()->GetArea() + 0x10C
    return bytes(module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr)) + 0x10c;
}
inline double sqf(float f) { return static_cast<double>(f) * f; }  // fld dword / fmul dword
// CExoString "k_def_pathfail01" local with the original unwind (state 0 -> ~CExoString)
struct ScriptName {
    alignas(4) unsigned char s[8];
    ScriptName() { frame_LogStringCtor_005e5a90(s, nullptr, k_def_pathfail01_0074c46c); }
    ~ScriptName() { frame_LogStringDtor_005e5c20(s, nullptr); }
};
inline void run_pathfail(ScriptName& name, U id) {          // CVirtualMachine::RunScript(name, id, id != invalid)
    frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)),
                      nullptr, name.s, id, id != k_object_invalid ? 1u : 0u);
}
}  // namespace

extern "C" {

// =========================================================================================================
// 00514AB0 CAvoidCreature::CAvoidCreature
void* __fastcall kotor_fastcall_00514ab0(void* self, void*) {
    for (I i = 0; i < 6; ++i) *hex(self, i) = vec(0, 0, 0);                         // 00514AB3..CC (Vector ctors)
    *vat(self, 0x54) = vec(0, 0, 0);                                               // 00514ACE..D4
    *vat(self, 0x68) = vec(0, 0, 0);                                               // 00514AD7..DD
    at<U>(self, 0) = 0;                                                             // 00514AE0
    kotor_fastcall_0050e270(self, nullptr);                                         // 00514AE4
    return self;
}

// 0050E270 CAvoidCreature::Reset (the side flag at +0 is kept)
void __fastcall kotor_fastcall_0050e270(void* self, void*) {
    for (I i = 0; i < 6; ++i) *hex(self, i) = vec(0, 0, 0);                         // 0050E274..8B (via stack temps)
    *vat(self, 0x54) = vec(0, 0, 0);                                               // 0050E38E..D5
    at<I>(self, 0x4c) = -1; at<I>(self, 0x50) = -1;                                 // 0050E39F..C0
    at<I>(self, 0x60) = -1; at<I>(self, 0x64) = -1;                                 // 0050E3C3..C6
    *vat(self, 0x68) = vec(0, 0, 0);                                               // 0050E3C9..F8
}

// =========================================================================================================
// 005CF1F0 CAvoidCreature::ComputeHexPoints(center, direction, radius): RET 0Ch. Six corners of a hexagon of
// circumradius radius/cos30 around center, the first one opposite the mover's direction, rotated by -60 deg
// steps about +Z; corners 1..5 are dropped onto the walkmesh (corner 0 keeps the center height).
void __fastcall kotor_fastcall_005cf1f0(void* self, void*, const Vector* center, const Vector* direction, float radius) {
    kotor_fastcall_0050e270(self, nullptr);                                         // 005CF1F8
    Vector back = vec(-direction->x, -direction->y, -direction->z);                 // 005CF201..20 [E-1C]
    Vector off;                                                                     // [E-28]
    normalize_004ab180(&off, &back);                                               // 005CF224..25
    vector_scale_assign_004a97a0(&off, nullptr,
        static_cast<float>(static_cast<double>(radius) / flt(k_cos30_0074c454)));  // 005CF22A..3E
    Quaternion q;                                                                   // [E-10]
    quaternion_construct_004ac960(&q, nullptr, vec(0, 0, 1.0f), flt(k_minus_60deg)); // 005CF243..7D
    Vector p = vec(static_cast<float>(static_cast<double>(off.x) + center->x),
                   static_cast<float>(static_cast<double>(off.y) + center->y),
                   static_cast<float>(static_cast<double>(off.z) + center->z));     // 005CF282..BA
    *hex(self, 0) = p;                                                              // 005CF2B1..CA
    for (I k = 1; k < 6; ++k) {                                                     // 005CF2CD..61A (unrolled x5)
        Vector rotated;                                                             // [E-1C]
        const Vector* r = quaternion_rotate_004a9bb0(&rotated, &q, &off);           // 005CF2CD..D8
        off = *r;                                                                   // 005CF2DD..F6
        p = vec(static_cast<float>(static_cast<double>(off.x) + center->x),
                static_cast<float>(static_cast<double>(off.y) + center->y),
                static_cast<float>(static_cast<double>(off.z) + center->z));        // 005CF2E3..2E [E-34]
        p.z = static_cast<float>(w5_server_compute_height_004aea50(server_app(), nullptr, area_cells(), &p, 1)); // 005CF332..5F
        *hex(self, k) = p;                                                          // 005CF363..77
    }
}

// =========================================================================================================
// 005CFC70 CAvoidCreature::IsPointInHex(point): RET 4. 1 unless the point is strictly outside one edge.
U __fastcall kotor_fastcall_005cfc70(void* self, void*, const Vector* p) {
    for (I i = 0; i < 6; ++i) {                                                     // 005CFC82..64
        const I next = (i + 1) % 6;                                                 // 005CFC82..8B (idiv)
        const Vector* a = hex(self, i);                                             // ESI-8
        const Vector* b = hex(self, next);
        Vector up = vec(0, 0, 1.0f);                                                // [E-48]
        Vector edge = vec(static_cast<float>(static_cast<double>(b->x) - a->x),
                          static_cast<float>(static_cast<double>(b->y) - a->y),
                          static_cast<float>(static_cast<double>(b->z) - a->z));    // 005CFCAC..D5 [E-54]
        Vector cross;                                                               // [E-C]
        const Vector* c = vector_cross_004ab210(&cross, &up, &edge);                // 005CFCD2..D9
        Vector inward = vec(-c->x, -c->y, -c->z);                                   // 005CFCDE..FE [E-3C]
        Vector n;                                                                   // [E-18]
        normalize_004ab180(&n, &inward);                                           // 005CFCF4..02
        Vector d = vec(static_cast<float>(static_cast<double>(p->x) - a->x),
                       static_cast<float>(static_cast<double>(p->y) - a->y),
                       static_cast<float>(static_cast<double>(p->z) - a->z));       // 005CFD07..29 [E-30]
        Vector m;                                                                   // [E-24]
        normalize_004ab180(&m, &d);                                                // 005CFD10..2D
        const double dot = (static_cast<double>(m.z) * n.z + static_cast<double>(m.y) * n.y) +
                           static_cast<double>(m.x) * n.x;                          // 005CFD32..4F
        if (dot < static_cast<double>(k_zero_0073d700)) return 0;                  // 005CFD51..5C (test ah,5; jnp)
    }
    return 1;
}

// 005CFAE0 CAvoidCreature::GetClosestPointTo(point): RET 4, EAX = index of the nearest hex corner (3D)
I __fastcall kotor_fastcall_005cfae0(void* self, void*, const Vector* p) {
    float best = flt(k_flt_max_0074c468);                                           // 005CFAE4 (ST, then [E+4])
    I index = -1;                                                                   // 005CFAF0
    for (I i = 0; i < 6; ++i) {                                                     // 005CFAEA..C5D (unrolled)
        const Vector* c = hex(self, i);
        const double dx = static_cast<double>(c->x) - p->x;
        const double dy = static_cast<double>(c->y) - p->y;
        const double dz = static_cast<double>(c->z) - p->z;
        const double d = (dz * dz + dx * dx) + dy * dy;                             // fld st(0)*st(1) + st(3)^2 + st(2)^2
        if (d < static_cast<double>(best)) {                                       // unrounded d vs float best (test ah,5)
            best = static_cast<float>(d);                                           // fst dword [E+4] / fld
            index = i;
        }
    }
    return index;
}

// =========================================================================================================
// 005D0160 CAvoidCreature::FindEntryAndExit(pathfind): RET 4. Walks the planned path (pairs at pf+90, count
// pf+8C floats) against the hex; records entry (+4C/+50/+54) and exit (+60/+64/+68); EAX = 1 when both found.
U __fastcall kotor_fastcall_005d0160(void* self, void*, void* pf) {
    I found = 0;                                                                    // [E-70]
    if (pf == nullptr) return 0;                                                    // 005D0167..A4
    const float* pts = at<const float*>(pf, 0x90);                                  // [E-60]
    if (pts == nullptr) return 0;                                                   // 005D01B0..B6
    if (at<I>(pf, 0x8c) == 0) return 0;                                             // 005D01BC..C4
    Vector prev = vec(pts[0], pts[1], 0);                                           // 005D01CC..FC [E-48]
    Vector cur = vec(0, 0, 0);                                                      // [E-54]
    I k = 2;                                                                        // EBP
    if (at<I>(pf, 0x8c) > 2) {                                                      // 005D01FA..00 (jle)
        do {
            cur = vec(pts[k], pts[k + 1], 0);                                       // 005D0210..33
            Vector hit = vec(0, 0, 0);                                              // [E-3C]
            for (I i = 0; i < 6;) {                                                 // 005D0254..7D
                const I next = (i + 1) % 6;                                         // 005D025E..6F
                const Vector* a = hex(self, i);                                     // EBX-8
                const Vector* b = hex(self, next);                                  // ECX
                const double a0 = static_cast<double>(prev.x) - a->x;               // 005D0257..75 (ST, unrounded)
                const double a1 = static_cast<double>(prev.y) - a->y;
                const double a2 = static_cast<double>(prev.z) - a->z;
                U near_prev_a = 1;                                                  // [E-5C]
                const Vector v1 = vec(static_cast<float>(static_cast<double>(prev.x) - b->x),
                                      static_cast<float>(static_cast<double>(prev.y) - b->y),
                                      static_cast<float>(static_cast<double>(prev.z) - b->z)); // [E-30]
                const Vector v2 = vec(static_cast<float>(static_cast<double>(cur.x) - a->x),
                                      static_cast<float>(static_cast<double>(cur.y) - a->y),
                                      static_cast<float>(static_cast<double>(cur.z) - a->z));  // [E-24]
                const Vector v3 = vec(static_cast<float>(static_cast<double>(cur.x) - b->x),
                                      static_cast<float>(static_cast<double>(cur.y) - b->y),
                                      static_cast<float>(static_cast<double>(cur.z) - b->z));  // [E-18]
                const double tol = k_tolerance_0073d708;
                if (!((a2 * a2 + a1 * a1) + a0 * a0 < tol)) near_prev_a = 0;       // 005D02EA..0D (test ah,5; jnp)
                U near_prev_b = 1;                                                  // [E-6C]
                if (!((sqf(v1.z) + sqf(v1.y)) + sqf(v1.x) < tol)) near_prev_b = 0;  // 005D0311..42
                U near_cur_a = 1;                                                   // [E-68]
                if (!((sqf(v2.z) + sqf(v2.y)) + sqf(v2.x) < tol)) near_cur_a = 0;   // 005D0346..77
                U near_cur_b = 1;                                                   // [E-64]
                if (!((sqf(v3.z) + sqf(v3.y)) + sqf(v3.x) < tol)) near_cur_b = 0;   // 005D037B..AC
                const I near = static_cast<I>(near_cur_b + near_cur_a + near_prev_b + near_prev_a); // 005D03B0..C2
                if (near >= 2) {                                                    // 005D03C4..C7 (jl)
                    if (found == 0 && (near_prev_a != 0 || near_prev_b != 0)) {     // 005D03C9..DB
                        at<I>(self, 0x60) = k / 2;                                  // 005D03DD..E8
                        *vat(self, 0x68) = prev;                                    // 005D03EB..FE
                        at<I>(self, 0x64) = i;                                      // 005D03FB
                        found = 1;                                                  // 005D0401
                    }
                    if (near_cur_a != 0 || near_cur_b != 0) {                       // 005D0409..17
                        at<I>(self, 0x60) = k / 2;                                  // 005D0419..24
                        *vat(self, 0x68) = cur;                                     // 005D0427..37
                        found = found + 1;                                          // 005D043A..42
                        at<I>(self, 0x64) = i;                                      // 005D043F
                    }
                }
                if (intersect_line_segments_2d_004ac340(&prev, &cur, a, b, &hit) != 0) { // 005D0446..64
                    if (found == 0) {                                               // 005D0466..6C
                        at<I>(self, 0x60) = k / 2;                                  // 005D0485..94
                        *vat(self, 0x68) = hit;                                     // 005D0497..A3
                        found = 1;                                                  // 005D04A6
                        at<I>(self, 0x64) = i;                                      // 005D04FB
                        break;
                    }
                    if (found >= 1) {                                               // 005D046E..71
                        at<I>(self, 0x4c) = at<I>(self, 0x60);                      // 005D04B0..B6
                        at<I>(self, 0x50) = at<I>(self, 0x64);                      // 005D04BC
                        *vat(self, 0x54) = *vat(self, 0x68);                        // 005D04B9..D1
                        at<I>(self, 0x60) = k / 2;                                  // 005D04D4..DF
                        *vat(self, 0x68) = hit;                                     // 005D04E2..F4
                        found = found + 1;                                          // 005D04EF..F7
                        at<I>(self, 0x64) = i;                                      // 005D04FB
                        break;
                    }
                }
                i = i + 1;                                                          // 005D0473..7D
            }
            prev = cur;                                                             // 005D04FE..28
            k += 2;                                                                 // 005D051F
        } while (k < at<I>(pf, 0x8c));                                              // 005D0519..2C (re-read)
    }
    if (at<I>(self, 0x50) == -1) {                                                  // 005D0535..3D
        if (at<I>(self, 0x64) == -1) return 0;                                      // 005D0543..52
        // 005D055D..F0: no entry crossing -> nearest corner to the current path point
        const I cursor = at<I>(pf, 0x9c);
        I yi = cursor - 1;
        if (yi < 1) yi = 1;                                                         // 005D0563..6B
        I xi = cursor - 2;
        if (xi < 0) xi = 0;                                                         // 005D0570..83 (setl/dec/and)
        Vector probe = vec(pts[xi], pts[yi], 0);                                    // 005D057A..96 [E-C]
        const I corner = kotor_fastcall_005cfae0(self, nullptr, &probe);            // 005D059A
        at<I>(self, 0x50) = corner;                                                 // 005D05A8
        I pair = (at<I>(pf, 0x9c) - 2) / 2;                                         // 005D05AB..B7 (cdq/sub/sar)
        if (pair <= 0) pair = 0;                                                    // 005D05B9..C1 (setle/dec/and)
        at<I>(self, 0x4c) = pair;                                                   // 005D05C3
        *vat(self, 0x54) = *hex(self, corner);                                      // 005D05C6..E1
        found = found + 1;                                                          // 005D05EF..F0
    }
    if (at<I>(self, 0x64) == -1) {                                                  // 005D05F4..F7
        const I n = at<I>(pf, 0x8c);                                                // 005D05F9
        Vector probe = vec(pts[n - 2], pts[n - 1], 0);                              // 005D05FF..16 [E-C]
        const I corner = kotor_fastcall_005cfae0(self, nullptr, &probe);            // 005D061E
        at<I>(self, 0x64) = corner;                                                 // 005D062C
        at<I>(self, 0x60) = at<I>(pf, 0x8c) / 2;                                    // 005D062F..3A
        *vat(self, 0x68) = *hex(self, corner);                                      // 005D063D..54
        found = found + 1;                                                          // 005D0657
    }
    const U result = found >= 2 ? 1u : 0u;                                          // 005D065B..64
    (void)aurora_vector_equal_004aa980(vat(self, 0x68), vat(self, 0x54));           // 005D0667..76 (result unused)
    return result;
}

// =========================================================================================================
// 005CF650 CAvoidCreature::FindPath_Left(int* count): RET 4, EAX = new Vector[count] (entry, corners walking
// down from the entry edge to the exit edge's far corner, exit)
Vector* __fastcall kotor_fastcall_005cf650(void* self, void*, I* count) {
    Vector path[10];                                                                // [E-78]
    for (I i = 0; i < 10; ++i) path[i] = vec(0, 0, 0);                              // 005CF656..6D
    *count = 0;                                                                     // 005CF676
    I edge = at<I>(self, 0x50);                                                     // 005CF678 ESI
    path[0] = *vat(self, 0x54);                                                     // 005CF67B..8E
    *count = 1;                                                                     // 005CF692
    path[1] = *hex(self, edge);                                                     // 005CF698..B9
    *count = 2;                                                                     // 005CF6AF
    I target = (at<I>(self, 0x64) + 1) % 6;                                         // 005CF6B5..C4
    while (edge != target) {                                                        // 005CF6C6..19
        edge = (edge - 1) % 6;                                                      // 005CF6D0..DB (idiv: -1 stays -1)
        if (edge < 0) edge = 5;                                                     // 005CF6DD..E1
        path[*count] = *hex(self, edge);                                            // 005CF6E6..09
        *count = *count + 1;                                                        // 005CF700..01
        target = (at<I>(self, 0x64) + 1) % 6;                                       // 005CF703..13 (re-read)
    }
    path[*count] = *vat(self, 0x68);                                                // 005CF71B..37
    const I n = *count + 1;                                                         // 005CF734
    *count = n;                                                                     // 005CF741
    Vector* out = static_cast<Vector*>(live_operator_new(static_cast<U>(n) * 12));  // 005CF73A..43
    if (out != nullptr) {
        for (I i = 0; i < n; ++i) out[i] = vec(0, 0, 0);                            // 005CF750..6C (Vector ctors)
    }
    // 005CF772..8C rep movsd/movsb. Original quirk: a failed allocation copies into address 0.
    const U size = static_cast<U>(*count) * 12;
    std::memcpy(out, path, size);
    return out;
}

// 005CF7A0 CAvoidCreature::FindPath_Right(int* count): RET 4, EAX = new Vector[count] (entry, corners walking
// up from the entry edge's far corner to the exit edge, exit)
Vector* __fastcall kotor_fastcall_005cf7a0(void* self, void*, I* count) {
    Vector path[10];                                                                // [E-78]
    for (I i = 0; i < 10; ++i) path[i] = vec(0, 0, 0);                              // 005CF7A6..BD
    *count = 0;                                                                     // 005CF7C6
    I corner = (at<I>(self, 0x50) + 1) % 6;                                         // 005CF7C8..D2 (EDX)
    path[0] = *vat(self, 0x54);                                                     // 005CF7D4..ED
    *count = 1;                                                                     // 005CF7E7
    path[1] = *hex(self, corner);                                                   // 005CF7F1..11
    *count = 2;                                                                     // 005CF808
    while (corner != at<I>(self, 0x64)) {                                           // 005CF80E..51 (re-read)
        corner = (corner + 1) % 6;                                                  // 005CF820..29
        path[*count] = *hex(self, corner);                                          // 005CF82B..4E
        *count = *count + 1;                                                        // 005CF834..46
    }
    path[*count] = *vat(self, 0x68);                                                // 005CF856..72
    const I n = *count + 1;                                                         // 005CF86F
    *count = n;                                                                     // 005CF87C
    Vector* out = static_cast<Vector*>(live_operator_new(static_cast<U>(n) * 12));  // 005CF875..7E
    if (out != nullptr) {
        for (I i = 0; i < n; ++i) out[i] = vec(0, 0, 0);                            // 005CF88A..A1
    }
    const U size = static_cast<U>(*count) * 12;                                     // 005CF8A7..C1 (null quirk as Left)
    std::memcpy(out, path, size);
    return out;
}

// 005D0690 CAvoidCreature::FindPath(int* count): RET 4, EAX = new Vector[*count]
Vector* __fastcall kotor_fastcall_005d0690(void* self, void*, I* count) {
    if (at<I>(self, 0x50) == at<I>(self, 0x64)) {                                   // 005D06A9..AF
        void* memory = live_operator_new(0x18);                                     // 005D06B2..B4
        Vector* pair = nullptr;
        if (memory != nullptr) {                                                    // 005D06C4..CA
            vector_constructor_iterator_00401130(memory, 0xc, 2,
                reinterpret_cast<void*>(&vector_default_construct_00409e70));       // 005D06CC..D6
            pair = static_cast<Vector*>(memory);
        }
        // 005D06DD..05 (original quirk: no null test; a failed allocation writes to address 0)
        copy_vector(&pair[0], vat(self, 0x54));
        copy_vector(&pair[1], vat(self, 0x68));
        *count = 2;                                                                 // 005D070D
        return pair;
    }
    if (at<U>(self, 0) != 0) return kotor_fastcall_005cf650(self, nullptr, count);  // 005D0725..31
    return kotor_fastcall_005cf7a0(self, nullptr, count);                           // 005D0748..4F
}

// =========================================================================================================
// 005CF8D0 CAvoidCreature::SideClearOfObstructions(id, from, to, U* blocker): RET 10h. EAX 1 when the mover's
// TestDirectLine succeeds; a -3 (creature) result reports the blocking object.
U __fastcall kotor_fastcall_005cf8d0(void* self, void*, U id, const Vector* from, const Vector* to, U* blocker) {
    (void)self;
    *blocker = k_object_invalid;                                                    // 005CF8DE
    WalkHit hit;                                                                    // [E-48] CWalkHitInfo
    hit.flag_0 = 0; hit.point_4 = vec(0, 0, 0); hit.normal_10 = vec(0, 0, 0);       // 005CF8ED..69
    for (I i = 0; i < 9; ++i) hit.scratch_1c[i] = 0.0f;
    hit.blocker_40 = k_object_invalid; hit.type_44 = 0;                             // 005CF96D / 005CF969
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);            // 005CF975..79
    I result = -4;                                                                  // 005CF988
    if (creature != nullptr) {                                                      // 005CF982..8D
        void* pf = at<void*>(creature, 0x340);
        float radius = at<float>(pf, 4);                                            // 005CF995..9E (over the arg slot)
        const float band = at<float>(pf, 0x14);                                     // 005CF9A2..A8
        result = w5_server_test_direct_line_004aee80(server_app(), nullptr, id, area_cells(), from, to, &radius,
                                                     band, 1, &hit);                // 005CF9AC..E6
    }
    if (k_debug_flag_00832880 != 0) {                                               // 005CF9E8..EF (debug heights)
        Vector a = *from, b = *to;                                                  // 005CF9F5..1E [E-60], [E-54]
        a.z = static_cast<float>(w5_server_compute_height_004aea50(server_app(), nullptr, area_cells(), &a, 0) +
                                 k_debug_lift_0073e46c);                            // 005CFA22..5A
        (void)w5_server_compute_height_004aea50(server_app(), nullptr, area_cells(), &b, 0); // 005CFA5E..8B
    }
    switch (static_cast<U>(result + 3)) {                                           // 005CFA8F..99 (table 005CFAC0)
    case 0: *blocker = hit.blocker_40; return 0;                                    // -3 creatures: 005CFAAB..B5
    case 4: return 1;                                                               // 1 clear: 005CFAA0
    default: return 0;                                                              // 005CFAB5
    }
}

// 005D0770 CAvoidCreature::IsPathClear(id, path, count, U* blocker): RET 10h
U __fastcall kotor_fastcall_005d0770(void* self, void*, U id, const Vector* path, I count, U* blocker) {
    *blocker = k_object_invalid;                                                    // 005D077F
    Vector prev = path[0];                                                          // 005D0786..A5 [E-C]
    Vector cur = vec(0, 0, 0);                                                      // 005D07A9..B9 [E-18]
    for (I i = 1; i < count; ++i) {                                                 // 005D079E..19 (jle / jl)
        cur = path[i];                                                              // 005D07C6..E2
        if (kotor_fastcall_005cf8d0(self, nullptr, id, &prev, &cur, blocker) == 0) return 0; // 005D07D8..F5
        prev = cur;                                                                 // 005D07F7..15
    }
    return 1;
}

// =========================================================================================================
// 005CECC0 CAvoidCreature::InsertPathSegmentBetween(first, last, segment, segment_count, pathfind): RET 14h.
// Replaces path pairs [first .. last] (first == last: just that one) of pf+90 with the segment's x/y pairs.
U __fastcall kotor_fastcall_005cecc0(void* self, void*, I first, I last, const Vector* segment, I segment_count,
                                     void* pf) {
    (void)self;
    const U* old = at<const U*>(pf, 0x90);                                          // 005CECCF EBP
    I skip;                                                                         // [E-4]
    I count;                                                                        // EDX, kept in the pf arg slot
    if (first == last) {                                                            // 005CECD8..DB
        count = at<I>(pf, 0x8c) + segment_count * 2 - 2;                            // 005CECDD..E7
        skip = 1;
    } else {
        count = at<I>(pf, 0x8c) + (first - last + segment_count) * 2;               // 005CECF5..01
        skip = 0;
    }
    U* out = static_cast<U*>(live_operator_new(static_cast<U>(count) * 4));         // 005CED08..16 (no null test)
    I w = 0;                                                                        // ESI
    for (I i = 0; i < first; ++i) { out[w++] = old[2 * i]; out[w++] = old[2 * i + 1]; } // 005CED24..43
    for (I j = 0; j < segment_count; ++j) {                                         // 005CED45..62
        out[w++] = bits(segment[j].x); out[w++] = bits(segment[j].y);
    }
    for (I i = last + skip; i < at<I>(pf, 0x8c) / 2; ++i) {                         // 005CED64..A0 (count re-read)
        out[w++] = old[2 * i]; out[w++] = old[2 * i + 1];
    }
    at<U>(pf, 0x90) = 0;                                                            // 005CEDA5
    at<U>(pf, 0x8c) = 0;                                                            // 005CEDAB
    live_free(const_cast<U*>(old));                                                 // 005CEDB1 (unconditional)
    at<U*>(pf, 0x90) = out;                                                         // 005CEDBD
    at<I>(pf, 0x8c) = count;                                                        // 005CEDC6
    return 1;
}

// =========================================================================================================
// 005D0840 CAvoidCreature::PlotPathAroundCreature(creature, blocker): RET 8.
// EAX: 1 detour inserted into the creature's path, 0 no detour (pathfail script run, or no entry/exit),
// -2 path end inside the blocker's hex.
I __fastcall kotor_fastcall_005d0840(void* self, void*, void* creature, void* blocker) {
    w5_cell_avoid_plot_count_00832884 = w5_cell_avoid_plot_count_00832884 + 1;      // 005D0860
    void* pf = at<void*>(creature, 0x340);                                          // 005D0874
    const U blocker_id = at<U>(blocker, 4);                                         // 005D0866
    at<U>(pf, 0x254) = blocker_id;                                                  // 005D087A (inline SetBlockingCreature)
    if (blocker_id == k_object_invalid) *vat(pf, 0x258) = vec(0, 0, 0);             // 005D0880..B1
    copy_vector(vat(at<void*>(creature, 0x340), 0x258), vat(blocker, 0x90));        // 005D08B4..D4
    U left_blocker = k_object_invalid;                                              // [E+8] (arg slot reused)
    U right_blocker = k_object_invalid;                                             // [E-28]
    I left_count = 0;                                                               // [E-2C]
    I right_count = 0;                                                              // [E+4] (arg slot reused)
    ScriptName name;                                                                // 005D08E6..F7 [E-20], EH state 0
    // 005D08FC..1B: empty loop over pf+8C (no effect)
    if (recovered_object_get_reputation_0057cb80(blocker, nullptr, at<U>(creature, 4), 0) <= 10) { // 005D091D..2D
        at<U>(at<void*>(creature, 0x340), 0x264) = at<U>(blocker, 4);               // 005D092F..38
        run_pathfail(name, at<U>(creature, 4));                                     // 005D093E..58
        if (at<U>(creature, 0xe8) != 0) return 0;                                   // 005D095D..65 -> 005D0A2F
    }
    {
        void* mover_pf = at<void*>(creature, 0x340);                                // 005D096B
        void* blocker_pf = at<void*>(blocker, 0x340);                               // 005D0971
        const float radius = static_cast<float>((static_cast<double>(at<float>(blocker_pf, 8)) +
                                                 at<float>(mover_pf, 8)) + k_hex_margin_007a2274); // 005D0977..8B
        kotor_fastcall_005cf1f0(self, nullptr, vat(blocker, 0x90), vat(creature, 0x9c), radius); // 005D0993..9A
    }
    const float* pts = at<const float*>(at<void*>(creature, 0x340), 0x90);          // 005D099F..A5 EBX
    Vector probe = vec(pts[0], pts[1], 0);                                          // 005D09AB..BF [E-18]
    (void)kotor_fastcall_005cfc70(self, nullptr, &probe);                           // 005D09C7 (result unused, quirk)
    const I n = at<I>(at<void*>(creature, 0x340), 0x8c);                            // 005D09CC..D2
    probe = vec(pts[n - 2], pts[n - 1], 0);                                         // 005D09D8..F1
    if (kotor_fastcall_005cfc70(self, nullptr, &probe) != 0) return -2;             // 005D09F9..18
    if (kotor_fastcall_005d0160(self, nullptr, at<void*>(creature, 0x340)) == 0) return 0; // 005D0A1D..42
    Vector* left = kotor_fastcall_005d0690(self, nullptr, &left_count);             // 005D0A47..4E [E-24]
    const I left_n = left_count;                                                    // EBP
    const U left_clear = kotor_fastcall_005d0770(self, nullptr, at<U>(creature, 4), left, left_n, &left_blocker); // 005D0A57..6A
    at<U>(self, 0) = at<U>(self, 0) == 0 ? 1u : 0u;                                 // 005D0A71..84 (other side)
    Vector* right = kotor_fastcall_005d0690(self, nullptr, &right_count);           // 005D0A7F..88 (over [E-2C])
    const U right_clear = kotor_fastcall_005d0770(self, nullptr, at<U>(creature, 4), right, right_count,
                                                  &right_blocker);                  // 005D0A8D..A4
    I left_score, right_score;                                                      // EDX, ECX
    if (left_clear != 0) left_score = 2;                                            // 005D0A6F..BB (neg/sbb/inc)
    else left_score = left_blocker != k_object_invalid ? 1 : 0;                     // 005D0ABD..D0
    if (right_clear != 0) right_score = 2;                                          // 005D0AA9..DF
    else right_score = right_blocker != k_object_invalid ? 1 : 0;                   // 005D0AE1..F0
    I result;                                                                       // ESI
    Vector* chosen = nullptr; I chosen_n = 0; U chosen_blocker = k_object_invalid;
    bool use = false;
    if (left_score < right_score) {                                                 // 005D0AF2..F4 (jl)
        if (right_score > 0) { use = true; chosen = right; chosen_n = right_count; chosen_blocker = right_blocker; } // 005D0B37..39
    } else if (left_score > 0) {                                                    // 005D0AF6..F8 (jle)
        use = true; chosen = left; chosen_n = left_n; chosen_blocker = left_blocker;
    }
    if (use) {                                                                      // 005D0AFA / 005D0BCB
        kotor_fastcall_005cecc0(self, nullptr, at<I>(self, 0x4c), at<I>(self, 0x60), chosen, chosen_n,
                                at<void*>(creature, 0x340));                        // 005D0AFA..11 / 005D0BCB..E6
        void* mover_pf = at<void*>(creature, 0x340);                                // 005D0B16
        const I entry = at<I>(self, 0x4c);                                          // 005D0B1C
        if (at<I>(mover_pf, 0x9c) > entry + entry) {                                // 005D0B1F..2A (jle)
            const I pair2 = entry + entry;
            at<I>(mover_pf, 0x9c) = pair2 >= 0 ? pair2 : 0;                         // 005D0B2C..77
        }
        if (chosen_blocker != k_object_invalid) {                                   // 005D0B7D..87 / 005D0C14..1A
            void* other = kotor_fastcall_004ae770(server_app(), nullptr, chosen_blocker); // 005D0B8D..97
            if (other != nullptr &&
                recovered_object_get_reputation_0057cb80(other, nullptr, at<U>(creature, 4), 0) <= 10) { // 005D0BA4..B4
                at<U>(at<void*>(creature, 0x340), 0x264) = chosen_blocker;          // 005D0BBA..C0
                run_pathfail(name, at<U>(creature, 4));                             // 005D0C4D..67
            }
        }
        result = 1;                                                                 // 005D0C6C
    } else {
        at<U>(at<void*>(creature, 0x340), 0x264) = k_object_invalid;                // 005D0B3F..45
        run_pathfail(name, at<U>(creature, 4));                                     // 005D0B4F..69
        result = 0;                                                                 // 005D0B6E
    }
    // 005D0C71..88: empty loop over pf+8C (no effect)
    if (left != nullptr) live_free(left);                                           // 005D0C8A..98
    if (right != nullptr) live_free(right);                                         // 005D0C9B..A9
    return result;                                                                  // ~CExoString at 005D0CB8
}

}  // extern "C"

static_assert(sizeof(w5::WalkHit) == 0x48, "CWalkHitInfo");
static_assert(k_path_failed_0074c45c == 0, "caller compare value");
