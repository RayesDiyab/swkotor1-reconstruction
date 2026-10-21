#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_pow_internal_006fc24d() {
    __asm {
        // 006fc24d
        mov         ecx,eax
        // 006fc24f
        push        eax
        // 006fc250
        wait
        // 006fc251
        fnstcw      word ptr [esp]
        // 006fc254
        cmp         word ptr [esp],27Fh
        // 006fc25a
        je L_006fc261
        // 006fc25c
        call task8_math_load_cw_00704215
    L_006fc261:
        and         ecx,7FF00000h
        // 006fc267
        lea         edx,[esp+8]
        // 006fc26b
        cmp         ecx,7FF00000h
        // 006fc271
        je L_006fc314
        // 006fc277
        call task8_math_fload_00704245
        // 006fc27c
        je L_006fc310
        // 006fc282
        test        eax,7FF00000h
        // 006fc287
        je L_006fc383
    L_006fc28d:
        mov         cl,byte ptr [esp+0Fh]
        // 006fc291
        and         cl,80h
        // 006fc294
        jne L_006fc3ed
    L_006fc29a:
        fyl2x
        // 006fc29c
        call task8_math_exp2_00704200
        // 006fc2a1
        cmp         cl,1
        // 006fc2a4
        jne L_006fc2a8
        // 006fc2a6
        fchs
    L_006fc2a8:
        cmp         dword ptr [task8_crt_fast_math_enabled_00833bdc],0
        // 006fc2af
        jne L_0070429e
        // 006fc2b5
        lea         ecx,[task8_pow_const_007a2788]
        // 006fc2bb
        mov         edx,1Dh
        // 006fc2c0
        jmp L_007042e9
    L_006fc2c5:
        cmp         dword ptr [task8_crt_fast_math_enabled_00833bdc],0
        // 006fc2cc
        jne L_0070429e
        // 006fc2d2
        lea         ecx,[task8_pow_const_007a2788]
        // 006fc2d8
        mov         edx,1Dh
        // 006fc2dd
        call task8_math_error_pair_007041a0
        // 006fc2e2
        pop         edx
        // 006fc2e3
        ret
    L_006fc2e4:
        lea         edx,[esp+8]
        // 006fc2e8
        call task8_math_fload_00704245
        // 006fc2ed
        test        byte ptr [esp+16h],8
        // 006fc2f2
        jne L_006fc2f7
        // 006fc2f4
        inc         ecx
        // 006fc2f5
        jmp L_006fc32a
    L_006fc2f7:
        faddp       st(1),st
        // 006fc2f9
        mov         eax,1
        // 006fc2fe
        jmp L_006fc2c5
    L_006fc300:
        test        byte ptr [esp+0Eh],8
        // 006fc305
        jne L_006fc2f7
    L_006fc307:
        faddp       st(1),st
        // 006fc309
        mov         eax,7
        // 006fc30e
        jmp L_006fc2c5
    L_006fc310:
        xor         ecx,ecx
        // 006fc312
        jmp L_006fc32a
    L_006fc314:
        xor         ecx,ecx
        // 006fc316
        and         eax,0FFFFFh
        // 006fc31b
        or          eax,dword ptr [esp+10h]
        // 006fc31f
        jne L_006fc2e4
        // 006fc321
        lea         edx,[esp+8]
        // 006fc325
        call task8_math_fload_00704245
    L_006fc32a:
        mov         eax,dword ptr [esp+0Ch]
        // 006fc32e
        mov         edx,eax
        // 006fc330
        and         eax,7FF00000h
        // 006fc335
        and         edx,0FFFFFh
        // 006fc33b
        cmp         eax,7FF00000h
        // 006fc340
        jne L_006fc348
        // 006fc342
        or          edx,dword ptr [esp+8]
        // 006fc346
        jne L_006fc300
    L_006fc348:
        test        ecx,ecx
        // 006fc34a
        jne L_006fc307
        // 006fc34c
        sub         esp,74h
        // 006fc34f
        mov         ecx,esp
        // 006fc351
        push        ecx
        // 006fc352
        sub         esp,10h
        // 006fc355
        fstp        qword ptr [esp]
        // 006fc358
        fstp        qword ptr [esp+8]
        // 006fc35c
        wait
        // 006fc35d
        fnsave      [ecx+8]
        // 006fc360
        call task8_math_pow_special_007051e1
        // 006fc365
        add         esp,10h
        // 006fc368
        pop         ecx
        // 006fc369
        frstor      [ecx+8]
        // 006fc36c
        fld         qword ptr [ecx]
        // 006fc36e
        add         esp,74h
        // 006fc371
        test        eax,eax
        // 006fc373
        je L_0070429e
        // 006fc379
        mov         eax,1
        // 006fc37e
        jmp L_006fc2c5
    L_006fc383:
        mov         eax,dword ptr [esp+0Ch]
        // 006fc387
        and         eax,0FFFFFh
        // 006fc38c
        or          eax,dword ptr [esp+8]
        // 006fc390
        jne L_006fc28d
        // 006fc396
        fstp        st(0)
        // 006fc398
        mov         eax,dword ptr [esp+14h]
        // 006fc39c
        and         eax,7FFFFFFFh
        // 006fc3a1
        or          eax,dword ptr [esp+10h]
        // 006fc3a5
        je L_006fc3e4
        // 006fc3a7
        call task8_math_integer_test_006fc412
        // 006fc3ac
        mov         ch,byte ptr [esp+0Fh]
        // 006fc3b0
        shr         ch,7
        // 006fc3b3
        test        dword ptr [esp+17h],80h
        // 006fc3bb
        je L_006fc3d3
        // 006fc3bd
        fld         tbyte ptr [task8_pow_const_007a3110]
        // 006fc3c3
        test        ch,cl
        // 006fc3c5
        je L_006fc3c9
        // 006fc3c7
        fchs
    L_006fc3c9:
        mov         eax,2
        // 006fc3ce
        jmp L_006fc2c5
    L_006fc3d3:
        fldz
        // 006fc3d5
        test        ch,cl
        // 006fc3d7
        je L_0070429e
        // 006fc3dd
        fchs
        // 006fc3df
        jmp L_0070429e
    L_006fc3e4:
        fstp        st(0)
        // 006fc3e6
        fld1
        // 006fc3e8
        jmp L_0070429e
    L_006fc3ed:
        fld         st(1)
        // 006fc3ef
        call task8_math_integer_test_006fc412
        // 006fc3f4
        fchs
        // 006fc3f6
        test        cl,cl
        // 006fc3f8
        jne L_006fc29a
        // 006fc3fe
        fstp        st(0)
        // 006fc400
        fstp        st(0)
        // 006fc402
        fld         tbyte ptr [task8_pow_const_007a30e0]
        // 006fc408
        mov         eax,1
        // 006fc40d
        jmp L_006fc2c5
    L_007042e9:
        sub         esp,8
        // 007042ec
        fst         qword ptr [esp]
        // 007042ef
        mov         eax,dword ptr [esp+4]
        // 007042f3
        add         esp,8
        // 007042f6
        and         eax,7FF00000h
        // 007042fb
        je L_0070433a
        // 007042fd
        cmp         eax,7FF00000h
        // 00704302
        je L_00704363
        // 00704304
        mov         ax,word ptr [esp]
        // 00704308
        cmp         ax,27Fh
        // 0070430c
        je L_00704338
        // 0070430e
        and         ax,20h
        // 00704312
        jne L_00704335
        // 00704314
        wait
        // 00704315
        fnstsw      ax
        // 00704317
        and         ax,20h
        // 0070431b
        je L_00704335
        // 0070431d
        mov         eax,8
    L_00704322:
        cmp         edx,1Dh
        // 00704325
        je L_0070432e
        // 00704327
        call task8_math_error_single_007041b7
        // 0070432c
        pop         edx
        // 0070432d
        ret
    L_0070432e:
        call task8_math_error_pair_007041a0
        // 00704333
        pop         edx
        // 00704334
        ret
    L_00704335:
        fldcw       word ptr [esp]
    L_00704338:
        pop         edx
        // 00704339
        ret
    L_0070433a:
        fld         qword ptr [task8_pow_const_0075a960 + 02Ch]
        // 00704340
        fxch        st(1)
        // 00704342
        fscale
        // 00704344
        fstp        st(1)
        // 00704346
        fld         st(0)
        // 00704348
        fabs
        // 0070434a
        fcomp       qword ptr [task8_pow_const_0075a960 + 01Ch]
        // 00704350
        wait
        // 00704351
        fnstsw      ax
        // 00704353
        sahf
        // 00704354
        mov         eax,4
        // 00704359
        jae L_00704322
        // 0070435b
        fmul        qword ptr [task8_pow_const_0075a960 + 03Ch]
        // 00704361
        jmp L_00704322
    L_00704363:
        fld         qword ptr [task8_pow_const_0075a960 + 024h]
        // 00704369
        fxch        st(1)
        // 0070436b
        fscale
        // 0070436d
        fstp        st(1)
        // 0070436f
        fld         st(0)
        // 00704371
        fabs
        // 00704373
        fcomp       qword ptr [task8_pow_const_0075a960 + 014h]
        // 00704379
        wait
        // 0070437a
        fnstsw      ax
        // 0070437c
        sahf
        // 0070437d
        mov         eax,3
        // 00704382
        jbe L_00704322
        // 00704384
        fmul        qword ptr [task8_pow_const_0075a960 + 034h]
        // 0070438a
        jmp L_00704322
    L_0070429e:
        cmp         word ptr [esp],27Fh
        // 007042a4
        je L_007042a9
        // 007042a6
        fldcw       word ptr [esp]
    L_007042a9:
        pop         edx
        // 007042aa
        ret
    }
}
