#pragma once
#include <cstdint>
struct ObjectVfxPosition;
extern "C" {
// Original full selector carrier is unused by this body. RET 4, @12.
void __fastcall object_vfx_006a30e0(void*,void*,std::uint32_t);
std::uint32_t __fastcall object_vfx_0060ed80(void*,void*,std::uint32_t);
// Raw x87 ST0 return; no argument or return materialization in the caller.
double __fastcall object_vfx_006109b0(void*,void*);
std::uint32_t __fastcall object_vfx_0064b4d0(void*,void*,const ObjectVfxPosition*);
}
