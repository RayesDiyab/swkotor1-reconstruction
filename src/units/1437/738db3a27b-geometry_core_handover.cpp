// lane-world-los: 3D-core-assigned vector/box helpers needed by the world line-of-sight closure.
// These addresses sit in lane-3d-render SUBLANES "core" address_list but had no body anywhere in the
// 2026-10-02/06 trees; kept in their own unit so the integrator can hand the file to lane-3d-render.
// 004AB130 Vector::Normalize is NOT here: owner lane-3d-model model_skin.cpp:32.
//
// x87 policy (D30-FINAL): double intermediates; float only where the original stores a dword (fstp/fst dword).
// Compare patterns (fcomp + fnstsw + test ah):  "test ah,5 / jp"  taken iff !(a < b);
// "test ah,41h / jne" taken iff !(a > b); "test ah,44h / jnp" taken iff a == b; "test ah,41h / jp" taken iff
// !(a <= b); "test ah,1 / jne" taken iff !(a >= b). NaN behaviour follows from the C++ relational operators.
#include "4bdde5ee56-world_los.h"

namespace kotor_world_los {
// Function-static Vector of 004AB250 (.bss 00831F28) and its guard word (.bss 00831F34, bit 0 only;
// no other reference to either address in swkotor.exe).
U guard_00831f34 = 0;
Vector plane_line_direction_00831f28 = {0.0f, 0.0f, 0.0f};
}

using namespace kotor_world_los;

extern "C" {

// 00423B70 fabs(float): fcomp [0073D700]=0.0; value < 0 -> fchs. Result in ST0 (double here).
double __cdecl aurora_fabs_00423b70(float value) {
    const double v = value;                                      // 00423B7A fld
    if (value < k_zero_0073d700) return -v;                      // 00423B74..00423B85 (jp skips fchs iff !(v<0))
    return v;
}

// 00437A90 operator/(out, v, divisor): divisor == 0.0 -> raw copy of v (00437AD0..00437AEB).
// Otherwise inv = 1.0/divisor (wide); fst stores float(inv) into the divisor slot (00437AAF); z uses the wide inv
// (00437AB3), y and x use the stored float inv (00437AB6/00437ABD). All products formed before the stores.
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor) {
    if (divisor == k_zero_0073d700) {                            // 00437A90..00437A9F (test ah,44h / jnp)
        copy_vector(out, v);                                     // 00437AD0..00437AE8 dword moves
        return out;
    }
    const double inv = static_cast<double>(k_one_0073d764) / divisor;   // 00437AA1..00437AAB
    const float inv_f = static_cast<float>(inv);                         // 00437AAF fst dword
    const double z = inv * v->z;                                          // 00437AB3
    const double y = static_cast<double>(inv_f) * v->y;                   // 00437AB6..00437ABA
    const double x = static_cast<double>(inv_f) * v->x;                   // 00437ABD..00437AC1
    out->x = static_cast<float>(x);                                       // 00437AC7
    out->y = static_cast<float>(y);                                       // 00437AC9
    out->z = static_cast<float>(z);                                       // 00437ACC
    return out;
}

// 004AA160 (Ghidra "GetRelativeSpacing?"): Cohen-Sutherland style out-code of point vs box into *out_code.
// x: < min -> 20h else > max -> 10h; y: < min -> |8 else > max -> |4; z: < min -> |2 else > max -> |1.
// Each "|=" is a byte read-modify-write of *out_code (004AA1A0/004AA1B3/004AA1C6/004AA1DC).
void __cdecl relative_spacing_004aa160(const Vector* point, const Vector* box_min, const Vector* box_max,
                                       unsigned char* out_code) {
    *out_code = 0;                                               // 004AA16D
    if (point->x < box_min->x) *out_code = 0x20;                 // 004AA170..004AA180
    else if (point->x > box_max->x) *out_code = 0x10;            // 004AA185..004AA190 (test ah,41h / jne)
    if (point->y < box_min->y) *out_code = static_cast<unsigned char>(*out_code | 8);       // 004AA193..004AA1A4
    else if (point->y > box_max->y) *out_code = static_cast<unsigned char>(*out_code | 4);  // 004AA1A6..004AA1B7
    if (point->z < box_min->z) *out_code = static_cast<unsigned char>(*out_code | 2);       // 004AA1B9..004AA1CB
    else if (point->z > box_max->z) *out_code = static_cast<unsigned char>(*out_code | 1);  // 004AA1CF..004AA1DC
}

