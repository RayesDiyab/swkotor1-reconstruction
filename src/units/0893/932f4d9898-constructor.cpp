#include "8b5c506ddb-constructor.h"
#include "b593572262-m50_vtables.h"   // lane-m50-vtables W-4: typed CGameObject_vtable 007457A0



namespace table29_required {
// Exact instruction source for 004D7D60..004D7D78. 004D7D6A mov [eax],007457A0h
// stores the CGameObject vtable; lane-m50-vtables (W-4) stores the typed
// CGameObject table kotor_vtable_cgameobject_007457a0 (relocated address, same
// 28 slots) instead of the unmapped original word. Callers that construct a
// derived class (CSWCModule 00643F40) overwrite it immediately, as the original.
__declspec(naked) void* __fastcall gui_game_object_004d7d60(
 void*,void*,std::uint32_t,std::uint32_t) {
 __asm {
  mov dl, byte ptr [esp+4]
  mov eax, ecx
  mov ecx, dword ptr [esp+8]



  mov dword ptr [eax], offset kotor_vtable_cgameobject_007457a0




  mov dword ptr [eax+4], ecx





  mov byte ptr [eax+8], dl










  ret 8

 }
}
}
namespace kotor_binding_required {
using table29_required::gui_game_object_004d7d60;
// Retained116's exact named port is a source-owned tail alias of the same body.
// It contributes no second recovered-address credit or private provider table.
__declspec(naked) void* __fastcall game_object_004D7D60(
 void*,void*,std::uint32_t,std::uint32_t) {
 __asm {jmp gui_game_object_004d7d60}
}
}
