#include "0aec0b9933-menu_row_common.h"
using namespace menu_row;
namespace {
bool below_maximum(void* creature) {
    void* stats=load<void*>(creature,0x2F8);
    const auto current=load<std::int16_t>(stats,0x4C);
    const auto maximum=recovered_save_stats::maximum_hp_00647a80(stats);
    return current<static_cast<std::int16_t>(maximum);
}
void disable_row(void* list,U offset) {
    void* flags=at(row(list,offset),0x30);
    store(flags,0,load<U>(flags)&0xFFFFFFFEu);
    flags=at(row(list,offset),0x30);
    store(flags,0,(load<U>(flags)&0xFFFFFFEBu)|0xAu);
}
}
extern "C" void __fastcall kotor_fastcall_006193a0(void* self,void*,void* list,void* item,U equipped_mode,U filter_mask) {
    const U index=load<U>(list,4);
    if(kotor_fastcall_00616520(self,nullptr,item,equipped_mode,filter_mask)==0)return;
    String label;
    const U language=kotor_fastcall_005ed4c0(client(),nullptr);
    exo_locstring_get_005ea130(at(item,0x280),nullptr,language,&label,0);
    {
        StringStorage detail_storage;
        String* detail=kotor_fastcall_005edeb0(client(),nullptr,detail_storage.get(),0x9475);
        StringCleanup detail_cleanup{detail_storage.get()};
        const char* suffix=WindowCStr005e5670(detail,nullptr);
        const char* name=WindowCStr005e5670(&label,nullptr);
        kotor_reconstruction::StartupFormat005e5680(&label,"%s (%s)",name,suffix);
    }
    // Label providers may change list size; the original reloads it here.
    kotor_fastcall_004fddd0(list,nullptr,static_cast<I>(load<U>(list,4)+1));
    const U offset=index*0x38;
    assign_row_label(list,offset,&label);
    store(row(list,offset),8,load<U>(item,0x14)|0x40000000u);
    set_callback(list,offset,{&kotor_fastcall_0060f590,0,0,0});
    store(row(list,offset),0x1C,load<U>(self,4));
    item_icon(item,at(row(list,offset),0x20));
    store(row(list,offset),0x34,load<U>(item,0x28C));
    const auto kind=load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC);
    if((kind==0x2D||kind==0x1A)&&!below_maximum(self)){
        void* creature=server_creature(self);
        if(creature==nullptr||load<U>(creature,0x9E0)==0)disable_row(list,offset);
        return;
    }
    if(kind==0x2F){
        bool injured=below_maximum(self);
        void* first=frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(),nullptr),nullptr,1);
        if(first!=nullptr&&kotor_m15a_creature_get_server_creature_0060fb20(first,nullptr)!=nullptr&&below_maximum(first))injured=true;
        void* second=frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client(),nullptr),nullptr,2);
        if(second!=nullptr&&kotor_m15a_creature_get_server_creature_0060fb20(second,nullptr)!=nullptr&&below_maximum(second))injured=true;
        void* creature=server_creature(self);
        if(!injured||creature==nullptr)disable_row(list,offset);
        return;
    }
    if(equipped_mode!=0){
        // Output scratch originally aliases the nonzero incoming mode word.
        void* property=reinterpret_cast<void*>(static_cast<std::uintptr_t>(equipped_mode));
        recovered_item_property_query_005539c0(item,nullptr,&property,0xA,0);
        const U value=load<std::uint8_t>(property,0xC);
        void* flags=at(row(list,offset),0x30);
        const U previous=load<U>(flags);
        store(flags,0,previous^((value^previous)&1));
    }
}
extern "C" void __fastcall kotor_fastcall_006196a0(void* self,void*,void* list,void* item,void* creature) {
    // This read precedes the original null-item branch, including its fault.
    const volatile U index=load<U>(list,4);
    if(item==nullptr)return;
    void* area=frame_GetServerArea_00605640(load<void*>(self,0x20),nullptr);
    if(load<U>(area,0x2B0)!=0)return;
    if(load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)!=6)return;
    String label;
    const U language=kotor_fastcall_005ed4c0(client(),nullptr);
    exo_locstring_get_005ea130(at(item,0x280),nullptr,language,&label,0);
    kotor_fastcall_004fddd0(list,nullptr,static_cast<I>(load<U>(list,4)+1));
    const U offset=index*0x38;
    assign_row_label(list,offset,&label);
    store(row(list,offset),8,load<U>(item,0x14)|0x40000000u);
    set_callback(list,offset,{&kotor_fastcall_00617e40,0,0,0});
    store(row(list,offset),0x1C,load<U>(creature,4));
    item_icon(item,at(row(list,offset),0x20));
    store(row(list,offset),0x34,load<U>(item,0x28C));
}
extern "C" void __fastcall kotor_fastcall_006198e0(void* self,void*,void* list,void* creature) {
    void* inventory=recovered_creature_inventory_004ef770(server_creature(self),nullptr,1);
    U index=0;
    while(index<load<U>(inventory,0x10)){
        void* item=w2x_inventory_item_005560b0(inventory,nullptr,index);
        kotor_fastcall_006196a0(self,nullptr,list,item,creature);
        ++index;
    }
}
