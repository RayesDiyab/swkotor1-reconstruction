#pragma once
#include <cstdint>
namespace kotor_renderer_init {
using FBWord=std::uint32_t;using FBByte=std::uint8_t;
using FBGen=void(__stdcall*)(int,FBWord*);using FBBind=void(__stdcall*)(FBWord,FBWord);
using FBParam=void(__stdcall*)(FBWord,FBWord,int);using FBTex2=void(__stdcall*)(FBWord,int,int,int,int,int,FBWord,FBWord,const void*);
using FBTex1=void(__stdcall*)(FBWord,int,int,int,int,FBWord,FBWord,const void*);using FBCopy=void(__stdcall*)(FBWord,int,FBWord,int,int,FBWord,FBWord,int);
using FBCurrent=int(__stdcall*)(void*,void*);using FBCreate=void*(__stdcall*)(void*);using FBShare=int(__stdcall*)(void*,void*);
using FBEnable=void(__stdcall*)(FBWord);using FBPbufferDC=void*(__stdcall*)(void*);using FBProgramString=void(__stdcall*)(FBWord,FBWord,int,const void*);
// Revision (lane-renderer-bodies): height borrows the M41 volatile window cell; FBCopy
// matches the single owner's glCopyTexImage2D_0073d2d8 adapter (width/height U32).
// Borrowed descriptor only. The four indexed objects require genuine six-element lifetimes.
// Each ramp/bloom target is a separately borrowed scalar, with no adjacency assumption.
struct FrameBufferBindings {
 FBWord* gate_0078d98c;FBByte* enabled_0078daf4;float* intensity_0078daec;
 FBWord* width_0078d1d4;const volatile FBWord* height_0078d1d8;FBWord* root_texture_007a687c;
 FBWord* textures_007a47c4;void** dc_007a47ec;void** contexts_007a4804;void** pbuffers_007a6824;
 const volatile FBWord* main_dc_007a47e4;const volatile FBWord* main_context_007a6854;FBByte* active_007a6860;
 FBWord* ramps[3];FBWord* bloom_ramps[3];
 FBWord* power_width_007a6890;FBWord* power_height_007a6894;
 FBByte* bloom_active_0078d990;FBWord* bloom_texture_007a68c0;
 void** bloom_pbuffer_007a68c4;void** bloom_dc_007a68c8;void** bloom_context_007a68cc;
 FBWord* program_007a68d4;const char* source_0078de48;const char* source_0078db80;
 const float* half_0073e9ac;const float* inv255_0073ede0;const float* scale255_0073ede4;const double* threshold_0073f3d0;
 const float* quadratic_0073f404;const float* ramp40_0073f408;const float* ramp_scale_0073f40c;const float* ramp22_0073f410;
 FBGen volatile* glGen_0073d24c;FBBind volatile* glBind_0073d240;FBParam volatile* glParam_0073d23c;
 FBTex2 volatile* glTex2_0073d280;FBTex1 volatile* glTex1_0073d2f0;FBCopy volatile* glCopy_0073d2d8;
 FBCurrent volatile* wglCurrent_0073d2b0;FBCreate volatile* wglCreate_0073d2b8;FBShare volatile* wglShare_0073d2b4;
 FBEnable volatile* glEnable_0073d390;FBEnable volatile* glDisable_0073d22c;
 FBPbufferDC volatile* getPbufferDC_007bb6b8;FBGen volatile* genPrograms_007bb528;FBBind volatile* bindProgram_007bb788;
 FBProgramString volatile* programString_007bb580;
};
extern const FrameBufferBindings& framebuffer;
void* __cdecl CreatePBuffer_00426560(FBWord,FBWord);
FBWord __cdecl FrameBufferAvailable_0045f820();FBWord __cdecl ATIFrameBufferAvailable_0045f860();
void __cdecl SetupATIFrameBuffer_00427490();void __cdecl ReportARBError_0047a2c0(const char*);
void __cdecl InitializeFrameBufferModifications_00427c90();
}
// Existing Task25/Main31 provider; assembly-only ST0 input, EDX:EAX output, ST0 consumed.
// No new x87 implementation or alias is owned by this package.
extern "C" unsigned __int64 __cdecl unresolved_ftol2_006fae8c();
