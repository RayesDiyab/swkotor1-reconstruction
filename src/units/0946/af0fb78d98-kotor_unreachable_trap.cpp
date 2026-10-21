// Terminate when execution reaches an unavailable reconstructed function.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "5907a9dfa7-kotor_unreachable_trap.h"
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long, const char*) {
    TerminateProcess(GetCurrentProcess(), KOTOR_UNREACHABLE_TRAP_EXIT_CODE);
    ExitProcess(KOTOR_UNREACHABLE_TRAP_EXIT_CODE);
}
