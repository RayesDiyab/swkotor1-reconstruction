#pragma once
#include "15d7bcda8b-resources.h"
#include <cstdint>
namespace c2da_resource_lower {
using Word=std::uint32_t;
using Ref=menu_resources::Ref;
using Manager=menu_resources::Manager;
}
extern "C" {
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
int __fastcall c2da_equal_text_004060d0(menu_resources::Ref*,void*,const char*);
// Genuine lower imports; this packet defines no replacement manager or vtable.
void __fastcall c2da_manager_cancel_004088f0(menu_resources::Manager*,void*,void*);
std::uint32_t __fastcall c2da_manager_demand_004089f0(menu_resources::Manager*,void*,void*);
void __fastcall c2da_manager_release_demand_00408c90(menu_resources::Manager*,void*,void*);
void __fastcall c2da_resource_serviced_00409b40(void*,void*,std::uint32_t);
void* __fastcall c2da_resource_base_ctor_00406460(void*,void*);
const void* __cdecl c2da_resource_methods_0073ec9c();

void __fastcall c2da_cancel_00409b00(void*,void*);
int __fastcall c2da_release_00409cf0(menu_resources::Manager*,void*,void*);
void* __fastcall c2da_resource_ctor_0041d730(void*,void*);
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*);
int __fastcall c2da_not_equal_text_00406060(menu_resources::Ref*,void*,const char*);
std::uint32_t __fastcall c2da_current_resource_demand_00409b10(void*,void*);
void __fastcall c2da_release_demand_00409b80(void*,void*);
void* __fastcall c2da_current_resource_data_00404dd0(void*,void*);
std::uint32_t __fastcall c2da_current_resource_size_00710e50(void*,void*);
}
