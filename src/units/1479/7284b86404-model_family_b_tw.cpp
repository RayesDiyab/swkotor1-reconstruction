#include "3860c80f01-model_family_b_support.h"
using namespace model_family_b_detail;

namespace {
// Both child branches use the same two original stack name buffers.
void replace_child(void* self,U child,U saved,U flag,U attributes,U count_slot,U first_slot,
                   U from,U to,U count,U attrs,U keep,U first,Name& a,Name& b) {
    if(pv(self,child)==nullptr)return;
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(from),nullptr,a.text);
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(to),nullptr,b.text);
    c2da_current_strlwr_007135df(a.text);c2da_current_strlwr_007135df(b.text);
    void* const node=pv(self,child);                                      // reload after conversions
    gvt(node)->s030_gob_replace_texture(gob(node),nullptr,wd(a.text),wd(b.text),count,attrs,first);
    if(keep!=0) {
        save_ref(at(self,saved),to);
        st<U>(self,flag,1);st<U8>(self,count_slot,static_cast<U8>(count));st<U8>(self,first_slot,static_cast<U8>(first));
        copy_forward(at(self,attributes),reinterpret_cast<void*>(attrs+first*2u),count*2u);
    }
}
void restore_child_environment(void* self,U child,U flag,U saved,Name& name) {
    void* node=pv(self,child);
    if(node==nullptr)return;
    const char* text=kEmpty_0073d71c;
    if(ld<U>(self,flag)!=0) {
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(at(self,saved)),nullptr,name.text);
        node=pv(self,child);text=name.text;
    }
    gvt(node)->s048_gob_set_environment_map(gob(node),nullptr,wd(text));
}
}

extern "C" {
U __fastcall model_family_b_0069c260(void* self,void*,U from,U to,U count,U attrs,U keep,U first) {
    Name a,b;
    replace_child(self,0x16C,0x11C,0x12C,0xF0,0x104,0x105,from,to,count,attrs,keep,first,a,b);
    replace_child(self,0x170,0x130,0x140,0x106,0x11A,0x11B,from,to,count,attrs,keep,first,a,b);
    return kotor_m15e_anim_base_replace_texture_0069cfd0(self,nullptr,from,to,count,attrs,keep,first);
}
U __fastcall model_family_b_0069c400(void* self,void*) {
    if(pv(self,0x16C)!=nullptr && ld<U>(self,0x12C)!=0) {
        Ref empty;empty.construct_00406d80(kEmpty_0073d71c);
        avt(self)->s014(self,nullptr,0xFD,wd(&empty),wd(at(self,0x11C)),ld<U8>(self,0x104),wd(at(self,0xF0)),0,ld<U8>(self,0x105));
    }
    if(pv(self,0x170)!=nullptr && ld<U>(self,0x140)!=0) {
        Ref empty;empty.construct_00406d80(kEmpty_0073d71c);
        avt(self)->s014(self,nullptr,0xFC,wd(&empty),wd(at(self,0x130)),ld<U8>(self,0x11A),wd(at(self,0x106)),0,ld<U8>(self,0x11B));
    }
    return kotor_m15e_anim_base_restore_texture_0069d0a0(self,nullptr);
}
U __fastcall model_family_b_0069c6d0(void* self,void*,U enabled,U keep,U ref) {
    Name name;
    const char* text=kEmpty_0073d71c;
    if(enabled!=0) {
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(ref),nullptr,name.text);
        text=name.text;
    }
    void* node=pv(self,0x16C);
    if(node!=nullptr)gvt(node)->s048_gob_set_environment_map(gob(node),nullptr,wd(text));
    node=pv(self,0x170);
    if(node!=nullptr)gvt(node)->s048_gob_set_environment_map(gob(node),nullptr,wd(text));
    if(keep!=0) {
        save_ref(at(self,0x144),ref);st<U>(self,0x154,enabled);
        save_ref(at(self,0x158),ref);st<U>(self,0x168,enabled);
    }
    return recovered_anim_base_set_env_map_0069d7b0(self,nullptr,enabled,keep,reinterpret_cast<void*>(ref));
}
U __fastcall model_family_b_0069c8a0(void* self,void*) {
    Name name;
    restore_child_environment(self,0x16C,0x154,0x144,name);
    restore_child_environment(self,0x170,0x168,0x158,name);
    return kotor_m15e_anim_base_restore_env_map_0069d8b0(self,nullptr);
}
// 0069CA70: GetAnimationName is constructed even for invalid parts or absent child.
U __fastcall model_family_b_0069ca70(void* self,void*,U model,U part,U animation,U rate,U blend,U flags) {
    StringSlot name;
    avt(self)->s040(self,nullptr,name.get(),animation);name.live=true;
    const U8 selected=static_cast<U8>(part);
    if(selected==0xFF)return recovered_anim_base_internal_0069e8d0(self,nullptr,model,animation,rate,blend,flags);
    U offset;
    if(selected==0xFD)offset=0x16C;
    else if(selected==0xFC)offset=0x170;
    else return 0;
    void* node=pv(self,offset);
    if(node==nullptr)return 0;
    const auto* const table=gvt(node);                                   // before CStr
    char* const text=WindowCStr005e5670(name.get(),nullptr);
    U const result=table->s018_gob_play_animation(gob(pv(self,offset)),nullptr,wd(text),rate,blend,flags);
    if(result==0) {
        node=pv(self,offset);
        constexpr char fallback_0073df6c[]="default";
        gvt(node)->s018_gob_play_animation(gob(node),nullptr,wd(fallback_0073df6c),rate,blend,flags);
    }
    return result;                                                       // preserve first result, not fallback
}
}
