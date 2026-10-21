#include "91ebd12484-providers.h"
namespace gui_viewport_list {
// Authored instruction-level reconstruction, with externally supplied cells;
// never executes or embeds the original binary. Preserve ambient x87 precision.
__declspec(naked) void __cdecl product_00458ef0(){
 __asm {
  mov edx,viewport
  mov eax,[edx]
  movsx ecx,word ptr [eax]
  mov eax,[edx+8]
  fld dword ptr [eax]
  mov edx,[edx+4]
  xor eax,eax
  test ecx,ecx
  jl done
 multiply:
  fmul dword ptr [edx+eax*4]
  inc eax
  cmp eax,ecx
  jle multiply
 done:
  ret
 }
}
}
