#include "bb30c55420-helpers.h"
// Recovered complete 40-byte body. Valid type1 writes33; types2/3 write34.
// Other signed int32 inputs have no call/write and are returned unchanged.
// The errno pointer result is preserved, and no new errno/PTD storage is owned.
extern "C" __declspec(naked) uint32_t __cdecl task8_math_port_00707c06(int32_t) {
    __asm {
        mov eax, dword ptr [esp+4]
        cmp eax, 1
        je domain_error
        jle done
        cmp eax, 3
        jg done
        call task8_math_port_006ffe78
        mov dword ptr [eax], 22h
        ret
    domain_error:
        call task8_math_port_006ffe78
        mov dword ptr [eax], 21h
    done:
        ret
    }
}
