#include "21d96af8ce-ftol2.h"




// The original does not change the ambient control word. It first rounds through
// FISTP under that word, then corrects a nonzero rounded integer toward zero using
// the sign of two binary32 stores. Integer zero and the integer-indefinite/minimum
// bit pattern use a distinct two-pop path. Functional assembly is necessary to
// retain ST(0), 80-bit intermediates, exception/status effects and register carry.
// No machine-code byte array, original body execution, or compiler cast is used.
extern "C" __declspec(naked) unsigned __int64 __cdecl unresolved_ftol2_006fae8c() {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 32
        and esp, -16



        fld st(0)
        fst dword ptr [esp+24]
        fistp qword ptr [esp+16]
        fild qword ptr [esp+16]
        mov edx, dword ptr [esp+24]
        mov eax, dword ptr [esp+16]
        test eax, eax
        jz zero_low
    correction:
        fsubp st(1), st(0)
        test edx, edx
        jns positive_input
        fstp dword ptr [esp]
        mov ecx, dword ptr [esp]
        xor ecx, 80000000h
        add ecx, 7fffffffh



        adc eax, 0

        mov edx, dword ptr [esp+20]
        adc edx, 0
        jmp done
    positive_input:
        fstp dword ptr [esp]
        mov ecx, dword ptr [esp]
        add ecx, 7fffffffh
        sbb eax, 0
        mov edx, dword ptr [esp+20]
        sbb edx, 0
        jmp done
    zero_low:
        mov edx, dword ptr [esp+20]
        test edx, 7fffffffh
        jnz correction
        fstp dword ptr [esp+24]
        fstp dword ptr [esp+24]
    done:






        leave
        ret
    }
}
