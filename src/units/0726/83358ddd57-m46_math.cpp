// lane-m46-3dcore fork D: core math / list / wind helpers referenced by the BSP, tile, room, construct and light
// units of the M45 cohort. Declarations are the stage's own (bsp.h, tile_scene.h, triangle_bin.h, bsp_callbacks.h;
// float_list_assign_00438a50 is declared only inside construct/light_manager.cpp, repeated here verbatim).
// Conventions: D1 (thiscall = __fastcall(self, edx_unused, ...)), D21 (x87 FCOS/FSIN and the genuine __ftol2 port
// through inline asm), D30-FINAL (wide intermediates double, float only at fstp dword; operand order of every
// fadd/fmul chain kept; compare idioms as the bsp.h table incl. the unordered case), allocator family
// 006FA7E6/006FA390 (and their thunks 006FC0D2/006FC0CD) -> live_operator_new / live_free (M41).
#include "558510f9c9-tile_scene.h"
#include "c0efdcfcef-bsp_callbacks.h"
#include "647ed12135-m46_camera_externs.h"

namespace kotor_world_los {   // world-los-world-los-cells owner of 007A19B0 (D55 area)
extern kotor_scene3d::Vector up_007a19b0;
}

extern "C" {
using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::Plane; using kotor_scene3d::PlaneList; using kotor_scene3d::PointerList;
// .data axis vectors read (never written: full .text cross reference 004ACAC4..004ACE85) by YawPitchRoll and its
// siblings; cold values from the pinned image. 007A19B0 (0,0,1) is owned by lane-world-los.
Vector x_axis_007a1998 = {1.0f, 0.0f, 0.0f};     // .data 0000803F 00000000 00000000
Vector y_axis_007a19a4 = {0.0f, 1.0f, 0.0f};     // .data 00000000 0000803F 00000000
// WindManager::GetGlobalWindNoPoint function statics: Quaternion 00831E18 (w,x,y,z) + guard 00831E28 bit 1
// (BSS; referenced only by 004A5950..004A5BD8).
Quaternion wind_rotation_00831e18 = {0.0f, 0.0f, 0.0f, 0.0f};
volatile U wind_rotation_guard_00831e28 = 0u;

// construct/light_manager.cpp spelling (core): CExoArrayList<float>::operator= 00438A50 thiscall RET 4, EAX = this
kotor_reconstruction::ArrayMutations* __fastcall float_list_assign_00438a50(kotor_reconstruction::ArrayMutations* self, void*,
                                                                            const kotor_reconstruction::ArrayMutations* source);
Vector* __fastcall vector_subtract_assign_004a9770(Vector* self, void* unused_edx, const Vector* other);
}

