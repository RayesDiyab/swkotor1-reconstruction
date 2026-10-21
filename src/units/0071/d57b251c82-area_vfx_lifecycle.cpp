// First complete areaVFX leaves; remaining family bodies/table are distinct future source tiers.
#include "9136b2d0c5-area_vfx.hpp"
#if !defined(_M_IX86)
#error AreaVFX providers require native Windows x86
#endif
using namespace kotor_w3;
extern "C" std::uint32_t __fastcall recovered_client_area_apply_fire_and_forget_visual_effect_00604c90(
    void* area,void*,std::uint32_t effect,float x,float y,float z) {
    void* allocation=live_operator_new(0x130);
    void* vfx=nullptr;
    try { if(allocation)vfx=area_vfx_construct_0068fe70(allocation,nullptr,1); }
    catch(...) { live_free(allocation);throw; } // original EHstate0 releases raw block on constructor failure
    if(vfx==nullptr)return 0;
    if(area_vfx_load_00690010(vfx,nullptr,static_cast<std::uint16_t>(effect))==0) {
        delete_object(vfx);return 0;
    }
    vslot<void(__fastcall*)(void*,void*,void*)>(vfx,0x94)(vfx,nullptr,rp(area,0x184));
    st<U>(vfx,0x11c,1);st<U>(vfx,0x118,1);
    Vector3 position{x,y,z};
    vslot<void(__fastcall*)(void*,void*,const Vector3*)>(vfx,0x8c)(vfx,nullptr,&position);
    area_vfx_play_00690510(vfx,nullptr,1);
    area_vfx_announce_00690940(vfx,nullptr);
    return 1;
}
extern "C" void* __fastcall area_vfx_construct_0068fe70(void* self,void*,U registered) {
    kotor_m15a_object_construct_0063e530(self,nullptr,0x0b);
    try {
        st<const void*>(self,0,&area_vfx_methods_00754278);
        U day,tick;
        frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(),nullptr),nullptr,&day,&tick);
        st<U>(self,0x3c,day);st<U>(self,0x40,tick);
        st<U>(self,0x100,0);st<U>(self,0x110,0);st<U>(self,0x114,0);
        st<U>(self,0x118,0);st<U>(self,0x11c,1);st<U>(self,0x104,0);st<std::uint16_t>(self,0x10c,0);
        if(registered!=0) {
            area_vfx_register_existing_004d80e0(kotor_fastcall_005ed570(client_app(),nullptr),nullptr,
                reinterpret_cast<U*>(static_cast<unsigned char*>(self)+4),self,0);
            recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(),nullptr),nullptr,self);
        }
        st<U>(self,0xfc,0);st<U>(self,0x120,0);st<U>(self,0x124,0);st<U>(self,0x128,0);st<U>(self,0x12c,0);
        st<U>(self,0x108,0xffffffffu);
    } catch(...) { kotor_m15e_object_destruct_0063e800(self,nullptr);throw; }
    return self;
}
extern "C" void __fastcall area_vfx_destroy_0068ff70(void* self,void*) {
    st<const void*>(self,0,&area_vfx_methods_00754278);
    try {
        const U id=ld<U>(self,4); // original caches ID before re-reading client manager
        recovered_object_array_erase_004d8290(kotor_fastcall_005ed570(client_app(),nullptr),nullptr,id);
        void* sound=rp(self,0xfc);if(sound!=nullptr)delete_object(sound);
        void* animation=rp(self,0x68);
        if(animation!=nullptr) {
            vslot<void(__fastcall*)(void*,void*)>(animation,0x78)(animation,nullptr);
            animation=rp(self,0x68);if(animation!=nullptr)delete_object(animation);
            st<U>(self,0x68,0);
        }
    } catch(...) { kotor_m15e_object_destruct_0063e800(self,nullptr);throw; }
    kotor_m15e_object_destruct_0063e800(self,nullptr);
}
extern "C" void* __fastcall area_vfx_scalar_destroy_00690960(void* self,void*,U flags) {
    area_vfx_destroy_0068ff70(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self; // original returns savedthis even after free
}
extern "C" void __fastcall area_vfx_announce_00690940(void* self,void*) {
    const Vector3* position=reinterpret_cast<const Vector3*>(static_cast<unsigned char*>(self)+0x24);
    const U effect=ld<U>(self,0x108);
    area_vfx_client_announce_005edf60(client_app(),nullptr,effect,1,position);
}
extern "C" void __fastcall area_vfx_client_announce_005edf60(void* client,void*,U effect,U kind,const Vector3* position) {
    area_vfx_internal_announce_005fb960(rp(client,4),nullptr,effect,kind,position);
}
extern "C" void __fastcall area_vfx_ai_update_00690980(void* self,void*) {
    if(ld<U>(self,0x114)!=0)return;
    U day,tick,out_day,out_tick;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(),nullptr),nullptr,&day,&tick);
    const U old_tick=ld<U>(self,0x40),old_day=ld<U>(self,0x3c);
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client_app(),nullptr),nullptr,
        day,tick,old_day,old_tick,&out_day,&out_tick);
    st<U>(self,0x44,out_tick);st<U>(self,0x3c,day);st<U>(self,0x40,tick);
    void* sound=rp(self,0xfc);
    if(sound!=nullptr)kotor_m15_sound_source_set_position_005d59e0(sound,nullptr,
        reinterpret_cast<const Vector3*>(static_cast<unsigned char*>(self)+0x24),1.5f);
    U remaining=ld<U>(self,0x104);
    if(remaining>0) {
        const U elapsed=ld<U>(self,0x44);
        if(remaining>elapsed)st<U>(self,0x104,remaining-elapsed);
        else {
            const std::uint16_t mode=ld<std::uint16_t>(self,0x10c);
            st<U>(self,0x104,0);
            if(mode==1) {
                if(ld<U>(self,0x118)!=0) {
                    st<U>(self,0x100,ld<U>(self,0x128)!=0?0u:3000u);
                    st<U>(self,0x110,1);
                } else area_vfx_play_00690510(self,nullptr,2);
            } else if(mode==3) { st<U>(self,0x100,3000);st<U>(self,0x110,1); }
        }
    }
    if(ld<U>(self,0x110)==0)return;
    remaining=ld<U>(self,0x100);
    const U elapsed=ld<U>(self,0x44);
    if(remaining>elapsed)st<U>(self,0x100,remaining-elapsed);
    else {
        if(ld<U>(self,0x11c)!=0) {delete_object(self);return;}
        if(ld<U>(self,0x124)!=0)vslot<void(__fastcall*)(void*,void*,void*)>(self,0x94)(self,nullptr,nullptr);
        st<U>(self,0x114,1);st<U>(self,0x110,0);
    }
}
