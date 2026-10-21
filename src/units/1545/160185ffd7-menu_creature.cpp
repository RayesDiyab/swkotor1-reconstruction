#include "666b8b0d63-menu_imports.h"
#include <cstring>

namespace {
using U=std::uint32_t;
using I=std::int32_t;
using B=unsigned char;
void* at(void* p,U n){return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p)+n);}
template<class T>T load(const void* p,U n){T v;std::memcpy(&v,static_cast<const B*>(p)+n,sizeof v);return v;}
template<class T>void store(void* p,U n,T v){std::memcpy(at(p,n),&v,sizeof v);}
void* pointer(U bits){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(bits));}
const KotorM15ObjectMethods_007516c8* methods(void* p){return load<const KotorM15ObjectMethods_007516c8*>(p,0);}
const menu_server::Slots* server_methods(void* p){return load<const menu_server::Slots*>(p,0);}
void* client(){return load<void*>(menu_continuation::app_manager_007a39fc,4);}
float bits_float(U bits){float v;std::memcpy(&v,&bits,sizeof v);return v;}
void* placeable(void* p){return pointer(methods(p)->s04c(p,nullptr));}
void* door(void* p){return pointer(methods(p)->s014(p,nullptr));}
void* trigger(void* p){return pointer(methods(p)->s03c(p,nullptr));}
}

extern "C" void __fastcall kotor_fastcall_00616230(void* self,void*,void* output){
    U equipped=0;
    I health=0;
    if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)!=nullptr){
        void* const object=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
        void* const creature=server_methods(object)->as_creature_030(object,nullptr);
        if(creature!=nullptr){
            equipped=kotor_fastcall_004efa80(creature,nullptr);
            if(load<U>(creature,0xa9c)!=0){
                health=static_cast<std::int16_t>(server_methods(creature)->current_hp_09c(creature,nullptr,0));
            }else{
                void* const stats=load<void*>(creature,0xa74);
                const auto sum=static_cast<std::uint16_t>(load<std::uint16_t>(stats,0x126)+load<std::uint16_t>(stats,0x124));
                health=static_cast<std::int16_t>(sum);
            }
        }
    }
    U index=load<U>(output,4);
    kotor_fastcall_0064af10(load<void*>(self,0x2f8),nullptr,output,3,health,0,equipped,bits_float(0x38d1b717u));
    while(static_cast<I>(index)<load<I>(output,4)){
        void* const row=at(load<void*>(output,0),index*0x38u);
        store(row,0xc,&kotor_fastcall_0060f330);
        store<U>(row,0x10,0);store<U>(row,0x14,0);store<U>(row,0x18,0);
        store(row,0x1c,load<U>(self,4));
        ++index; // count/data are re-read after each row, exactly as original.
    }
}

extern "C" void __fastcall kotor_fastcall_006197d0(void* self,void*,void* output,U mask){
    if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)==nullptr)return;
    void* const object=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
    void* const creature=server_methods(object)->as_creature_030(object,nullptr);
    if(creature==nullptr)return;
    if(load<U>(recovered_object_area_004cb120(creature,nullptr),0x2b0)==0){
        constexpr U equipment_offsets[]={0x23cu,0x240u,0x248u};
        for(U offset:equipment_offsets){
            // The client singleton is re-read before every equipment lookup.
            const U item_id=load<U>(self,offset);
            void* const item_client=client();
            void* const item=kotor_m15_client_get_item_by_id_005ed5a0(item_client,nullptr,item_id);
            if(item!=nullptr)kotor_fastcall_006193a0(self,nullptr,output,kotor_m15closure_00646c90(item,nullptr),1,mask);
        }
    }
    void* const inventory=recovered_creature_inventory_004ef770(creature,nullptr,1);
    for(U index=0;index<load<U>(inventory,0x10);++index){
        void* const item=w2x_inventory_item_005560b0(inventory,nullptr,index);
        kotor_fastcall_006193a0(self,nullptr,output,item,0,mask);
    }
}

extern "C" std::uint8_t __fastcall kotor_fastcall_0060fc00(void*,void*,void* target){
    if(target==nullptr)return 0xff;
    if(methods(target)->s02c_as_creature(target,nullptr)!=nullptr){
        void* const creature=methods(target)->s02c_as_creature(target,nullptr);
        if(methods(creature)->s138_is_hostile(creature,nullptr)!=0)return 3;
        return kotor_fastcall_005ee0f0(client(),nullptr)!=0?3:2;
    }
    if(door(target)!=nullptr)return 0;
    if(placeable(target)!=nullptr){
        void* const place=placeable(target);
        if(place!=nullptr){
            void* const object=kotor_fastcall_00682e00(place,nullptr);
            if(object!=nullptr && load<U>(object,0x324)!=0)return 0;
        }
        return 2;
    }
    return trigger(target)!=nullptr?1:0xff;
}

extern "C" void __fastcall kotor_fastcall_00619c20(void* self,void*,void* target,U category,void* output){
    switch(kotor_fastcall_0060fcc0(self,nullptr,target)){
    case 1:
        if(placeable(target)!=nullptr)kotor_fastcall_006837d0(placeable(target),nullptr,self,category,output);
        else if(door(target)!=nullptr)kotor_fastcall_00684410(door(target),nullptr,self,category,output);
        break;
    case 2:
        if(trigger(target)!=nullptr)kotor_fastcall_00691f00(trigger(target),nullptr,self,category,output);
        break;
    case 3:
        if(placeable(target)==nullptr)break;
        if(category==1){
            void* const place=placeable(target);
            if(methods(place)->s138_is_hostile(place,nullptr)!=0){
                const U id=load<U>(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr),4);
                void* const server=kotor_fastcall_00682e00(placeable(target),nullptr);
                if(static_cast<I>(recovered_object_get_reputation_0057cb80(server,nullptr,id,0))<=10){
                    kotor_fastcall_006191f0(self,nullptr,target,output);break;
                }
            }
        }
        kotor_fastcall_006837d0(placeable(target),nullptr,self,category,output);
        break;
    case 4:
        if(category==0){
            kotor_fastcall_00619950(self,nullptr,output,methods(target)->s02c_as_creature(target,nullptr));
            void* const creature=methods(target)->s02c_as_creature(target,nullptr);
            kotor_fastcall_00619b10(creature,nullptr,self,0,output);
        }else if(category==1)kotor_fastcall_006191f0(self,nullptr,target,output);
        else if(category==2)kotor_fastcall_006198e0(self,nullptr,output,methods(target)->s02c_as_creature(target,nullptr));
        break;
    default:break;
    }
}
