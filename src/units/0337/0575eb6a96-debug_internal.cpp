#include "0e6a7664d7-debug_constructor.h"
#include <new>
namespace kotor_menu20 {
DebugInternal::DebugInternal() {
    words[2]=0;
    words[1]=0;
    words[3]=0;
    words[4]=0;
    words[0]=0;
}
DebugInternal* __fastcall ConstructDebugInternal005ec290(DebugInternal* storage,void*) {
    return ::new(static_cast<void*>(storage)) DebugInternal;
}
}
