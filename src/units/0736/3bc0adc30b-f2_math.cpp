// f2_math.cpp - lane-m46-3dmodel fork F2: core math / list lowers reached by the 3D model, animation, emitter
// and mesh units (M45 newly exposed names). swkotor.exe GOG K1 (image base 00400000); every statement cites the
// original instruction(s) it reproduces (listings: lane-m46-3dmodel/d/<ADDR>.txt).
//
// D30-FINAL: the original sets x87 PC_53 / round-nearest once (007092C0), so every x87 intermediate is an IEEE
// double: intermediates here are `double`, `float` appears only at the original `fstp dword` stores. With PC_53
// the x87 add/mul/sub of these bodies round exactly like SSE2 double arithmetic (no transcendental except
// fsin/fcos in 004AC960 - see the equivalence note there).
// Consumer declarations: emitter.h (004213A0 / 0043E250 / 0044F420 / 004AC960, and normalize_004ab180 owner
// lane-world-los D55), anim3d.h (004A9A60), trimesh_skin.h (004AB1E0 / 00488160). Decorated names are identical.
#include "4ac9e30747-emitter.h"
#include <cmath>

using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::Vector; using kotor_scene3d::Quaternion;
using kotor_scene3d::PointerList;

extern "C" {
// anim3d.h:236 (exact copy) / trimesh_skin.h:210 (exact copy) / trimesh_skin.h (matrix list, RET 4)
Vector* __cdecl matrix_multiply_vector_004a9a60(Vector* out, const float* matrix9, const Vector* v);
Vector* __cdecl vector_multiply_004ab1e0(Vector* out, const Vector* a, const Vector* b);
void __fastcall matrix_list_allocate_00488160(PointerList* list, void* unused_edx, I capacity);
// lane-3d-model model_tangent.cpp (M45 provider @matrix_identity_construct_00437af0@8): Matrix::Matrix() identity
void* __fastcall matrix_identity_construct_00437af0(void* self, void* unused_edx);
}

namespace {
constexpr double kOne_0073d764 = 1.0;    // .rdata 0073D764 = 3F800000h (1.0f), fsubr operand
constexpr double kHalf_0073e9ac = 0.5;   // .rdata 0073E9AC = 3F000000h (0.5f), fmul operand
constexpr double kZero_0073d700 = 0.0;   // .rdata 0073D700 = 0 (0.0f), fcomp operand
}

