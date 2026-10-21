#include "0e6a7664d7-debug_constructor.h"
#include "b477fbe61f-allocator_live.h"
#include <new>
namespace kotor_menu20 {
Debug::Debug() {
    // filename construction precedes this body and allocation, as at005E938F.
    void* memory=live_operator_new(28);
    DebugInternal* created=memory
        ? ConstructDebugInternal005ec290(static_cast<DebugInternal*>(memory),nullptr)
        : nullptr;
    internal=created;
    enable_release_logging=0;
}
Debug* __fastcall ConstructDebug005e9370(Debug* storage,void*) {
    return ::new(static_cast<void*>(storage)) Debug;
}
}
