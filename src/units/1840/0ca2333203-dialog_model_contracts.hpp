// Private contracts for the three dialog model callee providers.
// New private header; not a shared header edit and not the prior packet header.
#pragma once

#include <cstdint>
#include <cstring>

namespace dlgmodel {

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

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

inline void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

template <class R, class... A>
inline R vcall(void* object, U32 slot, A... args) {
    using F = R(__thiscall*)(void*, A...);
    void** vt = *reinterpret_cast<void***>(object);
    return reinterpret_cast<F>(vt[slot / 4])(object, args...);
}

}  // namespace dlgmodel

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

// Selected C++ provider symbol:
// ?BorrowedEquals005e5290@StartupString@kotor_reconstruction@@QAEHPBD@Z
namespace kotor_reconstruction {
struct StartupString {
    int __thiscall BorrowedEquals005e5290(const char*);
};
}  // namespace kotor_reconstruction

extern "C" {

dlgmodel::U32 __fastcall kotor_fastcall_005a0240(
    void*, void*, dlgmodel::I32, dlgmodel::U32*);
int __fastcall kotor_fastcall_005a0080(void*, void*, void*, dlgmodel::U32);
void __fastcall kotor_fastcall_0060f540(void*, void*);

void* __fastcall kotor_fastcall_004ae750(void*, void*, dlgmodel::U32);
dlgmodel::U32 __fastcall kotor_fastcall_005eda50(void*, void*,
                                                  dlgmodel::U32);
void* __fastcall frame_R4_005ed690(void*, void*);
dlgmodel::U32 __fastcall kotor_fastcall_0062f840(void*, void*, void*);

dlgmodel::U32 __fastcall kotor_fastcall_004cc4b0(void*, void*);
dlgmodel::U32 __fastcall kotor_fastcall_004cb5c0(void*, void*, void*, float);
void* __fastcall c2da_default_ref_00405ed0(void*, void*);
void* __fastcall c2da_copy_00406120(void*, void*, void*, void*);

dlgmodel::U32 __fastcall kotor_fastcall_0062e020(
    void*, void*, dlgmodel::U32, dlgmodel::U32, void*, void*, void*, void*);
void __fastcall kotor_fastcall_005edcb0(void*, void*, dlgmodel::U32);

void* __cdecl live_operator_new(dlgmodel::U32);
void __cdecl live_free(void*);

}
