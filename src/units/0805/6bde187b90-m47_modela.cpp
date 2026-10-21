// m47_modela.cpp - lane-m47-3d group "modela": the hit-test leaves left unresolved by lane-m46-3dmodel F1
// (consumer model46-f1-part-core, f1_part_core.cpp:74-78, exact declarations replicated below):
//   004AA1F0 BoxIntersec, 004AB750 polyhit, 00440CF0 AnimMeshHitCheck, 00441570 HitCheckAABBnode.
// Static reading of GOG swkotor.exe (image base 00400000); every behaviour cites original instruction addresses.
//
// x87 policy (D30-FINAL): double intermediates; float only where the original stores a dword (fstp dword).
// Values the original moves with integer mov are copied as raw DWORDs (memcpy). fld dword / fstp dword pairs that
// only move a float are float assignments (identical value; an SNaN would be quieted by the x87 - not modelled).
// Compare patterns (fcomp + fnstsw + test ah), C0/C2/C3 all set when unordered:
//   "test ah,5  / jp"  taken iff !(a <  b)     "test ah,5  / jnp" taken iff a < b
//   "test ah,41h/ jne" taken iff !(a >  b)     "test ah,41h/ je"  taken iff a > b
//   "test ah,41h/ jp"  taken iff !(a <= b)     "test ah,44h/ jp"  taken iff !(a == b)
//   "test ah,44h/ jnp" taken iff a == b        "test ah,1  / jne" taken iff !(a >= b)
// No allocation, no throwing callee (all callees are extern "C" original-ABI bodies).
#include <cstdint>
#include <cstring>
#include "6c81d8874d-scene3d.h"          // kotor_scene3d::Vector, box_intersect_004aa610, vector_cross_004ab210,
                              // aurora_hit_info_clear_output_00456a60, aurora_pool_lock/unlock (lane-3d-render canonical)
#include "7ee14b5ec3-model_vtables.hpp"  // aurora_vt::VTable_00742f74 (VertexPrimitive table layout, lane-3d-model D39)

using kotor_scene3d::Vector;
using U = std::uint32_t;
using I = std::int32_t;
using I16 = std::int16_t;
using U8 = std::uint8_t;

extern "C" {
// ---- the four bodies of this unit (declarations identical to stage 3d/model46/f1_part_core.cpp:74-78)
I __cdecl box_intersec_004aa1f0(const Vector* start, Vector* end, const Vector* box_min, const Vector* box_max, Vector* hit);
I __cdecl polyhit_004ab750(const Vector* points, I count, const void* face_normal, const void* face_distance,
    const Vector* start, Vector* end, Vector* hit);
U __cdecl hit_check_aabb_node_00441570(void* node, const Vector* start, Vector* end, U raw_scale);
U __cdecl anim_mesh_hit_check_00440cf0(void* part, void* node, Vector start, Vector end, Vector* hit, U raw_scale);

// ---- existing M46 providers (declarations identical to their owners' headers / f1_part_core.cpp)
// lane-world-los world_los_geometry.h:26 (Vector layout {x,y,z} identical to kotor_world_los::Vector)
void __cdecl relative_spacing_004aa160(const Vector* point, const Vector* box_min, const Vector* box_max,
    unsigned char* out_code);
// lane-world-los world_los_geometry.h:13 (004AB9D0 polyhit, cdecl, EAX 0/1)
I __cdecl polyhit_004ab9d0(const Vector* points, I count, const Vector* start, const Vector* end,
    Vector* hit_out, Vector* normal_out);
// lane-world-los world_los_geometry.h:7
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float scale);
// lane-world-los (f1_part_core.cpp:68-70 exact declarations)
void __stdcall vector_constructor_iterator_00401130(void* base, U element_size, U count,
    void* (__fastcall* construct)(void*, void*));                                   // RET 10h
void* __fastcall vector_default_construct_00409e70(void* vector, void*);            // RET 0, EAX=this
// model46-f1-part-core cells (defined in f1_part_core.cpp; written by PartAABB::HitCheckGeom 004428F0)
extern void* volatile aurora_aabb_node_007a69b0;            // 00442910 (PartAABB MdlNode)
extern volatile U aurora_aabb_hit_info_007a69b4;            // 00442919 (CHitInfo*)
extern volatile U aurora_aabb_direction_mask_007a8a24;      // 0044298A (octant mask)

