#pragma once
#include <cstdint>
#include "7d0aafcc35-renderer_gl_imports.h"
namespace kotor_soft_shadow {
using U32=std::uint32_t;using I32=std::int32_t;using U8=std::uint8_t;
using GetDC=void*(__stdcall*)(void*);
using MakeContextCurrent=I32(__stdcall*)(void*,void*,void*);
using GenShader=U32(__stdcall*)(U32);
using One=void(__stdcall*)(U32);
using Zero=void(__stdcall*)();
using Three=void(__stdcall*)(U32,U32,U32);
using Ten=void(__stdcall*)(U32,U32,U32,U32,U32,U32,U32,U32,U32,U32);
using Nine=void(__stdcall*)(U32,U32,U32,U32,U32,U32,U32,U32,U32);
using Thirteen=void(__stdcall*)(U32,U32,U32,U32,U32,U32,U32,U32,U32,U32,U32,U32,U32);
using Twelve=void(__stdcall*)(U32,U32,U32,U32,U32,U32,U32,U32,U32,U32,U32,U32);
// Borrowed individual cells: no extent, initialization, state ownership or publication inferred.
struct Bindings {
    U8* enabled_0078dae9;U32* requested_0078e420;
    // Revision: borrowed read-only views; 0078D1D8 is the M40 window-cell owner's volatile U32.
    const volatile U32* width_0078d1d4;const volatile U32* height_0078d1d8;
    U32* scene_007a687c;U32* scratch_007a68f8;
    U32* power_width_007a6890;U32* power_height_007a6894;
    // Revision: same volatile U32 handle words as renderer_system::Views and kotor_pbuffer.
    const volatile U32* main_dc_007a47e4;const volatile U32* main_context_007a6854;
    void** buffer1_007a68e8;void** dc1_007a68ec;void** context1_007a68f0;U32* texture1_007a68f4;
    void** buffer2_007a68fc;void** dc2_007a6900;void** context2_007a6904;U32* texture2_007a6908;
    U32* shader_007a6928;
    GetDC volatile* get_dc_007bb6b8;
    MakeContextCurrent volatile* make_current_007bb818;
    GenShader volatile* gen_shader_007bb754;One volatile* bind_shader_007bb6cc;
    Zero volatile* begin_007bb7f4;Three volatile* sample_007bb620;
    Ten volatile* color2_007bb840;Nine volatile* alpha2_007bb5e8;
    Thirteen volatile* color3_007bb7fc;Twelve volatile* alpha3_007bb680;
    Zero volatile* end_007bb554;
};
extern const Bindings& bindings;
U32 __cdecl SoftAvailable_0045f7b0();
U32 __cdecl ATISoftAvailable_0045f7e0();
void* __cdecl CreatePBuffer_00426560(U32,U32);
void __cdecl PowerExtent_00427450(U32,U32,U32*,U32*);
// Revision: the nine GL/WGL imports are the single kotor_renderer_init adapter
// owner's entries (windows_gl_imports.cpp); no kotor_soft_shadow import names exist.
using kotor_renderer_init::glGenTextures_0073d24c;
using kotor_renderer_init::glBindTexture_0073d240;
using kotor_renderer_init::glCopyTexImage2D_0073d2d8;
using kotor_renderer_init::glTexParameteri_0073d23c;
using kotor_renderer_init::wglCreateContext_0073d2b8;
using kotor_renderer_init::wglShareLists_0073d2b4;
using kotor_renderer_init::wglMakeCurrent_0073d2b0;
using kotor_renderer_init::glEnable_0073d390;
using kotor_renderer_init::glDisable_0073d22c;
}
namespace kotor_renderer_init {void __cdecl InitializeSoftShadows_00426cc0();}
