#include "3860c80f01-model_family_b_support.h"
using namespace model_family_b_detail;

extern "C" {
// Original bug: the head-replace call is skipped for nonnull head and faults for null.
U __fastcall model_family_b_0069b410(void* self,void*,U from,U to,U count,U attrs,U keep,U first) noexcept(false) {
    Name a,b;
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(from),nullptr,a.text);
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(to),nullptr,b.text);
    c2da_current_strlwr_007135df(a.text);c2da_current_strlwr_007135df(b.text);
    void* const head=pv(self,-0xC);
    if(head==nullptr) {
        const auto* const table=*reinterpret_cast<const aurora_model::GobVTable* const volatile*>(head);
        table->s030_gob_replace_texture(nullptr,nullptr,wd(a.text),wd(b.text),count,attrs,first); // 0069B458 actual null read
    }
    if(keep!=0) {
        save_ref(at(self,-0x34),to);
        st<U>(self,-0x24,1);st<U8>(self,-0x38,static_cast<U8>(count));st<U8>(self,-0x37,static_cast<U8>(first));
        copy_forward(at(self,-0x4C),reinterpret_cast<void*>(attrs+first*2u),count*2u);
    }
    return model_family_b_0069c260(head_tw(self),nullptr,from,to,count,attrs,keep,first);
}
U __fastcall model_family_b_0069b4f0(void* self,void*) noexcept(false) {
    void* const head=pv(self,-0xC);
    if(head!=nullptr) {
        if(ld<U>(self,-0x24)!=0) {
            Ref empty;empty.construct_00406d80(kEmpty_0073d71c);
            void* const tw=head_tw(self);
            avt(tw)->s014(tw,nullptr,0xFE,wd(&empty),wd(at(self,-0x34)),ld<U8>(self,-0x38),wd(at(self,-0x4C)),0,ld<U8>(self,-0x37));
        } else gvt(head)->s03c_gob_restore_texture(gob(head),nullptr);
    }
    return model_family_b_0069c400(head_tw(self),nullptr);
}
U __fastcall model_family_b_0069b6a0(void* self,void*,U enabled,U keep,U ref) noexcept(false) {
    void* head=pv(self,-0xC);
    if(head==nullptr)return 0;
    Name name;const char* text=kEmpty_0073d71c;
    if(enabled!=0) {
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(ref),nullptr,name.text);
        head=pv(self,-0xC);text=name.text;
    }
    gvt(head)->s048_gob_set_environment_map(gob(head),nullptr,wd(text));
    if(keep!=0) {save_ref(at(self,-0x20),ref);st<U>(self,-0x10,enabled);}
    return model_family_b_0069c6d0(head_tw(self),nullptr,enabled,keep,ref);
}
U __fastcall model_family_b_0069b7c0(void* self,void*) noexcept(false) {
    void* head=pv(self,-0xC);
    if(head==nullptr)return 0;
    Name name;const char* text=kEmpty_0073d71c;
    if(ld<U>(self,-0x10)!=0) {
        gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(at(self,-0x20)),nullptr,name.text);
        head=pv(self,-0xC);text=name.text;
    }
    gvt(head)->s048_gob_set_environment_map(gob(head),nullptr,wd(text));
    return model_family_b_0069c8a0(head_tw(self),nullptr);
}
void __fastcall model_family_b_0069b840(void* self,void*) noexcept(false) {
    void* const head=pv(self,-0xC);
    if(head!=nullptr)gvt(head)->s0bc_gob_turn_off_shadows(gob(head),nullptr);
    recovered_anim_base_turn_off_shadows_0069dc30(head_tw(self),nullptr);
}
U __fastcall model_family_b_0069b870(void* self,void*,U ref,U flag) noexcept(false) {
    if(pv(self,-0xC)==nullptr)return 0;
    Name name;
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(ref),nullptr,name.text);
    c2da_current_strlwr_007135df(name.text);
    void* const head=pv(self,-0xC);
    gvt(head)->s150_gob_enable_render_bumped_out(gob(head),nullptr,wd(name.text),flag);
    return kotor_m15e_anim_base_enable_bumped_out_shell_0069dd90(head_tw(self),nullptr,ref,flag);
}
U __fastcall model_family_b_0069b8e0(void* self,void*) noexcept(false) {
    void* const head=pv(self,-0xC);
    if(head==nullptr)return 0;
    gvt(head)->s154_gob_disable_render_bumped_out(gob(head),nullptr);
    return model_disable_bumped_out_shell_0069ddf0(head_tw(self),nullptr);
}
U __fastcall model_family_b_0069b910(void* self,void*,U enabled) noexcept(false) {
    void* const head=pv(self,-0xC);
    if(head==nullptr)return 0;
    // 00449AB0 consumes only the low byte; the original SETNE DL leaves other bits unspecified.
    gvt(head)->s144_gob_set_motion_blurred(gob(head),nullptr,enabled!=0?1u:0u);
    return model_set_motion_blur_0069de10(head_tw(self),nullptr,enabled);
}
}
