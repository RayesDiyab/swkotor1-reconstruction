#pragma once
#include <cstdint>
namespace renderer_system {
using U32=std::uint32_t;using Getter=U32 (__stdcall*)();
struct Views { Getter get_context_0073d2e0,get_dc_0073d2dc;volatile U32 *context_007a6854,*dc_007a47e4,*cache6_007a683c,*zero_007a6898; };
extern Views& bindings() noexcept;
}
extern "C" std::uint32_t __cdecl renderer_system_00425c30();
