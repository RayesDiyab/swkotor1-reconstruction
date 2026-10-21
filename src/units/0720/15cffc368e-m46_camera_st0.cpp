// lane-m46-3dcore fork D: the four CAurCamera slots whose original result is returned in ST0
// (0045C170 GetYaw, 0045C1A0 GetPitch, 0045C1D0 GetDist, 0045D300 HitCheckViewVolumeNearest).
// The generated raw-word declarations in the stage (vtables2_gen.h CameraVTable +64/+68/+6C/+4C) give them an EAX
// result `U`; that contract is wrong for these bodies (fld at 0045C18C/0045C1BC/0045C1EC/0045D477, no EAX write).
// This unit therefore does not include scene3d.h (C2733 would reject the corrected return type) and the header diff
// in parts/D.json changes the four declarations + CameraVTable fields to `double` (ST0). Until that diff lands the
// typed table entries in vtables2.cpp disagree with these definitions in return channel only (no caller in the
// staged cohort reads these slots).
// Conventions as m46_camera.cpp (D1, D21, D30-FINAL, D41).
#include <cstdint>
#include <cstring>

namespace kotor_m46_st0 {
using U = std::uint32_t;
using I = std::int32_t;
struct Vector { float x, y, z; };                 // == kotor_scene3d::Vector (12 bytes)
struct Plane { float a, b, c, d; };               // == kotor_scene3d::Plane (16 bytes)
struct PlaneList { Plane* data; I count; I capacity; };   // == kotor_scene3d::PlaneList (Camera+1F8)
template<class T> inline T& field(void* object, U offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
// CAurBehavior table view: +1C query (thiscall, RET 0, EAX tested) used at 0045C17F / 0045C1AF / 0045C1DF.
struct BehaviorQuerySlots {
    const void* o00[0x1C / 4];
    U (__fastcall* query_1c)(void* self, void* unused_edx);
};
static_assert(sizeof(BehaviorQuerySlots) == 0x20, "behavior +1C");
inline const BehaviorQuerySlots& behavior_slots(void* behavior) {
    return *static_cast<const BehaviorQuerySlots*>(*static_cast<void* const volatile*>(behavior));
}
constexpr float kZero = 0.0f;   // [0073D700]

// Camera+18C behaviour: when present and its +1C query is non-zero, the float at [behaviour+off]; else 0.0f.
inline double behavior_float(void* camera, U offset) {
    void* behavior = field<void*>(camera, 0x18C);                     // 0045C173 / 0045C1A3 / 0045C1D3
    if (behavior != nullptr && behavior_slots(behavior).query_1c(behavior, nullptr) != 0u) {   // 0045C179..0045C184
        behavior = field<void*>(camera, 0x18C);                       // 0045C186 reload
        return static_cast<double>(field<float>(behavior, offset));   // 0045C18C fld dword
    }
    return static_cast<double>(kZero);                                // 0045C191 fld [0073D700]
}
} // namespace kotor_m46_st0

extern "C" {
using kotor_m46_st0::Vector; using kotor_m46_st0::Plane;
// world-los geometry_core_handover (D55 single provider): cdecl, EAX = out
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance,
                                                 const Vector* start, const Vector* end);
// gui-writers-vector-equal (M45): operator==(const Vector&, const Vector&) cdecl, EAX 0/1
std::uint32_t __cdecl aurora_vector_equal_004aa980(const void* left, const void* right);
// lane-m46-3dcore m46_math.cpp: Vector::operator-= 004A9770, thiscall RET 4, EAX = this
Vector* __fastcall vector_subtract_assign_004a9770(Vector* self, void* unused_edx, const Vector* other);

// 0045C170 Camera::GetYaw: ST0 = behaviour float +3C.
double __fastcall camera_get_yaw_0045c170(void* self, void*) {
    return kotor_m46_st0::behavior_float(self, 0x3C);                // 0045C18C fld [ecx+3Ch]
}
// 0045C1A0 Camera::GetPitch: ST0 = behaviour float +44.
double __fastcall camera_get_pitch_0045c1a0(void* self, void*) {
    return kotor_m46_st0::behavior_float(self, 0x44);                // 0045C1BC fld [ecx+44h]
}
// 0045C1D0 Camera::GetDist: ST0 = behaviour float +30.
double __fastcall camera_get_dist_0045c1d0(void* self, void*) {
    return kotor_m46_st0::behavior_float(self, 0x30);                // 0045C1EC fld [ecx+30h]
}

// 0045D300 Camera::HitCheckViewVolumeNearest(const Vector* start, const Vector* end, Vector* out, int* hit):
// thiscall RET 10h, ST0 = nearest distance (initially the far clip [this+1E0]).
double __fastcall camera_hit_check_view_volume_nearest_0045d300(void* self, void*, const Vector* start,
                                                                const Vector* end, Vector* out, std::int32_t* hit) {
    using namespace kotor_m46_st0;
    float nearest = field<float>(self, 0x1E0);                         // 0045D308..0045D30E
    if (hit != nullptr) *hit = 0;                                     // 0045D312..0045D34B (arg4)
    const Vector zero{0.0f, 0.0f, 0.0f};                              // 0045D319..0045D341 (second local vector)
    for (I plane_index = 2; plane_index < 6; ++plane_index) {        // 0045D355 ebx = 2, edi = 20h .. 60h
        const Plane* plane = &field<PlaneList>(self, 0x1F8).data[plane_index];   // 0045D360..0045D36A (reloaded)
        Vector tmp;
        const Vector* r = plane_line_intersection_004ab250(&tmp, reinterpret_cast<const Vector*>(plane), plane->d,
                                                           start, end);             // 0045D36D..0045D379
        Vector point = *r;                                            // 0045D37E..0045D398
        if (aurora_vector_equal_004aa980(&point, &zero) != 0u) continue;   // 0045D39C..0045D3A6
        bool inside = true;
        for (I k = 0; k < 6; ++k) {                                   // 0045D3AC..0045D3F0 (ecx 0..60h, edx = k)
            if (k == plane_index) continue;                           // 0045D3B0 cmp edx,ebx / je
            const Plane& p = field<PlaneList>(self, 0x1F8).data[k];   // 0045D3B4 reload
            const double dist = static_cast<double>(point.z) * static_cast<double>(p.c) +
                                static_cast<double>(point.y) * static_cast<double>(p.b) +
                                static_cast<double>(point.x) * static_cast<double>(p.a) +
                                static_cast<double>(p.d);             // 0045D3BA..0045D3D5
            if (dist > static_cast<double>(kZero)) { inside = false; break; }   // 0045D3D8..0045D3E3 test ah,41h je
        }
        if (!inside) continue;                                        // -> 0045D46A
        vector_subtract_assign_004a9770(&point, nullptr, &field<Vector>(self, 0x7C));   // 0045D3F2..0045D3FA
        const Plane& forward = field<PlaneList>(self, 0x1F8).data[1]; // 0045D403..0045D40C
        const double depth = static_cast<double>(point.z) * static_cast<double>(forward.c) +
                             static_cast<double>(point.y) * static_cast<double>(forward.b) +
                             static_cast<double>(point.x) * static_cast<double>(forward.a);   // 0045D3FF..0045D41E
        // 0045D420..0045D429: fcom [nearest]; test ah,5; jp -> reject unless depth < nearest (ordered)
        if (!(depth < static_cast<double>(nearest))) continue;
        // 0045D42B..0045D436: fcom 0.0; test ah,41h; jne -> reject unless depth > 0
        if (!(depth > static_cast<double>(kZero))) continue;
        nearest = static_cast<float>(depth);                          // 0045D43C fstp dword
        if (out != nullptr) *out = point;                             // 0045D438..0045D455 (arg3)
        if (hit != nullptr) *hit = 1;                                 // 0045D458..0045D460 (arg4)
    }
    return static_cast<double>(nearest);                              // 0045D477 fld [esp+10h]
}
} // extern "C"
