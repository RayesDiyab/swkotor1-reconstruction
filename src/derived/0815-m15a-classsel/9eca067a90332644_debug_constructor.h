#pragma once
#include "e1805047c9b7563b_startup_string.h"
#include <cstddef>
#include <cstdint>
namespace kotor_menu20 {
// Unknown original word meanings remain unknown. Constructor leaves14h/18h.
struct DebugInternal {
    volatile std::uint32_t words[7];
    __declspec(noinline) __thiscall DebugInternal();
};
// Complete16-byte object replaces the older12-byte borrowed Debug declaration
// only in this isolated coherent cohort. The String is the existing actual type.
struct Debug {
    kotor_reconstruction::StartupString filename;
    DebugInternal* volatile internal;
    volatile std::uint32_t enable_release_logging;
    __declspec(noinline) __thiscall Debug();
    __declspec(noinline) void __thiscall open_logs_005e93f0(std::uint32_t argument);
};
static_assert(sizeof(DebugInternal)==28 && sizeof(Debug)==16 &&
    offsetof(Debug,filename)==0 && offsetof(Debug,internal)==8 &&
    offsetof(Debug,enable_release_logging)==12,"observed x86 Debug layouts");
// Physical ECX receiver / ignored EDX / no stack args / full pointer EAX.
// These placement entries begin actual C++ object lifetimes; no nominal cast.
__declspec(noinline) Debug* __fastcall ConstructDebug005e9370(Debug*,void*);
__declspec(noinline) DebugInternal* __fastcall ConstructDebugInternal005ec290(DebugInternal*,void*);
}
