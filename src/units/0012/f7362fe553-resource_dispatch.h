#pragma once
#include "71329cf9aa-resource_methods.h"
#include "87898e7506-base_methods.h"
#include <cstdint>

// The identity branch is the same contract as the r2 CRes2DA loader proposal.
// Each genuine descriptor uses its own exact source callback type.
// Other descriptors retain the prior thiscall transport and its domain limits.
namespace c2da_r29_dispatch {
using Word=std::uint32_t;
inline const void* table(void* resource) {
    return *reinterpret_cast<const void* volatile*>(resource);
}
inline Word serviced(void* resource) {
    const void* methods=table(resource);
    if(methods==&c2da_resource_method_table_0073ec9c)
        return c2da_resource_method_table_0073ec9c.serviced_0041d790(resource,nullptr);
    if(methods==&cres_base_method_table_0073d86c)
        return cres_base_method_table_0073d86c.serviced_0063e7a0();
    using Legacy=Word(__thiscall*)(void*);
    const auto bytes=static_cast<const unsigned char*>(methods);
    return (*reinterpret_cast<Legacy const volatile*>(bytes+16))(resource);
}
inline void freed(void* resource) {
    const void* methods=table(resource);
    if(methods==&c2da_resource_method_table_0073ec9c) {
        (void)c2da_resource_method_table_0073ec9c.freed_0041d770(resource,nullptr);
        return;
    }
    if(methods==&cres_base_method_table_0073d86c) {
        (void)cres_base_method_table_0073d86c.freed_0063e7a0();
        return;
    }
    using Legacy=void(__thiscall*)(void*);
    const auto bytes=static_cast<const unsigned char*>(methods);
    (*reinterpret_cast<Legacy const volatile*>(bytes+12))(resource);
}
}
