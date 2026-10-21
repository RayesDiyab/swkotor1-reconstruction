#include "7b17d2192a-pow_arithmetic.h"
// Complete recovered instruction/control-flow body; strict unknown ports stay imports.
extern "C" __declspec(naked) void __cdecl task8_math_error_pair_007041a0() {
    __asm {
        // 007041a0
        push        ebp
        // 007041a1
        mov         ebp,esp
        // 007041a3
        add         esp,0FFFFFFE0h
        // 007041a6
        mov         dword ptr [ebp-20h],eax
        // 007041a9
        mov         eax,dword ptr [ebp+18h]
        // 007041ac
        mov         dword ptr [ebp-10h],eax
        // 007041af
        mov         eax,dword ptr [ebp+1Ch]
        // 007041b2
        mov         dword ptr [ebp-0Ch],eax
        // 007041b5
        jmp L_007041c0
    L_007041c0:
        fstp        qword ptr [ebp-8]
        // 007041c3
        mov         dword ptr [ebp-1Ch],ecx
        // 007041c6
        mov         eax,dword ptr [ebp+10h]
        // 007041c9
        mov         ecx,dword ptr [ebp+14h]
        // 007041cc
        mov         dword ptr [ebp-18h],eax
        // 007041cf
        mov         dword ptr [ebp-14h],ecx
        // 007041d2
        lea         eax,[ebp+8]
        // 007041d5
        lea         ecx,[ebp-20h]
        // 007041d8
        push        eax
        // 007041d9
        push        ecx
        // 007041da
        push        edx
        // 007041db
        call task8_math_x87_error_007096b2
        // 007041e0
        add         esp,0Ch
        // 007041e3
        fld         qword ptr [ebp-8]
        // 007041e6
        cmp         word ptr [ebp+8],27Fh
        // 007041ec
        je L_007041f1
        // 007041ee
        fldcw       word ptr [ebp+8]
    L_007041f1:
        leave
        // 007041f2
        ret
    }
}
