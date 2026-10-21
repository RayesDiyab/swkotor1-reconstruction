#pragma once
#include <Windows.h>
#include <cstddef>
#include <cstdint>
namespace kotor_startup {
// The saved one-byte Ghidra placeholder is not the allocated object extent.
// Allocation32 and stores through+31 establish this observed complete layout.
struct TimersInternal {
    volatile std::int32_t supported;
    volatile std::uint32_t mode;
    volatile std::uint32_t frequency_low,frequency_high;
    volatile std::uint32_t quotient_low,quotient_high;
    volatile double period;
};
struct Timers {TimersInternal* volatile internal;};
struct TimerImports {BOOL (WINAPI *query_performance_frequency)(LARGE_INTEGER*);};
// Link exactly one import context. The runtime context binds the real DLL API;
// the finite fixture context is expressly a controlled API-result seam.
extern const TimerImports timer_imports;
static_assert(sizeof(Timers)==4 && sizeof(TimersInternal)==32 &&
    offsetof(TimersInternal,mode)==4 && offsetof(TimersInternal,frequency_low)==8 &&
    offsetof(TimersInternal,quotient_low)==16 && offsetof(TimersInternal,period)==24,
    "observed x86 Timers allocation and stores");
__declspec(noinline) Timers* __fastcall ConstructTimers005e91e0(Timers*,void*);
__declspec(noinline) TimersInternal* __fastcall ConstructTimersInternal005ec160(TimersInternal*,void*);
}
