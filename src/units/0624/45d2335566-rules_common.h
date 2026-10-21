#pragma once
// lane-rules: shared contract for the CSWRules/2DA subtree reconstructed from
// swkotor.exe (GOG K1, sha256 9c10e045..., base 0x400000).
// D1 form: original __thiscall -> __fastcall(self, unused EDX, stack args...).
// Receivers are minimum addressed views of the original allocations; only the
// cited byte offsets are read/written (no invented fields).
// Every provider declared here is an EXISTING owner (M41 object or another lane
// packet, cited per line); identical declarations to the owner's header.
#include "c534281817-startup_string.h"   // M41 kotor_reconstruction::StartupString (005B3190/005E5A90/005E5C20, 005E5290, 005E5790, 005E5C50)
#include "c30aaa70eb-resources.h"        // M41 menu_resources::Ref (16-byte CResRef) + construct_00406d80
#include "10d1301a0a-table_contract.h"   // lane-startup-misc donor-c2da: c2da_current::Table (0x54) + 00413CC0/00414A30
#include "f430e94c06-load.h"             // lane-startup-misc donor-c2da: 004143B0
#include "e76b1c7603-getstring.h"        // lane-startup-misc donor-c2da: 00413100 / 00413270 / GuiAssign005e5140
#include "7b8c81fcff-allocator_live.h"   // M41 live_operator_new 006FA7E6 / live_free 006FA390
#include "9d93d7ea2f-rules_image_data.h" // D13: original .rdata strings (definitions ONLY in the local generated unit)
#include <cstdint>
#include <cstring>
#include <new>

namespace options_video { struct String; }   // lane-config StringNotEqual_005e5390 receiver type (object view)

extern "C" {
// ---- C2DA (lane-startup-misc D38 / lane-tlk D44) ----
__declspec(noinline) std::int32_t __fastcall c2da_current_get_float_00413430(c2da_current::Table*, void*, std::int32_t, c2da_current::String*, float*);
__declspec(noinline) std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, std::int32_t, c2da_current::String*, std::int32_t*);
void __fastcall c2da_current_unload_004139e0(c2da_current::Table*, void*);
std::int32_t __fastcall c2da_current_get_string_entry_00413190(c2da_current::Table* self, void* unused_edx,
    std::int32_t row, std::int32_t column, kotor_reconstruction::StartupString* out);           // lane-tlk, RET0Ch
std::int32_t __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table* self, void* unused_edx,
    std::int32_t row, std::int32_t column, std::int32_t* out);                                  // lane-tlk, RET0Ch
// ---- CExoString (M41 / lanes) ----
kotor_reconstruction::StartupString* __fastcall resman_string_from_int_005e5bc0(
    kotor_reconstruction::StartupString* self, void* unused_edx, int value);                  // lane-resman 005E5BC0
// ---- CResRef (M41 / D9 / D45) ----
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*, void*);                 // D9 lane-m42-menu-joins
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, menu_resources::Ref*); // D45 lane-tpc assign6120
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref*, void*, menu_resources::Ref*, const char*); // M41 resource-ref-assignment
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*, void*, menu_resources::String*);  // M41 ref-bodies
// ---- CRT (>=006FA000; eh vector iterators are original-runtime ports) ----
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, std::uint32_t size, std::int32_t count,
    void* (__fastcall* construct)(void*, void*), void (__fastcall* destroy)(void*, void*));      // lane-3d-model
void __stdcall c2da_destroy_array_006fa852(void*, c2da_current::Word, c2da_current::Word,
    c2da_current::StringDestroy);                                                              // M41 task10-c2da-array-normal
const char* __cdecl c2da_empty_0073d71c();
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*, void*);                       // M41 window-native-strings (005E5670 CExoString::CStr)                                                     // M41 (startup-misc binding.h import)
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* output, const kotor_reconstruction::StartupString* right); // M41 ini-string-priority (C++ linkage, global ns)
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // M41 canonical-assignment-format
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*, int);  // M41
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*, int); // M41
}
namespace alias_add {
kotor_reconstruction::StartupString* __fastcall Upper005e6360(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*); // M41
}
namespace directory_list {   // lane-resource-lowers (single owner of 005E5600 / 005E6080)
char __fastcall At005e5600(kotor_reconstruction::StartupString*, void*, std::int32_t);
kotor_reconstruction::StartupString* __fastcall LowerCase005e6080(kotor_reconstruction::StartupString*, void*, kotor_reconstruction::StartupString*);
}
namespace client_movies {    // lane-config (005E5390 CExoString::operator!=(char const*), RET4)
std::int32_t __fastcall StringNotEqual_005e5390(options_video::String* self, void* unused_edx, const char* text);
}
extern "C" {
// 007A39DC: byte cell read by Load2DArrays (005C4D1F/005C4DFE) and LoadIPRPCostTables
// (005C47F9). .data initial value 0; no writer in the whole .text (full dumpbin scan:
// 11 references, all reads). Owner: lane-rules.
// declared as kotor_rules_cell8_007a39dc in rules_image_data.h (D13 generated cell, image value)
}

