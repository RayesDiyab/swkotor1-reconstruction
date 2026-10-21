#pragma once
#include <cstdint>

// Declaration-only prerequisites. These are not definitions of canonical cells.
// Main must alias the real objects; the descriptor investigation records the
// current storage/type gaps. No guessed defaults or copied state live here.
namespace kotor_renderer_validation_required {
extern volatile std::uint32_t& cache_0078e508;
extern const volatile std::uint32_t& mask_0078e4d0;
extern const volatile std::uint32_t& capabilities_007bb85c;
extern const volatile std::uint32_t& control_0078e4fc;
extern const volatile std::uint32_t& mask_0078e4f4;
}
