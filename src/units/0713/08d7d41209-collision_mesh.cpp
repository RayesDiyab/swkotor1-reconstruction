// lane-world-los / fork MESH: CSWCollisionMesh transforms and NoNonWalkPolys (swkotor.exe GOG K1).
// 00596A00 WorldToLocal, 00596AA0 LocalToWorld, 00596B30 (rotate), 005972B0 NoNonWalkPolys.
//
// Mesh layout (byte offsets, each cited at its first use; Ghidra placeholder names from swkotor.exe.h
// CSWCollisionMesh quoted only as orientation):
//   +00 vtable (CSWCollisionMeshMethods; slot +14h called at 00598017, Ghidra name GetWalkCheckMaterialMask)
//   +04 dword "world_coords": nonzero -> transforms are identity copies (00596A06, 00596AA6, 00596B30, 005972DF)
//   +20/+24/+28 floats (subtracted first by WorldToLocal 00596A38..00596A54, added last by LocalToWorld)
//   +28,+34 floats subtracted from the z bounds (005972FF/00597302)
//   +2C/+30/+34 floats (subtracted last by WorldToLocal 00596A7C..00596A85, added first by LocalToWorld)
//   +38 Quaternion (00596A51, 00596AF4, 00596B5F, 0059942F, 0059982F, 00599E2F)
//   +54 Vector* vertices (00598061), +58 face end (00598024 / 00598C79), +5C face start (00598021),
//   +60 face index triples (I[3] per face, 00598050), +64 per-face material ids (005980FC),
//   +68 per-face normals (Vector per face, 00598F0E / 005997FE)
//
// x87 policy (D30-FINAL): every fld/arith chain is evaluated in double; float only where the original stores
// with fstp/fst dword. Raw DWORD moves (mov) are reproduced as raw 32-bit copies (Frame::w).
//
// Frame model: 005972B0 reuses its 0x234-byte local area for differently-typed values whose lifetimes overlap
// through address-taken vectors (e.g. L22C is both a scalar and the z of the vector at L234; LDC is both the
// y of the normalize output at LE0 and the negated direction x; the vector at L1B0 read at 00599DF4 overlays
// the edge flags L1B0/L1AC and L1A8). To reproduce every such overlay exactly, the locals are one byte array
// addressed by the original frame offset: slot "Lxxx" = [entry ESP - xxx] (entry ESP = return address slot).
// D2: the array is value-initialised to zero (original stack garbage has no faithful value); every slot that
// the original reads on a reachable path is written earlier on that path except the L1B0/L1AC/L1A8 overlay
// noted at 00599DF4, which holds the values this function stored there earlier on the same path.
#include <cmath>
#include "dc46124e28-world_los.h"

namespace kotor_world_los {
volatile U debug_walk_draw_0083286c = 0u;      // .data 0083286C = 00000000
volatile U use_walk_check_mask_007a2070 = 1u;  // .data 007A2070 = 00000001
U debug_squares_007a2074[19] = {               // .data 007A2074..007A20BF = FFFFFFFF x19
    0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
    0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
    0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu};
}

namespace {
using kotor_world_los::U;
using kotor_world_los::I;
using kotor_world_los::Vector;
using kotor_world_los::Quaternion;

// .rdata float constants (bit patterns read from swkotor.exe)
constexpr float k_zero_0073d700 = 0.0f;                    // 00000000
constexpr float k_0_001_0073d708 = 0x1.0624dep-10f;        // 3A83126F
constexpr float k_0_01_0073dfec = 0x1.47ae14p-7f;          // 3C23D70A
constexpr float k_half_0073e9ac = 0.5f;                    // 3F000000
constexpr float k_0_03_00747678 = 0x1.eb851ep-6f;          // 3CF5C28F
constexpr float k_third_0074a094 = 0x1.555476p-2f;         // 3EAAAA3B (0.33333)
constexpr U k_flt_max_bits = 0x7F7FFFFFu;                  // 00597FFB immediate

// CSWCollisionMesh vtable view: only slot +14h is called in this closure (00598017, no stack args, EAX result).
struct CollisionMeshVtbl {
    void* slot_00[5];
    U (__fastcall* walk_check_material_mask_14)(void* self, void* unused_edx);
};
static_assert(offsetof(CollisionMeshVtbl, walk_check_material_mask_14) == 0x14, "slot +14h");

inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline U mesh_word(void* mesh, U off) { U v; std::memcpy(&v, bytes(mesh) + off, 4); return v; }
inline double mesh_float(void* mesh, U off) { float v; std::memcpy(&v, bytes(mesh) + off, 4); return v; }
inline const Quaternion* mesh_orientation(void* mesh) { return reinterpret_cast<const Quaternion*>(bytes(mesh) + 0x38); }
inline const Vector* mesh_vertices(void* mesh) { return reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(mesh_word(mesh, 0x54))); }
inline const I* mesh_faces(void* mesh) { return reinterpret_cast<const I*>(static_cast<std::uintptr_t>(mesh_word(mesh, 0x60))); }
inline const I* mesh_materials(void* mesh) { return reinterpret_cast<const I*>(static_cast<std::uintptr_t>(mesh_word(mesh, 0x64))); }
inline const Vector* mesh_normal(void* mesh, U face) {
    return reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(mesh_word(mesh, 0x68) + face * 12u));
}

// 005972B0 local area: slot Lxxx lives at bytes[0x234 - xxx] (sub esp,234h at 005972B4).
struct Frame {
    alignas(4) unsigned char b[0x234];
    unsigned char* p(U off) { return b + (0x234u - off); }
    float& f(U off) { return *reinterpret_cast<float*>(p(off)); }
    U& w(U off) { return *reinterpret_cast<U*>(p(off)); }
    I& i(U off) { return *reinterpret_cast<I*>(p(off)); }
    double d(U off) { return f(off); }
    Vector* v(U off) { return reinterpret_cast<Vector*>(p(off)); }
    void copy3(U dst, const void* src) { std::memcpy(p(dst), src, 12); }
    void copy3(U dst, U src) { std::memmove(p(dst), p(src), 12); }
};
inline U raw(const float& value) { U u; std::memcpy(&u, &value, 4); return u; }

Vector* ltw(void* mesh, Vector* out, const Vector* local) {
    return collision_mesh_local_to_world_00596aa0(mesh, nullptr, out, local);
}

// One debug quad: four LocalToWorld calls into the frame array at dst (dst, dst-C, dst-18, dst-24), then
// "if (handle != -1) AurRemoveDrawableSquare(handle); handle = AurAddDrawableSquare(corners, color)".
// Shape repeated at 0059783C..005978CB (and 15 more sites, cited at each call).
void debug_square(void* mesh, Frame& fr, U dst, const Vector* c0, const Vector* c1, const Vector* c2,
                  const Vector* c3, U handle_addr, U color) {
    ltw(mesh, fr.v(dst), c0);
    ltw(mesh, fr.v(dst - 0x0C), c1);
    ltw(mesh, fr.v(dst - 0x18), c2);
    ltw(mesh, fr.v(dst - 0x24), c3);
    U& handle = kotor_world_los::debug_squares_007a2074[(handle_addr - 0x007A2074u) / 4u];
    if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);
    handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(dst)), color);
}

bool debug_enabled() { return kotor_world_los::debug_walk_draw_0083286c == 1u; }

// Projection of (p - a_local) on the path direction n (L1E0,L1DC,L1D8), original operand order:
// ((pz-az)*nz + (py-ay)*ny) + (px-ax)*nx (00598400..0059842A, 0059860A..0059863D, 00598855.., 005988F0..).
double project(Frame& fr, double px, double py, double pz) {
    return (((pz - fr.d(0x220)) * fr.d(0x1D8)) + ((py - fr.d(0x224)) * fr.d(0x1DC))) + ((px - fr.d(0x228)) * fr.d(0x1E0));
}
}  // namespace

extern "C" {

// 00596A00..00596A94 CSWCollisionMesh::WorldToLocal(out, world), RET 8, EAX = out.
Vector* __fastcall collision_mesh_world_to_local_00596a00(void* mesh, void*, Vector* out, const Vector* world) {
    if (mesh_word(mesh, 4) != 0) {                 // 00596A06..00596A0B
        std::memcpy(out, world, 12);               // 00596A0D..00596A24 raw DWORD copy
        return out;
    }
    Vector d;                                      // [esp+4] local
    d.x = static_cast<float>(static_cast<double>(world->x) - mesh_float(mesh, 0x20));   // 00596A32..00596A40
    d.y = static_cast<float>(static_cast<double>(world->y) - mesh_float(mesh, 0x24));   // 00596A44..00596A4A
    d.z = static_cast<float>(static_cast<double>(world->z) - mesh_float(mesh, 0x28));   // 00596A4E..00596A59
    Quaternion inverse;                            // [esp+1C] local
    const Quaternion* q = quaternion_negate_w_004a9b50(&inverse, mesh_orientation(mesh)); // 00596A5D
    Vector rotated;                                // [esp+10] local
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, q, &d);                       // 00596A6B (EAX used)
    const double z = static_cast<double>(r->z) - mesh_float(mesh, 0x34);                 // 00596A70
    const double y = static_cast<double>(r->y) - mesh_float(mesh, 0x30);                 // 00596A79
    const double x = static_cast<double>(r->x) - mesh_float(mesh, 0x2C);                 // 00596A7F
    out->x = static_cast<float>(x);                // 00596A89
    out->y = static_cast<float>(y);                // 00596A8B
    out->z = static_cast<float>(z);                // 00596A8E
    return out;
}

