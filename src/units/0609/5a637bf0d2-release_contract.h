#pragma once
#include "2d9e471d28-functional_bootstrap.h"

namespace kotor_functional {
// Source-support postconditions; no original-address or worker-body credit.
enum class ReleaseRefusal : unsigned {
    InvalidMetadata = 90,
    PreparedOwnershipRetained = 91,
    WindowOwnershipRetained = 92,
    AggregateOwnershipRetained = 93,
    HeapAllocationsRetained = 94,
    HeapInspectionFailed = 95
};
[[noreturn]] void fail_closed_release(ReleaseRefusal) noexcept;
// Defined beside the actual GameLifetime singleton, never a caller-supplied flag.
bool aggregate_lifetime_idle() noexcept;
// Returning is permitted only after the genuine owner clears its partial/full
// result and the existing attachment/aggregate metadata is fully detached.
void release_game_checked(PreparedGame*) noexcept;
// Called after locale/INI scopes close, immediately before GameHeap teardown.
// It detects retained actual heap blocks; it does not certify worker quiescence.
void require_empty_source_heap(HANDLE) noexcept;
}
