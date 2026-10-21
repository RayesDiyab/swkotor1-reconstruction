#pragma once
#include <cstdint>
// Unique raw-word carrier for original MOV-only12-byte position by value.
struct ObjectVfxPosition { std::uint32_t x,y,z; };
static_assert(sizeof(ObjectVfxPosition)==12,"original position stack carrier");
extern "C" {
void* __fastcall object_vfx_006a09e0(void* self,void* unused_edx);
std::uint32_t __fastcall object_vfx_006a1880(void* self,void* unused_edx,
    std::uint32_t effect,std::uint32_t arg1,std::uint32_t target_id,
    std::uint32_t arg2,std::uint32_t arg3,std::uint32_t arg4,
    std::uint32_t arg5,ObjectVfxPosition position);
}
