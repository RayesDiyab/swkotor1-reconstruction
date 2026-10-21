#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_exp2_00704200() {
    __asm {
        // 00704200
        fld         st(0)
        // 00704202
        frndint
        // 00704204
        fsubr       st(1),st
        // 00704206
        fxch        st(1)
        // 00704208
        fchs



        // 0070420a
        f2xm1
        // 0070420c
        fld1
        // 0070420e
        faddp       st(1),st
        // 00704210
        fscale
        // 00704212
        fstp        st(1)
        // 00704214
        ret
    }
}
