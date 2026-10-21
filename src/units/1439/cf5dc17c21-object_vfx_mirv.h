#pragma once
#include <cstdint>
extern "C" {
void __fastcall object_vfx_006a4240(void*,void*,std::uint32_t);
// Original leaves the genuine FTOL EDX:EAX pair intact, then RET8.
std::uint64_t __fastcall object_vfx_duration_0063d010(void*,void*,const void*,std::uint32_t);
}
