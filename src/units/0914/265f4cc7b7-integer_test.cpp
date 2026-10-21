#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_integer_test_006fc412() {
    __asm {
        // 006fc412
        fld         st(0)
        // 006fc414
        frndint
        // 006fc416
        fcomp       st(1)
        // 006fc418
        mov         cl,0
        // 006fc41a
        wait
        // 006fc41b
        fnstsw      ax
        // 006fc41d
        sahf
        // 006fc41e
        jne L_006fc437
        // 006fc420
        fmul        qword ptr [task8_pow_const_007a2780]
        // 006fc426
        inc         cl
        // 006fc428
        fld         st(0)
        // 006fc42a
        frndint
        // 006fc42c
        fcompp
        // 006fc42e
        wait
        // 006fc42f
        fnstsw      ax
        // 006fc431
        sahf
        // 006fc432
        jne L_006fc436
        // 006fc434
        inc         cl
    L_006fc436:
        ret
    L_006fc437:
        fstp        st(0)
        // 006fc439
        ret
    }
}
