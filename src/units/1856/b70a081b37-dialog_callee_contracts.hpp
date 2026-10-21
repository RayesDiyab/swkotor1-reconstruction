// Private contracts for the three dialog camera callee providers.
// New private header; not a shared header edit and not the core packet header.
#pragma once

#include <cstdint>
#include <cstring>

namespace dlgcallee {

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

inline U8 lb(const void* p, U32 off) {
    U8 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, 1);
    return v;
}

inline U32 lw(const void* p, U32 off) {
    U32 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, 4);
    return v;
}

inline void lwset(void* p, U32 off, U32 v) {
    std::memcpy(static_cast<U8*>(p) + off, &v, 4);
}

inline void* lp(const void* p, U32 off) {
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(lw(p, off)));
}

template <class R, class... A>
inline R vcall(void* object, U32 slot, A... args) {
    using F = R(__thiscall*)(void*, A...);
    void** vt = *reinterpret_cast<void***>(object);
    return reinterpret_cast<F>(vt[slot / 4])(object, args...);
}

// Exact x87 compare/status helper for 006A6DA0.  The comparison is performed
// with x87, not SSE; ST0 is popped by fcomp.
inline U16 x87_cmp(const float* a, const float* b) {
    U16 sw;
    __asm {
        mov eax, a
        fld dword ptr [eax]
        mov eax, b
        fcomp dword ptr [eax]
        fnstsw ax
        mov word ptr [sw], ax
    }
    return sw;
}

inline U16 x87_add_cmp(const float* a, const float* b, const float* c) {
    U16 sw;
    __asm {
        mov eax, a
        fld dword ptr [eax]
        mov eax, b
        fadd dword ptr [eax]
        mov eax, c
        fcomp dword ptr [eax]
        fnstsw ax
        mov word ptr [sw], ax
    }
    return sw;
}

}  // namespace dlgcallee

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

extern "C" {

int __fastcall kotor_fastcall_005f3070(void*, void*, dlgcallee::U32,
                                       dlgcallee::U32, void*);
void __fastcall kotor_fastcall_005f2d80(void*, void*);
int __fastcall kotor_fastcall_006a6da0(void*, void*);

void* __fastcall kotor_fastcall_004ae750(void*, void*, dlgcallee::U32);
void __fastcall recovered_set_activity_004f00a0(void*, void*, int, int, int);  // c09/c11 name (w2_ifo_native.obj)
int __fastcall frame_GetPauseState_004ae980(void*, void*, dlgcallee::U32);  // c09/c11 name (0457-successor.obj)
dlgcallee::I64 __fastcall kotor_fastcall_004adc70(void*, void*);
dlgcallee::U32 __fastcall kotor_fastcall_005eda50(void*, void*, dlgcallee::U32);
void* __fastcall kotor_fastcall_005ed580(void*, void*, dlgcallee::U32);
dlgcallee::U32 __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall kotor_fastcall_004ae910(void*, void*, dlgcallee::U32);

unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, dlgcallee::U32,
                                                  void**);
void __fastcall kotor_fastcall_005d5b80(void*, void*);

dlgcallee::U32 __fastcall kotor_fastcall_005a0240(void*, void*, int, void**);
void __fastcall kotor_fastcall_005a0080(void*, void*, void*,  // arg1 = object pointer (delivered 005a0080 signature)
                                        dlgcallee::U32);
void __fastcall kotor_fastcall_0060f540(void*, void*);

// (the explicit __cdecl __alldiv declaration was removed: it decorated as ___alldiv; the division is now written as
// I64 / 1000 so the compiler binds the real libcmt __alldiv helper)

}

