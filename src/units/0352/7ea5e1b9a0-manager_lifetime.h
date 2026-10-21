#pragma once
#include "40141ba4d9-manager_destructor.h"
#include "f166f203af-base_ini.h"
#include "c46300269d-allocator_live.h"
namespace menu_resources {
// Raw, suitably aligned96-byte storage is provided by the original allocator.
// This bridge starts actual canonical C++ lifetimes without preclearing bytes.
// String+4C starts solely inside genuine construct_00409bf0.
__declspec(noinline) Manager* __fastcall ConstructOwnedManager00409bf0(void*,void*);
// Normal verified destructor sequence first; trivial wrapper/Manager lifetime
// ends follow. Parent StopServices still owns Manager free and sole-cell clear.
// Production requires externally verified worker termination and stable owners.
__declspec(noinline) void __fastcall DestroyOwnedManager00408670(Manager*,void*);
// Actual allocator heap identity only; this does not establish worker drain.
__declspec(noinline) bool __cdecl ManagerHeapMatches(HANDLE captured);
}
