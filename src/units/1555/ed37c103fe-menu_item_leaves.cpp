#include "0aec0b9933-menu_row_common.h"
using namespace menu_row;
extern "C" menu_resources::Ref* __fastcall kotor_fastcall_005556b0(void* self,void*,menu_resources::Ref* output) {
    void* base=kotor_m15b_item_get_base_item_005b4790(self,nullptr);
    if(base==nullptr){output->construct_00406d80("");return output;}
    // The selected 005B3050 consumes only the signed low WORD of variation.
    U variation=load<std::uint8_t>(self,load<std::uint8_t>(base,0x18)!=0?0x266:0x264);
    recovered_base_item_get_icon_resref_005b3050(base,nullptr,output,variation,0x6D);
    return output;
}
extern "C" U __fastcall kotor_fastcall_0059b6a0(void* self,void*,std::uint8_t index) {
    if(index>=load<std::uint8_t>(self,0x130))return 0xFFFFFFFFu;
    return load<U>(load<void*>(self,0x12C),U{index}*4);
}
extern "C" U __fastcall kotor_fastcall_00555a60(void* self,void*,std::uint8_t index) {
    if(static_cast<I>(index)>=load<I>(self,0x248))return 0;
    void* property=at(load<void*>(self,0x250),U{index}*0x1C);
    if(property==nullptr||load<std::uint16_t>(property)!=0xA)return 0;
    U kind=U{load<std::uint16_t>(property,6)}-1;
    if(kind>0x11||kind==6||kind==12)return 0;
    if(load<std::uint8_t>(property,0xC)==0)return 0;
    // Original index-byte table maps kinds 0..17 to these nine destinations.
    if(kind==0)return load<std::uint8_t>(self,0x28C);
    if(kind>=1&&kind<=5){
        I amount=load<I>(self,0x258);
        if(amount<=0)return 0;
        return static_cast<U>(amount/static_cast<I>(6-kind));
    }
    if(kind>=7&&kind<=11)return load<std::uint8_t>(property,0x10);
    if(kind>=13)return 1;
    return 0;
}