namespace {
using namespace kotor_scene3d;
constexpr float kZero = 0.0f;                     // [0073D700]
constexpr float kOne = 1.0f;                      // [0073D764]
constexpr float kHalf = 0.5f;                     // [0073E9AC]
constexpr float kDegToRad = 0.01745329238474369f; // [007415F8] 0x3C8EFA35

inline double x87_cos(double x) {
    double r;
    __asm {
        fld qword ptr x
        fcos
        fstp qword ptr r
    }
    return r;
}
inline double x87_sin(double x) {
    double r;
    __asm {
        fld qword ptr x
        fsin
        fstp qword ptr r
    }
    return r;
}
// `fld dword ptr [value]; call 006FAE8C` -> EAX (low DWORD of the __ftol2 result), D30-FINAL original CRT port.
inline I x87_ftol_float(const float* value) {
    I r;
    __asm {
        mov ecx, value
        fld dword ptr [ecx]
        call genuine_x87_ftol_006fae8c
        mov r, eax
    }
    return r;
}
// `shl eax,2; cdq; xor eax,edx; sub eax,edx; and 8000000Fh; jns; dec; or 0FFFFFFF0h; inc` = abs(v*4) % 16 with
// the C signed remainder (wrapping arithmetic, no UB).
inline I wind_cell(I v) {
    const U shifted = static_cast<U>(v) << 2;
    const U sign = (shifted & 0x80000000u) ? 0xFFFFFFFFu : 0u;
    const I magnitude = static_cast<I>((shifted ^ sign) - sign);
    return magnitude % 16;
}

// CAurTriangleListElem (18h): +00 CExoArrayList<float> {data,count,capacity}, +0C/+10/+14 words (bsp triangle_bin.h).
struct FloatList { float* data; I count; I capacity; };
struct TriangleListElem { FloatList corners; U face_0c; U part_10; U count_14; };
static_assert(sizeof(TriangleListElem) == 0x18, "CAurTriangleListElem");

// float through `fld dword / fstp dword` (x87 load/store: quiets a signalling NaN; every other value exact).
inline float x87_float_copy(float v) { const double wide = static_cast<double>(v); return static_cast<float>(wide); }

// Shared append loop of 004A7340 / 00438A50: grow (factor 2, `initial` when empty), copy, free, append.
inline void float_list_append_all(FloatList* self, const FloatList* source, I initial) {
    for (I i = 0; i < source->count; ++i) {
        const float value = x87_float_copy(source->data[i]);           // fld [eax+ebx*4]; fstp [esp+14h]
        if (self->count == self->capacity) {                           // cmp ecx,eax / jne
            const I capacity = (self->capacity != 0) ? self->capacity * 2 : initial;
            float* old = self->data;
            self->capacity = capacity;                                 // stored before the allocation
            self->data = static_cast<float*>(live_operator_new(static_cast<U>(capacity) * 4u));
            for (I k = 0; k < self->count; ++k)
                reinterpret_cast<U*>(self->data)[k] = reinterpret_cast<const U*>(old)[k];   // raw DWORD copy
            if (old != nullptr) live_free(old);
        }
        self->data[self->count] = x87_float_copy(value);               // fld [esp+14h]; fstp [eax+edx*4]
        self->count = self->count + 1;
    }
}
} // namespace

extern "C" {

// 004AC960 Quaternion::Quaternion(Vector axis, float angle): thiscall RET 10h, EAX = this.
// [M45 compose] 004AC960 body removed: vector/quaternion math owner lane-m46-3dmodel (D70 pattern) (definition removed; declaration kept)
Quaternion* __fastcall quaternion_from_axis_angle_004ac960(Quaternion* self, void*, Vector axis, float angle);

// 004ACAC0 YawPitchRoll(out, yaw, pitch, roll) (degrees): cdecl.
// out = (Q(Z, yaw) * Q(X, pitch)) * Q(Y, roll), axes from .data 007A19B0 / 007A1998 / 007A19A4.
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion* out, float yaw, float pitch, float roll) {
    Quaternion q_roll, q_pitch, q_yaw, tmp;
    quaternion_from_axis_angle_004ac960(&q_roll, nullptr, y_axis_007a19a4,
        static_cast<float>(static_cast<double>(roll) * static_cast<double>(kDegToRad)));    // 004ACAC0..004ACAF4
    const Quaternion* p = quaternion_from_axis_angle_004ac960(&q_pitch, nullptr, x_axis_007a1998,
        static_cast<float>(static_cast<double>(pitch) * static_cast<double>(kDegToRad)));   // 004ACAF9..004ACB29
    const Quaternion* y = quaternion_from_axis_angle_004ac960(&q_yaw, nullptr, kotor_world_los::up_007a19b0,
        static_cast<float>(static_cast<double>(yaw) * static_cast<double>(kDegToRad)));     // 004ACB2E..004ACB5F
    const Quaternion* t = quaternion_multiply_004a9ac0(&tmp, y, p);    // 004ACB64..004ACB6A
    quaternion_multiply_004a9ac0(out, t, &q_roll);                     // 004ACB76..004ACB78 (q_roll pushed at 004ACB09)
    // 004ACB7D mov eax,edx: the original returns 004A9AC0's leftover EDX (not reproducible); the only caller
    // (00454CB7) does not consume EAX. Labeled deviation (D2-style): return out.
    return out;
}

