#pragma once
#include "c630930406-resource_lower.h"
#include <cstdint>

// Manager and consumed CRes boundary are imported from the sealed current cohort.
// Resource, list internal, key-table entry, and effect receiver remain opaque.
extern "C" {
std::uint32_t __fastcall c2da_manager_release_demand_body_00408c90(
    menu_resources::Manager*,void*,void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
void __fastcall c2da_005e9c10(void*,void*,void*);
void* __fastcall c2da_005e9d70(void*,void*,void*);
void* __fastcall c2da_manager_004076e0(menu_resources::Manager*,void*,void*);
std::uint32_t __fastcall c2da_005e6990(void*,void*);
void __stdcall c2da_sleep_0073d1e4(std::uint32_t);
std::uint32_t __fastcall c2da_manager_00408530(menu_resources::Manager*,void*);
std::uint32_t __fastcall c2da_manager_004078f0(menu_resources::Manager*,void*,void*,std::uint32_t);
std::uint32_t __fastcall c2da_manager_00407bd0(menu_resources::Manager*,void*,void*,std::uint32_t);
std::uint32_t __fastcall c2da_manager_00407d50(menu_resources::Manager*,void*,void*,std::uint32_t);
std::uint32_t __fastcall c2da_manager_00407e00(menu_resources::Manager*,void*,void*,std::uint32_t);
void __fastcall c2da_manager_00408b90(menu_resources::Manager*,void*,void*,std::uint32_t);
}
