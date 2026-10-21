#pragma once
#include <cstdint>
namespace kotor_aurora_factory {
using Word=std::uint32_t;
// Borrowed views only. Task6 supplies the sole canonical producer/publication.
// Descriptor pointers remain immutable across allocator/new-handler callbacks;
// the pointed-to singleton/count/state cells are live, distinct and writable.
struct Views {
    void* volatile* singleton_007b921c;
    Word volatile* counter_007b9218;
    std::uint16_t volatile* state_007b9220;
    const void* vtable_00741518;
};
extern Views* const views;
static_assert(sizeof(void*)==4 && sizeof(Views)==16,"original x86 pointer views");
}
extern "C" {
__declspec(noinline) void* __cdecl reconstructed_factory_0044ecd0();
__declspec(noinline) void* __cdecl unresolved_factory_0044ed60();
__declspec(noinline) void __cdecl unresolved_quick_cleanup_006FB243();
}
