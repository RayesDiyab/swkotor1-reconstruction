#include "45f886360f-constructors.h"
#include "83b3582c10-allocator_live.h"
// Dependency wiring only; no new original address or allocator storage owner.
namespace kotor_list_dependencies {
void* __cdecl allocate(uint32_t n){return live_operator_new(n);}
void __cdecl release(void* p){live_free(p);}
}
