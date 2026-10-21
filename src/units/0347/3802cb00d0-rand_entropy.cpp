#include "e8a5e6befc-rand_constructor.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Physical raw stack observations require the pinned MSVC x86 route.
#endif
namespace startup_rand {
__declspec(naked) void __fastcall AlignedGenSeed(Internal*,void*) {
    __asm {
        push ebp
        mov ebp,esp
        and esp,0fffffff8h
        call GenSeed005ed080
        mov esp,ebp
        pop ebp
        ret
    }
}
// Transport only. Task17 remains the sole Add algorithm owner.
static void __fastcall ExistingAdd(Internal* self,void*,unsigned char* source,std::int32_t count) {
    self->Add(source,count);
}
__declspec(naked) void __fastcall GenSeed005ed080(Internal*,void*) {
    // At entry ESP=S. Source is[S-80h,S), the final128 bytes of this
    // ACh callee-local allocation, not the pool or caller's return/arguments.
    // Raw REP MOVSD deliberately observes the uncleared actual stack backing.
    __asm {
        sub esp,0ach
        push ebx
        push esi
        mov ebx,ecx
        push edi
        mov edi,[ebx+64h]
        lea eax,[esp+0ch]
        mov ecx,20h
        lea esi,[esp+38h]
        push eax
        rep movsd
        call dword ptr [rand_imports]
        test eax,eax
        jz system_observation
        lea eax,[esp+0ch]
        push 8
        push eax
        mov ecx,ebx
        call ExistingAdd
    system_observation:
        lea eax,[esp+14h]
        push eax
        call dword ptr [rand_imports+4]
        lea eax,[esp+14h]
        push 24h
        push eax
        mov ecx,ebx
        call ExistingAdd
        pop edi
        pop esi
        pop ebx
        add esp,0ach
        ret
    }
}
}
