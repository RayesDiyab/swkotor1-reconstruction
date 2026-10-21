#pragma once
#include <cstdint>
namespace kotor_renderer_init {
using ATIWord=std::uint32_t;
using ATIGen=ATIWord(__stdcall*)(ATIWord);
using ATINullary=void(__stdcall*)();
using ATIUnary=void(__stdcall*)(ATIWord);
using ATIThree=void(__stdcall*)(ATIWord,ATIWord,ATIWord);
using ATIColor1=void(__stdcall*)(ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord);
using ATIColor2=void(__stdcall*)(ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord);
using ATIColor3=void(__stdcall*)(ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord);
using ATIAlpha1=void(__stdcall*)(ATIWord,ATIWord,ATIWord,ATIWord,ATIWord,ATIWord);
// All targets are borrowed. Main publishes the descriptor only after typed live cells exist.
// Function-pointer objects are volatile and reloaded for every original indirect call.
// Shader IDs are separate scalar lifetimes; no contiguous-array inference is made.
struct ATISetupBindings {
 ATIWord* shader_007a692c;
 ATIWord* shader_007a6924;
 ATIWord* shader_007a6920;
 ATIWord* shader_007a691c;
 ATIWord* shader_007a6918;
 ATIWord* shader_007a690c;
 ATIWord* shader_007a6910;
 ATIWord* shader_007a6914;
 ATIUnary const volatile* glEnable_0073d390;
 ATIUnary const volatile* glDisable_0073d22c;
 ATIGen volatile* glGenFragmentShadersATI_007bb754;
 ATIUnary volatile* glBindFragmentShaderATI_007bb6cc;
 ATINullary volatile* glBeginFragmentShaderATI_007bb7f4;
 ATINullary volatile* glEndFragmentShaderATI_007bb554;
 ATIThree volatile* glSampleMapATI_007bb620;
 ATIThree volatile* glPassTexCoordATI_007bb804;
 ATIColor2 volatile* glColorFragmentOp2ATI_007bb840;
 ATIColor3 volatile* glColorFragmentOp3ATI_007bb7fc;
 ATIColor1 volatile* glColorFragmentOp1ATI_007bb6d4;
 ATIAlpha1 volatile* glAlphaFragmentOp1ATI_007bb710;
};
extern const ATISetupBindings& ati_setup;
void __cdecl SetupATIFrameBuffer_00427490();
}
