// Original derived visual-effect animation family; full original tables supplied separately.
#include "837b70edb8-area_vfx_native.hpp"
using namespace kotor_w3;
using namespace area_vfx_native;
extern "C" {
struct AreaVfxModelMethods;
extern const AreaVfxModelMethods area_vfx_model_methods_00758b68;
void* __fastcall kotor_m15e_anim_base_construct_0069dfb0(void*,void*);
void __fastcall kotor_m15e_anim_base_destruct_0069e140(void*,void*);
U __fastcall kotor_m15e_anim_base_unload_model_0069d740(void*,void*);
void __stdcall c2da_destroy_array_006fa852(void*,U,U,void(__fastcall*)(void*,void*));
void* __fastcall area_vfx_model_construct_006e5ce0(void*,void*);
void __fastcall area_vfx_model_destroy_006e5d00(void*,void*);
}
extern "C" void* __fastcall area_vfx_model_construct_006e5ce0(void* self,void*) {
    kotor_m15e_anim_base_construct_0069dfb0(self,nullptr);
    st<const void*>(self,0,&area_vfx_model_methods_00758b68);
    st<U8>(self,0xc4,8);return self;
}
extern "C" void __fastcall area_vfx_model_destroy_006e5d00(void* self,void*) {
    st<const void*>(self,0,&area_vfx_model_methods_00758b68);
    try { kotor_m15e_anim_base_unload_model_0069d740(self,nullptr); }
    catch(...) { kotor_m15e_anim_base_destruct_0069e140(self,nullptr);throw; }
    kotor_m15e_anim_base_destruct_0069e140(self,nullptr);
}
extern "C" void* __fastcall area_vfx_model_delete_006d9070(void* self,void*,U flags) {
    if((flags&2u)!=0) {
        void* cookie=static_cast<unsigned char*>(self)-4;
        const U count=ld<U>(cookie,0);
        // Existing0186 normal-path iterator retained; full shared006FA852 exceptional closure remains explicit.
        c2da_destroy_array_006fa852(self,0xf0,count,&area_vfx_model_destroy_006e5d00);
        if((flags&1u)!=0)live_free(cookie);
        return cookie;
    }
    area_vfx_model_destroy_006e5d00(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self;
}
extern "C" U __fastcall area_vfx_model_load_006e5d50(void* self,void*,void* reference,U) {
    char name[17];
    gui_load_resref_copy_to_string_00405fb0(reference,nullptr,name);
    void* gob=aurora_new_aur_object_00449cc0(name,"visualeffect",nullptr,nullptr);
    st<void*>(self,0xb8,gob);
    if(gob==nullptr)return 0;
    ResRef out;
    c2da_copy_00406120(at(self,0x2c),nullptr,&out,reference);
    st<U>(self,0x3c,0);
    gob=rp(self,0xb8);
    (void)vslot<U(__fastcall*)(void*,void*)>(gob,0xe4)(gob,nullptr);
    return 1;
}
extern "C" void* __fastcall area_vfx_model_animation_name_006e5dc0(void*,void*,void* out,U animation) {
    const char* name;
    switch(U16(animation)) {
      case 0:name="default";break;
      case 1:name="impact";break;
      case 2:name="duration";break;
      case 3:name="cessation";break;
      default:name="";break;
    }
    frame_LogStringCtor_005e5a90(out,nullptr,name);return out;
}
extern "C" U __fastcall area_vfx_model_animate_006e5e60(void* self,void*,void* gob,U animation,U arg3,U arg4,U arg5) {
    LocalString name;
    if(gob==nullptr)return 0;
    ExoString temporary;
    void* result=vslot<void*(__fastcall*)(void*,void*,void*,U)>(self,0x40)(self,nullptr,&temporary,animation);
    try { async_assign_string_005e5c50(&name.value,nullptr,result); }
    catch(...) { frame_LogStringDtor_005e5c20(&temporary,nullptr);throw; }
    frame_LogStringDtor_005e5c20(&temporary,nullptr);
    const auto target=vslot<U(__fastcall*)(void*,void*,const char*,U,U,U)>(gob,0x18);
    return target(gob,nullptr,async_string_cstr_005e5670(&name.value,nullptr),arg3,arg4,arg5)!=0;
}
extern "C" U __fastcall area_vfx_load_model_006903d0(void* self,void*,void* reference,U,U) {
    if(rp(self,0x68)==nullptr) {
        void* allocation=live_operator_new(0xf0);
        void* model=nullptr;
        try { if(allocation!=nullptr)model=area_vfx_model_construct_006e5ce0(allocation,nullptr); }
        catch(...) { live_free(allocation);throw; }
        st<void*>(self,0x68,model);
        if(model==nullptr)return 0;
    }
    (void)c2da_ref_00405fe0(reference,nullptr); // original initial call, discarded result
    void* model=rp(self,0x68);
    if(vslot<U(__fastcall*)(void*,void*,void*,U)>(model,0xc)(model,nullptr,reference,0xff)==0) {
        char message[256];
        format_006fadb0(message,"CSWCVisualEffect::LoadModel: Failed to load visual effect model '%s'.",c2da_ref_00405fe0(reference,nullptr));
        return 0;
    }
    model=rp(self,0x68);
    void* gob=vslot<void*(__fastcall*)(void*,void*,U)>(model,8)(model,nullptr,0xff);
    if(gob==nullptr)return 0;
    (void)vslot<U(__fastcall*)(void*,void*,U)>(gob,0x7c)(gob,nullptr,0x0b);
    (void)vslot<U(__fastcall*)(void*,void*,U)>(gob,0x74)(gob,nullptr,reinterpret_cast<U>(self));
    (void)vslot<U(__fastcall*)(void*,void*,U)>(gob,0xc4)(gob,nullptr,3);
    Vector3 color{1.0f,1.0f,1.0f};
    vslot<U(__fastcall*)(void*,void*,U,U,U,U)>(self,0xa4)(self,nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(&color)),3,0,0);
    if(ld<U>(self,0x12c)!=0)(void)vslot<U(__fastcall*)(void*,void*)>(gob,0xdc)(gob,nullptr);
    return 1;
}
extern "C" void* __fastcall area_vfx_base_name_0063e730(void*,void*,void* out) {
    frame_LogStringCtor_005e5a90(out,nullptr,"");return out;
}
