#pragma once
// MSVC x86 assembly-call contracts; ordinary C++ calls do not supply ST0/ST1.
// ftol: consume ST0, return all 64 result bits in EDX:EAX, retain deeper stack.
extern "C" void __cdecl genuine_x87_ftol_006fae8c();
// pow entry: consume exponent ST0 and base ST1, return ST0. The original
// implementation deliberately spills binary64 operands inside its two paths.
// This transport is complete; arithmetic providers and CPU-selection cell
// remain strict imports, explicitly held in REPORT.md.
extern "C" void __cdecl genuine_x87_pow_006fc1f0();
