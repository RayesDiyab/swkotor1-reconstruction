#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_x87_error_007096b2() {
    __asm {
        // 007096b2
        push        ebp
        // 007096b3
        mov         ebp,esp
        // 007096b5
        and         esp,0FFFFFFF0h
        // 007096b8
        sub         esp,84h
        // 007096be
        mov         eax,dword ptr [task8_crt_security_cookie_007a3180]
        // 007096c3
        xor         eax,dword ptr [ebp+4]
        // 007096c6
        push        ebx
        // 007096c7
        mov         dword ptr [esp+00000084h],eax
        // 007096ce
        mov         eax,dword ptr [ebp+10h]
        // 007096d1
        movzx       ecx,word ptr [eax]
        // 007096d4
        push        esi
        // 007096d5
        mov         esi,dword ptr [ebp+0Ch]
        // 007096d8
        mov         eax,dword ptr [esi]
        // 007096da
        dec         eax
        // 007096db
        push        edi
        // 007096dc
        mov         dword ptr [esp+0Ch],ecx
        // 007096e0
        je L_0070970d
        // 007096e2
        dec         eax
        // 007096e3
        je L_00709709
        // 007096e5
        dec         eax
        // 007096e6
        je L_00709705
        // 007096e8
        dec         eax
        // 007096e9
        je L_00709701
        // 007096eb
        dec         eax
        // 007096ec
        je L_0070970d
        // 007096ee
        dec         eax
        // 007096ef
        dec         eax
        // 007096f0
        je L_007096f9
        // 007096f2
        dec         eax
        // 007096f3
        jne L_00709769
        // 007096f5
        push        10h
        // 007096f7
        jmp L_0070970f
    L_007096f9:
        mov         dword ptr [esi],1
        // 007096ff
        jmp L_00709769
    L_00709701:
        push        12h
        // 00709703
        jmp L_0070970f
    L_00709705:
        push        11h
        // 00709707
        jmp L_0070970f
    L_00709709:
        push        4
        // 0070970b
        jmp L_0070970f
    L_0070970d:
        push        8
    L_0070970f:
        pop         ebx
        // 00709710
        push        ecx
        // 00709711
        lea         edi,[esi+18h]
        // 00709714
        push        edi
        // 00709715
        push        ebx
        // 00709716
        call task8_math_port_007079e4
        // 0070971b
        add         esp,0Ch
        // 0070971e
        test        eax,eax
        // 00709720
        jne L_00709769
        // 00709722
        mov         eax,dword ptr [ebp+8]
        // 00709725
        cmp         eax,10h
        // 00709728
        je L_0070973b
        // 0070972a
        cmp         eax,16h
        // 0070972d
        je L_0070973b
        // 0070972f
        cmp         eax,1Dh
        // 00709732
        je L_0070973b
        // 00709734
        and         dword ptr [esp+50h],0FFFFFFFEh
        // 00709739
        jmp L_00709750
    L_0070973b:
        mov         ecx,dword ptr [esp+50h]
        // 0070973f
        fld         qword ptr [esi+10h]
        // 00709742
        and         ecx,0FFFFFFE3h
        // 00709745
        fstp        qword ptr [esp+40h]
        // 00709749
        or          ecx,3
        // 0070974c
        mov         dword ptr [esp+50h],ecx
    L_00709750:
        push        edi
        // 00709751
        lea         ecx,[esi+8]
        // 00709754
        push        ecx
        // 00709755
        push        eax
        // 00709756
        push        ebx
        // 00709757
        lea         eax,[esp+1Ch]
        // 0070975b
        push        eax
        // 0070975c
        lea         eax,[esp+24h]
        // 00709760
        push        eax
        // 00709761
        call task8_math_port_0070773f
        // 00709766
        add         esp,18h
    L_00709769:
        push        0FFFFh
        // 0070976e
        push        dword ptr [esp+10h]
        // 00709772
        call task8_math_port_00707e25
        // 00709777
        cmp         dword ptr [esi],8
        // 0070977a
        pop         ecx
        // 0070977b
        pop         ecx
        // 0070977c
        je L_00709792
        // 0070977e
        cmp         dword ptr [task8_crt_matherr_user_007a36d0],0
        // 00709785
        jne L_00709792
        // 00709787
        push        esi
        // 00709788
        call task8_math_port_00706a22
        // 0070978d
        test        eax,eax
        // 0070978f
        pop         ecx
        // 00709790
        jne L_0070979a
    L_00709792:
        push        dword ptr [esi]
        // 00709794
        call task8_math_port_00707c06
        // 00709799
        pop         ecx
    L_0070979a:
        mov         ecx,dword ptr [esp+0000008Ch]
        // 007097a1
        xor         ecx,dword ptr [ebp+4]
        // 007097a4
        call task8_math_port_00705d37
        // 007097a9
        pop         edi
        // 007097aa
        pop         esi
        // 007097ab
        pop         ebx
        // 007097ac
        mov         esp,ebp
        // 007097ae
        pop         ebp
        // 007097af
        ret
    }
}
