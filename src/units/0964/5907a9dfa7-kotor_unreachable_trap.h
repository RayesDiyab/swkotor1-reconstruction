#pragma once
// D83 shared fatal trap (lane-m48-traps). One definition: src/trap/kotor_unreachable_trap.cpp.
// Every D83 trap body / trapped branch calls this with the ORIGINAL instruction or function address and the
// original symbol. It never returns: it appends one line to <exe dir>\kotor_unreachable_trap.log, emits the same
// line through OutputDebugStringA, and terminates the process with exit code KOTOR_UNREACHABLE_TRAP_EXIT_CODE.
// Traps are milestone-2+ debt (TRAPS.json); they are not reconstructed behaviour and never return a value.
#define KOTOR_UNREACHABLE_TRAP_EXIT_CODE 0x0D83D83Du
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long original_address, const char* symbol);
