#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_classify_integer_00705177() {
    __asm {
        // 00705177
        push        ebp
        // 00705178
        mov         ebp,esp
        // 0070517a
        push        ecx
        // 0070517b
        push        ecx
        // 0070517c
        fld         qword ptr [ebp+8]
        // 0070517f
        push        ecx
        // 00705180
        push        ecx
        // 00705181
        fstp        qword ptr [esp]
        // 00705184
        call task8_math_port_006fe52c
        // 00705189
        test        al,90h
        // 0070518b
        pop         ecx
        // 0070518c
        pop         ecx
        // 0070518d
        jne L_007051dd
        // 0070518f
        fld         qword ptr [ebp+8]
        // 00705192
        push        ecx
        // 00705193
        push        ecx
        // 00705194
        fstp        qword ptr [esp]
        // 00705197
        call task8_math_port_0070a53b
        // 0070519c
        fstp        qword ptr [ebp-8]
        // 0070519f
        pop         ecx
        // 007051a0
        fld         qword ptr [ebp-8]
        // 007051a3
        pop         ecx
        // 007051a4
        fcomp       qword ptr [ebp+8]
        // 007051a7
        fnstsw      ax
        // 007051a9
        test        ah,44h
        // 007051ac
        jp L_007051dd
        // 007051ae
        fld         qword ptr [ebp+8]
        // 007051b1
        push        ecx
        // 007051b2
        fdiv        qword ptr [task8_pow_const_00741df8]
        // 007051b8
        push        ecx
        // 007051b9
        fstp        qword ptr [ebp-8]
        // 007051bc
        fld         qword ptr [ebp-8]
        // 007051bf
        fstp        qword ptr [esp]
        // 007051c2
        call task8_math_port_0070a53b
        // 007051c7
        fcomp       qword ptr [ebp-8]
        // 007051ca
        pop         ecx
        // 007051cb
        pop         ecx
        // 007051cc
        fnstsw      ax
        // 007051ce
        test        ah,44h
        // 007051d1
        jp L_007051d8
        // 007051d3
        push        2
        // 007051d5
        pop         eax
        // 007051d6
        leave
        // 007051d7
        ret
    L_007051d8:
        xor         eax,eax
        // 007051da
        inc         eax
        // 007051db
        leave
        // 007051dc
        ret
    L_007051dd:
        xor         eax,eax
        // 007051df
        leave
        // 007051e0
        ret
    }
}
