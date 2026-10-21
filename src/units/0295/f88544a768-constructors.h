#pragma once
#include <stdint.h>
#include <stddef.h>
namespace kotor_list_dependencies {
void* __cdecl allocate(uint32_t bytes); // exact 006FA7E6 contract; never host throwing new
void __cdecl release(void* allocation); // paired allocator cleanup dependency
}
namespace kotor_list {
struct Internal {
    void* head;
    void* tail;
    int32_t count;
    Internal* __thiscall Construct(); // 005E9AA0; no stack arguments, EAX=this
};
struct SoundList {
    Internal* internal;
    SoundList* __thiscall Construct(); // 00605800
};
struct ModelList {
    Internal* internal;
    ModelList* __thiscall Construct(); // 0070D2D0
};
static_assert(sizeof(void*)==4,"purchased GOG x86 ABI");
static_assert(sizeof(Internal)==12 && offsetof(Internal,tail)==4 && offsetof(Internal,count)==8,"internal layout");
static_assert(sizeof(SoundList)==4 && sizeof(ModelList)==4,"wrapper pointer layout");
}
