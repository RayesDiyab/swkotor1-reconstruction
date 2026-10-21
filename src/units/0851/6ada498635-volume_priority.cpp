#include "93ce1efce2-volume_priority.h"



using namespace sound_volume29_required;
using sound_lower29_required::volume_005dc930;
using sound_volume29::priority_005dcd50;
// Instruction-shaped source preserves x87 precision, rounding, exception flags,
// conversion ABI and callback-visible memory order. It never calls original code.
namespace sound_lower29_required {
__declspec(naked) void __fastcall volume_005dc930(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t) {
 __asm {
  push ebp
  mov ebp,dword ptr [esp+0ch]
  push esi
  mov esi,ecx
  mov ecx,dword ptr [esi+20h]
  push edi
  push esi
  call group_005d6ad0
  mov edi,eax
  mov al,byte ptr [esi+8]
  cmp al,4
  je use_argument
  cmp al,15h
  je use_argument
  fld dword ptr [one_0073d764]
  jmp scale_volume
 use_argument:
  fld dword ptr [esp+18h]
 scale_volume:
  push ebx
  mov bl,byte ptr [esp+14h]
  movzx eax,bl
  mov dword ptr [esp+14h],eax
  fild dword ptr [esp+14h]
  fmul st,st(1)
  call unresolved_ftol2_006fae8c
  fstp st(0)
  mov dword ptr [esp+1ch],eax
  mov eax,dword ptr [esp+18h]
  test eax,eax
  jne retained_volume

  mov byte ptr [esi+46h],bl

 retained_volume:
  mov ecx,dword ptr [esi+20h]
  mov eax,dword ptr [ecx+8]
  test eax,eax
  pop ebx
  je done_volume
  cmp dword ptr [esi],0
  je done_volume
  push esi
  call predicate_006054e0
  test eax,eax
  jne selected_gain
  mov eax,dword ptr [esp+14h]
  test eax,eax
  jne selected_gain
  mov ecx,dword ptr [esi+20h]
  push 1
  push edi
  call gain_005d6b10
  fstp dword ptr [esp+10h]
  mov ecx,dword ptr [esi+20h]
  push 0
  push edi
  call gain_005d6b10
  fcomp dword ptr [esp+10h]
  fnstsw ax
  test ah,44h



  jnp gain_zero

  mov ebp,1
  jmp selected_gain
 gain_zero:
  xor ebp,ebp
 selected_gain:
  mov ecx,dword ptr [esi+20h]
  push ebp
  push edi
  call gain_005d6b10
  movzx ecx,byte ptr [esi+50h]
  mov edx,dword ptr [esi]
  mov dword ptr [esp+10h],ecx
  fild dword ptr [esp+10h]
  fmul st,st(1)
  fmul dword ptr [scale_0073ee00]
  fimul dword ptr [esp+18h]
  fmul dword ptr [scale_0073ee00]
  fstp dword ptr [esp+10h]
  mov eax,dword ptr [esp+10h]
  push eax
  fstp st(0)
  push eax
  push edx
  call dword ptr [AIL_set_stream_volume_levels]
  mov eax,dword ptr [esp+10h]
  mov ecx,esi
  mov dword ptr [esi+54h],eax
  call refresh_005dc560
 done_volume:
  pop edi
  pop esi
  pop ebp
  ret 0ch
 }
}
}
namespace sound_volume29 {
__declspec(naked) void __fastcall priority_005dcd50(void*,void*,std::uint32_t) {
 __asm {
  mov dl,byte ptr [esp+4]
  push esi
  mov esi,ecx
  mov eax,dword ptr [esi+20h]
  mov ecx,dword ptr [eax+4ch]
  movzx eax,dl
  lea eax,[eax+eax*2]
  push edi
  lea edi,[ecx+eax*8]
  test edi,edi
  je old_group

  mov byte ptr [esi+8],dl

  jmp found_group
 old_group:
  movzx eax,byte ptr [esi+8]
  lea edx,[eax+eax*2]
  lea edi,[ecx+edx*8]
 found_group:
  mov al,byte ptr [edi+7]
  xor ecx,ecx
  mov cl,byte ptr [esi+46h]
  push 3f800000h
  push 0
  mov byte ptr [esi+50h],al
  push ecx
  mov ecx,esi
  call volume_005dc930







  fld dword ptr [edi+8]
  fld dword ptr [edi+0ch]
  pop edi
  fstp dword ptr [esi+48h]
  fstp dword ptr [esi+4ch]

  pop esi
  ret 4
 }
}
}
namespace table29_required {
__declspec(naked) void __fastcall gui_priority_005d5ba0(void*,void*,std::uint32_t) {
 __asm {
  mov ecx,dword ptr [ecx]
  test ecx,ecx
  je null_outer
  jmp priority_005dcd50
 null_outer:
  ret 4
 }
}
}

