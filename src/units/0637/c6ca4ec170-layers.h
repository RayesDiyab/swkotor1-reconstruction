#pragma once
#include <cstdint>
#include "85f8a6470f-menu_draw.h"
namespace aurora_gui {
using U32=std::uint32_t;using U16=std::uint16_t;using I32=std::int32_t;
// Borrowed canonical storage only; no cells/defaults are defined by this packet.
struct Cells {
    volatile U32* gate_007bb4d0;
    volatile U32 *width_0078d1d4,*height_0078d1d8;
    volatile short* index_007b9460;
    volatile U16* frames5_007b9468;
    volatile float* alpha_007b9248;
    const volatile U32 *lighting_0078d45c,*depth_0078d474,*blend_0078d448,*texture_0078d458;
};
// Complete descriptor acquired from one genuinely constructed retained owner.
// No static Views address binding or zero/partial descriptor is published.
const Cells& cells() noexcept;
void __stdcall get_integer(U32,I32*);
void __stdcall matrix_mode(U32);
void __stdcall push_matrix();void __stdcall pop_matrix();void __stdcall load_identity();
void __stdcall ortho(double,double,double,double,double,double);
void __stdcall blend_func(U32,U32);
void __stdcall viewport(I32,I32,I32,I32);
void __stdcall begin(U32);void __stdcall vertex4f(float,float,float,float);void __stdcall end();
void __stdcall clear(U32);void __stdcall color4ubv(const unsigned char*);
}
namespace kotor_renderer_init {void __stdcall glEnable_0073d390(std::uint32_t);void __stdcall glDisable_0073d22c(std::uint32_t);}
extern "C" void __cdecl aurora_viewport_00459170(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
extern "C" void __cdecl aurora_color_00459190(std::uint32_t);
extern "C" std::uint32_t __cdecl aurora_vector_not_equal_0043e1f0(const void*,const void*);
extern "C" std::uint32_t __cdecl aurora_vector_equal_004aa980(const void*,const void*);
