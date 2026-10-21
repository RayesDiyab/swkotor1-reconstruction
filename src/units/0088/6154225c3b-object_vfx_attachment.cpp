#include "0ee5801492-startup_string.h"
#include "2a850f1bd9-object_vfx_core.h"
#include "8d8fefca35-object_vfx_handlers.h"
#include "9c3144a11f-kotor_unreachable_trap.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
using U=std::uint32_t;using I=std::int32_t;using H=std::uint16_t;using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace aurora_model { struct Gob; struct Scene; }
namespace c2da_current { struct Table; }
namespace kotor_menu_window_cells{extern void*volatile manager_007a39fc;}
extern "C"{
extern void*kotor_cell_rules_007a3a28;
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*,const char*,std::FILE*,std::FILE*);
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed700(void*,void*);
U __fastcall gob_get_property_0x68_0063c070(aurora_model::Gob*,void*);
void __fastcall kotor_fastcall_004015a0(void*,void*);
I __fastcall c2da_current_get_string_00413270(void*,void*,I,void*,void*);
I __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table*,void*,I,I,I*);
extern U kotor_rules_cell32_007a2124;
void* __cdecl vector_normalized_004ab180(void*,const void*);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
void* __cdecl yaw_pitch_roll_004acac0(void*,float,float,float);
void* __fastcall vector_add_assign_004a9740(void*,void*,const void*);
char* __fastcall WindowCStr005e5670(String*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
String* __fastcall resman_string_from_int_005e5bc0(String*,void*,I);
I __fastcall kotor_fastcall_00414110(void*,void*,String*,String*,I*);
void __cdecl genuine_x87_ftol_006fae8c(); // original ST0 -> EDX:EAX, no stack arguments
void __fastcall object_vfx_006a0d30(void*,void*);
U __fastcall object_vfx_006a0620(void*,void*,U);
void __fastcall object_vfx_006a1650(void*,void*,void*,void*,String*);
}
namespace{
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void*ptr(const void*p,U o){return rd<void*>(p,o);}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using Getter=void*(__fastcall*)(void*,void*);
using One=U(__fastcall*)(aurora_model::Gob*,void*,U);
using None=U(__fastcall*)(void*,void*);
using Fog=U(__fastcall*)(aurora_model::Gob*,void*);
using SceneAttach=I(__fastcall*)(aurora_model::Gob*,void*,aurora_model::Scene*);
using Attach=void*(__fastcall*)(void*,void*,void*,const char*,I);
using GetModel=void*(__fastcall*)(void*,void*,U,U);
using Reattach=U(__fastcall*)(aurora_model::Gob*,void*,U,U);
using Animate=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U);
struct Vector{U x,y,z;};struct Quat{U w,x,y,z;};
using TransformGet=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U);
using TransformSet=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U,U,U,U,U);
void*client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void*target(U id){void*o=invoke(kotor_fastcall_005ed580,client(),nullptr,id);return o?slot<Getter>(o,0x0c)(o,nullptr):nullptr;}
String*field(void*p,U o){return reinterpret_cast<String*>(static_cast<B*>(p)+o);}
// Constructor-style return temporary. Activate only after the original helper
// successfully constructs it; no default-constructor call is added.
struct ReturnString{
    alignas(String)B storage[sizeof(String)];bool active=false;
    String*get(){return reinterpret_cast<String*>(storage);}
    void destroy(){if(active){active=false;get()->~StartupString();}}
    ~ReturnString(){destroy();}
};
void delta_and_zero_small(Vector*delta,void*from,void*to){
    const U epsilon=0x38d1b717;H status;
    __asm{
        mov esi,from
        mov edi,to
        mov ecx,delta
        fld dword ptr [esi+24h]
        fsub dword ptr [edi+24h]
        fstp dword ptr [ecx]
        fld dword ptr [esi+28h]
        fsub dword ptr [edi+28h]
        fstp dword ptr [ecx+4]
        fld dword ptr [esi+2ch]
        fsub dword ptr [edi+2ch]
        fstp dword ptr [ecx+8]
        fld dword ptr [ecx+8]
        fmul dword ptr [ecx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [ecx+4]
        faddp st(1)
        fld dword ptr [ecx]
        fmul dword ptr [ecx]
        faddp st(1)
        fcomp dword ptr epsilon
        fnstsw ax
        mov status,ax
    }
    const U masked=status&0x4100u;
    if(masked==0x0100u||masked==0x4000u)*delta={0,0,0};
}
U yaw32bits(const void*vector){
    U yaw;
    __asm{
        push vector
        call kotor_m15a_yaw_004aa0f0
        add esp,4
        fstp dword ptr yaw
    }
    return yaw;
}
}
extern "C"{
U __fastcall object_vfx_006a0680(void*self,void*,U kind){
    String name;
    const U index=static_cast<H>(kind+0xfda8u)-8u;
    const char*names[]={"v_lightns_dur","v_lightnx_dur","v_drddisab_dur","v_drdkill_dur","v_deathfld_dur","v_drain_dur","v_flame_dur","v_stunray_dur","v_coldray_dur","v_ionray01_dur","v_ionray02_dur","v_fstorm_dur","v_drdstun_dur","v_fshock_dur"};
    if(index<=13)(void)invoke(GuiAssign005e5140,&name,nullptr,names[index]);
    const char*text=invoke(WindowCStr005e5670,&name,nullptr);
    void*m=invoke(aurora_new_aur_object_00449cc0,text,"fxbeam",nullptr,nullptr);
    wr<void*>(self,0x8c,m);if(!m)return 0;
    slot<One>(m,0xc4)(static_cast<aurora_model::Gob*>(m),nullptr,3);
    m=ptr(self,0x8c);slot<Fog>(m,0xe4)(static_cast<aurora_model::Gob*>(m),nullptr);
    return 1;
}
U __fastcall object_vfx_006a0800(void*self,void*,U kind){
    const U n=static_cast<H>(kind-1200u);const char*model;const char*tag;
    if(n<=1){model="v_fizzle_imp";tag="fxfail";}
    else if(n==2){model="v_fresist_imp";tag="fxresist";}
    else return 0;
    wr<void*>(self,0x8c,invoke(aurora_new_aur_object_00449cc0,model,tag,nullptr,nullptr));
    return ptr(self,0x8c)?1u:0u;
}
U __fastcall object_vfx_006a0860(void*self,void*,U kind){
    const U n=static_cast<H>(kind-1700u);
    if(n<=2){
        const char*models[]={"v_medal_dur","v_revmask1_dur","v_revmask2_dur"};
        const char*tags[]={"fxmedal","fxrevanmask1","fxrevanmask2"};
        wr<void*>(self,0x8c,invoke(aurora_new_aur_object_00449cc0,models[n],tags[n],nullptr,nullptr));
    }
    return ptr(self,0x8c)?1u:0u; // invalid selector preserves preexisting model
}
void __fastcall object_vfx_006a05b0(void*self,void*,U){
    void*m=ptr(self,0x8c);if(!m)return;
    (void)slot<Attach>(m,0x50)(m,nullptr,nullptr,nullptr,0);
    m=ptr(self,0x8c);(void)slot<SceneAttach>(m,0x4c)(static_cast<aurora_model::Gob*>(m),nullptr,nullptr);
}
U __fastcall object_vfx_006a0620(void*self,void*,U kind){
    const H n=static_cast<H>(kind);if(n==0xffff)return 0;
    void*m=ptr(self,0x8c);if(!m||n!=1201)return 1000;
    U start;U end=kind;
    using Length=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U);
    slot<Length>(m,0x1c)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("impact")),static_cast<U>(reinterpret_cast<std::uintptr_t>(&start)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&end)));
    const U thousand=0x447a0000;U answer;
    __asm{
        fld dword ptr end
        fmul dword ptr thousand
        call genuine_x87_ftol_006fae8c
        mov answer,eax
    }
    return answer;
}
void __fastcall object_vfx_006a0d30(void*self,void*){
    void*to=target(rd<U>(self,0x40));void*from=target(rd<U>(self,0x44));
    if(!to||!from||!ptr(self,0x8c))return;
    void*m=ptr(self,0x8c);Attach first=slot<Attach>(m,0x50);
    const char*name=invoke(WindowCStr005e5670,field(self,0x24),nullptr);
    void*node=slot<GetModel>(from,0x98)(from,nullptr,0xff,1);
    (void)first(ptr(self,0x8c),nullptr,node,name,0);
    m=ptr(self,0x8c);Reattach second=slot<Reattach>(m,0xa4);
    name=invoke(WindowCStr005e5670,field(self,0x2c),nullptr);
    node=slot<GetModel>(to,0x98)(to,nullptr,0xff,1);
    (void)second(static_cast<aurora_model::Gob*>(ptr(self,0x8c)),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(node)),static_cast<U>(reinterpret_cast<std::uintptr_t>(name)));
}
void __fastcall object_vfx_006a3fd0(void*self,void*,U){
    invoke(object_vfx_006a0d30,self,nullptr);
    void*m=ptr(self,0x8c);
    slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("cast01")),0x3f800000,0,0);
}
void __fastcall object_vfx_006a4680(void*self,void*,U kind){
    const U n=static_cast<H>(kind-1700u);
    const char*names[]={"medalhook","revmask1hook","revmask2hook"};
    if(n<=2)(void)invoke(GuiAssign005e5140,field(self,0x2c),nullptr,names[n]);
    void*m=ptr(self,0x8c);if(!m)return;
    void*view=target(rd<U>(self,0x40));m=ptr(self,0x8c);Attach attach=slot<Attach>(m,0x50);
    const char*name=invoke(WindowCStr005e5670,field(self,0x2c),nullptr);
    void*node=slot<GetModel>(view,0x98)(view,nullptr,0xff,1); // original unchecked view
    (void)attach(ptr(self,0x8c),nullptr,node,name,0);
}
void __fastcall object_vfx_006a1650(void*self,void*,void*model,void*node,String*name){
    I orientation;
    bool failed;
    {
        String column("OrientationOff");ReturnString row;
        String*returned=invoke(resman_string_from_int_005e5bc0,row.get(),nullptr,static_cast<I>(rd<H>(self,0xc4)));
        row.active=true;
        void*table=ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x88);
        failed=!invoke(kotor_fastcall_00414110,table,nullptr,returned,&column,&orientation);
        row.destroy();
    }
    if(failed)orientation=0;
    Attach attach=slot<Attach>(model,0x50);
    const char*text=invoke(WindowCStr005e5670,name,nullptr);
    if(!orientation){(void)attach(model,nullptr,node,text,0);return;}
    Vector position{0,0,0};Quat rotation{0x3f800000,0,0,0};
    (void)attach(model,nullptr,node,text,3);
    slot<TransformGet>(model,0x98)(static_cast<aurora_model::Gob*>(model),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("root")),static_cast<U>(reinterpret_cast<std::uintptr_t>(&position)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&rotation)));
    // Original identity quaternion replaces the returned rotation, but the
    // model-supplied position survives the preceding transform query.
    rotation={0x3f800000,0,0,0};
    slot<TransformSet>(model,0x94)(static_cast<aurora_model::Gob*>(model),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("root")),position.x,position.y,position.z,rotation.w,rotation.x,rotation.y,rotation.z);
}
void __fastcall object_vfx_006a2e10(void*self,void*,U kind){
    const U n=static_cast<H>(kind+0xfb50u);
    const char*names[]={"headconjure","handconjure","impact"};
    if(n<=2)(void)invoke(GuiAssign005e5140,field(self,0x2c),nullptr,names[n]);
    if(!ptr(self,0x8c))return;
    I mode=0;void*view;
    if(n==2){
        Vector position{0,0,0};Quat rotation{0x3f800000,0,0,0};
        void*from=target(rd<U>(self,0x44));void*to=target(rd<U>(self,0x40));
        void*m=ptr(self,0x8c);slot<TransformGet>(m,0x98)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("root")),static_cast<U>(reinterpret_cast<std::uintptr_t>(&position)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&rotation)));
        if(from&&to){
            Vector delta;delta_and_zero_small(&delta,from,to);
            Vector unit;void*normalized=invoke(vector_normalized_004ab180,&unit,&delta);
            std::memcpy(&delta,normalized,sizeof(delta));
            const U yaw_bits=yaw32bits(&delta);float yaw;std::memcpy(&yaw,&yaw_bits,4);
            Quat computed;void*q=invoke(yaw_pitch_roll_004acac0,&computed,yaw,0.0f,0.0f);
            std::memcpy(&rotation,q,sizeof(rotation));
        }
        m=ptr(self,0x8c);slot<TransformSet>(m,0x94)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("root")),position.x,position.y,position.z,rotation.w,rotation.x,rotation.y,rotation.z);
        view=target(rd<U>(self,0x40));mode=3;
    }else view=target(rd<U>(self,0x40));
    void*m=ptr(self,0x8c);Attach attach=slot<Attach>(m,0x50);
    const char*name=invoke(WindowCStr005e5670,field(self,0x2c),nullptr);
    void*model=slot<GetModel>(view,0x98)(view,nullptr,0xff,1);
    (void)attach(ptr(self,0x8c),nullptr,model,name,mode);
    m=ptr(self,0x8c);slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("impact")),0x3f800000,1,0);
}
void __fastcall object_vfx_006a4000(void*self,void*,U){
    if(rd<B>(invoke(kotor_fastcall_005ed700,client(),nullptr),0x72)<2)return;
    void*view=target(rd<U>(self,0x40));if(!view)return;
    void*creature=slot<Getter>(view,0x2c)(view,nullptr);
    if(creature)invoke(kotor_fastcall_004015a0,creature,nullptr); // unique original void-return provider
    void*animation=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(gob_get_property_0x68_0063c070,static_cast<aurora_model::Gob*>(view),nullptr)));
    slot<None>(animation,0x78)(animation,nullptr);
}
void __fastcall object_vfx_006a4060(void*self,void*,U){
    Vector position{0,0,0};
    if(rd<B>(invoke(kotor_fastcall_005ed700,client(),nullptr),0x72)<2)return;
    void*view=target(rd<U>(self,0x40));
    if(!ptr(view,0x20))return;
    void*creature=slot<Getter>(view,0x2c)(view,nullptr);if(!creature)return;
    std::memcpy(&position,static_cast<B*>(view)+0x24,sizeof(position));
    if(slot<GetModel>(creature,0x98)(creature,nullptr,0xff,1)){
        Vector offset{0,0,0};Quat rotation{0x3f800000,0,0,0};
        void*m=slot<GetModel>(creature,0x98)(creature,nullptr,0xff,1);
        slot<TransformGet>(m,0xa0)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("impact")),static_cast<U>(reinterpret_cast<std::uintptr_t>(&offset)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&rotation)));
        (void)invoke(vector_add_assign_004a9740,&position,nullptr,&offset);
    }
    String output;
    {
        String column("bloodcolr");
        const I row=rd<H>(ptr(creature,0x21c),0x18);
        void*table=ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x0c);
        (void)invoke(c2da_current_get_string_00413270,table,nullptr,row,&column,&output);
    }
    const I row=rd<H>(ptr(creature,0x21c),0x18);I value;
    void*table=ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x0c);
    (void)invoke(c2da_current_get_int_entry_00413510,static_cast<c2da_current::Table*>(table),nullptr,row,static_cast<I>(kotor_rules_cell32_007a2124),&value);
    // Original reads then discards value/output and creates no chunk model here.
}
}
