#include "c6ca4ec170-layers.h"

// Original 004AA980: cdecl, two borrowed Vector representation pointers,
// full EAX 0/1, no storage and no callees. Keep x87 FCOMP (including its
// exception/status effects), ordered short circuit, and an unchanged x87 TOP.
extern "C" std::uint32_t __cdecl aurora_vector_equal_004aa980(
    const void* left, const void* right) {
    std::uint32_t result;
    __asm {
        mov ecx,left
        mov edx,right






        fld dword ptr [ecx]
        fcomp dword ptr [edx]
        fnstsw ax




        test ah,44h
        jp not_equal

        fld dword ptr [ecx+4]
        fcomp dword ptr [edx+4]
        fnstsw ax




        test ah,44h
        jp not_equal

        fld dword ptr [ecx+8]
        fcomp dword ptr [edx+8]
        fnstsw ax




        test ah,44h
        jp not_equal




        mov eax,1

        jmp finished
not_equal:
        xor eax,eax
finished:
        mov result,eax
    }
    return result;
}
