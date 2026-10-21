#include "f7362fe553-resource_dispatch.h"
#include "708fbab234-manager_demand.h"
#include "6e414386b5-base_cancel_service.h"
#include "a55a4c4384-ini_cleanup.h"
#include <cstdint>

// Sole source proposal owner: /root/r25_resource_loaders. Task10 catalog plus
// R27 pinned PE audit supply the original evidence. Imported Manager and all
// actual list/resource/method receiver lifetimes remain owned by Main6.
namespace {
using Word=std::uint32_t;
template<class T> T read(void* p,Word off) {
    return *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off);
}
template<class T> void write(void* p,Word off,T value) {
    *reinterpret_cast<T volatile*>(static_cast<unsigned char*>(p)+off)=value;
}
template<class Fn> Fn method(void* p,Word slot) {
    return read<Fn>(read<void*>(p,0),slot);
}
using Effect=void(__thiscall*)(void*);
}

// Exact consumed lower carriers; no replacement lower algorithms/storage.
extern "C" void __fastcall c2da_manager_00406540(menu_resources::Manager*,void*,void*);

// Original EAX0/1 is unclaimed by the exact current consumed-void transport.
extern "C" void __fastcall c2da_manager_00407020(
    menu_resources::Manager* manager,void*,void* resource) {
    if (!resource || read<void*>(resource,0x10)==nullptr) return;
    void* list=read<void*>(manager,0x20);
    void* position=read<void*>(list,0);
    void* payload=position?read<void*>(position,8):nullptr;
    while (payload && payload!=resource) {
        payload=reinterpret_cast<void*>(unresolved_list_next_005e9b90(
            read<void*>(manager,0x20),nullptr,&position));
    }
    if (!payload) return;
    c2da_r29_dispatch::freed(resource);
    if ((read<unsigned char>(resource,0x0c)&0x10u)!=0) return;
    c2da_005e9c10(read<void*>(manager,0x20),nullptr,position);
    c2da_manager_00406540(manager,nullptr,resource);
    write(resource,0x0c,read<Word>(resource,0x0c)&0xfffffeebu);
}

extern "C" Word __fastcall c2da_manager_00408530(
    menu_resources::Manager* manager,void*) {
    void* entry=c2da_manager_004076e0(manager,nullptr,read<void*>(manager,0x28));
    if (!entry) return 0;
    const Word id=read<Word>(read<void*>(manager,0x28),8);
    const Word kind=id>>30u;
    if (kind==3u) {
        void* file=read<void*>(manager,0x2c);
        if (file) {
            c2da_005e69e0(file,nullptr);
            // Original006FA390 is the authenticated5-byte tail-jump to the
            // existing sole006FB7B2 live_free owner. No new allocator body.
            live_free(file);
            write(manager,0x2c,static_cast<void*>(nullptr));
        }
    } else {
        void* array=read<void*>(entry,0x30);
        // Authenticated004085D4 switch: type0 uses indexed slot; types1/2 slot0.
        const Word index=kind==0u?((id>>20u)&0x3ffu):0u;
        void* receiver=read<void*>(array,index*4u);
        method<Effect>(receiver,0x18)(receiver);
    }
    void* resource=read<void*>(manager,0x28);
    write(resource,0x0c,read<Word>(resource,0x0c)&0xffffffefu);
    resource=read<void*>(manager,0x28);
    write(resource,0x0c,read<Word>(resource,0x0c)|4u);
    resource=read<void*>(manager,0x28);
    if (read<void*>(resource,0x10)==nullptr) return 0;
    write(resource,6,std::uint16_t{0});
    resource=read<void*>(manager,0x28);
    const Word result=c2da_r29_dispatch::serviced(resource);
    (void)base_ini_reconstruction::remove_head_005e9c80(
        read<base_ini_reconstruction::List*>(manager,0x24),nullptr);
    write(manager,0x28,static_cast<void*>(nullptr));
    write(manager,0x2c,static_cast<void*>(nullptr));
    return result;
}
