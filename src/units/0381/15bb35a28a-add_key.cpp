#include "4a3a41c731-add_key.h"
#include <cstring>
#include <new>

namespace {
menu_resources::Word ReadWord(const void* storage,unsigned offset) {
    menu_resources::Word value;
    std::memcpy(&value,static_cast<const unsigned char*>(storage)+offset,4);
    return value;
}
void StoreWord(void* storage,unsigned offset,menu_resources::Word value) {
    std::memcpy(static_cast<unsigned char*>(storage)+offset,&value,4);
}
}

// Exact existing export: ECX Manager, unused EDX, stack name/kind/flags;
// full EAX32, callee RET12, decorated @unresolved_add_key_00406e20@20.
// Normal, nonthrowing lower-call domain only; original FS/EH/invalid-memory
// behavior and every genuine lower-provider/lifetime gate remain unclaimed.
extern "C" __declspec(noinline) menu_resources::Word __fastcall
unresolved_add_key_00406e20(menu_resources::Manager* self,void*,
    menu_resources::String* name,menu_resources::Word kind,
    menu_resources::Word flags) {
    using menu_resources::Word;
    using menu_resources::String;
    using base_ini_reconstruction::List;
    using base_ini_reconstruction::ListWrapper;
    if(kind-1u>3u)return 0;

    const unsigned offset=0x10u+4u*(kind-1u);
    const Word mask=kind==1u?0u:kind==2u?0x80000000u:
                    kind==3u?0x40000000u:0x20000000u;
    // These actual member lifetimes are started by ConstructOwnedManager,
    // before the selected genuine Manager constructor publishes its result.
    auto* wrapper=std::launder(
        reinterpret_cast<ListWrapper*>(self->bytes+offset));
    auto list=[&]()->List* {
        return static_cast<volatile ListWrapper*>(wrapper)->internal;
    };
    Word used[2]={0,0};
    Word id=0;
    List* initial_list=list();
    void* position=static_cast<volatile List*>(initial_list)->head;
    if(position) {
        void* key=reinterpret_cast<void*>(
            unresolved_list_at_pos_005e9b60(initial_list,nullptr,position));
        while(position) {
            if(key) {
                // The selected constructor starts exactly this actual String
                // at key+20. No differently nominal snapshot/cast is used.
                auto* key_name=std::launder(reinterpret_cast<String*>(
                    static_cast<unsigned char*>(key)+0x20));
                if(add_key_lower::Equal005e5200(key_name,nullptr,name)) {
                    String message;
                    if(kind==4u) {
                        // Original reads capacity before data, then pushes
                        // both by-value String DWORDs to the vararg formatter.
                        const Word capacity=name->capacity;
                        char* data=name->data;
                        kotor_reconstruction::StartupFormat005e5680(
                            &message,add_key_lower::rebuild_message_0073d8a8,
                            data,capacity);
                    }
                    add_key_lower::Rebuild00410260(key,nullptr);
                    return 1;
                }
                const Word index=ReadWord(key,0x28)&0x0fffffffu;
                // Only indices0..63 are in the observed two-DWORD bitmap
                // domain; larger values overwrite the original stack.
                used[index>>5]|=Word(1)<<(index&31u);
            }
            // Reload the actual wrapper after each opaque comparison/miss.
            key=reinterpret_cast<void*>(
                unresolved_list_next_005e9b90(list(),nullptr,&position));
        }
    }
    for(Word candidate=1;candidate<64u;++candidate) {
        if(!(used[candidate>>5]&(Word(1)<<(candidate&31u)))) {
            id=candidate;
            break;
        }
    }
    id|=mask; // exhausted bitmap deliberately retains low ID0
    void* allocation=live_operator_new(0x34);
    void* key=allocation?key_table_construct_0040d030(allocation,nullptr):nullptr;
    auto* inserted=ini_addhead::AddHead005e9d70(list(),nullptr,key);
    const Word result=add_key_lower::Build00410190(
        key,nullptr,kind,name,id,1u);
    // Preserve the post-build flags store and original null-key invalid path.
    StoreWord(key,8,flags);
    if(!result) {
        add_key_lower::Remove005e9c10(list(),nullptr,inserted);
        add_key_lower::Destroy0040d0d0(key,nullptr);
        live_free(key);
    }
    return result;
}
