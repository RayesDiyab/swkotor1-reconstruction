// Fork F3 carrier (decision D21: naked function with inline asm, no ml.exe).
// [vtable+04] of an AurTextureFind result: 00423429 mov edx,[eax]; 0042342B mov ecx,eax;
// 0042342D call [edx+4]; EAX (pointer) is returned by 004233E0. Zero stack words, RET 0 callee.
// Reloads the receiver's live vtable and tail-jumps the slot with ECX = receiver.
#include "fd1e4c71e2-texture_externals_f3.h"
namespace texture {
extern "C" __declspec(naked) Basic* __fastcall texture_call_slot_04(void*) {
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+4]
    }
}
}
