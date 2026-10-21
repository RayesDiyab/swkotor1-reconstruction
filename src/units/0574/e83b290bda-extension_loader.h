#pragma once
#include <cstddef>
#include <cstdint>
#include "37d96ffda3-render_pool.h"
namespace texture { struct Basic; }
namespace kotor_renderer_init {
using U32=std::uint32_t;using I32=std::int32_t;using U8=std::uint8_t;
using RawProcedure=void(__stdcall*)();
using LoadProcedure=RawProcedure(__stdcall*)(const char*);
using StringProcedure=const char*(__stdcall*)(U32);
using ExtensionString=const char*(__stdcall*)(void*);
// Revision (lane-renderer-bodies, D13 round): the pool receiver is the real
// polymorphic kotor_renderer_gl::RequestPoolManager (fork F1, render_pool.h);
// 0043751D `jmp [edx+2Ch]` is its slot-11 virtual RestorePoolManager.
using PoolReceiver=kotor_renderer_gl::RequestPoolManager;
// Dispatch cells 007A47E0/007A6820/007A47DC hold typed function addresses
// (immediates at 004372A5..004372B9 / 004373CC..004373E0).
using EnableDispatch=void(__cdecl*)(U32,texture::Basic*,texture::Basic*,texture::Basic*,texture::Basic*);
using ConstantDispatch=void(__cdecl*)(const float*,float);
struct CapabilityRow { const char* name;const U32* mask;bool wgl; };
struct ProcedureRow { const char* name;void* cell; };
struct ExtensionLoaderBindings {
    const LoadProcedure* load_iat_0073d310;const StringProcedure* string_iat_0073d290;
    const char* extension_name_0073f45c;void* extension_cell_007bb7ec;
    const CapabilityRow* capability_rows_28;const ProcedureRow* procedure_rows_201;
    U32* capabilities_007bb85c;const U32* renderer_mask_0078e49c;
    const char* special_renderer_0073f52c;U8* renderer_flag_0078e398;
    float* anisotropy_007a6858;const U32* ati_mask_0078e4d0;
    EnableDispatch* enable_007a47e0;ConstantDispatch* constant0_007a6820;ConstantDispatch* constant1_007a47dc;
    EnableDispatch ati_enable,nv_enable;ConstantDispatch ati_constant0,nv_constant0,ati_constant1,nv_constant1;
    const char* first_base_0078f600;const char* second_base_00791240;
    const char* ati_suffix_0078fe18;const char* nv_suffix_0078fd78;
    char** first_program_0083028c;char** second_program_00830290;
    const I32* programs_0078e5ec;U32* vertex_enum_0078d46c;  // D5: I32 word
    PoolReceiver* const* pool_007a68a4;
};
extern ExtensionLoaderBindings& extension_loader;
// 004364C0 call [0073D2DC]: the existing M41 owner aurora_context_adoption::current_dc_0073d2dc (U32).
extern void __stdcall glGetFloatv_0073d218(U32,float*);
extern U8 __cdecl CheckExtension_0045f980(const char*,const char*);
// 006FBD00 is CRT _strstr (Ghidra LIBRARY_FUNCTION=y), called at 00437258.
extern const char* __cdecl StringSearch_006fbd00(const char*,const char*);
extern I32 __stdcall AnisotropicAvailable_0045f8e0();
extern void __stdcall InitAttenuationTexture_004263c0();
extern void __stdcall InitializeATIFragmentShaders_004794e0();
extern U32 __stdcall VertexBufferAvailable_0045fc90();
extern void __stdcall InitVertexBufferPool_00436400();
extern void __stdcall InitSystemPool_00436390();
void __stdcall LoadExtensions_00436490();
}
