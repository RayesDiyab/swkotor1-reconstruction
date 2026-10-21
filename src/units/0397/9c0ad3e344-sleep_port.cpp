#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "d06e8b44c9-manager_demand.h"
// Original IAT Sleep port0073D1E4. Real SDK/Kernel32 import, no scheduler substitute.
// Composition only; there is no new recovered implementation/address credit.
extern "C" __declspec(noinline) void __stdcall c2da_sleep_0073d1e4(std::uint32_t ms) {
    ::Sleep(ms);
}
