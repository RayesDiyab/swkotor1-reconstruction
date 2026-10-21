// anim_controllers.cpp - lane-3d-anim: NewController evaluation, node InternalControl family, ControlDispatch,
// Part::Animate, vector/quaternion interpolation and Bezier helpers, scene animation flags and the texture
// controller update of swkotor.exe (GOG K1). Every statement cites the original instruction(s) it reproduces.
// Float discipline (D30-FINAL): `double` = x87 register value, `float` only at a cited fstp/fst dword store.
#include "3775ec35d8-anim3d.h"
#include <cmath>

using namespace kotor_anim3d;

namespace {
inline float f32(double value) { return static_cast<float>(value); }   // fstp dword (RC nearest)
// NewController record (16 bytes, MdlNode+38 array). Observed fields only:
//  +00 I32 target offset inside the Part (00483358 add edi,part; also the controller id: 8 = position 00485049,
//      0x14 = orientation 00484F7A)          +04 I16 base offset inside the MdlNode, -1 = none (00483370 / 0048337A)
//  +06 U16 key rows (00482B85)              +08 U16 first key-time index in MdlNode+44 data (00482B94)
//  +0A U16 first value index (00482C26)     +0C U8 low nibble = value columns, high nibble 1 = Bezier (00482BF3 / 00482C48)
inline I target_offset(void* c) { return field<I>(c, 0x00); }
inline I16 base_offset(void* c) { return field<I16>(c, 0x04); }
inline I rows(void* c) { return static_cast<I>(field<U16>(c, 0x06)); }
inline I time_index(void* c) { return static_cast<I>(field<U16>(c, 0x08)); }
inline I data_index(void* c) { return static_cast<I>(field<U16>(c, 0x0A)); }
inline U columns(void* c) { return static_cast<U>(field<U8>(c, 0x0C)) & 0x0Fu; }
inline bool bezier(void* c) { return (field<U8>(c, 0x0C) & 0xF0u) == 0x10u; }
// MdlNode+44 controller data (float array; values copied raw where the original uses mov).
inline float* node_data(void* node) { return field<float*>(node, 0x44); }
inline U raw(const float* base, I index) { U u; std::memcpy(&u, base + index, 4); return u; }
inline void put_raw(void* dst, U value) { std::memcpy(dst, &value, 4); }
inline void* controllers(void* node) { return field<void*>(node, 0x38); }
inline I controller_count(void* node) { return field<I>(node, 0x3C); }
inline void* controller_at(void* node, I index) { return at(controllers(node), static_cast<U>(index) * 16u); }

// DoEvent payload built by ReportKey/ReportKeys (16-byte stack record at [esp+20h]: 00484ECE / 00484EF0 /
// 00484EF4 / 00484EF8 and 00485497 / 00485493 / 004854A9). Fields as stored by the original.
struct KeyEvent { U time; Vector* position; Quaternion* orientation; U8 position_key; U8 unused[3]; };
static_assert(sizeof(KeyEvent) == 16, "ReportKey record");
const char kReportKey_00743268[] = "ReportKey";
}

extern "C" {
// ======================================================================= scene / texture flags
// 0044F3B0 Scene::DisableAnimations: mov byte ptr [ecx+0DAh],0 ; ret
// Raw-word slot contract (vtables_gen.h Scene vtbl+64): EAX is not written by the original (it keeps the caller's value);
// D2-label: returns 0, no consumer of the slot reads it.
U __fastcall scene_disable_animations_0044f3b0(void* scene, void*) { field<U8>(scene, 0xDA) = 0; return 0; }
// 0044F3C0 Scene::IsAnimating: mov al,byte ptr [ecx+0DAh] ; ret
kotor_scene3d::U8 __fastcall scene_is_animating_0044f3c0(void* scene, void*) { return field<U8>(scene, 0xDA); }

// 00420E50 AurTextureForceControllerUpdate (cdecl, no args). ECX is not an input (00420E5D lea ecx,[ecx] is padding).
void __cdecl aur_texture_force_controller_update_00420e50() {
    PointerList* list = anim_cells.ordered_textures_007a4798;
    if (list->size <= 0) return;                                             // 00420E50..00420E5A (jle)
    for (I i = 0; ;) {
        void* texture = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            reinterpret_cast<const U*>(static_cast<std::uintptr_t>(list->data))[i]));   // 00420E60..00420E65
        if (field<U8>(texture, 0xE7) != 0 && field<U>(texture, 0x34) != 0) { // 00420E68..00420E77
            slots<TextureSlots>(texture)->slotb4(texture, nullptr);          // 00420E79..00420E7D call [edx+0B4h]
            field<U8>(texture, 0xE7) = 0;                                    // 00420E83
        }
        ++i;                                                                 // 00420E8F
        if (!(i < list->size)) break;                                        // 00420E8A / 00420E90..00420E92 (count reloaded)
    }
}

// ======================================================================= interpolation helpers
// 0046EEA0 Interpolate(Vector* out, Vector* a, Vector* b, float t) cdecl -> out = (1-t)a + t b.
Vector* __cdecl vector_interpolate_0046eea0(Vector* out, const Vector* a, const Vector* b, float t) {
    const double bx = static_cast<double>(t) * b->x;                         // 0046EEA7..0046EEAB (wide)
    const double by = static_cast<double>(t) * b->y;                         // 0046EEAD..0046EEB1 (wide)
    const float bz = f32(static_cast<double>(t) * b->z);                     // 0046EEB4..0046EEBF fstp [esp+14h]
    const double s = static_cast<double>(kOne_0073d764) - t;                 // 0046EEC3..0046EEC9
    const double ax = s * a->x;                                              // 0046EECD..0046EECF (wide)
    const float ay = f32(s * a->y);                                          // 0046EED1..0046EED6 fstp [esp+4]
    const float az = f32(s * a->z);                                          // 0046EEDA..0046EEE3 fstp [esp+8]
    out->x = f32(ax + bx);                                                   // 0046EEE7..0046EEE9
    out->y = f32(static_cast<double>(ay) + by);                              // 0046EEEB..0046EEF1
    out->z = f32(static_cast<double>(az) + bz);                              // 0046EEF8..0046EF00
    return out;                                                              // 0046EEDF mov eax,[esp+1Ch]
}

// 00481AA0 Interpolate(Quaternion* out, Quaternion* a, Quaternion* b, float t) cdecl: slerp(out, *a, b, t); EAX = out.
Quaternion* __cdecl quaternion_interpolate_00481aa0(Quaternion* out, const Quaternion* a, const Quaternion* b, float t) {
    const Quaternion copy = *a;                                              // 00481AB3..00481ACC (16-byte by-value copy)
    quaternion_slerp_004a9e00(out, copy, b, t);                              // 00481ACF (EAX of slerp discarded)
    return out;                                                              // 00481AD7 mov eax,esi
}

// 004AAAE0 InterpolateBezier(float p0, float p3, float p1, float p2, float t) cdecl -> ST0. De Casteljau over the
// control polygon (p0, p1, p2, p3) = (arg1, arg3, arg4, arg2), as the original orders its loads.
double __cdecl interpolate_bezier_004aaae0(float p0, float p3, float p1, float p2, float t) {
    const double s = static_cast<double>(kOne_0073d764) - t;                 // 004AAAE3..004AAAE9
    const double l1 = static_cast<double>(p0) * s + static_cast<double>(p1) * t;   // 004AAAED..004AAAFB
    const double l2 = s * p1 + static_cast<double>(p2) * t;                  // 004AAAFD..004AAB0B
    const double l3 = s * p2 + static_cast<double>(p3) * t;                  // 004AAB0D..004AAB1B
    const float m1 = f32(l1 * s + l2 * t);                                   // 004AAB1D..004AAB29 fstp [esp]
    const double m2 = l2 * s + l3 * t;                                       // 004AAB2D..004AAB35
    return static_cast<double>(m1) * s + m2 * t;                             // 004AAB37..004AAB45 (ST0)
}

