// Required original model-ignore-list helpers; canonical live ArrayMutations
// and selected getters/allocator remain the sole storage/algorithm owners.
#include "b96846784b-m15_dust_models.h"
#include "cca80f7c35-m15e.h"
#include "177aadaf45-scene3d.h"
#include <cstring>
extern "C" {
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,std::uint32_t,void**);
void* __fastcall kotor_fastcall_005ed580(void*,void*,std::uint32_t);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,std::uint32_t);
std::uint32_t __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*,void*,std::uint32_t);
}
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace gui_viewport_list {void __fastcall add_00444fe0(kotor_reconstruction::ArrayMutations*,void*,int);}
namespace {
using U=std::uint32_t;
using B=unsigned char;
using List=kotor_reconstruction::ArrayMutations;
template<class T> T load(const void* p,U offset){T value;std::memcpy(&value,static_cast<const B*>(p)+offset,sizeof value);return value;}
const KotorM15ObjectMethods_007516c8* methods(void* object){return load<const KotorM15ObjectMethods_007516c8*>(object,0);}
std::int32_t signed_bits(U value){std::int32_t result;std::memcpy(&result,&value,4);return result;}
void* lookup(void* internal,U id){
    void* object=nullptr;
    if(kotor_fastcall_004d8230(load<void*>(internal,0x14),nullptr,id,&object)!=0)return nullptr;
    return object;
}
void* client(){return load<void*>(kotor_menu_window_cells::manager_007a39fc,4);}
void append_inline16(List* list,void* gob){
    const U capacity=static_cast<U>(list->capacity);
    if(static_cast<U>(list->size)==capacity)
        menu_list_growth::allocate_005e03c0(list,nullptr,capacity!=0?capacity+capacity:16u);
    const U count=static_cast<U>(list->size);
    const U value=reinterpret_cast<U>(gob);
    auto* const at=reinterpret_cast<volatile U*>(list->data+count*4u);
    *at=value;
    list->size=signed_bits(count+1u);
}
void append_call8(List* list,void* gob){gui_viewport_list::add_00444fe0(list,nullptr,signed_bits(reinterpret_cast<U>(gob)));}
void* model(void* object,U part){return methods(object)->s098_get_model(object,nullptr,part,1);}
void item_parts(void* item,List* list){
    void* const object=static_cast<B*>(item)+0x10;
    // Original loop calls FF,FE,FD,FC,FB, reloading the vptr each iteration.
    // Only the low part byte is defined in its incoming temporary DWORD.
    for(U part=0xff;part>=0xfb;--part)
        if(void* const gob=model(object,part))append_inline16(list,gob);
}
}
extern "C" {
void __fastcall m15_dust_collect_client_005ed9a0(void* self,void*,U id,List* list){
    m15_dust_collect_internal_005fcc90(load<void*>(self,4),nullptr,id,list);
}
void __fastcall m15_dust_collect_internal_005fcc90(void* self,void*,U id,List* list){
    void* const object=lookup(self,id);
    if(object==nullptr)return;
    const B type=load<B>(object,8);
    if(type==5)m15_dust_collect_creature_005f97e0(self,nullptr,id,list);
    else if(type==6)m15_dust_collect_item_005f99f0(self,nullptr,id,list);
    else m15_dust_collect_other_005f9aa0(self,nullptr,id,list);
}
void __fastcall m15_dust_collect_creature_005f97e0(void* self,void*,U id,List* list){
    void* const object=lookup(self,id);
    if(object==nullptr || load<B>(object,8)!=5)return;
    void* const creature=methods(object)->s02c_as_creature(object,nullptr);
    if(creature==nullptr)return;
    if(void* const gob=model(creature,0xff))append_inline16(list,gob);
    if(void* const gob=model(creature,0xfe))append_inline16(list,gob);
    // The app/client is re-read before each equipped-item query, as original.
    void* app=client();
    U item_id=kotor_m15a_creature_get_equipped_item_id_0060ca80(creature,nullptr,1);
    void* item=kotor_m15_client_get_item_by_id_005ed5a0(app,nullptr,item_id);
    if(item!=nullptr){void* const sub=static_cast<B*>(item)+0x10;if(void* const gob=model(sub,0xff))append_call8(list,gob);}
    app=client();item_id=kotor_m15a_creature_get_equipped_item_id_0060ca80(creature,nullptr,0x10);
    item=kotor_m15_client_get_item_by_id_005ed5a0(app,nullptr,item_id);
    if(item!=nullptr)item_parts(item,list);
    app=client();item_id=kotor_m15a_creature_get_equipped_item_id_0060ca80(creature,nullptr,0x20);
    item=kotor_m15_client_get_item_by_id_005ed5a0(app,nullptr,item_id);
    if(item!=nullptr)item_parts(item,list);
}
void __fastcall m15_dust_collect_item_005f99f0(void* self,void*,U id,List* list){
    void* const object=lookup(self,id);
    if(object==nullptr || load<B>(object,8)!=6)return;
    void* const item=reinterpret_cast<void*>(methods(object)->s034(object,nullptr));
    if(item!=nullptr)item_parts(item,list);
}
void __fastcall m15_dust_collect_other_005f9aa0(void*,void*,U id,List* list){
    void* const object=kotor_fastcall_005ed580(client(),nullptr,id);
    if(object==nullptr)return;
    void* const sub=methods(object)->s00c_as_creature_or_self(object,nullptr);
    if(sub!=nullptr)if(void* const gob=model(sub,0xff))append_call8(list,gob);
}
}
