// m47_modelb_shadow.cpp - lane-m47-3d fork "modelb": stencil shadow-volume helpers of the GOG swkotor.exe (image base
// 0x400000) referenced by model46-f2-part (PartTriMesh::Project 00474220 / PartSkin::Project 00477B90):
//   004741A0 ProcessFaces, 0046F580 ClipProjectionCaps, 004269F0 GLRender::BuildShadowArray,
//   00426660 GLRender::RenderShadowArray,
// plus their callees with no provider anywhere (authored here): 00426940 AllocateShadowMemory, 0046F650 ClipProjection,
// 0045F9F0 AurTwoSidedStencilAvailable. Listings: lane-m47-3d/d/<ADDR>.txt (local only).
//
// Exported names/ABI are exactly the consumer declarations in the M46 stage 3d/model46/f2_part.cpp lines 44-48
// (extern "C" __cdecl, caller cleanup; every original body ends in a plain RET).
// D30-FINAL: x87 intermediates are double; float only where the original stores with `fstp dword` (or moves a stored
// float DWORD with mov). fmul dword of two binary32 values is exact in double (48-bit product), so double products
// equal the original's 53-bit-PC products; sums are rounded at 53 bits exactly like PC_53 x87.
// x87 compare idiom `fcomp 0.0 (0073D700); fnstsw; test ah,41h; jne` = "jump unless st > 0 (ordered)": written as
// `!(value > 0.0)` (NaN -> jump), matching C0/C3 set on unordered.
// EH: 00426660 / 00426940 have SEH frames (states 0/-1) that only clear the static guard bit if operator new throws;
// the original operator new returns NULL (D30-FINAL) and nothing here throws, so the funclets (0071582E / 0071584E)
// are cited, not emitted.
#include "6c81d8874d-scene3d.h"                    // kotor_scene3d::Vector / Plane (3d/render, canonical)
#include "5d11c14380-model_load.hpp"               // live_operator_new (006FA7E6 / thunk 006FC0D2), live_free (006FA390 / thunk 006FC0CD)
#include "75b7842bf0-renderer_gl_imports_gen.h"    // kotor_renderer_init GL import adapters (aurora-reviewed/gl-cohort)
#include "61e3cacb4e-renderer_gl_procs.h"          // kotor_renderer_gl_procs extension cells / loader masks
#include <cstdint>

using kotor_scene3d::Vector;
using kotor_scene3d::Plane;
using U = std::uint32_t;
using I = std::int32_t;
using U16 = std::uint16_t;

// ---- existing providers (identical declarations to their owners' headers)
extern "C" {
// world-los/world_los_geometry.h (M46 world-los-geometry-core-handover)
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance,
    const Vector* a, const Vector* b);
// model46-f2-part storage (first definer f2_part.cpp)
extern volatile std::uint16_t shadow_vertex_marks_007a4820[0x1000];
extern volatile std::uint16_t shadow_edge_indices_007a69c0[0x800 * 3 * 2];
}
namespace kotor_renderer_init { extern U32 __stdcall VertexBufferAvailable_0045fc90(); }   // extension_loader.h (renderer-bodies)
namespace kotor_renderer_gl_cells { extern std::uint32_t capabilities_007bb85c; }        // renderer_gl_cells.h (renderer-bodies)
namespace r4_lowers_cells { extern std::uint32_t volatile render_counts_00827fa8[6]; }   // r4-lowers/r4_lowers.h (D69 owner of 00827FB0)