// 004AC110 InterpolateBezier(Vector* out, p0, p3, p1, p2, float t) cdecl; control polygon p0 -> p1 -> p2 -> p3
// (arg2 -> arg4 -> arg5 -> arg3). Wide/stored split per component exactly as the original register allocation.
Vector* __cdecl vector_interpolate_bezier_004ac110(Vector* out, const Vector* p0, const Vector* p3,
                                                     const Vector* p1, const Vector* p2, float t) {
    // 004AC11D..004AC134 zero three stack Vectors at [esp+3Ch..5Ch]; they are never read (no observable effect).
    const double td = t;
    const double s = static_cast<double>(kOne_0073d764) - td;               // 004AC110..004AC119
    // a = s*p0 + t*p1
    const double a_t0 = td * p1->x, a_t1 = td * p1->y;                        // 004AC13A..004AC148 (wide)
    const float a_t2 = f32(td * p1->z);                                      // 004AC14B..004AC152 [38]
    const double a_s0 = s * p0->x;                                           // 004AC156..004AC158 (wide)
    const float a_s1 = f32(s * p0->y);                                       // 004AC15A..004AC15F [1C]
    const float a_s2 = f32(s * p0->z);                                       // 004AC163..004AC16C [20]
    const float a_x = f32(a_s0 + a_t0);                                      // 004AC170..004AC172 [24]
    const double a_y = static_cast<double>(a_s1) + a_t1;                    // 004AC176..004AC17E (wide)
    const double a_z = static_cast<double>(a_s2) + a_t2;                    // 004AC180..004AC184 (wide)
    // b = s*p1 + t*p2
    const double b_t0 = td * p2->x, b_t1 = td * p2->y;                        // 004AC188..004AC192 (ecx = [esp+74h] = p2)
    const float b_t2 = f32(td * p2->z);                                      // 004AC195..004AC19C [38]
    const double b_s0 = s * p1->x;                                           // 004AC1A0..004AC1A2 (eax = [esp+70h] = p1)
    const float b_s1 = f32(s * p1->y);                                       // 004AC1A4..004AC1A9 [1C]
    const float b_s2 = f32(s * p1->z);                                       // 004AC1AD..004AC1B6 [20]
    const float b_x = f32(b_s0 + b_t0);                                      // 004AC1BA..004AC1BC [0]
    const float b_y = f32(static_cast<double>(b_s1) + b_t1);                // 004AC1C0..004AC1C6 [4]
    const float b_z = f32(static_cast<double>(b_s2) + b_t2);                // 004AC1CE..004AC1D6 [8]
    // c = s*p2 + t*p3
    const double c_t0 = td * p3->x, c_t1 = td * p3->y;                        // 004AC1DA..004AC1E4 (eax = [esp+6Ch] = p3)
    const float c_t2 = f32(td * p3->z);                                      // 004AC1E7..004AC1EE [38]
    const double c_s0 = s * p2->x;                                           // 004AC1F2..004AC1F4 (ecx = p2)
    const float c_s1 = f32(s * p2->y);                                       // 004AC1F6..004AC1FB [10]
    const float c_s2 = f32(s * p2->z);                                       // 004AC1FF..004AC204 [14]
    const float c_x = f32(c_s0 + c_t0);                                      // 004AC208..004AC20A [18]
    const float c_y = f32(static_cast<double>(c_s1) + c_t1);                // 004AC20E..004AC214 [1C]
    const float c_z = f32(static_cast<double>(c_s2) + c_t2);                // 004AC21C..004AC224 [20]
    // d = s*a + t*b
    const float bt_x = f32(static_cast<double>(b_x) * td);                  // 004AC228..004AC230 [0C]
    const float bt_y = f32(static_cast<double>(b_y) * td);                  // 004AC234..004AC23C [10]
    const float bt_z = f32(static_cast<double>(b_z) * td);                  // 004AC240..004AC248 [14]
    const float as_x = f32(static_cast<double>(a_x) * s);                   // 004AC24C..004AC252 [24]
    const float as_y = f32(a_y * s);                                         // 004AC256..004AC25A [28]
    const double as_z = a_z * s;                                             // 004AC25E (wide)
    const double d_x = static_cast<double>(as_x) + bt_x;                    // 004AC260..004AC264 (wide)
    const float d_y = f32(static_cast<double>(as_y) + bt_y);                // 004AC268..004AC270 [34]
    const float d_z = f32(as_z + bt_z);                                      // 004AC274..004AC27A [38]
    // e = s*b + t*c
    const double ct_x = static_cast<double>(c_x) * td;                      // 004AC27E..004AC282 (wide)
    const double ct_y = static_cast<double>(c_y) * td;                      // 004AC286..004AC28A (wide)
    const float ct_z = f32(static_cast<double>(c_z) * td);                  // 004AC28E..004AC296 [20]
    const double bs_x = static_cast<double>(b_x) * s;                       // 004AC29A..004AC29E (wide)
    const float bs_y = f32(static_cast<double>(b_y) * s);                   // 004AC2A0..004AC2A6 [28]
    const float bs_z = f32(static_cast<double>(b_z) * s);                   // 004AC2AA..004AC2B0 [2C]
    const float e_x = f32(bs_x + ct_x);                                      // 004AC2B4..004AC2B6 [0C]
    const double e_y = static_cast<double>(bs_y) + ct_y;                    // 004AC2BA..004AC2C2 (wide)
    const double e_z = static_cast<double>(bs_z) + ct_z;                    // 004AC2C4..004AC2C8 (wide)
    // out = s*d + t*e
    const float et_x = f32(static_cast<double>(e_x) * td);                  // 004AC2CC..004AC2D4 [18]
    const float et_y = f32(e_y * td);                                        // 004AC2D8..004AC2DE [1C]
    const float et_z = f32(e_z * td);                                        // 004AC2E2..004AC2E6 [20]
    const double ds_x = d_x * s;                                             // 004AC2EE (wide)
    const double ds_y = static_cast<double>(d_y) * s;                       // 004AC2F2..004AC2F6 (wide)
    const float ds_z = f32(static_cast<double>(d_z) * s);                   // 004AC2F8..004AC2FE [38]
    out->x = f32(ds_x + et_x);                                               // 004AC302..004AC310
    out->y = f32(ds_y + et_y);                                               // 004AC312..004AC320
    out->z = f32(static_cast<double>(ds_z) + et_z);                         // 004AC323..004AC333
    return out;                                                              // 004AC2EA mov eax,[esp+64h]
}

// ======================================================================= NewController
// 00482B70 NewController::GetKeyIndexAndAlpha(float t, MdlNode* node, int* index, float* alpha), RET 10h.
void __fastcall controller_get_key_index_and_alpha_00482b70(void* controller, void*, float t, void* node, I* index, float* alpha) {
    *index = 0;                                                              // 00482B79
    put_raw(alpha, 0x3F800000u);                                             // 00482B7F (1.0f)
    if (*index < rows(controller)) {                                         // 00482B85..00482B91
        I i = *index;                                                        // 00482B9C
        for (;;) {
            const float key = node_data(node)[time_index(controller) + i];   // 00482B94..00482BA3 ([edi+44h] reloaded)
            if (!(t > key)) break;                                           // fcomp; test ah,41h; jne 00482BB9
            ++i;                                                             // 00482BAE
            *index = i;                                                      // 00482BAF
            if (!(i < rows(controller))) break;                              // 00482BB1..00482BB7
        }
    }
    const I k = *index;                                                      // 00482BBA
    if (k != 0 && k != rows(controller)) {                                   // 00482BBC..00482BC6
        const float* p = node_data(node) + (time_index(controller) + k);     // 00482BC8..00482BD5
        *alpha = f32((static_cast<double>(t) - p[-1]) / (static_cast<double>(p[0]) - p[-1]));   // 00482BCC..00482BE2
    }
}

