// texture_capability.cpp - lane-texture-epoch PROPOSAL: AurTextureCompressionAvailable 0045F950.
// Original 0045F950..0045F97A (stdcall/no arguments, RET0, full EAX; caller 0041EB10/0041EB15 tests EAX):
//   0045F950 mov eax,[0078E53C]; 0045F955 cmp eax,-1; jne -> ret (cached value, full EAX)
//   0045F95A mov eax,[0078E4B8]; 0045F95F mov ecx,[0078E4BC]; 0045F965 or ecx,eax
//   0045F967 mov eax,[007BB85C]; 0045F96C and eax,ecx; 0045F96E sub eax,ecx; neg; sbb eax,eax; inc eax
//            => eax = ((existingExtensions & required) == required) ? 1 : 0
//   0045F975 mov [0078E53C],eax; 0045F97A ret
// Same shape as the reviewed lane-renderer-bodies 0045F7B0/0045F820 predicates; same capability owner.
#include "59377d41d2-texture_cells.hpp"

extern "C" acquisition::Word __stdcall acquisition_compression_available_0045f950(){
 using namespace texture_cells;
 acquisition::Word value=compression_cache_0078e53c;                         // 0045F950
 if(value!=0xffffffffu)return value;                                          // 0045F955..0045F958
 const acquisition::Word arb=*static_cast<const volatile std::uint32_t*>(&kotor_renderer_gl_procs::mask_0078e4b8);            // 0045F95A
 const acquisition::Word s3tc=*static_cast<const volatile std::uint32_t*>(&kotor_renderer_gl_procs::mask_0078e4bc);      // 0045F95F
 const acquisition::Word required=s3tc|arb;                                   // 0045F965
 const acquisition::Word existing=*static_cast<const volatile std::uint32_t*>(&kotor_renderer_gl_cells::capabilities_007bb85c); // 0045F967
 value=((existing&required)==required)?1u:0u;                                 // 0045F96C..0045F974
 compression_cache_0078e53c=value;                                            // 0045F975
 return value;
}
