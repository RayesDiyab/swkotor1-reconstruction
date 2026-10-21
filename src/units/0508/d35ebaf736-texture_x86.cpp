// texture_x86.cpp - lane-texture-epoch PROPOSAL (integrator D21: no ml.exe). Minimal physical transports as
// __declspec(naked) functions with MSVC x86 inline __asm, compiled by the pinned cl. No original bytes copied.
//  * get_image_wrapper_0041ea20: original 0041EA20 writes its own incoming argument cells ([esp+30h] at 0041EA29)
//    and passes the addresses of incoming arg5/arg6 cells (0041EA67/0041EA4D) to the processed callback. C++ cannot
//    name a parameter's caller-owned stack cell, so the naked entry passes &cell1 (= esp+4) to the typed C++ core.
//  * texture_call_*: zero-stack thiscall-shaped virtual calls; reload the live vtable at each call and tail-jump the
//    slot with ECX = receiver (callee returns directly to the C++ caller, RET0).
//  * texture_list_remove_00445080: D1 fastcall carrier for CExoArrayList::Remove 00445080 == task-14
//    ArrayMutations::RemoveReverse32 (ECX this, one stack word, RET4); EDX unused.
#include "f31f0dbce3-texture.hpp"
using acquisition::get_image_wrapper_core_0041ea20; // visible to inline __asm by plain name

extern "C" {
__declspec(naked) char* __cdecl get_image_wrapper_0041ea20(const char*,acquisition::Signed*,acquisition::Signed*,
    acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,acquisition::Signed*,float*,
    std::uint8_t volatile*,std::int16_t volatile*,acquisition::Signed){
 __asm {
  lea eax,[esp+4]                          // caller's first argument cell
  push eax
  call get_image_wrapper_core_0041ea20     // cdecl core(WrapperCells*), EAX = full pointer result
  add esp,4
  ret                                      // cdecl RET0 (0041EACB/0041EBCC)
 }
}
__declspec(naked) void __fastcall texture_call_free_b0(texture::Basic*){            // 0041F6A9..AD, 00421743..48
 __asm { mov eax,dword ptr [ecx] }
 __asm { jmp dword ptr [eax+0B0h] }
}
__declspec(naked) std::uint8_t __fastcall texture_call_flag_0c_al(texture::Basic*){ // 0041F8C5..CD
 __asm { mov eax,dword ptr [ecx] }
 __asm { jmp dword ptr [eax+0Ch] }
}
__declspec(naked) std::uint8_t __fastcall texture_call_processed_30_al(texture::Basic*){ // 00421738..3F
 __asm { mov eax,dword ptr [ecx] }
 __asm { jmp dword ptr [eax+30h] }
}
__declspec(naked) texture::U __fastcall texture_call_free_b0_eax(texture::Basic*){  // 0041F320..0041F324
 __asm { mov eax,dword ptr [ecx] }
 __asm { jmp dword ptr [eax+0B0h] }         // EAX of the slot returns to the C++ caller
}
}

namespace {
// Single-inheritance x86 member-function pointer: one code address (asserted). Read by the naked carrier only.
using RemoveMember=void (__thiscall kotor_reconstruction::ArrayMutations::*)(std::uint32_t);
static_assert(sizeof(RemoveMember)==4,"x86 single-inheritance member pointer is one code address");
const RemoveMember remove_reverse32_target=&kotor_reconstruction::ArrayMutations::RemoveReverse32;
}
extern "C" __declspec(naked) void __fastcall texture_list_remove_00445080(texture::List*,void*,texture::U){ // 00423090..95
 __asm { jmp dword ptr [remove_reverse32_target] }  // ECX=list, [esp+4]=value: thiscall RET4 contract
}