// 00482BF0 NewController::GetFloatValue(float t, MdlNode* node, float* out), RET 0Ch, EAX 1/0.
U __fastcall controller_get_float_value_00482bf0(void* controller, void*, float t, void* node, float* out) {
    if (columns(controller) != 1u) return 0;                                 // 00482BF3..00482BFF
    I index; float alpha;                                                    // (original reuses its own arg slots)
    controller_get_key_index_and_alpha_00482b70(controller, nullptr, t, node, &index, &alpha);   // 00482C02..00482C19
    const float* d = node_data(node);
    if (index == 0) {                                                        // 00482C1E..00482C24
        put_raw(out, raw(d, data_index(controller)));                        // 00482C26..00482C35
        return 1;
    }
    const I n = rows(controller);                                            // 00482C40
    if (index == n) {                                                        // 00482C44
        if (bezier(controller)) put_raw(out, raw(d, data_index(controller) + 3 * n - 3));   // 00482C48..00482C68
        else put_raw(out, raw(d, data_index(controller) + n - 1));            // 00482C73..00482C85
        return 1;
    }
    if (bezier(controller)) {                                                // 00482C90..00482C9D
        const I e = data_index(controller) + 3 * index - 3;                  // 00482CA2..00482CA5
        const float tangent_b = f32(static_cast<double>(d[e + 4]) + d[e + 2]);   // 00482CA9..00482CBD [esp+4]
        const float tangent_a = f32(static_cast<double>(d[e + 3]) + d[e]);       // 00482CC1..00482CCB [esp]
        *out = f32(interpolate_bezier_004aaae0(d[e], d[e + 2], tangent_a, tangent_b, alpha));   // 00482CCE..00482CDC
        return 1;
    }
    const I k = data_index(controller) + index;                              // 00482CF6
    *out = f32((static_cast<double>(kOne_0073d764) - alpha) * d[k - 1] + static_cast<double>(alpha) * d[k]);   // 00482CE8..00482D10
    return 1;
}

// 00482D20 NewController::GetVectorValue(float t, MdlNode* node, Vector* out), RET 0Ch, EAX 1/0.
U __fastcall controller_get_vector_value_00482d20(void* controller, void*, float t, void* node, Vector* out) {
    if (columns(controller) != 3u) return 0;                                 // 00482D26..00482D35
    I index; float alpha;                                                    // [esp+8] / t slot
    controller_get_key_index_and_alpha_00482b70(controller, nullptr, t, node, &index, &alpha);   // 00482D38..00482D4F
    U guard = anim_vector_guard_00830304;                                    // 00482D54
    if ((guard & 1u) == 0) {                                                 // 00482D59
        guard |= 1u; anim_vector_guard_00830304 = guard;                     // 00482D5D..00482D60
        anim_vector_key_a_008302f8[0] = 0.0f; anim_vector_key_a_008302f8[1] = 0.0f; anim_vector_key_a_008302f8[2] = 0.0f; // 00482D65..00482D79
    }
    if ((guard & 2u) == 0) {                                                 // 00482D83
        guard |= 2u; anim_vector_guard_00830304 = guard;                     // 00482D87..00482D8A
        anim_vector_key_b_008302ec[0] = 0.0f; anim_vector_key_b_008302ec[1] = 0.0f; anim_vector_key_b_008302ec[2] = 0.0f; // 00482D8F..00482DA3
    }
    U scale_bits = 0x3F800000u;                                              // 00482DB0 [esp+44h] = 1.0f
    if (target_offset(controller) == 8) scale_bits = aurora_control_weight_00798b38;   // 00482DAD..00482DC0
    const double scale = bits_to_float(scale_bits);
    I key = -1;                                                              // direct-copy key (00482DF4 path)
    if (index == 0) {                                                        // 00482DC4..00482DCC
        key = data_index(controller);
    } else if (bezier(controller)) {                                         // 00482DD2..00482DE3
        if (index == rows(controller)) key = data_index(controller) + 9 * index - 9;   // 00482DE9..00482DF0
    } else {
        if (index == rows(controller)) key = data_index(controller) + 3 * index - 3;   // 00482F86..00482F91
    }
    if (key >= 0) {                                                          // 00482DF4: copy then scale in place
        put_raw(&out->x, raw(node_data(node), key));                         // 00482DF4..00482DFE
        put_raw(&out->y, raw(node_data(node), key + 1));                     // 00482E00..00482E08
        const float z = node_data(node)[key + 2];                            // 00482E0B..00482E0E fld
        out->z = z;                                                          // 00482E13 fst [eax+8]
        out->x = f32(scale * out->x);                                        // 00482E17..00482E2C
        out->y = f32(scale * out->y);                                        // 00482E25..00482E3A
        out->z = f32(static_cast<double>(z) * scale);                       // 00482E36..00482E45 (ST0 copy * scale)
        return 1;
    }
    Vector result;
    if (bezier(controller)) {                                                // 00482E53
        const I e = data_index(controller) + 9 * index - 9;                  // 00482E56
        anim_vector_key_a_008302f8[0] = node_data(node)[e];                  // 00482E5A..00482E5D
        anim_vector_key_a_008302f8[1] = node_data(node)[e + 1];              // 00482E66..00482E6A
        anim_vector_key_a_008302f8[2] = node_data(node)[e + 2];              // 00482E73..00482E77
        anim_vector_key_b_008302ec[0] = node_data(node)[e + 9];              // 00482E80..00482E84
        anim_vector_key_b_008302ec[1] = node_data(node)[e + 10];             // 00482E8D..00482E95
        anim_vector_key_b_008302ec[2] = node_data(node)[e + 11];             // 00482E9E..00482EA6
        const float* q = node_data(node);                                    // 00482EAC
        const double a0 = q[e + 6], a1 = q[e + 7];                           // 00482EAF..00482EB3 (wide)
        const float a2 = q[e + 8];                                           // 00482EB7..00482EBB [esp+14h]
        const double b0 = q[e + 12], b1 = q[e + 13];                         // 00482EBF..00482EC3 (wide)
        const float b2 = q[e + 14];                                          // 00482EC7..00482ECF [esp+2Ch]
        Vector out_tangent, in_tangent;                                      // [esp+24h] / [esp+18h]
        in_tangent.x = f32(static_cast<double>(anim_vector_key_b_008302ec[0]) + b0);   // 00482ED4..00482EE7
        in_tangent.y = f32(static_cast<double>(anim_vector_key_b_008302ec[1]) + b1);   // 00482EF0..00482EF9
        in_tangent.z = f32(static_cast<double>(anim_vector_key_b_008302ec[2]) + b2);   // 00482F01..00482F0B
        out_tangent.x = f32(static_cast<double>(anim_vector_key_a_008302f8[0]) + a0);  // 00482F0F..00482F17
        out_tangent.y = f32(static_cast<double>(anim_vector_key_a_008302f8[1]) + a1);  // 00482F1B..00482F23
        out_tangent.z = f32(static_cast<double>(anim_vector_key_a_008302f8[2]) + a2);  // 00482F2B..00482F35
        const Vector* r = vector_interpolate_bezier_004ac110(&result,
            reinterpret_cast<const Vector*>(anim_vector_key_a_008302f8), reinterpret_cast<const Vector*>(anim_vector_key_b_008302ec),
            &out_tangent, &in_tangent, alpha);                               // 00482ED3..00482F39
        const float x = f32(scale * r->x);                                   // 00482F3E..00482F4B
        const float y = f32(scale * r->y);                                   // 00482F53..00482F5A
        const float z = f32(scale * r->z);                                   // 00482F5E..00482F6B
        out->x = x; out->y = y; out->z = z;                                  // 00482F69 / 00482F73 / 00482F77
        return 1;
    }
    const I e = data_index(controller) + 3 * index - 3;                      // 00482F99
    anim_vector_key_a_008302f8[0] = node_data(node)[e];                      // 00482F9D..00482FA1
    anim_vector_key_a_008302f8[1] = node_data(node)[e + 1];                  // 00482FAA..00482FAE
    anim_vector_key_a_008302f8[2] = node_data(node)[e + 2];                  // 00482FB7..00482FBB
    anim_vector_key_b_008302ec[0] = node_data(node)[e + 3];                  // 00482FC4..00482FC8
    anim_vector_key_b_008302ec[1] = node_data(node)[e + 4];                  // 00482FD1..00482FD8
    anim_vector_key_b_008302ec[2] = node_data(node)[e + 5];                  // 00482FE1..00482FEA
    const Vector* r = vector_interpolate_0046eea0(&result, reinterpret_cast<const Vector*>(anim_vector_key_a_008302f8),
                                                  reinterpret_cast<const Vector*>(anim_vector_key_b_008302ec), alpha);   // 00482FE9..00482FFB
    const float x = f32(scale * r->x);                                       // 00483000..0048300E
    const float y = f32(scale * r->y);                                       // 00483013..0048301A
    const float z = f32(scale * r->z);                                       // 0048301E..00483032
    out->x = x; out->y = y; out->z = z;                                      // 0048302D..0048303A
    return 1;
}

