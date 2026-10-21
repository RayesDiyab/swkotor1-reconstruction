#include "cdd98abf6f-timers_constructor.h"
#include "16adb8eacb-allocator_live.h"
#include <new>
namespace kotor_startup {
Timers* __fastcall ConstructTimers005e91e0(Timers* storage,void*) {
    auto* self=::new(static_cast<void*>(storage)) Timers;
    void* memory=live_operator_new(32);
    TimersInternal* created=memory
        ? ConstructTimersInternal005ec160(static_cast<TimersInternal*>(memory),nullptr)
        : nullptr;
    self->internal=created;
    return self;
}
}
