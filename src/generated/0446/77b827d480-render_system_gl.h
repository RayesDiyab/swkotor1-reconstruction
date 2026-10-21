#pragma once
#include <cstdint>
#include "502f4dd31a-peer-system.h"
namespace aurora_context_adoption {
std::uint32_t __stdcall current_context_0073d2e0();
std::uint32_t __stdcall current_dc_0073d2dc();
// The integrator retains both descriptor and canonical cells for every call.
renderer_system::Views make_live_system_view(
    volatile std::uint32_t& context,volatile std::uint32_t& dc,
    volatile std::uint32_t (&cache)[6],volatile std::uint32_t& zero) noexcept;
}