// 00483050 NewController::GetQuaternionFromIndexLocation(int index, MdlNode* node, Quaternion* out), RET 0Ch.
void __fastcall controller_get_quaternion_from_index_00483050(void* controller, void*, I index, void* node, Quaternion* out) {
    const U format = columns(controller);                                    // 00483055..0048305C
    I k = index - 1;                                                         // 00483068
    k &= (k < 0) ? 0 : -1;                                                   // 00483069..00483071 (setl/dec/and)
    if (format == 4u) {                                                      // 0048305F
        const I e = data_index(controller) + 4 * k;                          // 00483073..00483077
        put_raw(&out->x, raw(node_data(node), e));                           // 0048307A..00483084
        put_raw(&out->y, raw(node_data(node), e + 1));                       // 00483087..0048308F
        put_raw(&out->z, raw(node_data(node), e + 2));                       // 00483092..0048309A
        put_raw(&out->w, raw(node_data(node), e + 3));                       // 0048309D..004830A4
        return;
    }
    if (format != 2u) return;                                                // 004830AA..004830AD
    const U packed = raw(node_data(node), data_index(controller) + k);       // 004830B7..004830CB
    const float x = f32(static_cast<double>(static_cast<I>(packed & 0x7FFu)) * kQuat11_00743250 - kOne_0073d764);          // 004830CE..004830EE
    put_raw(&out->x, float_to_bits(x));                                      // 004830F2..004830F6
    const float y = f32(static_cast<double>(static_cast<I>((packed >> 11) & 0x7FFu)) * kQuat11_00743250 - kOne_0073d764);  // 004830F9..0048311F
    put_raw(&out->y, float_to_bits(y));                                      // 00483127..0048312B
    const float z = f32(static_cast<double>(static_cast<I>(packed >> 22)) * kQuat10_0074324c - kOne_0073d764);            // 0048310C / 00483123..0048313A
    put_raw(&out->z, float_to_bits(z));                                      // 00483142..0048314A
    const double sum = (static_cast<double>(z) * z + static_cast<double>(y) * y) + static_cast<double>(x) * x;   // 0048313E..0048315F
    if (sum <= static_cast<double>(kOne_0073d764)) {                         // fcom 1.0; test ah,41h; jp 00483180 (falls through for <=)
        out->w = f32(std::sqrt(static_cast<double>(kOne_0073d764) - sum));   // 0048316E..00483179
        return;
    }
    const double inverse = static_cast<double>(kOne_0073d764) / std::sqrt(sum);   // 00483180 fsqrt / 00483188 fdivr
    put_raw(&out->w, 0u);                                                    // 00483182
    out->x = f32(static_cast<double>(x) * inverse);                         // 0048318E..00483194
    out->y = f32(static_cast<double>(y) * inverse);                         // 00483197..0048319D
    out->z = f32(inverse * z);                                               // 004831A0..004831A4
}

// 004831B0 NewController::GetQuaternionValue(float t, MdlNode* node, Quaternion* out), RET 0Ch, EAX 1/0.
U __fastcall controller_get_quaternion_value_004831b0(void* controller, void*, float t, void* node, Quaternion* out) {
    const U format = columns(controller);                                    // 004831B5..004831BD
    if (format != 4u && format != 2u) return 0;                              // 004831C0..004831D2
    I index; float alpha;                                                    // node slot / t slot
    controller_get_key_index_and_alpha_00482b70(controller, nullptr, t, node, &index, &alpha);   // 004831D5..004831EB
    U guard = anim_quaternion_guard_00830328;                                // 004831F0
    if ((guard & 1u) == 0) {                                                 // 004831F5
        guard |= 1u; anim_quaternion_guard_00830328 = guard;                 // 004831F9..004831FC
        put_raw(&anim_quaternion_key_a_00830318.z, 0u); put_raw(&anim_quaternion_key_a_00830318.y, 0u);   // 00483201 / 0048320B
        put_raw(&anim_quaternion_key_a_00830318.x, 0u); put_raw(&anim_quaternion_key_a_00830318.w, 0x3F800000u); // 00483215 / 0048321F
    }
    if ((guard & 2u) == 0) {                                                 // 00483229
        guard |= 2u; anim_quaternion_guard_00830328 = guard;                 // 0048322D..00483230
        put_raw(&anim_quaternion_key_b_00830308.z, 0u); put_raw(&anim_quaternion_key_b_00830308.y, 0u);   // 00483235 / 0048323F
        put_raw(&anim_quaternion_key_b_00830308.x, 0u); put_raw(&anim_quaternion_key_b_00830308.w, 0x3F800000u); // 00483249 / 00483253
    }
    if (index == 0) {                                                        // 0048325D..00483263
        controller_get_quaternion_from_index_00483050(controller, nullptr, 0, node, out);   // 00483265..0048326E
        return 1;
    }
    const I n = rows(controller);                                            // 00483281
    if (index == n) {                                                        // 00483285
        controller_get_quaternion_from_index_00483050(controller, nullptr, n, node, out);   // 00483289..00483292
        return 1;
    }
    controller_get_quaternion_from_index_00483050(controller, nullptr, index, node, &anim_quaternion_key_a_00830318);      // 004832A5..004832AE
    controller_get_quaternion_from_index_00483050(controller, nullptr, index + 1, node, &anim_quaternion_key_b_00830308);  // 004832B3..004832BD
    Quaternion result;                                                       // [esp+0Ch]
    quaternion_slerp_004a9e00(&result, anim_quaternion_key_a_00830318, &anim_quaternion_key_b_00830308, alpha);         // 004832C2..004832F9
    *out = result;                                                           // 004832FE..0048331F
    return 1;
}

