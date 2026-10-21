// Authored private header — new file; no shared header edit.
// Plan review: PLAN-REVIEW.md ACCEPT-WITH-FIXES; fixes applied.
//
// Exact provider/data declarations and bounded accessors for the
// endar-dialog-core four bodies (004CD2D0, 004CD460, 004CD500, 004CB4F0).
// All providers are extern with external linkage; none is wrapped in an
// anonymous namespace. Missing providers are declared only and remain
// unresolved externals.


#pragma once

#include <cstdint>

using U32 = std::uint32_t;
using I32 = std::int32_t;
using U8 = std::uint8_t;

// ---------------------------------------------------------------------------
// Existing selected providers. Bind by these exact symbols, not by numeric VA.
// ---------------------------------------------------------------------------
extern "C" unsigned __fastcall frame_R4_005ed690(void*, void*);
extern "C" void* __fastcall kotor_fastcall_005ed530(void*, void*);
extern "C" void* __fastcall kotor_fastcall_004aede0(void*, void*);
extern "C" void __fastcall frame_GetWorldTime_004ade40(void*, void*, U32*, U32*);
extern "C" U32 __fastcall kotor_fastcall_004adf50(void*, void*, U32, U32, U32, U32, U32*, U32*);
extern "C" int __fastcall recovered_event_time_compare_004adfa0(void*, void*, U32, U32, U32, U32);
extern "C" void* __fastcall recovered_object_array_004aed70(void*, void*);
extern "C" unsigned __fastcall kotor_fastcall_004d8230(void*, void*, U32, void*);
extern "C" void __fastcall w5_creature_set_lock_orientation_to_object_004f34a0(void*, void*, U32, U32);

namespace menu_continuation {
struct ExoBase;
// Declaration-only bounded view of the selected 0457 ExoTimers receiver; the
// member declaration matches W4's w4_providers.hpp. The object layout is not
// asserted here.
struct ExoTimers {
    long long __thiscall GetHighResolutionTimer_005e9260();
};
extern ExoBase* exo_base_007a39e0;
}

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

namespace menu_tables_m41 {
namespace cells {
extern const unsigned int invalid_object_00746644;
}
}

// Selected 8-byte projectile literal. The i686 COFF map symbol is
// `_object_projectile_literal_0074666c`, i.e. the C identifier below with the
// usual leading-underscore decoration; the original compares byte [0x74666C].
extern "C" const unsigned char object_projectile_literal_0074666c[8];

// ---------------------------------------------------------------------------
// The four owned bodies (cross-calls inside this lane).
// ---------------------------------------------------------------------------
extern "C" int __fastcall kotor_fastcall_004cd2d0(void*, void*);
extern "C" int __fastcall kotor_fastcall_004cd460(void*, void*, U32);
extern "C" int __fastcall kotor_fastcall_004cd500(void*, void*);
extern "C" int __fastcall kotor_fastcall_004cb4f0(void*, void*);

// ---------------------------------------------------------------------------
// Missing one-level providers: declared, never defined in this lane. They
// remain unresolved externals so the objects cannot silently link to a stub.
// Exact original call contracts:
//   @20/ret 0xC  ECX=client,  stack id1,id2,dialog
//   @8/ret 0     ECX=client
//   @8/ret 0     ECX=client
//   @8/ret 0     ECX=gui
//   @8/ret 0     ECX=module
//   @8/ret 0     ECX=gui
//   @24/ret 0x10 ECX=dialog,  stack self,reply,entry,&status
//   @16/ret 8    ECX=dialog,  stack self,reply
//   @16/ret 8    ECX=dialog,  stack self,flag
//   @12/ret 4    ECX=object,  stack self_id
// ---------------------------------------------------------------------------
extern "C" int __fastcall kotor_fastcall_005edc90(void*, void*, U32, U32, void*);
extern "C" int __fastcall kotor_fastcall_005edbc0(void*, void*);
extern "C" void __fastcall kotor_fastcall_005edbd0(void*, void*);
extern "C" int __fastcall kotor_fastcall_0062d570(void*, void*);
extern "C" int __fastcall kotor_fastcall_006412b0(void*, void*);
extern "C" int __fastcall kotor_fastcall_0062abb0(void*, void*);
extern "C" int __fastcall kotor_fastcall_005a4010(void*, void*, void*, I32, U32, U32*);
extern "C" int __fastcall kotor_fastcall_005a3820(void*, void*, void*, I32);
extern "C" int __fastcall kotor_fastcall_005a0a40(void*, void*, void*, U32);
extern "C" int __fastcall kotor_fastcall_004cd660(void*, void*, U32);

namespace endar_dialog_core {

// Bounded raw accessors preserve the original dword/byte width and fault at
// the original address for a null/invalid base. No checks or fallback values.
inline U32 load_u32(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const U32*>(address);
}

inline I32 load_i32(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const I32*>(address);
}

inline U8 load_u8(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<const U8*>(address);
}

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}

inline void store_u32(void* base, std::uintptr_t offset, U32 value) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    *reinterpret_cast<U32*>(address) = value;
}

inline void store_ptr(void* base, std::uintptr_t offset, void* value) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    *reinterpret_cast<void**>(address) = value;
}

// [ [007A39FC] + 4 ]: client-internal pointer. Re-read on every call.
inline void* client_internal() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 4);
}

// [ [007A39FC] + 8 ]: server pointer. Re-read on every call.
inline void* server_app() {
    return load_ptr(kotor_menu_window_cells::manager_007a39fc, 8);
}

// vtable +0x30 in the original is a no-stack-argument thiscall returning a
// pointer/int. Use a fastcall pointer with a dummy EDX; the callee ignores EDX.
using VFn0 = void* (__fastcall*)(void*, void*);
inline void* vcall0(void* self, std::uintptr_t byte_offset) {
    void** const vtable = *reinterpret_cast<void***>(self);
    return reinterpret_cast<VFn0>(vtable[byte_offset / 4])(self, nullptr);
}

// Dialog vtable [0] scalar deleting destructor: one stack dword (free flag),
// callee-clean RET 4 (no caller add esp after the original call).
using VDel = void (__fastcall*)(void*, void*, U32);
inline void vdel(void* object, U32 flag) {
    void** const vtable = *reinterpret_cast<void***>(object);
    reinterpret_cast<VDel>(vtable[0])(object, nullptr, flag);
}

inline int queue_entry(void* self, U32 entry) {
    store_u32(self, 0x4C, entry);
    store_u32(self, 0x48, 1);
    return 1;
}

inline int queue_replies(void* self) {
    store_u32(self, 0x48, 1);
    store_u32(self, 0x4C, 0xFFFFFFFFu);
    return 1;
}

}  // namespace endar_dialog_core
