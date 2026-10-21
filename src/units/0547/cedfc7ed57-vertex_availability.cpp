#include "112afbaf4f-vertex_availability.h"
namespace kotor_renderer_init {
std::int32_t __cdecl VertexProgramsAvailable_0045f770() {
    std::uint32_t cached = *availability.cache_0078e51c;
    if (cached == 0xffffffffu) {
        const std::uint32_t required = *availability.required_0078e4d4;
        cached = ((*availability.capabilities_007bb85c & required) == required) ? 1u : 0u;
        *availability.cache_0078e51c = cached;
    }
    return (cached & *availability.enabled_0078e5ec) != 0 ? 1 : 0;
}
}
