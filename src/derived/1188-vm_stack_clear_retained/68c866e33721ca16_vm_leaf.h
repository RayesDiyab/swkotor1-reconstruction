#pragma once
#include <cstdint>
namespace kotor::vm_leaf {
// Selected state only, with original VM-internal offsets recorded. This host
// representation is not the original layout and owns none of its pointed data.
struct Internal {
    std::int32_t return_type{};       // +1C
    std::uint32_t return_bits{};      // +20: raw 32-bit output, not a host pointer
    std::int32_t depth{};             // +1AC (stack +00), signed test
    std::int32_t base_pointer{};      // +1B0 (stack +04), preserved; export name
    std::uint32_t capacity{};         // +1B4 (stack +08), preserved by pops
    std::uint8_t* tags{};             // +1B8 (stack +0C), one byte per element
    std::uint32_t* words{};           // +1BC (stack +10), raw 32-bit elements
    Internal* stack_owner{};          // +1C0 (stack +14), export VM back-pointer
    void* command_implementer{};      // +3D4
};
struct Machine { Internal* internal; }; // original wrapper has 32-bit pointer +00
using InitializeSlot1 = void (*)(void* implementer);
// Matching live output pointers are required only on successful branches.
// State/arrays/output objects must not alias. Positive depth requires that the
// top tag is readable; a matching tag also requires its word to be readable.
std::uint32_t return_value_005d1820(const Internal&, std::int32_t* type,
                                  std::uint32_t* bits) noexcept;
void set_implementer_005d1a40(Internal&, void*, InitializeSlot1);
std::uint32_t pop_integer_005d24c0(Internal&, std::int32_t* output) noexcept;
std::uint32_t pop_object_005d26a0(Internal&, std::uint32_t* output) noexcept;
std::uint32_t pop_integer_005d1000(Machine&, std::int32_t* output) noexcept;
std::uint32_t pop_object_005d10c0(Machine&, std::uint32_t* output) noexcept;
}
