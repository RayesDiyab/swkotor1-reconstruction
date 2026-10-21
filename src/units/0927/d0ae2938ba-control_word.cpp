#include "bb30c55420-helpers.h"
// The observed caller mask word is overwritten. FNSTCW only writes its low
// 16 bits; the saved ECX upper half participates in that 32-bit merged word.
// Do not replace this body with host _controlfp or erase the stack write.
extern "C" __declspec(naked) int32_t __cdecl task8_math_port_00707e25(uint32_t, uint32_t) {
    __asm {
        push ebp
        mov ebp, esp
        push ecx
        fwait
        fnstcw word ptr [ebp-4]
        mov eax, dword ptr [ebp+0Ch]
        mov ecx, dword ptr [ebp+8]




        and ecx, dword ptr [ebp+0Ch]
        not eax

        and eax, dword ptr [ebp-4]
        or eax, ecx
        mov dword ptr [ebp+0Ch], eax
        fldcw word ptr [ebp+0Ch]
        movsx eax, word ptr [ebp-4]
        leave
        ret
    }
}