// 00596AA0..00596B26 CSWCollisionMesh::LocalToWorld(out, local), RET 8, EAX = out.
Vector* __fastcall collision_mesh_local_to_world_00596aa0(void* mesh, void*, Vector* out, const Vector* local) {
    if (mesh_word(mesh, 4) != 0) {                 // 00596AA6..00596AAB
        std::memcpy(out, local, 12);               // 00596AAD..00596AC4
        return out;
    }
    Vector t;                                      // [esp+4] local
    t.x = static_cast<float>(mesh_float(mesh, 0x2C) + static_cast<double>(local->x));   // 00596AD2..00596AE0
    t.y = static_cast<float>(mesh_float(mesh, 0x30) + static_cast<double>(local->y));   // 00596AE4..00596AEA
    t.z = static_cast<float>(mesh_float(mesh, 0x34) + static_cast<double>(local->z));   // 00596AEE..00596AF9
    Vector rotated;                                // [esp+10] local
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, mesh_orientation(mesh), &t); // 00596AFD
    const double z = static_cast<double>(r->z) + mesh_float(mesh, 0x28);                 // 00596B02
    const double y = static_cast<double>(r->y) + mesh_float(mesh, 0x24);                 // 00596B0B
    const double x = mesh_float(mesh, 0x20) + static_cast<double>(r->x);                 // 00596B11..00596B15
    out->x = static_cast<float>(x);                // 00596B1B
    out->y = static_cast<float>(y);                // 00596B1D
    out->z = static_cast<float>(z);                // 00596B20
    return out;
}

// 00596B30..00596B6F: identity copy when [mesh+4] != 0, else operator*(orientation +38, v). RET 8, EAX = out.
Vector* __fastcall collision_mesh_rotate_00596b30(void* mesh, void*, Vector* out, const Vector* v) {
    if (mesh_word(mesh, 4) != 0) {                 // 00596B30..00596B36
        std::memcpy(out, v, 12);                   // 00596B38..00596B4F
        return out;
    }
    quaternion_rotate_004a9bb0(out, mesh_orientation(mesh), v);   // 00596B56..00596B64
    return out;                                    // 00596B6C mov eax,esi (out)
}

