#pragma once
#include "c91c1f9559-resource_lower.h"
#include <cstddef>
#include <type_traits>

// Narrow data proposal, not a CRes2DA object or class layout.
// Original slots 0, 3 and 4 use ECX thiscall. These free-function source ports
// use the project's fastcall transport with an explicitly unused EDX argument.
// Main6 must qualify coherent source dispatch before admitting this descriptor.
namespace c2da_resource_methods {
using Word = c2da_resource_lower::Word;
using DeletingDestroy = void* (__fastcall*)(void*, void*, Word);
using ConstantWord = Word (__stdcall*)();
using ResourceCallback = Word (__fastcall*)(void*, void*);

struct Methods0073ec9c {
    DeletingDestroy deleting_destroy_0041d7f0;
    ConstantWord constant_word_00406490;
    ConstantWord constant_word_0063e7f0;
    ResourceCallback freed_0041d770;
    ResourceCallback serviced_0041d790;
};
static_assert(sizeof(void*) == 4 && sizeof(Word) == 4, "x86 transport");
static_assert(std::is_standard_layout<Methods0073ec9c>::value, "table offsets");
static_assert(sizeof(Methods0073ec9c) == 20 && alignof(Methods0073ec9c) == 4,
              "exact five-cell data proposal");
static_assert(offsetof(Methods0073ec9c, deleting_destroy_0041d7f0) == 0, "slot0");
static_assert(offsetof(Methods0073ec9c, constant_word_00406490) == 4, "slot1");
static_assert(offsetof(Methods0073ec9c, constant_word_0063e7f0) == 8, "slot2");
static_assert(offsetof(Methods0073ec9c, freed_0041d770) == 12, "slot3");
static_assert(offsetof(Methods0073ec9c, serviced_0041d790) == 16, "slot4");
}

// All five are genuine missing-body imports. No recorder or success provider
// belongs in this production proposal. Word describes EAX bits, not signedness.
extern "C" {
void* __fastcall c2da_resource_deleting_destroy_0041d7f0(
    void* self, void* unused_edx, c2da_resource_methods::Word flags);
c2da_resource_methods::Word __stdcall c2da_resource_constant_word_00406490();
c2da_resource_methods::Word __stdcall c2da_resource_constant_word_0063e7f0();
c2da_resource_methods::Word __fastcall c2da_resource_freed_0041d770(
    void* self, void* unused_edx);
c2da_resource_methods::Word __fastcall c2da_resource_serviced_0041d790(
    void* self, void* unused_edx);
extern const c2da_resource_methods::Methods0073ec9c c2da_resource_method_table_0073ec9c;
}