extern "C" {
// ================================================================ 004A9A60 operator*(Matrix const&, Vector const&)
// cdecl(out, m, v), returns out. m = 3x3 float matrix (9 DWORDs). All 9 products are loaded before the first
// store (stores 004A9AAA..004A9AAF), so out may alias v.
Vector* __cdecl matrix_multiply_vector_004a9a60(Vector* out, const float* m, const Vector* v) {
    const double vx = v->x, vy = v->y, vz = v->z;                                        // [eax],[eax+4],[eax+8]
    const double z = (static_cast<double>(m[8]) * vz + static_cast<double>(m[2]) * vx)   // 004A9A68..004A9A73
                     + static_cast<double>(m[5]) * vy;                                   // 004A9A75..004A9A7B
    const double y = (static_cast<double>(m[7]) * vz + static_cast<double>(m[1]) * vx)   // 004A9A7D..004A9A88
                     + static_cast<double>(m[4]) * vy;                                   // 004A9A8A..004A9A90
    const double x = (static_cast<double>(m[6]) * vz + static_cast<double>(m[3]) * vy)   // 004A9A92..004A9A9E
                     + static_cast<double>(m[0]) * vx;                                   // 004A9AA0..004A9AA8
    out->x = static_cast<float>(x);                                                      // 004A9AAA fstp [eax]
    out->y = static_cast<float>(y);                                                      // 004A9AAC fstp [eax+4]
    out->z = static_cast<float>(z);                                                      // 004A9AAF fstp [eax+8]
    return out;                                                                          // 004A9AA4 eax = out
}

// ================================================================ 004AB1E0 VectorMultiply (component product)
Vector* __cdecl vector_multiply_004ab1e0(Vector* out, const Vector* a, const Vector* b) {
    const double z = static_cast<double>(a->z) * b->z;                                   // 004AB1E4..004AB1EB
    const double y = static_cast<double>(a->y) * b->y;                                   // 004AB1EE..004AB1F1
    const double x = static_cast<double>(a->x) * b->x;                                   // 004AB1F4..004AB1FA
    out->x = static_cast<float>(x);                                                      // 004AB1FC
    out->y = static_cast<float>(y);                                                      // 004AB1FE
    out->z = static_cast<float>(z);                                                      // 004AB201
    return out;                                                                          // 004AB1F6 eax = out
}

// ================================================================ 0043E250 Quaternion::zdir (thiscall RET 4)
// Quaternion (w,x,y,z) at +0/+4/+8/+C (scene3d.h). Third column of the rotation matrix.
Vector* __fastcall quaternion_zdir_0043e250(const Quaternion* q, void*, Vector* out) {
    const double x = q->x, y = q->y;                                                     // 0043E250 / 0043E257
    const double sum = x * x + y * y;                                                    // 0043E25A..0043E262
    const double zz = kOne_0073d764 - (sum + sum);                                       // 0043E264 fadd st,st; 0043E266 fsubr 1.0
    const double zy = (static_cast<double>(q->z) * q->y - static_cast<double>(q->w) * q->x); // 0043E270..0043E27B
    const double zyx2 = zy + zy;                                                         // 0043E27D
    const double wy = (static_cast<double>(q->w) * q->y + static_cast<double>(q->z) * q->x); // 0043E27F..0043E28A
    const double wy2 = wy + wy;                                                          // 0043E28C
    out->x = static_cast<float>(wy2);                                                    // 0043E28E fstp [eax]
    out->y = static_cast<float>(zyx2);                                                   // 0043E290 fstp [eax+4]
    out->z = static_cast<float>(zz);                                                     // 0043E293 fstp [eax+8]
    return out;                                                                          // 0043E253 eax = out; 0043E296 ret 4
}

// ================================================================ 0044F420 Quaternion::ydir (thiscall RET 4)
Vector* __fastcall quaternion_ydir_0044f420(const Quaternion* q, void*, Vector* out) {
    const double a = static_cast<double>(q->y) * q->z + static_cast<double>(q->x) * q->w; // 0044F420..0044F42F
    const double a2 = a + a;                                                             // 0044F431
    const double x = q->x, z = q->z;                                                     // 0044F436 / 0044F433
    const double sum = x * x + z * z;                                                    // 0044F439..0044F441
    const double b = kOne_0073d764 - (sum + sum);                                        // 0044F443 fadd st,st; 0044F445 fsubr 1.0
    const double c = static_cast<double>(q->x) * q->y - static_cast<double>(q->w) * q->z; // 0044F44F..0044F45A
    const double c2 = c + c;                                                             // 0044F45C
    out->x = static_cast<float>(c2);                                                     // 0044F45E
    out->y = static_cast<float>(b);                                                      // 0044F460
    out->z = static_cast<float>(a2);                                                     // 0044F463
    return out;                                                                          // 0044F423 eax = out; 0044F466 ret 4
}

// ================================================================ 004AC960 Quaternion::Quaternion(Vector axis, float angle)
// thiscall RET 10h (axis by value = 3 stack DWORDs, angle = 1), returns this.
// Original 004AC960: FCOMP32/status parity, then raw x87 half-angle FCOS/FSIN.
// No C++ transcendental or intermediate 64-bit spill. Public ABI is unchanged.
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void*, Vector axis, float angle) {
    const float zero=static_cast<float>(kZero_0073d700);
    const float half=static_cast<float>(kHalf_0073e9ac);
    unsigned short status;
    __asm {
        fld angle
        fcomp zero
        fnstsw ax
        mov status,ax
    }
    // Original TEST AH,44h / JP computes for nonzero and unordered; equal
    // alone takes identity. Keep FCOMP's signaling behavior, not UCOMISS.
    if((status&0x4400)==0x4000){
        self->z=0.0f; self->y=0.0f; self->x=0.0f;
        self->w=1.0f;
        return self;
    }
    Vector unit;
    const Vector* const n=normalize_004ab180(&unit,&axis);
    Vector* const raw_axis=&axis;
    __asm {
        fld angle
        fmul half
        mov eax,n
        mov edx,raw_axis
        mov ecx,dword ptr[eax]
        mov dword ptr[edx],ecx
        mov ecx,dword ptr[eax+4]
        fld st(0)
        mov eax,dword ptr[eax+8]
        fcos
        mov dword ptr[edx+4],ecx
        mov dword ptr[edx+8],eax
        mov eax,self
        fstp dword ptr[eax]
        fsin
        fld dword ptr[edx]
        fmul st(0),st(1)
        fld st(1)
        fmul dword ptr[edx+4]
        fxch st(2)
        fmul dword ptr[edx+8]
        fstp dword ptr[edx+8]
        mov ecx,dword ptr[edx+8]
        fstp dword ptr[eax+4]
        mov dword ptr[eax+0ch],ecx
        fstp dword ptr[eax+8]
    }
    return self;
}

