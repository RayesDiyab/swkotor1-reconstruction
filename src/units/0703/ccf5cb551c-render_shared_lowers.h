#pragma once
// Shared renderer lowers owned by fork F2 (lane-renderer-bodies). F3 references
// these exact declarations. All cdecl (caller pops; plain RET at each body end).
#include <cstdint>
namespace texture {
struct Basic;  // CAurTexture object; layout owned by lane-texture-epoch E1 texture.hpp
extern "C" {
// lane-texture-epoch texture_x86.asm carrier: [vtable+0C] (0041EBF0 IsCubeMap in Basic table), AL only.
std::uint8_t __fastcall texture_call_flag_0c_al(Basic*);
// fork-f2 src/texture_bind_b4.asm (single owner): [vtable+B4] Bind (004214B0), zero stack, result ignored.
void __fastcall texture_call_bind_b4(Basic*);
}
}
namespace kotor_renderer_gl {
// 0041FE80 GLRender::ActivateTextureStage: [esp+4] stage -> 007A6898 (0041FE84);
// if MultitextureAvailable (0045F710) != 0, glActiveTextureARB(007A6898+84C0h)
// via slot 007BB80C (0041FE92..0041FE9F). EAX on return is not meaningful.
void __cdecl ActivateTextureStage_0041fe80(std::uint32_t stage);
// 00423D80 GLRender::MakeLookUpTexture: one float stack argument ([esp+4]).
void __cdecl MakeLookUpTexture_00423d80(float exponent);
// 004AAAB0 Homogenize: copies 3 dwords from xyz and 1 dword from w into the
// static float[4] at 00831F18 and returns its address (004AAAD9).
float* __cdecl Homogenize_004aaab0(const float* xyz, const float* w);
// 0045F710 (multitexture predicate; cache 0078E514, mask 0078E484 = ARB_multitexture).
std::uint32_t __cdecl MultitextureAvailable_0045f710();
}
