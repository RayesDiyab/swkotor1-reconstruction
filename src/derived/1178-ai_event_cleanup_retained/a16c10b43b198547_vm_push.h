#pragma once
#include "f670a0021bb0aa19_vm_leaf.h" // Shared state owned by the mac-vm-leaf package.
#include <cstdint>

namespace kotor::vm_push {
// The caller binds the existing allocation family (006FA7E6 / 006FA390).
// Allocations must succeed, be mutually disjoint, and be suitably aligned for
// uint32_t. Callbacks must not mutate/alias the VM state or reenter this code.
// This bounded seam does not specify original allocation-failure behavior.
struct Allocator {
    void* context;
    void* (*allocate)(void* context, std::uint32_t bytes);
    void (*release)(void* context, void* allocation);
};
// Normal states have 0 <= depth <= capacity <= 0x3ffffeff. For capacity>0,
// both arrays have capacity readable/writable elements, with no state aliasing.
// A negative-depth AddToTop with nonmatching capacity performs no action.
// Integer/object pushes require normal state and a successful append.
// No ownership/destructor policy is added for existing word payloads.
void add_to_top_005d15a0(vm_leaf::Internal&, std::int32_t type, const Allocator&);
std::uint32_t push_integer_005d1850(vm_leaf::Internal&, std::int32_t, const Allocator&);
std::uint32_t push_object_005d1a10(vm_leaf::Internal&, std::uint32_t, const Allocator&);
std::uint32_t push_integer_005d1010(vm_leaf::Machine&, std::int32_t, const Allocator&);
std::uint32_t push_object_005d10d0(vm_leaf::Machine&, std::uint32_t, const Allocator&);
}
