#include "4206024f16-ports.h"
extern "C" __declspec(naked) void __cdecl task8_math_port_006fe52c() {
 __asm {
 push ebp
 mov ebp,esp
 mov eax,[ebp+14]
 mov ecx,07ff0h
 mov edx,eax
 and edx,ecx
 cmp dx,cx
 jne finite_classification
 fld qword ptr [ebp+8]
 push ecx
 push ecx
 fstp qword ptr [esp]
 call task8_math_port_0070762a
 dec eax
 pop ecx
 pop ecx
 je positive_infinity
 dec eax
 je negative_infinity
 dec eax
 je quiet_nan
 xor eax,eax
 inc eax
 pop ebp
 ret
quiet_nan:
 push 2
return_stack_constant:
 pop eax
 pop ebp
 ret
negative_infinity:
 push 4
 jmp return_stack_constant
positive_infinity:
 mov eax,0200h
 pop ebp
 ret
finite_classification:
 and eax,08000h
 test dx,dx
 mov ecx,eax
 jne compare_zero
 test dword ptr [ebp+12],0fffffh
 jne subnormal
 cmp dword ptr [ebp+8],0
 je compare_zero
subnormal:
 mov eax,ecx
 neg eax
 sbb eax,eax
 and eax,0ffffff90h
 add eax,080h
 pop ebp
 ret
compare_zero:
 fld qword ptr [ebp+8]
 fcomp qword ptr task8_pow_const_0073edf8
 fnstsw ax
 test ah,044h
 mov eax,ecx
 jp normal
 neg eax
 sbb eax,eax
 and eax,0ffffffe0h
 add eax,040h
 pop ebp
 ret
normal:
 neg eax
 sbb eax,eax




 and eax,0ffffff08h
 add eax,0100h

 pop ebp
 ret
 }
}
