// Private contracts for the two assigned dialog-target-lookup originals:
//   004CC4B0 CSWSDialog::GetPlayerObjectId
//   004CB5C0 CSWSObject::GetNearestObjectByName
// New private header; no shared/repository header edit.
#pragma once

#include <cstdint>
#include <cstring>

namespace dtl {

using U8 = std::uint8_t;
using U32 = std::uint32_t;

inline U32 lw(const void* p, U32 off) {
    U32 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, sizeof v);
    return v;
}

inline void* lp(const void* p, U32 off) {
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(lw(p, off)));
}

inline void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

inline float as_float(U32 bits) {
    float v;
    std::memcpy(&v, &bits, sizeof v);
    return v;
}

// Original 004CB62C / 004CB6FC: mov eax,[object]; call dword ptr [eax+28h].
// The selected real slot providers are __fastcall (ECX=object, dummy EDX, RET 0).
inline void* as_area_slot_28(void* object) {
    using Fn = void* (__fastcall*)(void*, void*);
    void** const vtbl = *reinterpret_cast<void***>(object);
    return reinterpret_cast<Fn>(vtbl[0x28 / 4])(object, nullptr);
}

}  // namespace dtl

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}  // namespace kotor_menu_window_cells

namespace kotor_reconstruction {
struct StartupString {
    // Exact selected provider symbol:
    // ?BorrowedEquals005e5290@StartupString@kotor_reconstruction@@QAEHPBD@Z
    int __thiscall BorrowedEquals005e5290(const char*);
};
}  // namespace kotor_reconstruction

extern "C" {

dtl::U32 __fastcall kotor_fastcall_004cc4b0(void*, void*);
dtl::U32 __fastcall kotor_fastcall_004cb5c0(void*, void*, const void*, float);

void* __fastcall recovered_object_array_004aed70(void*, void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*, void*, dtl::U32, void**);
dtl::U32 __fastcall recovered_area_first_object_005071b0(void*, void*, dtl::U32*);
dtl::U32 __fastcall recovered_area_next_object_005071e0(void*, void*, dtl::U32*);
int __fastcall exostring_compare_no_case_005e6450(void*, void*, const void*);

}  // extern "C"