// 004AA610 BoxIntersect(start, end, min, max): -1 if start inside; 0 if both out-codes share a bit;
// else 1..6 = face entered (x-min, x-max, y-min, y-max, z-min, z-max), tested in that order on start's code.
// For each face: t = (plane - s)/(e - s) wide, u = 1.0 - t, the two other coordinates u*s + t*e.
// x/y faces: both coordinates stored as float before the range checks; z faces: first coordinate stored as
// float, the second kept wide on the FPU (004AA8B1 fcom / 004AA932 fcom).  A z-face failure returns 0 at once
// (004AA96E/004AA970) instead of falling through.
I __cdecl box_intersect_004aa610(const Vector* line_start, const Vector* line_end, const Vector* box_min,
                                 const Vector* box_max) {
    const Vector* s = line_start; const Vector* e = line_end;
    const Vector* lo = box_min; const Vector* hi = box_max;
    unsigned char code_start;                                    // [esp+0Eh]
    relative_spacing_004aa160(s, lo, hi, &code_start);           // 004AA622..004AA62A
    if (code_start == 0) return -1;                              // 004AA636..004AA645
    unsigned char code_end;                                      // [esp+13h]
    relative_spacing_004aa160(e, lo, hi, &code_end);             // 004AA64B..004AA653
    const unsigned char c = code_start;
    if ((c & code_end) != 0) return 0;                           // 004AA663 test cl,al / jne 004AA970

    auto in_range = [](float v, float a, float b) { return v >= a && v <= b; };  // "test ah,1/jne" then "test ah,41h/jp"
    if (c & 0x20) {                                              // 004AA66B
        const double t = (static_cast<double>(lo->x) - s->x) / (static_cast<double>(e->x) - s->x);  // 004AA674..004AA67D
        const double u = static_cast<double>(k_one_0073d764) - t;                                    // 004AA67F..004AA685
        const float y = static_cast<float>(u * s->y + t * e->y);                                     // 004AA687..004AA693
        const float z = static_cast<float>(u * s->z + t * e->z);                                     // 004AA697..004AA6A1
        if (in_range(y, lo->y, hi->y) && in_range(z, lo->z, hi->z)) return 1;                        // 004AA6A5..004AA6F9
    } else if (c & 0x10) {                                       // 004AA6FA
        const double t = (static_cast<double>(hi->x) - s->x) / (static_cast<double>(e->x) - s->x);  // 004AA6FF..004AA707
        const double u = static_cast<double>(k_one_0073d764) - t;
        const float y = static_cast<float>(u * s->y + t * e->y);                                     // 004AA711..004AA71D
        const float z = static_cast<float>(u * s->z + t * e->z);                                     // 004AA721..004AA72B
        if (in_range(y, lo->y, hi->y) && in_range(z, lo->z, hi->z)) return 2;                        // 004AA72F..004AA773
    }
    if (c & 8) {                                                 // 004AA774
        const double t = (static_cast<double>(lo->y) - s->y) / (static_cast<double>(e->y) - s->y);  // 004AA77D..004AA789
        const double u = static_cast<double>(k_one_0073d764) - t;
        const float x = static_cast<float>(u * s->x + t * e->x);                                     // 004AA793..004AA79D
        const float z = static_cast<float>(u * s->z + t * e->z);                                     // 004AA7A1..004AA7AB
        if (in_range(x, lo->x, hi->x) && in_range(z, lo->z, hi->z)) return 3;                        // 004AA7AF..004AA802
    } else if (c & 4) {                                          // 004AA803
        const double t = (static_cast<double>(hi->y) - s->y) / (static_cast<double>(e->y) - s->y);  // 004AA808..004AA814
        const double u = static_cast<double>(k_one_0073d764) - t;
        const float x = static_cast<float>(u * s->x + t * e->x);                                     // 004AA81E..004AA828
        const float z = static_cast<float>(u * s->z + t * e->z);                                     // 004AA82C..004AA836
        if (in_range(x, lo->x, hi->x) && in_range(z, lo->z, hi->z)) return 4;                        // 004AA83A..004AA87D
    }
    if (c & 2) {                                                 // 004AA87E
        const double t = (static_cast<double>(lo->z) - s->z) / (static_cast<double>(e->z) - s->z);  // 004AA883..004AA88F
        const double u = static_cast<double>(k_one_0073d764) - t;
        const float x = static_cast<float>(u * s->x + t * e->x);                                     // 004AA899..004AA8A3
        const double y = u * s->y + t * e->y;                                                        // 004AA8A7..004AA8AF (wide)
        if (!(y >= lo->y)) return 0;                             // 004AA8B1..004AA8B9 -> 004AA96E
        if (!(y <= hi->y)) return 0;                             // 004AA8BF..004AA8C7
        if (!(x >= lo->x)) return 0;                             // 004AA8CD..004AA8D9
        if (!(x <= hi->x)) return 0;                             // 004AA8DF..004AA8EA
        return 5;                                                // 004AA8F3
    }
    if (c & 1) {                                                 // 004AA8FD
        const double t = (static_cast<double>(hi->z) - s->z) / (static_cast<double>(e->z) - s->z);  // 004AA902..004AA90E
        const double u = static_cast<double>(k_one_0073d764) - t;
        const float x = static_cast<float>(u * s->x + t * e->x);                                     // 004AA918..004AA922
        const double y = t * e->y + u * s->y;                                                        // 004AA926..004AA930 (wide)
        if (!(y >= lo->y)) return 0;                             // 004AA932..004AA93A
        if (!(y <= hi->y)) return 0;                             // 004AA93C..004AA944
        if (!(x >= lo->x)) return 0;                             // 004AA946..004AA952
        if (!(x <= hi->x)) return 0;                             // 004AA954..004AA95F
        return 6;                                                // 004AA964
    }
    return 0;                                                    // 004AA970..004AA979
}

