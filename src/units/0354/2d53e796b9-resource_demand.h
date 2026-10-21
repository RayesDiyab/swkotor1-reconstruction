#pragma once
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This slice requires the pinned MSVC x86 ABI.
#endif
namespace menu_resources {struct Manager;}
namespace kotor_resource_demand {
// ABI views only. Neither class describes, owns or accesses an original field.
// Never use their sizeof or construct them as a production resource/manager.
struct ResourceDemandView {
    __declspec(noinline) std::int32_t __thiscall Demand();
};
struct ResourceManagerView {
    // Unresolved original004089F0: ECX manager, one stack resource word, RET4.
    std::int32_t __thiscall Demand(ResourceDemandView* resource);
};
// Unresolved production owner/binding for original007A39E8. No storage here.
extern menu_resources::Manager* volatile resource_manager_007a39e8;
// Canonical Manager-typed004089F0 provider remains unresolved until its owned
// Manager/Resource/List caller cohort is converted; no live facade/downcast.
std::int32_t __fastcall DemandOnCanonicalManager004089f0(
    menu_resources::Manager*,void*,ResourceDemandView*);
}
