#include "4206024f16-ports.h"
extern "C" __declspec(naked) void __cdecl task8_math_port_0070762a() {
 __asm {
 push ebp
 mov ebp,esp
 xor edx,edx
 cmp dword ptr [ebp+12],07ff00000h
 jne check_negative_inf
 cmp dword ptr [ebp+8],edx
 jne nan_checks
 xor eax,eax
 inc eax
 pop ebp
 ret
check_negative_inf:
 cmp dword ptr [ebp+12],0fff00000h
 jne nan_checks
 cmp dword ptr [ebp+8],edx
 jne nan_checks
 push 2
 jmp return_stack_constant
nan_checks:
 mov ecx,[ebp+14]
 mov eax,07ff8h
 and ecx,eax
 cmp cx,ax
 jne signalling_check



 push 3

 jmp return_stack_constant
signalling_check:
 cmp cx,07ff0h
 jne finite
 test dword ptr [ebp+12],07ffffh
 jne signalling
 cmp dword ptr [ebp+8],edx
 je finite
signalling:
 push 4
return_stack_constant:
 pop eax
 pop ebp
 ret
finite:
 xor eax,eax
 pop ebp
 ret
 }
}