// 00483330 NewController::Control(Part* part, MdlNode* node, float t0, float t1, float weight), RET 14h.
// t0 < -95 selects "blend the Part's current value toward the value at t1"; otherwise blend value(t0) -> value(t1).
void __fastcall controller_control_00483330(void* controller, void*, void* part, void* node, float t0, float t1, float weight) {
    const U format = columns(controller);                                    // 00483336..0048333E
    if (format == 1u) {                                                      // 00483341
        float* target = static_cast<float*>(at(part, static_cast<U>(target_offset(controller))));   // 0048334A..00483361
        std::memcpy(&anim_control_current_00830330, target, 4);              // 00483358 mov eax,[edi+ebx] / 00483366
        controller_get_float_value_00482bf0(controller, nullptr, t1, node, &anim_control_value_00830334);   // 0048335B..0048336B
        if (base_offset(controller) != -1) {                                 // 00483370..00483378
            void* base_node = field<void*>(part, 0x04);                      // 00483383 mov eax,[ebx+4]
            anim_control_value_00830334 = f32(static_cast<double>(anim_control_value_00830334) +
                field<float>(base_node, static_cast<U>(static_cast<I>(base_offset(controller)))));   // 0048337A..00483389
        }
        if (t0 < kMinus95_00743254) {                                        // 0048338F..0048339E
            *target = f32((static_cast<double>(kOne_0073d764) - weight) * bits_to_float(anim_control_current_00830330) +
                          static_cast<double>(anim_control_value_00830334) * weight);   // 004833A0..004833BC
            return;
        }
        controller_get_float_value_00482bf0(controller, nullptr, t0, node, &anim_control_previous_0083032c);   // 004833C8..004833D5
        *target = f32((static_cast<double>(kOne_0073d764) - weight) * anim_control_previous_0083032c +
                      static_cast<double>(anim_control_value_00830334) * weight);       // 004833DA..004833F6
        return;
    }
    if (format == 3u) {                                                      // 00483402
        Vector* target = static_cast<Vector*>(at(part, static_cast<U>(target_offset(controller))));   // 0048340B..00483441
        Vector previous = {0.0f, 0.0f, 0.0f};                                // 00483411..00483421 [esp+1Ch]
        Vector value = {0.0f, 0.0f, 0.0f};                                   // 00483429..00483439 [esp+10h]
        const Vector current = *target;                                      // 00483443..0048345E [esp+2Ch]
        controller_get_vector_value_00482d20(controller, nullptr, t1, node, &value);   // 00483459..0048346A
        const Vector* base = nullptr;
        if (base_offset(controller) != -1) {                                 // 0048346F..00483477
            base = static_cast<const Vector*>(at(field<void*>(part, 0x04), static_cast<U>(static_cast<I>(base_offset(controller)))));   // 00483479..0048347F
            vector_add_assign_004a9740(&value, nullptr, base);               // 00483481..00483486
        }
        Vector result;                                                       // [esp+3Ch]
        if (t0 < kMinus95_00743254) {                                        // 00483491..004834A0
            const Vector* r = vector_interpolate_0046eea0(&result, &current, &value, weight);   // 004834A2..004834B6
            *target = *r;                                                    // 004834BB..004834CB
            return;
        }
        controller_get_vector_value_00482d20(controller, nullptr, t0, node, &previous);   // 004834D8..004834E9
        if (base_offset(controller) != -1) vector_add_assign_004a9740(&previous, nullptr, base);   // 004834EE..004834FA
        const Vector* r = vector_interpolate_0046eea0(&result, &previous, &value, weight);   // 004834FF..00483513
        *target = *r;                                                        // 00483518..00483528
        return;
    }
    if (format != 4u && format != 2u) return;                                // 00483535..0048353D
    Quaternion* target = static_cast<Quaternion*>(at(part, static_cast<U>(target_offset(controller))));   // 00483543..00483569
    Quaternion value; put_raw(&value.w, 0x3F800000u); put_raw(&value.x, 0u); put_raw(&value.y, 0u); put_raw(&value.z, 0u); // 00483549..00483561 [esp+3Ch]
    Quaternion current = *target;                                            // 0048356F..00483589 [esp+1Ch]
    Quaternion scratch; put_raw(&scratch.w, 0x3F800000u); put_raw(&scratch.x, 0u); put_raw(&scratch.y, 0u); put_raw(&scratch.z, 0u); // 00483595..004835AD [esp+2Ch]
    controller_get_quaternion_value_004831b0(controller, nullptr, t1, node, &value);   // 00483584..004835B9
    if (t0 < kMinus95_00743254) {                                            // 004835BE..004835CD
        const Quaternion* r = quaternion_interpolate_00481aa0(&scratch, &current, &value, weight);   // 004835CF..00483604
        *target = *r;                                                        // 00483609..0048361C
        return;
    }
    controller_get_quaternion_value_004831b0(controller, nullptr, t0, node, &scratch);   // 004835DE..004835EB
    const Quaternion* r = quaternion_interpolate_00481aa0(&current, &scratch, &value, weight);   // 004835F0..00483604
    *target = *r;                                                            // 00483609..0048361C
}

// 00484EC0 NewController::ReportKey(Part* part, MdlNode* node, int key, float delta), RET 10h.
void __fastcall controller_report_key_00484ec0(void* controller, void*, void* part, void* node, I key, U raw_delta) {
    KeyEvent event{};                                                        // D2: pad bytes [2D..2F] are never stored by the original (00484EF8 stores one byte); value-initialized
    event.time = raw_delta;                                                  // 00484EC3 / 00484ECE
    event.position = nullptr;                                                // 00484EF0
    event.orientation = nullptr;                                             // 00484EF4
    event.position_key = 1;                                                  // 00484EF8
    void* gob = field<void*>(part, 0x40);                                    // 00484ED9..00484EDD / 00484EFC
    const float key_time = node_data(node)[time_index(controller) + key];    // 00484ED5..00484F08 (fld/fstp: float -> float)
    const U format = columns(controller);                                    // 00484F03..00484F0C
    Vector position; Quaternion orientation;                                 // [esp+14h] / [esp+30h]
    if (format == 1u) return;                                                // 00484F0F..00484F11
    if (format == 3u) {                                                      // 00484F17
        position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;            // 00484F2D..00484F35
        controller_get_vector_value_00482d20(controller, nullptr, key_time, node, &position);   // 00484F20..00484F39
        event.position = &position;                                          // 00484F3E..00484F42
        event.position_key = 1;                                              // 00484F4D
        put_raw(&orientation.z, 0u); put_raw(&orientation.y, 0u); put_raw(&orientation.x, 0u); put_raw(&orientation.w, 0x3F800000u); // 00484F51..00484F5D
        U found = 0;                                                         // 00484F49
        for (I j = 0; j < controller_count(node); ++j) {                     // 00484F46..00484F9C (count reloaded)
            void* other = controller_at(node, j);                            // 00484F70..00484F73
            if (other == controller || target_offset(other) != 0x14) continue;   // 00484F76..00484F7D
            found = controller_get_quaternion_value_004831b0(other, nullptr, key_time, node, &orientation);   // 00484F7F..00484F8A
            if (found != 0) break;                                           // 00484F8F..00484F91
        }
        if (found == 0) orientation = field<Quaternion>(part, 0x14);         // 00484F9E..00484FC0 (Part orientation)
        event.orientation = &orientation;                                    // 00484FC4..00484FC8
    } else if (format == 4u || format == 2u) {                               // 00484FD1..00484FD9
        put_raw(&orientation.z, 0u); put_raw(&orientation.y, 0u); put_raw(&orientation.x, 0u); put_raw(&orientation.w, 0x3F800000u); // 00484FEC..00485004
        controller_get_quaternion_value_004831b0(controller, nullptr, key_time, node, &orientation);   // 00484FDF..0048500C
        event.orientation = &orientation;                                    // 00485011..00485015
        event.position_key = 0;                                              // 0048501C
        position.x = 0.0f; position.y = 0.0f; position.z = 0.0f;            // 00485021..00485031
        for (I j = 0; j < controller_count(node); ++j) {                     // 00485019 / 00485062..0048506B
            void* other = controller_at(node, j);                            // 00485040..00485043
            if (other == controller || target_offset(other) != 8) continue;  // 00485045..0048504C
            if (controller_get_vector_value_00482d20(other, nullptr, key_time, node, &position) != 0) {   // 0048504E..00485060
                event.position = &position;                                  // 0048506F..00485073
                break;
            }
        }
    } else {
        return;                                                              // 00484FD9 jne 0048508D
    }
    gob_table(gob)->do_event(as_gob(gob), nullptr, kReportKey_00743268, as_word(&event));   // 00485077..00485087
}

