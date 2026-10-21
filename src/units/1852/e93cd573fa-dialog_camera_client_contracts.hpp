// Private contracts for the five dialog camera/client dependency providers.
// New private header; not a shared header edit and not the core packet header.
#pragma once

#include <cstdint>
#include <cstring>

namespace dlgcam {

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;

inline U32 lw(const void* p, U32 off) {
    U32 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, 4);
    return v;
}

inline U16 lh(const void* p, U32 off) {
    U16 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, 2);
    return v;
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

}  // namespace dlgcam

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

extern "C" {
int __fastcall kotor_fastcall_005edc90(void*, void*, dlgcam::U32, dlgcam::U32, void*);
void __fastcall kotor_fastcall_005edbd0(void*, void*);
int __fastcall kotor_fastcall_0062d570(void*, void*);
int __fastcall kotor_fastcall_006412b0(void*, void*);
int __fastcall kotor_fastcall_0062abb0(void*, void*);

void* __fastcall kotor_fastcall_005ed580(void*, void*, dlgcam::U32);
int __fastcall kotor_fastcall_0063c4a0(void*, void*, dlgcam::U32);
const char* __fastcall WindowCStr005e5670(void*, void*);

int __fastcall kotor_fastcall_005f3070(void*, void*, dlgcam::U32, dlgcam::U32, void*);
void __fastcall kotor_fastcall_005f2d80(void*, void*);
int __fastcall kotor_fastcall_006a6da0(void*, void*);
}
