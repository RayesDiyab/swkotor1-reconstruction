#include "572d069072-base_cancel_service.h"
#include <cstring>
using Word=std::uint32_t;
using Half=std::uint16_t;
using Manager=menu_resources::Manager;
namespace {
template<class T>T load(const void* p,unsigned offset){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+offset,sizeof(v));return v;}
template<class T>void store(void* p,unsigned offset,T v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,sizeof(v));}
void* next(void* list,void** position){Word value=unresolved_list_next_005e9b90(list,nullptr,position);void* result;std::memcpy(&result,&value,4);return result;}
}
extern "C" __declspec(noinline) void* __fastcall c2da_resource_base_ctor_00406460(void* self,void*) {
    store(self,0,c2da_resource_base_methods_0073d86c());
    store(self,4,Half{0});store(self,8,Word{0xffffffffu});store(self,6,Half{0});
    store(self,0x18,Word{0});store(self,0x10,Word{0});store(self,0x14,Word{0});
    store(self,0x1c,Word{0});store(self,0x20,Word{0});store(self,0x24,Word{0});store(self,0x0c,Word{0});



    return self;
}
extern "C" __declspec(noinline) Half __fastcall c2da_manager_cancel_raw_004088f0(Manager* manager,void*,void* resource) {
    if(!resource)return 0;
    const Word flags=load<Word>(resource,0x0c);
    if(flags&4u)return 0;
    Half requests=load<Half>(resource,6);
    if(!requests)return 0;



    requests=static_cast<Half>(requests-1u);

    store(resource,6,requests);
    if(!requests&&(flags&0x10u)) {
        void* key=c2da_manager_004076e0(manager,nullptr,resource);
        if(!key)return 0;
        const Word id=load<Word>(resource,8);
        const Word kind=id>>30u;



        const Word buffer_kind=3u;

        if(kind==buffer_kind) {
            void* buffer=load<void*>(manager,0x2c);
            if(buffer&&!load<Half>(resource,6)) {
                c2da_005e69e0(buffer,nullptr);
                live_free(buffer);
            }
        } else if(kind!=3u) {
            const void* array=load<const void*>(key,0x30);
            const Word index=kind==0u?((id>>20u)&0x3ffu):0u;
            void* archive=load<void*>(array,index*4u);
            const void* methods=load<const void*>(archive,0);
            using Close=void(__thiscall*)(void*);
            Close close=load<Close>(methods,0x14);
            close(archive);
        }
        store(manager,0x28,Word{0});store(manager,0x2c,Word{0});
    }
    void* position=load<void*>(load<void*>(manager,0x24),0);
    void* payload=position?load<void*>(position,8):nullptr;
    while(position) {
        if(payload==resource) {
            if(!load<Half>(resource,6))c2da_005e9c10(load<void*>(manager,0x24),nullptr,position);
            break;
        }
        payload=next(load<void*>(manager,0x24),&position);
    }
    return load<Half>(resource,6);
}
// Existing callers consume void; one algorithm retains the original AX result.
extern "C" __declspec(noinline) void __fastcall c2da_manager_cancel_004088f0(Manager* manager,void*,void* resource) {
    (void)c2da_manager_cancel_raw_004088f0(manager,nullptr,resource);
}
extern "C" __declspec(noinline) void __fastcall c2da_resource_serviced_00409b40(void* resource,void*,Word) {
    if(!resource)return;
    if(load<Half>(resource,4)>0u) {store(resource,0x0c,load<Word>(resource,0x0c)|0x100u);return;}



    const Word mask=0x14u;

    if((load<Word>(resource,0x0c)&mask)&&load<void*>(resource,0x10))
        c2da_manager_00407020(c2da_manager_007a39e8(),nullptr,resource);
}
