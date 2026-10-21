#pragma once
#include "a16c10b43b198547_vm_push.h"
namespace kotor::vm_engine_push {
// Exact implementer virtual slot0x20 contract. The provider owns engine-value
// copying; this donor does not substitute pointer borrowing or fake success.
using CopyEngineStructure = std::uint32_t (*)(void* implementer,
                                             std::uint32_t type,
                                             std::uint32_t value);
// With a nonnull implementer, valid normal stack state, type0..9, nonnull
// stack_owner and successful existing allocation are required for append.
// Copy may mutate/reenter state only if it returns with that domain intact.
std::uint32_t push_005d19c0(vm_leaf::Internal&, std::uint32_t type,
    std::uint32_t value, CopyEngineStructure, const vm_push::Allocator&);
std::uint32_t push_005d10b0(vm_leaf::Machine&, std::uint32_t type,
    std::uint32_t value, CopyEngineStructure, const vm_push::Allocator&);
}
