// Private contracts for the GUI dialog-model publication lane.
// Every @...@N direct callee is a selected extern "C" __fastcall facade with the
// original ECX+stack contract and a dead/ignored dummy EDX. The three dynamic
// selected slots are also @N fastcall facades; vcall_fast passes EDX=nullptr.
#pragma once
#include <cstdint>
#include <cstring>

namespace dlgpub {
using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using I32 = std::int32_t;

inline U32 lw(const void* p, U32 off) {
    U32 v;
    std::memcpy(&v, static_cast<const U8*>(p) + off, 4);
    return v;
}

inline void lwset(void* p, U32 off, U32 v) {
    std::memcpy(static_cast<U8*>(p) + off, &v, 4);
}

inline void* lp(const void* p, U32 off) {
    return reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(lw(p, off)));
}

template <class R, class... A>
inline R vcall_fast(void* object, U32 slot, A... args) {
    using F = R(__fastcall*)(void*, void*, A...);
    void** const vt = *reinterpret_cast<void***>(object);
    return reinterpret_cast<F>(vt[slot / 4])(object, nullptr, args...);
}
}  // namespace dlgpub

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

namespace kotor_menu_10_19 {
struct String {
    alignas(4) unsigned char bytes[8];
    String* __thiscall default_005b3190();
    const char* __thiscall text_005e5670();
    void __thiscall destroy_005e5c20();
};
}  // namespace kotor_menu_10_19

extern "C" {
// Selected canonical allocator (undecorated identifier; cdecl adds the leading _).
void* __cdecl live_operator_new(dlgpub::U32 size) noexcept;
void __cdecl live_free(void* pointer) noexcept;

// Selected original 00449CC0 provider (cdecl, 4 stack args, RET0).
void* __cdecl aurora_new_aur_object_00449cc0(
    const char* name, const char* suffix, int arg3, int arg4);

// Selected original 00401130 array-constructor helper (stdcall 4 args, RET0x10).
void __stdcall vector_constructor_iterator_00401130(
    void* dest, dlgpub::U32 element_size, dlgpub::U32 count,
    void* (__fastcall* ctor)(void*, void*));

// Selected real original 00405ED0 16-byte zero constructor (@8, RET0).
void* __fastcall c2da_default_ref_00405ed0(void*, void*);

// Selected direct callees. Every @...@N is fastcall with a dummy EDX.
void* __fastcall c2da_copy_00406120(void*, void*, void*, void*);   // @16
int __fastcall gui_load_resref_is_valid_00406030(void*, void*);    // @8
void __fastcall gui_load_resref_copy_to_string_00405fb0(
    void*, void*, void*);                                          // @12
void __fastcall c2da_ref_to_string_00405f70(void*, void*, void*);  // @12
int __fastcall c2da_not_equal_text_00406060(
    void*, void*, const char*);                                    // @12
void* __fastcall kotor_fastcall_005ed530(void*, void*);            // @8
void* __fastcall kotor_fastcall_005ed580(
    void*, void*, dlgpub::U32);                                    // @12
dlgpub::U32 __fastcall kotor_fastcall_0062a8b0(
    void*, void*, void*);                                          // @12

// This lane's exports (declared so each TU sees the cross-TU contracts).
int __fastcall kotor_fastcall_0062e020(
    void*, void*, dlgpub::U32, dlgpub::U32, void*, void*, void*, void*);
void __fastcall kotor_fastcall_005edcb0(void*, void*, dlgpub::U32);
int __fastcall kotor_fastcall_0062b850(void*, void*, dlgpub::U32);
void __fastcall kotor_fastcall_00640f70(void*, void*, void*);
int __fastcall kotor_fastcall_0062c620(void*, void*);
}
