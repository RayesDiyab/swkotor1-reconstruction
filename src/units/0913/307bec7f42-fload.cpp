#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_fload_00704245() {
    __asm {
        // 00704245
        mov         eax,dword ptr [edx+4]
        // 00704248
        and         eax,7FF00000h
        // 0070424d
        cmp         eax,7FF00000h
        // 00704252
        je L_00704257
        // 00704254
        fld         qword ptr [edx]
        // 00704256
        ret
    L_00704257:
        mov         eax,dword ptr [edx+4]
        // 0070425a
        sub         esp,0Ah
        // 0070425d
        or          eax,7FFF0000h
        // 00704262
        mov         dword ptr [esp+6],eax
        // 00704266
        mov         eax,dword ptr [edx+4]
        // 00704269
        mov         ecx,dword ptr [edx]
        // 0070426b
        shld        eax,ecx,0Bh
        // 0070426f
        shl         ecx,0Bh
        // 00704272
        mov         dword ptr [esp+4],eax
        // 00704276
        mov         dword ptr [esp],ecx
        // 00704279
        fld         tbyte ptr [esp]
        // 0070427c
        add         esp,0Ah
        // 0070427f
        test        eax,0
        // 00704284
        mov         eax,dword ptr [edx+4]
        // 00704287
        ret
    }
}
