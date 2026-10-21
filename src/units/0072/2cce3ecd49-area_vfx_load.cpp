// Full original00690010 loader; original cached rule-column cells are existing owners.
#include "32a11cddfd-area_vfx_native.hpp"
using namespace kotor_w3;
using namespace area_vfx_native;
extern "C" {
extern volatile U kotor_rules_cell32_007a21b4,kotor_rules_cell32_007a21b8,kotor_rules_cell32_007a21bc;
extern volatile U kotor_rules_cell32_007a21c0,kotor_rules_cell32_007a21c4,kotor_rules_cell32_007a21c8;
extern volatile U kotor_rules_cell32_007a21cc,kotor_rules_cell32_007a21d0,kotor_rules_cell32_007a21d4;
extern volatile U kotor_rules_cell32_007a21d8,kotor_rules_cell32_007a21dc;
}
extern "C" U __fastcall area_vfx_load_00690010(void* self,void*,U effect) {
    LocalString model;
    LocalString row(effect);
    I32 orient;
    if(object_vfx_table_00414260(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21d0),&orient)==0)
        orient=0;
    st<U>(self,0x120,orient!=0?1u:0u);
    bool found=false;
    if(ld<std::int8_t>(kotor_fastcall_005ed700(client_app(),nullptr),0)>=0 &&
       object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21cc),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21b4),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21b8),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21bc),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21c0),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21c4),&model.value)!=0)
        found=true;
    else if(object_vfx_table_00413ec0(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21c8),&model.value)!=0)
        found=true;
    I32 flash;
    float middle,last;
    if(object_vfx_table_00414260(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21d4),&flash)==0)flash=0;
    if(object_vfx_table_00414080(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21d8),&middle)==0)
        std::memset(&middle,0,4);
    if(object_vfx_table_00414080(visual_table(),nullptr,&row.value,I32(kotor_rules_cell32_007a21dc),&last)==0)
        std::memset(&last,0,4);
    if(flash==1) {
        void* app=client_app(); // original caches app before the x87 conversion
        const I32 duration=milliseconds(last);
        object_vfx_camera_00641730(kotor_fastcall_005ed530(app,nullptr),nullptr,duration);
    } else if(flash==2) {
        void* app=client_app();
        const I32 second=milliseconds(middle);
        const I32 first=milliseconds(last);
        object_vfx_camera_006416f0(kotor_fastcall_005ed530(app,nullptr),nullptr,first,second);
    }
    if(found) {
        ResRef reference;
        async_ref_construct_string_00406d60(&reference,nullptr,&model.value);
        if(vslot<U(__fastcall*)(void*,void*,const ResRef*,U,U)>(self,0x78)(self,nullptr,&reference,0xff,8)==0)
            return 0;
    }
    if(effect==39 || effect==40)vslot<U(__fastcall*)(void*,void*,U)>(self,0xa8)(self,nullptr,3);
    st<U>(self,0x108,effect);
    return 1;
}
