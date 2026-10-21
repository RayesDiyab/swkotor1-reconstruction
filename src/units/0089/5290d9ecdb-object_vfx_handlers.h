#pragma once
#include <cstdint>
// Shared declarations only. Original address ownership is assigned by root.
extern "C" {
#define OBJECT_VFX_ONE(name) void __fastcall object_vfx_##name(void*,void*,std::uint32_t)
OBJECT_VFX_ONE(006a1220);OBJECT_VFX_ONE(006a14e0);OBJECT_VFX_ONE(006a15d0);
OBJECT_VFX_ONE(006a1610);OBJECT_VFX_ONE(006a34b0);OBJECT_VFX_ONE(006a3660);
OBJECT_VFX_ONE(006a3740);OBJECT_VFX_ONE(006a4720);OBJECT_VFX_ONE(006a47b0);
OBJECT_VFX_ONE(006a47f0);OBJECT_VFX_ONE(006a04c0);OBJECT_VFX_ONE(006a4a80);
OBJECT_VFX_ONE(006a38e0);OBJECT_VFX_ONE(006a4830);OBJECT_VFX_ONE(006a3e80);
OBJECT_VFX_ONE(006a4920);OBJECT_VFX_ONE(006a4c00);OBJECT_VFX_ONE(006a4580);
OBJECT_VFX_ONE(006a2e10);OBJECT_VFX_ONE(006a4680);OBJECT_VFX_ONE(006a3fd0);
OBJECT_VFX_ONE(006a05b0);OBJECT_VFX_ONE(006a4000);OBJECT_VFX_ONE(006a4060);
OBJECT_VFX_ONE(006a5920);
// Optional selector domains are OPEN, with no implementation credit or adoption.
OBJECT_VFX_ONE(006a2af0);OBJECT_VFX_ONE(006a30e0);OBJECT_VFX_ONE(006a4240);
#undef OBJECT_VFX_ONE
void __fastcall object_vfx_006a1470(void*,void*);
void __fastcall object_vfx_006a1570(void*,void*);
void __fastcall object_vfx_006a0df0(void*,void*);
void __fastcall object_vfx_006a49a0(void*,void*,std::uint32_t,std::uint32_t);
void __fastcall object_vfx_006a4e70(void*,void*,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall object_vfx_006a4fa0(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_006a0680(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_006a0800(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_006a0860(void*,void*,std::uint32_t);
}