// 00485250 NewController::ReportKeys(Part* part, MdlNode* node, float t0, float t1), RET 10h.
void __fastcall controller_report_keys_00485250(void* controller, void*, void* part, void* node, float t0, float t1) {
    if (field<void*>(part, 0x40) == nullptr) return;                         // 00485250..00485261
    if (t0 < kZero_0073d700) put_raw(&t0, 0u);                               // 00485267..00485287
    const I first = time_index(controller);                                  // 00485273 / 00485281
    if (t1 > anim_report_length_008302d0) t1 = anim_report_length_008302d0;  // 0048528B..004852A2
    const I n = rows(controller);                                            // 004852A6
    KeyEvent event{};                                                        // D2 (see ReportKey)
    event.position = nullptr; event.orientation = nullptr; event.position_key = 1;   // 004852AE..004852B6
    float sum = 0.0f;                                                        // 004852BB [esp+38h]
    I i0 = 0, i1 = 0;                                                        // 004852C3 / 004852C7
    if (n > 0) {                                                             // 004852CB
        const float* times = node_data(node) + first;                        // 004852CD..004852D0
        while (t0 > times[i0]) {                                             // 004852D3..004852DE (test ah,41h; jne exits)
            ++i0; if (!(i0 < n)) break;                                      // 004852E0..004852E6
        }
        while (t1 > times[i1]) {                                             // 00485300..0048530B
            ++i1; if (!(i1 < n)) break;                                      // 0048530D..00485313
        }
    }
    if ((anim_report_reverse_008302c8 & 1u) != 0) return;                    // 0048531D..00485324 (test byte ptr)
    if (t0 < t1) {                                                           // 0048532A..00485337
        for (I k = i0; k < i1; ++k) {                                        // 00485339..0048539B
            const float* d = node_data(node);
            const float delta = (k == i0) ? f32(static_cast<double>(d[first + k]) - t0)            // 00485357..0048535D
                                          : f32(static_cast<double>(d[first + k]) - d[first + k - 1]); // 00485363..0048536B
            controller_report_key_00484ec0(controller, nullptr, part, node, k, float_to_bits(delta));   // 0048536E..00485380
            sum = f32(static_cast<double>(sum) + delta);                     // 00485385..00485397
        }
    } else {
        const float* d = node_data(node);
        const float base = (i0 == n) ? f32(static_cast<double>(anim_report_length_008302d0) - t0)     // 004853A2..004853B0
                                     : f32(static_cast<double>(anim_report_length_008302d0) - d[n + first - 1]);   // 004853B2..004853BE
        for (I k = i0; k < rows(controller); ++k) {                          // 004853C2..00485423 (rows reloaded)
            const float* q = node_data(node);
            // Original quirk kept: the first wrapped key reads data[k] without the key-time offset (004853DE..004853E4).
            const float delta = (k == i0 && k < rows(controller)) ? f32(static_cast<double>(q[k]) - t0)
                                                   : f32(static_cast<double>(q[first + k]) - q[first + k - 1]);   // 004853EA..004853F3
            controller_report_key_00484ec0(controller, nullptr, part, node, k, float_to_bits(delta));   // 004853F6..00485408
            sum = f32(static_cast<double>(sum) + delta);                     // 0048540D..0048541F
        }
        for (I k = 0; k < i1; ++k) {                                         // 00485429..00485481
            const float* q = node_data(node);
            const float delta = (k == 0) ? f32(static_cast<double>(base) + q[first])                  // 00485434..0048543F
                                         : f32(static_cast<double>(q[first + k]) - q[first + k - 1]); // 00485444..00485454
            controller_report_key_00484ec0(controller, nullptr, part, node, k, float_to_bits(delta));   // 00485457..00485469
            sum = f32(static_cast<double>(sum) + delta);                     // 0048546E..0048547D
        }
    }
    event.time = float_to_bits(f32(static_cast<double>(*anim_cells.frame_delta_0078e574) - sum));   // 00485483..0048549E
    event.orientation = nullptr; event.position = nullptr;                   // 00485493 / 00485497
    event.position_key = (columns(controller) == 3u) ? 1 : 0;                // 0048549B..004854A9
    void* gob = field<void*>(part, 0x40);                                    // 00485489 / 004854AD
    gob_table(gob)->do_event(as_gob(gob), nullptr, kReportKey_00743268, as_word(&event));   // 004854AD..004854BC
}

// ======================================================================= MdlNode InternalControl family
// 004854D0 MdlNode::InternalControl(Part* part, float t0, float t1, float weight), RET 10h.
void __fastcall node_internal_control_004854d0(void* node, void*, void* part, float t0, float t1, float weight) {
    part_table(part)->s02c(part, nullptr);                                   // 004854D5..004854DD (result unused)
    for (I i = 0; i < controller_count(node); ++i) {                         // 004854E0..00485556 (count reloaded)
        controller_control_00483330(controller_at(node, i), nullptr, part, node, t0, t1, weight);   // 004854F2..00485504
        if (anim_report_run_008302cc != nullptr) {                           // 00485509..00485510
            const float reported = (t0 < kMinus95_00743254) ? f32(static_cast<double>(kMinus100_0074186c) - t0) : t0;   // 00485512..00485538
            controller_report_keys_00485250(controller_at(node, i), nullptr, part, node, reported, t1);   // 00485516..00485540
        }
    }
}
// 004855D0 MdlNodeCamera::InternalControl: part vtbl+28 (result unused) then MdlNode::InternalControl.
void __fastcall node_camera_internal_control_004855d0(void* node, void*, void* part, float t0, float t1, float weight) {
    part_table(part)->s028(part, nullptr);                                   // 004855D5..004855DC (result unused)
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);     // 004855DF..004855F1
}
// 00485600 MdlNodeReference::InternalControl: part vtbl+38 then MdlNode::InternalControl.
void __fastcall node_reference_internal_control_00485600(void* node, void*, void* part, float t0, float t1, float weight) {
    part_table(part)->s038(part, nullptr);                                   // 00485605..0048560C (result unused)
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);     // 0048560F..00485621
}
// 00485630 MdlNodeTriMesh::InternalControl: part vtbl+2C then MdlNode::InternalControl.
void __fastcall node_trimesh_internal_control_00485630(void* node, void*, void* part, float t0, float t1, float weight) {
    part_table(part)->s02c(part, nullptr);                                   // 00485635..0048563C (result unused)
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);     // 0048563F..00485651
}