namespace kotor_rules {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
using Table = c2da_current::Table;
using Ref = menu_resources::Ref;

inline unsigned char* bytes(void* p) noexcept { return static_cast<unsigned char*>(p); }
inline Word rd32(const void* base, unsigned off) noexcept { Word v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, 4); return v; }
inline void wr32(void* base, unsigned off, Word v) noexcept { std::memcpy(static_cast<unsigned char*>(base) + off, &v, 4); }
inline std::uint16_t rd16(const void* base, unsigned off) noexcept { std::uint16_t v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, 2); return v; }
inline void wr16(void* base, unsigned off, std::uint16_t v) noexcept { std::memcpy(static_cast<unsigned char*>(base) + off, &v, 2); }
inline unsigned char rd8(const void* base, unsigned off) noexcept { return static_cast<const unsigned char*>(base)[off]; }
inline void wr8(void* base, unsigned off, unsigned char v) noexcept { static_cast<unsigned char*>(base)[off] = v; }
inline void wrf(void* base, unsigned off, float v) noexcept { std::memcpy(static_cast<unsigned char*>(base) + off, &v, 4); }
template<class T> inline T* ptr_at(const void* base, unsigned off) noexcept {
    return reinterpret_cast<T*>(static_cast<std::uintptr_t>(rd32(base, off)));
}
inline void put_ptr(void* base, unsigned off, const void* p) noexcept {
    wr32(base, off, static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)));
}

// 006FA7E6 + ctor with the original EH-funclet behaviour (state 0): when the
// constructor unwinds, the block is released through 006FA390. Null allocation
// -> constructor skipped, result 0 (cmp/je -> xor eax,eax).
template<class Ctor> inline void* allocate_construct(Word size, Ctor ctor) {
    void* p = live_operator_new(size);
    if (!p) return nullptr;
    try { return ctor(p); }
    catch (...) { live_free(p); throw; }
}
// Standard "new C2DA(CResRef(name), 0)" sequence: push 54h / call 006FA7E6 /
// push 0 / sub esp,10h / mov ecx,esp / push name / call 00406D80 / mov ecx,eax-block / call 00413CC0.
inline Table* new_c2da(const char* name) {
    return static_cast<Table*>(allocate_construct(0x54, [name](void* p) {
        Ref r;                                       // stack CResRef slot (sub esp,10h; mov ecx,esp)
        r.construct_00406d80(name);                  // 00406D80
        return static_cast<void*>(c2da_current_construct_00413cc0(p, nullptr, r, 0)); // 00413CC0 RET14h
    }));
}
// "call [vtbl+0](1)" on a C2DA: typed slot0 (D41) = 00414A30 deleting dtor.
inline void delete_c2da(Table* t) {
    static_cast<const c2da_current::TableSlot0*>(t->helper.methods)->deleting(t, nullptr, 1);
}
// GetINTEntry / GetFLOATEntry / GetCExoStringEntry with a temporary CExoString column
// (005E5A90 ... call ... 005E5C20; unwind state destroys the temporary).
inline std::int32_t int_entry(Table* t, std::int32_t row, const char* column, std::int32_t* out) {
    String name(column);
    return c2da_current_get_int_00413660(t, nullptr, row, &name, out);
}
inline std::int32_t float_entry(Table* t, std::int32_t row, const char* column, float* out) {
    String name(column);
    return c2da_current_get_float_00413430(t, nullptr, row, &name, out);
}
inline std::int32_t string_entry(Table* t, std::int32_t row, const char* column, String* out) {
    String name(column);
    return c2da_current_get_string_00413270(t, nullptr, row, &name, out);
}
inline std::int32_t row_count(const Table* t) noexcept { return static_cast<std::int32_t>(rd32(t, 0x24)); } // C2DA +24h rows
inline std::int32_t column_count(const Table* t) noexcept { return static_cast<std::int32_t>(rd32(t, 0x28)); } // C2DA +28h columns
}
