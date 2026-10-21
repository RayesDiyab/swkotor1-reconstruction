#pragma once
// Declarations for the additional entries of the single M40 GL/WGL/Win32 adapter
// owner (aurora-reviewed/adapters/windows_gl_imports.cpp, kotor_renderer_init).
// The aa_probe entries (GetStockObject/RegisterClassA/CreateWindowExA/
// DestroyWindow/glGetString/GetVersionExA) keep their existing declarations in
// the owner-accepted canonical aa_probe.h. Every definition calls the real
// opengl32/gdi32/user32/kernel32 import named by the original IAT cell suffix
// (original IAT map: dumpbin /imports swkotor.exe, see lane NOTES.md).
#include "f3f4667038-renderer_initialize.h"
namespace kotor_renderer_init {
// Soft-shadow direct calls (original call dword ptr [IAT] sites in 00426CC0).
extern void __stdcall glGenTextures_0073d24c(I32, U32*);
extern void __stdcall glBindTexture_0073d240(U32, U32);
extern void __stdcall glCopyTexImage2D_0073d2d8(U32, I32, U32, I32, I32, U32, U32, I32);
extern void __stdcall glTexParameteri_0073d23c(U32, U32, I32);
extern I32 __stdcall wglShareLists_0073d2b4(void*, void*);
// Loader prerequisite (00436490 anisotropy query); no consumer in this cohort.
extern void __stdcall glGetFloatv_0073d218(U32, float*);
// IAT cells that the original reads as values (mov reg,[IAT]) rather than calls
// directly. Each cell holds the address of the adapter that calls the import.
using IatWindowProcedure = I32 (__stdcall*)(void*, U32, U32, I32);
using IatRawProcedure = void (__stdcall*)();
using IatLoadProcedure = IatRawProcedure (__stdcall*)(const char*);
using IatStringProcedure = const char* (__stdcall*)(U32);
extern I32 __stdcall DefWindowProcA_0073d3b0(void*, U32, U32, I32);
extern IatRawProcedure __stdcall wglGetProcAddress_0073d310(const char*);
extern const IatWindowProcedure iat_DefWindowProcA_0073d3b0;
extern const IatLoadProcedure iat_wglGetProcAddress_0073d310;
extern const IatStringProcedure iat_glGetString_0073d290;
// Writable-typed IAT-value cells read by 00427490/00427C90 as `mov reg,[IAT]`.
// Each holds the adapter that calls the real import (no IAT pun, no casts).
extern void (__stdcall* iat_glEnable_0073d390)(U32);
extern void (__stdcall* iat_glDisable_0073d22c)(U32);
extern void (__stdcall* iat_glGenTextures_0073d24c)(I32, U32*);
extern void (__stdcall* iat_glBindTexture_0073d240)(U32, U32);
extern void (__stdcall* iat_glTexParameteri_0073d23c)(U32, U32, I32);
extern void (__stdcall* iat_glTexImage2D_0073d280)(U32, I32, I32, I32, I32, I32, U32, U32, const void*);
extern void (__stdcall* iat_glTexImage1D_0073d2f0)(U32, I32, I32, I32, I32, U32, U32, const void*);
extern void (__stdcall* iat_glCopyTexImage2D_0073d2d8)(U32, I32, U32, I32, I32, U32, U32, I32);
extern I32 (__stdcall* iat_wglMakeCurrent_0073d2b0)(void*, void*);
extern void* (__stdcall* iat_wglCreateContext_0073d2b8)(void*);
extern I32 (__stdcall* iat_wglShareLists_0073d2b4)(void*, void*);
}