extern "C" {
// ---- consumer declarations (M46 3d/model46/f2_part.cpp:44-48), defined here
I __cdecl clip_projection_caps_0046f580(Vector* direction_scaled, Vector* point, U frustum);
void __cdecl process_faces_004741a0(const Vector* light, void* face_records, void* node);
U __cdecl gl_render_build_shadow_array_004269f0(const Vector* light, float* length, U frustum,
    Vector* direction_scaled, Vector* point, I vertex_count, U pool);
void __cdecl gl_render_render_shadow_array_00426660(U array, I edges, U caps);
// ---- authored callees (no provider in any lane / stage)
void* __cdecl gl_render_allocate_shadow_memory_00426940(I size);
void __cdecl clip_projection_0046f650(Vector* v, const Vector* origin, U frustum);
U __cdecl aur_two_sided_stencil_available_0045f9f0();

// ---- original cells referenced only by the bodies in this unit (raw-image absolute-reference search, cited)
// 0078E540 .data cold FFFFFFFF: AurTwoSidedStencilAvailable cache (only 0045F9F1 / 0045FA0C).
volatile U two_sided_stencil_cache_0078e540 = 0xFFFFFFFFu;
// 007A68DC BSS, only read at 004267A1 (no writer in the image -> 0 for the process).
volatile U shadow_two_sided_enabled_007a68dc = 0;
// 007A68E0 BSS, only read at 00426BA1 (no writer -> 0: ClipProjection always runs).
volatile U shadow_skip_clip_007a68e0 = 0;
// 0078D994 .data cold 1, only read at 00426BC9 (no writer).
volatile U shadow_collapse_far_0078d994 = 1;
// 007A47E8 BSS: vertex array pointer, written 00426A97 (BuildShadowArray), read 0042678C (RenderShadowArray).
void* volatile shadow_vertex_array_007a47e8 = nullptr;
// RenderShadowArray static index buffer: guard 007A694C bit 1, pointer 007A6948 (004266A3 / 004266B2).
volatile U render_shadow_guard_007a694c = 0;
std::uint16_t* volatile render_shadow_indices_007a6948 = nullptr;
// AllocateShadowMemory statics: guard 007A6958 (bits 1, 2), block 007A6954, capacity 007A6950; maximum 00828050.
volatile U shadow_memory_guard_007a6958 = 0;
void* volatile shadow_memory_block_007a6954 = nullptr;
volatile I shadow_memory_capacity_007a6950 = 0;
volatile I shadow_memory_peak_00828050 = 0;
// BuildShadowArray statics: guard 007A6980 (bit 1 -> 007A6974 direction, bit 2 -> 007A695C/007A6968 pair).
volatile U build_shadow_guard_007a6980 = 0;
Vector build_shadow_direction_007a6974 = {0.0f, 0.0f, 0.0f};
Vector build_shadow_pair_007a695c[2] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
}

namespace {
namespace gl = kotor_renderer_init;
namespace procs = kotor_renderer_gl_procs;
constexpr double kZero_0073d700 = 0.0;   // .rdata 0073D700 (float 0)
inline double W(float v) { return static_cast<double>(v); }
inline float S(double v) { return static_cast<float>(v); }
inline const Plane* plane_of(U frustum) { return reinterpret_cast<const Plane*>(static_cast<std::uintptr_t>(frustum)); }
// ((n.z*v.z + n.y*v.y) + n.x*v.x) + d : fld [v+8]; fmul [n+8]; fld [v+4]; fmul [n+4]; faddp; fld [v]; fmul [n]; faddp; fadd [n+C]
inline double plane_distance(const Vector& v, const Plane& p) {
    return ((W(v.z) * W(p.c) + W(v.y) * W(p.b)) + W(v.x) * W(p.a)) + W(p.d);
}
inline void copy_vector(Vector* to, const Vector* from) {   // three DWORD movs (no x87)
    const U x = kotor_scene3d::float_to_bits(from->x), y = kotor_scene3d::float_to_bits(from->y),
            z = kotor_scene3d::float_to_bits(from->z);
    to->x = kotor_scene3d::bits_to_float(x); to->y = kotor_scene3d::bits_to_float(y); to->z = kotor_scene3d::bits_to_float(z);
}
}

