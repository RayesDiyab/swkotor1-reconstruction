#pragma once
#include "30f95f1762-base_ini.h"
#include "78a4d5c6e4-base_extension.h"
namespace path_resolution {
using String=kotor_reconstruction::StartupString;using Word=std::uint32_t;
using Base=base_ini_reconstruction::Base;
// Borrow the actual nonvolatile void* cell through const volatile access. No
// pointer-to-pointer reinterpretation or duplicate Base/extension view type.
struct Bindings {void* const volatile* base_007a39e0;void(__cdecl* format_005e5680)(String*,const char*,...);const char* empty_0073d71c;const char* with_extension_0074e470;const char* without_extension_00740ccc;};
extern const Bindings* bindings;
extern "C" std::int32_t __fastcall find_005e5f20(String*,void*,Word,std::int32_t);
String* __fastcall resolve_005e68a0(alias_path::AliasListView*,void*,String*,String*,Word);
String* __fastcall resolve_internal_005eb6b0(alias_path::InternalView*,void*,String*,String*,Word);
Bindings ForActualOwners(const char*,const char*,const char*);
String* __fastcall ResolveActual(void*,void*,String*,String*,Word);
}
