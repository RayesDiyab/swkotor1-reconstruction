#include "37fa053dee-list_key_ports.h"
#include <cstring>
#include <new>
namespace {
menu_resources::Word ReadWord(const void* storage,unsigned offset) {
    menu_resources::Word result;
    std::memcpy(&result,static_cast<const unsigned char*>(storage)+offset,sizeof(result));
    return result;
}
}
// Newly recovered004076E0 body; switch targets authenticated from00407768.
// Resource and key storage stay opaque. Manager/ListWrapper/Internal/Node
// lifetimes and sole list-accessor bodies are imported from actual Main177.
extern "C" __declspec(noinline) void* __fastcall c2da_manager_004076e0(
    menu_resources::Manager* manager,void*,void* resource) {
    unsigned offset;
    switch(ReadWord(resource,8)>>30) {
    case 0:offset=0x10;break;
    case 1:offset=0x1c;break;
    case 2:offset=0x18;break;
    default:offset=0x14;break;
    }
    auto* wrapper=std::launder(reinterpret_cast<base_ini_reconstruction::ListWrapper*>(manager->bytes+offset));
    auto list=[&]()->base_ini_reconstruction::List* {
        return static_cast<volatile base_ini_reconstruction::ListWrapper*>(wrapper)->internal;
    };
    auto* initial=list();
    void* position=static_cast<volatile base_ini_reconstruction::List*>(initial)->head;
    if(!position)return nullptr;
    void* key=reinterpret_cast<void*>(unresolved_list_at_pos_005e9b60(initial,nullptr,position));
    while(position) {
        if(key) {
            const menu_resources::Word id=(ReadWord(resource,8)>>14)&0x3fu;
            if(((id^ReadWord(key,0x28))&0x0fffffffu)==0u)return key;
        }
        key=reinterpret_cast<void*>(unresolved_list_next_005e9b90(list(),nullptr,&position));
    }
    return nullptr;
}