// ================================================================ 004741A0 ProcessFaces (cdecl, 3 args, RET)
// esi = node; for i < [node+5C] (re-read each pass, 00474201): face = [node+58] + i*20h;
// out[i].facing = fstp dword(((f.y*l.y + f.z*l.z) + f.x*l.x) + f.d) (004741C0..004741E3);
// out[i]+4..+F = face+14..+1F (three DWORD moves, 004741E6..004741FA). Out stride 10h (004741D2).
// Note: the original keeps its loop counter in its own third-argument stack slot (004741AC / 0047420A); that slot
// belongs to this callee's frame for the call's duration and the caller (004744A0..004744B3) does not reuse it.
void __cdecl process_faces_004741a0(const Vector* light, void* face_records, void* node) {
    unsigned char* const n = static_cast<unsigned char*>(node);
    unsigned char* out = static_cast<unsigned char*>(face_records);                       // 004741B2 edx
    for (I i = 0; i < aurora_model::get<I>(n, 0x5C); ++i) {                                // 004741A5..004741B0 / 00474201..0047420E
        const unsigned char* const face = static_cast<const unsigned char*>(
            static_cast<void*>(aurora_model::ptr<unsigned char>(aurora_model::get<U>(n, 0x58)))) + static_cast<U>(i) * 0x20u;   // 004741C0..004741C7
        const double facing = ((W(aurora_model::get<float>(face, 4)) * W(light->y) +
                                W(aurora_model::get<float>(face, 8)) * W(light->z)) +
                               W(aurora_model::get<float>(face, 0)) * W(light->x)) +
                              W(aurora_model::get<float>(face, 0x0C));                      // 004741C3..004741E0
        aurora_model::put<float>(out, 0, S(facing));                                       // 004741E3 fstp [edx-10h]
        aurora_model::put<U>(out, 4, aurora_model::get<U>(face, 0x14));                    // 004741E6..004741EF
        aurora_model::put<U>(out, 8, aurora_model::get<U>(face, 0x18));                    // 004741F1..004741F4
        aurora_model::put<U>(out, 0x0C, aurora_model::get<U>(face, 0x1C));                 // 004741F7..004741FA
        out += 0x10;                                                                       // 004741D2 add edx,10h
    }
}

// ================================================================ 0046F580 ClipProjectionCaps (cdecl, 3 args, RET)
// ebx = arg2 (point), edi = arg1 (direction_scaled), esi = frustum plane.
// if plane(point) > 0: { if plane.d > 0 return 0; point = PlaneLineIntersection(plane, d, arg1, point) }
// if plane(arg1) > 0: arg1 = PlaneLineIntersection(plane, d, arg1, point); return 1.
I __cdecl clip_projection_caps_0046f580(Vector* direction_scaled, Vector* point, U frustum) {
    const Plane* const plane = plane_of(frustum);                                          // 0046F58C esi
    if (plane_distance(*point, *plane) > kZero_0073d700) {                                 // 0046F588..0046F5B4
        if (W(plane->d) > kZero_0073d700) return 0;                                        // 0046F5B6..0046F5CE (EAX = 0)
        Vector local;                                                                      // [esp+0Ch]
        const Vector* const r = plane_line_intersection_004ab250(&local, reinterpret_cast<const Vector*>(plane),
            plane->d, direction_scaled, point);                                            // 0046F5CF..0046F5DB
        copy_vector(point, r);                                                             // 0046F5E0..0046F5F2
    }
    if (plane_distance(*direction_scaled, *plane) > kZero_0073d700) {                      // 0046F5F5..0046F617
        Vector local;
        const Vector* const r = plane_line_intersection_004ab250(&local, reinterpret_cast<const Vector*>(plane),
            plane->d, direction_scaled, point);                                            // 0046F619..0046F625
        copy_vector(direction_scaled, r);                                                  // 0046F62A..0046F63A
    }
    return 1;                                                                              // 0046F63F
}

// ================================================================ 0046F650 ClipProjection (cdecl, 3 args, RET)
// esi = arg1 v, ecx = arg3 plane: if plane(v) > 0, v = PlaneLineIntersection(plane, d, arg2, v).
void __cdecl clip_projection_0046f650(Vector* v, const Vector* origin, U frustum) {
    const Plane* const plane = plane_of(frustum);                                          // 0046F650 ecx
    if (plane_distance(*v, *plane) > kZero_0073d700) {                                     // 0046F65C..0046F67E
        Vector local;                                                                      // [esp+4]
        const Vector* const r = plane_line_intersection_004ab250(&local, reinterpret_cast<const Vector*>(plane),
            plane->d, origin, v);                                                          // 0046F680..0046F690
        copy_vector(v, r);                                                                 // 0046F695..0046F6A5
    }
}

