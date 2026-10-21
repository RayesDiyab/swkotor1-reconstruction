#include "ceced3efcb-status.h"
// Original instruction/control-flow body; borrowed ports remain explicit.
extern "C" __declspec(naked) void __cdecl task8_math_raise_flags_00707e49() {
    __asm {
        // 00707e49
        push        ecx
        // 00707e4a
        push        ecx
        // 00707e4b
        mov         cl,byte ptr [esp+0Ch]
        // 00707e4f
        test        cl,1
        // 00707e52
        je L_00707e5f
        // 00707e54
        fld         tbyte ptr [task8_status_const_007a33c0]
        // 00707e5a
        fistp       dword ptr [esp+0Ch]
        // 00707e5e
        wait
    L_00707e5f:
        test        cl,8
        // 00707e62
        je L_00707e75
        // 00707e64
        wait
        // 00707e65
        fnstsw      ax
        // 00707e67
        fld         tbyte ptr [task8_status_const_007a33c0]
        // 00707e6d
        fstp        qword ptr [esp]
        // 00707e71
        wait
        // 00707e72
        wait
        // 00707e73
        fnstsw      ax
    L_00707e75:
        test        cl,10h
        // 00707e78
        je L_00707e85
        // 00707e7a
        fld         tbyte ptr [task8_status_const_007a33cc]
        // 00707e80
        fstp        qword ptr [esp]
        // 00707e84
        wait
    L_00707e85:
        test        cl,4
        // 00707e88
        je L_00707e93
        // 00707e8a
        fldz
        // 00707e8c
        fld1
        // 00707e8e
        fdivrp      st(1),st
        // 00707e90
        fstp        st(0)
        // 00707e92
        wait
    L_00707e93:
        test        cl,20h
        // 00707e96
        je L_00707e9f
        // 00707e98
        fldpi
        // 00707e9a
        fstp        qword ptr [esp]
        // 00707e9e
        wait
    L_00707e9f:
        pop         ecx
        // 00707ea0
        pop         ecx
        // 00707ea1
        ret
    }
}
