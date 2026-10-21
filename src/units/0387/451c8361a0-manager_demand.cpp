#include "d06e8b44c9-manager_demand.h"
#include <cstring>

namespace {
using Word=std::uint32_t;
using Half=std::uint16_t;
using Manager=menu_resources::Manager;
template<class T>T read(const void* object,unsigned offset) {
    T value;std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof(value));return value;
}
template<class T>void write(void* object,unsigned offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof(value));
}
void* member(Manager* manager,unsigned offset) {return reinterpret_cast<void*>(manager->word(offset));}
Word demanded(void* resource) {
    const Half count=read<Half>(resource,4);



    write(resource,4,static_cast<Half>(static_cast<Word>(count)+1u));

    return read<Word>(resource,0x10);
}
using Effect=void(__thiscall*)(void*);
void release_key_effect(void* key) {
    void* slots=read<void*>(key,0x30);
    void* receiver=read<void*>(slots,0);
    const void* methods=read<const void*>(receiver,0);
    const Effect effect=read<Effect>(methods,0x14);
    effect(receiver);
}
}

extern "C" __declspec(noinline) Word __fastcall c2da_manager_demand_004089f0(
    Manager* manager,void*,void* resource) {
    if(!resource)return 0;
    const Word key=read<Word>(resource,8);
    if(key==0xffffffffu)return 0;
    const Word flags=read<Word>(resource,0x0c);
    if((flags&4u) && read<Word>(resource,0x10)) {
        if(read<Half>(resource,4)==0u && (flags&0x100u)) {
            void* list=member(manager,0x20);
            void* position=read<void*>(list,0);
            Word payload=position?read<Word>(position,8):0u;
            while(payload) {
                if(payload==reinterpret_cast<Word>(resource)) {
                    c2da_005e9c10(member(manager,0x20),nullptr,position);
                    write(resource,0x0c,read<Word>(resource,0x0c)&~0x100u);
                    break;
                }
                payload=unresolved_list_next_005e9b90(member(manager,0x20),nullptr,&position);
            }
        }
        return demanded(resource);
    }
    if(flags&0x10u) {
        void* completion=member(manager,0x2c);
        if(!completion)return read<Word>(resource,0x10);
        if(!c2da_005e6990(completion,nullptr)) {
            do {
                c2da_sleep_0073d1e4(5u);
            } while(!c2da_005e6990(member(manager,0x2c),nullptr));
        }
        if(!c2da_manager_00408530(manager,nullptr))return 0;
        return demanded(resource);
    }
    Word loaded=0;
    switch(key>>30u) {



    case 0:loaded=c2da_manager_00407e00(manager,nullptr,resource,0u);break;

    case 1:loaded=c2da_manager_00407d50(manager,nullptr,resource,0u);break;
    case 2:loaded=c2da_manager_00407bd0(manager,nullptr,resource,0u);break;
    case 3:loaded=c2da_manager_004078f0(manager,nullptr,resource,0u);break;
    }
    if(!loaded)return 0;
    void* list=member(manager,0x24);
    void* position=read<void*>(list,0);
    Word payload=position?read<Word>(position,8):0u;
    while(position) {
        if(payload==reinterpret_cast<Word>(resource)) {
            c2da_005e9c10(member(manager,0x24),nullptr,position);
            break;
        }
        payload=unresolved_list_next_005e9b90(member(manager,0x24),nullptr,&position);
    }
    return demanded(resource);
}

extern "C" __declspec(noinline) Word __fastcall c2da_manager_release_demand_body_00408c90(
    Manager* manager,void*,void* resource) {
    if(!resource)return 0xffffffffu;
    if((read<Word>(resource,8)&0xc0000000u)==0x40000000u)
        release_key_effect(c2da_manager_004076e0(manager,nullptr,resource));
    const Half count=read<Half>(resource,4);
    if(count>1u) {
        write(resource,4,static_cast<Half>(count-1u));



        return count;

    }
    if(!count)return 0xffffffffu;
    const Word flags=read<Word>(resource,0x0c);
    write(resource,4,Half{0});
    if(flags&0x100u) {



        c2da_manager_00408b90(manager,nullptr,resource,0u);



        if(read<Word>(resource,0x10)) {

            (void)c2da_005e9d70(member(manager,0x20),nullptr,resource);
            c2da_manager_00408b90(manager,nullptr,resource,0u);
        }
        write(resource,0x0c,read<Word>(resource,0x0c)&~0x100u);
        return 0;
    }
    (void)c2da_005e9d70(member(manager,0x20),nullptr,resource);
    write(resource,0x0c,read<Word>(resource,0x0c)|0x100u);
    return 0;
}

// The consumed00409B80 endpoint discards EAX. The complete release algorithm
// above remains the sole body owner; this join adds no reconstructed address.
extern "C" __declspec(noinline) void __fastcall c2da_manager_release_demand_00408c90(
    Manager* manager,void*,void* resource) {
    (void)c2da_manager_release_demand_body_00408c90(manager,nullptr,resource);
}
