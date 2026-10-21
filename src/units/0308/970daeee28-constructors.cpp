#include "45f886360f-constructors.h"
#include <new>
namespace kotor_list {
Internal* Internal::Construct() {
    tail=0;
    head=0;
#ifndef NEGATIVE_KEEP_COUNT
    count=0;
#endif
    return this;
}
template<class Wrapper> static Wrapper* construct(Wrapper* self) {
#ifdef NEGATIVE_PRECLEAR
    self->internal=0;
#endif
#ifdef NEGATIVE_ALLOCATION_SIZE
    const uint32_t size=8;
#else
    const uint32_t size=12;
#endif
    Internal* p=static_cast<Internal*>(kotor_list_dependencies::allocate(size));
    if(p) {
        // Original wrappers install distinct FuncInfo handlers for a state-0
        // allocation cleanup. This C++ catch expresses the cleanup intent only;
        // original OS-dispatched EH/SEH compatibility is a separate runtime gate.
        try { p=(new(p) Internal)->Construct(); } // placement starts lifetime; allocates nothing
        catch(...) { kotor_list_dependencies::release(p); throw; }
    }
#ifdef NEGATIVE_KEEP_ON_NULL
    if(p) self->internal=p;
#else
    self->internal=p;
#endif
    return self;
}
SoundList* SoundList::Construct() { return construct(this); }
ModelList* ModelList::Construct() { return construct(this); }
}
