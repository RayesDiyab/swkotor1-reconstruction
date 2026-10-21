#include "ceced3efcb-status.h"
// Original instruction/control-flow body; borrowed ports remain explicit.
extern "C" __declspec(naked) void __cdecl task8_math_decompose_00707685() {
    __asm {
        // 00707685
        push        ebp
        // 00707686
        mov         ebp,esp
        // 00707688
        push        ecx
        // 00707689
        push        ecx
        // 0070768a
        fld         qword ptr [ebp+8]
        // 0070768d
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707693
        fnstsw      ax
        // 00707695
        test        ah,44h
        // 00707698
        jp L_007076a3
        // 0070769a
        fldz
        // 0070769c
        xor         edx,edx
        // 0070769e
        jmp L_00707732
    L_007076a3:
        xor         ecx,ecx
        // 007076a5
        test        word ptr [ebp+0Eh],7FF0h
        // 007076ab
        jne L_0070770f
        // 007076ad
        test        dword ptr [ebp+0Ch],0FFFFFh
        // 007076b4
        jne L_007076bb
        // 007076b6
        cmp         dword ptr [ebp+8],ecx
        // 007076b9
        je L_0070770f
    L_007076bb:
        fld         qword ptr [ebp+8]
        // 007076be
        mov         edx,0FFFFFC03h
        // 007076c3
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 007076c9
        fnstsw      ax
        // 007076cb
        test        ah,5
        // 007076ce
        jp L_007076d5
        // 007076d0
        xor         eax,eax
        // 007076d2
        inc         eax
        // 007076d3
        jmp L_007076ea
    L_007076d5:
        xor         eax,eax
        // 007076d7
        jmp L_007076ea
    L_007076d9:
        shl         dword ptr [ebp+0Ch],1
        // 007076dc
        test        byte ptr [ebp+0Bh],80h
        // 007076e0
        je L_007076e6
        // 007076e2
        or          dword ptr [ebp+0Ch],1
    L_007076e6:
        shl         dword ptr [ebp+8],1
        // 007076e9
        dec         edx
    L_007076ea:
        test        byte ptr [ebp+0Eh],10h
        // 007076ee
        je L_007076d9
        // 007076f0
        and         byte ptr [ebp+0Eh],0EFh
        // 007076f4
        cmp         eax,ecx
        // 007076f6
        je L_007076fc
        // 007076f8
        or          byte ptr [ebp+0Fh],80h
    L_007076fc:
        fld         qword ptr [ebp+8]
        // 007076ff
        push        ecx
        // 00707700
        push        ecx
        // 00707701
        push        ecx
        // 00707702
        fstp        qword ptr [esp]
        // 00707705
        call task8_math_port_00707600
        // 0070770a
        add         esp,0Ch
        // 0070770d
        jmp L_00707732
    L_0070770f:
        fld         qword ptr [ebp+8]
        // 00707712
        push        ecx
        // 00707713
        push        ecx
        // 00707714
        push        ecx
        // 00707715
        fstp        qword ptr [esp]
        // 00707718
        call task8_math_port_00707600
        // 0070771d
        mov         edx,dword ptr [ebp+0Eh]
        // 00707720
        shr         edx,4
        // 00707723
        and         edx,7FFh
        // 00707729
        add         esp,0Ch
        // 0070772c
        sub         edx,3FEh
    L_00707732:
        mov         eax,dword ptr [ebp+10h]
        // 00707735
        fstp        qword ptr [ebp-8]
        // 00707738
        fld         qword ptr [ebp-8]
        // 0070773b
        mov         dword ptr [eax],edx
        // 0070773d
        leave
        // 0070773e
        ret
    }
}