// ================================================================ 004213A0 CExoArrayList<Vector>::SetSize (thiscall RET 4)
void __fastcall vector_list_set_size_004213a0(PointerList* list, void*, I size) {
    if (size == 0) {                                                                     // 004213A6..004213AA
        const U data = list->data;                                                       // 004213AC
        if (data != 0) {                                                                 // 004213AE..004213B0
            live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));       // 004213B2..004213B8 (006FA390)
            list->data = static_cast<U>(size);                                           // 004213BB mov [esi],edi (edi = 0)
        }
        list->size = size;                                                               // 004213BD mov [esi+4],edi
        list->capacity = 0;                                                              // 004213C1
        return;
    }
    vector_list_allocate_00421310(list, nullptr, size);                                  // 004213CC..004213CF
    list->size = size;                                                                   // 004213D4
}

// ================================================================ 00488160 CExoArrayList<Matrix>::allocate (thiscall RET 4)
// Matrix = 9 floats (0x24 bytes, 00488198 add edi,24h). EAX on return = the old data word or (when it was freed)
// the free result; the trimesh_skin.h declaration is void.
void __fastcall matrix_list_allocate_00488160(PointerList* list, void*, I capacity) {
    const U old = list->data;                                                            // 0048816A..00488174 ([esp+10h])
    list->capacity = capacity;                                                           // 00488178
    unsigned char* block = static_cast<unsigned char*>(
        live_operator_new(static_cast<U>(capacity) * 0x24u));                           // 0048816C..0048817B (006FA7E6)
    if (block != nullptr) {                                                              // 00488185..00488187
        unsigned char* m = block;
        I n = static_cast<I>(static_cast<U>(capacity) - 1u);                             // 00488189 dec esi
        if (n >= 0) {                                                                    // 0048818A..0048818E test; jl
            ++n;                                                                         // 00488190 inc esi
            for (; n != 0; --n) {                                                        // 0048819B..0048819C dec; jne
            matrix_identity_construct_00437af0(m, nullptr);                              // 00488191..00488193
            m += 0x24;                                                                   // 00488198
            }
        }
    } else {
        block = nullptr;                                                                 // 004881A0 xor ebp,ebp
    }
    list->data = static_cast<U>(reinterpret_cast<std::uintptr_t>(block));               // 004881A9
    for (I i = 0; i < list->size; ++i) {                                                 // 004881A2..004881CB (count re-read each pass)
        // rep movs 9 DWORDs from old+i*24h to [list->data]+i*24h (004881B0..004881C0; data re-read each pass)
        const volatile U* src = reinterpret_cast<const volatile U*>(static_cast<std::uintptr_t>(old) + static_cast<U>(i) * 0x24u);
        volatile U* dst = reinterpret_cast<volatile U*>(static_cast<std::uintptr_t>(list->data) + static_cast<U>(i) * 0x24u);
        for (int k = 0; k < 9; ++k) dst[k] = src[k];
    }
    if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old)));  // 004881CD..004881DA (006FA390)
}
}
