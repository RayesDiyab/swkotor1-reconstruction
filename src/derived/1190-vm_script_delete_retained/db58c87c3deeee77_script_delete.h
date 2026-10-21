#pragma once
#include "ef338a99d727da76_stack_clear.h"
namespace kotor::vm_delete {
using U32=std::uint32_t;
// Descriptor offsets are original byte offsets; accessors are transparent/live.
// A handle is opaque and must never be cast into a host pointer.
struct Descriptor {
 void* context;
 U32 (*read)(void*,U32 offset);
 void (*write)(void*,U32 offset,U32 value);
};
struct Dependencies {
 void* context;
 // Pure mapping to the existing ClearStack owner; no copied stack layout.
 vm_stack_clear::View (*resolve_stack)(void*,U32 handle);
 vm_stack_clear::Dependencies stack;
 void (*free_006fa390)(void*,U32 handle);
};
// Original ECX is unused; one descriptor stack argument, RET4, EAX=1.
// Dispatch/mappings are immutable. Reached accesses and cached handles remain
// valid across callbacks. Exceptions propagate at their original operation.
U32 delete_script_005d44f0(const Descriptor&,const Dependencies&);
}