// ================================================================ 00426940 AllocateShadowMemory (cdecl, 1 arg, RET)
// Static block (guard 007A6958 bit 1: block = new(size) on first call; bit 2: capacity = size on first call);
// peak 00828050 = max(peak, size); if size > capacity: free(block); block = new(size); capacity = size.
// EAX = block (new or cached; 0042697D / 0042698F / 004269D4).
void* __cdecl gl_render_allocate_shadow_memory_00426940(I size) {
    void* block;
    if ((shadow_memory_guard_007a6958 & 1u) == 0) {                                        // 00426946..00426967 (test byte)
        shadow_memory_guard_007a6958 = shadow_memory_guard_007a6958 | 1u;                  // 00426969
        block = ::live_operator_new(static_cast<U>(size));                     // 0042696F..00426978 (006FC0D2)
        shadow_memory_block_007a6954 = block;                                              // 00426980
    } else {
        block = shadow_memory_block_007a6954;                                              // 0042698F
    }
    if ((shadow_memory_guard_007a6958 & 2u) == 0) {                                        // 00426994..004269A1
        shadow_memory_guard_007a6958 = shadow_memory_guard_007a6958 | 2u;                  // 004269A3
        shadow_memory_capacity_007a6950 = size;                                            // 004269A9
    }
    if (size > shadow_memory_peak_00828050) shadow_memory_peak_00828050 = size;            // 004269AF..004269B7 (signed)
    if (size > shadow_memory_capacity_007a6950) {                                          // 004269BD..004269C3 (signed)
        ::live_free(block);                                                    // 004269C5..004269C6 (006FC0CD)
        block = ::live_operator_new(static_cast<U>(size));                     // 004269CB..004269CC (006FC0D2)
        shadow_memory_block_007a6954 = block;                                              // 004269D4
        shadow_memory_capacity_007a6950 = size;                                            // 004269D9
    }
    return block;                                                                          // EAX
}

// ================================================================ 0045F9F0 AurTwoSidedStencilAvailable (no args, RET)
// cache 0078E540 (cold -1): on first call cache = (capabilities 007BB85C & mask 0078E4E0) != 0 (neg/sbb/neg).
U __cdecl aur_two_sided_stencil_available_0045f9f0() {
    U cache = two_sided_stencil_cache_0078e540;                                            // 0045F9F0
    if (cache == 0xFFFFFFFFu) {                                                            // 0045F9F5..0045F9F8
        cache = (kotor_renderer_gl_cells::capabilities_007bb85c & procs::mask_0078e4e0) != 0 ? 1u : 0u;   // 0045F9FA..0045FA09
        two_sided_stencil_cache_0078e540 = cache;                                          // 0045FA0B
    }
    return cache;                                                                          // EAX
}

