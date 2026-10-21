#pragma once
#include <cstdint>
namespace kotor_renderer_init {
struct VertexAvailabilityBindings {
    std::uint32_t* cache_0078e51c;
    const std::uint32_t* required_0078e4d4;
    const std::uint32_t* capabilities_007bb85c;
    const std::int32_t* enabled_0078e5ec;
};
extern VertexAvailabilityBindings& availability;
std::int32_t __cdecl VertexProgramsAvailable_0045f770();
}
