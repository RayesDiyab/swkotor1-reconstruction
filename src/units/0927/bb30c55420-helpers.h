#pragma once
#include <stdint.h>
// x86 cdecl: caller cleans 4 or 8 bytes. EAX is a full 32-bit result.
extern "C" uint32_t __cdecl task8_math_port_00707c06(int32_t);
extern "C" int32_t __cdecl task8_math_port_00707e25(uint32_t, uint32_t);
// Assembly-only existing sealed errno bridge; EAX carries int*.
extern "C" void __cdecl task8_math_port_006ffe78();
