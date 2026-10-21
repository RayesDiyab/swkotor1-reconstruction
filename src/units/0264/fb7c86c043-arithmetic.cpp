#include "6ff625219a-arithmetic.h"
#if defined(COMPILER_ERROR)
#error Deliberate compiler-error control
#endif
using kotor_menu_10_19::Word;
using kotor_menu_10_19::Wide;
using kotor_menu_10_19::Divmod;
// Authored restoring division using only defined unsigned 32-bit operations.
// No compiler-provided 64-bit divide/modulo helper or game-state dependency.
extern "C" __declspec(noinline) void __cdecl kotor_unsigned_divide_words(
    Word nl,Word nh,Word dl,Word dh,Divmod* out) noexcept {
    Word ql=0,qh=0,rl=0,rh=0;
    for(unsigned remaining=64;remaining!=0;--remaining){
        const Word incoming=nh>>31;
        nh=(nh<<1)|(nl>>31);nl<<=1;
        const Word carry=rh>>31;
        rh=(rh<<1)|(rl>>31);rl=(rl<<1)|incoming;
        qh=(qh<<1)|(ql>>31);ql<<=1;
        if(carry || rh>dh || (rh==dh && rl>=dl)){
            const Word before=rl;
            rl-=dl;



            rh=rh-dh-Word(before<dl);

            ql|=1;
        }
    }



    *out={ql,qh,rl,rh};
}
extern "C" __declspec(naked) void __stdcall unresolved_aulldvrm_006fd0c0(Wide,Wide) {
    __asm {
        // Preserve the original unsigned DIV-by-zero exception class. This
        // branch is statically inspected only; fault-frame parity is unproved.
        mov eax,[esp+12]
        or eax,[esp+16]
        jnz divisor_nonzero
        mov eax,[esp+8]
        xor edx,edx
        div dword ptr [esp+12]
    divisor_nonzero:
        push ebp
        mov ebp,esp
        sub esp,16
        lea eax,[ebp-16]
        push eax
        push dword ptr [ebp+20]
        push dword ptr [ebp+16]
        push dword ptr [ebp+12]
        push dword ptr [ebp+8]
        call kotor_unsigned_divide_words
        add esp,20
        mov eax,[ebp-16]
        mov edx,[ebp-12]




        mov ecx,[ebp-8]
        mov ebx,[ebp-4]

        mov esp,ebp
        pop ebp
        ret 16
    }
}
namespace kotor_menu_10_19::dependencies {
__declspec(naked) Wide __stdcall unsigned_divide_006fb030(Wide,Wide) {
    __asm {
        // A normal unsigned 64-bit stdcall result must preserve caller EBX.
        push ebx
        mov ecx,esp
        push dword ptr [ecx+20]
        push dword ptr [ecx+16]
        push dword ptr [ecx+12]
        push dword ptr [ecx+8]
        call unresolved_aulldvrm_006fd0c0



        pop ebx

        ret 16
    }
}
}
