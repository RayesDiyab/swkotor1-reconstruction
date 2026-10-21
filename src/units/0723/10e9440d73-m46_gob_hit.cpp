// lane-m46-3dcore fork B: Gob vtable 00741268 slot bodies - hit tests, part placement, lights, motion-blur render.
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1); instruction addresses cited per behaviour. Conventions as in
// m46_gob_small.cpp (D1 raw-word contract; D30-FINAL: double intermediates, float at each original fstp dword).
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <gl/GL.h>
#include "fb4ea0259f-m46_gob_externs.h"

using namespace m46_gob;
using aurora_model::Gob;
using kotor_scene3d::cells;

namespace {
inline Vector vec(U x, U y, U z) { return Vector{bits_to_float(x), bits_to_float(y), bits_to_float(z)}; }
inline void copy3(void* dst, const void* src) {
    field<U>(dst, 0) = field<U>(const_cast<void*>(src), 0);
    field<U>(dst, 4) = field<U>(const_cast<void*>(src), 4);
    field<U>(dst, 8) = field<U>(const_cast<void*>(src), 8);
}
inline void copy4(void* dst, const void* src) {
    copy3(dst, src);
    field<U>(dst, 0xC) = field<U>(const_cast<void*>(src), 0xC);
}

// World point -> gob-local point: rotate(conjugate(orientation), point - position). Each difference component is an
// original fstp dword (0043EBFD..0043EC35 / 0043EC4C..0043EC8A); 004A9B50 + 004A9BB0 as in the original.
Vector to_local(void* gob, const Vector& point) {
    const float* position = &field<float>(gob, 0x78);                   // lea esi,[ebx+78h]
    Vector difference;
    difference.x = S(W(point.x) - W(position[0]));
    difference.y = S(W(point.y) - W(position[1]));
    difference.z = S(W(point.z) - W(position[2]));
    Quaternion inverse;
    Vector rotated;
    const Quaternion* q = quaternion_negate_w_004a9b50(&inverse, &field<Quaternion>(gob, 0x84));   // lea edi,[ebx+84h]
    const Vector* r = quaternion_rotate_004a9bb0(&rotated, q, &difference);
    return *r;                                                          // three DWORD copies out of the result
}

// Shared body of Gob::HitCheckPart 0043E930 and Gob::HitCheck 0043EB80 (identical instruction sequences except the
// part source and the scale word pushed to Part::HitCheck; noted at the two call sites).
U hit_check_common(Gob* self, bool part_from_argument, void* argument_part, const Vector& start, const Vector& end,
                   Vector* out, void* hit, U part_scale_bits) {
    if (hit == nullptr) hit = cells.scene_hit_info_007b93a8;            // 0043EB89..0043EB91 mov ebp,7B93A8h
    const U model = field<U>(self, 0x58);                              // 0043EB96
    if (model == 0) return 0;                                           // 0043EB9B -> 0043EBDE xor eax,eax
    const U16 model_flags = field<U16>(ptr<void>(model), 0x50);         // 0043EB9D mov ax,[eax+50h]
    if ((model_flags >> 8) & 1u) return 0;                              // 0043EBA1..0043EBAA (ah & 1)
    if (field<U8>(hit, 0x5E) != 0 && field<U8>(self, 0x17D) != 0) return 0;   // 0043EBAC..0043EBBB
    if (field<U8>(hit, 0x5D) != 0) {                                    // 0043EBBD..0043EBC2
        // 0043EBC4..0043EBFB: value = bit 3 of the flag low byte ? Gob+DC : Gob+D8; fcomp [0078E858]; test ah,41h;
        // both branches continue only when C0 = C3 = 0 or C0 = C3 = 1 (greater, or unordered) -> !(value <= minimum).
        const double value = ((model_flags & 0xFFu) >> 3) & 1u ? W(field<float>(self, 0xDC)) : W(field<float>(self, 0xD8));
        if (value <= W(*cells.fade_minimum_0078e858)) return 0;
    }
    const Vector local_start = to_local(self, start);                  // 0043EBFD..0043EC5F
    Vector local_end = to_local(self, end);                            // 0043EC4C..0043ECBE
    // 0043ECAA..0043ECD7: SphereIntersect(start', end', &Gob+13C, raw [Gob+138]); zero -> return 0.
    if (sphere_intersect_004abdd0(&local_start, &local_end, &field<Vector>(self, 0x13C), field<U>(self, 0x138)) == 0)
        return 0;
    U hit_flag = 0;                                                     // 0043EC22 mov [esp+64h],0 (result word)
    void* part = part_from_argument ? argument_part : field<void*>(self, 0x6C);   // 0043EA8D [esp+3Ch] / 0043ECDD [ebx+6Ch]
    if (part != nullptr) {
        // Part::HitCheck (Part vtbl+1C, 9 words, RET 24h): (start', end', &end' as output, hit, scale).
        const U result = table<PartTable>(part)->s01c(part, nullptr,
            float_to_bits(local_start.x), float_to_bits(local_start.y), float_to_bits(local_start.z),
            float_to_bits(local_end.x), float_to_bits(local_end.y), float_to_bits(local_end.z),
            word(&local_end), word(hit), part_scale_bits);
        if (result != 0) {
            hit_flag = 1;                                               // 0043ED42 mov [esp+64h],1
            Vector rotated, sum;
            const Vector* r = quaternion_rotate_004a9bb0(&rotated, &field<Quaternion>(self, 0x84), &local_end);   // 0043ED37..0043ED4A
            const Vector* s = vector_sum_00423b90(&sum, &field<Vector>(self, 0x78), r);   // 0043ED4F..0043ED56
            local_end = *s;                                             // 0043ED5B..0043ED80 ([esp+60h..68h])
            copy3(kotor_scene3d::at(hit, 0x1C), &local_end);            // 0043ED6B..0043ED73 hit+1C
            Vector normal;
            const Vector* n = quaternion_rotate_004a9bb0(&normal, &field<Quaternion>(self, 0x84),
                                                         &field<Vector>(hit, 0x40));   // 0043ED76..0043ED84
            copy3(kotor_scene3d::at(hit, 0x40), n);                     // 0043ED89..0043ED96
            if (out != nullptr) *out = local_end;                       // 0043ED99..0043EDB5
        }
    }
    return hit_flag != 0 ? word(self) : 0u;                             // 0043EDB8..0043EDC3 neg/sbb/and ebx
}
}

