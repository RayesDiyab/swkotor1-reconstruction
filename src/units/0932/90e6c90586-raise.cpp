#include "219cea7e34-helpers.h"
// One four-byte alias to the installed WINAPI import; no replacement provider.
#pragma comment(linker, "/alternatename:_task8_raiseexception_iat=__imp__RaiseException@16")
// Original precision/control bit-mapping quirks and per-field writes preserved.
extern "C" __declspec(naked) void __cdecl task8_math_port_0070773f(void) {
    __asm {
        push        ebp
        mov         ebp,esp
        mov         ecx,dword ptr [ebp+8]
        xor         eax,eax
        mov         dword ptr [ecx+4],eax
        mov         ecx,dword ptr [ebp+8]
        mov         dword ptr [ecx+8],eax
        mov         ecx,dword ptr [ebp+8]
        push        ebx
        xor         ebx,ebx
        mov         dword ptr [ecx+0Ch],eax
        mov         cl,byte ptr [ebp+10h]
        push        esi
        inc         ebx
        test        cl,10h
        push        edi
        je L_00707771
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+4],ebx
        mov         dword ptr [ebp+10h],0C000008Fh
    L_00707771:
        test        cl,2
        je L_00707784
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+4],2
        mov         dword ptr [ebp+10h],0C0000093h
    L_00707784:
        _emit 084h
        _emit 0CBh
        je L_00707796
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+4],4
        mov         dword ptr [ebp+10h],0C0000091h
    L_00707796:
        test        cl,4
        je L_007077A9
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+4],8
        mov         dword ptr [ebp+10h],0C000008Eh
    L_007077A9:
        test        cl,8
        je L_007077BC
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+4],10h
        mov         dword ptr [ebp+10h],0C0000090h
    L_007077BC:
        mov         esi,dword ptr [ebp+0Ch]
        mov         ecx,dword ptr [esi]
        mov         eax,dword ptr [ebp+8]
        shl         ecx,4
        not         ecx
        xor         ecx,dword ptr [eax+8]
        push        2
        and         ecx,10h
        xor         dword ptr [eax+8],ecx
        mov         ecx,dword ptr [esi]
        mov         eax,dword ptr [ebp+8]
        shl         ecx,1
        not         ecx
        xor         ecx,dword ptr [eax+8]
        pop         edi
        and         ecx,8
        xor         dword ptr [eax+8],ecx
        mov         ecx,dword ptr [esi]
        mov         eax,dword ptr [ebp+8]
        shr         ecx,1
        not         ecx
        xor         ecx,dword ptr [eax+8]
        and         ecx,4
        xor         dword ptr [eax+8],ecx
        mov         ecx,dword ptr [esi]
        mov         eax,dword ptr [ebp+8]
        shr         ecx,3
        not         ecx
        xor         ecx,dword ptr [eax+8]
        and         ecx,edi
        xor         dword ptr [eax+8],ecx
        mov         ecx,dword ptr [esi]
        mov         eax,dword ptr [ebp+8]
        shr         ecx,5
        not         ecx
        xor         ecx,dword ptr [eax+8]
        and         ecx,ebx
        xor         dword ptr [eax+8],ecx
        call task8_math_port_00707e0a
        // TEST BL,AL exact original commutative encoding.
        _emit 084h
        _emit 0C3h
        je L_0070782D
        mov         ecx,dword ptr [ebp+8]
        or          dword ptr [ecx+0Ch],10h
    L_0070782D:
        test        al,4
        je L_00707838
        mov         ecx,dword ptr [ebp+8]
        or          dword ptr [ecx+0Ch],8
    L_00707838:
        test        al,8
        je L_00707843
        mov         ecx,dword ptr [ebp+8]
        or          dword ptr [ecx+0Ch],4
    L_00707843:
        test        al,10h
        je L_0070784D
        mov         ecx,dword ptr [ebp+8]
        or          dword ptr [ecx+0Ch],edi
    L_0070784D:
        test        al,20h
        je L_00707857
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+0Ch],ebx
    L_00707857:
        mov         eax,dword ptr [esi]
        mov         ecx,0C00h
        and         eax,ecx
        je L_00707896
        cmp         eax,400h
        je L_00707888
        cmp         eax,800h
        je L_0070787C
        cmp         eax,ecx
        jne L_0070789C
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax],3
        jmp L_0070789C
    L_0070787C:
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax]
        and         ecx,0FFFFFFFEh
        or          ecx,edi
        jmp L_00707892
    L_00707888:
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax]
        and         ecx,0FFFFFFFDh
        or          ecx,ebx
    L_00707892:
        mov         dword ptr [eax],ecx
        jmp L_0070789C
    L_00707896:
        mov         eax,dword ptr [ebp+8]
        and         dword ptr [eax],0FFFFFFFCh
    L_0070789C:
        mov         eax,dword ptr [esi]
        mov         ecx,300h
        and         eax,ecx
        je L_007078C7
        cmp         eax,200h
        je L_007078BA
        cmp         eax,ecx
        jne L_007078D4
        mov         eax,dword ptr [ebp+8]
        and         dword ptr [eax],0FFFFFFE3h
        jmp L_007078D4
    L_007078BA:
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax]
        and         ecx,0FFFFFFE7h
        or          ecx,4
        jmp L_007078D2
    L_007078C7:
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax]
        and         ecx,0FFFFFFEBh
        or          ecx,8
    L_007078D2:
        mov         dword ptr [eax],ecx
    L_007078D4:
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [ebp+14h]
        shl         ecx,5
        xor         ecx,dword ptr [eax]
        and         ecx,1FFE0h
        xor         dword ptr [eax],ecx
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+20h],ebx
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax+20h]
        and         ecx,0FFFFFFE3h
        or          ecx,edi
        mov         dword ptr [eax+20h],ecx
        mov         eax,dword ptr [ebp+18h]
        fld         qword ptr [eax]
        mov         eax,dword ptr [ebp+8]
        fstp        qword ptr [eax+10h]
        mov         eax,dword ptr [ebp+8]
        or          dword ptr [eax+60h],ebx
        mov         eax,dword ptr [ebp+8]
        mov         ecx,dword ptr [eax+60h]
        and         ecx,0FFFFFFE3h
        or          ecx,edi
        mov         edi,dword ptr [ebp+1Ch]
        mov         dword ptr [eax+60h],ecx
        fld         qword ptr [edi]
        mov         eax,dword ptr [ebp+8]
        fstp        qword ptr [eax+50h]
        call task8_math_port_00707e17
        lea         eax,[ebp+8]
        push        eax
        push        ebx
        push        0
        push        dword ptr [ebp+10h]
        call dword ptr [task8_raiseexception_iat]
        mov         eax,dword ptr [ebp+8]
        test        byte ptr [eax+8],10h
        je L_00707946
        and         dword ptr [esi],0FFFFFFFEh
    L_00707946:
        test        byte ptr [eax+8],8
        je L_0070794F
        and         dword ptr [esi],0FFFFFFFBh
    L_0070794F:
        test        byte ptr [eax+8],4
        je L_00707958
        and         dword ptr [esi],0FFFFFFF7h
    L_00707958:
        test        byte ptr [eax+8],2
        je L_00707961
        and         dword ptr [esi],0FFFFFFEFh
    L_00707961:
        test        byte ptr [eax+8],bl
        je L_00707969
        and         dword ptr [esi],0FFFFFFDFh
    L_00707969:
        mov         ecx,dword ptr [eax]
        and         ecx,3
        sub         ecx,0
        mov         edx,0FFFFF3FFh
        je L_007079A9
        dec         ecx
        je L_00707997
        dec         ecx
        je L_00707987
        dec         ecx
        jne L_007079AB
        or          byte ptr [esi+1],0Ch
        jmp L_007079AB
    L_00707987:
        mov         ecx,dword ptr [esi]
        and         ecx,0FFFFFBFFh
        or          ecx,800h
        jmp L_007079A5
    L_00707997:
        mov         ecx,dword ptr [esi]
        and         ecx,0FFFFF7FFh
        or          ecx,400h
    L_007079A5:
        mov         dword ptr [esi],ecx
        jmp L_007079AB
    L_007079A9:
        and         dword ptr [esi],edx
    L_007079AB:
        mov         ecx,dword ptr [eax]
        shr         ecx,2
        and         ecx,7
        sub         ecx,0
        je L_007079CE
        dec         ecx
        je L_007079C2
        dec         ecx
        jne L_007079DA
        and         dword ptr [esi],edx
        jmp L_007079DA
    L_007079C2:
        mov         ecx,dword ptr [esi]
        and         ecx,edx
        or          ecx,200h
        jmp L_007079D8
    L_007079CE:
        mov         ecx,dword ptr [esi]
        and         ecx,edx
        or          ecx,300h
    L_007079D8:
        mov         dword ptr [esi],ecx
    L_007079DA:
        fld         qword ptr [eax+50h]
        fstp        qword ptr [edi]
        pop         edi
        pop         esi
        pop         ebx
        pop         ebp
        ret
    }
}