// 004AAE70 VectorMin(out, a, b): cdecl, EAX = out. Per component a < b ? a : b (test ah,5 jp; unordered -> b).
// [M46 compose] D70: 004AAE70 VectorMin single owner = lane-m46-3dmodel (f1_part_core.cpp) (definition removed; declaration kept)
Vector* __cdecl vector_min_004aae70(Vector* out, Vector a, Vector b);

// 004AAEE0 VectorMax(out, a, b): cdecl, EAX = out. Per component a > b ? a : b (test ah,41h jne; unordered -> b).
// [M46 compose] D70: 004AAEE0 VectorMax single owner = lane-m46-3dmodel (f1_part_core.cpp) (definition removed; declaration kept)
Vector* __cdecl vector_max_004aaee0(Vector* out, Vector a, Vector b);

// 004AAF50 BoxAbovePlane(min*, max*, plane*): cdecl, ST0. Per axis: plane component > 0 ? first box corner : second.
double __cdecl box_above_plane_004aaf50(const Vector* minimum, const Vector* maximum, const Plane* plane) {
    const float z = (static_cast<double>(plane->c) > static_cast<double>(kZero)) ? minimum->z : maximum->z;   // 004AAF54..004AAF72
    const float y = (static_cast<double>(plane->b) > static_cast<double>(kZero)) ? minimum->y : maximum->y;   // 004AAF75..004AAF8A
    const float x = (static_cast<double>(plane->a) > static_cast<double>(kZero)) ? minimum->x : maximum->x;   // 004AAF8D..004AAFA0
    const double yb = static_cast<double>(y) * static_cast<double>(plane->b);   // 004AAFA2..004AAFA5
    const double xa = static_cast<double>(x) * static_cast<double>(plane->a);   // 004AAFA8..004AAFAA
    const double sum = yb + xa;                                                 // 004AAFAC faddp
    return sum + static_cast<double>(z) * static_cast<double>(plane->c) +      // 004AAFAE..004AAFB3
           static_cast<double>(plane->d);                                       // 004AAFB5
}

// 0045D790 CExoArrayList<Plane>::CExoArrayList(int size): thiscall RET 4, EAX = this.
PlaneList* __fastcall plane_list_construct_0045d790(PlaneList* list, void*, I size) {
    list->count = 0; list->capacity = 0; list->data = nullptr;          // 0045D79B..0045D7A1
    if (size != 0) plane_list_allocate_0045c270(list, nullptr, size);   // 0045D799..0045D7A8
    return list;                                                        // 0045D7AD
}

// 004AD5E0 CExoArrayList<Plane>::Add(Plane): thiscall RET 10h.
void __fastcall plane_list_add_004ad5e0(PlaneList* list, void*, Plane plane) {
    if (list->count == list->capacity) {                                // 004AD5E3..004AD5E9
        const I capacity = (list->capacity != 0) ? list->capacity + list->capacity : 8;   // 004AD5EB..004AD5F3
        plane_list_allocate_0045c270(list, nullptr, capacity);          // 004AD5F8..004AD5FB
    }
    list->data[list->count] = plane;                                    // 004AD600..004AD622 (raw DWORD stores)
    list->count = list->count + 1;                                      // 004AD625
}

