#include "d06e8b44c9-manager_demand.h"
#include "9f442eedcf-file_thread.h"
#include <cstdint>
#include <type_traits>
// Proposed replacement of the R26 standalone-cell import: the real Task30
// singleton member is the sole completion word, not an alias or a second cell.
using Port=std::uint32_t(__fastcall*)(void*,void*);
static_assert(std::is_same_v<decltype(&c2da_005e6990),Port>);



extern "C" std::uint32_t __fastcall c2da_005e6990(void* wrapper,void*) {
    // Preserve the original wrapper+0 load even though tail005EBC50 ignores ECX.
    // The underlying tail reads the complete shared DWORD, never normalizes AL.
    void* internal=*static_cast<void* const volatile*>(wrapper);
    (void)internal;
    const volatile auto& worker=kotor_file::global_file_thread;
    return worker.complete;
}
