#include "d6795b968b-ref_ring.hpp"

namespace {
// One process-shared lifetime. Original file-backed .data initializes both to
// zero; the first call selects slot 1. No reset, allocation or TLS surrogate.
std::uint32_t shared_index{};
unsigned char shared_slots[4][17]{};
static_assert(sizeof(shared_index) == 4 && sizeof(shared_slots) == 68,
              "original index and four character slots");
}

namespace ref_00405fe0_private {
const Shared bindings{shared_index, shared_slots};
}
