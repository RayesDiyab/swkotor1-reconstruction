#pragma once
#include <cstdint>
namespace renderer_texture {
using U32=std::uint32_t;
struct Views { const volatile U32 *initialized_007b90f0,*memory_0078d3f4;void (__cdecl* free_font_0044cc60)();void (__cdecl* init_font_0044cc40)();U32 (__cdecl* set_memory_00421d90)(U32); };
extern Views& bindings() noexcept;
}
extern "C" std::uint32_t __cdecl renderer_texture_00422360();
