#include "708fbab234-manager_demand.h"
#include "6542d604d6-allocator_live.h"
#include "f7362fe553-resource_dispatch.h"
#include <cstdint>
#include <type_traits>
namespace {
using Word=std::uint32_t;
template<class T> T read(void* p,Word offset) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset);
}
template<class T> void write(void* p,Word offset,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+offset)=value;
}
}

// Original CExoResMan::FreeChunk, contiguous 004070A0..00407199. Resources and
// lists are existing opaque objects; this function creates no lifetime/storage.
extern "C" Word __fastcall c2da_manager_004070a0(menu_resources::Manager* manager,void*) {
    void* list=read<void*>(manager,0x20);
    void* position=read<void*>(list,0);
    // Original reads the head twice before taking the first payload.
    void* head=read<void*>(list,0);
    void* resource=head?read<void*>(head,8):nullptr;
    Word freed_bytes=0;
    while(resource) {
        list=read<void*>(manager,0x20);
        if(!read<void*>(list,0)||freed_bytes>=0x100000u)break;
        const Word flags=read<Word>(resource,0x0c);
        if((flags&0x10u)!=0) {
            resource=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list,nullptr,&position));
            continue;
        }
        if(read<std::uint16_t>(resource,4)>0) {
            write(resource,0x0c,flags&0xfffffeffu);
        } else {
            c2da_r29_dispatch::freed(resource);
            const Word before=read<Word>(manager,8);
            void* data=read<void*>(resource,0x10);
            freed_bytes-=before;
            // This inline branch is present in the original FreeChunk itself.
            // It reuses the sole allocator; it does not define FreeResourceData.
            if(data) {
                if((read<Word>(resource,8)&0xc0000000u)!=0x40000000u) {
                    if(read<Word>(resource,0x20)!=0) {
                        const Word address=static_cast<Word>(reinterpret_cast<std::uintptr_t>(data));
                        data=reinterpret_cast<void*>(static_cast<std::uintptr_t>(address-Word{6}));
                    }
                    live_free(data);
                    const Word size=read<Word>(resource,0x18);
                    const Word pool=read<Word>(manager,8);
                    write(manager,8,pool+size);
                }
                write(resource,0x18,Word{0});
                write(resource,0x10,static_cast<void*>(nullptr));
            }
            freed_bytes+=read<Word>(manager,8);
            write(resource,0x0c,read<Word>(resource,0x0c)&0xfffffeebu);
        }
        void* old_position=position;
        resource=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            read<void*>(manager,0x20),nullptr,&position));
        c2da_005e9c10(read<void*>(manager,0x20),nullptr,old_position);
    }
    return freed_bytes!=0?1u:0u;
}
using FreeChunk=Word(__fastcall*)(menu_resources::Manager*,void*);
static_assert(std::is_same<decltype(&c2da_manager_004070a0),FreeChunk>::value,"full EAX, no stack arguments");
static_assert(sizeof(void*)==4,"x86 opaque address carriers");
