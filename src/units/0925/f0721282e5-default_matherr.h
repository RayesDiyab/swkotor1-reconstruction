#pragma once
// Assembly-call-only port. Original 00706A22 ignores the caller's stack word,
// clears all 32 bits of EAX, and returns without popping that word.
// A C++ void declaration deliberately avoids inventing an exception-record type.
extern "C" void __cdecl task8_math_port_00706a22(void);
