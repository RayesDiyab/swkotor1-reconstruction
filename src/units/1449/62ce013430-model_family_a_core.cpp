#include "98e20b8cb4-model_family_a_internal.h"
using namespace model_family_a;
namespace {
const char talk_00754c00[]="talk";
const char head_hook_007549f0[]="GoggleHook";
}
extern "C" {
U __fastcall model_family_a_00699180(void* self,void*,U a,U b,U c,U d,U e,U f) noexcept(false){
    if(!model_family_b_0069c260(tw(self),nullptr,a,b,c,d,e,f))return 0;
    return model_family_b_0069b410(head(self),nullptr,a,b,c,d,e,f)?1u:0u;
}
U __fastcall model_family_a_00699200(void* self,void*) noexcept(false){return model_family_b_0069b4f0(head(self),nullptr);}
U __fastcall model_family_a_006992d0(void* self,void*,U a,U b,U c) noexcept(false){
    if(!model_family_b_0069c6d0(tw(self),nullptr,a,b,c))return 0;
    return model_family_b_0069b6a0(head(self),nullptr,a,b,c)?1u:0u;
}
U __fastcall model_family_a_00699370(void* self,void*) noexcept(false){
    if(!model_family_b_0069c8a0(tw(self),nullptr))return 0;
    return model_family_b_0069b7c0(head(self),nullptr)?1u:0u;
}
U __fastcall model_family_a_006994a0(void* self,void*,U a,U b,U c,U d) noexcept(false){
    void* body=ld<void*>(self,vb(self,-8,1)+0xb0);
    if(a&&b){
        if(body)gvt(body)->s120_gob_play_out_of_order_animation(gob(body),nullptr,word(talk_00754c00),a,b,c,d);
        void* h=ld<void*>(self,vb(self,-8,2)+0x3c);
        if(h)gvt(h)->s120_gob_play_out_of_order_animation(gob(h),nullptr,word(talk_00754c00),a,b,c,d);
    }else{
        if(body)gvt(body)->s124_gob_stop_out_of_order_animation(gob(body),nullptr,word(talk_00754c00));
        void* h=ld<void*>(self,vb(self,-8,2)+0x3c);
        if(h)gvt(h)->s124_gob_stop_out_of_order_animation(gob(h),nullptr,word(talk_00754c00));
    }
    return 1;
}
U __fastcall model_family_a_00699640(void* self,void*,U part,U animation,U rate,U blend,U flags) noexcept(false){
    const U* const table=ld<const U*>(self,-8);
    if((part&0xffu)==0xffu){
        const U body=ld<U>(self,table[1]+0xb0);
        return model_family_a_00699fd0(off(self,table[3]+0x54),nullptr,body,0xff,animation,rate,blend,flags);
    }
    const U h=ld<U>(self,table[2]+0x3c);
    return model_family_b_0069ca70(off(self,table[1]-8),nullptr,h,part,animation,rate,blend,flags);
}
U __fastcall model_family_a_006996b0(void* self,void*,U a,U b,U c,U d,U e) noexcept(false){return recovered_anim_base_internal_0069e8d0(tw(self),nullptr,a,b,c,d,e);}
U __fastcall model_family_a_006996c0(void* self,void*,U a,U b,U c,U d,U e,U f) noexcept(false){return model_set_animation_on_subobject_internal_0069d5a0(tw(self),nullptr,a,b,c,d,e,f);}
void __fastcall model_family_a_006997c0(void* self,void*) noexcept(false){
    model_family_b_0069b840(head(self),nullptr);
    model_family_a_0069a180(wield(self),nullptr);
}
void __fastcall model_family_a_006997f0(void* self,void*,U value) noexcept(false){model_family_a_0069a190(wield(self),nullptr,value);}
U __fastcall model_family_a_00699800(void* self,void*,U a,U b) noexcept(false){return model_family_b_0069b870(head(self),nullptr,a,b);}
U __fastcall model_family_a_00699810(void* self,void*) noexcept(false){return model_family_b_0069b8e0(head(self),nullptr);}
U __fastcall model_family_a_00699820(void* self,void*,U value) noexcept(false){return model_family_b_0069b910(head(self),nullptr,value);}
void __fastcall model_family_a_00699b90(void* self,void*) noexcept(false){
    if(ld<U>(self,vb(self,-8,3)+0x44)==0)return;
    void* item=kotor_m15_client_get_item_by_id_005ed5a0(client(),nullptr,ld<U>(self,vb(self,-8,2)+0x40));
    if(item){
        void* object=off(item,0x10);
        const auto* object_table=ld<const KotorM15ObjectMethods_007516c8*>(object,0);
        void* model=object_table->s098_get_model(object,nullptr,0xff,1);
        if(model){
            void* h=ld<void*>(self,vb(self,-8,2)+0x3c);
            gvt(model)->s050_gob_attach_to_object(model,nullptr,h,head_hook_007549f0,0);
        }
    }
    kotor_m15e_anim_base_refresh_hilite_0069ec00(tw(self),nullptr);
    st<U>(self,vb(self,-8,3)+0x44,0);
}
U __fastcall model_family_a_00699fd0(void* self,void*,U a,U part,U c,U d,U e,U f) noexcept(false){
    if((part&0xffu)!=0xffu)return 0;
    void* base=off(self,vb(self,-0x5c,1)-0x5c);
    return avt(base)->s0f0(base,nullptr,a,c,d,e,f);
}
void __fastcall model_family_a_0069a180(void* self,void*) noexcept(false){recovered_anim_base_turn_off_shadows_0069dc30(off(self,vb(self,-0x5c,1)-0x5c),nullptr);}
void* __fastcall model_family_a_0069aa60(void* self,void*,U flags) noexcept(false){
    void* allocation=off(self,-0x5c);
    kotor_m15e_anim_base_wield_destruct_00699ed0(self,nullptr);
    kotor_m15e_anim_base_tw_destruct_0069cd10(self,nullptr);
    if(flags&1u)live_free(allocation);
    return allocation;
}
}
