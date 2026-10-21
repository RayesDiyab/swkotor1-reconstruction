#pragma once
#include <cstddef>
#include <cstdint>
namespace kotor_renderer_cells {
// Existing saved explicit global/type annotation, not a count from padding.
constexpr std::size_t viewport_capacity=10;
extern float viewport_alphas_007b9248[viewport_capacity];
extern std::uint16_t viewport_frame_words_007b9468[viewport_capacity*5];
// Proposed sole signed16 index remains the R1 object; no second definition.
extern short viewport_index_007b9460;
// Same genuine U16[50] owner; element2 is width0, element3 is height0.
// Draw uses +5*index and word axes0/1. No nominal or character-pointer cast.
const volatile std::uint16_t* BorrowFrameDimensionWords() noexcept;
}
