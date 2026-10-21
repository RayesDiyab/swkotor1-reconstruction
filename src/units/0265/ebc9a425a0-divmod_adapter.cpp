#include "b6a6f59d48-startup_views.h"
namespace kotor_menu_10_19::dependencies {
__declspec(naked) void __cdecl divmod_adapter(Wide,Wide,Divmod*){
    __asm {
        push ebx
        push esi
        mov esi,esp
        push dword ptr [esi+24]
        push dword ptr [esi+20]
        push dword ptr [esi+16]
        push dword ptr [esi+12]
        call unresolved_aulldvrm_006fd0c0
        mov esi,[esi+28]
        mov [esi],eax
        mov [esi+4],edx
        mov [esi+8],ecx
        mov [esi+12],ebx
        pop esi
        pop ebx
        ret
    }
}
}
