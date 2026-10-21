#include "ceced3efcb-status.h"
// Original instruction/control-flow body; borrowed ports remain explicit.
extern "C" __declspec(naked) void __cdecl task8_math_port_007079e4() {
    __asm {
        // 007079e4
        push        ebp
        // 007079e5
        mov         ebp,esp
        // 007079e7
        sub         esp,10h
        // 007079ea
        mov         eax,dword ptr [ebp+8]
        // 007079ed
        push        ebx
        // 007079ee
        push        esi
        // 007079ef
        xor         ebx,ebx
        // 007079f1
        mov         esi,eax
        // 007079f3
        and         esi,1Fh
        // 007079f6
        inc         ebx
        // 007079f7
        test        al,8
        // 007079f9
        mov         dword ptr [ebp-4],esi
        // 007079fc
        je L_00707a12
        // 007079fe
        test        byte ptr [ebp+10h],bl
        // 00707a01
        je L_00707a12
        // 00707a03
        push        ebx
        // 00707a04
        call task8_math_raise_flags_00707e49
        // 00707a09
        pop         ecx
        // 00707a0a
        and         esi,0FFFFFFF7h
        // 00707a0d
        jmp L_00707be4
    L_00707a12:
        test        al,4
        // 00707a14
        je L_00707a2c
        // 00707a16
        test        byte ptr [ebp+10h],4
        // 00707a1a
        je L_00707a2c
        // 00707a1c
        push        4
        // 00707a1e
        call task8_math_raise_flags_00707e49
        // 00707a23
        pop         ecx
        // 00707a24
        and         esi,0FFFFFFFBh
        // 00707a27
        jmp L_00707be4
    L_00707a2c:
        test        al,bl
        // 00707a2e
        je L_00707b0c
        // 00707a34
        test        byte ptr [ebp+10h],8
        // 00707a38
        je L_00707b0c
        // 00707a3e
        push        8
        // 00707a40
        call task8_math_raise_flags_00707e49
        // 00707a45
        mov         eax,dword ptr [ebp+10h]
        // 00707a48
        pop         ecx
        // 00707a49
        mov         ecx,0C00h
        // 00707a4e
        and         eax,ecx
        // 00707a50
        je L_00707ae2
        // 00707a56
        cmp         eax,400h
        // 00707a5b
        je L_00707ab8
        // 00707a5d
        cmp         eax,800h
        // 00707a62
        je L_00707a8e
        // 00707a64
        cmp         eax,ecx
        // 00707a66
        jne L_00707b04
        // 00707a6c
        mov         ecx,dword ptr [ebp+0Ch]
        // 00707a6f
        fld         qword ptr [ecx]
        // 00707a71
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707a77
        fld         qword ptr [task8_pow_const_007a32b8 + 8]
        // 00707a7d
        fnstsw      ax
        // 00707a7f
        test        ah,41h
        // 00707a82
        je L_00707a86
        // 00707a84
        fchs
    L_00707a86:
        fstp        qword ptr [ebp-10h]
        // 00707a89
        fld         qword ptr [ebp-10h]
        // 00707a8c
        jmp L_00707b02
    L_00707a8e:
        mov         ecx,dword ptr [ebp+0Ch]
        // 00707a91
        fld         qword ptr [ecx]
        // 00707a93
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707a99
        fnstsw      ax
        // 00707a9b
        test        ah,41h
        // 00707a9e
        jne L_00707aa8
        // 00707aa0
        fld         qword ptr [task8_pow_const_007a32b0]
        // 00707aa6
        jmp L_00707ab0
    L_00707aa8:
        fld         qword ptr [task8_pow_const_007a32b8 + 8]
        // 00707aae
        fchs
    L_00707ab0:
        fstp        qword ptr [ebp-10h]
        // 00707ab3
        fld         qword ptr [ebp-10h]
        // 00707ab6
        jmp L_00707b02
    L_00707ab8:
        mov         ecx,dword ptr [ebp+0Ch]
        // 00707abb
        fld         qword ptr [ecx]
        // 00707abd
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707ac3
        fnstsw      ax
        // 00707ac5
        test        ah,41h
        // 00707ac8
        jne L_00707ad2
        // 00707aca
        fld         qword ptr [task8_pow_const_007a32b8 + 8]
        // 00707ad0
        jmp L_00707ada
    L_00707ad2:
        fld         qword ptr [task8_pow_const_007a32b0]
        // 00707ad8
        fchs
    L_00707ada:
        fstp        qword ptr [ebp-10h]
        // 00707add
        fld         qword ptr [ebp-10h]
        // 00707ae0
        jmp L_00707b02
    L_00707ae2:
        mov         ecx,dword ptr [ebp+0Ch]
        // 00707ae5
        fld         qword ptr [ecx]
        // 00707ae7
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707aed
        fld         qword ptr [task8_pow_const_007a32b0]
        // 00707af3
        fnstsw      ax
        // 00707af5
        test        ah,41h
        // 00707af8
        je L_00707afc
        // 00707afa
        fchs
    L_00707afc:
        fstp        qword ptr [ebp-10h]
        // 00707aff
        fld         qword ptr [ebp-10h]
    L_00707b02:
        fstp        qword ptr [ecx]
    L_00707b04:
        and         esi,0FFFFFFFEh
        // 00707b07
        jmp L_00707be4
    L_00707b0c:
        test        al,2
        // 00707b0e
        je L_00707be4
        // 00707b14
        test        byte ptr [ebp+10h],10h
        // 00707b18
        je L_00707be4
        // 00707b1e
        xor         esi,esi
        // 00707b20
        test        al,10h
        // 00707b22
        je L_00707b26
        // 00707b24
        mov         esi,ebx
    L_00707b26:
        push        edi
        // 00707b27
        mov         edi,dword ptr [ebp+0Ch]
        // 00707b2a
        fld         qword ptr [edi]
        // 00707b2c
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707b32
        fnstsw      ax
        // 00707b34
        test        ah,44h
        // 00707b37
        jnp L_00707bce
        // 00707b3d
        fld         qword ptr [edi]
        // 00707b3f
        lea         eax,[ebp-8]
        // 00707b42
        push        eax
        // 00707b43
        push        ecx
        // 00707b44
        push        ecx
        // 00707b45
        fstp        qword ptr [esp]
        // 00707b48
        call task8_math_decompose_00707685
        // 00707b4d
        mov         ecx,dword ptr [ebp-8]
        // 00707b50
        fstp        qword ptr [ebp-10h]
        // 00707b53
        fld         qword ptr [ebp-10h]
        // 00707b56
        add         ecx,0FFFFFA00h
        // 00707b5c
        add         esp,0Ch
        // 00707b5f
        cmp         ecx,0FFFFFBCEh
        // 00707b65
        jge L_00707b71
        // 00707b67
        fmul        qword ptr [task8_pow_const_0073edf8]
        // 00707b6d
        mov         esi,ebx
        // 00707b6f
        jmp L_00707bc4
    L_00707b71:
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00707b77
        fnstsw      ax
        // 00707b79
        test        ah,5
        // 00707b7c
        jp L_00707b82
        // 00707b7e
        mov         edx,ebx
        // 00707b80
        jmp L_00707b84
    L_00707b82:
        xor         edx,edx
    L_00707b84:
        xor         eax,eax
        // 00707b86
        mov         al,byte ptr [ebp-0Ah]
        // 00707b89
        and         eax,0Fh
        // 00707b8c
        or          eax,10h
        // 00707b8f
        mov         word ptr [ebp-0Ah],ax
        // 00707b93
        mov         eax,0FFFFFC03h
        // 00707b98
        cmp         ecx,eax
        // 00707b9a
        jge L_00707bbb
        // 00707b9c
        sub         eax,ecx
    L_00707b9e:
        test        byte ptr [ebp-10h],bl
        // 00707ba1
        je L_00707ba9
        // 00707ba3
        test        esi,esi
        // 00707ba5
        jne L_00707ba9
        // 00707ba7
        mov         esi,ebx
    L_00707ba9:
        shr         dword ptr [ebp-10h],1
        // 00707bac
        test        byte ptr [ebp-0Ch],bl
        // 00707baf
        je L_00707bb5
        // 00707bb1
        or          byte ptr [ebp-0Dh],80h
    L_00707bb5:
        shr         dword ptr [ebp-0Ch],1
        // 00707bb8
        dec         eax
        // 00707bb9
        jne L_00707b9e
    L_00707bbb:
        test        edx,edx
        // 00707bbd
        je L_00707bc7
        // 00707bbf
        fld         qword ptr [ebp-10h]
        // 00707bc2
        fchs
    L_00707bc4:
        fstp        qword ptr [ebp-10h]
    L_00707bc7:
        fld         qword ptr [ebp-10h]
        // 00707bca
        fstp        qword ptr [edi]
        // 00707bcc
        jmp L_00707bd0
    L_00707bce:
        mov         esi,ebx
    L_00707bd0:
        test        esi,esi
        // 00707bd2
        pop         edi
        // 00707bd3
        je L_00707bdd
        // 00707bd5
        push        10h
        // 00707bd7
        call task8_math_raise_flags_00707e49
        // 00707bdc
        pop         ecx
    L_00707bdd:
        and         dword ptr [ebp-4],0FFFFFFFDh
        // 00707be1
        mov         esi,dword ptr [ebp-4]
    L_00707be4:
        test        byte ptr [ebp+8],10h
        // 00707be8
        je L_00707bfb
        // 00707bea
        test        byte ptr [ebp+10h],20h
        // 00707bee
        je L_00707bfb
        // 00707bf0
        push        20h
        // 00707bf2
        call task8_math_raise_flags_00707e49
        // 00707bf7
        pop         ecx
        // 00707bf8
        and         esi,0FFFFFFEFh
    L_00707bfb:
        xor         eax,eax
        // 00707bfd
        test        esi,esi
        // 00707bff
        pop         esi
        // 00707c00
        sete        al
        // 00707c03
        pop         ebx
        // 00707c04
        leave
        // 00707c05
        ret
    }
}

