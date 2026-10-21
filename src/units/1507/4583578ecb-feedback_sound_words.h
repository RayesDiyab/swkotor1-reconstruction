#pragma once
#include <cstdint>
namespace feedback {
// Defined in a separate translation unit: retain an ordinary potentially throwing call.
// Only this selector13 seam transports the original three position words without FP loads.
void __cdecl play_feedback_sound_words(void* reference, const void* position,
                                      std::uint32_t z_offset_bits) noexcept(false);
}
