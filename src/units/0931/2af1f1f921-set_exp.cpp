#include "416769497d-set_exp_ports.h"
// Original00707600: bit replacement after x87 binary64 spill, three stack words.
extern "C" __declspec(naked) void __cdecl task8_math_port_00707600(){
 __asm {
 push ebp
 mov ebp,esp
 push ecx
 push ecx
 mov eax,[ebp+16]
 fld qword ptr [ebp+8]
 mov ecx,[ebp+14]
 fstp qword ptr [ebp-8]



 add eax,03feh

 shl eax,4
 and ecx,0ffff800fh
 or eax,ecx
 mov word ptr [ebp-2],ax
 fld qword ptr [ebp-8]
 leave
 ret
 }
}
