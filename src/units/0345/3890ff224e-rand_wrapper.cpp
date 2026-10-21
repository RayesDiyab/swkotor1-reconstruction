#include "e8a5e6befc-rand_constructor.h"
#include "dcaf3f57f3-allocator_live.h"
#include <new>
namespace startup_rand {
Rand* __fastcall ConstructRand005ea420(Rand* storage,void*) {
    auto* self=::new(static_cast<void*>(storage)) Rand;
    void* memory=live_operator_new(144);
    Internal* created=memory
        ? ConstructRandInternal005ed0e0(static_cast<Internal*>(memory),nullptr)
        : nullptr;
    self->internal=created;
    return self;
}
}
