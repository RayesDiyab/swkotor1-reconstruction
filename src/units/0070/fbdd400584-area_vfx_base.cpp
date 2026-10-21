// Faithful shared base position/highlight providers required by areaVFX; existing lower owners retained.
#include "9136b2d0c5-area_vfx.hpp"
using namespace kotor_w3;
namespace sound_volume29_required { unsigned __fastcall predicate_006054e0(void*,void*,void*); }
extern "C" {
// Canonical0831 storage owner; no duplicate cell.
extern volatile U kotor_m15e_cell_0083395c;
}
extern "C" void __fastcall area_vfx_base_set_position_0063f340(void* self,void*,const Vector3* position) {
    std::memcpy(static_cast<unsigned char*>(self)+0x24,position,12);
    if(kotor_m15e_cell_0083395c!=0)
        vslot<void*(__fastcall*)(void*,void*)>(self,0x2c)(self,nullptr); // original result discarded
    const U8 type=ld<U8>(self,8);
    if(type==9 || type==10 || type==6) { // original00746638/39/35
        bool check_area=true;
        if(type==9) {
            void* placeable=vslot<void*(__fastcall*)(void*,void*)>(self,0x4c)(self,nullptr);
            void* server_placeable=kotor_fastcall_00682e00(placeable,nullptr);
            if(server_placeable!=nullptr && ld<U>(server_placeable,0x440)!=0 && ld<U>(server_placeable,0x444)==0)check_area=false;
        }
        if(check_area) {
            void* area=rp(self,0x20);
            if(area!=nullptr && sound_volume29_required::predicate_006054e0(area,nullptr,const_cast<Vector3*>(position))!=0) {
                recovered_client_object_set_fade_state_0063e220(self,nullptr,1);
                recovered_client_object_set_desired_fade_state_0063ce60(self,nullptr,1,0,0,0);
            }
        }
    }
    void* animation=rp(self,0x68);
    if(animation!=nullptr && ld<U>(self,0x50)==0)
        vslot<void(__fastcall*)(void*,void*,const Vector3*)>(animation,0x68)(animation,nullptr,position);
    vslot<void(__fastcall*)(void*,void*)>(self,0x90)(self,nullptr);
}
extern "C" U __fastcall area_vfx_base_unhilite_0063cb30(void* self,void*,U channel) {
    void* animation=rp(self,0x68);
    return animation==nullptr?0u:area_vfx_anim_unhilite_006a0440(animation,nullptr,channel);
}
extern "C" U __fastcall area_vfx_anim_unhilite_006a0440(void* self,void*,U channel) {
    if(rp(self,0xb8)==nullptr || static_cast<U8>(channel)>=5)return 0;
    st<U>(self,0x90+static_cast<U8>(channel)*4u,0);
    kotor_m15e_anim_base_refresh_hilite_0069ec00(self,nullptr);
    return 1;
}
extern "C" U __fastcall area_vfx_base_hilite_0063cb00(void* self,void*,const Vector3* color,U channel,U unused,U flag) {
    (void)unused; // original ignores third stack argument while forwarding first/second/fourth
    void* animation=rp(self,0x68);
    return animation==nullptr?0u:area_vfx_anim_hilite_006a03e0(animation,nullptr,color,channel,flag);
}
extern "C" U __fastcall area_vfx_anim_hilite_006a03e0(void* self,void*,const Vector3* color,U channel,U flag) {
    if(rp(self,0xb8)==nullptr || static_cast<U8>(channel)>=5)return 0;
    const U index=static_cast<U8>(channel);
    std::memcpy(static_cast<unsigned char*>(self)+0x54+index*12,color,12);
    st<U>(self,0x90+index*4,1);st<U>(self,0xa4+index*4,flag);
    kotor_m15e_anim_base_refresh_hilite_0069ec00(self,nullptr);
    return 1;
}
