// anim_contested.cpp - lane-3d-anim: addresses listed in lane-3d-model REMAINING.json group "animation" that
// lane-3d-render SUBLANES.json assigns to the per-frame core (00437CC0, 00438AF0, 004A9B80, 004A9D30, 004A9E00).
// Authored here as one separately stageable unit; the integrator stages it or drops it in favour of a core provider
// (single-owner rule, D35). 00485560 / 00485590 are authored by lane-3d-construct / lane-3d-emitter and are not here.
#include "3775ec35d8-anim3d.h"

using namespace kotor_anim3d;

namespace {
inline float f32(double value) { return static_cast<float>(value); }
// x87 transcendental steps of the original, executed on the x87 unit (D21 inline asm; D30-FINAL: original CRT port).
// Under the original CW 0x027F (PC_53, RC nearest) these produce the original results bit for bit.
// facos 006FC0E0 -> _acos main path 006FC118..006FC126 for |x| < 1: fpatan(sqrt((1+x)(1-x)), x).
inline double x87_acos(double x) {
    double result;
    __asm {
        fld qword ptr x
        fld1
        fadd st, st(1)
        fld1
        fsub st, st(2)
        fmulp st(1), st
        fsqrt
        fxch st(1)
        fpatan
        fstp qword ptr result
    }
    return result;
}
inline double x87_sin(double x) {   // fsin (004A9EB0 / 004A9EC6 / 004A9EE3)
    double result;
    __asm {
        fld qword ptr x
        fsin
        fstp qword ptr result
    }
    return result;
}
}

extern "C" {
// 00437CC0 CExoArrayList<Quaternion>::Allocate(int capacity), RET 4. New elements are identity quaternions (w = 1).
void __fastcall quaternion_list_allocate_00437cc0(PointerList* list, void*, I capacity) {
    const U old = list->data;                                                // 00437CCC
    list->capacity = capacity;                                               // 00437CD2
    unsigned char* block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(capacity) << 4));   // 00437CCE..00437CD5 (006FC0D2 -> 006FA7E6)
    if (block != nullptr) {                                                  // 00437CDF..00437CE1
        for (I i = 0; i <= capacity - 1; ++i) {                              // 00437CE3..00437D02
            unsigned char* e = block + 16 * i;
            const U one = 0x3F800000u, zero = 0u;
            std::memcpy(e + 12, &zero, 4); std::memcpy(e + 8, &zero, 4);    // 00437CF3 / 00437CF6
            std::memcpy(e + 4, &zero, 4); std::memcpy(e + 0, &one, 4);      // 00437CF8 / 00437CFB
        }
    }
    list->data = static_cast<U>(reinterpret_cast<std::uintptr_t>(block));   // 00437D06..00437D08
    for (I i = 0; i < list->size; ++i)                                       // 00437D0A..00437D3B (size reloaded)
        std::memcpy(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(list->data)) + 16 * i,
                    reinterpret_cast<const unsigned char*>(static_cast<std::uintptr_t>(old)) + 16 * i, 16);   // 00437D15..00437D2F
    if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old)));   // 00437D3F..00437D44 (006FC0CD -> 006FA390)
}

// 00438AF0 CExoArrayList<Quaternion>::Add(Quaternion), RET 10h.
void __fastcall quaternion_list_add_00438af0(PointerList* list, void*, Quaternion value) {
    if (list->size == list->capacity) {                                      // 00438AF3..00438AF9
        const I grow = (list->capacity != 0) ? list->capacity * 2 : 0x10;    // 00438AFB..00438B03
        quaternion_list_allocate_00437cc0(list, nullptr, grow);              // 00438B08..00438B0B
    }
    std::memcpy(reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(list->data)) + 16 * list->size, &value, 16);   // 00438B10..00438B32
    list->size = list->size + 1;                                             // 00438B35
}

// 004A9B80 quaternionScalarMult(Quaternion* out, Quaternion* q, float s) cdecl -> out.
Quaternion* __cdecl quaternion_scalar_multiply_004a9b80(Quaternion* out, const Quaternion* q, float s) {
    const double z = static_cast<double>(s) * q->z;                         // 004A9B84..004A9B88
    const double y = static_cast<double>(s) * q->y;                         // 004A9B8B..004A9B8F
    const double x = static_cast<double>(s) * q->x;                         // 004A9B92..004A9B96
    const double w = static_cast<double>(s) * q->w;                         // 004A9B99..004A9B9D
    out->w = f32(w); out->x = f32(x); out->y = f32(y); out->z = f32(z);     // 004A9BA3..004A9BAB
    return out;                                                              // 004A9B9F
}

// 004A9D30 quaternionDotProduct(Quaternion* a, Quaternion* b) cdecl -> ST0.
double __cdecl quaternion_dot_004a9d30(const Quaternion* a, const Quaternion* b) {
    return ((static_cast<double>(a->z) * b->z + static_cast<double>(a->y) * b->y) + static_cast<double>(a->x) * b->x)
           + static_cast<double>(a->w) * b->w;                               // 004A9D38..004A9D52
}

// 004A9E00 slerp(Quaternion* out, Quaternion from, Quaternion* to, float t) cdecl -> out.
Quaternion* __cdecl quaternion_slerp_004a9e00(Quaternion* out, Quaternion from, const Quaternion* to, float t) {
    double cosine = quaternion_dot_004a9d30(&from, to);                      // 004A9E04..004A9E0E
    if (cosine < static_cast<double>(kZero_0073d700)) {                      // 004A9E13..004A9E21
        from.x = -from.x; from.y = -from.y; from.z = -from.z; from.w = -from.w;   // 004A9E23..004A9E47 (fchs; fstp)
        if (cosine < static_cast<double>(kZero_0073d700)) cosine = -cosine;  // 004A9E4B..004A9E58
    }
    const float c = f32(cosine);                                             // 004A9E5A fst [esp+4]
    if (cosine < kOneDouble_0073ede8 || cosine != cosine) {                  // 004A9E5E..004A9E69 (test ah,1; je)
        const float angle = f32(x87_acos(c));                                // 004A9E6B..004A9E74 (fld [esp+4]; facos; fst)
        if (!(angle == kZero_0073d700)) {                                    // 004A9E78..004A9E83 (test ah,44h; jp)
            const float sine = f32(x87_sin(angle));                          // 004A9EAB..004A9EB6 [esp+0Ch]
            const double scaled = static_cast<double>(angle) * t;            // 004A9EBA..004A9EBE
            const float at = f32(scaled);                                    // 004A9EC2 fst [esp+8]
            const float s1 = f32(x87_sin(scaled) / sine);                    // 004A9EC6..004A9ECC
            Quaternion toward;                                               // [esp+20h]
            const Quaternion* a = quaternion_scalar_multiply_004a9b80(&toward, to, s1);   // 004A9ECF..004A9ED1
            const float s0 = f32(x87_sin(static_cast<double>(angle) - at) / sine);       // 004A9ED6..004A9EF1
            Quaternion away;                                                 // [esp+30h]
            const Quaternion* b = quaternion_scalar_multiply_004a9b80(&away, &from, s0); // 004A9EF4..004A9EF6
            Quaternion sum;                                                  // [esp+10h]
            const Quaternion* r = quaternion_add_004a9d00(&sum, b, a);       // 004A9EFE..004A9F04
            *out = *r;                                                       // 004A9F09..004A9F2A
            return out;
        }
    }
    *out = from;                                                             // 004A9E85..004A9EA3
    return out;
}

} // extern "C"
