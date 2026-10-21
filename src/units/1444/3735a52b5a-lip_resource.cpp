#include "51e94608b6-lip_resource.h"
#include "84feb2ce51-resources.h"
#include <cstring>

using U=std::uint32_t;
using I=std::int32_t;
using B=std::uint8_t;
using Ref=menu_resources::Ref;
using String=kotor_reconstruction::StartupString;
namespace {
using Delete=void*(__fastcall*)(void*,void*,U);
using Constant=U(__stdcall*)();
using Callback=U(__fastcall*)(void*,void*);
struct HelperMethods { Delete destroy; };
struct ResourceMethods { Delete destroy;Constant constant;Constant zero;Callback freed;Callback serviced; };
static_assert(sizeof(HelperMethods)==4&&sizeof(ResourceMethods)==20,"Original LIP method tables");
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
void* at(void*p,U o){return static_cast<B*>(p)+o;}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
}
extern "C" {
menu_resources::Manager* __cdecl c2da_manager_007a39e8();
const char* __cdecl c2da_empty_0073d71c();
Ref* __fastcall c2da_default_ref_00405ed0(Ref*,void*);
Ref* __fastcall unresolved_ref_assign_00406290(Ref*,void*,Ref*,const char*);
Ref* __fastcall c2da_copy_00406120(Ref*,void*,Ref*,Ref*);
Ref* __fastcall async_ref_construct_string_00406d60(Ref*,void*,String*);
int __fastcall resource_equal_004060b0(Ref*,void*,Ref*);
int __fastcall c2da_equal_text_004060d0(Ref*,void*,const char*);
int __fastcall c2da_not_equal_text_00406060(Ref*,void*,const char*);
void __fastcall c2da_cancel_00409b00(void*,void*);
int __fastcall c2da_release_00409cf0(menu_resources::Manager*,void*,void*);
void* __fastcall c2da_lookup_004074d0(menu_resources::Manager*,void*,Ref*,U);
void __fastcall c2da_register_00407680(menu_resources::Manager*,void*,Ref*,U,void*);
void __fastcall c2da_request_00408620(void*,void*);
U __fastcall c2da_current_resource_demand_00409b10(void*,void*);
U __fastcall c2da_release_demand_00409b80(void*,void*);
void* __fastcall c2da_current_resource_data_00404dd0(void*,void*);
void* __fastcall c2da_resource_base_ctor_00406460(void*,void*);
void __fastcall c2da_resource_base_destroy_00409d40(void*,void*);
U __stdcall c2da_resource_constant_word_00406490();
U __stdcall c2da_resource_constant_word_0063e7f0();
U __fastcall aurora_txi_resource_freed_00710e60(void*,void*);
U __fastcall aurora_generic_resource_serviced_005d4c60(void*,void*);
I __cdecl kotor_strncmp_006faff0(const char*,const char*,U);
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
void __fastcall lip_base_destroy_0070c2f0(void*,void*);
void* __fastcall lip_base_deleting_0070c4b0(void*,void*,U);
void __fastcall lip_resource_destroy_0070c6b0(void*,void*);
void* __fastcall lip_resource_deleting_0070c6c0(void*,void*,U);
extern const HelperMethods lip_base_methods_0075fb0c;
extern const HelperMethods lip_methods_0075fb10;
extern const ResourceMethods lip_resource_methods_0075fb20;
const HelperMethods lip_base_methods_0075fb0c{&lip_base_deleting_0070c4b0};
const HelperMethods lip_methods_0075fb10{&lip_deleting_0070c670};
const ResourceMethods lip_resource_methods_0075fb20{
    &lip_resource_deleting_0070c6c0,&c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0,&aurora_txi_resource_freed_00710e60,
    &aurora_generic_resource_serviced_005d4c60};
}
namespace {
Ref* reference(void*self){return reinterpret_cast<Ref*>(at(self,0xc));}
struct BaseCleanup {
    void*self;
    ~BaseCleanup() noexcept(false){if(self)lip_base_destroy_0070c2f0(self,nullptr);}
};
// Original0073D700 fallback DWORD, verified from the image.
const U missing_frame_time=0x00000000u;
}
extern "C" {
__declspec(naked) float __fastcall lip_duration_0070c280(void*,void*){
    __asm { fld dword ptr [ecx+28h] }
    __asm { ret }
}
U __fastcall lip_frame_id_0070c290(void*self,void*,U index){
    if(index>=rd<U>(self,0x24))return 0;
    return rd<B>(ptr(self,0x2c),index*5u+4u);
}
__declspec(naked) float __fastcall lip_frame_time_0070c2c0(void*,void*,U){
    __asm {
        mov eax,dword ptr [esp+4]
        cmp eax,dword ptr [ecx+24h]
        jae missing_frame
        mov ecx,dword ptr [ecx+2ch]
        lea eax,[eax+eax*4]
        fld dword ptr [ecx+eax]
        ret 4
    missing_frame:
        fld dword ptr missing_frame_time
        ret 4
    }
}
// Reuses aurora_family_impl::base_destroy's proven algorithm, with the LIP
// table and actual throwing-call contract; do not import its noexcept qualifier.
void __fastcall lip_base_destroy_0070c2f0(void*self,void*){
    void*initial=ptr(self,8);
    wr<const void*>(self,0,&lip_base_methods_0075fb0c);
    if(!initial)return;
    if(!invoke(c2da_not_equal_text_00406060,reference(self),nullptr,c2da_empty_0073d71c()))return;
    if(rd<U>(self,4)){
        invoke(c2da_cancel_00409b00,ptr(self,8),nullptr);
        wr<U>(self,4,0);
    }
    if(invoke(c2da_release_00409cf0,c2da_manager_007a39e8(),nullptr,ptr(self,8)))return;
    if(void*resource=ptr(self,8))slot<Delete>(resource,0)(resource,nullptr,1u);
    wr<void*>(self,8,nullptr);
}
void* __fastcall lip_base_deleting_0070c4b0(void*self,void*,U flags){
    invoke(lip_base_destroy_0070c2f0,self,nullptr);
    if(flags&1u)invoke(live_free,self);
    return self;
}
// Reuses aurora_family_impl::set_ref's exact branch algorithm, with LIP's
// original tag0BBC/size34/constructor and canonical selected lower interfaces.
void __fastcall lip_set_ref_0070c350(void*self,void*,Ref*incoming,I request){
    Ref*current=reference(self);
    if(invoke(resource_equal_004060b0,incoming,nullptr,current))return;
    if(ptr(self,8)&&invoke(c2da_not_equal_text_00406060,current,nullptr,c2da_empty_0073d71c())){
        if(rd<U>(self,4))invoke(c2da_cancel_00409b00,ptr(self,8),nullptr);
        if(!invoke(c2da_release_00409cf0,c2da_manager_007a39e8(),nullptr,ptr(self,8))){
            if(void*resource=ptr(self,8))slot<Delete>(resource,0)(resource,nullptr,1u);
            wr<void*>(self,8,nullptr);
        }
    }
    if(invoke(c2da_equal_text_004060d0,incoming,nullptr,c2da_empty_0073d71c())){
        wr<void*>(self,8,nullptr);
        Ref discarded;
        invoke(unresolved_ref_assign_00406290,current,nullptr,&discarded,c2da_empty_0073d71c());
        return;
    }
    void*selected=invoke(c2da_lookup_004074d0,c2da_manager_007a39e8(),nullptr,incoming,0xbbcu);
    if(!selected){
        void*raw=invoke(live_operator_new,0x34u);
        try{if(raw)selected=invoke(lip_resource_construct_0070c690,raw,nullptr);}
        catch(...){invoke(live_free,raw);throw;}
        invoke(c2da_register_00407680,c2da_manager_007a39e8(),nullptr,incoming,0xbbcu,selected);
        if(!selected)return;
    }
    wr<void*>(self,8,selected);
    if(request){wr<U>(self,4,1);invoke(c2da_request_00408620,selected,nullptr);}
    Ref discarded;
    invoke(c2da_copy_00406120,current,nullptr,&discarded,incoming);
}
void* __fastcall lip_construct_0070c4d0(void*self,void*){
    wr<const void*>(self,0,&lip_base_methods_0075fb0c);
    invoke(c2da_default_ref_00405ed0,reference(self),nullptr);
    wr<void*>(self,8,nullptr);
    Ref discarded;
    invoke(unresolved_ref_assign_00406290,reference(self),nullptr,&discarded,c2da_empty_0073d71c());
    wr<U>(self,4,0);
    wr<U>(self,0x24,0);wr<U>(self,0x28,0);wr<void*>(self,0x2c,nullptr);
    // Original does not initialize+1C/+20. In particular do not invent loaded=0.
    wr<const void*>(self,0,&lip_methods_0075fb10);
    return self;
}
void __fastcall lip_destroy_0070c520(void*self,void*){
    wr<const void*>(self,0,&lip_methods_0075fb10);
    BaseCleanup base{self};
    if(rd<U>(self,0x20)){
        invoke(c2da_release_demand_00409b80,ptr(self,8),nullptr);
        wr<U>(self,0x20,0);
    }
    wr<void*>(self,0x2c,nullptr);
    base.self=nullptr;
    invoke(lip_base_destroy_0070c2f0,self,nullptr);
}
void* __fastcall lip_deleting_0070c670(void*self,void*,U flags){
    invoke(lip_destroy_0070c520,self,nullptr);
    if(flags&1u)invoke(live_free,self);
    return self;
}
U __fastcall lip_load_0070c590(void*self,void*,String name){
    Ref resource_name;
    invoke(async_ref_construct_string_00406d60,&resource_name,nullptr,&name);
    invoke(lip_set_ref_0070c350,self,nullptr,&resource_name,0);
    if(!invoke(c2da_current_resource_demand_00409b10,ptr(self,8),nullptr))return 0;
    void*data=invoke(c2da_current_resource_data_00404dd0,ptr(self,8),nullptr);
    wr<void*>(self,0x2c,data);
    if(invoke(kotor_strncmp_006faff0,"LIP V1.0",static_cast<const char*>(data),8u))return 0;
    void*cursor=at(ptr(self,0x2c),8);
    wr<void*>(self,0x2c,cursor);
    U duration_bits=rd<U>(cursor,0);
    cursor=at(cursor,4);wr<void*>(self,0x2c,cursor);
    wr<U>(self,0x28,duration_bits);
    U count=rd<U>(cursor,0);wr<U>(self,0x24,count);
    cursor=at(cursor,4);wr<void*>(self,0x2c,cursor);
    wr<U>(self,0x20,1);
    return 1;
}
void* __fastcall lip_resource_construct_0070c690(void*self,void*){
    invoke(c2da_resource_base_ctor_00406460,self,nullptr);
    wr<U>(self,0x28,0);wr<U>(self,0x2c,0);wr<U>(self,0x30,0);
    wr<const void*>(self,0,&lip_resource_methods_0075fb20);
    return self;
}
void __fastcall lip_resource_destroy_0070c6b0(void*self,void*){
    wr<const void*>(self,0,&lip_resource_methods_0075fb20);
    invoke(c2da_resource_base_destroy_00409d40,self,nullptr);
}
void* __fastcall lip_resource_deleting_0070c6c0(void*self,void*,U flags){
    invoke(lip_resource_destroy_0070c6b0,self,nullptr);
    if(flags&1u)invoke(live_free,self);
    return self;
}
}