// 0046E240 SafePointer<WindManager>::operator=(WindManager*): thiscall RET 4, EAX = this. The registration list is
// the WindManager's first member (+0): remove from the old target (00445080), append to the new one (005E03C0 growth).
void* __fastcall wind_safe_pointer_assign_0046e240(void* safe_pointer, void*, void* wind) {
    void* old = field<void*>(safe_pointer, 0);                          // 0046E244
    if (old != nullptr) texture_list_remove_00445080(old, nullptr, safe_pointer);   // 0046E246..0046E24B
    field<void*>(safe_pointer, 0) = wind;                               // 0046E256
    if (wind != nullptr) {                                              // 0046E254 / 0046E258
        PointerList* list = static_cast<PointerList*>(wind);
        if (field<I>(wind, 4) == field<I>(wind, 8)) {                   // 0046E25A..0046E260
            const U capacity = (field<U>(wind, 8) != 0u) ? field<U>(wind, 8) * 2u : 8u;   // 0046E262..0046E26A
            menu_list_growth::allocate_005e03c0(list, nullptr, capacity);   // 0046E26F..0046E272
        }
        field<void**>(wind, 0)[field<I>(wind, 4)] = safe_pointer;       // 0046E277..0046E27C
        field<I>(wind, 4) = field<I>(wind, 4) + 1;                      // 0046E27F
    }
    return safe_pointer;                                                // 0046E282
}

