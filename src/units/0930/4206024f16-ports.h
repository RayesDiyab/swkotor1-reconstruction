#pragma once
// Assembly-call-only: low/high IEEE binary64 stack words; EAX32; caller cleanup.
// Exact machine code also reads two padding bytes after the argument and masks them out.
extern "C" void __cdecl task8_math_port_006fe52c();
extern "C" void __cdecl task8_math_port_0070762a();
extern "C" const unsigned char task8_pow_const_0073edf8[16];
