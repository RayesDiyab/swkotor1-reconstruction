#pragma once
#include "db1c5eeda2-resources.h"
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace ref_00405fe0_private {
// References to one Main-owned shared index and the same four slots for every caller.
// This packet defines no storage and publishes no binding.
struct Shared {
    std::uint32_t& index;
    unsigned char (&slots)[4][17];
};
extern const Shared bindings; // Undefined here; Main owns one shared lifetime.
static_assert(sizeof(menu_resources::Ref)==16 && alignof(menu_resources::Ref)==1,
              "current nominal Ref");
static_assert(offsetof(menu_resources::Ref,bytes)==0 &&
              std::is_standard_layout<menu_resources::Ref>::value &&
              std::is_trivially_default_constructible<menu_resources::Ref>::value &&
              std::is_trivially_destructible<menu_resources::Ref>::value,"Ref lifetime");
std::uint32_t next_index_bits(std::uint32_t) noexcept;
char* get(menu_resources::Ref*,Shared) noexcept;
}
