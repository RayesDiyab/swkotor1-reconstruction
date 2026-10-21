#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_pow_special_007051e1() {
    __asm {
        // 007051e1
        push        ebp
        // 007051e2
        mov         ebp,esp
        // 007051e4
        push        ecx
        // 007051e5
        push        ecx
        // 007051e6
        fld         qword ptr [ebp+8]
        // 007051e9
        push        esi
        // 007051ea
        fcom        qword ptr [task8_pow_const_0073edf8]
        // 007051f0
        xor         edx,edx
        // 007051f2
        xor         esi,esi
        // 007051f4
        fnstsw      ax
        // 007051f6
        test        ah,5
        // 007051f9
        jp L_007051fd
        // 007051fb
        fchs
    L_007051fd:
        mov         eax,7FF00000h
        // 00705202
        fstp        qword ptr [ebp-8]
        // 00705205
        cmp         dword ptr [ebp+14h],eax
        // 00705208
        mov         ecx,0FFF00000h
        // 0070520d
        jne L_0070524d
        // 0070520f
        cmp         dword ptr [ebp+10h],edx
        // 00705212
        jne L_0070528c
        // 00705214
        fld         qword ptr [ebp-8]
        // 00705217
        fcomp       qword ptr [task8_pow_const_0073ede8]
        // 0070521d
        fnstsw      ax
        // 0070521f
        test        ah,41h
        // 00705222
        je L_007052aa
        // 00705228
        fld         qword ptr [ebp-8]
        // 0070522b
        fcomp       qword ptr [task8_pow_const_0073ede8]
        // 00705231
        fnstsw      ax
        // 00705233
        test        ah,5
        // 00705236
        mov         eax,dword ptr [ebp+18h]
        // 00705239
        jnp L_007052c8
    L_0070523f:
        fld         qword ptr [task8_pow_const_007a32b8]
        // 00705245
        xor         esi,esi
        // 00705247
        inc         esi
        // 00705248
        jmp L_0070533c
    L_0070524d:
        cmp         dword ptr [ebp+14h],ecx
        // 00705250
        jne L_0070528c
        // 00705252
        cmp         dword ptr [ebp+10h],edx
        // 00705255
        jne L_0070528c
        // 00705257
        fld         qword ptr [ebp-8]
        // 0070525a
        fcomp       qword ptr [task8_pow_const_0073ede8]
        // 00705260
        fnstsw      ax
        // 00705262
        test        ah,41h
        // 00705265
        jne L_0070526e
        // 00705267
        fldz
        // 00705269
        jmp L_00705339
    L_0070526e:
        fld         qword ptr [ebp-8]
        // 00705271
        fcomp       qword ptr [task8_pow_const_0073ede8]
        // 00705277
        fnstsw      ax
        // 00705279
        test        ah,5
        // 0070527c
        mov         eax,dword ptr [ebp+18h]
        // 0070527f
        jp L_0070523f
        // 00705281
        fld         qword ptr [task8_pow_const_007a32b0]
        // 00705287
        jmp L_0070533c
    L_0070528c:
        cmp         dword ptr [ebp+0Ch],eax
        // 0070528f
        jne L_007052d0
        // 00705291
        cmp         dword ptr [ebp+8],edx
        // 00705294
        jne L_0070533e
        // 0070529a
        fld         qword ptr [ebp+10h]
        // 0070529d
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 007052a3
        fnstsw      ax
        // 007052a5
        test        ah,41h
        // 007052a8
        jne L_007052b5
    L_007052aa:
        fld         qword ptr [task8_pow_const_007a32b0]
        // 007052b0
        jmp L_00705339
    L_007052b5:
        fld         qword ptr [ebp+10h]
        // 007052b8
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 007052be
        fnstsw      ax
        // 007052c0
        test        ah,5
        // 007052c3
        mov         eax,dword ptr [ebp+18h]
        // 007052c6
        jp L_007052cc
    L_007052c8:
        fldz
        // 007052ca
        jmp L_0070533c
    L_007052cc:
        fld1
        // 007052ce
        jmp L_0070533c
    L_007052d0:
        cmp         dword ptr [ebp+0Ch],ecx
        // 007052d3
        jne L_0070533e
        // 007052d5
        cmp         dword ptr [ebp+8],edx
        // 007052d8
        jne L_0070533e
        // 007052da
        fld         qword ptr [ebp+10h]
        // 007052dd
        push        ecx
        // 007052de
        push        ecx
        // 007052df
        fstp        qword ptr [esp]
        // 007052e2
        call task8_math_classify_integer_00705177
        // 007052e7
        fld         qword ptr [ebp+10h]
        // 007052ea
        pop         ecx
        // 007052eb
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 007052f1
        pop         ecx
        // 007052f2
        mov         ecx,eax
        // 007052f4
        fnstsw      ax
        // 007052f6
        test        ah,41h
        // 007052f9
        jne L_00705310
        // 007052fb
        cmp         ecx,1
        // 007052fe
        fld         qword ptr [task8_pow_const_007a32b0]
        // 00705304
        jne L_00705308
        // 00705306
        fchs
    L_00705308:
        fstp        qword ptr [ebp+10h]
        // 0070530b
        fld         qword ptr [ebp+10h]
        // 0070530e
        jmp L_00705339
    L_00705310:
        fld         qword ptr [ebp+10h]
        // 00705313
        fcomp       qword ptr [task8_pow_const_0073edf8]
        // 00705319
        fnstsw      ax
        // 0070531b
        test        ah,5
        // 0070531e
        jp L_00705337
        // 00705320
        cmp         ecx,1
        // 00705323
        jne L_0070532d
        // 00705325
        fld         qword ptr [task8_pow_const_007a32d0]
        // 0070532b
        jmp L_0070532f
    L_0070532d:
        fldz
    L_0070532f:
        fstp        qword ptr [ebp+10h]
        // 00705332
        fld         qword ptr [ebp+10h]
        // 00705335
        jmp L_00705339
    L_00705337:
        fld1
    L_00705339:
        mov         eax,dword ptr [ebp+18h]
    L_0070533c:
        fstp        qword ptr [eax]
    L_0070533e:
        mov         eax,esi
        // 00705340
        pop         esi
        // 00705341
        leave
        // 00705342
        ret
    }
}
