#pragma once
#include "0906d6ed87-resources.h"
namespace kotor_resource_demand {
// Migration of the EXISTING Task12 source-owned007A39E8 slot. Adopt as a
// replacement declaration/definition cohort, never alongside the old cell.
extern menu_resources::Manager* volatile resource_manager_007a39e8;
}
namespace menu_resources {
__declspec(noinline) Manager* __cdecl BorrowCanonicalManager007a39e8();
}