// ================================================================ 004269F0 GLRender::BuildShadowArray (cdecl, 7 args, RET)
// Args (original offsets after the prologue): +58 light, +5C length (float*), +60 frustum, +64 direction_scaled,
// +68 point, +6C vertex count, +70 vertex positions (12-byte stride).
U __cdecl gl_render_build_shadow_array_004269f0(const Vector* light, float* length, U frustum,
    Vector* direction_scaled, Vector* point, I vertex_count, U pool) {
    U guard = build_shadow_guard_007a6980;                                                 // 004269F0
    if ((guard & 1u) == 0) {                                                               // 004269FD..004269FF (test dl,al)
        guard |= 1u; build_shadow_guard_007a6980 = guard;                                  // 00426A01..00426A03
        build_shadow_direction_007a6974 = Vector{0.0f, 0.0f, 0.0f};                        // 00426A08..00426A1C
    }
    if ((guard & 2u) == 0) {                                                               // 00426A26..00426A2C
        guard |= 2u; build_shadow_guard_007a6980 = guard;                                  // 00426A2E..00426A31
        for (Vector& v : build_shadow_pair_007a695c) v = Vector{0.0f, 0.0f, 0.0f};         // 00426A36..00426A4E (2 x 3 DWORD 0)
    }
    // Renumber marked vertices: every mark == 1 becomes the running 1-based index (00426A50..00426A78).
    I marked = 0;                                                                          // edi
    for (I v = 0; v < vertex_count; ++v) {
        if (shadow_vertex_marks_007a4820[v] == 1u) {                                       // 00426A60 cmp word,dx(=1)
            ++marked;                                                                      // 00426A6A
            shadow_vertex_marks_007a4820[v] = static_cast<std::uint16_t>(marked);          // 00426A6B
        }
    }
    const I pair_bytes = marked * 24;                                                      // 00426A7C..00426A7F (lea; shl 3)
    unsigned char* const array =
        static_cast<unsigned char*>(gl_render_allocate_shadow_memory_00426940(pair_bytes + 0x18));   // 00426A82..00426A8F
    shadow_vertex_array_007a47e8 = array;                                                  // 00426A96
    const unsigned char* source = reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(pool));   // 00426AA5..00426AAD
    for (I v = 0; v < vertex_count; ++v, source += 0x0C) {                                 // 00426AB1..00426C73
        const std::uint16_t mark = shadow_vertex_marks_007a4820[v];                        // 00426AB1
        const I slot = static_cast<I>(mark) * 6 - 6;                                       // 00426ABC..00426AC6 (DWORD index)
        if (mark == 0) continue;                                                           // 00426AB9 / 00426ACA jbe
        Vector& near_v = build_shadow_pair_007a695c[0];                                    // 007A695C
        Vector& far_v = build_shadow_pair_007a695c[1];                                     // 007A6968
        near_v.x = aurora_model::get<float>(source, 0);                                    // 00426AD4..00426AD6 (DWORD mov)
        near_v.y = aurora_model::get<float>(source, 4);                                    // 00426AE1..00426AE4
        near_v.z = aurora_model::get<float>(source, 8);                                    // 00426AEA..00426AED
        Vector delta;                                                                      // [esp+30h]
        delta.x = S(W(near_v.x) - W(light->x));                                            // 00426ADB..00426AFA
        delta.y = S(W(near_v.y) - W(light->y));                                            // 00426B02..00426B0C
        delta.z = S(W(near_v.z) - W(light->z));                                            // 00426B10..00426B19
        Vector unit;                                                                       // [esp+48h]
        copy_vector(&build_shadow_direction_007a6974, normalize_004ab180(&unit, &delta));  // 00426B1D..00426B3A
        const double l = W(*length);                                                       // 00426B36..00426B40 fld [arg2]
        const double dx_l = W(build_shadow_direction_007a6974.x) * l;                      // 00426B42..00426B4B (stays in st0)
        const float dy_l = S(W(build_shadow_direction_007a6974.y) * l);                    // 00426B4D..00426B55 fstp [esp+40h]
        const float dz_l = S(W(build_shadow_direction_007a6974.z) * l);                    // 00426B59..00426B61 fstp [esp+44h]
        far_v.x = S(dx_l + W(light->x));                                                   // 00426B65..00426B87
        far_v.y = S(W(dy_l) + W(light->y));                                                // 00426B71..00426B8D
        far_v.z = S(W(dz_l) + W(light->z));                                                // 00426B7C..00426B9B
        if (shadow_skip_clip_007a68e0 == 0) {                                              // 00426BA0..00426BA7
            clip_projection_0046f650(&near_v, light, frustum);                             // 00426BA9..00426BB4
            clip_projection_0046f650(&far_v, light, frustum);                              // 00426BB9..00426BC5
        }
        if (shadow_collapse_far_0078d994 != 0) {                                           // 00426BC8..00426BCF
            const double a = W(near_v.x) - W(far_v.x);                                     // 00426BD1..00426BD7
            const double b = W(near_v.y) - W(far_v.y);                                     // 00426BDD..00426BE3
            const double c = W(near_v.z) - W(far_v.z);                                     // 00426BE9..00426BEF
            const double along = (W(build_shadow_direction_007a6974.z) * c +
                                  b * W(build_shadow_direction_007a6974.y)) +
                                 a * W(build_shadow_direction_007a6974.x);                 // 00426BF5..00426C0F
            if (along > kZero_0073d700) copy_vector(&far_v, &near_v);                      // 00426C11..00426C3D
        }
        Vector* const target = reinterpret_cast<Vector*>(array + static_cast<U>(slot) * 4u);   // 00426C42..00426C4A
        copy_vector(&target[0], &near_v);                                                  // 00426C4D..00426C57 rep movsd x6
        copy_vector(&target[1], &far_v);
    }
    Vector* const tail = reinterpret_cast<Vector*>(array + pair_bytes);                    // 00426C79..00426C81
    copy_vector(&tail[0], direction_scaled);                                               // 00426C83..00426C92 (arg4)
    copy_vector(&tail[1], point);                                                          // 00426C95..00426CAC (arg5)
    return static_cast<U>(marked);                                                         // 00426CA9 EAX = edi
}