// 004A5950 WindManager::GetGlobalWindNoPoint(Vector* out, const Vector* point, float scale): thiscall RET 0Ch.
Vector* __fastcall wind_manager_get_global_wind_no_point_004a5950(void* wind, void*, Vector* out, const Vector* point,
                                                                  float scale) {
    const Vector* global = &field<Vector>(wind, 0x18);                  // 004A5956..004A5960 edi = this+18h
    if (static_cast<double>(global->x) == static_cast<double>(kZero) &&
        static_cast<double>(global->y) == static_cast<double>(kZero) &&
        static_cast<double>(global->z) == static_cast<double>(kZero)) { // 004A5956..004A5988 (test ah,44h jp x3)
        const U* src = reinterpret_cast<const U*>(global);
        U* dst = reinterpret_cast<U*>(out);
        dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];              // 004A598A..004A599E raw copies
        return out;
    }
    const I ix = wind_cell(x87_ftol_float(&point->x));                  // 004A59AE..004A59CC
    const I iy = wind_cell(x87_ftol_float(&point->y));                  // 004A59CD..004A59EB
    float angle_a = 0.0f;                                               // 004A59F9 [esp+2Ch] = 0
    float angle_b = 0.0f;                                               // 004A5A01 [esp+10h] = 0
    if ((wind_rotation_guard_00831e28 & 1u) == 0u) {                    // 004A59EC..004A5A09
        wind_rotation_guard_00831e28 = wind_rotation_guard_00831e28 | 1u;   // 004A5A0B
        wind_rotation_00831e18.z = 0.0f;                                // 004A5A11 [00831E24]
        wind_rotation_00831e18.y = 0.0f;                                // 004A5A1B [00831E20]
        wind_rotation_00831e18.x = 0.0f;                                // 004A5A25 [00831E1C]
        wind_rotation_00831e18.w = 1.0f;                                // 004A5A2F [00831E18]
    }
    const float* grid0 = field<const float*>(wind, 0x830);              // 004A5A3F
    const float* grid1 = field<const float*>(wind, 0x834);              // 004A5A4B
    const double blend = static_cast<double>(field<float>(wind, 0x840));
    const double one_minus = static_cast<double>(kOne) - blend;         // 004A5A39..004A5A45
    const I index = ix * 16 + iy;                                       // 004A5A51..004A5A5A
    const double sample = one_minus * static_cast<double>(grid0[index]) +
                          static_cast<double>(grid1[index]) * blend;    // 004A5A56..004A5A69
    const double magnitude = static_cast<double>(scale) * sample;       // 004A5A6B..004A5A6F
    const float vx = static_cast<float>(magnitude * static_cast<double>(global->x));   // 004A5A71..004A5A75
    const float vy = static_cast<float>(magnitude * static_cast<double>(global->y));   // 004A5A79..004A5A7E
    const float vz = static_cast<float>(magnitude * static_cast<double>(global->z));   // 004A5A82..004A5A85
    const double k1 = static_cast<double>(field<float>(wind, 0x838));
    if (!(k1 == static_cast<double>(kZero))) {                          // 004A5A89..004A5A9A test ah,44h jnp
        const double v = sample * k1;                                   // 004A5A9C
        angle_a = static_cast<float>(((v + v) - k1) * static_cast<double>(kHalf));   // 004A5AA2..004A5AB0
    }
    const double k2 = static_cast<double>(field<float>(wind, 0x83C));
    if (!(k2 == static_cast<double>(kZero))) {                          // 004A5AB8..004A5AC9
        const I index2 = iy * 16 + ix;                                  // 004A5ACB..004A5AD0
        const double sample2 = one_minus * static_cast<double>(grid0[index2]) +
                               static_cast<double>(grid1[index2]) * blend;   // 004A5AD3..004A5ADF
        const double v = sample2 * k2;                                  // 004A5AE1
        angle_b = static_cast<float>(((v + v) - k2) * static_cast<double>(kHalf));   // 004A5AE7..004A5AF5
    }
    const float cos_a = cosine_lut_004aadf0(angle_a);                   // 004A5B01..004A5B07
    const float sin_a = sine_lut_004aae30(angle_a);                     // 004A5B0B..004A5B11
    const float cos_b = cosine_lut_004aadf0(angle_b);                   // 004A5B19..004A5B1F
    const float sin_b = sine_lut_004aae30(angle_b);                     // 004A5B23..004A5B24 (kept in ST0)
    wind_rotation_00831e18.w = static_cast<float>(static_cast<double>(cos_b) * static_cast<double>(cos_a));   // 004A5B29..004A5B38
    wind_rotation_00831e18.x = static_cast<float>(static_cast<double>(cos_a) * static_cast<double>(sin_b));   // 004A5B3E..004A5B44
    wind_rotation_00831e18.y = static_cast<float>(static_cast<double>(sin_b) * static_cast<double>(sin_a));   // 004A5B4A..004A5B4E
    wind_rotation_00831e18.z = static_cast<float>(static_cast<double>(cos_b) * static_cast<double>(sin_a));   // 004A5B54..004A5B5C
    const double qw = static_cast<double>(wind_rotation_00831e18.w), qx = static_cast<double>(wind_rotation_00831e18.x),
                 qy = static_cast<double>(wind_rotation_00831e18.y), qz = static_cast<double>(wind_rotation_00831e18.z);
    const double xx = qx * qx;                                          // 004A5B62..004A5B68
    const double yy = qy * qy;                                          // 004A5B6E..004A5B74
    const double zz = qz * qz;                                          // 004A5B7A..004A5B80
    const double xy = qy * qx;                                          // 004A5B86..004A5B8C
    const float xz = static_cast<float>(qz * qx);                       // 004A5B92..004A5B9E
    const float xw = static_cast<float>(qx * qw);                       // 004A5BA2..004A5BAE
    const float yz = static_cast<float>(qz * qy);                       // 004A5BB2..004A5BBE
    const float yw = static_cast<float>(qy * qw);                       // 004A5BC2..004A5BCE
    const double zw = qz * qw;                                          // 004A5BD2..004A5BD8
    const double dx = static_cast<double>(vx), dy = static_cast<double>(vy), dz = static_cast<double>(vz);
    {
        const double s = zz + yy;                                       // 004A5BDE..004A5BE0
        double r = (static_cast<double>(kOne) - (s + s)) * dx;          // 004A5BE2..004A5BEA
        const double t = xy - zw;                                       // 004A5BEE..004A5BF0
        r = r + (t + t) * dy;                                           // 004A5BF2..004A5BF8
        const double u = static_cast<double>(yw) + static_cast<double>(xz);   // 004A5BFA..004A5BFE
        r = r + (u + u) * dz;                                           // 004A5C02..004A5C08
        out->x = static_cast<float>(r);                                 // 004A5C0A
    }
    {
        const double s = zz + xx;                                       // 004A5C0C..004A5C0E
        double r = (static_cast<double>(kOne) - (s + s)) * dy;          // 004A5C10..004A5C18
        const double t = static_cast<double>(yz) - static_cast<double>(xw);   // 004A5C1C..004A5C20
        r = r + (t + t) * dz;                                           // 004A5C24..004A5C2C
        const double u = zw + xy;                                       // 004A5C2F..004A5C32
        r = r + (u + u) * dx;                                           // 004A5C34..004A5C3A
        out->y = static_cast<float>(r);                                 // 004A5C3E
    }
    {
        const double s = yy + xx;                                       // 004A5C43
        double r = (static_cast<double>(kOne) - (s + s)) * dz;          // 004A5C45..004A5C4D
        const double t = static_cast<double>(yz) + static_cast<double>(xw);   // 004A5C51..004A5C55
        r = r + (t + t) * dy;                                           // 004A5C59..004A5C5F
        const double u = static_cast<double>(xz) - static_cast<double>(yw);   // 004A5C61..004A5C65
        r = r + (u + u) * dx;                                           // 004A5C69..004A5C6F
        out->z = static_cast<float>(r);                                 // 004A5C71
    }
    return out;                                                         // EAX = out (004A5B31 mov eax,[esp+38h])
}

