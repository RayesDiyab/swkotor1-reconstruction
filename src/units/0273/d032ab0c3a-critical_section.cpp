#include "fb1b0a129e-critical_section.h"
#include <new>
namespace kotor_startup {
CriticalSectionInternal* CriticalSectionInternal::Construct() {
    kotor_startup_dependencies::initialize(&storage);
    return this;
}
void CriticalSectionInternal::Destroy() {
    kotor_startup_dependencies::destroy(&storage);
}
void CriticalSectionInternal::Enter() {
    kotor_startup_dependencies::enter(&storage);
}
void CriticalSectionInternal::Leave() {
    kotor_startup_dependencies::leave(&storage);
}
CriticalSection* CriticalSection::Construct() {
#ifdef NEGATIVE_ALLOCATION_SIZE
    const uint32_t bytes = 0x14;
#else
    const uint32_t bytes = 0x18;
#endif
    CriticalSectionInternal* p = static_cast<CriticalSectionInternal*>(
        kotor_startup_dependencies::allocate_internal(bytes));
    if (p) {
        // Observed EH state 0 frees allocated storage if internal construction
        // unwinds. Modern C++ exception behavior is candidate-only evidence;
        // historical FrameHandler/SEH matching remains a runtime dependency.
        try {
            p = (::new(p) CriticalSectionInternal)->Construct();
        } catch (...) {
            kotor_startup_dependencies::free_internal(p);
            throw;
        }
    }
    internal = p;
    return this;
}
void CriticalSection::Enter() { internal->Enter(); }
void CriticalSection::Leave() { internal->Leave(); }
void CriticalSection::Destroy() {
    CriticalSectionInternal* p = internal;
    if (p) {
        p->Destroy();
        kotor_startup_dependencies::free_internal(p);
    }
#ifndef NEGATIVE_NO_CLEAR
    internal = 0;
#endif
}
}
