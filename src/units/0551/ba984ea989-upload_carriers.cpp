// lane-upload U-D1 naked carriers (D21 style, same shape as lane-texture-epoch texture_x86.asm carriers).
// Each reloads the receiver's live vtable pointer and tail-jumps the slot with ECX = receiver and the
// caller's return address; zero stack words; the slot's own `ret` returns to the caller with its EAX.
// No C++ function pointer is formed, so no function-pointer conversion exists.
#include "b195664429-upload.h"

extern "C" {
// Texture +04 (Basic 00641DB0 self_return / Reference 0063E7F0 return_zero): original call sites
// 004214DC, 004214F0, 00421529, 00420C13 and every getter prologue (e.g. 0041EEFA).
__declspec(naked) font::Texture* __fastcall upload_call_query_04(font::Texture*){
 __asm {
  mov eax,dword ptr [ecx]
  jmp dword ptr [eax+4]
 }
}
// Controller (+34 object) slot +04: Bind 0042153C..0042153E (mov eax,[ecx]; call [eax+4]).
__declspec(naked) void __fastcall upload_controller_call_04(texture_upload::U){
 __asm {
  mov eax,dword ptr [ecx]
  jmp dword ptr [eax+4]
 }
}
// Controller slot +10: glImage 00420A6E..00420A70 (mov edx,[ecx]; call [edx+10h]).
__declspec(naked) void __fastcall upload_controller_call_10(texture_upload::U){
 __asm {
  mov edx,dword ptr [ecx]
  jmp dword ptr [edx+10h]
 }
}
}