// ---- cells owned here: image-wide byte search finds these addresses ONLY inside the function that uses them
// (007A8A28/2C/30: 0044157x..004428xx; 00831F78..00831F90: 004AB7D6..004AB962). All .bss (zero).
volatile U aurora_aabb_hit_face_007a8a28 = 0;     // face index of the last AABB hit (00441765 ...); never read
volatile U aurora_aabb_child_order_007a8a2c = 0;  // child-order selector (004417A2 ...); no writer in the image
volatile U aurora_aabb_visit_count_007a8a30 = 0;  // ++ per visited node (00441574..00441586 / 004417D1..004417D7)
float aurora_polyhit_normal_00831f78[3] = {0.0f, 0.0f, 0.0f};   // polyhit function-static (guard bit 1)
float aurora_polyhit_point_00831f84[3] = {0.0f, 0.0f, 0.0f};    // polyhit function-static (guard bit 0)
volatile U aurora_polyhit_guard_00831f90 = 0;                   // guard word (004AB7D5..004AB80B)
}

namespace {
template <class T> inline T rd(const void* p, U off) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T)); return v;
}
template <class T> inline void wr(void* p, U off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(T)); }
inline unsigned char* bp(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
inline const unsigned char* bp(const void* p, U off) { return static_cast<const unsigned char*>(p) + off; }
inline void* obj(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline void copy3(void* dst, const void* src) { std::memcpy(dst, src, 12); }   // three integer mov pairs
inline double W(float v) { return static_cast<double>(v); }                    // fld dword
inline float f32(double v) { return static_cast<float>(v); }                   // fstp dword (PC_53, RC nearest)
inline float as_float(U bits) { float f; std::memcpy(&f, &bits, 4); return f; }
constexpr float kOne_0073d764 = 1.0f;        // .rdata 3F800000
constexpr float kZero_0073d700 = 0.0f;       // .rdata 00000000
constexpr U kInsideEps_007440f8 = 0xB727C5ACu; // .rdata float -1.0e-5 (004AB96B fcomp dword)
using PrimitiveTable = aurora_vt::VTable_00742f74;   // VertexPrimitive layout (all four primitive tables identical)
inline const PrimitiveTable* primitive_vt(void* p) { return static_cast<const PrimitiveTable*>(rd<const void*>(p, 0)); }
}

extern "C" {

// ============================================================================================ 004AA1F0
// BoxIntersec(start, end, box_min, box_max, hit) cdecl. Outcodes from RelativeSpacing 004AA160 (20h x<min.x,
// 10h x>max.x, 8 y<min.y, 4 y>max.y, 2 z<min.z, 1 z>max.z). Returns -1 when start is inside, 0 on a trivial
// reject / no face crossing, else the crossed face 1..6 (min.x, max.x, min.y, max.y, min.z, max.z) with hit set.
// For the face plane c = k: t = (k - s.c)/(e.c - s.c) (wide), other components (1-t)*s + t*e stored as dwords,
// accepted when min <= v <= max for both (fcomp: test ah,1 / jne and test ah,41h / jp). end is not written.
I __cdecl box_intersec_004aa1f0(const Vector* start, Vector* end, const Vector* box_min, const Vector* box_max, Vector* hit) {
    const Vector* const s = start;     // esi
    const Vector* const e = end;       // edi
    const Vector* const mn = box_min;  // ebp
    const Vector* const mx = box_max;  // ebx
    unsigned char code_start = 0, code_end = 0;                                         // [esp+0Eh] / [esp+0Fh]
    relative_spacing_004aa160(s, mn, mx, &code_start);                                  // 004AA202..004AA20A
    if (code_start == 0) return -1;                                                     // 004AA20F..004AA225
    relative_spacing_004aa160(e, mn, mx, &code_end);                                    // 004AA226..004AA233
    if ((code_start & code_end) != 0) return 0;                                         // 004AA238..004AA245 -> 004AA5F7
    const U8 c = code_start;                                                            // cl
    // ---------------- x faces
    if ((c & 0x20) != 0 || (c & 0x10) != 0) {                                           // 004AA24B / 004AA2F1
        const bool low = (c & 0x20) != 0;
        const float k = low ? mn->x : mx->x;                                            // [ebp] / [ebx]
        const double t = (W(k) - W(s->x)) / (W(e->x) - W(s->x));                        // 004AA254..004AA25D / 004AA2FA..004AA302
        const double u = W(kOne_0073d764) - t;                                          // 004AA25F..004AA265
        const float y = f32(u * W(s->y) + t * W(e->y));                                 // 004AA267..004AA273 / 004AA30C..004AA318
        const float z = f32(u * W(s->z) + t * W(e->z));                                 // 004AA277..004AA281 / 004AA31C..004AA326
        if (y >= mn->y && y <= mx->y && z >= mn->z && z <= mx->z) {                     // 004AA285..004AA2C7 / 004AA32A..004AA360
            std::memcpy(&hit->x, low ? &mn->x : &mx->x, 4);                             // 004AA2CD / 004AA362 (raw)
            hit->y = y; hit->z = z;                                                     // 004AA2E1..004AA2E4 / 004AA375..004AA378 (raw dwords)
            return low ? 1 : 2;                                                         // 004AA2E7 / 004AA37B
        }
    }
    // ---------------- y faces (004AA385)
    if ((c & 8) != 0 || (c & 4) != 0) {                                                 // 004AA385 / 004AA42B
        const bool low = (c & 8) != 0;
        const float k = low ? mn->y : mx->y;
        const double t = (W(k) - W(s->y)) / (W(e->y) - W(s->y));                        // 004AA38E..004AA39A / 004AA434..004AA440
        const double u = W(kOne_0073d764) - t;                                          // 004AA39C..004AA3A2
        const float x = f32(u * W(s->x) + t * W(e->x));                                 // 004AA3A4..004AA3AE ([esp+18h])
        const float z = f32(u * W(s->z) + t * W(e->z));                                 // 004AA3B2..004AA3BC ([esp+14h])
        if (x >= mn->x && x <= mx->x && z >= mn->z && z <= mx->z) {                     // 004AA3C0..004AA401 / 004AA466..004AA49B
            hit->x = x;                                                                 // 004AA40B..004AA413 fld/fstp
            std::memcpy(&hit->y, low ? &mn->y : &mx->y, 4);                             // 004AA415 / 004AA4AB (raw)
            hit->z = z;                                                                 // 004AA41E / 004AA4B4
            return low ? 3 : 4;                                                         // 004AA421 / 004AA4B7
        }
    }
    // ---------------- z faces (004AA4C1)
    if ((c & 2) != 0 || (c & 1) != 0) {                                                 // 004AA4C1 / 004AA563
        const bool low = (c & 2) != 0;
        const float k = low ? mn->z : mx->z;
        const double t = (W(k) - W(s->z)) / (W(e->z) - W(s->z));                        // 004AA4CA..004AA4D6 / 004AA56C..004AA578
        const double u = W(kOne_0073d764) - t;                                          // 004AA4D8..004AA4DE
        const float x = f32(u * W(s->x) + t * W(e->x));                                 // 004AA4E0..004AA4EA ([esp+18h])
        const float y = f32(u * W(s->y) + t * W(e->y));                                 // 004AA4EE..004AA4F8 ([esp+14h]; 004AA590..004AA59C)
        if (y >= mn->y && y <= mx->y && x >= mn->x && x <= mx->x) {                     // 004AA4FC..004AA539 / 004AA5A0..004AA5D1 (y first)
            hit->x = x;                                                                 // 004AA543..004AA54B fld/fstp
            hit->y = y;                                                                 // 004AA54E (raw)
            std::memcpy(&hit->z, low ? &mn->z : &mx->z, 4);                             // 004AA551..004AA556 / 004AA5E5..004AA5EA
            return low ? 5 : 6;                                                         // 004AA559 / 004AA5ED
        }
    }
    return 0;                                                                           // 004AA5F7..004AA600
}

// ============================================================================================ 004AB750
// polyhit(points, count, face_normal, face_distance*, start, end, hit) cdecl. Plane distances of start/end are
// stored as dwords (d1 at the face_normal argument slot [esp+40h], d2 at the start slot [esp+48h] - callee-owned
// argument cells, not observable). Rejects when d1 < 0, d2 > 0 or d1 == d2. Hit point P = (1-t)*start + t*end,
// t = d1/(d1-d2), kept in the function-static 00831F84 (guard bit 0); per edge i the cross product
// (P[i+1]-P[i]) x (hit-P[i]) is kept in the function-static 00831F78 (guard bit 1) and must satisfy
// dot(cross, normal) >= -1e-5 (007440F8). EAX 0/1; hit written only on success (null allowed). end is not written.
I __cdecl polyhit_004ab750(const Vector* points, I count, const void* face_normal, const void* face_distance,
    const Vector* start, Vector* end, Vector* hit) {
    const float* const n = static_cast<const float*>(face_normal);                     // edi
    const float dist = rd<float>(face_distance, 0);                                     // [eax]
    const Vector* const s = start;                                                      // ecx
    const Vector* const e = end;                                                        // edx
    const float d1 = f32(((W(n[1]) * W(s->y) + W(n[0]) * W(s->x)) + W(s->z) * W(n[2])) + W(dist));   // 004AB764..004AB77A
    const float d2 = f32(((W(n[1]) * W(e->y) + W(n[0]) * W(e->x)) + W(n[2]) * W(e->z)) + W(dist));   // 004AB77E..004AB794
    if (d1 < kZero_0073d700) return 0;                                                  // 004AB798..004AB7A7 (test ah,5; jnp)
    if (d2 > kZero_0073d700) return 0;                                                  // 004AB7AD..004AB7BC (test ah,41h; je)
    if (d1 == d2) return 0;                                                             // 004AB7C2..004AB7CF (test ah,44h; jnp)
    U guard = aurora_polyhit_guard_00831f90;                                            // 004AB7D5
    if ((guard & 1u) == 0) {                                                            // 004AB7DA..004AB7DC
        guard |= 1u; aurora_polyhit_guard_00831f90 = guard;                            // 004AB7DE..004AB7E1
        wr<U>(aurora_polyhit_point_00831f84, 0, 0); wr<U>(aurora_polyhit_point_00831f84, 4, 0);
        wr<U>(aurora_polyhit_point_00831f84, 8, 0);                                     // 004AB7E6..004AB7FA
    }
    if ((guard & 2u) == 0) {                                                            // 004AB804..004AB806
        guard |= 2u; aurora_polyhit_guard_00831f90 = guard;                            // 004AB808..004AB80B
        wr<U>(aurora_polyhit_normal_00831f78, 0, 0); wr<U>(aurora_polyhit_normal_00831f78, 4, 0);
        wr<U>(aurora_polyhit_normal_00831f78, 8, 0);                                    // 004AB810..004AB824
    }
    const double t = W(d1) / (W(d1) - W(d2));                                           // 004AB82E..004AB83D (fsub; fdivr)
    const double ex = t * W(e->x);                                                      // 004AB844..004AB846 (kept wide)
    const float ey = f32(t * W(e->y));                                                  // 004AB848..004AB84D [esp+2Ch]
    const float ez = f32(t * W(e->z));                                                  // 004AB851..004AB856 [esp+30h]
    const double u = W(kOne_0073d764) - t;                                              // 004AB85A..004AB860
    const float sx = f32(u * W(s->x));                                                  // 004AB862..004AB866 [esp+1Ch]
    const float sy = f32(u * W(s->y));                                                  // 004AB86A..004AB86F [esp+20h]
    const float sz = f32(u * W(s->z));                                                  // 004AB873..004AB876 [esp+24h]
    const float hx = f32(W(sx) + ex);                                                   // 004AB87A..004AB880 [esp+10h]
    std::memcpy(&aurora_polyhit_point_00831f84[0], &hx, 4);                             // 004AB884..004AB888
    const float hy = f32(W(sy) + W(ey));                                                // 004AB896..004AB89E [esp+14h]
    std::memcpy(&aurora_polyhit_point_00831f84[1], &hy, 4);                             // 004AB8A6..004AB8AE
    const float hz = f32(W(sz) + W(ez));                                                // 004AB8A2..004AB8B6 [esp+18h]
    std::memcpy(&aurora_polyhit_point_00831f84[2], &hz, 4);                             // 004AB8BA..004AB8BE
    I inside = 1;                                                                       // 004AB88F mov eax,1
    for (I i = 0; i < count; ) {                                                        // 004AB8B4 / 004AB8C4..004AB8C8 (jge)
        const I next = (i + 1) % count;                                                 // 004AB8CE..004AB8DB (cdq; idiv)
        const Vector* const p = &points[i];                                             // esi-8 (esi = &points[i].z)
        Vector to_hit, side, cross_out;
        to_hit.x = f32(W(hx) - W(p->x));                                             // 004AB8D1..004AB8DF [esp+28h]
        to_hit.y = f32(W(hy) - W(p->y));                                             // 004AB8E3..004AB8EE [esp+2Ch]
        to_hit.z = f32(W(hz) - W(p->z));                                             // 004AB8F3..004AB8F9 [esp+30h]
        const Vector* const q = &points[next];                                          // 004AB8FD..004AB904
        side.x = f32(W(q->x) - W(p->x));                                             // 004AB900..004AB910 [esp+1Ch]
        side.y = f32(W(q->y) - W(p->y));                                             // 004AB914..004AB91A [esp+20h]
        side.z = f32(W(q->z) - W(p->z));                                             // 004AB91E..004AB928 [esp+24h]
        const Vector* const c = vector_cross_004ab210(&cross_out, &side, &to_hit);      // 004AB92C (out, side, to_hit)
        copy3(aurora_polyhit_normal_00831f78, c);                                       // 004AB931..004AB94B (raw)
        const double dot = (W(aurora_polyhit_normal_00831f78[1]) * W(n[1]) + W(aurora_polyhit_normal_00831f78[0]) * W(n[0])) +
                           W(aurora_polyhit_normal_00831f78[2]) * W(n[2]);              // 004AB942..004AB969
        inside = (dot >= as_float(kInsideEps_007440f8)) ? 1 : 0;                        // 004AB96B..004AB97F (test ah,1; jne)
        // 004AB981 add esi,0Ch (next point)
        i = i + 1;                                                                      // 004AB986 mov ecx,ebx
        if (inside == 0) return 0;                                                      // 004AB984..004AB995 (hit not written)
    }
    if (inside != 0 && hit != nullptr) {                                                // 004AB996..004AB9A0
        std::memcpy(&hit->x, &hx, 4); std::memcpy(&hit->y, &hy, 4); std::memcpy(&hit->z, &hz, 4);   // 004AB9A2..004AB9B3 (locals)
    }
    return inside;                                                                      // EAX
}

// ============================================================================================ 00440CF0
// AnimMeshHitCheck(part, anim-mesh node, start(v), end(v), hit, scale) cdecl. Locks the vertex pool of the
// part's vertex primitive (part+70 -> [vtbl+0C] (VertexPrimitiveAnim: self_return 00641DB0) -> +1C handle) with
// LockPool(handle,0,0,8); every face (node+58, stride 20h, count node+5C re-read per face; word corners
// +1A/+1C/+1E, vertex = base + index*12) is tested with polyhit 004AB9D0(tri, 3, &start, &end, &end, 0) - the
// by-value end argument is shortened in place by each hit - and the results are OR-accumulated in the node
// argument cell ([esp+60h]). Scale != 1.0 multiplies each vertex component (fld scale; fmul; fstp dword).
// After UnlockPool: if accumulated != 0 and hit != 0, *hit = end (raw). EAX = accumulator; 0 when node/part null.
U __cdecl anim_mesh_hit_check_00440cf0(void* part, void* node, Vector start, Vector end, Vector* hit, U raw_scale) {
    if (node == nullptr) return 0;                                                      // 00440CF5..00440CFD
    if (part == nullptr) return 0;                                                      // 00440D03..00440D09
    U hits = 0;                                                                         // 00440D16 mov [esp+60h],ebp (node arg cell)
    void* const holder = rd<void*>(part, 0x70);                                         // 00440D0F
    void* const primitive = obj(primitive_vt(holder)->s00c(holder, nullptr));          // 00440D12..00440D1A call [eax+0Ch]
    const U base = static_cast<U>(reinterpret_cast<std::uintptr_t>(
        aurora_pool_lock_00423f10(rd<U>(primitive, 0x1C), 0, 0, 8)));                  // 00440D1D..00440D25 push 8,0,0,[eax+1Ch]
    const float scale = as_float(raw_scale);
    const bool unit = (scale == kOne_0073d764);                                         // 00440D2A..00440D46 (test ah,44h; jp -> scaled)
    U face_off = 0;                                                                     // ebp
    for (I f = 0; f < rd<I>(node, 0x5C); ++f) {                                         // 00440D43..00440D4E / 00440E36..00440E46 (inc ebx; reload)
        Vector tri[3];
        for (int k = 0; k < 3; ++k) { wr<U>(&tri[k], 0, 0); wr<U>(&tri[k], 4, 0); wr<U>(&tri[k], 8, 0); }   // 00440D54..00440D6E (inline Vector())
        const unsigned char* const face = static_cast<const unsigned char*>(obj(rd<U>(node, 0x58))) + face_off;  // 00440D70..00440D73
        for (int k = 0; k < 3; ++k) {
            const I16 index = rd<I16>(face, static_cast<U>(0x1A + 2 * k));             // 00440D76 / 00440DAC / 00440DE0 (movsx)
            const float* const v = static_cast<const float*>(obj(base + static_cast<U>(static_cast<I>(index) * 12))); // lea [i*3]; [esi+i*4]
            if (unit) {
                std::memcpy(&tri[k].x, &v[0], 4);                                       // 00440D80..00440D98 (raw x)
                tri[k].y = v[1];                                                        // 00440D86..00440D9C (fld/fstp)
                tri[k].z = v[2];                                                        // 00440D8F..00440DA8 (fld/fstp)
            } else {
                tri[k].x = f32(W(scale) * W(v[0]));                                     // 00440E83..00440E9B / 00440ECF..00440EE6 / 00440F1A..00440F31
                tri[k].y = f32(W(scale) * W(v[1]));                                     // 00440EA3..00440EAD / ...
                tri[k].z = f32(W(scale) * W(v[2]));                                     // 00440EB1..00440EC7 / ...
            }
        }
        hits |= static_cast<U>(polyhit_004ab9d0(tri, 3, &start, &end, &end, nullptr));  // 00440D89 push 0 .. 00440E2B / 00440F7F; 00440E30..00440E42
        face_off += 0x20;                                                               // 00440E3D / 00440F91
    }
    void* const holder2 = rd<void*>(part, 0x70);                                        // 00440FA0..00440FA4
    void* const primitive2 = obj(primitive_vt(holder2)->s00c(holder2, nullptr));       // 00440FA7..00440FA9
    (void)aurora_pool_unlock_00423f30(rd<U>(primitive2, 0x1C));                         // 00440FAC..00440FB0
    if (hits != 0 && hit != nullptr) copy3(hit, &end);                                  // 00440FBC..00440FDB (raw)
    return hits;                                                                        // 00440FDF mov eax,ecx
}

}  // extern "C"

// ============================================================================================ 00441570
// HitCheckAABBnode(node, start, end, scale) cdecl, recursive over the PartAABB tree (node: min +0, max +0C,
// children +18/+1C, face index +20 (-1 = internal), octant bits +24). The original body contains ONE inlined
// level of its own recursion, and the two copies order the children differently (binary-observed; C++ leaves the
// operand order of a + b unspecified, MSVC chose differently per copy):
//   * outer copy (each real call): ++visit counter; box test; leaf, or children ordered by 007A8A2C and
//     (node+24 & 007A8A24):  2C!=0 && bit -> 1C,18 ; 2C!=0 && !bit -> 18,1C ; 2C==0 && bit -> 18,1C ;
//     2C==0 && !bit -> 1C,18  (004417A2 / 00441CBE / 00442070 / 004424EF); each child runs the inlined copy.
//   * inlined copy (one level down): ++visit counter; box test (scaled box via vectorComponentMult 00423BF0);
//     leaf, or real calls 00441570(c1C) then 00441570(c18) on every path (004419A8..00441A58 etc.; the flag and
//     mask are read and tested but both branches are identical).
// Result = sum of the children's results (leaf: 0/1). Leaf (face f): fails when bit (1 << face+10) is clear in
// CHitInfo+4; three vertices (MdlNode+198, raw, NOT scaled - both scale branches compute the same address,
// 004416A9 / 004418AE / 0044205F) are tested with polyhit 004AB750(tri, 3, face, face+0C, start, end, end); on a hit
// CHitInfo::ClearOutput, info+40 = face normal, 007A8A28 = info+3C = f, info+34 = 1 << face+10, info+28 = info+1C
// = *end.
namespace {
U aabb_leaf(I face_index, const Vector* start, Vector* end) {
    void* const tree = aurora_aabb_node_007a69b0;                                       // 00441638
    unsigned char* const face = static_cast<unsigned char*>(obj(rd<U>(tree, 0x58))) + static_cast<U>(face_index) * 0x20u;   // 0044163E..0044164F
    const U info = aurora_aabb_hit_info_007a69b4;                                       // 00441641
    const U surface_bit = 1u << (rd<U>(face, 0x10) & 31u);                              // 0044164B..00441656 (shl edx,cl)
    if ((surface_bit & rd<U>(obj(info), 4)) == 0) return 0;                             // 00441658..00441673
    Vector tri[3];
    vector_constructor_iterator_00401130(tri, 0xC, 3, &vector_default_construct_00409e70);   // 00441674..00441682
    const U verts = rd<U>(aurora_aabb_node_007a69b0, 0x198);                            // 00441694..0044169A (reloaded)
    for (int k = 0; k < 3; ++k) {
        const I16 index = rd<I16>(face, static_cast<U>(0x1A + 2 * k));                 // 004416A5 / 004416CD / 004416EB (movsx)
        copy3(&tri[k], obj(verts + static_cast<U>(static_cast<I>(index) * 12)));        // 004416AB..00441718 (raw; scale unused)
    }
    if (polyhit_004ab750(tri, 3, face, face + 0xC, start, end, end) == 0) return 0;     // 004416F8..00441726
    aurora_hit_info_clear_output_00456a60(obj(aurora_aabb_hit_info_007a69b4), nullptr); // 00441728..00441736
    void* const out = obj(aurora_aabb_hit_info_007a69b4);                               // 0044173B
    copy3(bp(out, 0x40), face);                                                         // 00441740..00441752
    const U bit = 1u << (rd<U>(face, 0x10) & 31u);                                      // 00441755..0044175D
    aurora_aabb_hit_face_007a8a28 = static_cast<U>(face_index);                         // 00441765
    wr<U>(out, 0x34, bit);                                                              // 0044176B
    wr<U>(out, 0x3C, static_cast<U>(face_index));                                       // 0044176E
    copy3(bp(out, 0x28), end);                                                          // 00441771..00441780
    copy3(bp(out, 0x1C), end);                                                          // 00441783..00441790
    return 1;                                                                           // 0044172E [esp+10h] = 1
}

// The inlined copy (one level below a real call): 004417CE..00441A58 (child 1C, flag&&bit), 00441A6E.. (child 18),
// 00441CBE.. / 00441F31.. / 0044207F.. / 004422F3.. / 004424EF.. / 00442762.. (the other child slots).
U aabb_child_inlined(void* child, const Vector* start, Vector* end, U raw_scale) {
    aurora_aabb_visit_count_007a8a30 = aurora_aabb_visit_count_007a8a30 + 1;            // 004417D1..004417D7
    const float scale = as_float(raw_scale);
    I inside;
    if (scale == kOne_0073d764) {                                                       // 004417A8..004417E1 (test ah,44h; jp)
        inside = box_intersect_004aa610(start, end, static_cast<const Vector*>(child),
                                        static_cast<const Vector*>(static_cast<void*>(bp(child, 0x0C))));   // 004417E3..004417EA
    } else {
        Vector t_max, t_min;
        const Vector* const smax = vector_component_mult_00423bf0(&t_max, static_cast<const Vector*>(static_cast<void*>(bp(child, 0x0C))), scale); // 00441806..00441817
        const Vector* const smin = vector_component_mult_00423bf0(&t_min, static_cast<const Vector*>(child), scale);   // 0044181F..00441827
        inside = box_intersect_004aa610(start, end, smin, smax);                        // 0044182F..00441832
    }
    if (inside == 0) return 0;                                                          // 004417F9..00441801
    const I face_index = rd<I>(child, 0x20);                                            // 00441839..00441843
    if (face_index != -1) return aabb_leaf(face_index, start, end);                     // 00441849..004419A3 (same steps; 007A8A28 stored last at 0044199A)
    (void)aurora_aabb_child_order_007a8a2c;                                             // 004419A8 read; both branches identical
    (void)aurora_aabb_direction_mask_007a8a24;                                          // 004419B7 / 00441A0D read; both branches identical
    const U r1 = hit_check_aabb_node_00441570(rd<void*>(child, 0x1C), start, end, raw_scale);   // 004419BF..004419C5 / 004419E9 / 00441A1B / 00441A3A
    const U r2 = hit_check_aabb_node_00441570(rd<void*>(child, 0x18), start, end, raw_scale);   // 004419CA..004419D5 / 004419F9 / 00441A2B / 00441A4A
    return r1 + r2;                                                                     // 004419DA..004419E3 (add)
}
}  // namespace

extern "C" U __cdecl hit_check_aabb_node_00441570(void* node, const Vector* start, Vector* end, U raw_scale) {
    aurora_aabb_visit_count_007a8a30 = aurora_aabb_visit_count_007a8a30 + 1;            // 00441574..00441586
    const float scale = as_float(raw_scale);
    I inside;
    if (scale == kOne_0073d764) {                                                       // 00441570..004415A7 (test ah,44h; jp)
        inside = box_intersect_004aa610(start, end, static_cast<const Vector*>(node),
                                        static_cast<const Vector*>(static_cast<void*>(bp(node, 0x0C))));   // 004415A9..004415B0
    } else {
        Vector smax, smin;                                                              // [esp+10h..] / [esp+1Ch..]
        smax.x = f32(W(scale) * W(rd<float>(node, 0x0C)));                              // 004415C5..004415D4
        smax.y = f32(W(scale) * W(rd<float>(node, 0x10)));                              // 004415D8..004415E2
        smax.z = f32(W(scale) * W(rd<float>(node, 0x14)));                              // 004415E6..004415F0
        smin.x = f32(W(scale) * W(rd<float>(node, 0x00)));                              // 004415F4..004415FD
        smin.y = f32(W(scale) * W(rd<float>(node, 0x04)));                              // 00441601..0044160B
        smin.z = f32(W(scale) * W(rd<float>(node, 0x08)));                              // 0044160F..0044161D
        inside = box_intersect_004aa610(start, end, &smin, &smax);                      // 00441619..004415B0 (jmp 004415AD)
    }
    if (inside == 0) return 0;                                                          // 004415B8..004415C4
    const I face_index = rd<I>(node, 0x20);                                             // 0044162C
    if (face_index != -1) return aabb_leaf(face_index, start, end);                     // 0044162F..004417A1
    U r1, r2;
    const bool bit = (rd<U>(node, 0x24) & aurora_aabb_direction_mask_007a8a24) != 0;    // 004417BF..004417C5 / 00442070..00442076
    if (aurora_aabb_child_order_007a8a2c != 0) {                                        // 004417A2..004417B9
        if (bit) {
            r1 = aabb_child_inlined(rd<void*>(node, 0x1C), start, end, raw_scale);      // 004417CE..00441A58
            r2 = aabb_child_inlined(rd<void*>(node, 0x18), start, end, raw_scale);      // 00441A5C..00441C98
        } else {
            r1 = aabb_child_inlined(rd<void*>(node, 0x18), start, end, raw_scale);      // 00441CBE..00441F2F
            r2 = aabb_child_inlined(rd<void*>(node, 0x1C), start, end, raw_scale);      // 00441F31..
        }
    } else {
        if (bit) {
            r1 = aabb_child_inlined(rd<void*>(node, 0x18), start, end, raw_scale);      // 0044207F..004422F1
            r2 = aabb_child_inlined(rd<void*>(node, 0x1C), start, end, raw_scale);      // 004422F3..004424EE
        } else {
            r1 = aabb_child_inlined(rd<void*>(node, 0x1C), start, end, raw_scale);      // 004424EF..00442760
            r2 = aabb_child_inlined(rd<void*>(node, 0x18), start, end, raw_scale);      // 00442762..004428E8
        }
    }
    return r1 + r2;                                                                     // 00441A9C..00441AA2 / 004428D7..004428DB (lea eax,[edx+ebp])
}
