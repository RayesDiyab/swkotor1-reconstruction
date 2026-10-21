#include "fc85507e20-remove_current.h"
#include <new>
using namespace async_load;
// Task10 00407830 algorithm, projected only onto current Main storage/ports.
extern "C" __declspec(noinline) Word __fastcall
async_remove_key_00407830(Manager* self,void*,String* name,Word kind) {
    if(kind-1u>3u)return 0;
    const unsigned offset=0x10+4*(kind-1u);
    using base_ini_reconstruction::List;
    using base_ini_reconstruction::ListWrapper;
    auto* wrapper=std::launder(reinterpret_cast<ListWrapper*>(self->bytes+offset));
    auto list=[&]()->List* {return static_cast<volatile ListWrapper*>(wrapper)->internal;};
    // The real incoming kind slot becomes an actual void* cursor object.
    using Position=void*;
    Position* position=::new(static_cast<void*>(&kind)) Position(
        static_cast<volatile List*>(list())->head);
    if(!*position)return 0;
    void* key=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(list(),nullptr,*position));
    while(*position) {
        if(key) {
            String* key_name=std::launder(reinterpret_cast<String*>(
                static_cast<unsigned char*>(key)+0x20));
            if(add_key_lower::Equal005e5200(key_name,nullptr,name)) {
                async_unload_key_0040d2e0(key,nullptr);
                add_key_lower::Remove005e9c10(list(),nullptr,*position);
                add_key_lower::Destroy0040d0d0(key,nullptr);
                live_free(key);
                return 1;
            }
        }
        key=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list(),nullptr,position));
    }
    return 0;
}
