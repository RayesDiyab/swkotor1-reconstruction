#pragma once
// lane-window: CExoArrayList storage owners (one definition shared by all lane units).
// Storage is constant-initialised and trivially destructible (.bss zero, as in the image):
// no .CRT$XC* initializer and no compiler-registered atexit in any lane-window object.
// The original DOES have genuine static initializers for both lists, which only register
// their atexit frees (no constructor runs; the storage is loader-zero BSS):
//   __xc entry 0078D0C8 -> 0073B110: push 0073BE50; call _atexit 006FB332  (fonts 0083052C)
//   __xc entry 0078D0CC -> 0073B120: push 0073BE70; call _atexit 006FB332  (vertex programs 008305CC)
// The two callbacks are provided below as plain functions; registering them belongs to the
// integrator's reviewed initializer policy (open gate), not to these units.
#include "459924a746-array_mutations.h"
#include <cstdint>
namespace kotor_aurora_window {
extern kotor_reconstruction::ArrayMutations fonts_0083052c;            // 0083052C data / 00830530 size / 00830534 capacity
extern kotor_reconstruction::ArrayMutations vertex_programs_008305cc;  // 008305CC data / 008305D0 size / 008305D4 capacity
}
extern "C" {
void __cdecl kotor_atexit_0073be50();   // free(fonts.data) if non-null (0073BE50..0073BE80)
void __cdecl kotor_atexit_0073be70();   // free(vertex_programs.data) if non-null (0073BE70..0073BE80)
}
