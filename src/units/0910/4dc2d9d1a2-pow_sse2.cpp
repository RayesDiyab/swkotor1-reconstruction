#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_pow_sse2_internal_00704439() {
    __asm {
        // 00704439
        movlpd      xmm0,qword ptr [esp+4]
        // 0070443f
        movlpd      xmm7,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 00704447
        movlpd      xmm2,qword ptr [task8_pow_const_0075a960 + 03900h]
        // 0070444f
        andpd       xmm7,xmm0
        // 00704453
        movsd       xmm4,xmm0
        // 00704457
        psrlq       xmm0,2Ch
        // 0070445c
        pextrw      eax,xmm0,0
        // 00704461
        orpd        xmm7,xmm2
        // 00704465
        pextrw      ecx,xmm4,3
        // 0070446a
        and         eax,0FFh
        // 0070446f
        add         eax,1
        // 00704472
        and         eax,1FEh
        // 00704477
        mulsd       xmm7,qword ptr [eax*4+task8_pow_const_0075a960 + 060h]
        // 00704480
        movlpd      xmm5,qword ptr [eax*4+task8_pow_const_0075a960 + 060h]
        // 00704489
        add         eax,eax
        // 0070448b
        movapd      xmm6, [eax*4+task8_pow_const_0075a960 + 0470h]
        // 00704494
        mov         edx,7FEFh
        // 00704499
        sub         edx,ecx
        // 0070449b
        sub         ecx,10h
        // 0070449e
        or          ecx,edx
        // 007044a0
        cmp         ecx,80000000h
        // 007044a6
        jae L_00704787
        // 007044ac
        mov         ecx,0
        // 007044b1
        mov         edx,3FE7Fh
    L_007044b6:
        movd        xmm1,edx
        // 007044ba
        psubq       xmm0,xmm1
        // 007044be
        psrlq       xmm0,8
        // 007044c3
        cvtdq2pd    xmm0,xmm0
        // 007044c7
        movlpd      xmm1,qword ptr [task8_pow_const_0075a960 + 03948h]
        // 007044cf
        movsd       xmm3,xmm7
        // 007044d3
        psrlq       xmm7,26h
        // 007044d8
        pextrw      eax,xmm7,0
        // 007044dd
        andpd       xmm4, [task8_pow_const_0075a960 + 038F0h]
        // 007044e5
        and         eax,0FFh
        // 007044ea
        add         eax,1
        // 007044ed
        and         eax,1FEh
        // 007044f2
        mulsd       xmm3,qword ptr [eax*4+task8_pow_const_0075a960 + 0C80h]
        // 007044fb
        mulsd       xmm5,qword ptr [eax*4+task8_pow_const_0075a960 + 0C80h]
        // 00704504
        add         eax,eax
        // 00704506
        addpd       xmm6, [eax*4+task8_pow_const_0075a960 + 01090h]
        // 0070450f
        orpd        xmm4, [task8_pow_const_0075a960 + 03900h]
        // 00704517
        addsd       xmm6,xmm0
        // 0070451b
        andpd       xmm1,xmm4
        // 0070451f
        movsd       xmm2,xmm3
        // 00704523
        psrlq       xmm3,1Fh
        // 00704528
        pextrw      eax,xmm3,0
        // 0070452d
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03948h]
        // 00704535
        subsd       xmm4,xmm1
        // 00704539
        movlpd      xmm7,qword ptr [task8_pow_const_0075a960 + 03950h]
        // 00704541
        and         eax,1FFh
        // 00704546
        add         eax,1
        // 00704549
        and         eax,3FEh
        // 0070454e
        mulsd       xmm5,qword ptr [eax*4+task8_pow_const_0075a960 + 018A0h]
        // 00704557
        mulsd       xmm2,qword ptr [eax*4+task8_pow_const_0075a960 + 018A0h]
        // 00704560
        addpd       xmm6, [eax*8+task8_pow_const_0075a960 + 020B0h]
        // 00704569
        andpd       xmm0,xmm5
        // 0070456d
        subsd       xmm5,xmm0
        // 00704571
        addsd       xmm7,xmm2
        // 00704575
        movsd       xmm3,xmm0
        // 00704579
        mulsd       xmm0,xmm1
        // 0070457d
        mulsd       xmm1,xmm5
        // 00704581
        mulsd       xmm3,xmm4
        // 00704585
        subsd       xmm2,xmm0
        // 00704589
        mulsd       xmm4,xmm5
        // 0070458d
        movsd       xmm0,xmm6
        // 00704591
        subsd       xmm2,xmm1
        // 00704595
        addsd       xmm6,xmm7
        // 00704599
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 0070459f
        pextrw      eax,xmm1,3
        // 007045a4
        subsd       xmm2,xmm3
        // 007045a8
        subsd       xmm0,xmm6
        // 007045ac
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 03948h]
        // 007045b4
        pextrw      edx,xmm6,3
        // 007045b9
        subsd       xmm2,xmm4
        // 007045bd
        movsd       xmm4,xmm6
        // 007045c1
        addsd       xmm0,xmm7
        // 007045c5
        subsd       xmm7,xmm2
        // 007045c9
        subsd       xmm6,xmm2
        // 007045cd
        unpcklpd    xmm7,xmm7
        // 007045d1
        and         eax,7FF0h
        // 007045d6
        cmp         eax,7FF0h
        // 007045db
        jae L_00704af8
        // 007045e1
        and         edx,7FF0h
        // 007045e7
        sub         eax,3FF0h
        // 007045ec
        add         eax,edx
        // 007045ee
        mov         edx,40A0h
        // 007045f3
        sub         edx,eax
        // 007045f5
        sub         eax,3C70h
        // 007045fa
        or          edx,eax
        // 007045fc
        cmp         edx,80000000h
        // 00704602
        jae L_00704ee1
    L_00704608:
        subsd       xmm4,xmm6
        // 0070460c
        subsd       xmm2,xmm4
        // 00704610
        movlpd      xmm4,qword ptr [task8_pow_const_0075a960 + 03948h]
        // 00704618
        andpd       xmm3,xmm1
        // 0070461c
        andpd       xmm4,xmm6
        // 00704620
        subsd       xmm0,xmm2
        // 00704624
        xorpd       xmm2,xmm2
        // 00704628
        mov         edx,4060h
        // 0070462d
        pinsrw      xmm2,edx,3
        // 00704632
        movsd       xmm5,xmm3
        // 00704636
        mulsd       xmm3,xmm4
        // 0070463a
        subsd       xmm6,xmm4
        // 0070463e
        subsd       xmm1,xmm5
        // 00704642
        mulsd       xmm3,xmm2
        // 00704646
        mulsd       xmm5,xmm6
        // 0070464a
        movapd      xmm2, [task8_pow_const_0075a960 + 030C0h]
        // 00704652
        mulsd       xmm4,xmm1
        // 00704656
        cvtsd2si    eax,xmm3
        // 0070465a
        mulsd       xmm6,xmm1
        // 0070465e
        movapd      xmm1, [task8_pow_const_0075a960 + 030D0h]
        // 00704666
        addsd       xmm5,xmm4
        // 0070466a
        pshufd      xmm4,xmm6,0EEh
        // 0070466f
        addsd       xmm5,xmm6
        // 00704673
        mov         edx,1FF7Fh
        // 00704678
        sub         edx,eax
        // 0070467a
        add         eax,1E1FFh
        // 0070467f
        or          edx,eax
        // 00704681
        sub         eax,1E1FFh
        // 00704686
        cmp         edx,0
        // 00704689
        jle L_00704c71
        // 0070468f
        add         ecx,eax
        // 00704691
        and         eax,7Fh
        // 00704694
        and         ecx,0FFFFFF80h
        // 00704697
        add         ecx,1FF80h
        // 0070469d
        addsd       xmm4,xmm0
        // 007046a1
        movsd       xmm0,xmm3
        // 007046a5
        addsd       xmm3,qword ptr [task8_pow_const_0075a960 + 038E0h]
        // 007046ad
        mulpd       xmm2,xmm7
        // 007046b1
        subsd       xmm3,qword ptr [task8_pow_const_0075a960 + 038E0h]
        // 007046b9
        mulpd       xmm7,xmm7
        // 007046bd
        subsd       xmm0,xmm3
        // 007046c1
        add         eax,eax
        // 007046c3
        add         eax,eax
        // 007046c5
        add         eax,eax
        // 007046c7
        add         eax,eax
        // 007046c9
        movapd      xmm3, [eax+task8_pow_const_0075a960 + 030E0h]
        // 007046d1
        movapd      xmm6, [task8_pow_const_0075a960 + 03920h]
        // 007046d9
        mulpd       xmm1,xmm7
        // 007046dd
        addpd       xmm2,xmm1
        // 007046e1
        pshufd      xmm1,xmm2,0EEh
        // 007046e6
        mulsd       xmm2,xmm7
        // 007046ea
        xorpd       xmm7,xmm7
        // 007046ee
        mov         edx,3F80h
        // 007046f3
        addsd       xmm2,xmm1
        // 007046f7
        pinsrw      xmm7,edx,3
        // 007046fc
        addsd       xmm2,xmm4
        // 00704700
        movlpd      xmm4,qword ptr [task8_pow_const_0075a960 + 03940h]
        // 00704708
        movd        xmm1,ecx
        // 0070470c
        mulsd       xmm2,qword ptr [esp+0Ch]
        // 00704712
        mulsd       xmm0,xmm7
        // 00704716
        psllq       xmm1,2Dh
        // 0070471b
        pshufd      xmm1,xmm1,44h
        // 00704720
        movapd      xmm7, [task8_pow_const_0075a960 + 03930h]
        // 00704728
        addsd       xmm5,xmm2
        // 0070472c
        mulpd       xmm3,xmm1
        // 00704730
        addsd       xmm0,xmm5
        // 00704734
        unpcklpd    xmm0,xmm0
        // 00704738
        mulpd       xmm6,xmm0
        // 0070473c
        mulsd       xmm4,xmm0
        // 00704740
        mulpd       xmm0,xmm0
        // 00704744
        addpd       xmm7,xmm6
        // 00704748
        mulpd       xmm7,xmm0
        // 0070474c
        mulsd       xmm0,xmm3
        // 00704750
        pshufd      xmm6,xmm7,0EEh
        // 00704755
        mulsd       xmm0,xmm7
        // 00704759
        pshufd      xmm5,xmm3,0EEh
        // 0070475e
        mulsd       xmm6,xmm3
        // 00704762
        mulsd       xmm4,xmm3
        // 00704766
        addsd       xmm0,xmm5
        // 0070476a
        sub         esp,10h
        // 0070476d
        addsd       xmm0,xmm6
        // 00704771
        addsd       xmm0,xmm4
        // 00704775
        addsd       xmm0,xmm3
        // 00704779
        movlpd      qword ptr [esp+4],xmm0
        // 0070477f
        fld         qword ptr [esp+4]
        // 00704783
        add         esp,10h
        // 00704786
        ret
    L_00704787:
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 0070478d
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 03910h]
        // 00704795
        movd        eax,xmm1
        // 00704799
        movsd       xmm2,xmm1
        // 0070479d
        andpd       xmm1,xmm3
        // 007047a1
        psrlq       xmm1,20h
        // 007047a6
        movd        ecx,xmm1
        // 007047aa
        cmp         ecx,7FF00000h
        // 007047b0
        jae L_00704890
        // 007047b6
        or          eax,ecx
        // 007047b8
        cmp         eax,0
        // 007047bb
        je L_00704bd4
        // 007047c1
        cmp         edx,0
        // 007047c4
        jge L_007048cf
        // 007047ca
        neg         edx
        // 007047cc
        add         edx,7FEFh
        // 007047d2
        psllq       xmm3,34h
        // 007047d7
        orpd        xmm2,xmm3
        // 007047db
        mov         ecx,3F3h
        // 007047e0
        movd        xmm3,ecx
        // 007047e4
        psrlq       xmm1,14h
        // 007047e9
        psubd       xmm1,xmm3
        // 007047ed
        pxor        xmm3,xmm3
        // 007047f1
        pmaxsw      xmm1,xmm3
        // 007047f5
        psllq       xmm2,xmm1
        // 007047f9
        pcmpeqd     xmm2,xmm3
        // 007047fd
        pmovmskb    eax,xmm2
        // 00704801
        mov         ecx,edx
        // 00704803
        and         edx,7FFFh
        // 00704809
        cmp         edx,7FF0h
        // 0070480f
        jae L_007049b8
        // 00704815
        and         eax,0FFh
        // 0070481a
        cmp         eax,0FFh
        // 0070481f
        jne L_00704ab1
        // 00704825
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 0070482b
        movlpd      xmm2,qword ptr [esp+0Ch]
        // 00704831
        mov         ecx,3F4h
        // 00704836
        movd        xmm3,ecx
        // 0070483a
        andpd       xmm1, [task8_pow_const_0075a960 + 03910h]
        // 00704842
        psrlq       xmm1,34h
        // 00704847
        psubd       xmm1,xmm3
        // 0070484b
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 03970h]
        // 00704853
        psllq       xmm2,xmm1
        // 00704857
        pcmpeqd     xmm2,xmm3
        // 0070485b
        pmovmskb    eax,xmm2
        // 0070485f
        and         eax,0FFh
        // 00704864
        mov         ecx,3FF01h
        // 00704869
        add         ecx,eax
        // 0070486b
        and         ecx,40000h
        // 00704871
        cmp         edx,10h
        // 00704874
        jb L_007048d4
        // 00704876
        mov         edx,0BFE7Fh
        // 0070487b
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 00704883
        movlpd      xmm2,qword ptr [task8_pow_const_0075a960 + 03900h]
        // 0070488b
        jmp L_007044b6
    L_00704890:
        movlpd      xmm7,qword ptr [esp+4]
        // 00704896
        movlpd      xmm4,qword ptr [esp+4]
        // 0070489c
        movd        edx,xmm7
        // 007048a0
        psrlq       xmm7,20h
        // 007048a5
        movd        eax,xmm7
        // 007048a9
        mov         ecx,eax
        // 007048ab
        and         eax,7FFFFFFFh
        // 007048b0
        cmp         eax,7FF00000h
        // 007048b5
        jb L_00704b4c
        // 007048bb
        ja L_00704a9f
        // 007048c1
        cmp         edx,0
        // 007048c4
        ja L_00704a9f
        // 007048ca
        jmp L_00704b4c
    L_007048cf:
        mov         ecx,0
    L_007048d4:
        xorpd       xmm0,xmm0
        // 007048d8
        mov         eax,43F0h
        // 007048dd
        pinsrw      xmm0,eax,3
        // 007048e2
        movlpd      xmm7,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 007048ea
        movlpd      xmm2,qword ptr [task8_pow_const_0075a960 + 03900h]
        // 007048f2
        mulsd       xmm0,xmm4
        // 007048f6
        movd        edx,xmm4
        // 007048fa
        psrlq       xmm4,20h
        // 007048ff
        movd        eax,xmm4
        // 00704903
        cmp         edx,0
        // 00704906
        je L_0070495a
    L_00704908:
        andpd       xmm7,xmm0
        // 0070490c
        movsd       xmm4,xmm0
        // 00704910
        andpd       xmm0, [task8_pow_const_0075a960 + 03910h]
        // 00704918
        psrlq       xmm0,2Ch
        // 0070491d
        pextrw      eax,xmm0,0
        // 00704922
        orpd        xmm7,xmm2
        // 00704926
        and         eax,0FFh
        // 0070492b
        add         eax,1
        // 0070492e
        and         eax,1FEh
        // 00704933
        mulsd       xmm7,qword ptr [eax*4+task8_pow_const_0075a960 + 060h]
        // 0070493c
        movlpd      xmm5,qword ptr [eax*4+task8_pow_const_0075a960 + 060h]
        // 00704945
        add         eax,eax
        // 00704947
        movapd      xmm6, [eax*4+task8_pow_const_0075a960 + 0470h]
        // 00704950
        mov         edx,43E7Fh
        // 00704955
        jmp L_007044b6
    L_0070495a:
        mov         edx,eax
        // 0070495c
        and         edx,7FFFFFFFh
        // 00704962
        cmp         edx,0
        // 00704965
        jne L_00704908
        // 00704967
        mov         edx,dword ptr [esp+10h]
        // 0070496b
        and         edx,80000000h
        // 00704971
        cmp         edx,0
        // 00704974
        je L_007049a7
        // 00704976
        shl         ecx,0Dh
        // 00704979
        and         eax,ecx
        // 0070497b
        mov         edx,7FF00000h
        // 00704980
        or          edx,eax
        // 00704982
        movd        xmm0,edx
        // 00704986
        psllq       xmm0,20h
        // 0070498b
        movlpd      xmm1,qword ptr [task8_pow_const_0075a960 + 03900h]
        // 00704993
        movlpd      xmm2,qword ptr [esp+4]
        // 00704999
        divsd       xmm1,xmm2
        // 0070499d
        mov         edx,1Bh
        // 007049a2
        jmp L_00704c41
    L_007049a7:
        shl         ecx,0Dh
        // 007049aa
        and         eax,ecx
        // 007049ac
        cmp         eax,0
        // 007049af
        jne L_00704a59
        // 007049b5
        fldz
        // 007049b7
        ret
    L_007049b8:
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 007049c0
        xorpd       xmm1,xmm1
        // 007049c4
        andpd       xmm3,xmm4
        // 007049c8
        pcmpeqd     xmm1,xmm3
        // 007049cc
        pmovmskb    ecx,xmm1
        // 007049d0
        and         ecx,0FFh
        // 007049d6
        cmp         ecx,0FFh
        // 007049dc
        jne L_00704a9f
        // 007049e2
        pextrw      ecx,xmm4,3
        // 007049e7
        and         ecx,8000h
        // 007049ed
        cmp         ecx,0
        // 007049f0
        je L_00704a83
        // 007049f6
        and         eax,0FFh
        // 007049fb
        cmp         eax,0FFh
        // 00704a00
        jne L_00704a67
        // 00704a02
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 00704a08
        movlpd      xmm2,qword ptr [esp+0Ch]
        // 00704a0e
        mov         ecx,3F4h
        // 00704a13
        movd        xmm3,ecx
        // 00704a17
        andpd       xmm1, [task8_pow_const_0075a960 + 03910h]
        // 00704a1f
        psrlq       xmm1,34h
        // 00704a24
        psubd       xmm1,xmm3
        // 00704a28
        pxor        xmm3,xmm3
        // 00704a2c
        psllq       xmm2,xmm1
        // 00704a30
        pcmpeqd     xmm2,xmm3
        // 00704a34
        pmovmskb    eax,xmm2
        // 00704a38
        and         eax,0FFh
        // 00704a3d
        cmp         eax,0FFh
        // 00704a42
        je L_00704a67
        // 00704a44
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 00704a4a
        pextrw      eax,xmm1,3
        // 00704a4f
        and         eax,8000h
        // 00704a54
        cmp         eax,0
        // 00704a57
        je L_00704a60
    L_00704a59:
        fld         qword ptr [task8_pow_const_0075a960 + 03970h]
        // 00704a5f
        ret
    L_00704a60:
        fld         qword ptr [task8_pow_const_0075a960 + 03968h]
        // 00704a66
        ret
    L_00704a67:
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 00704a6d
        pextrw      eax,xmm1,3
        // 00704a72
        and         eax,8000h
        // 00704a77
        cmp         eax,0
        // 00704a7a
        je L_00704bbe
        // 00704a80
        fldz
        // 00704a82
        ret
    L_00704a83:
        movlpd      xmm1,qword ptr [esp+0Ch]
        // 00704a89
        pextrw      eax,xmm1,3
        // 00704a8e
        and         eax,8000h
        // 00704a93
        cmp         eax,0
        // 00704a96
        je L_00704bbe
        // 00704a9c
        fldz
        // 00704a9e
        ret
    L_00704a9f:
        addsd       xmm4,xmm4
        // 00704aa3
        movsd       xmm0,xmm4
        // 00704aa7
        mov         edx,3EEh
        // 00704aac
        jmp L_00704c41
    L_00704ab1:
        movlpd      xmm2,qword ptr [esp+4]
        // 00704ab7
        movd        eax,xmm2
        // 00704abb
        psrlq       xmm2,20h
        // 00704ac0
        movd        edx,xmm2
        // 00704ac4
        and         edx,7FFFFFFFh
        // 00704aca
        or          eax,edx
        // 00704acc
        mov         ecx,0
        // 00704ad1
        cmp         eax,0
        // 00704ad4
        je L_0070495a
        // 00704ada
        movlpd      xmm1,qword ptr [task8_pow_const_0075a960 + 03958h]
        // 00704ae2
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03980h]
        // 00704aea
        mulsd       xmm1,xmm1
        // 00704aee
        mov         edx,1Ch
        // 00704af3
        jmp L_00704c41
    L_00704af8:
        movlpd      xmm4,qword ptr [esp+4]
        // 00704afe
        movlpd      xmm2,qword ptr [esp+0Ch]
        // 00704b04
        movd        eax,xmm4
        // 00704b08
        cmp         eax,0
        // 00704b0b
        jne L_00704b4c
        // 00704b0d
        psrlq       xmm4,20h
        // 00704b12
        movd        edx,xmm4
        // 00704b16
        cmp         edx,3FF00000h
        // 00704b1c
        je L_00704c2a
        // 00704b22
        cmp         edx,0BFF00000h
        // 00704b28
        jne L_00704b4c
        // 00704b2a
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 00704b32
        xorpd       xmm1,xmm1
        // 00704b36
        andpd       xmm3,xmm2
        // 00704b3a
        pcmpeqd     xmm1,xmm3
        // 00704b3e
        pmovmskb    eax,xmm1
        // 00704b42
        cmp         eax,0FFh
        // 00704b47
        jne L_00704bc5
        // 00704b49
        fld1
        // 00704b4b
        ret
    L_00704b4c:
        movlpd      xmm3,qword ptr [task8_pow_const_0075a960 + 038F0h]
        // 00704b54
        xorpd       xmm1,xmm1
        // 00704b58
        andpd       xmm3,xmm2
        // 00704b5c
        pcmpeqd     xmm1,xmm3
        // 00704b60
        pmovmskb    eax,xmm1
        // 00704b64
        and         eax,0FFh
        // 00704b69
        cmp         eax,0FFh
        // 00704b6e
        jne L_00704bc5
        // 00704b70
        pextrw      eax,xmm2,3
        // 00704b75
        movlpd      xmm4,qword ptr [esp+4]
        // 00704b7b
        and         eax,8000h
        // 00704b80
        xor         ecx,0BFF00000h
        // 00704b86
        or          edx,ecx
        // 00704b88
        cmp         edx,0
        // 00704b8b
        je L_00704c2a
        // 00704b91
        cmp         eax,0
        // 00704b94
        je L_00704baa
        // 00704b96
        pextrw      eax,xmm4,3
        // 00704b9b
        and         eax,7FF0h
        // 00704ba0
        cmp         eax,3FF0h
        // 00704ba5
        jb L_00704bbe
        // 00704ba7
        fldz
        // 00704ba9
        ret
    L_00704baa:
        pextrw      eax,xmm4,3
        // 00704baf
        and         eax,7FF0h
        // 00704bb4
        cmp         eax,3FF0h
        // 00704bb9
        jae L_00704bbe
        // 00704bbb
        fldz
        // 00704bbd
        ret
    L_00704bbe:
        fld         qword ptr [task8_pow_const_0075a960 + 03960h]
        // 00704bc4
        ret
    L_00704bc5:
        addsd       xmm2,xmm2
        // 00704bc9
        movsd       xmm0,xmm2
        // 00704bcd
        mov         edx,3EEh
        // 00704bd2
        jmp L_00704c41
    L_00704bd4:
        movd        eax,xmm4
        // 00704bd8
        psrlq       xmm4,20h
        // 00704bdd
        movd        edx,xmm4
        // 00704be1
        and         edx,7FFFFFFFh
        // 00704be7
        mov         ecx,eax
        // 00704be9
        or          eax,edx
        // 00704beb
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03900h]
        // 00704bf3
        mov         edx,1Ah
        // 00704bf8
        cmp         eax,0
        // 00704bfb
        je L_00704c41
        // 00704bfd
        movd        eax,xmm4
        // 00704c01
        mov         edx,1Dh
        // 00704c06
        and         eax,7FFFFFFFh
        // 00704c0b
        cmp         eax,7FF00000h
        // 00704c10
        ja L_00704c41
        // 00704c12
        jb L_00704c19
        // 00704c14
        cmp         ecx,0
        // 00704c17
        ja L_00704c41
    L_00704c19:
        sub         esp,10h
        // 00704c1c
        movlpd      qword ptr [esp+4],xmm0
        // 00704c22
        fld         qword ptr [esp+4]
        // 00704c26
        add         esp,10h
        // 00704c29
        ret
    L_00704c2a:
        movlpd      xmm1,qword ptr [task8_pow_const_0075a960 + 03980h]
        // 00704c32
        movlpd      xmm0,qword ptr [esp+0Ch]
        // 00704c38
        addsd       xmm0,xmm1
        // 00704c3c
        mov         edx,1Ch
    L_00704c41:
        sub         esp,1Ch
        // 00704c44
        movlpd      qword ptr [esp+10h],xmm0
        // 00704c4a
        mov         dword ptr [esp+0Ch],edx
        // 00704c4e
        mov         edx,esp
        // 00704c50
        add         edx,10h
        // 00704c53
        mov         dword ptr [esp+8],edx
        // 00704c57
        add         edx,18h
        // 00704c5a
        mov         dword ptr [esp+4],edx
        // 00704c5e
        sub         edx,8
        // 00704c61
        mov         dword ptr [esp],edx
        // 00704c64
        call task8_math_sse_error_007053de
        // 00704c69
        fld         qword ptr [esp+10h]
        // 00704c6d
        add         esp,1Ch
        // 00704c70
        ret
    L_00704c71:
        cmp         eax,0
        // 00704c74
        jle L_00704c9e
        // 00704c76
        cmp         eax,40000h
        // 00704c7b
        jae L_00704ebd
        // 00704c81
        push        esi
        // 00704c82
        mov         edx,eax
        // 00704c84
        and         eax,7Fh
        // 00704c87
        add         ecx,3FF00h
        // 00704c8d
        sub         edx,80h
        // 00704c93
        and         edx,0FFFFFF80h
        // 00704c96
        push        edi
        // 00704c97
        mov         edi,3FF0h
        // 00704c9c
        jmp L_00704cc4
    L_00704c9e:
        cmp         eax,0FFFC0200h
        // 00704ca3
        jle L_00704e9a
        // 00704ca9
        push        esi
        // 00704caa
        mov         edx,eax
        // 00704cac
        and         eax,7Fh
        // 00704caf
        add         ecx,80h
        // 00704cb5
        and         edx,0FFFFFF80h
        // 00704cb8
        add         edx,3FE80h
        // 00704cbe
        push        edi
        // 00704cbf
        mov         edi,0
    L_00704cc4:
        addsd       xmm4,xmm0
        // 00704cc8
        movsd       xmm0,xmm3
        // 00704ccc
        addsd       xmm3,qword ptr [task8_pow_const_0075a960 + 038E0h]
        // 00704cd4
        mov         esi,edx
        // 00704cd6
        sub         esi,1FF80h
        // 00704cdc
        mulpd       xmm2,xmm7
        // 00704ce0
        subsd       xmm3,qword ptr [task8_pow_const_0075a960 + 038E0h]
        // 00704ce8
        mulpd       xmm7,xmm7
        // 00704cec
        subsd       xmm0,xmm3
        // 00704cf0
        add         eax,eax
        // 00704cf2
        add         eax,eax
        // 00704cf4
        add         eax,eax
        // 00704cf6
        add         eax,eax
        // 00704cf8
        movapd      xmm3, [eax+task8_pow_const_0075a960 + 030E0h]
        // 00704d00
        movapd      xmm6, [task8_pow_const_0075a960 + 03920h]
        // 00704d08
        mulpd       xmm1,xmm7
        // 00704d0c
        addpd       xmm2,xmm1
        // 00704d10
        pshufd      xmm1,xmm2,0EEh
        // 00704d15
        mulsd       xmm2,xmm7
        // 00704d19
        addsd       xmm2,xmm1
        // 00704d1d
        addsd       xmm2,xmm4
        // 00704d21
        movlpd      xmm4,qword ptr [task8_pow_const_0075a960 + 03940h]
        // 00704d29
        movd        xmm1,edx
        // 00704d2d
        sub         edx,1FF80h
        // 00704d33
        neg         edx
        // 00704d35
        sar         edx,7
        // 00704d38
        add         edx,2
        // 00704d3b
        mov         eax,edx
        // 00704d3d
        and         eax,20h
        // 00704d40
        add         edx,eax
        // 00704d42
        xorpd       xmm7,xmm7
        // 00704d46
        mov         eax,3F80h
        // 00704d4b
        pinsrw      xmm7,eax,3
        // 00704d50
        mulsd       xmm2,qword ptr [esp+14h]
        // 00704d56
        mulsd       xmm0,xmm7
        // 00704d5a
        psllq       xmm1,2Dh
        // 00704d5f
        pshufd      xmm1,xmm1,44h
        // 00704d64
        movapd      xmm7, [task8_pow_const_0075a960 + 03930h]
        // 00704d6c
        addsd       xmm5,xmm2
        // 00704d70
        mulpd       xmm3,xmm1
        // 00704d74
        addsd       xmm0,xmm5
        // 00704d78
        unpcklpd    xmm0,xmm0
        // 00704d7c
        mulpd       xmm6,xmm0
        // 00704d80
        mulsd       xmm4,xmm0
        // 00704d84
        mulpd       xmm0,xmm0
        // 00704d88
        addpd       xmm7,xmm6
        // 00704d8c
        mulpd       xmm7,xmm0
        // 00704d90
        mulsd       xmm0,xmm3
        // 00704d94
        pshufd      xmm6,xmm7,0EEh
        // 00704d99
        mulsd       xmm0,xmm7
        // 00704d9d
        pshufd      xmm5,xmm3,0EEh
        // 00704da2
        mulsd       xmm6,xmm3
        // 00704da6
        mulsd       xmm4,xmm3
        // 00704daa
        movd        xmm7,ecx
        // 00704dae
        psllq       xmm7,2Dh
        // 00704db3
        movd        xmm2,edx
        // 00704db7
        pcmpeqd     xmm1,xmm1
        // 00704dbb
        psllq       xmm1,xmm2
        // 00704dbf
        addsd       xmm0,xmm5
        // 00704dc3
        addsd       xmm0,xmm6
        // 00704dc7
        andpd       xmm1,xmm3
        // 00704dcb
        addsd       xmm0,xmm4
        // 00704dcf
        xorpd       xmm6,xmm6
        // 00704dd3
        pcmpeqd     xmm4,xmm4
        // 00704dd7
        psllq       xmm4,xmm2
        // 00704ddb
        subsd       xmm3,xmm1
        // 00704ddf
        movsd       xmm2,xmm1
        // 00704de3
        addsd       xmm1,xmm0
        // 00704de7
        andpd       xmm1,xmm4
        // 00704deb
        pinsrw      xmm6,edi,3
        // 00704df0
        pop         edi
        // 00704df1
        subsd       xmm2,xmm1
        // 00704df5
        addsd       xmm0,xmm2
        // 00704df9
        addsd       xmm0,xmm3
        // 00704dfd
        cmp         esi,0
        // 00704e00
        jg L_00704e50
        // 00704e02
        pop         esi
        // 00704e03
        mulsd       xmm0,xmm7
        // 00704e07
        mulsd       xmm1,xmm7
        // 00704e0b
        addsd       xmm0,xmm1
        // 00704e0f
        mulsd       xmm6,xmm0
        // 00704e13
        addsd       xmm0,xmm6
        // 00704e17
        pextrw      eax,xmm0,3
        // 00704e1c
        and         eax,7FF0h
        // 00704e21
        mov         edx,18h
        // 00704e26
        cmp         eax,7FF0h
        // 00704e2b
        je L_00704c41
        // 00704e31
        mov         edx,19h
        // 00704e36
        cmp         eax,0
        // 00704e39
        je L_00704c41
        // 00704e3f
        sub         esp,10h
        // 00704e42
        movlpd      qword ptr [esp+4],xmm0
        // 00704e48
        fld         qword ptr [esp+4]
        // 00704e4c
        add         esp,10h
        // 00704e4f
        ret
    L_00704e50:
        pop         esi
        // 00704e51
        addsd       xmm0,xmm1
        // 00704e55
        mulsd       xmm0,xmm7
        // 00704e59
        mulsd       xmm6,xmm0
        // 00704e5d
        addsd       xmm0,xmm6
        // 00704e61
        pextrw      eax,xmm0,3
        // 00704e66
        and         eax,7FF0h
        // 00704e6b
        mov         edx,18h
        // 00704e70
        cmp         eax,7FF0h
        // 00704e75
        je L_00704c41
        // 00704e7b
        mov         edx,19h
        // 00704e80
        cmp         eax,0
        // 00704e83
        je L_00704c41
        // 00704e89
        sub         esp,10h
        // 00704e8c
        movlpd      qword ptr [esp+4],xmm0
        // 00704e92
        fld         qword ptr [esp+4]
        // 00704e96
        add         esp,10h
        // 00704e99
        ret
    L_00704e9a:
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03978h]
        // 00704ea2
        movd        xmm1,ecx
        // 00704ea6
        mulsd       xmm0,xmm0
        // 00704eaa
        psllq       xmm1,2Dh
        // 00704eaf
        orpd        xmm0,xmm1
        // 00704eb3
        mov         edx,19h
        // 00704eb8
        jmp L_00704c41
    L_00704ebd:
        mov         edx,18h
        // 00704ec2
        cmp         ecx,0
        // 00704ec5
        je L_00704ed4
        // 00704ec7
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03968h]
        // 00704ecf
        jmp L_00704c41
    L_00704ed4:
        movlpd      xmm0,qword ptr [task8_pow_const_0075a960 + 03960h]
        // 00704edc
        jmp L_00704c41
    L_00704ee1:
        pshufd      xmm4,xmm1,44h
        // 00704ee6
        mulpd       xmm4,xmm6
        // 00704eea
        pextrw      eax,xmm4,3
        // 00704eef
        and         eax,7FF0h
        // 00704ef4
        mov         edx,40A0h
        // 00704ef9
        sub         edx,eax
        // 00704efb
        sub         eax,3C70h
        // 00704f00
        or          edx,eax
        // 00704f02
        cmp         edx,80000000h
        // 00704f08
        jb L_00704608
        // 00704f0e
        cmp         eax,80000000h
        // 00704f13
        jb L_00704f35
        // 00704f15
        or          ecx,1FF80h
        // 00704f1b
        movd        xmm0,ecx
        // 00704f1f
        psllq       xmm0,2Dh
        // 00704f24
        sub         esp,10h
        // 00704f27
        movlpd      qword ptr [esp+4],xmm0
        // 00704f2d
        fld         qword ptr [esp+4]
        // 00704f31
        add         esp,10h
        // 00704f34
        ret
    L_00704f35:
        movlpd      xmm4,qword ptr [esp+4]
        // 00704f3b
        pextrw      edx,xmm4,3
        // 00704f40
        and         edx,7FF0h
        // 00704f46
        sub         edx,3FF0h
        // 00704f4c
        pextrw      eax,xmm1,3
        // 00704f51
        xor         eax,edx
        // 00704f53
        and         eax,8000h
        // 00704f58
        cmp         eax,0
        // 00704f5b
        jne L_00704e9a
        // 00704f61
        jmp L_00704ebd
    }
}