// 00485660 MdlNodeAnimMesh::InternalControl(Part* part, float t0, float t1, float weight), RET 10h.
// Blends the anim-mesh vertex positions (pool vp+1C, 12-byte rows) and texture coordinates (pool vp+20, 8-byte rows)
// toward the sampled frames (node+1C4 / node+1C8, sample period node+19C, frame counts node+1CC / node+1D0), then
// runs MdlNode::InternalControl. Register-resident x87 values are `double`; every fstp dword is a float store.
void __fastcall node_anim_mesh_internal_control_00485660(void* node, void*, void* part, float t0, float t1, float weight) {
    void* anim_part = as_object(part_table(part)->s030(part, nullptr));      // 00485668..00485672 (ebp, [esp+1Ch])
    void* held = anim_part;                                                  // [esp+1Ch]
    const double w = weight;
    const float period = field<float>(node, 0x19C);
    I frame;                                                                 // edi / [esp+10h]
    if (period == kZero_0073d700) frame = 0;                                 // 00485675..0048568C (jnp: equal) -> 00485849
    else frame = ftol2(static_cast<double>(t1) / period);                    // 00485692..0048569F __ftol2
    bool interpolate = (period == kZero_0073d700) || frame >= 0;             // 004856A6..004856AC (jl -> clamp)
    if (interpolate && !(frame < field<I>(node, 0x1CC) - 1)) interpolate = false;   // 004856B2..004856BB (jge -> clamp)
    if (interpolate) {
        const float frac = f32(static_cast<double>(t1) / field<float>(node, 0x19C) - static_cast<double>(frame));   // 004856C1..004856D7 fisub (node+19C re-read)
        void* object = field<void*>(anim_part, 0x70);                        // 004856C8
        void* primitive = slots<MeshObjectSlots>(object)->slot0c(object, nullptr);   // 004856D1..004856DB (ebp)
        unsigned char* vertex = static_cast<unsigned char*>(aurora_pool_lock_00423f10(field<U>(primitive, 0x1C), 0, 0, 1));   // 004856DE..004856EA
        if (field<I16>(node, 0x180) > 0) {                                   // 004856EF..004856FB
            const double s = static_cast<double>(kOne_0073d764) - frac;     // 00485701..0048570A (kept in ST)
            const float keep = f32(static_cast<double>(kOne_0073d764) - weight);   // 0048570E..0048571B [esp+18h]
            for (I k = 0; k < static_cast<I>(field<I16>(node, 0x180)); ++k, vertex += 12) {   // 00485822..0048582F
                float* v = reinterpret_cast<float*>(vertex);
                const float z = v[2];                                        // 00485720 / 0048572E [esp+7Ch] (raw)
                const double p0 = v[0], p1 = v[1];                           // 00485722 / 0048572B
                const float* a = field<float*>(node, 0x1C4) + 3 * (field<I>(node, 0x1CC) * k + frame);   // 00485725..00485740
                const double a0 = a[0], a1 = a[1];                           // 00485740 / 00485743
                const float a2 = a[2];                                       // 00485747..0048574B [esp+64h]
                const double b0 = a[3], b1 = a[4];                           // 0048574F / 00485753
                const float b2 = a[5];                                       // 00485757..0048575B [esp+58h]
                const float fb0 = f32(b0 * frac);                            // 0048575F..00485765 [20]
                const float fb1 = f32(b1 * frac);                            // 00485769..0048576D [24]
                const float fb2 = f32(static_cast<double>(b2) * frac);      // 00485771..00485779 [28]
                const float sa0 = f32(s * a0);                               // 0048577D..00485781 [68]
                const double sa1 = s * a1;                                   // 00485785..0048578B (wide)
                const double sa2 = s * a2;                                   // 0048578D..0048578F (wide)
                const float ix = f32(static_cast<double>(sa0) + fb0);       // 00485793..0048579B [38]
                const float iy = f32(sa1 + fb1);                             // 0048579F..004857A5 [3C]
                const double iz = sa2 + fb2;                                 // 004857A9 (wide)
                const float wx = f32(static_cast<double>(ix) * w);          // 004857AD..004857B8 [2C]
                const float wy = f32(static_cast<double>(iy) * w);          // 004857BC..004857C7 [30]
                const float wz = f32(iz * w);                                // 004857CB..004857D2 [34]
                const float kx = f32(static_cast<double>(keep) * p0);       // 004857D6..004857DC [44]
                const float ky = f32(static_cast<double>(keep) * p1);       // 004857E0..004857E6 [48]
                const double kz = static_cast<double>(keep) * z;            // 004857EE..004857F2 (wide)
                const double x = static_cast<double>(kx) + wx;              // 004857F6..004857FA
                const double y = static_cast<double>(ky) + wy;              // 004857FE..00485802
                const float zf = f32(kz + wz);                               // 00485806..00485813 [esp+88h]
                v[0] = f32(x);                                               // 0048581A
                v[1] = f32(y);                                               // 0048581D
                v[2] = zf;                                                   // 00485820 (raw edx)
            }
        }
        aurora_pool_unlock_00423f30(field<U>(primitive, 0x1C));              // 00485837..0048583B
    } else if (field<I>(node, 0x1CC) != 0) {                                 // 00485854..0048585C
        void* object = field<void*>(anim_part, 0x70);                        // 00485862
        void* primitive = slots<MeshObjectSlots>(object)->slot0c(object, nullptr);   // 00485865..00485867
        held = primitive;                                                    // 0048586E mov [esp+24h],eax (= [esp+1Ch])
        unsigned char* vertex = static_cast<unsigned char*>(aurora_pool_lock_00423f10(field<U>(primitive, 0x1C), 0, 0, 1));   // 0048586A..00485878
        if (frame < 0) frame = 0;                                            // 00485880..00485886
        if (frame > field<I>(node, 0x1CC) - 1) frame = field<I>(node, 0x1CC) - 1;   // 0048588A..00485897
        if (field<I16>(node, 0x180) > 0) {                                   // 0048589B..004858A4
            const double s = static_cast<double>(kOne_0073d764) - weight;   // 004858AA..004858B3 (kept in ST)
            for (I k = 0; k < static_cast<I>(field<I16>(node, 0x180)); ++k, vertex += 12) {   // 004858E0..0048596A
                float* v = reinterpret_cast<float*>(vertex);
                const float z = v[2];                                        // 004858C0 / 004858CE [esp+7Ch]
                const double p0 = v[0], p1 = v[1];                           // 004858C2 / 004858CB
                const float* a = field<float*>(node, 0x1C4) + 3 * (field<I>(node, 0x1CC) * k + frame);   // 004858C5..004858DD
                const double a0 = a[0], a1 = a[1];                           // 004858E4 / 004858E7
                const float a2 = a[2];                                       // 004858EB..004858EF [esp+88h]
                const float wa0 = f32(a0 * w);                               // 004858F6..004858FF [2C]
                const float wa1 = f32(a1 * w);                               // 00485903..0048590A [30]
                const float wa2 = f32(static_cast<double>(a2) * w);         // 0048590E..0048591C [34]
                const float sp0 = f32(s * p0);                               // 00485920..00485924 [44]
                const float sp1 = f32(s * p1);                               // 00485928..0048592C [48]
                const double sz = static_cast<double>(z) * s;               // 00485934..00485938 (wide)
                const double x = static_cast<double>(sp0) + wa0;            // 0048593A..0048593E
                const double y = static_cast<double>(sp1) + wa1;            // 00485942..00485946
                const float zf = f32(sz + wa2);                              // 0048594A..00485954 [esp+70h]
                v[0] = f32(x);                                               // 00485958
                v[2] = zf;                                                   // 0048595B (raw edx)
                v[1] = f32(y);                                               // 0048595E
            }
        }
        aurora_pool_unlock_00423f30(field<U>(held, 0x1C));                   // 00485972..0048597A
    }
    if (field<I>(node, 0x1D0) != 0) {                                        // 00485982..0048598A
        void* object = field<void*>(anim_part, 0x70);                        // 00485990 (ebp = anim part on both paths)
        void* primitive = slots<MeshObjectSlots>(object)->slot0c(object, nullptr);   // 00485993..00485995 (ebp)
        unsigned char* uv = static_cast<unsigned char*>(aurora_pool_lock_00423f10(field<U>(primitive, 0x20), 0, 0, 1));   // 00485998..004859A4
        const I uv_frames = field<I>(node, 0x1D0);
        if (frame >= 0 && frame < uv_frames - 1) {                           // 004859AC..004859BD
            const float frac = f32(static_cast<double>(t1) / field<float>(node, 0x19C) - static_cast<double>(frame));   // 004859C3..004859DD [esp+18h] (node+19C re-read)
            if (field<I16>(node, 0x180) > 0) {                               // 004859CA..004859E1
                const double s = static_cast<double>(kOne_0073d764) - frac; // 004859E7..004859F0 (kept)
                const float keep = f32(static_cast<double>(kOne_0073d764) - weight);   // 004859F4..00485A01 [esp+14h]
                for (I k = 0; k < static_cast<I>(field<I16>(node, 0x180)); ++k) {      // 00485A29 / 00485ABD..00485AC6
                    float* t = reinterpret_cast<float*>(uv + 8 * k);
                    const double u0 = t[0], u1 = t[1];                       // 00485A16 / 00485A19
                    const float* a = field<float*>(node, 0x1C8) + (2 * k * field<I>(node, 0x1D0) + 2 * frame);   // 00485A10..00485A27
                    const double a0 = a[0], a1 = a[1];                       // 00485A2A / 00485A2D
                    const float fb0 = f32(static_cast<double>(a[2]) * frac); // 00485A35..00485A3D [44]
                    const float fb1 = f32(static_cast<double>(a[3]) * frac); // 00485A31 / 00485A41..00485A45 [48]
                    const float sa0 = f32(s * a0);                           // 00485A49..00485A4D [80]
                    const double sa1 = s * a1;                               // 00485A54..00485A5A (wide)
                    const double ix = static_cast<double>(sa0) + fb0;       // 00485A5C..00485A63 (wide)
                    const float iy = f32(sa1 + fb1);                         // 00485A67..00485A6D [78]
                    const float wx = f32(ix * w);                            // 00485A71..00485A78 [2C]
                    const float wy = f32(static_cast<double>(iy) * w);      // 00485A7C..00485A87 [30]
                    const float k0 = f32(static_cast<double>(keep) * u0);   // 00485A8B..00485A91 [38]
                    const float k1 = f32(static_cast<double>(keep) * u1);   // 00485A95..00485A9B [3C]
                    t[0] = f32(static_cast<double>(k0) + wx);               // 00485AA3..00485AA7 / 00485AB5
                    t[1] = f32(static_cast<double>(k1) + wy);               // 00485AAB..00485AAF / 00485AB9
                }
            }
        } else if (uv_frames != 0) {                                         // 00485AD1..00485AD9
            I j = (frame <= 0) ? 0 : frame;                                  // 00485ADF..00485AE7 (setle/dec/and)
            if (!(j < uv_frames - 1)) j = uv_frames - 1;                     // 00485AE9..00485AEE
            if (field<I16>(node, 0x180) > 0) {                               // 00485AF0..00485AF9
                const double s = static_cast<double>(kOne_0073d764) - weight;   // 00485AFF..00485B08 (kept)
                for (I k = 0; k < static_cast<I>(field<I16>(node, 0x180)); ++k) {      // 00485B2C / 00485B76..00485B7F
                    float* t = reinterpret_cast<float*>(uv + 8 * k);
                    const double u0 = t[0], u1 = t[1];                       // 00485B16 / 00485B19
                    const float* a = field<float*>(node, 0x1C8) + (2 * k * field<I>(node, 0x1D0) + 2 * j);   // 00485B10..00485B29
                    const float wa0 = f32(static_cast<double>(a[0]) * w);   // 00485B30..00485B39 [38]
                    const float wa1 = f32(static_cast<double>(a[1]) * w);   // 00485B2D / 00485B3D..00485B44 [3C]
                    const float s0 = f32(s * u0);                            // 00485B48..00485B4C [44]
                    const float s1 = f32(s * u1);                            // 00485B50..00485B54 [48]
                    t[0] = f32(static_cast<double>(s0) + wa0);              // 00485B5C..00485B60 / 00485B6E
                    t[1] = f32(static_cast<double>(s1) + wa1);              // 00485B64..00485B68 / 00485B72
                }
            }
        }
        aurora_pool_unlock_00423f30(field<U>(primitive, 0x20));              // 00485B83..00485B87
    }
    part_table(part)->s02c(part, nullptr);                                   // 00485B8F..00485B9A (result unused)
    node_internal_control_004854d0(node, nullptr, part, t0, t1, weight);     // 00485B9D..00485BB8
}

