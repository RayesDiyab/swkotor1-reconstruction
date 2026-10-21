// Native binding of w2::ftol2_006fae8c to the existing __ftol2 provider (custom ABI: consumes ST(0), returns EDX:EAX).
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "28478d5d50-w2_ftol.hpp"
extern "C" void __cdecl genuine_x87_ftol_006fae8c();
namespace w2 {
std::uint32_t ftol2_006fae8c(double value) {
    std::uint32_t eax_out;
    __asm {
        fld qword ptr value
        call genuine_x87_ftol_006fae8c
        mov eax_out, eax
    }
    return eax_out;
}
}
