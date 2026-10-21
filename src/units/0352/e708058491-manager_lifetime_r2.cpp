#include "7ea5e1b9a0-manager_lifetime.h"
#include <new>
namespace menu_resources {
Manager* __fastcall ConstructOwnedManager00409bf0(void* storage,void*) {
    auto* self=::new(storage) Manager; // default initialization, no byte clear
    for(unsigned offset=0x10;offset<=0x24;offset+=4)
        (void)::new(self->bytes+offset) base_ini_reconstruction::ListWrapper;
    (void)::new(self->bytes+0x40) kotor_startup::CriticalSection;
    (void)::new(self->bytes+0x3c) Word; // live typed thread-ID output, no initialization
    // Genuine constructor starts String exactly once, constructs the separate
    // list/critical children, calls actual GFF initialization and thread import.
    return self->construct_00409bf0();
}
void __fastcall DestroyOwnedManager00408670(Manager* self,void*) {
    DestroyManager00408670(self,nullptr);
    for(unsigned offset=0x24;;offset-=4) {
        auto* wrapper=std::launder(reinterpret_cast<base_ini_reconstruction::ListWrapper*>(self->bytes+offset));
        wrapper->~ListWrapper();
        if(offset==0x10)break;
    }
    std::launder(reinterpret_cast<Word*>(self->bytes+0x3c))->~Word();
    self->~Manager();
    // No allocation release, global write or fabricated quiescence result.
}
bool __cdecl ManagerHeapMatches(HANDLE captured) {
    return captured && live_globals.heap && *live_globals.heap==captured;
}
static_assert(sizeof(Manager)==96 && alignof(Manager)==4 &&
              sizeof(base_ini_reconstruction::ListWrapper)==4 &&
              alignof(base_ini_reconstruction::ListWrapper)==4 &&
              sizeof(kotor_startup::CriticalSection)==4,"canonical member storage");
}
