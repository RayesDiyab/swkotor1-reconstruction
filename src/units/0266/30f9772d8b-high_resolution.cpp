#include "8b7e073bf2-contracts.h"
namespace kotor_menu_10_19 {
// Authored reconstruction of both verified ranges. x87 values stay in ST0;
// there is no intermediate binary64 rounding or host C++ float-to-int cast.
__declspec(naked) Wide __thiscall TimerDependency::high_resolution_005e9260(){
 __asm {
  mov ecx,dword ptr [ecx]
  sub esp,8
  push esi
  mov esi,ecx
  cmp dword ptr [esi],1



  jne fallback

  lea eax,[esp+4]
  push eax
  call unresolved_QueryPerformanceCounter_0073d1a8
  fild qword ptr [esp+4]
  mov eax,dword ptr [esi+4]
  fmul qword ptr [esi+18h]
  pop esi
  add esp,8
  jmp unresolved_ftol2_006fae8c
 fallback:
  call unresolved_GetTickCount_005ec200
  mov ecx,1000
  mul ecx



  pop esi
  add esp,8
  ret
 }
}
}
