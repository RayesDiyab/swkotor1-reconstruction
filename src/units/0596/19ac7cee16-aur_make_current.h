#pragma once
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Approved MSVC x86 compiler required
#endif
// AurMakeCurrent 0044ED90..0044EDFF (Ghidra name; 112 bytes). Despite the name it does
// NOT call wglMakeCurrent: optional clock busy-wait, GlobalMemoryStatus -> available
// physical KiB float published to 007B9274 (WinMain frame-word bias, 00404576), then a
// tail jump to the texture scheduler 00421EE0 (pending-texture build/upload work).
namespace aur_make_current {
// Borrowed game cells. The sole owner (Main/game_prepare) publishes the actual objects;
// this unit defines no storage for them.
struct Cells {
    const float volatile* delay_007b9270; // read at 0044ED90 / 0044EDA6 only
    float volatile* bias_007b9274;        // written at 0044EDF2; WinMain reads it at 00404576
};
extern const Cells& cells;
}
extern "C" {
// Original CRT _clock 006FC546 (Ghidra library match "_clock"); genuine provider is a
// Main decision (not substituted here). cdecl, no arguments, clock_t (long) in EAX.
long __cdecl crt_clock_006fc546();
// Existing task25 FTOL provider (M40 world-timer-x87-conversion.obj). Custom ABI:
// consumes ST(0), returns signed 64 bits in EDX:EAX, no stack arguments.
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
// Texture scheduler 00421EE0 as declared by task-8 texture-scheduler-source-r1
// (scheduler.h:57, extern "C" U __stdcall, no arguments).
unsigned int __stdcall texture_make_current_00421ee0();
// Name and shape exactly as consumed by corrected R4 composition.h:18 (draw.cpp:46/48,
// entry_router.cpp:31): cdecl, no arguments, no result (EAX dead at all four original
// callers 00401C11, 0040CC63, 00419418, 0060339F).
void __cdecl frame_RenderBoundary_0044ed90();
}
