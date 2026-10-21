#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <stdint.h>

// Link obligations, not replacement allocator or Kernel32 implementations.
namespace kotor_startup_dependencies {
void* __cdecl allocate_internal(uint32_t bytes);
void __cdecl free_internal(void* storage);
void __stdcall initialize(CRITICAL_SECTION* storage);
void __stdcall destroy(CRITICAL_SECTION* storage);
void __stdcall enter(CRITICAL_SECTION* storage);
void __stdcall leave(CRITICAL_SECTION* storage);
}

namespace kotor_startup {
struct CriticalSectionInternal {
    CRITICAL_SECTION storage;
    CriticalSectionInternal* __thiscall Construct(); // 005EC250
    void __thiscall Destroy();                      // 005EC260
    void __thiscall Enter();                        // 005EC270
    void __thiscall Leave();                        // 005EC280
};
struct CriticalSection {
    CriticalSectionInternal* internal;
    CriticalSection* __thiscall Construct(); // 005E9290
    void __thiscall Enter();                // 005E9300
    void __thiscall Leave();                // 005E9310
    void __thiscall Destroy();              // 005E9320
};
static_assert(sizeof(void*) == 4, "GOG x86 contract");
static_assert(sizeof(CriticalSectionInternal) == 0x18, "Win32 internal layout");
static_assert(sizeof(CriticalSection) == 4, "wrapper pointer only");
}
