#pragma once
#include "77d07bad29444d41_stack_cleanup.h"
namespace kotor::vm_stack_clear {
using vm_stack_cleanup::I32;
using vm_stack_cleanup::U32;
// Shared cleanup dependencies remain owned by mac-vm-stack-cleanup. Arrays use
// native pointers in host fixtures; their release maps to the same original free.
struct Dependencies {
    vm_stack_cleanup::Dependencies elements;
    void (*release_array_006fa390)(void*,void*);
};
struct View {
    vm_stack_cleanup::View elements;
    void* (*tags)(void*);
    void* (*words)(void*);
    void (*clear_word)(void*,I32);
    void (*set_base)(void*,I32);
    void (*set_capacity)(void*,U32);
    void (*set_tags)(void*,void*);
    void (*set_words)(void*,void*);
};
// Immutable tables/context, transparent accessors. Callbacks may change VM
// fields; all cached traversal indices must remain readable and writable.
// If both arrays are nonnull, depth is 0..INT_MAX (negative depths excluded).
// Engine slots require owner AND callable implementer, including zero payloads.
View view(vm_leaf::Internal&) noexcept;
void clear_stack_005d2140(const View&,const Dependencies&);
}