// 004A6440 WindManager::GetGlobalPointWind(Vector* out, const Vector* point, float time): thiscall RET 0Ch; the
// third argument is not read. Forwards to pointSourceWindVector 004A5C80(out, *point).
Vector* __fastcall wind_manager_get_global_point_wind_004a6440(void* wind, void*, Vector* out, const Vector* point, float) {
    wind_manager_point_source_wind_vector_004a5c80(wind, nullptr, out, point->x, point->y, point->z);   // 004A6440..004A645F
    return out;                                                         // 004A6464 mov eax,esi
}

// 004A72D0 CAurTriangleListElem::~CAurTriangleListElem: thiscall RET 0 (EH frame 007181A8: member CExoArrayList<float>
// destructor, state 0). Body frees the corner list, clears all six words, then runs the member destructor.
void __fastcall triangle_list_elem_destruct_004a72d0(void* self, void*) {
    TriangleListElem* e = static_cast<TriangleListElem*>(self);
    if (e->corners.data != nullptr) {                                   // 004A72EE..004A72F8
        live_free(e->corners.data);                                     // 004A72FB
        e->corners.data = nullptr;                                      // 004A7303
    }
    e->corners.capacity = 0; e->corners.count = 0;                      // 004A7305..004A7308
    e->face_0c = 0u; e->part_10 = 0u; e->count_14 = 0u;                 // 004A730B..004A7311
    float* member = e->corners.data;                                    // 004A7314 member dtor (state -1)
    if (member != nullptr) live_free(member);                           // 004A7316..004A7323
}

// 004A7340 CExoArrayList<float>::CExoArrayList(const CExoArrayList<float>&) (list part of the CAurTriangleListElem
// copy, 004A81E9): thiscall RET 4, EAX = this. Growth cap ? cap*2 : 8 through 006FA7E6 / 006FA390.
void* __fastcall triangle_list_elem_copy_construct_004a7340(void* self, void*, const void* source) {
    FloatList* list = static_cast<FloatList*>(self);
    list->count = 0; list->capacity = 0; list->data = nullptr;          // 004A734B..004A7351
    float_list_append_all(list, static_cast<const FloatList*>(source), 8);   // 004A7353..004A73D7
    return self;                                                        // 004A73DA
}

// 00438A50 CExoArrayList<float>::operator=: thiscall RET 4, EAX = this. Count reset only (storage kept); growth
// cap ? cap*2 : 10h through the operator new / free thunks 006FC0D2 / 006FC0CD (jmp 006FA7E6 / jmp 006FA390).
kotor_reconstruction::ArrayMutations* __fastcall float_list_assign_00438a50(kotor_reconstruction::ArrayMutations* self, void*,
                                                                            const kotor_reconstruction::ArrayMutations* source) {
    FloatList* list = reinterpret_cast<FloatList*>(self);
    list->count = 0;                                                    // 00438A5B
    float_list_append_all(list, reinterpret_cast<const FloatList*>(source), 0x10);   // 00438A5E..00438ADB
    return self;                                                        // 00438ADE
}