// 004AB180 normalize(out, v): m = |v| (004AB100, ST0); m < 1e-9 (qword 007440F0, ordered) -> out = (1,0,0);
// otherwise (including NaN m, "test ah,5 / jp" taken) inv_f = float(1.0/m)
// stored to a dword local (004AB1B9), out = float(inv_f * v) with all three products formed before the stores.
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v) {
    auto* const resultPointer = out;
    const double magnitude = vector_magnitude_004ab100(v);
    // Preserve fcom (including unordered), the qword threshold, the reciprocal
    // dword store, and inv-register versus vector-memory NaN operand roles.
    float inverse;
    __asm {
        fld magnitude
        fcom qword ptr [k_epsilon_007440f0]
        fnstsw ax
        test ah, 5
        jp normalized
        fstp st(0)
        mov eax, resultPointer
        mov dword ptr [eax], 03f800000h
        mov dword ptr [eax+4], 0
        mov dword ptr [eax+8], 0
        jmp done
    normalized:
        fld dword ptr [k_one_0073d764]
        fdiv st(0), st(1)
        fstp inverse
        fstp st(0)
        mov ecx, v
        fld inverse
        fmul dword ptr [ecx+8]
        fld inverse
        fmul dword ptr [ecx+4]
        fld inverse
        fmul dword ptr [ecx]
        mov eax, resultPointer
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    done:
    }
    return out;
}

// 004AB250 planelineintersection(out, normal, distance, a, b): function-static dir (00831F28, guard 00831F34 bit 0)
// = float(b - a); d = float(dir.x*n.x + dir.z*n.z + dir.y*n.y) stored over the b slot (004AB2E2);
// t = 0.0 if d == 0.0, else -((a.z*n.z + a.y*n.y + n.x*a.x) + distance)/d (wide).
// out.x/out.y = float(float(dir*t) + a); out.z = float(dir.z*t + a.z) (no intermediate store, 004AB335..004AB339).
Vector* __cdecl plane_line_intersection_004ab250(Vector* out, const Vector* normal, float distance,
                                                 const Vector* a, const Vector* b) {
    if ((guard_00831f34 & 1u) == 0) {                            // 004AB250..004AB260 (byte test)
        guard_00831f34 |= 1u;                                    // 004AB262 (dword or)
        plane_line_direction_00831f28.x = 0.0f;                  // 004AB268
        plane_line_direction_00831f28.y = 0.0f;                  // 004AB272
        plane_line_direction_00831f28.z = 0.0f;                  // 004AB27C
    }
    Vector dir;                                                  // [esp+0..8]
    dir.x = static_cast<float>(static_cast<double>(b->x) - a->x);  // 004AB28A..004AB292
    dir.y = static_cast<float>(static_cast<double>(b->y) - a->y);  // 004AB296..004AB29C
    dir.z = static_cast<float>(static_cast<double>(b->z) - a->z);  // 004AB2A0..004AB2BD
    copy_vector(&plane_line_direction_00831f28, &dir);           // 004AB2AE/004AB2B3/004AB2C9 dword copies
    const float d = static_cast<float>(static_cast<double>(dir.x) * normal->x +
        static_cast<double>(dir.z) * normal->z + static_cast<double>(dir.y) * normal->y);  // 004AB2C5..004AB2E2
    double t = k_zero_0073d700;                                  // 004AB2E6
    if (!(d == k_zero_0073d700)) {                               // 004AB2EC..004AB2FB (jnp taken iff d == 0)
        t = -(((static_cast<double>(a->z) * normal->z + static_cast<double>(a->y) * normal->y +
                static_cast<double>(normal->x) * a->x) + distance) / d);                    // 004AB2FD..004AB31B
    }
    const float px = static_cast<float>(static_cast<double>(dir.x) * t);   // 004AB31D..004AB327
    const float py = static_cast<float>(static_cast<double>(dir.y) * t);   // 004AB32B..004AB331
    const double z = static_cast<double>(dir.z) * t + a->z;               // 004AB335..004AB339
    const double y = static_cast<double>(py) + a->y;                      // 004AB33C..004AB340
    const double x = static_cast<double>(px) + a->x;                      // 004AB343..004AB347
    out->x = static_cast<float>(x);                                       // 004AB349
    out->y = static_cast<float>(y);                                       // 004AB34B
    out->z = static_cast<float>(z);                                       // 004AB34E
    return out;
}

} // extern "C"