extern "C" {
// Gob::HitCheckPart(Part*, start xyz, end xyz, Vector* out, CHitInfo* hit) (RET 24h); part scale word 1.0f (0043EA9B).
U __fastcall gob_hit_check_part_0043e930(Gob* self, void*, U part, U sx, U sy, U sz, U ex, U ey, U ez, U out, U hit) {   // +88
    return hit_check_common(self, true, ptr<void>(part), vec(sx, sy, sz), vec(ex, ey, ez), ptr<Vector>(out),
                            ptr<void>(hit), 0x3F800000u);
}
// Gob::HitCheck(Vector start, Vector end, Vector* out, CHitInfo* hit) (RET 20h); part scale word [Gob+1B4] (0043ECEA).
I __fastcall gob_hit_check_0043eb80(Gob* self, void*, Vector start, Vector end, Vector* out, void* hit) {   // +8C
    return static_cast<I>(hit_check_common(self, false, nullptr, start, end, out, hit, field<U>(self, 0x1B4)));
}

// Gob::SetPartPosition(name, world position xyz, world orientation wxyz) (RET 20h).
U __fastcall gob_set_part_position_0043f280(Gob* self, void*, U name, U px, U py, U pz, U qw, U qx, U qy, U qz) {   // +94
    void* part = gob_vt(self)->s10c_gob_get_part(self, nullptr, ptr<const char>(name));   // 0043F280..0043F28D
    if (part == nullptr) return 0;                                      // 0043F295..0043F29F (EAX = 0)
    field<U>(part, 0x08) = 0; field<U>(part, 0x0C) = 0; field<U>(part, 0x10) = 0;   // 0043F2A2..0043F2D0 local position
    field<U>(part, 0x14) = 0x3F800000u; field<U>(part, 0x18) = 0;       // 0043F2D3..0043F310 local orientation (1,0,0,0)
    field<U>(part, 0x1C) = 0; field<U>(part, 0x20) = 0;
    U parent_position[3] = {0, 0, 0};                                   // 0043F31C..0043F32C
    U parent_orientation[4] = {0x3F800000u, 0, 0, 0};                   // 0043F334..0043F34C
    // 0043F313..0043F359: part->GetWorld(&position, &orientation) with the identity local transform = parent frame.
    (void)table<PartTable>(part)->s00c(part, nullptr, word(parent_position), word(parent_orientation));
    const float* v = reinterpret_cast<const float*>(parent_position);
    Vector difference;
    difference.x = S(W(bits_to_float(px)) - W(v[0]));                   // 0043F35C..0043F36D
    difference.y = S(W(bits_to_float(py)) - W(v[1]));                   // 0043F372..0043F37F
    difference.z = S(W(bits_to_float(pz)) - W(v[2]));                   // 0043F383..0043F38B
    const Quaternion* frame = reinterpret_cast<const Quaternion*>(parent_orientation);
    Quaternion inverse;
    Vector local_position;
    const Quaternion* qi = quaternion_negate_w_004a9b50(&inverse, frame);                // 0043F38F
    const Vector* lp = quaternion_rotate_004a9bb0(&local_position, qi, &difference);    // 0043F39D
    copy3(kotor_scene3d::at(part, 8), lp);                              // 0043F3A2..0043F3BB
    const Quaternion wanted{bits_to_float(qw), bits_to_float(qx), bits_to_float(qy), bits_to_float(qz)};
    Quaternion inverse2, product, normalized;
    const Quaternion* qi2 = quaternion_negate_w_004a9b50(&inverse2, frame);             // 0043F3B2..0043F3C4
    const Quaternion* m = quaternion_multiply_004a9ac0(&product, qi2, &wanted);         // 0043F3CC..0043F3D2
    const Quaternion* n = quaternion_normalize_004a9d60(&normalized, *m);               // 0043F3D7..0043F3F5 (by value)
    copy4(kotor_scene3d::at(part, 0x14), n);                            // 0043F3FA..0043F40D
    field<U>(part, 0x48) = 1;                                           // 0043F413..0043F418
    return 1;
}

// Gob::GetMaximumLightRadius() -> float (original channel ST0: 00447F86 fld [esp+0Ch]; plain ret). The canonical
// raw contract types the slot as U (EAX); the bits of the result are returned here and header_diffs proposes the
// typed `float` return (ST0) - see parts/B.json.
U __fastcall gob_get_maximum_light_radius_00447f00(Gob* self, void*) {   // +C8
    U maximum = 0;                                                      // 00447F1D mov [esp+0Ch],0 (0.0f)
    PointerList lights{};                                               // 00447F25..00447F2D
    gob_collect_lights_00447180(self, nullptr, &lights);               // 00447F31..00447F3A
    const I count = lights.size;                                       // 00447F3F
    U* data = ptr<U>(lights.data);                                     // 00447F43
    for (I i = 0; i < count; ++i) {                                    // 00447F47..00447F6B
        void* light = ptr<void>(data[i]);
        // fld [light+58h]; fcomp max; test ah,41h; jne skip -> update only when greater (not less/equal/unordered).
        if (W(field<float>(light, 0x58)) > W(bits_to_float(maximum))) maximum = field<U>(light, 0x58);   // raw copy 00447F61
    }
    if (data != nullptr) live_free(data);                              // 00447F6D..00447F7A (006FA390)
    return maximum;
}

// Gob::SetLightPriority(int priority) (RET 4): Part+74 of every light part.
U __fastcall gob_set_light_priority_00445e30(Gob* self, void*, U priority) {   // +C4
    PointerList lights{};                                               // 00445E4B..00445E53
    find_all_light_parts_00445ae0(field<void*>(self, 0x6C), &lights);   // 00445E57..00445E64
    for (I i = 0; i < lights.size; ++i)                                 // 00445E70..00445E91 (size re-read)
        field<U>(ptr<void>(ptr<U>(lights.data)[i]), 0x74) = priority;   // 00445E80..00445E87
    if (lights.data != 0) live_free(ptr<void>(lights.data));            // 00445E93..00445EA5
    return 0;                                                           // EAX incidental
}

// Gob::RenderBlur() (plain ret; tail jmp glPopMatrix).
U __fastcall gob_render_blur_00474e10(Gob* self, void*) {             // +14C
    glPushMatrix();                                                     // 00474E13 call [0073D300]
    if (*cells.transform_enabled_0078e5f0 != 0)                         // 00474E19..00474E20
        gl_transform_0046f060(&field<Vector>(self, 0x78), &field<Quaternion>(self, 0x84), 1);   // 00474E22..00474E2F
    const U scale = field<U>(self, 0x1B4);                              // 00474E37
    if (scale != 0x3F800000u)                                           // 00474E3D cmp eax,3F800000h (raw bits)
        glScalef(bits_to_float(scale), bits_to_float(scale), bits_to_float(scale));   // 00474E44..00474E4B
    gob_proxy_part_draw_00474c90(self, nullptr, field<void*>(self, 0x6C), 1);   // 00474E51..00474E59
    copy3(kotor_scene3d::at(self, 0x94), kotor_scene3d::at(self, 0x78));    // 00474E5E..00474E74 previous position
    copy4(kotor_scene3d::at(self, 0xA0), kotor_scene3d::at(self, 0x84));    // 00474E77..00474E96 previous orientation
    glPopMatrix();                                                      // 00474E9A jmp [0073D2FC]
    return 0;                                                           // EAX incidental (glPopMatrix)
}
} // extern "C"
