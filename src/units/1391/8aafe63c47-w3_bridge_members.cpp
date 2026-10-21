// lane-w3 composition bridge (member-function owners). Compiled as its own TU against the EXISTING M60 headers
// (include roots: m60-compose/s, .../task-12/menu-provider-40-49/normal-retained/source,
// .../task-36/menu-services-00-09/resource-r1/source, m60-compose/s/frame-gui/source). Each export forwards to the
// existing runtime1115 member owner; nothing is re-implemented.
#include "b1517191e8-normal.h"             // menu_normal_retained_40_49::Module (0034-service-normal-17.obj construct_00643f40)
#include "00a0946121-resources.h"          // menu_resources::Manager (0027-service-resource-10.obj encapsulated_004087a0)
#include "cfe9cac1ae-menu_continuation.h"  // menu_continuation::ExoTimers (0457-r4-lowers GetHighResolutionTimer_005e9260)

extern "C" {
// 00643F40 CSWCModule::CSWCModule on caller memory (138h bytes).
void* __fastcall recovered_bridge_module_construct_00643f40(void* memory, void*) {
    return static_cast<menu_normal_retained_40_49::Module*>(memory)->construct_00643f40();
}
// 004087A0 CExoResMan::AddEncapsulatedResourceFile(CExoString const&, int).
std::uint32_t __fastcall recovered_bridge_resman_add_encapsulated_004087a0(void* resman, void*, void* name, std::uint32_t type) {
    return static_cast<menu_resources::Manager*>(resman)->encapsulated_004087a0(
        static_cast<kotor_reconstruction::StartupString*>(name), type);
}
// 005E9260 CExoTimers::GetHighResolutionTimer.
std::uint64_t __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*) {
    return static_cast<std::uint64_t>(static_cast<menu_continuation::ExoTimers*>(timers)->GetHighResolutionTimer_005e9260());
}
}
