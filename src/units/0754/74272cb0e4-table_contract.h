#pragma once
#include "44007537dd-binding.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <type_traits>

namespace c2da_current {
using Helper=c2da_binding::Helper;
using Ref=menu_resources::Ref;
using String=kotor_reconstruction::StartupString;
using Word=std::uint32_t;

// One concrete producer carrier: actual Helper first subobject, an explicitly
// constructed String at the observed offset, and an otherwise opaque tail.
struct Table {
    Helper helper;
    alignas(String) unsigned char string_storage[sizeof(String)];
    alignas(4) unsigned char opaque_tail[48];
};
static_assert(std::is_standard_layout<Helper>::value && std::is_standard_layout<Table>::value,"actual first-member subobject");
static_assert(sizeof(Helper)==28 && sizeof(Table)==84 && alignof(Table)==4,"observed producer storage");
static_assert(offsetof(Table,helper)==0 && offsetof(Table,string_storage)==0x1c && offsetof(Table,opaque_tail)==0x24,"observed constructor offsets");
static_assert(std::is_trivially_default_constructible<Ref>::value && std::is_trivially_destructible<Ref>::value,"canonical trivial Ref lifetime");
static_assert(std::is_trivially_default_constructible<Helper>::value && std::is_trivially_destructible<Helper>::value,"actual trivial Helper lifetime");
static_assert(std::is_trivially_default_constructible<Table>::value && std::is_trivially_destructible<Table>::value,"actual trivial Table lifetime");
static_assert(sizeof(String)==8 && sizeof(Word)==4 && sizeof(void*)==4,"current native canonical carriers");

inline Word load_word(const Table* self,unsigned offset) {
    Word result;std::memcpy(&result,reinterpret_cast<const unsigned char*>(self)+offset,4);return result;
}
inline void store_word(Table* self,unsigned offset,Word value) {
    std::memcpy(reinterpret_cast<unsigned char*>(self)+offset,&value,4);
}
template<class T> inline T* load_pointer(const Table* self,unsigned offset) {
    T* result;std::memcpy(&result,reinterpret_cast<const unsigned char*>(self)+offset,4);return result;
}
inline String* default_string(Table* self) {
    return std::launder(reinterpret_cast<String*>(self->string_storage));
}
using StringDestroy=void(__fastcall*)(String*,void*);
struct HelperSlot0 {Helper*(__fastcall* deleting)(Helper*,void*,Word);};
struct TableSlot0 {Table*(__fastcall* deleting)(Table*,void*,Word);};
static_assert(sizeof(HelperSlot0)==4 && sizeof(TableSlot0)==4,"only the observed native slot0");
const HelperSlot0* initial_methods() noexcept;
const TableSlot0* final_methods() noexcept;
}

extern "C" {
// Entry consumes ECX receiver, Ref16 by value and flag32, RET20. Its source
// helper receives a pointer to the actual incoming Ref stack slot.
c2da_current::Table* __fastcall c2da_current_construct_00413cc0(
    void*,void*,c2da_current::Ref,c2da_current::Word);
c2da_current::Table* __cdecl c2da_current_construct_from_stack_00413cc0(
    void*,c2da_current::Ref*,c2da_current::Word);
void __fastcall c2da_current_destroy_00413d80(c2da_current::Table*,void*);
void __fastcall c2da_current_unload_004139e0(c2da_current::Table*,void*);
void __fastcall c2da_current_helper_destroy_00413ae0(c2da_current::Helper*,void*);
c2da_current::Table* __fastcall c2da_current_delete_00414a30(
    c2da_current::Table*,void*,c2da_current::Word);
c2da_current::Helper* __fastcall c2da_current_helper_delete_00413ca0(
    c2da_current::Helper*,void*,c2da_current::Word);
// Genuine owner imports, never fallback definitions in production.
c2da_current::Ref* __fastcall c2da_default_ref_00405ed0(c2da_current::Ref*,void*);
void __fastcall c2da_release_demand_00409b80(void*,void*);
void __stdcall c2da_destroy_array_006fa852(
    void*,c2da_current::Word,c2da_current::Word,c2da_current::StringDestroy);
void __fastcall c2da_current_string_destroy_005e5c20(c2da_current::String*,void*);
}
