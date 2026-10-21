#include "bb96bfd227-critical_section.h"
static_assert(sizeof(kotor_startup::CriticalSection) == 4 &&
              sizeof(kotor_startup::CriticalSectionInternal) == 24,
              "ECX is the wrapper; its separate internal is the Win32 structure");
extern "C" void* __fastcall unresolved_critical_ctor_005e9290(void* receiver, void*) {
    // Manager supplies its four-byte member at+0x40. EDX is a dummy, not a second
    // receiver or stack argument. Allocation/import/lifetime remain owner gates.
    return static_cast<kotor_startup::CriticalSection*>(receiver)->Construct();
}