// 0043F890 MdlNode::ControlDispatch(Part* part, float t0, float t1, float weight), RET 10h: switch on the node type word
// (byte table 0043F9E4, jump table 0043F9C8) and tail-jump into the type's InternalControl with ECX = AsType(node).
void __fastcall aurora_node_control_dispatch_0043f890(aurora_model::MdlNode* self, void* unused_edx,
    aurora_model::Part* part_in, aurora_model::U raw_time, aurora_model::U raw_a, aurora_model::U raw_weight) {
    (void)unused_edx;
    void* node = self; void* part = part_in;
    const float t0 = bits_to_float(raw_time), t1 = bits_to_float(raw_a), weight = bits_to_float(raw_weight);
    const U16 type = field<U16>(node, 0);                                    // 0043F890 mov ax,[ecx]
    const I sx = static_cast<I>(static_cast<I16>(type));                     // 0043F893 movsx
    auto as8 = [&](U8 mask) -> void* { return (static_cast<U8>(type) & mask) == mask ? node : nullptr; };      // and al,K; sub; neg; sbb; not; and
    auto as16 = [&](U16 mask) -> void* { return static_cast<U16>(type & mask) == mask ? node : nullptr; };     // and ax,K; ...
    switch (sx) {
    case 0x001: node_internal_control_004854d0(node, nullptr, part, t0, t1, weight); return;                  // 0043F9BF (table entry 0)
    case 0x003: node_light_internal_control_00485560(as8(0x03), nullptr, part, t0, t1, weight); return;      // 0043F8B9..0043F8C7
    case 0x005: node_emitter_internal_control_00485590(as8(0x05), nullptr, part, t0, t1, weight); return;    // 0043F8CC..0043F8DA
    case 0x009: node_camera_internal_control_004855d0(as8(0x09), nullptr, part, t0, t1, weight); return;     // 0043F8DF..0043F8ED
    case 0x011: node_reference_internal_control_00485600(as8(0x11), nullptr, part, t0, t1, weight); return;  // 0043F8F2..0043F900
    case 0x021: node_trimesh_internal_control_00485630(as8(0x21), nullptr, part, t0, t1, weight); return;    // 0043F905..0043F913
    case 0x061: node_trimesh_internal_control_00485630(as8(0x61), nullptr, part, t0, t1, weight); return;    // 0043F918..0043F926
    case 0x0A1: node_anim_mesh_internal_control_00485660(as8(0xA1), nullptr, part, t0, t1, weight); return;  // 0043F95B..0043F969
    case 0x121: node_trimesh_internal_control_00485630(as16(0x121), nullptr, part, t0, t1, weight); return;  // 0043F945..0043F956
    case 0x221: node_trimesh_internal_control_00485630(as16(0x221), nullptr, part, t0, t1, weight); return;  // 0043F96E..0043F981
    case 0x401: node_internal_control_004854d0(as16(0x401), nullptr, part, t0, t1, weight); return;         // 0043F9AC..0043F9BF
    case 0x821: node_trimesh_internal_control_00485630(as16(0x821), nullptr, part, t0, t1, weight); return;  // 0043F996..0043F9A7
    default: return;                                                         // 0043F9C4 ret 10h
    }
}

// 00440110 Part::Animate(float dt), Part vtbl+6C, RET 4: forward to each child's vtbl+6C. Raw-word slot contract
// (model_vtables.hpp Slot1): EAX on return is the last value loaded from [part+34h] (00440114 / 0044012E).
U __fastcall part_animate_00440110(void* self, void* unused_edx, U raw_dt) {
    (void)unused_edx;
    U count = field<U>(self, 0x34);                                          // 00440114
    for (I i = 0; i < static_cast<I>(count); count = field<U>(self, 0x34)) { // 0044012E..00440134 (count reloaded)
        void* child = as_object(reinterpret_cast<const U*>(as_object(field<U>(self, 0x30)))[i]);   // 00440122..00440125
        part_table(child)->s06c(child, nullptr, raw_dt);                     // 00440128..0044012B
        ++i;                                                                 // 00440131
    }
    return count;
}
} // extern "C"
