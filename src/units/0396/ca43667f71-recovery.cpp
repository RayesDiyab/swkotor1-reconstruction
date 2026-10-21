#include "ccfda63995-case_bodies.h"
#include <malloc.h>
#include <type_traits>
static_assert(std::is_same_v<decltype(&c2da_case_stack_recover_00708b4f),void(__cdecl*)()>);
static_assert(std::is_same_v<decltype(&_resetstkoflw),int(__cdecl*)()>);



// Modern CRT seam, not a reconstructed original algorithm or parity claim.
// Invoke the installed CRT implementation and preserve the caller's ignored result.
extern "C" void __cdecl c2da_case_stack_recover_00708b4f() {
    (void)_resetstkoflw();
}
