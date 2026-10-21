#pragma once
#include <cstddef>
#include <cstdint>
// The known pointer prefix plus opaque bytes preserve each saved data extent.
// No element/class/count/capacity semantics are inferred for the opaque bytes.
struct CleanupStorage4 { void* volatile head; };
struct CleanupStorage12 { void* volatile head; std::uint8_t opaque[8]; };
static_assert(sizeof(void*)==4 && sizeof(CleanupStorage4)==4 && sizeof(CleanupStorage12)==12,"Verified x86 storage extents");
static_assert(offsetof(CleanupStorage4,head)==0 && offsetof(CleanupStorage12,head)==0,"Observed pointer is the prefix");
extern "C" {
extern CleanupStorage4 historical_storage_007a477c;
extern CleanupStorage12 historical_storage_007a478c;
extern CleanupStorage12 historical_storage_007a693c;
extern CleanupStorage12 historical_storage_007a8a88;
extern CleanupStorage12 historical_storage_007a8a7c;
extern CleanupStorage12 historical_storage_007a8a70;
extern CleanupStorage12 historical_storage_007a8a64;
extern CleanupStorage4 historical_storage_007a8a58;
extern CleanupStorage12 historical_storage_007b8fcc;
extern CleanupStorage12 historical_storage_007b8fd8;
extern CleanupStorage12 historical_storage_007b90fc;
extern CleanupStorage12 historical_storage_007b9108;
extern CleanupStorage12 historical_storage_007b9398;
}
