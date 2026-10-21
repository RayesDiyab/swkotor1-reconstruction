#include "d96d4f8406-r7_exit_setter_joins.h"

#if !defined(_M_IX86)
#error "005EDBE0 is an x86 thiscall body"
#endif

// Combined 005EDBE0 -> 0061F7C0 body. The original forwarder loads the inner
// client object from outer+4 and the target stores a 32-bit 1 at inner+0x2BC.
// Keep the direct accesses: no guard, no fabricated return value, no trap.
extern "C" void __fastcall disable_input_005edbe0(void* outer, void*) {
  unsigned char* inner = *reinterpret_cast<unsigned char**>(
      static_cast<unsigned char*>(outer) + 4);
  *reinterpret_cast<unsigned*>(inner + 0x2bc) = 1;
}