// ================================================================ 00426660 GLRender::RenderShadowArray (cdecl, 3 args, RET)
// Args: +1C marked vertex count (BuildShadowArray EAX), +20 edge count, +24 unused (never read).
void __cdecl gl_render_render_shadow_array_00426660(U array, I edges, U caps) {
    (void)caps;                                                                            // [esp+24h] not read
    std::uint16_t* indices;
    if ((render_shadow_guard_007a694c & 1u) == 0) {                                        // 00426676..00426685 (test byte)
        render_shadow_guard_007a694c = render_shadow_guard_007a694c | 1u;                  // 00426687
        indices = static_cast<std::uint16_t*>(::live_operator_new(0x3000u));   // 0042668D..0042669A (006FC0D2)
        render_shadow_indices_007a6948 = indices;                                          // 004266A2
    } else {
        indices = render_shadow_indices_007a6948;                                          // 004266B1
    }
    if (edges == 0) return;                                                                // 004266B6..004266BC je 00426920
    const U16 cap_near = static_cast<U16>(array + array);                                  // 004266C2..004266C7 esi = 2*n
    const U16 cap_far = static_cast<U16>(array + array + 1u);                              // 004266D2 edi = 2*n+1
    std::uint16_t* out = indices;                                                          // eax
    for (I e = 0; e < edges; ++e) {                                                        // 004266D5 jle / 0042676D jl
        const U a = shadow_edge_indices_007a69c0[e * 2];                                   // 004266E0 movzx [ebx*4+7A69C0]
        const U b = shadow_edge_indices_007a69c0[e * 2 + 1];                               // 004266E8 movzx [ebx*4+7A69C2]
        const U16 ca = static_cast<U16>(shadow_vertex_marks_007a4820[a] * 2u - 2u);        // 004266F0 / 00426708
        const U16 cb = static_cast<U16>(shadow_vertex_marks_007a4820[b] * 2u - 2u);        // 004266F8 / 0042670C
        if (e % 2 == 0) {                                                                  // 00426700..00426717 (signed remainder 0)
            *out++ = cap_near;                                                             // 00426719
            *out++ = ca;                                                                   // 0042671F
            *out++ = cb;                                                                   // 00426725
            *out++ = static_cast<U16>(ca + 1u);                                            // 0042672B..0042672C
            *out++ = static_cast<U16>(cb + 1u);                                            // 00426732..00426733
            *out++ = cap_far;                                                              // 00426739
        } else {
            *out++ = cap_far;                                                              // 0042673E
            *out++ = static_cast<U16>(cb + 1u);                                            // 00426744..00426747
            *out++ = static_cast<U16>(ca + 1u);                                            // 0042674D..00426750
            *out++ = cb;                                                                   // 00426756
            *out++ = ca;                                                                   // 0042675C
            *out++ = cap_near;                                                             // 00426762
        }
    }
    if (gl::VertexBufferAvailable_0045fc90() != 0)                                         // 00426775..0042677C
        procs::glBindBufferARB_007bb7b4(0x8892u, 0u);                                      // 0042677E..00426785 (ARRAY_BUFFER)
    gl::glVertexPointer_0073d2a8(3, 0x1406u, 0x0C, shadow_vertex_array_007a47e8);          // 0042678B..0042679A
    const I count = edges * 6;                                                             // 0042684D..00426851 / 004268BF..004268C8
    if (shadow_two_sided_enabled_007a68dc != 0 && aur_two_sided_stencil_available_0045f9f0() != 0) {   // 004267A0..004267B4
        gl::glDisable_0073d22c(0x0B44u);                                                   // 004267BA..004267C5 (CULL_FACE)
        gl::glEnable_0073d390(0x8910u);                                                    // 004267C7..004267CC (STENCIL_TEST_TWO_SIDE_EXT)
        procs::glActiveStencilFaceEXT_007bb624(0x405u);                                    // 004267D2..004267D7 (BACK)
        gl::glStencilOp_0073d270(0x1E00u, 0x1E00u, 0x8508u);                               // 004267DD..004267F2 (KEEP, KEEP, DECR_WRAP)
        gl::glStencilMask_0073d278(0xFFFFFFFFu);                                           // 004267F4..004267FC
        gl::glStencilFunc_0073d298(0x207u, 0, 0xFFFFFFFFu);                                // 004267FE..00426807 (ALWAYS)
        procs::glActiveStencilFaceEXT_007bb624(0x404u);                                    // 0042680D..00426812 (FRONT)
        gl::glStencilOp_0073d270(0x1E00u, 0x1E00u, 0x8507u);                               // 00426818..00426827 (KEEP, KEEP, INCR_WRAP)
        gl::glStencilMask_0073d278(0xFFFFFFFFu);                                           // 00426829..0042682B
        if (gl::VertexBufferAvailable_0045fc90() != 0)                                     // 0042682D..00426834
            procs::glBindBufferARB_007bb7b4(0x8893u, 0u);                                  // 00426836..0042683D (ELEMENT_ARRAY_BUFFER)
        gl::glDrawElements_0073d258(5u, count, 0x1403u, indices);                          // 00426843..00426856 (TRIANGLE_STRIP, USHORT)
        gl::glEnable_0073d390(0x0B44u);                                                    // 0042685C..00426861
        gl::glFrontFace_0073d274(0x901u);                                                  // 00426867..0042686C (CCW)
        gl::glDisable_0073d22c(0x8910u);                                                   // 00426872..00426877
    } else {
        gl::glFrontFace_0073d274(0x900u);                                                  // 0042687E..00426883 (CW)
        gl::glStencilOp_0073d270(0x1E00u, 0x1E02u, 0x1E00u);                               // 00426889..00426898 (KEEP, INCR, KEEP)
        if (gl::VertexBufferAvailable_0045fc90() != 0)                                     // 0042689E..004268A5
            procs::glBindBufferARB_007bb7b4(0x8893u, 0u);                                  // 004268A7..004268AE
        gl::glDrawElements_0073d258(5u, count, 0x1403u, indices);                          // 004268B4..004268CD
        gl::glFrontFace_0073d274(0x901u);                                                  // 004268CF..004268D4 (CCW)
        gl::glStencilOp_0073d270(0x1E00u, 0x1E03u, 0x1E00u);                               // 004268DA..004268E9 (KEEP, DECR, KEEP)
        if (gl::VertexBufferAvailable_0045fc90() != 0)                                     // 004268EF..004268F6
            procs::glBindBufferARB_007bb7b4(0x8893u, 0u);                                  // 004268F8..004268FF
        gl::glDrawElements_0073d258(5u, count, 0x1403u, indices);                          // 00426905..0042690E
    }
    r4_lowers_cells::render_counts_00827fa8[2] =
        r4_lowers_cells::render_counts_00827fa8[2] + static_cast<U>(edges) * 8u;           // 00426910..00426919 (00827FB0 += edges*8)
}
