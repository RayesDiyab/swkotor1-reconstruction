// m46_segment_project.cpp - lane-m46-3dmodel: 004AB360 segmentproject(out, a, b, point) (callee of the F5 P2P Grav mover
// 00493AE0 at 004944D1..004944E8 / 0049451D). cdecl, caller pops 10h, returns `out` in EAX. Authored from the pinned
// GOG swkotor.exe (no donor found in the 2026-10-02/06 trees or lane objects).
// D30-FINAL: x87 values are `double` while on the stack; `float` exactly where the original stores with fstp dword:
// the three difference components (004AB3CE/004AB3DC/004AB3F5), the x and y products t*dx / t*dy (004AB48F/004AB499)
// and the three output components (004AB4B2..004AB4B7). The z product t*dz is NOT stored before its add (004AB49D..A1).
// Function-static vector: guard bit 0 of 00831F44 and Vector 00831F38..40 (BSS). The exe references these four words
// only in this function (8 sites, all listed below) and never reads the vector back; the stores are reproduced.
// The guard is the original non-thread-safe test/or pattern (not a C++ magic static).
#include "4ac9e30747-emitter.h"   // kotor_scene3d::Vector; aurora_vector_equal_004aa980 (M45 owner _aurora_vector_equal_004aa980)

extern "C" {
volatile kotor_scene3d::U segment_project_guard_00831f44 = 0u;          // BSS
volatile kotor_scene3d::U segment_project_delta_00831f38[3] = {0u, 0u, 0u};   // BSS Vector (x,y,z float bits)

kotor_scene3d::Vector* __cdecl segment_project_004ab360(kotor_scene3d::Vector* out, const kotor_scene3d::Vector* a,
                                                       const kotor_scene3d::Vector* b, const kotor_scene3d::Vector* point) {
    using kotor_scene3d::float_to_bits;
    if (aurora_vector_equal_004aa980(a, b) != 0) {                      // 004AB36D..004AB379 (push b; push a; call)
        out->x = a->x; out->y = a->y; out->z = a->z;                    // 004AB37B..004AB38F (DWORD copies)
        return out;                                                     // 004AB396 (EAX = out)
    }
    if ((segment_project_guard_00831f44 & 1u) == 0u) {                  // 004AB397..004AB3A4
        segment_project_guard_00831f44 = segment_project_guard_00831f44 | 1u;   // 004AB3A6
        segment_project_delta_00831f38[0] = 0u;                         // 004AB3AC
        segment_project_delta_00831f38[1] = 0u;                         // 004AB3B6
        segment_project_delta_00831f38[2] = 0u;                         // 004AB3C0
    }
    const float dx = static_cast<float>(static_cast<double>(b->x) - static_cast<double>(a->x));   // 004AB3CA..004AB3CE
    const float dy = static_cast<float>(static_cast<double>(b->y) - static_cast<double>(a->y));   // 004AB3D6..004AB3DC
    const float dz = static_cast<float>(static_cast<double>(b->z) - static_cast<double>(a->z));   // 004AB3E0..004AB3F5
    segment_project_delta_00831f38[0] = float_to_bits(dx);             // 004AB3EA
    segment_project_delta_00831f38[1] = float_to_bits(dy);             // 004AB3EF
    segment_project_delta_00831f38[2] = float_to_bits(dz);             // 004AB405
    // 004AB3F9..004AB41D: ((dx*dx + dz*dz) + dy*dy), wide.
    const double len2 = (static_cast<double>(dx) * dx + static_cast<double>(dz) * dz) + static_cast<double>(dy) * dy;
    double t = len2;                                                    // len2 == 0: ST0 (= 0.0) flows on as t
    // 004AB41F..004AB42A: fcom 0.0f; test ah,44h; jnp -> skip the division only when len2 == 0 (C3 alone).
    if (!(len2 == 0.0)) {
        const double px = static_cast<double>(point->x) - a->x;         // 004AB430..004AB432
        const double py = static_cast<double>(point->y) - a->y;         // 004AB434..004AB437
        const double pz = static_cast<double>(point->z) - a->z;         // 004AB43A..004AB43D
        t = ((pz * dz + px * dx) + py * dy) / len2;                     // 004AB440..004AB452 (fdivrp: dot / len2)
    }
    // 004AB454..004AB461: fld 1.0f; fcomp t; test ah,5; jp -> t = 1.0 only when 1.0 < t (C0 alone).
    if (1.0 < t) {
        t = 1.0;                                                        // 004AB463..004AB465
    } else if (0.0 > t) {                                               // 004AB46D..004AB47A: test ah,41h; jne keeps t
        t = 0.0;                                                        // 004AB47C..004AB47E
    }                                                                   // (NaN keeps t: both compares unordered)
    const float tx = static_cast<float>(dx * t);                        // 004AB484..004AB48F fstp [esp+10h]
    const float ty = static_cast<float>(dy * t);                        // 004AB493..004AB499 fstp [esp+14h]
    const double zz = t * dz + a->z;                                    // 004AB49D..004AB4A1 (wide t*dz + az)
    out->x = static_cast<float>(static_cast<double>(tx) + a->x);       // 004AB4AB..004AB4B2
    out->y = static_cast<float>(static_cast<double>(ty) + a->y);       // 004AB4A4..004AB4B4
    out->z = static_cast<float>(zz);                                    // 004AB4B7
    return out;                                                         // 004AB488 (EAX = out), 004AB4BD ret
}
}