// 005972B0..00599EC0 CSWCollisionMesh::NoNonWalkPolys(a, b, zlow, zhigh, radius, walk_hit), RET 18h.
// Returns 1 when the swept 2-D capsule a->b (radius f5, z band [f3,f4] in local space) meets no walk-check
// face, else 0 (with CWalkHitInfo filled when walk_hit != null).
I __fastcall collision_mesh_no_nonwalk_polys_005972b0(void* mesh, void*, Vector* a, Vector* b, float f3, float f4,
    float f5, void* walk_hit) {
    Frame fr{};                                    // D2: value-initialised local area (see header comment)
    const double r = f5;                           // A5 loads (fld/fmul dword [A5])
    collision_mesh_world_to_local_00596a00(mesh, nullptr, fr.v(0x228), a);   // 005972BE..005972C6 a_l = L228
    collision_mesh_world_to_local_00596a00(mesh, nullptr, fr.v(0x204), b);   // 005972CB..005972DA b_l = L204
    const U world = mesh_word(mesh, 4);            // 005972DF
    if (world != 0) fr.w(0x198) = raw(f3);         // 005972E8..005972EF
    else fr.f(0x198) = static_cast<float>((static_cast<double>(f3) - mesh_float(mesh, 0x28)) - mesh_float(mesh, 0x34)); // 005972F8..00597305
    if (world != 0) fr.w(0x1BC) = raw(f4);         // 00597310..00597317
    else fr.f(0x1BC) = static_cast<float>((static_cast<double>(f4) - mesh_float(mesh, 0x28)) - mesh_float(mesh, 0x34)); // 00597320..0059732D
    if (fr.d(0x204) == fr.d(0x228) && fr.d(0x200) == fr.d(0x224)) return 1;  // 00597334..0059735F (test ah,44h / jp)

    // 00597362..0059737C: box[4] at L120, L114, L108, LFC zeroed (12 DWORDs, edi = 0)
    for (U k = 0; k < 4; ++k) { fr.w(0x120 - 12 * k) = 0; fr.w(0x11C - 12 * k) = 0; fr.w(0x118 - 12 * k) = 0; }
    fr.w(0x78) = 0; fr.w(0x74) = 0;                // 0059738E / 00597399
    const double dx = fr.d(0x204) - fr.d(0x228);   // 0059737E..00597389
    fr.f(0x34) = static_cast<float>(dx);           // 005973A4 fst
    fr.w(0x70) = 0;                                // 005973AB
    fr.f(0x18) = static_cast<float>(dx);           // 005973B6
    fr.w(0x10) = 0;                                // 005973BD
    const double dy = fr.d(0x200) - fr.d(0x224);   // 005973C8..005973CC
    fr.f(0x3C) = static_cast<float>(dy);           // 005973D0
    fr.f(0x14) = static_cast<float>(dy);           // 005973D7
    fr.f(0x38) = static_cast<float>(vector_magnitude_004ab100(fr.v(0x18)));   // 005973DE..005973EA
    {
        const Vector* n = normalize_004ab180(fr.v(0xE0), fr.v(0x18));        // 005973F1..005973FA
        const U nx = raw(n->x), ny = raw(n->y), nz = raw(n->z);              // 005973FF..00597404
        fr.w(0x1E0) = nx;                          // 00597407
        fr.w(0x1DC) = ny;                          // 00597411
        fr.f(0xDC) = static_cast<float>(-fr.d(0x1E0));   // 0059740B..00597415 (overwrites y of L E0 output)
        fr.w(0x1D8) = nz;                          // 00597420
    }
    {
        const double X = fr.d(0x1E0) * r;          // 0059741C..00597424
        const double Y = fr.d(0x1DC) * r;          // 0059742E..00597432
        const double Z = fr.d(0x1D8) * r;          // 00597439..0059743D
        fr.f(0x22C) = static_cast<float>(Z);       // 00597444
        fr.f(0x1EC) = static_cast<float>(-Y);      // 00597448..0059744C
        fr.f(0x1E8) = static_cast<float>(X);       // 00597450..00597452
        fr.f(0x1A8) = static_cast<float>(fr.d(0x1EC) + fr.d(0x228));   // 00597456..0059745E
        fr.w(0x120) = fr.w(0x1A8);                 // 00597465..00597470
        fr.f(0x1A4) = static_cast<float>(fr.d(0x1E8) + fr.d(0x224));   // 0059746C..0059747B
        fr.f(0x1A0) = fr.f(0x220);                 // 00597482..0059748D (fld/fstp dword)
        fr.w(0x11C) = fr.w(0x1A4);                 // 005974A1
        fr.w(0x118) = fr.w(0x1A0);                 // 005974A8
        const double xb = X + fr.d(0x204);         // 00597494..0059749D
        fr.f(0x19C) = static_cast<float>(xb);      // 005974AF fst
        fr.f(0xEC) = static_cast<float>(xb);       // 005974B6
        const double yb = Y + fr.d(0x200);         // 005974BD
        fr.f(0x1B8) = static_cast<float>(yb);      // 005974C1 fst
        const double zb = fr.d(0x22C) + fr.d(0x1FC);   // 005974C8..005974CC
        fr.f(0x188) = static_cast<float>(zb);      // 005974D0 fst
        fr.f(0x210) = static_cast<float>(fr.d(0x1EC) + fr.d(0xEC));    // 005974D7..005974E2
        fr.f(0x20C) = static_cast<float>(yb + fr.d(0x1E8));           // 005974EA..005974F0
        fr.f(0x208) = static_cast<float>(zb);      // 005974F4
    }
    fr.w(0x114) = fr.w(0x210);                     // 00597510
    fr.w(0x22C) = fr.w(0x188);                     // 0059751B..00597522
    fr.w(0x110) = fr.w(0x20C);                     // 00597526
    fr.w(0x10C) = fr.w(0x208);                     // 0059753B
    fr.f(0x1B4) = static_cast<float>(fr.d(0x19C) - fr.d(0x1EC));   // 005974F8..0059752D
    fr.w(0x108) = fr.w(0x1B4);                     // 00597546
    fr.f(0x1B0) = static_cast<float>(fr.d(0x1B8) - fr.d(0x1E8));   // 00597542..0059754D
    fr.f(0x1AC) = fr.f(0x22C);                     // 00597554..0059755F
    fr.w(0x104) = fr.w(0x1B0);                     // 00597575
    fr.w(0x100) = fr.w(0x1AC);                     // 0059757C
    fr.f(0x1B4) = static_cast<float>(fr.d(0x228) - fr.d(0x1EC));   // 00597566..00597583
    fr.w(0xFC) = fr.w(0x1B4);                      // 00597595
    fr.f(0x1B0) = static_cast<float>(fr.d(0x224) - fr.d(0x1E8));   // 00597591..005975A0
    fr.f(0x1AC) = fr.f(0x220);                     // 005975A7..005975B2
    fr.w(0xF8) = fr.w(0x1B0);                      // 005975C9
    fr.w(0xF4) = fr.w(0x1AC);                      // 005975D0
    fr.f(0x8C) = static_cast<float>(r + r);        // 005975B9..005975D7

    // 005975DE..00597611 quadrant of the path direction
    I q;
    if (!(fr.d(0x204) >= fr.d(0x228))) q = (fr.d(0x200) < fr.d(0x224)) ? 3 : 2;   // jne / jnp
    else q = (fr.d(0x200) < fr.d(0x224)) ? 0 : 1;                                  // jp
    // 00597616..005976B1: XY bounds of the (unextended) box, index = signed mod 4 of q+k (q in 0..3)
    fr.f(0xD4) = static_cast<float>(fr.d(0x120 - 12 * ((q + 3) % 4)) - k_0_01_0073dfec);   // 00597629..0059763F
    fr.f(0xD0) = static_cast<float>(fr.d(0x11C - 12 * ((q + 2) % 4)) - k_0_01_0073dfec);   // 00597650..00597666
    fr.f(0x6C) = static_cast<float>(fr.d(0x120 - 12 * ((q + 1) % 4)) + k_0_01_0073dfec);   // 0059767C..00597689
    fr.f(0x68) = static_cast<float>(fr.d(0x11C - 12 * (q % 4)) + k_0_01_0073dfec);         // 0059769A..005976B1

    // 005976B8..00597745: extend box[1] by radius along normalize(box0 - box1)
    fr.f(0x1D4) = static_cast<float>(fr.d(0x1A8) - fr.d(0x210));   // 005976B8..005976C3
    fr.f(0x1D0) = static_cast<float>(fr.d(0x1A4) - fr.d(0x20C));   // 005976C7..005976D2
    fr.f(0x1CC) = static_cast<float>(fr.d(0x1A0) - fr.d(0x208));   // 005976D6..005976E1
    {
        const Vector* u = normalize_004ab180(fr.v(0x234), fr.v(0x1D4));   // 005976E5
        const double X = r * u->x, Y = r * u->y, Z = r * u->z;          // 005976EA..00597704
        fr.f(0x22C) = static_cast<float>(Z);       // 00597707
        fr.f(0x1B4) = static_cast<float>(fr.d(0x114) + X);   // 0059770B..00597714
        fr.w(0x114) = fr.w(0x1B4);                 // 00597722
        fr.f(0x1B0) = static_cast<float>(Y + fr.d(0x110));   // 00597729..00597735
        fr.w(0x110) = fr.w(0x1B0);                 // 00597745
        fr.f(0x1AC) = static_cast<float>(fr.d(0x22C) + fr.d(0x10C));   // 0059774C..0059775C
        fr.w(0x10C) = fr.w(0x1AC);                 // 00597778
    }
    // 00597763..00597836: extend box[2] by radius along normalize(box3 - box2)
    fr.f(0x1D4) = static_cast<float>(fr.d(0xFC) - fr.d(0x108));    // 00597763..0059777F
    fr.f(0x1D0) = static_cast<float>(fr.d(0xF8) - fr.d(0x104));    // 00597783..00597791
    fr.f(0x1CC) = static_cast<float>(fr.d(0xF4) - fr.d(0x100));    // 00597795..005977A3
    {
        const Vector* u = normalize_004ab180(fr.v(0x234), fr.v(0x1D4));   // 005977AA
        const double X = r * u->x, Y = r * u->y, Z = r * u->z;          // 005977AF..005977CC
        fr.f(0x22C) = static_cast<float>(Z);       // 005977CF
        fr.f(0x1B4) = static_cast<float>(fr.d(0x108) + X);   // 005977D3..005977DC
        fr.f(0x1B0) = static_cast<float>(Y + fr.d(0x104));   // 005977E3..005977EA
        fr.w(0x104) = fr.w(0x1B0);                 // 0059780C
        fr.f(0x1AC) = static_cast<float>(fr.d(0x22C) + fr.d(0x100));   // 005977F3..0059781A
        fr.w(0x108) = fr.w(0x1B4);                 // 00597828
        fr.w(0x100) = fr.w(0x1AC);                 // 0059782F
    }

    if (debug_enabled()) {                         // 00597813 / 00597836
        debug_square(mesh, fr, 0x17C, fr.v(0x120), fr.v(0x114), fr.v(0x108), fr.v(0xFC), 0x007A20BCu, 0xFFF00FFFu); // 0059783C..005978CB
        // 005978BD..00597998: low-z copies of the four corners
        fr.w(0x1C8) = fr.w(0x120); fr.w(0x1C4) = fr.w(0x11C); fr.w(0x1C0) = fr.w(0x198);
        fr.w(0x1A8) = fr.w(0x114); fr.w(0x1A4) = fr.w(0x110); fr.w(0x1A0) = fr.w(0x198);
        fr.w(0x210) = fr.w(0x108); fr.w(0x20C) = fr.w(0x104); fr.w(0x208) = fr.w(0x198);
        fr.w(0x1EC) = fr.w(0xFC);  fr.w(0x1E8) = fr.w(0xF8);  fr.w(0x1E4) = fr.w(0x198);
        debug_square(mesh, fr, 0xC8, fr.v(0x120), fr.v(0x114), fr.v(0x1C8), fr.v(0x1A8), 0x007A20B8u, 0xFFF0FFF0u);  // 0059797B..00597A0B
        debug_square(mesh, fr, 0x17C, fr.v(0x114), fr.v(0x108), fr.v(0x1A8), fr.v(0x210), 0x007A20B4u, 0xFFF0FFF0u); // 00597A10..00597A91
        debug_square(mesh, fr, 0xC8, fr.v(0x108), fr.v(0xFC), fr.v(0x210), fr.v(0x1EC), 0x007A20B0u, 0xFFF0FFF0u);   // 00597A96..00597B14
        debug_square(mesh, fr, 0x17C, fr.v(0xFC), fr.v(0x120), fr.v(0x1EC), fr.v(0x1C8), 0x007A20ACu, 0xFFF0FFF0u);  // 00597B19..00597B94
        fr.w(0x1A0) = fr.w(0x1BC); fr.w(0x208) = fr.w(0x1BC); fr.w(0x1C0) = fr.w(0x1BC); fr.w(0x1E4) = fr.w(0x1BC); // 00597B99..00597BCB
        debug_square(mesh, fr, 0xC8, fr.v(0x120), fr.v(0x114), fr.v(0x1C8), fr.v(0x1A8), 0x007A20A8u, 0xFFF0F0F0u);  // 00597BAE..00597C50
        debug_square(mesh, fr, 0x17C, fr.v(0x114), fr.v(0x108), fr.v(0x1A8), fr.v(0x210), 0x007A20A4u, 0xFFF0F0F0u); // 00597C3E..00597CD6
        debug_square(mesh, fr, 0xC8, fr.v(0x108), fr.v(0xFC), fr.v(0x210), fr.v(0x1EC), 0x007A20A0u, 0xFFF0F0F0u);   // 00597CC4..00597D59
        debug_square(mesh, fr, 0x17C, fr.v(0xFC), fr.v(0x120), fr.v(0x1EC), fr.v(0x1C8), 0x007A209Cu, 0xFFF0F0F0u);  // 00597D47..00597DD9
        // 00597DCA..00597E91: path end segment (b_l, extended end point (L19C,L1B8,L188))
        ltw(mesh, fr.v(0xC8), fr.v(0x204));       // 00597DDE
        fr.w(0x1D4) = fr.w(0x19C); fr.w(0x1D0) = fr.w(0x1B8); fr.w(0x1CC) = fr.w(0x188);   // 00597DE3..00597E0F
        ltw(mesh, fr.v(0xBC), fr.v(0x1D4));       // 00597E13
        fr.w(0x1D4) = fr.w(0x19C); fr.w(0x1D0) = fr.w(0x1B8); fr.w(0x1CC) = fr.w(0x188);   // 00597E18..00597E41
        ltw(mesh, fr.v(0xB0), fr.v(0x1D4));       // 00597E48
        ltw(mesh, fr.v(0xA4), fr.v(0x204));       // 00597E5C
        {
            U& handle = kotor_world_los::debug_squares_007a2074[(0x007A2098u - 0x007A2074u) / 4u];
            if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);         // 00597E61..00597E71
            handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(0xC8)), 0xFFFFFFFFu); // 00597E74..00597E91
        }
        // 00597E83..00597F01: high-z copies of box[1] (L1B4) and box[2] (L148)
        fr.w(0x1B4) = fr.w(0x114); fr.w(0x1B0) = fr.w(0x110); fr.w(0x1AC) = fr.w(0x1BC);
        fr.w(0x148) = fr.w(0x108); fr.w(0x144) = fr.w(0x104); fr.w(0x140) = fr.w(0x1BC);
        debug_square(mesh, fr, 0x17C, fr.v(0x1B4), fr.v(0x1B4), fr.v(0x148), fr.v(0x148), 0x007A2094u, 0xFFFFFFFFu);  // 00597EEF..00597F77
    }

    // 00597F7C..00597F94: triangle vertex slots L17C, L170, L164 zeroed (9 DWORDs)
    for (U k = 0; k < 3; ++k) { fr.w(0x17C - 12 * k) = 0; fr.w(0x178 - 12 * k) = 0; fr.w(0x174 - 12 * k) = 0; }
    fr.w(0x1B8) = 0; fr.w(0x188) = 0;              // 00597F9C / 00597FA3 (vertex-hit flag, hit flag)
    fr.w(0x21C) = 0; fr.w(0x218) = 0; fr.w(0x214) = 0;   // 00597FAA..00597FBA hit point
    fr.w(0x1F8) = 0; fr.w(0x1F4) = 0; fr.w(0x1F0) = 0;   // 00597FC2..00597FD2
    fr.w(0x194) = 0; fr.w(0x190) = 0; fr.w(0x18C) = 0;   // 00597FDA..00597FF0
    fr.w(0x180) = k_flt_max_bits;                  // 00597FFB best projection
    fr.w(0xF0) = 0xFFFFFFFFu;                      // 00598006 material mask
    if (kotor_world_los::use_walk_check_mask_007a2070 != 0) {   // 00597F96 / 00598011
        CollisionMeshVtbl* vt = *static_cast<CollisionMeshVtbl**>(mesh);   // 00598013
        fr.w(0xF0) = vt->walk_check_material_mask_14(mesh, nullptr);        // 00598017..0059801A
    }

    fr.i(0x130) = static_cast<I>(mesh_word(mesh, 0x5C));   // 00598021 / 00598030
    if (fr.i(0x130) < static_cast<I>(mesh_word(mesh, 0x58))) {   // 00598024 / 00598037 (signed jge)
        fr.i(0x184) = fr.i(0x130) * 12;            // 0059803D..00598043
        do {
            // 00598050..005980DC: load the face's three vertices (index < 0 keeps the slot's previous value)
            {
                const I* face = reinterpret_cast<const I*>(reinterpret_cast<const unsigned char*>(mesh_faces(mesh)) + fr.i(0x184));
                for (U k = 0; k < 3; ++k) {
                    const I index = face[k];
                    if (index >= 0) fr.copy3(0x17C - 12 * k, &mesh_vertices(mesh)[index]);
                }
            }
            if (!(fr.d(0x198) <= fr.d(0x1BC))) goto next_face;                                  // 005980E3..005980F6
            if ((fr.w(0xF0) & (1u << (static_cast<U>(mesh_materials(mesh)[fr.i(0x130)]) & 31u))) == 0) goto next_face; // 005980FC..00598119
            if (!(fr.d(0x174) <= fr.d(0x1BC)) && !(fr.d(0x168) <= fr.d(0x1BC)) && !(fr.d(0x15C) <= fr.d(0x1BC))) goto next_face; // 0059811F..0059815C
            if (!(fr.d(0x174) >= fr.d(0x198)) && !(fr.d(0x168) >= fr.d(0x198)) && !(fr.d(0x15C) >= fr.d(0x198))) goto next_face; // 00598162..0059819F
            if (!(fr.d(0x17C) <= fr.d(0x6C)) && !(fr.d(0x170) <= fr.d(0x6C)) && !(fr.d(0x164) <= fr.d(0x6C))) goto next_face;    // 005981A5..005981E2
            if (!(fr.d(0x178) <= fr.d(0x68)) && !(fr.d(0x16C) <= fr.d(0x68)) && !(fr.d(0x160) <= fr.d(0x68))) goto next_face;    // 005981E8..00598225
            if (!(fr.d(0x17C) >= fr.d(0xD4)) && !(fr.d(0x170) >= fr.d(0xD4)) && !(fr.d(0x164) >= fr.d(0xD4))) goto next_face;    // 0059822B..00598268
            if (!(fr.d(0x178) >= fr.d(0xD0)) && !(fr.d(0x16C) >= fr.d(0xD0)) && !(fr.d(0x160) >= fr.d(0xD0))) goto next_face;    // 0059826E..005982AB

            // 005982B1..005984D2: classify each vertex against the swept capsule
            fr.w(0x1B4) = 1; fr.w(0x1B0) = 1; fr.w(0x1AC) = 1;    // 005982B8..005982CB edge flags
            for (I k = 0; k < 3; ++k) {
                const U vtx = 0x17C - 12 * static_cast<U>(k);     // edx = &L17C + 12k (005982D4 / 005984CC)
                fr.w(0x1F8) = fr.w(vtx);                          // 005982E2..005982E4
                fr.f(0x1A8) = static_cast<float>(fr.d(0x1F8) - fr.d(0x120));   // 005982E8..005982FD
                fr.w(0x1F4) = fr.w(vtx - 4);                      // 005982F9
                fr.w(0x1F0) = fr.w(vtx - 8);                      // 00598304
                fr.f(0x1A4) = static_cast<float>(fr.d(0x1F4) - fr.d(0x11C));   // 00598308..00598313
                const double dz = fr.d(0x1F0) - fr.d(0x118);      // 0059831A..0059831E
                fr.f(0x1A0) = static_cast<float>(dz);             // 00598325 fst
                const double along = ((dz * fr.d(0x1D8)) + (fr.d(0x1A4) * fr.d(0x1DC))) + (fr.d(0x1A8) * fr.d(0x1E0)); // 0059832C..00598348
                bool outside = false;   // every path below assigns it (C4701 only)
                bool far_test = false;
                if (!(along <= fr.d(0x38))) far_test = true;                          // 0059834A..00598356 -> 00598484
                else if (!(along >= k_zero_0073d700)) far_test = true;                // 0059835C..00598367 -> 00598486
                else {
                    const double side = ((fr.d(0x1A4) * fr.d(0xDC)) + (fr.d(0x1A8) * fr.d(0x1DC))) + (fr.d(0x1A0) * k_zero_0073d700); // 0059836D..00598395
                    if (!(side <= fr.d(0x8C))) outside = true;                        // 00598397..005983A3 -> 00598479
                    else outside = !(side >= k_zero_0073d700);                        // 005983A9..005983B4
                }
                if (far_test) {                                                       // 00598486..005984C4
                    const double X = fr.d(0x1F8) - fr.d(0x204);
                    const double Y = fr.d(0x1F4) - fr.d(0x200);
                    const double Z = fr.d(0x1F0) - fr.d(0x1FC);
                    const double d2 = ((Z * Z) + (Y * Y)) + (X * X);
                    outside = !(r * r >= d2);                                         // fcompp; test ah,1; jne
                }
                if (outside) { fr.i(0x148 - 4 * static_cast<U>(k)) = 1; continue; }  // 0059847B (ebx = 1)
                fr.i(0x148 - 4 * static_cast<U>(k)) = 0;                              // 005983BE
                if (!(fr.d(0x1F0) >= fr.d(0x198))) continue;                          // 005983BA..005983D5
                if (!(fr.d(0x1F0) <= fr.d(0x1BC))) continue;                          // 005983DB..005983EB
                if (walk_hit == nullptr) return 0;                                    // 005983F1..005983FA -> 0059941B
                const double t = project(fr, fr.d(0x1F8), fr.d(0x1F4), fr.d(0x1F0));  // 00598400..0059842A
                if (t < fr.d(0x180)) {                                                // 0059842C..00598438
                    fr.f(0x180) = static_cast<float>(t);                              // 00598440
                    fr.w(0x21C) = fr.w(vtx); fr.w(0x218) = fr.w(vtx - 4); fr.w(0x214) = fr.w(vtx - 8);   // 00598447..00598465
                    fr.w(0x188) = 1; fr.w(0x1B8) = 1;                                 // 00598453 / 0059845A
                    fr.i(0x128) = fr.i(0x130);                                        // 00598469 (ebp = face)
                    fr.i(0x12C) = k;                                                  // 00598470
                }
            }
            // 005984D8..0059851C: an edge is skipped only when both of its vertices are inside
            {
                const I o0 = fr.i(0x148), o1 = fr.i(0x144);
                if (o0 == 0 && o1 == 0) fr.w(0x1B4) = 0;                              // 005984F0
                if (o1 == 0 && fr.i(0x140) == 0) fr.w(0x1B0) = 0;                     // 00598506
                if (fr.i(0x140) == 0 && o0 == 0) fr.w(0x1AC) = 0;                     // 0059851C
            }
            // 00598523..005989B0: edges
            for (I e = 0; e < 3;) {
                const U cur = 0x17C - 12 * static_cast<U>(e);                         // L124 pointer (0059852A / 005989A3)
                fr.w(0x1F8) = fr.w(cur); fr.w(0x1F0) = fr.w(cur - 8);                 // 00598538..00598544
                fr.i(0x7C) = e + 1;                                                   // 00598548..0059854B
                fr.w(0x1F4) = fr.w(cur - 4);                                          // 00598553
                {
                    const U nxt = 0x17C - 12 * static_cast<U>((e + 1) % 3);           // 00598552..00598561 (cdq; idiv 3)
                    fr.w(0x18C) = fr.w(nxt - 8); fr.w(0x194) = fr.w(nxt); fr.w(0x190) = fr.w(nxt - 4);   // 00598568..00598586
                }
                if (fr.i(0x1B4 - 4 * static_cast<U>(e)) == 1) {                       // 00598577 / 0059858D
                    // 00598593..0059873B: box edges (k-1, k mod 4), k = 1..4, edge 1->2 skipped
                    fr.i(0x19C) = 1;                                                  // 00598593
                    U box_a = 0x120;                                                  // ebp = &L120 (0059859E)
                    for (;;) {
                        const I k = fr.i(0x19C);                                      // 005985A5
                        if (k - 1 != 1) {                                             // 005985AC..005985B0
                            const U box_b = 0x120 - 12 * static_cast<U>(k % 4);       // 005985B6..005985D4
                            const I hit = intersect_line_segments_2d_004ac340(fr.v(0x1F8), fr.v(0x194), fr.v(box_a),
                                fr.v(box_b), fr.v(0x78));                             // 005985CC..005985EF
                            if (hit == 1) {                                           // 005985F2
                                if (walk_hit == nullptr) return 0;                    // 005985FB..00598604
                                const double t = project(fr, fr.d(0x78), fr.d(0x74), fr.d(0x70));   // 0059860A..0059863D
                                fr.f(0x14C) = static_cast<float>(t);                  // 0059863F fst
                                if (t < fr.d(0x180)) {                                // 00598646..00598652
                                    if (debug_enabled())                              // 00598658..005986C6
                                        debug_square(mesh, fr, 0xC8, fr.v(box_a), fr.v(box_a), fr.v(box_b), fr.v(box_b),
                                                     0x007A2090u, 0xFFFFFFFFu);
                                    fr.w(0x21C) = fr.w(0x78); fr.w(0x218) = fr.w(0x74);   // 005986CB..005986EB
                                    fr.w(0x188) = 1; fr.w(0x1B8) = 0;                 // 005986F6 / 00598701
                                    fr.w(0x214) = fr.w(0x70);                         // 0059870C
                                    fr.i(0x128) = fr.i(0x130);                        // 00598710
                                    fr.i(0x12C) = e;                                  // 00598717
                                    fr.w(0x180) = fr.w(0x14C);                        // 0059871E
                                }
                            }
                        }
                        box_a -= 12;                                                  // 0059872C add ebp,0Ch
                        fr.i(0x19C) = k + 1;                                          // 0059872F..00598730
                        if (!(k < 4)) break;                                          // 00598737..0059873B
                    }
                    // 00598741..0059883C: capsule end-circle test against the edge
                    const float radius = static_cast<float>(r + k_0_001_0073d708);    // 00598745..00598753, fstp [esp] 005987E2
                    fr.w(0x54) = fr.w(0x1F8);                                         // 00598761
                    fr.w(0xC) = fr.w(0x194);                                          // 0059876F
                    fr.w(0x50) = fr.w(0x1F4);                                         // 0059877C
                    fr.w(0x1D4) = fr.w(0x204);                                        // 0059878A
                    fr.w(0x4C) = fr.w(0x1F0);                                         // 0059878E
                    fr.w(0x8) = fr.w(0x190);                                          // 005987A1
                    fr.w(0x4) = fr.w(0x18C);                                          // 005987AC
                    fr.w(0x1D0) = fr.w(0x200);                                        // 005987B7
                    fr.w(0x1CC) = fr.w(0x1FC);                                        // 005987BB
                    fr.w(0x210) = 0; fr.w(0x20C) = 0; fr.w(0x208) = 0;                // 005987E6..005987F6
                    fr.w(0x1EC) = 0; fr.w(0x1E8) = 0; fr.w(0x1E4) = 0;                // 005987FE..0059880E
                    fr.w(0x4C) = 0; fr.w(0x4) = 0; fr.w(0x1CC) = 0;                   // 00598816..0059882C
                    // flag = setne al (walk_hit != 0) at 0059879E; EAX bits 8..31 hold L204's upper bits (fork GEOM:
                    // 004AAB50 does not read the flag).
                    const I roots = intersect_circle_2d_004aab50(fr.v(0x1D4), radius, fr.v(0x54), fr.v(0xC),
                        fr.v(0x210), fr.v(0x1EC), walk_hit != nullptr);               // 00598837
                    if (walk_hit == nullptr) {                                        // 0059883F..00598841
                        if (roots > 0) return 0;                                      // 00598989..0059898B -> 0059941B
                    } else if (roots == 1 || roots == 2) {                            // 00598847..0059884F
                        const U candidates[2] = {0x1EC, 0x210};                       // roots==2: L1EC (00598855) then L210 (005988F0)
                        for (U c = (roots == 2) ? 0u : 1u; c < 2; ++c) {
                            const U pt = candidates[c];
                            const double t = project(fr, fr.d(pt), fr.d(pt - 4), fr.d(pt - 8));   // 00598855..0059887F / 005988F0..0059891A
                            if (!(t >= k_zero_0073d700)) continue;                    // 00598881..0059888C / 0059891C..00598927
                            if (!(t < fr.d(0x180))) continue;                         // 0059888E..0059889A / 00598929..00598935
                            fr.f(0x180) = static_cast<float>(t);                      // 005988A0 / 0059893B
                            fr.w(0x21C) = fr.w(pt);                                   // 005988AF / 0059894A
                            fr.i(0x128) = fr.i(0x130);                                // 005988BA / 00598959
                            fr.w(0x214) = fr.w(pt - 8);                               // 005988C1 / 00598962
                            fr.w(0x218) = fr.w(pt - 4);                               // 005988C7 / 00598955
                            fr.w(0x188) = 1; fr.w(0x1B8) = 0;                         // 005988D3..005988DE / 0059896E..00598979
                            fr.i(0x12C) = e % 3;                                      // 005988C5..005988E5 / 00598960..00598980
                        }
                    }
                }
                e = fr.i(0x7C);                                                       // 0059899C
            }

            // 005989B6..00598C65: is a_l inside the triangle (XY, side tests with cross(up, v_i - a_l))?
            {
                fr.f(0x24) = static_cast<float>(fr.d(0x170) - fr.d(0x17C));          // 005989B6..005989D8
                fr.f(0x20) = static_cast<float>(fr.d(0x16C) - fr.d(0x178));          // 005989E0..005989EE
                fr.f(0x1C) = static_cast<float>(fr.d(0x168) - fr.d(0x174));          // 005989F5..00598A03
                fr.f(0x98) = static_cast<float>(fr.d(0x17C) - fr.d(0x228));          // 00598A0A..00598A15
                fr.f(0x94) = static_cast<float>(fr.d(0x178) - fr.d(0x224));          // 00598A1C..00598A27
                fr.f(0x90) = static_cast<float>(fr.d(0x174) - fr.d(0x220));          // 00598A2E..00598A39
                const Vector* c = vector_cross_004ab210(fr.v(0x48), &kotor_world_los::up_007a19b0, fr.v(0x98));   // 00598A40
                const U cx = raw(c->x), cy = raw(c->y), cz = raw(c->z);              // 00598A4C..00598A51
                fr.w(0x1C0) = cz; fr.w(0x1C4) = cy; fr.w(0x1C8) = cx;                // 00598A54..00598A77
                const double dot = ((fr.d(0x1C) * fr.d(0x1C0)) + (fr.d(0x20) * fr.d(0x1C4))) + (fr.d(0x1C8) * fr.d(0x24)); // 00598A45..00598A8E
                if (!(dot >= k_zero_0073d700)) goto next_face;                       // 00598A90..00598A9B
            }
            {
                fr.f(0x88) = static_cast<float>(fr.d(0x164) - fr.d(0x170));          // 00598AA1..00598AC3
                fr.f(0x84) = static_cast<float>(fr.d(0x160) - fr.d(0x16C));          // 00598ACB..00598AD9
                fr.f(0x80) = static_cast<float>(fr.d(0x15C) - fr.d(0x168));          // 00598AE0..00598AEE
                fr.f(0xEC) = static_cast<float>(fr.d(0x170) - fr.d(0x228));          // 00598AF5..00598B00
                fr.f(0xE8) = static_cast<float>(fr.d(0x16C) - fr.d(0x224));          // 00598B07..00598B12
                fr.f(0xE4) = static_cast<float>(fr.d(0x168) - fr.d(0x220));          // 00598B19..00598B24
                const Vector* c = vector_cross_004ab210(fr.v(0x30), &kotor_world_los::up_007a19b0, fr.v(0xEC));   // 00598B2B
                const U cx = raw(c->x), cy = raw(c->y), cz = raw(c->z);              // 00598B37..00598B3C
                fr.w(0x1C0) = cz; fr.w(0x1C4) = cy; fr.w(0x1C8) = cx;                // 00598B3F..00598B62
                const double dot = ((fr.d(0x80) * fr.d(0x1C0)) + (fr.d(0x84) * fr.d(0x1C4))) + (fr.d(0x1C8) * fr.d(0x88)); // 00598B30..00598B79
                if (!(dot >= k_zero_0073d700)) goto next_face;                       // 00598B7B..00598B86
            }
            {
                fr.f(0x13C) = static_cast<float>(fr.d(0x17C) - fr.d(0x164));         // 00598B8C..00598BAB
                fr.f(0x138) = static_cast<float>(fr.d(0x178) - fr.d(0x160));         // 00598BB3..00598BC1
                fr.f(0x134) = static_cast<float>(fr.d(0x174) - fr.d(0x15C));         // 00598BC8..00598BD6
                fr.f(0x234) = static_cast<float>(fr.d(0x164) - fr.d(0x228));         // 00598BDD..00598BE8
                fr.f(0x230) = static_cast<float>(fr.d(0x160) - fr.d(0x224));         // 00598BEC..00598BF7
                fr.f(0x22C) = static_cast<float>(fr.d(0x15C) - fr.d(0x220));         // 00598BFB..00598C06
                const Vector* c = vector_cross_004ab210(fr.v(0x60), &kotor_world_los::up_007a19b0, fr.v(0x234));  // 00598C0A
                const U cx = raw(c->x), cy = raw(c->y), cz = raw(c->z);              // 00598C16..00598C1B
                fr.w(0x1C0) = cz; fr.w(0x1C4) = cy; fr.w(0x1C8) = cx;                // 00598C1E..00598C41
                const double dot = ((fr.d(0x134) * fr.d(0x1C0)) + (fr.d(0x138) * fr.d(0x1C4))) + (fr.d(0x1C8) * fr.d(0x13C)); // 00598C0F..00598C58
                if (dot >= k_zero_0073d700) {                                         // 00598C5A..00598C65 -> 00599403
                    if (walk_hit != nullptr) {                                        // 00599403..0059940C
                        kotor_world_los::view<I>(walk_hit, 0) = 2;                    // 0059940E
                        kotor_world_los::view<I>(walk_hit, 0x44) = 3;                 // 00599414
                    }
                    return 0;                                                         // 0059941B..00599427
                }
            }
        next_face:
            fr.i(0x130) = fr.i(0x130) + 1;                                            // 00598C6B..00598C82
            fr.i(0x184) = fr.i(0x184) + 12;                                           // 00598C89
        } while (fr.i(0x130) < static_cast<I>(mesh_word(mesh, 0x58)));               // 00598C79..00598C90 ([esi+58] re-read)
    }

    // 00598C96..00598CBB
    if (walk_hit == nullptr) return 1;                                                // 00598C9D..00598CAD -> 00599EB1
    const I hit_face = fr.i(0x128);                                                   // 00598C9F ebx
    const I hit_index = fr.i(0x12C);                                                  // 00598CA6 edi
    if (fr.i(0x188) != 1) return 1;                                                   // 00598CB3..00598CBB

    // 00598CC1..00598D59: reload the hit face's vertices (index < 0 keeps the slot)
    {
        const I* face = mesh_faces(mesh) + 3 * hit_face;
        for (U k = 0; k < 3; ++k) {
            const I index = face[k];
            if (index >= 0) fr.copy3(0x17C - 12 * k, &mesh_vertices(mesh)[index]);
        }
    }
    {
        const U v0 = 0x17C - 12 * static_cast<U>(hit_index);                         // 00598D60..00598D63
        fr.w(0x1F4) = fr.w(v0 - 4); fr.w(0x1F8) = fr.w(v0); fr.w(0x1F0) = fr.w(v0 - 8);   // 00598D6A..00598D7A
        const U v1 = 0x17C - 12 * static_cast<U>((hit_index + 1) % 3);               // 00598D7E..00598D8C
        fr.w(0x18C) = fr.w(v1 - 8); fr.w(0x190) = fr.w(v1 - 4); fr.w(0x194) = fr.w(v1);   // 00598D93..00598DAD
        const U v2 = 0x17C - 12 * static_cast<U>((hit_index + 2) % 3);               // 00598DA9..00598DBE
        fr.w(0x208) = fr.w(v2 - 8); fr.w(0x210) = fr.w(v2); fr.w(0x20C) = fr.w(v2 - 4);   // 00598DC5..00598DDC
    }

    if (debug_enabled()) {                                                            // 00598DD1..00598DE0
        debug_square(mesh, fr, 0xC8, fr.v(0x1F8), fr.v(0x194), fr.v(0x210), fr.v(0x1F8), 0x007A208Cu, 0xFFFFFFFFu);   // 00598DE6..00598E6D
        debug_square(mesh, fr, 0x17C, fr.v(0x1F8), fr.v(0x1F8), fr.v(0x194), fr.v(0x194), 0x007A2088u, 0xFFFF0F0Fu);  // 00598E5E..00598EF0
        ltw(mesh, fr.v(0xC8), fr.v(0x21C));                                           // 00598EE1..00598EF5
        ltw(mesh, fr.v(0xBC), fr.v(0x21C));                                           // 00598EFA..00598F09
        for (U pass = 0; pass < 2; ++pass) {                                          // 00598F0E..00598F6D, 00598F72..00598FCB
            const Vector* nrm = mesh_normal(mesh, static_cast<U>(hit_face));          // [esi+68] + 12*face (re-read)
            const double X = static_cast<double>(nrm->x) * k_half_0073e9ac;
            const double Y = static_cast<double>(nrm->y) * k_half_0073e9ac;
            const double Z = static_cast<double>(nrm->z) * k_half_0073e9ac;
            fr.f(0xD8) = static_cast<float>(Z);
            fr.f(0x234) = static_cast<float>(fr.d(0x21C) + X);
            fr.f(0x230) = static_cast<float>(Y + fr.d(0x218));
            fr.f(0x22C) = static_cast<float>(fr.d(0xD8) + fr.d(0x214));
            ltw(mesh, fr.v(pass == 0 ? 0xB0 : 0xA4), fr.v(0x234));
        }
        {
            U& handle = kotor_world_los::debug_squares_007a2074[(0x007A2084u - 0x007A2074u) / 4u];
            if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);  // 00598FD0..00598FE0
            handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(0xC8)), 0xFF00FF00u); // 00598FE3..00599028
        }
        // 00598FF5..00599073: triangle centroid * 0.33333
        {
            const double sx = fr.d(0x194) + fr.d(0x1F8);
            const double sy = fr.d(0x190) + fr.d(0x1F4);
            fr.f(0xD8) = static_cast<float>(fr.d(0x18C) + fr.d(0x1F0));
            fr.f(0x234) = static_cast<float>(sx + fr.d(0x210));
            const double Y = fr.d(0x20C) + sy;
            const double Z = fr.d(0x208) + fr.d(0xD8);
            fr.f(0x1EC) = static_cast<float>(fr.d(0x234) * k_third_0074a094);
            fr.f(0x1E8) = static_cast<float>(Y * k_third_0074a094);
            fr.f(0x1E4) = static_cast<float>(Z * k_third_0074a094);
        }
        ltw(mesh, fr.v(0x17C), fr.v(0x1EC));                                          // 00599077
        ltw(mesh, fr.v(0x170), fr.v(0x1EC));                                          // 0059908B
        for (U pass = 0; pass < 2; ++pass) {                                          // 00599090..005990E7, 005990EC..00599143
            const Vector* nrm = mesh_normal(mesh, static_cast<U>(hit_face));
            const double X = static_cast<double>(nrm->x) * k_half_0073e9ac;
            const double Y = static_cast<double>(nrm->y) * k_half_0073e9ac;
            const double Z = static_cast<double>(nrm->z) * k_half_0073e9ac;
            fr.f(0xD8) = static_cast<float>(Z);
            fr.f(0x234) = static_cast<float>(X + fr.d(0x1EC));
            fr.f(0x230) = static_cast<float>(Y + fr.d(0x1E8));
            fr.f(0x22C) = static_cast<float>(fr.d(0xD8) + fr.d(0x1E4));
            ltw(mesh, fr.v(pass == 0 ? 0x164 : 0x158), fr.v(0x234));
        }
        {
            U& handle = kotor_world_los::debug_squares_007a2074[(0x007A2080u - 0x007A2074u) / 4u];
            if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);  // 00599148..00599158
            handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(0x17C)), 0xFF00FF00u); // 0059915B..00599170
        }
    }

    // 00599175..005991B4: vertex-style slide when the hit vertex differs from the next vertex in z or the hit was a vertex
    bool vertex_slide;
    if (!(fr.d(0x1F8) == fr.d(0x194)) || !(fr.d(0x1F4) == fr.d(0x190))) vertex_slide = fr.i(0x1B8) != 0;   // -> 005991AB
    else if (!(fr.d(0x1F0) == fr.d(0x18C))) vertex_slide = true;                                         // 005991A9 -> 005991BA
    else vertex_slide = fr.i(0x1B8) != 0;                                                                 // 005991AB..005991B4

    if (vertex_slide) {                                                               // 005991BA..00599596
        fr.f(0x13C) = static_cast<float>(fr.d(0x21C) - fr.d(0x228));                // 005991BA..005991D3
        fr.f(0x138) = static_cast<float>(fr.d(0x218) - fr.d(0x224));                // 005991DB..005991E3
        fr.f(0x134) = static_cast<float>(fr.d(0x214) - fr.d(0x220));                // 005991EA..005991F2
        fr.f(0x234) = static_cast<float>(fr.d(0x210) - fr.d(0x1F8));                // 005991F9..00599201
        fr.f(0x230) = static_cast<float>(fr.d(0x20C) - fr.d(0x1F4));                // 00599205..0059920D
        fr.f(0x22C) = static_cast<float>(fr.d(0x208) - fr.d(0x1F0));                // 00599211..00599219
        fr.f(0xEC) = static_cast<float>(fr.d(0x1F8) - fr.d(0x194));                 // 0059921D..00599228
        fr.f(0xE8) = static_cast<float>(fr.d(0x1F4) - fr.d(0x190));                 // 0059922F..0059923A
        fr.f(0xE4) = static_cast<float>(fr.d(0x1F0) - fr.d(0x18C));                 // 00599241..0059924C
        const Vector* c1 = vector_cross_004ab210(fr.v(0x60), &kotor_world_los::up_007a19b0, fr.v(0x234));   // 00599253
        const Vector* p1 = normalize_004ab180(fr.v(0x30), c1);                       // 00599258..00599261 (edi)
        const Vector* c2 = vector_cross_004ab210(fr.v(0x48), &kotor_world_los::up_007a19b0, fr.v(0xEC));    // 00599266..0059927D
        const Vector* p2 = normalize_004ab180(fr.v(0xE0), c2);                       // 00599282..0059928B
        {
            const double z = static_cast<double>(p1->z) + p2->z;                     // 00599290..00599293
            const double y = static_cast<double>(p1->y) + p2->y;                     // 0059929D..005992A1
            const double x = static_cast<double>(p1->x) + p2->x;                     // 005992A8..005992AB
            fr.f(0x98) = static_cast<float>(x); fr.f(0x94) = static_cast<float>(y); fr.f(0x90) = static_cast<float>(z);  // 005992AD..005992BB
        }
        normalize_004ab180(fr.v(0x204), fr.v(0x98));                                 // 00599296..005992C2 (b_l := slide dir)
        {
            const double dd = ((((fr.d(0x134) * fr.d(0x1D8)) + (fr.d(0x138) * fr.d(0x1DC))) + (fr.d(0x13C) * fr.d(0x1E0)))
                               + k_0_001_0073d708) - r;                              // 005992C7..005992F5
            fr.f(0x88) = static_cast<float>(fr.d(0x1E0) * dd);                       // 005992FC..0059930D
            const Vector* nrm = mesh_normal(mesh, static_cast<U>(hit_face));         // 00599300..00599308
            const U nx = raw(nrm->x), ny = raw(nrm->y), nz = raw(nrm->z);            // 0059930B..0059931B
            fr.w(0x210) = nx; fr.w(0x208) = nz; fr.w(0x20C) = ny;                    // 00599320..00599328
            fr.f(0x84) = static_cast<float>(fr.d(0x1DC) * dd);                       // 00599317..0059932C
            fr.f(0x80) = static_cast<float>(fr.d(0x1D8) * dd);                       // 00599333..00599339
        }
        {
            const Vector& up = kotor_world_los::up_007a19b0;
            const double updot = ((static_cast<double>(up.x) * fr.d(0x210)) + (static_cast<double>(up.z) * fr.d(0x208)))
                                 + (static_cast<double>(up.y) * fr.d(0x20C));       // 00599342..00599362
            if (updot <= k_zero_0073d700) {                                          // 00599364..0059936F
                const double bdot = ((fr.d(0x204) * fr.d(0x210)) + (fr.d(0x1FC) * fr.d(0x208))) + (fr.d(0x200) * fr.d(0x20C)); // 00599371..0059938B
                if (bdot < k_zero_0073d700) {                                        // 0059938D..00599398
                    fr.f(0x13C) = static_cast<float>(-fr.d(0x204)); fr.w(0x204) = fr.w(0x13C);   // 0059939A..005993B2
                    fr.f(0x138) = static_cast<float>(-fr.d(0x200)); fr.w(0x200) = fr.w(0x138);   // 005993AE..005993CC
                    fr.f(0x134) = static_cast<float>(-fr.d(0x1FC)); fr.w(0x1FC) = fr.w(0x134);   // 005993BF..005993DE
                }
            }
        }
        if (mesh_word(mesh, 4) != 0) fr.copy3(0x1D4, 0x204);                         // 005993E2..005993FD
        else quaternion_rotate_004a9bb0(fr.v(0x1D4), mesh_orientation(mesh), fr.v(0x204));   // 0059942A..00599438
        fr.f(0x234) = static_cast<float>(fr.d(0x88) + fr.d(0x228));                  // 00599440..00599456
        fr.f(0x230) = static_cast<float>(fr.d(0x84) + fr.d(0x224));                  // 0059945B..00599470
        fr.f(0x22C) = static_cast<float>(fr.d(0x80) + fr.d(0x220));                  // 00599474..0059947F
        Vector* w5 = ltw(mesh, fr.v(0x60), fr.v(0x21C));                             // 00599483
        Vector* w4 = ltw(mesh, fr.v(0x30), fr.v(0x21C));                             // 00599498
        Vector* w2 = ltw(mesh, fr.v(0x48), fr.v(0x234));                             // 005994B5
        walk_hit_info_initialize_004bb570(walk_hit, nullptr, 1, w2, fr.v(0x1D4), w4, w5, 2, fr.v(0x210)); // 00599440..005994BF
        if (debug_enabled()) {                                                        // 005994C4..00599591
            ltw(mesh, fr.v(0xC8), fr.v(0x21C));
            ltw(mesh, fr.v(0xBC), fr.v(0x21C));
            for (U pass = 0; pass < 2; ++pass) {                                      // 005994F9..00599564
                fr.f(0x234) = static_cast<float>(fr.d(0x21C) + fr.d(0x204));
                fr.f(0x230) = static_cast<float>(fr.d(0x200) + fr.d(0x218));
                fr.f(0x22C) = static_cast<float>(fr.d(0x1FC) + fr.d(0x214));
                ltw(mesh, fr.v(pass == 0 ? 0xB0 : 0xA4), fr.v(0x234));
            }
            U& handle = kotor_world_los::debug_squares_007a2074[(0x007A207Cu - 0x007A2074u) / 4u];
            if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);
            handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(0xC8)), 0xFFF00FFFu);
        }
        return 0;                                                                     // 00599596..005995A2
    }

    // 005995A5..00599EAE: edge slide
    {
        const double ex = fr.d(0x1F8) - fr.d(0x194);                                  // 005995B8..005995C0
        fr.w(0x210) = fr.w(0x228);                                                    // 005995AC / 005995C7
        fr.w(0x20C) = fr.w(0x224);                                                    // 005995B4 / 005995CF
        fr.f(0x234) = static_cast<float>(ex);                                         // 005995D3
        fr.w(0x208) = 0;                                                              // 005995E2
        fr.w(0x50) = fr.w(0x200);                                                     // 005995BC / 005995EA
        fr.f(0x230) = static_cast<float>(fr.d(0x1F4) - fr.d(0x190));                 // 005995B0 / 005995DF..005995F1
        fr.w(0x22C) = 0;                                                              // 005995F5
    }
    normalize_004ab180(fr.v(0x1C8), fr.v(0x234));                                    // 005995D7..005995FF (e at L1C8)
    {
        const double neg_ey = -fr.d(0x1C4);                                           // 00599604..00599612
        fr.w(0x1DC) = fr.w(0x1C8);                                                    // 0059960B / 00599614
        fr.f(0x1E0) = static_cast<float>(neg_ey);                                     // 00599618
        fr.w(0x1D8) = 0;                                                              // 0059961C
    }
    fr.f(0xF0) = static_cast<float>(fr.d(0x1FC) * k_zero_0073d700);                  // 00599624..0059962E
    fr.f(0x184) = static_cast<float>(fr.d(0x220) * k_zero_0073d700);                 // 00599635..0059963F
    {
        const double num = (((fr.d(0x1C0) * fr.d(0x1FC)) + (fr.d(0x1C4) * fr.d(0x200))) + (fr.d(0x1C8) * fr.d(0x204)))
                         - (((fr.d(0x1C0) * fr.d(0x220)) + (fr.d(0x1C4) * fr.d(0x224))) + (fr.d(0x1C8) * fr.d(0x228)));   // 00599646..00599690
        const double den = (((fr.d(0x1DC) * fr.d(0x200)) + (fr.d(0x1E0) * fr.d(0x204))) + fr.d(0xF0))
                         - (((fr.d(0x1DC) * fr.d(0x224)) + (fr.d(0x1E0) * fr.d(0x228))) + fr.d(0x184));  // 00599692..005996C4
        const double qr = (num / den) * r;                                            // 005996C6..005996C8
        fr.f(0x124) = static_cast<float>(r * r);                                      // 005996CF..005996DD
        fr.f(0x14C) = static_cast<float>(std::sqrt((qr * qr) + fr.d(0x124)) + k_0_03_00747678);   // 005996E4..00599706
    }
    fr.w(0xD4) = fr.w(0x1F8);                                                         // 005996F7 / 0059970D
    fr.w(0x1D0) = fr.w(0x190);                                                        // 0059971B / 00599726
    fr.w(0xD0) = fr.w(0x1F4);                                                         // 005996FB / 0059972A
    fr.f(0x234) = static_cast<float>(fr.d(0x194) - fr.d(0x1F8));                     // 00599714..00599735
    fr.w(0x1D4) = fr.w(0x194);                                                        // 005996FF / 00599739
    fr.f(0x230) = static_cast<float>(fr.d(0x1D0) - fr.d(0xD0));                      // 0059973D..00599758
    fr.f(0xEC) = static_cast<float>(fr.d(0x204) - fr.d(0x210));                      // 0059975C..00599778
    fr.w(0xCC) = 0;                                                                   // 00599765
    fr.w(0x22C) = 0;                                                                  // 00599770
    fr.f(0xE8) = static_cast<float>(fr.d(0x50) - fr.d(0x20C));                       // 0059977F..0059978A
    fr.f(0xE4) = static_cast<float>(-fr.d(0x208));                                    // 00599791..00599797 (-0.0f)
    vector_cross_004ab210(fr.v(0x98), fr.v(0xEC), fr.v(0x234));                       // 00599741..0059979E
    if (fr.d(0x98) == k_zero_0073d700 && fr.d(0x94) == k_zero_0073d700 && fr.d(0x90) == k_zero_0073d700) {  // 005997A3..005997E8
        // 005997EE..00599976: path parallel to the edge
        fr.w(0x21C) = fr.w(0x228); fr.w(0x218) = fr.w(0x224); fr.w(0x214) = fr.w(0x220);   // 005997EE..00599812
        const Vector* nrm = mesh_normal(mesh, static_cast<U>(hit_face));             // 005997FE..0059980E
        if (mesh_word(mesh, 4) != 0) fr.copy3(0x1D4, nrm);                           // 0059980B..00599828
        else quaternion_rotate_004a9bb0(fr.v(0x1D4), mesh_orientation(mesh), nrm);   // 0059982E..00599838
        const Vector* nrm7 = mesh_normal(mesh, static_cast<U>(hit_face));            // 00599840..00599845
        Vector* w5 = ltw(mesh, fr.v(0x60), fr.v(0x228));                             // 00599857
        Vector* w4 = ltw(mesh, fr.v(0x30), fr.v(0x228));                             // 0059986C
        Vector* w2 = ltw(mesh, fr.v(0x48), fr.v(0x228));                             // 00599889
        walk_hit_info_initialize_004bb570(walk_hit, nullptr, 1, w2, fr.v(0x1D4), w4, w5, 2, nrm7);   // 00599893
        if (debug_enabled()) {                                                        // 00599898..00599965
            ltw(mesh, fr.v(0xC8), fr.v(0x21C));
            ltw(mesh, fr.v(0xBC), fr.v(0x21C));
            for (U pass = 0; pass < 2; ++pass) {
                fr.f(0x234) = static_cast<float>(fr.d(0x21C) + fr.d(0x1E0));
                fr.f(0x230) = static_cast<float>(fr.d(0x1DC) + fr.d(0x218));
                fr.f(0x22C) = static_cast<float>(fr.d(0x1D8) + fr.d(0x214));
                ltw(mesh, fr.v(pass == 0 ? 0xB0 : 0xA4), fr.v(0x234));
            }
            U& handle = kotor_world_los::debug_squares_007a2074[(0x007A2078u - 0x007A2074u) / 4u];
            if (handle != 0xFFFFFFFFu) aur_remove_drawable_square_0044f050(handle);
            handle = aur_add_drawable_square_0044efb0(reinterpret_cast<const float*>(fr.v(0xC8)), 0xFFF00FFFu);
        }
        return 0;                                                                     // 0059996A..00599976
    }
    // 00599979..005999FB: closest point on the edge line to the path line (result ST0 discarded)
    fr.f(0x234) = static_cast<float>(fr.d(0x1D4) - fr.d(0xD4));                      // 00599979..0059998F
    fr.f(0x230) = static_cast<float>(fr.d(0x1D0) - fr.d(0xD0));                      // 00599994..005999B2
    fr.f(0x22C) = static_cast<float>(-fr.d(0xCC));                                    // 005999B6..005999C4
    fr.f(0xEC) = static_cast<float>(fr.d(0x204) - fr.d(0x210));                      // 005999C8..005999D0
    fr.f(0xE8) = static_cast<float>(fr.d(0x50) - fr.d(0x20C));                       // 005999D7..005999E2
    fr.f(0xE4) = static_cast<float>(-fr.d(0x208));                                    // 005999E9..005999EF
    static_cast<void>(distance_between_lines_004abf00(fr.v(0x210), fr.v(0xEC), fr.v(0xD4), fr.v(0x234),
                                                      fr.v(0x21C), nullptr));         // 0059997D..005999FB (arg6 push 0)
    if (debug_enabled()) {                                                            // 005999FD..00599A08
        fr.f(0x1D4) = static_cast<float>(fr.d(0x21C) + fr.d(0x1E0));                 // 00599A0E..00599A23
        fr.f(0x1D0) = static_cast<float>(fr.d(0x1DC) + fr.d(0x218));                 // 00599A29..00599A31
        fr.f(0x1CC) = static_cast<float>(fr.d(0x1D8) + fr.d(0x214));                 // 00599A35..00599A3D
        debug_square(mesh, fr, 0xC8, fr.v(0x21C), fr.v(0x21C), fr.v(0x1D4), fr.v(0x1D4), 0x007A2074u, 0xFF55FF55u);  // 00599A44..00599AAD
    }
    {
        const double ax_bx = fr.d(0x228) - fr.d(0x204);                               // 00599AB2..00599ABA
        fr.f(0x7C) = static_cast<float>(ax_bx);                                       // 00599AC7 fst
        fr.f(0x234) = static_cast<float>(ax_bx);                                      // 00599ACE
        const double ay_by = fr.d(0x224) - fr.d(0x200);                               // 00599AD2..00599AD6
        fr.f(0x8C) = static_cast<float>(ay_by);                                       // 00599ADA
        fr.f(0x230) = static_cast<float>(ay_by);                                      // 00599AE1
        const double az_bz = fr.d(0x220) - fr.d(0x1FC);                               // 00599AE5..00599AE9
        fr.f(0x19C) = static_cast<float>(az_bz);                                      // 00599AED
        fr.f(0x22C) = static_cast<float>(az_bz);                                      // 00599AF4
    }
    {
        const Vector* u = normalize_004ab180(fr.v(0x60), fr.v(0x234));                // 00599AB6..00599AF8
        const double X = fr.d(0x14C) * u->x;                                          // 00599AFD..00599B0F
        fr.w(0x140) = 0;                                                              // 00599B04
        const double Y = fr.d(0x14C) * u->y;                                          // 00599B14..00599B1B
        fr.f(0x1A8) = static_cast<float>(fr.d(0x21C) + X);                            // 00599B1E..00599B24
        fr.f(0x1A4) = static_cast<float>(Y + fr.d(0x218));                            // 00599B2B..00599B2F
    }
    {
        const double L34 = fr.d(0x34), L3C = fr.d(0x3C);                              // 00599B38 / 00599B3F
        const double dz = fr.d(0x1FC) - fr.d(0x220);                                  // 00599B46..00599B4A
        fr.f(0x148) = static_cast<float>(fr.d(0x1A8) - fr.d(0x228));                 // 00599B4E..00599B59
        fr.f(0x144) = static_cast<float>(fr.d(0x1A4) - fr.d(0x224));                 // 00599B60..00599B6B
        fr.f(0x14C) = static_cast<float>(((L34 * L34) + (dz * dz)) + (L3C * L3C));    // 00599B72..00599B82
    }
    {
        const double s = ((fr.d(0x148) * fr.d(0x148)) + (fr.d(0x140) * fr.d(0x140))) + (fr.d(0x144) * fr.d(0x144));   // 00599B8F..00599BBB
        fr.f(0x1B8) = static_cast<float>(s);                                          // 00599BBD fst
        if (s > fr.d(0x14C)) {                                                        // 00599BC4..00599BD0 (test ah,41h; jne)
            fr.f(0x1EC) = static_cast<float>(-fr.d(0x1C8)); fr.w(0x1C8) = fr.w(0x1EC);   // 00599BD6..00599BEB
            fr.f(0x1E8) = static_cast<float>(-fr.d(0x1C4)); fr.w(0x1C4) = fr.w(0x1E8);   // 00599BE4..00599C09
            fr.f(0x1E4) = static_cast<float>(-fr.d(0x1C0));                           // 00599BFC..00599C10
            const double A = -fr.d(0x1E8);                                            // 00599C14..00599C1F
            fr.w(0x234) = fr.w(0x7C);                                                 // 00599BF1 / 00599C29
            fr.w(0x230) = fr.w(0x8C);                                                 // 00599C18 / 00599C39
            fr.w(0x1C0) = fr.w(0x1E4);                                                // 00599C21 / 00599C41
            fr.w(0x22C) = fr.w(0x19C);                                                // 00599C4C / 00599C62
            const double P = ((fr.d(0x1E8) * fr.d(0x200)) + (fr.d(0x1EC) * fr.d(0x204))) + (fr.d(0x1E4) * fr.d(0x1FC));   // 00599C2D..00599C6A
            const double Q = ((fr.d(0x1E4) * fr.d(0x220)) + (fr.d(0x1E8) * fr.d(0x224))) + (fr.d(0x1EC) * fr.d(0x228));   // 00599C6C..00599C86
            const double num = P - Q;                                                 // 00599C88
            const double D1 = ((fr.d(0x1EC) * fr.d(0x200)) + (A * fr.d(0x204))) + fr.d(0xF0);   // 00599C8A..00599C98
            const double S = (fr.d(0x1EC) * fr.d(0x224)) + (A * fr.d(0x228));         // 00599C9F..00599CAB
            const double den = D1 - (S + fr.d(0x184));                                // 00599CAD..00599CB6
            const double w = ((num / den) * r) + k_0_03_00747678;                     // 00599CB8..00599CC1
            fr.f(0x184) = static_cast<float>(std::sqrt((w * w) + fr.d(0x124)));       // 00599CC7..00599CD4
            const Vector* u = normalize_004ab180(fr.v(0x60), fr.v(0x234));            // 00599C53..00599CDD
            const double X = fr.d(0x184) * u->x;                                      // 00599CE2..00599CEC
            const double Y = fr.d(0x184) * u->y;                                      // 00599CEE..00599CF5
            const double Z = fr.d(0x184) * u->z;                                      // 00599CF8..00599CFF
            fr.f(0xD8) = static_cast<float>(Z);                                       // 00599D02
            fr.f(0x13C) = static_cast<float>(fr.d(0x21C) + X); fr.w(0x1A8) = fr.w(0x13C);   // 00599D09..00599D1D
            fr.f(0x138) = static_cast<float>(Y + fr.d(0x218)); fr.w(0x1A4) = fr.w(0x138);   // 00599D24..00599D38
            fr.f(0x134) = static_cast<float>(fr.d(0xD8) + fr.d(0x214)); fr.w(0x1A0) = fr.w(0x134);   // 00599D3F..00599D58
        }
    }
    fr.f(0x1A0) = static_cast<float>(((fr.d(0x19C) / fr.d(0x14C)) * fr.d(0x1B8)) + fr.d(0x220));   // 00599D5F..00599D78
    if (fr.d(0x1F8) == fr.d(0x194) && fr.d(0x1F4) == fr.d(0x190)) {                   // 00599D7F..00599DA1
        // 00599DA3..00599E04 / 00599E91..00599E9D: degenerate edge in XY
        const Vector* nrm7 = mesh_normal(mesh, static_cast<U>(hit_face));            // 00599DA3..00599DAC
        Vector* w5 = ltw(mesh, fr.v(0x60), fr.v(0x194));                             // 00599DC3
        Vector* w4 = ltw(mesh, fr.v(0x30), fr.v(0x1F8));                             // 00599DD8
        Vector* w3 = collision_mesh_rotate_00596b30(mesh, nullptr, fr.v(0x48),
                                                    mesh_normal(mesh, static_cast<U>(hit_face)));   // 00599DDD..00599DEE
        // 00599DF4: the vector at L1B0 overlays slots L1B0/L1AC (edge flags) and L1A8 (see header comment)
        Vector* w2 = ltw(mesh, fr.v(0xE8), fr.v(0x1B0));                             // 00599DF4..00599E93
        walk_hit_info_initialize_004bb570(walk_hit, nullptr, 1, w2, w3, w4, w5, 2, nrm7);   // 00599E98..00599E9D
        return 0;                                                                     // 00599EA2..00599EAE
    }
    // 00599E09..00599E9D
    if (mesh_word(mesh, 4) != 0) fr.copy3(0x1D4, 0x1E0);                             // 00599E09..00599E24
    else quaternion_rotate_004a9bb0(fr.v(0x1D4), mesh_orientation(mesh), fr.v(0x1E0));   // 00599E2A..00599E38
    const Vector* nrm7 = mesh_normal(mesh, static_cast<U>(hit_face));                // 00599E40..00599E46
    Vector* w5 = ltw(mesh, fr.v(0x60), fr.v(0x194));                                 // 00599E5E
    Vector* w4 = ltw(mesh, fr.v(0x30), fr.v(0x1F8));                                 // 00599E73
    Vector* w2 = ltw(mesh, fr.v(0x48), fr.v(0x1A8));                                 // 00599E93
    walk_hit_info_initialize_004bb570(walk_hit, nullptr, 1, w2, fr.v(0x1D4), w4, w5, 1, nrm7);   // 00599E9D
    return 0;                                                                         // 00599EA2..00599EAE
}

}  // extern "C"
