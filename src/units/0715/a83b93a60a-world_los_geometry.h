#pragma once
// lane-world-los geometry declarations (included by world_los.h only). Owner: fork GEOM.
extern "C" {
using kotor_world_los::U; using kotor_world_los::I; using kotor_world_los::Vector; using kotor_world_los::Quaternion;
// geometry (geometry.cpp)
// 00423BF0 vectorComponentMult: cdecl, EAX = out
Vector* __cdecl vector_component_mult_00423bf0(Vector* out, const Vector* v, float scale);
// 004AAB50 "IntersectCircle2d" (3-D quadratic segment/sphere test): cdecl, EAX = root count 0/1/2; flag unused
I __cdecl intersect_circle_2d_004aab50(const Vector* center, float radius, const Vector* a, const Vector* b,
    Vector* out1, Vector* out2, bool flag);
// 004AB9D0 polyhit: cdecl, EAX 0/1; hit_out / normal_out written only on success (each may be null)
I __cdecl polyhit_004ab9d0(const Vector* points, I count, const Vector* start, const Vector* end,
    Vector* hit_out, Vector* normal_out);
// 004ABF00 DistanceBetweenLines(p1, dir1, p2, dir2, closest1_out, closest2_out): cdecl, ST0 (float-rounded)
double __cdecl distance_between_lines_004abf00(const Vector* p1, const Vector* p2, const Vector* p3,
    const Vector* p4, Vector* p5, Vector* p6);
// 004AC340 IntersectLineSegments2d: cdecl, EAX 0/1 (setne of a local flag byte)
I __cdecl intersect_line_segments_2d_004ac340(const Vector* p1, const Vector* p2, const Vector* p3,
    const Vector* p4, Vector* p5);

// 3D-core-assigned geometry (geometry_core_handover.cpp; SUBLANES core address_list, not yet authored there).
// Names match existing consumer declarations: lane-3d-anim anim3d.h:256, lane-3d-bsp bsp.h:266/278/282,
// lane-3d-emitter emitter.h:185/191.
double __cdecl aurora_fabs_00423b70(float value);                                                    // ST0
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);
void __cdecl relative_spacing_004aa160(const Vector* point, const Vector* box_min, const Vector* box_max,
    unsigned char* out_code);
I __cdecl box_intersect_004aa610(const Vector* line_start, const Vector* line_end, const Vector* box_min,
    const Vector* box_max);
// 004AB130 Vector::Normalize: owner lane-3d-model model_skin.cpp:32 (declaration only here)
void __fastcall vector_normalize_004ab130(Vector* self, void*);
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance,
    const Vector* a, const Vector* b);
}