// 004A9770 Vector::operator-=(const Vector&): thiscall RET 4, EAX = this (callee of 0045D3FA).
Vector* __fastcall vector_subtract_assign_004a9770(Vector* self, void*, const Vector* other) {
    self->x = static_cast<float>(static_cast<double>(self->x) - static_cast<double>(other->x));   // 004A9776..004A977A
    self->y = static_cast<float>(static_cast<double>(self->y) - static_cast<double>(other->y));   // 004A977C..004A9782
    self->z = static_cast<float>(static_cast<double>(self->z) - static_cast<double>(other->z));   // 004A9785..004A978B
    return self;
}

// CExoArrayList<CAurTriangleListElem>::Add 004A77C0: thiscall(elem BY VALUE, 18h) RET 18h; the callee assigns the
// element (00438A50 + three words) and destroys its argument (004A783A -> 004A72D0). NEW undefined external.
void __fastcall triangle_list_add_004a77c0(PointerList* list, void* unused_edx, TriangleListElem elem);

// 004A81B0 CExoArrayList<CAurTriangleListElem>::CExoArrayList(const&): thiscall RET 4, EAX = this. Each source
// element is copy-constructed directly in the outgoing argument area (004A81DB sub esp,18h; 004A81E9 list copy;
// 004A81EE..004A81FD words) and handed to Add, which owns and destroys it.
PointerList* __fastcall triangle_list_copy_construct_004a81b0(PointerList* self, void*, const PointerList* source) {
    self->size = 0; self->capacity = 0; self->data = 0u;               // 004A81BD..004A81C3
    for (I i = 0; i < source->size; ++i) {                             // 004A81C6..004A821C (count re-read per pass)
        const TriangleListElem* src = reinterpret_cast<const TriangleListElem*>(
            static_cast<std::uintptr_t>(source->data)) + i;            // 004A81D1..004A81E0 (offset += 18h)
        TriangleListElem arg;
        triangle_list_elem_copy_construct_004a7340(&arg.corners, nullptr, &src->corners);   // 004A81E6..004A81E9
        arg.face_0c = src->face_0c;                                     // 004A81EE..004A81F1
        arg.part_10 = src->part_10;                                     // 004A81F4..004A81F7
        arg.count_14 = src->count_14;                                   // 004A81FA..004A81FD
        triangle_list_add_004a77c0(self, nullptr, arg);                 // 004A8200..004A8202 (callee destroys)
    }
    return self;                                                        // 004A8220
}

// 004A8230 CExoArrayList<SafePointer<CAurTexture>>::Add(SafePointer by value): thiscall RET 4. Grow (cap ? cap*2 : 8,
// 0043DE00), take slot data[count], count++, SafePointer::operator= 00447FA0 on the slot, then destroy the by-value
// argument (004A8283..004A828F, identical to the out-of-line 00422380 the compiler emits for ~SafeTextureArg).
// EH frame 00718248 (state 0 = argument destructor) is cited; the destructor runs on the normal path.
void __fastcall safe_texture_list_add_004a8230(PointerList* list, void*, kotor_scene3d_bsp::SafeTextureArg value) {
    if (list->size == list->capacity) {                                 // 004A8248..004A8256
        const I capacity = (list->capacity != 0) ? list->capacity + list->capacity : 8;   // 004A8258..004A8260
        aurora_texture_ptr_list_allocate_0043de00(list, nullptr, capacity);   // 004A8265..004A8268
    }
    const I count = list->size;                                         // 004A826D
    void** slot = reinterpret_cast<void**>(static_cast<std::uintptr_t>(list->data)) + count;   // 004A8270..004A8272
    list->size = count + 1;                                             // 004A8275..004A8276
    assign_00447fa0(slot, nullptr, value.target);                       // 004A8279..004A827E
}                                                                       // ~SafeTextureArg: 004A8283..004A828F

} // extern "C"
