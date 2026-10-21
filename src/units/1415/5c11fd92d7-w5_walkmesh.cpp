// W5 walkmesh queries (G16): CSWRoomSurfaceMesh / CSWCollisionMesh / CSWSRoom query side.
// Reconstructed from swkotor.exe (GOG, SHA-256 9c10e045...91435), read-only disassembly.
// Load side (room_mesh_binary_005807c0, mesh-load-v22) is reused unchanged; this file only reads it.
#include "4f99fc5850-w5_walkmesh.hpp"
#ifdef W5_WALK_TRACE
#include <cstdio>
#endif
#include <windows.h>

using namespace w5;

extern "C" {
float w5_aabb_all_elapsed_0083283c = 0.0f;
std::int64_t w5_query_timer_frequency_00832848 = 0;
U w5_query_timer_guard_00832850 = 0;
}

namespace {
constexpr float k_zero_0073d700 = 0.0f;           // .rdata 0073D700
constexpr float k_no_height_00749448 = -9.0f;     // .rdata 00749448 (C1100000)
constexpr float k_nudge_0x3a83126f = 0.001f;      // immediate 3A83126F
constexpr U k_up_1000 = 0x447a0000u;              // 1000.0 immediate
constexpr U k_down_1000 = 0xc47a0000u;            // -1000.0 immediate
constexpr double k_side_zero_0073edf8 = 0.0;      // .rdata 0073EDF8 (qword 0.0)
constexpr I k_walk_iterations_200 = 0x200;

inline Vector vec_bits(U x, U y, U z) { Vector v; std::memcpy(&v.x, &x, 4); std::memcpy(&v.y, &y, 4); std::memcpy(&v.z, &z, 4); return v; }
inline const Vector* normal_of(void* mesh, I face) {
    return reinterpret_cast<const Vector*>(at<unsigned char*>(mesh, mesh::normals_68) + face * 12);
}
inline float distance_of(void* mesh, I face) { return reinterpret_cast<const float*>(at<unsigned char*>(mesh, mesh::distances_6c))[face]; }

// x87 "fld a; fcomp b; fnstsw; test ah,1; jne" : taken when a < b or unordered.
inline bool below_or_unordered(float a, float b) { return !(a >= b); }
// "test ah,41h; jp skip" : falls through (store) when a <= b and ordered.
inline bool at_most_ordered(float a, float b) { return a <= b; }
}  // namespace

