#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_sse_error_007053de() {
    __asm {
        // 007053de
        push        ebp
        // 007053df
        mov         ebp,esp
        // 007053e1
        sub         esp,24h
        // 007053e4
        mov         eax,dword ptr [task8_crt_security_cookie_007a3180]
        // 007053e9
        xor         eax,dword ptr [ebp+4]
        // 007053ec
        mov         ecx,0A1h
        // 007053f1
        mov         dword ptr [ebp-4],eax
        // 007053f4
        mov         eax,dword ptr [ebp+14h]
        // 007053f7
        cmp         eax,ecx
        // 007053f9
        push        esi
        // 007053fa
        jg L_00705513
        // 00705400
        je L_00705507
        // 00705406
        cmp         eax,18h
        // 00705409
        jg L_00705497
        // 0070540f
        je L_00705484
        // 00705411
        push        2
        // 00705413
        pop         ecx
        // 00705414
        sub         eax,ecx
        // 00705416
        je L_00705475
        // 00705418
        dec         eax
        // 00705419
        je L_00705469
        // 0070541b
        sub         eax,5
        // 0070541e
        je L_0070545a
        // 00705420
        dec         eax
        // 00705421
        je L_0070544e
        // 00705423
        sub         eax,5
        // 00705426
        je L_0070543b
        // 00705428
        dec         eax
        // 00705429
        jne L_00705664
        // 0070542f
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A3Ch
        // 00705436
        jmp L_007054d8
    L_0070543b:
        mov         dword ptr [ebp-24h],3
        // 00705442
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A3Ch
        // 00705449
        jmp L_0070559a
    L_0070544e:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A34h
        // 00705455
        jmp L_00705601
    L_0070545a:
        mov         dword ptr [ebp-24h],ecx
        // 0070545d
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A34h
        // 00705464
        jmp L_0070559a
    L_00705469:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0074dc58
        // 00705470
        jmp L_00705601
    L_00705475:
        mov         dword ptr [ebp-24h],ecx
        // 00705478
        mov         dword ptr [ebp-20h],offset task8_pow_const_0074dc58
        // 0070547f
        jmp L_0070559a
    L_00705484:
        mov         dword ptr [ebp-24h],3
    L_0070548b:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A30h
        // 00705492
        jmp L_0070559a
    L_00705497:
        sub         eax,19h
        // 0070549a
        je L_007054d1
        // 0070549c
        dec         eax
        // 0070549d
        je L_007054c5
        // 0070549f
        dec         eax
        // 007054a0
        je L_007054bc
        // 007054a2
        dec         eax
        // 007054a3
        je L_007055fa
        // 007054a9
        dec         eax
        // 007054aa
        jne L_00705664
        // 007054b0
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A30h
        // 007054b7
        jmp L_00705622
    L_007054bc:
        mov         dword ptr [ebp-24h],2
        // 007054c3
        jmp L_0070548b
    L_007054c5:
        mov         eax,dword ptr [ebp+10h]
        // 007054c8
        fld1
        // 007054ca
        fstp        qword ptr [eax]
        // 007054cc
        jmp L_00705664
    L_007054d1:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A30h
    L_007054d8:
        mov         eax,dword ptr [ebp+8]
        // 007054db
        fld         qword ptr [eax]
        // 007054dd
        mov         eax,dword ptr [ebp+0Ch]
        // 007054e0
        mov         esi,dword ptr [ebp+10h]
        // 007054e3
        fstp        qword ptr [ebp-1Ch]
        // 007054e6
        fld         qword ptr [eax]
        // 007054e8
        lea         eax,[ebp-24h]
        // 007054eb
        fstp        qword ptr [ebp-14h]
        // 007054ee
        push        eax
        // 007054ef
        fld         qword ptr [esi]
        // 007054f1
        mov         dword ptr [ebp-24h],4
        // 007054f8
        fstp        qword ptr [ebp-0Ch]
        // 007054fb
        call        dword ptr [task8_crt_matherr_callback_007a316c]
        // 00705501
        pop         ecx
        // 00705502
        jmp L_0070565f
    L_00705507:
        mov         dword ptr [ebp-24h],3
        // 0070550e
        jmp L_00705593
    L_00705513:
        mov         ecx,3EAh
        // 00705518
        cmp         eax,ecx
        // 0070551a
        jg L_007055de
        // 00705520
        je L_007055d5
        // 00705526
        sub         eax,0A2h
        // 0070552b
        je L_0070558c
        // 0070552d
        sub         eax,4
        // 00705530
        je L_0070557c
        // 00705532
        sub         eax,4
        // 00705535
        je L_0070556c
        // 00705537
        dec         eax
        // 00705538
        je L_00705560
        // 0070553a
        sub         eax,33Dh
        // 0070553f
        je L_00705554
        // 00705541
        dec         eax
        // 00705542
        jne L_00705664
        // 00705548
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A34h
        // 0070554f
        jmp L_00705622
    L_00705554:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0074dc58
        // 0070555b
        jmp L_00705622
    L_00705560:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A28h
        // 00705567
        jmp L_00705601
    L_0070556c:
        mov         dword ptr [ebp-24h],2
        // 00705573
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A28h
        // 0070557a
        jmp L_0070559a
    L_0070557c:
        mov         dword ptr [ebp-24h],3
        // 00705583
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A20h
        // 0070558a
        jmp L_0070559a
    L_0070558c:
        mov         dword ptr [ebp-24h],4
    L_00705593:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A18h
    L_0070559a:
        mov         eax,dword ptr [ebp+8]
        // 0070559d
        fld         qword ptr [eax]
        // 0070559f
        mov         eax,dword ptr [ebp+0Ch]
        // 007055a2
        mov         esi,dword ptr [ebp+10h]
        // 007055a5
        fstp        qword ptr [ebp-1Ch]
        // 007055a8
        fld         qword ptr [eax]
        // 007055aa
        lea         eax,[ebp-24h]
        // 007055ad
        fstp        qword ptr [ebp-14h]
        // 007055b0
        push        eax
        // 007055b1
        fld         qword ptr [esi]
        // 007055b3
        fstp        qword ptr [ebp-0Ch]
        // 007055b6
        call        dword ptr [task8_crt_matherr_callback_007a316c]
        // 007055bc
        test        eax,eax
        // 007055be
        pop         ecx
        // 007055bf
        jne L_0070565f
        // 007055c5
        call task8_math_port_006ffe78
        // 007055ca
        mov         dword ptr [eax],22h
        // 007055d0
        jmp L_0070565f
    L_007055d5:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A3Ch
        // 007055dc
        jmp L_00705622
    L_007055de:
        sub         eax,3EBh
        // 007055e3
        je L_0070561b
        // 007055e5
        dec         eax
        // 007055e6
        je L_00705612
        // 007055e8
        dec         eax
        // 007055e9
        je L_00705609
        // 007055eb
        dec         eax
        // 007055ec
        je L_007055fa
        // 007055ee
        dec         eax
        // 007055ef
        jne L_00705664
        // 007055f1
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A10h
        // 007055f8
        jmp L_00705622
    L_007055fa:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A30h
    L_00705601:
        mov         eax,dword ptr [ebp+8]
        // 00705604
        mov         esi,dword ptr [ebp+10h]
        // 00705607
        jmp L_0070562c
    L_00705609:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A08h
        // 00705610
        jmp L_00705622
    L_00705612:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 03A00h
        // 00705619
        jmp L_00705622
    L_0070561b:
        mov         dword ptr [ebp-20h],offset task8_pow_const_0075a960 + 039F8h
    L_00705622:
        mov         eax,dword ptr [ebp+8]
        // 00705625
        fld         qword ptr [eax]
        // 00705627
        mov         esi,dword ptr [ebp+10h]
        // 0070562a
        fstp        qword ptr [esi]
    L_0070562c:
        fld         qword ptr [eax]
        // 0070562e
        mov         eax,dword ptr [ebp+0Ch]
        // 00705631
        fstp        qword ptr [ebp-1Ch]
        // 00705634
        mov         dword ptr [ebp-24h],1
        // 0070563b
        fld         qword ptr [eax]
        // 0070563d
        lea         eax,[ebp-24h]
        // 00705640
        fstp        qword ptr [ebp-14h]
        // 00705643
        push        eax
        // 00705644
        fld         qword ptr [esi]
        // 00705646
        fstp        qword ptr [ebp-0Ch]
        // 00705649
        call        dword ptr [task8_crt_matherr_callback_007a316c]
        // 0070564f
        test        eax,eax
        // 00705651
        pop         ecx
        // 00705652
        jne L_0070565f
        // 00705654
        call task8_math_port_006ffe78
        // 00705659
        mov         dword ptr [eax],21h
    L_0070565f:
        fld         qword ptr [ebp-0Ch]
        // 00705662
        fstp        qword ptr [esi]
    L_00705664:
        mov         ecx,dword ptr [ebp-4]
        // 00705667
        xor         ecx,dword ptr [ebp+4]
        // 0070566a
        pop         esi
        // 0070566b
        call task8_math_port_00705d37
        // 00705670
        leave
        // 00705671
        ret
    }
}
