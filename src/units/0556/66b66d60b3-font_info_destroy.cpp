// 00420610..0042066C CAurFontInfo::~CAurFontInfo (lane-font packet 5).
// Evidence: lane-font/disasm/00420610.asm.txt. The original SEH frame (state 0 -> -1 between
// the two frees, 00420632/0042064A) only covers non-throwing free calls and is not reproduced.
// Naked body transports the raw EAX the original leaves: 0 when [+24] and [+18] are both null,
// otherwise whatever the last 006FA390 free call returned (live_free is that allocator family).
#include "43e0af9c1e-font_txi.hpp"
extern "C" __declspec(naked) font_txi::U __fastcall font_info_destroy_00420610(font_txi::FontInfo*,void*){
 __asm {
  push esi
  mov esi,ecx                 // 00420627
  mov eax,dword ptr [esi+24h] // 0042062D lowerright data (words[9])
  test eax,eax
  je skip_lower               // 0042063A
  push eax
  call live_free              // 0042063D call 006FA390
  add esp,4
 skip_lower:
  mov esi,dword ptr [esi+18h] // 00420645 upperleft data (words[6])
  test esi,esi
  je skip_upper               // 00420652
  push esi
  call live_free              // 00420655 call 006FA390
  add esp,4
 skip_upper:
  pop esi
  ret                         // 0042066C RET0
 }
}