extern "C" {

// 004A9810..004A9844 PlaneHeightAtPoint. ECX = normal; fcomp normal.z vs 0.0; test ah,44h; jp -> compute.
double __cdecl w5_plane_height_at_point_004a9810(const Vector* normal, float distance, const Vector* point) {
    if (static_cast<double>(normal->z) == static_cast<double>(k_zero_0073d700)) return k_zero_0073d700; // 004A9817..004A982A (NaN computes)
    const double yy = static_cast<double>(normal->y) * point->y;           // 004A982F..004A9832
    const double xx = static_cast<double>(normal->x) * point->x;           // 004A9835..004A9837
    return -(((yy + xx) + distance) / normal->z);                          // 004A9839..004A9842
}

// 00580340..005803B8 QueryTimer. Local static (guard bit 0 at 00832850) caches the frequency at 00832848.
std::int64_t __cdecl w5_query_timer_00580340() {
    if ((w5_query_timer_guard_00832850 & 1u) == 0) {                       // 00580340..00580350
        w5_query_timer_guard_00832850 |= 1u;                               // 00580352..0058035F
        LARGE_INTEGER f; QueryPerformanceFrequency(&f);                    // 00580365 [0073D1AC]
        w5_query_timer_frequency_00832848 = f.QuadPart;                    // 0058036B..00580379
    }
    LARGE_INTEGER c; QueryPerformanceCounter(&c);                          // 00580384 [0073D1A8]
    const std::int64_t scaled = static_cast<std::int64_t>(static_cast<std::uint64_t>(c.QuadPart) * 1000000u); // __allmul (wraps)
    return scaled / w5_query_timer_frequency_00832848;                     // __alldiv (signed)
}

// 005805E0..0058061D CSWRoomSurfaceMesh::ComputeHeight(face, point). Local (x, y, -9.0); z of point unused.
double __fastcall w5_surface_mesh_face_height_005805e0(void* mesh, void*, I face, Vector point) {
    Vector local;                                                          // [esp]
    std::memcpy(&local.x, &point.x, 4);                                    // 005805E3..005805EB
    std::memcpy(&local.y, &point.y, 4);                                    // 005805E7..005805EF
    local.z = k_no_height_00749448;                                        // 00580602 C1100000
    return w5_plane_height_at_point_004a9810(normal_of(mesh, face), distance_of(mesh, face), &local); // 005805F3..00580615
}

// 00581610..00581714 CSWRoomSurfaceMesh::CheckAABBAll(from, to, hit). Same shape as CheckAABBWalkable
// (00581530) but uses the all-material mask (+E4) and times the query into 0083283C.
U __fastcall w5_surface_mesh_check_aabb_all_00581610(void* mesh, void*, Vector from, Vector to, SurfaceHit* hit) {
    if (at<U>(mesh, mesh::aabb_count_b0) == 0) return 0;                  // 00581616..00581626
    const std::int64_t start = w5_query_timer_00580340();                 // 0058162C (EBP:EBX)
    hit->mask_0 = at<U>(mesh, mesh::all_mask_e4);                         // 00581638 / 00581649
    I result = surface_mesh_check_aabb_node_00580920(mesh, nullptr, at<I>(mesh, mesh::aabb_root_d4), &from, &to, hit); // 00581655
    if (hit->face_c == -1) {                                               // 0058165A..0058165E
        Vector dir = vec_bits(0x3f800000u, 0x3f800000u, 0u);               // [esp+10] (1,1,0)
        Vector step;                                                       // [esp+1C]
        normalize_004ab180(&step, &dir);                                   // 00581682
        vector_scale_assign_004a97a0(&step, nullptr, k_nudge_0x3a83126f);  // 00581693
        vector_add_assign_004a9740(&from, nullptr, &step);                 // 005816A1
        vector_add_assign_004a9740(&to, nullptr, &step);                   // 005816AF
        result = surface_mesh_check_aabb_node_00580920(mesh, nullptr, at<I>(mesh, mesh::aabb_root_d4), &from, &to, hit); // 005816C8
    }
    if (result > 0) {                                                      // 005816CD..005816CF (signed)
        copy_vector(&hit->point_10, &to);                                  // 005816D1..005816EC (`to` as left by callees)
        hit->hit_8 = 1;                                                    // 005816E5
    }
    const std::int64_t elapsed = w5_query_timer_00580340() - start;      // 005816EF..005816F6
    w5_aabb_all_elapsed_0083283c = static_cast<float>(elapsed);           // 00581700 fild qword / fstp dword
    return hit->hit_8;                                                     // 0058170A
}

// 00581720..00581920 CSWRoomSurfaceMesh::FindTriangle(point*, walkableOnly, nudge dir by value).
I __fastcall w5_surface_mesh_find_triangle_00581720(void* mesh, void*, const Vector* point, I walkable, Vector dir) {
    Vector local;                                                          // [esp+2C] (local.z is never read)
    collision_mesh_world_to_local_00596a00(mesh, nullptr, &local, point);  // 00581733
    U lx, ly; std::memcpy(&lx, &local.x, 4); std::memcpy(&ly, &local.y, 4);
    Vector from = vec_bits(lx, ly, k_up_1000);                             // [esp+14] (x, y, 1000)
    Vector to = vec_bits(lx, ly, k_down_1000);                             // [esp+8]  (x, y, -1000)
    Vector step = vec_bits(0, 0, 0);                                       // [esp+20]
    SurfaceHit hit;                                                        // [esp+38]
    hit.mask_0 = 0xffffffffu; hit.order_4 = 0; hit.hit_8 = 0; hit.face_c = -1;
    hit.point_10 = step; hit.bit_1c = 0;                                   // 00581786..005817BE
    if (walkable) w5_adapt_check_aabb_walkable_00581530(mesh, nullptr, from, to, &hit); // 005817FD
    else w5_surface_mesh_check_aabb_all_00581610(mesh, nullptr, from, to, &hit);        // 0058183D
    if (hit.face_c != -1) return hit.face_c;                               // 00581842..00581849 -> 0058191B
    normalize_004ab180(&step, &dir);                                       // 00581859
    vector_scale_assign_004a97a0(&step, nullptr, k_nudge_0x3a83126f);      // 0058186A
    vector_add_assign_004a9740(&from, nullptr, &step);                     // 00581878
    vector_add_assign_004a9740(&to, nullptr, &step);                       // 00581886
    if (walkable) w5_adapt_check_aabb_walkable_00581530(mesh, nullptr, from, to, &hit); // 005818C8
    else w5_surface_mesh_check_aabb_all_00581610(mesh, nullptr, from, to, &hit);        // 00581912
    return hit.face_c;                                                     // 005818CD / 00581917
}

// 00581B30..00581BC4 CSWRoomSurfaceMesh::ComputeHeight(point, walkableOnly). Nudge direction (1,1,0).
double __fastcall w5_surface_mesh_compute_height_00581b30(void* mesh, void*, Vector point, I walkable) {
    const Vector dir = vec_bits(0x3f800000u, 0x3f800000u, 0u);             // 00581B3B..00581B68
    const I face = w5_surface_mesh_find_triangle_00581720(mesh, nullptr, &point, walkable, dir); // 00581B73
    if (face == -1) return k_no_height_00749448;                           // 00581B78..00581B87
    Vector local;                                                          // [esp+4]
    std::memcpy(&local.x, &point.x, 4); std::memcpy(&local.y, &point.y, 4); // 00581B8A..00581B96 (world x, y)
    local.z = k_no_height_00749448;                                        // 00581BA2
    return w5_plane_height_at_point_004a9810(normal_of(mesh, face), distance_of(mesh, face), &local); // 00581BB8
}

// 00579430..00579462 CSWSRoom::ComputeHeight(point, walkableOnly): mesh at room+3C.
double __fastcall w5_room_compute_height_00579430(void* room, void*, Vector point, I walkable) {
    void* const surface = at<void*>(room, 0x3c);                           // 00579430
    if (surface == nullptr) return k_zero_0073d700;                        // 00579433..0057943B
    return w5_surface_mesh_compute_height_00581b30(surface, nullptr, point, walkable); // 0057945D
}

// 005970E0..0059711B CSWCollisionMesh::GetVertex(index, out). Signed index check against +50.
void __fastcall w5_collision_mesh_get_vertex_005970e0(void* mesh, void*, I index, Vector* out) {
    const unsigned char* vertices = at<unsigned char*>(mesh, mesh::vertices_54);
    if (vertices == nullptr || index >= at<I>(mesh, mesh::vertex_count_50)) return; // 005970E0..005970F1
    Vector world;                                                          // [esp]
    const Vector* r = collision_mesh_local_to_world_00596aa0(mesh, nullptr, &world, reinterpret_cast<const Vector*>(vertices + index * 12)); // 005970FF
    copy_vector(out, r);                                                   // 00597104..00597115 (DWORD moves)
}

// 00597090..005970D5 CSWCollisionMesh::GetVertex(index, x*, y*, z*). fld/fstp dword per component.
void __fastcall w5_collision_mesh_get_vertex_xyz_00597090(void* mesh, void*, I index, float* x, float* y, float* z) {
    const unsigned char* vertices = at<unsigned char*>(mesh, mesh::vertices_54);
    if (vertices == nullptr || index >= at<I>(mesh, mesh::vertex_count_50)) return; // 00597090..005970A1
    Vector world;
    collision_mesh_local_to_world_00596aa0(mesh, nullptr, &world, reinterpret_cast<const Vector*>(vertices + index * 12)); // 005970AF
    *x = world.x; *y = world.y; *z = world.z;                              // 005970B4..005970D0
}

// 00580620..005806B2 CSWRoomSurfaceMesh::GetTriangleAdjacency(unused, face, a*, b*, c*).
// Each edge value is -1 or edge/3 (signed truncation, magic 55555556h).
void __fastcall w5_surface_mesh_triangle_adjacency_00580620(void* mesh, void*, I, I face, I* a, I* b, I* c) {
    if (face >= at<I>(mesh, mesh::face_count_58)) return;                  // 00580624..00580627 (signed)
    const I* adjacency = at<I*>(mesh, mesh::adjacency_88);
    if (adjacency == nullptr) return;                                      // 0058062D..00580635
    const I* row = adjacency + face * 3;
    *a = row[0] == -1 ? -1 : row[0] / 3;                                   // 00580638..00580659
    *b = row[1] == -1 ? -1 : row[1] / 3;                                   // 0058065B..00580680
    *c = row[2] == -1 ? -1 : row[2] / 3;                                   // 00580682..005806AF
}

// 00584220..00584861 CSWRoomSurfaceMesh::ClippedLineSegmentWalkable.
// Walks the walkable triangles from `from` toward `to` (at most 200h steps), tracking the min/max
// height crossed in *z_min / *z_max. Returns 1 when `to` is reached inside a walkable face or the
// walk leaves the mesh through an open edge (adjacency -1); 0 when blocked.
U __fastcall w5_surface_mesh_clipped_segment_walkable_00584220(void* mesh, void*, const Vector* from,
    const Vector* to, Vector* clip_from, Vector* clip_to, float* z_min, float* z_max, U* edge) {
    if (!w5_adapt_compute_clipped_line_segment_00583e30(mesh, nullptr, *from, *to, clip_from, clip_to, edge)) // 00584294
        return 0;                                                          // -> 00584742
    I iteration = 0;                                                       // [E+28]
    Vector tri[3];                                                         // [E+A0] zeroed 005842A1..005842BF
    std::memset(tri, 0, sizeof tri);
    const double dz = static_cast<double>(to->z) - from->z;                // 005842C1..005842C6
    const double dy = static_cast<double>(to->y) - from->y;                // 005842CE..005842D1
    const double dx = static_cast<double>(to->x) - from->x;                // 005842D7..005842DD
    Vector cur; copy_vector(&cur, from);                                   // [E+10] raw copy
    Vector seg;                                                            // [E+1C] direction, later reused
    seg.x = static_cast<float>(dx); seg.y = static_cast<float>(dy);        // 005842EB / 005842F5
    Vector target; copy_vector(&target, to);                               // [E+4C]
    seg.z = static_cast<float>(dz);                                        // 00584300
    Vector hit_point = vec_bits(0, 0, 0);                                  // [E+58]
    Vector hit_normal = vec_bits(0, 0, 0);                                 // [E+70]
    I face = w5_surface_mesh_find_triangle_00581720(mesh, nullptr, &cur, 1, seg); // 00584364
    if (face == -1) return 0;                                              // 0058436C..00584370
    if (face < 0) return 0;                                                // 00584376..00584378 -> 00584815 (!= -1)
    // [E+64] vertex indices: only refreshed when face < face count; an out-of-range face keeps the
    // previous (initially indeterminate) indices, preserved here as the original frame slot.
    I index[3];
    index[0] = index[1] = index[2] = 0x7fffffff;  // original: indeterminate; positive fill makes GetVertex reject (no OOB)
    auto refresh_indices = [&](I f) {
        if (f < at<I>(mesh, mesh::face_count_58)) {                        // 0058437E / 0058444A (signed)
            const U* row = at<U*>(mesh, mesh::faces_60) + f * 3;
            std::memcpy(index, row, 12);
        }
    };
    refresh_indices(face);
    for (int k = 0; k < 3; ++k) w5_collision_mesh_get_vertex_005970e0(mesh, nullptr, index[k], &tri[k]); // 005843A0..005843C4
    // 005843C6..00584418: vertical probe through cur.x/cur.y.
    Vector probe_top; probe_top.x = cur.x; probe_top.y = cur.y; probe_top.z = flt(k_up_1000);     // [E+34]
    Vector probe_bottom; probe_bottom.x = cur.x; probe_bottom.y = cur.y; probe_bottom.z = flt(k_down_1000); // [E+1C]
    if (polyhit_004ab9d0(tri, 3, &probe_top, &probe_bottom, &hit_point, &hit_normal) != 0) {     // 00584418
        U z; std::memcpy(&z, &hit_point.z, 4);
        std::memcpy(&cur.z, &z, 4);                                        // 0058442A [E+18]
        std::memcpy(&clip_from->z, &z, 4);                                 // 0058442E [arg3+8]
    }
    I adjacent[3];                                                         // [E+40]
    float xs[3], ys[3], zs[3];                                             // [E+7C] [E+94] [E+C4]
    I inside[3];                                                           // [E+88]
    for (;;) {
        if (iteration >= k_walk_iterations_200) return 0;                  // 00584431..0058443A -> 005846FB (debug resref print elided: output-free)
        ++iteration;                                                       // 00584449..0058444C
        refresh_indices(face);                                             // 00584450..0058446B
        w5_surface_mesh_triangle_adjacency_00580620(mesh, nullptr, 0, face, &adjacent[0], &adjacent[1], &adjacent[2]); // 00584483
        for (int k = 0; k < 3; ++k) {                                      // 00584491..005844C7
            w5_collision_mesh_get_vertex_xyz_00597090(mesh, nullptr, index[k], &xs[k], &ys[k], &zs[k]);
            w5_collision_mesh_get_vertex_005970e0(mesh, nullptr, index[k], &tri[k]);
        }
        if (iteration == 1) {                                              // 005844C9..005844E8
            std::memcpy(z_max, &cur.z, 4);
            std::memcpy(z_min, &cur.z, 4);
        } else if (!below_or_unordered(cur.z, *z_max)) {                   // 005844EA..005844FC
            std::memcpy(z_max, &cur.z, 4);
        } else if (at_most_ordered(cur.z, *z_min)) {                       // 00584506..00584518
            std::memcpy(z_min, &cur.z, 4);
        }
        I count = 0;                                                       // 00584522 (EDI)
        for (int k = 0; k < 3; ++k) {                                      // 00584530..0058459B
            const int j = (k + 1) % 3;
            const double a = (static_cast<double>(ys[k]) - ys[j]) * (static_cast<double>(to->x) - xs[k]);
            const double b = (static_cast<double>(xs[j]) - xs[k]) * (static_cast<double>(to->y) - ys[k]);
            if (a + b >= k_side_zero_0073edf8) { inside[k] = 1; ++count; }   // 00584570..0058457B: C0 clear (>= 0, ordered)
            else inside[k] = 0;
        }
#ifdef W5_WALK_TRACE
        std::printf("TRACE it=%d face=%d count=%d inside=%d%d%d adj=%d,%d,%d cur=(%.4f,%.4f,%.4f)\n", iteration, face, count, inside[0], inside[1], inside[2], adjacent[0], adjacent[1], adjacent[2], cur.x, cur.y, cur.z);
#endif
        if (count == 0) return 0;                                          // 0058459D..0058459F
        if (count == 3) {                                                  // 005845A5..005845A8 -> 0058475F
            Vector end_top; copy_vector(&end_top, &target);                // [E+1C]
            Vector end_bottom; copy_vector(&end_bottom, &target);          // [E+40]
            end_top.z = flt(k_up_1000);                                    // 005847A1 [E+24]
            end_bottom.z = flt(k_down_1000);                               // 005847A9 [E+48]
            float z;
            if (polyhit_004ab9d0(tri, 3, &end_top, &end_bottom, &hit_point, &hit_normal) != 0) { // 005847B1
                std::memcpy(&clip_to->z, &hit_point.z, 4);                 // 005847BD..005847CC [arg4+8]
                z = hit_point.z;
            } else {
                z = target.z;                                              // 005847D1 [E+54]
            }
            if (!below_or_unordered(z, *z_max)) { *z_max = z; return 1; }  // 005847D5..005847F9
            if (at_most_ordered(z, *z_min)) *z_min = z;                    // 005847E7..005847F9
            return 1;                                                      // 00584805 / 005847FB
        }
        if (count == 2) {                                                  // 005845AE..00584649
            Vector crossing = vec_bits(0, 0, 0);                           // [E+34]
            for (int k = 2; k >= 0; --k) {
                const int j = (k + 1) % 3;
                if (inside[k] != 0) continue;
                if (intersect_line_segments_2d_004ac340(&cur, &target, &tri[k], &tri[j], &crossing) == 0) return 0; // 00584611..0058461B
                face = adjacent[k];                                        // 00584621..0058462D
                copy_vector(&cur, &crossing);                              // 00584635..0058463D
            }
        } else if (count == 1) {                                           // 0058464E..005846DF
            Vector crossing = vec_bits(0, 0, 0);                           // [E+1C]
            U found = 0;                                                   // EBX
            for (int k = 2; k >= 0 && found == 0; --k) {
                const int j = (k + 1) % 3;
                if (inside[k] != 0) continue;
                if (intersect_line_segments_2d_004ac340(&cur, &target, &tri[k], &tri[j], &crossing) == 1) { // 005846A6..005846B1
                    face = adjacent[k];
                    found = 1;
                    copy_vector(&cur, &crossing);
                }
            }
            if (found == 0) return 0;                                      // 005846DD..005846DF
        }
        if (face >= 0) continue;                                           // 005846E1..005846E7
        if (iteration >= k_walk_iterations_200) return 0;                  // 005846ED..005846F5 -> 005846FB
        if (face != -1) return 0;                                          // 00584815..00584818
        if (!below_or_unordered(cur.z, *z_max)) { std::memcpy(z_max, &cur.z, 4); return 1; } // 0058481E..0058483D
        if (at_most_ordered(cur.z, *z_min)) std::memcpy(z_min, &cur.z, 4); // 00584842..0058485A
        return 1;                                                          // 0058485C
    }
}

// 00579670 CSWSRoom::ClippedLineSegmentWalkable: mov ecx,[ecx+3Ch]; jmp 00584220 (no null test).
U __fastcall w5_room_clipped_segment_walkable_00579670(void* room, void*, const Vector* from, const Vector* to,
    Vector* clip_from, Vector* clip_to, float* z_min, float* z_max, U* edge) {
    return w5_surface_mesh_clipped_segment_walkable_00584220(at<void*>(room, 0x3c), nullptr, from, to, clip_from,
        clip_to, z_min, z_max, edge);
}

}  // extern "C"
