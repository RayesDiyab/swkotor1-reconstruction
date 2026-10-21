#pragma once
#include "68c866e33721ca16_vm_leaf.h"
namespace kotor::vm_stack_cleanup {
using I32=std::int32_t;
using U32=std::uint32_t;
// Accessors stay live across cleanup callbacks. The host view refers to the
// shared Internal owner; the x86 view refers to its embedded stack at +1AC.
// Tables/context bindings are immutable; accessors are transparent. Dependency
// callbacks may mutate VM fields, but must keep every later cached loop index
// readable. Ordinary shrinking requests require 0 <= requested < old depth;
// signed non-shrinking requests (including negatives) are exact no-ops.
struct View {
    void* context;
    I32 (*depth)(void*);
    void (*set_depth)(void*,I32);
    std::uint8_t (*tag)(void*,I32);
    U32 (*word)(void*,I32);
    void* (*implementer)(void*);
};
struct Dependencies {
    void* context;
    void (*destroy_string_005e5c20)(void*,U32 handle);
    void (*free_006fa390)(void*,U32 handle);
    void (*destroy_engine_slot6)(void*,void* implementer,I32 type,U32 handle);
};
// Engine tags require a live stack owner AND callable command implementer,
// even when their payload is zero. Push's owner-only check is insufficient.
View view(kotor::vm_leaf::Internal&) noexcept;
void set_stack_pointer_005d21f0(const View&,I32 requested,const Dependencies&);
}
