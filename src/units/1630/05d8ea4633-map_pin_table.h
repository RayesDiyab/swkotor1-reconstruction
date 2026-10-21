#pragma once
// 00754598 CSWCMapPin canonical method table -- typed data-owner contract.
//
// Original identity (read-only oracle swkotor.exe SHA-256
// 9C10E0450A6EECA417E036E3CDE7474FED1F0A92AAB018446D156944DEA91435):
//   00754598..007546E0, 328 bytes, 82 dwords.
//   007546DC is the last slot; 007546E0 is end-exclusive (the next byte is ASCII
//   "k_sup_gohawk", not a table slot).
//
// The rebuilt image does not place this object at the original VA.  The symbol
// below is a normal COFF data object with normal relocations to the selected
// method bodies.  It is not the CSWCObject table 007516C8: map-pin differs at
// slots 0, 29, 30, 31 and 32.
#include "848078071a-map_pin_ctor.h"

namespace kotor_map_pin {

constexpr std::uint32_t kVtableSlotCount = 82;
constexpr std::uint32_t kVtableSizeBytes = 328;
constexpr std::uint32_t kVtableLastSlotOffset = 81 * sizeof(void*);  // 0x144
constexpr std::uint32_t kOriginalVtableEndExclusive = 0x007546E0;    // identity only
constexpr std::uint32_t kOriginalVtableLastSlot = 0x007546DC;        // identity only

static_assert(kVtableSlotCount * sizeof(void*) == kVtableSizeBytes,
              "82 slots * 4 bytes = 328 bytes");
static_assert(kVtableLastSlotOffset + sizeof(void*) == kVtableSizeBytes,
              "last slot at 007546DC; end-exclusive at 007546E0");
static_assert(sizeof(recovered_client_map_pin_methods_00754598) == kVtableSizeBytes,
              "canonical CSWCMapPin data owner must be 328 bytes");

}  // namespace kotor_map_pin
