// Original inherited CSWCObject bodies used by the projectile table00757CC8.
#include "42237bfd9e-object_projectile_base.h"
#include "ead2dcbdd7-m15e.h"
#include "bd9f6ce7e8-m15e_animbase.h"
#include "0935fa9afa-resources.h"
#include "864facc966-startup_string.h"
#include "fe0daa935e-scene3d.h"
#include <cstring>
using U=std::uint32_t;
using H=std::uint16_t;
using B=unsigned char;
using Ref=menu_resources::Ref;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void __fastcall object_vfx_006a6590(void*,void*,U);
// Real canonical iterator: selected0168 wrapper ->0167 NextStep; primary iterator-reuse-t1 proof.
U __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
Ref* __fastcall c2da_copy_00406120(Ref*,void*,Ref*,Ref*);
void* __fastcall kotor_m15b_portrait_construct_00678ec0(void*,void*);
Ref* __fastcall kotor_m15e_portrait_get_portrait_00678ef0(void*,void*,Ref*,U);
U __fastcall kotor_m15e_portrait_set_portrait_id_006790d0(void*,void*,U);
void __fastcall gui_load_slot_ret_004015a0(void*,void*);
kotor_scene3d::Quaternion* __cdecl rotation_arc_004ab630(kotor_scene3d::Quaternion*,kotor_scene3d::Vector,kotor_scene3d::Vector);
U __fastcall frame_R4_005ed690(void*,void*);
// Required654byte bark body is separately owned; this declaration is not an implementation.
void __fastcall object_projectile_bark_006a9920(void*,void*,U,kotor_reconstruction::StartupString*,Ref*,U);
void __fastcall kotor_fastcall_0040bc70(void*,void*,void*,U,U);
U __fastcall kotor_fastcall_0063c380(void*,void*,U);
}
namespace {
template<class T>T rd(const void*p,U off){T v;std::memcpy(&v,static_cast<const B*>(p)+off,sizeof(v));return v;}
template<class T>void wr(void*p,U off,T v){std::memcpy(static_cast<B*>(p)+off,&v,sizeof(v));}
void* ptr(const void*p,U off){return rd<void*>(p,off);}
void* at(void*p,U off){return static_cast<B*>(p)+off;}
const KotorM15ObjectMethods_007516c8* methods(const void*p){return rd<const KotorM15ObjectMethods_007516c8*>(p,0);}
const KotorM15eAnimBaseVtbl* anim_methods(const void*p){return rd<const KotorM15eAnimBaseVtbl*>(p,0);}
// The original D140 unwind state0 invokes004015A0 on the16byte portrait.
// Its cleanup is RET-only, but invoke the same real provider on normal and exceptional exits.
struct PortraitCleanup{
    void*p;
    ~PortraitCleanup(){gui_load_slot_ret_004015a0(p,nullptr);}
};
}
extern "C" {
void __fastcall object_projectile_base_0063daa0(void*self,void*){
    //0063DAA4..DAB6 deliberately dereferences this+6C even when the list is empty.
    void*list=ptr(self,0x6c);
    void*position=ptr(list,0);
    void*node=ptr(list,0);
    void*effect=node?ptr(node,8):nullptr;
    if(effect){
        do{
            object_vfx_006a6590(effect,nullptr,1);
            //0063DAC3 reloads the list after each Stop; the iterator updates position.
            effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(ptr(self,0x6c),nullptr,&position)));
        }while(effect);
    }
}
void* __fastcall object_projectile_base_0063e760(void*self,void*){
    void*anim=ptr(self,0x68);
    return anim_methods(anim)->s008(anim,nullptr,0xff);
}
void __fastcall object_projectile_base_0063cfd0(void*self,void*,void*source){
    Ref dead;
    c2da_copy_00406120(static_cast<Ref*>(at(self,0xa8)),nullptr,&dead,static_cast<Ref*>(source));
    methods(self)->s0f0_get_portrait(self,nullptr,&dead,1);
}
void* __fastcall object_projectile_base_0063e770(void*self,void*,void*out,U variant){
    kotor_m15e_portrait_get_portrait_00678ef0(at(self,0xa8),nullptr,static_cast<Ref*>(out),variant);
    return out;
}
void __fastcall object_projectile_base_0063d140(void*self,void*,U id){
    alignas(4) B portrait[16];
    kotor_m15b_portrait_construct_00678ec0(portrait,nullptr);
    PortraitCleanup cleanup{portrait};
    if(kotor_m15e_portrait_set_portrait_id_006790d0(portrait,nullptr,id)){
        //0063D180 captures the vtable before GetPortrait; do not reload it afterward.
        const auto*table=methods(self);
        Ref image;
        Ref*got=kotor_m15e_portrait_get_portrait_00678ef0(portrait,nullptr,&image,0);
        table->s0ec_set_portrait(self,nullptr,got);
    }
    //0063D1A0 stores only the low word, even when SetPortraitId failed.
    wr<H>(self,0xb8,static_cast<H>(id));
}
H __fastcall object_projectile_base_0063e790(void*self,void*){
    return rd<H>(self,0xb8); //0063E790 defines AX only.
}
void __fastcall object_projectile_base_0063d0a0(void*self,void*,const float*normal){
    //0063D0AD..D0C0 copy threeDWORDs in order before testing the animation pointer.
    wr<U>(self,0xd8,rd<U>(normal,0));
    wr<U>(self,0xdc,rd<U>(normal,4));
    wr<U>(self,0xe0,rd<U>(normal,8));
    void*anim=ptr(self,0x68);
    if(anim){
        //0063D0E4 preserves the old table across RotationArc; receiver reload follows it.
        const auto*table=anim_methods(anim);
        const kotor_scene3d::Vector up{0.0f,0.0f,1.0f};
        kotor_scene3d::Vector direction;
        std::memcpy(&direction,normal,12);
        kotor_scene3d::Quaternion arc;
        auto*q=rotation_arc_004ab630(&arc,up,direction);
        anim=ptr(self,0x68);
        table->s0b4(anim,nullptr,reinterpret_cast<const float*>(q));
    }
}
U __fastcall object_projectile_base_0063e7b0(void*self,void*,U a,U b,U c,U d){
    return methods(self)->s124_bark_bubble(self,nullptr,a,b,c,d);
}
U __fastcall object_projectile_base_0060f560(void*,void*,U a,U b,U c,U d){
    void*client=ptr(kotor_menu_window_cells::manager_007a39fc,4);
    void*gui=reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed690(client,nullptr)));
    object_projectile_bark_0062ab30(gui,nullptr,a,
        reinterpret_cast<kotor_reconstruction::StartupString*>(static_cast<std::uintptr_t>(b)),
        reinterpret_cast<Ref*>(static_cast<std::uintptr_t>(c)),d);
    return 1; //0060F588 forced success, including lower's disabled branch.
}
//0062AB30 original61bytes; authorized necessary lower, ownership assigned by root.
void __fastcall object_projectile_bark_0062ab30(void*self,void*,U actor,kotor_reconstruction::StartupString*text,Ref*voice,U flags){
    if(rd<U>(self,0x108)){
        object_projectile_bark_006a9920(ptr(self,0x4c),nullptr,actor,text,voice,flags);
        //0062AB59 reloads the panel receiver after SetBark.
        void*panel=ptr(self,0x4c);
        kotor_fastcall_0040bc70(ptr(self,0x38),nullptr,panel,4,1);
    }
}
void __fastcall object_projectile_base_0063e7d0(void*self,void*,const float*position,U,float){
    methods(self)->s08c_set_position(self,nullptr,position);
}
void __fastcall object_projectile_base_0063e7e0(void*self,void*,const float*orientation){
    methods(self)->s088_set_orientation(self,nullptr,orientation);
}
void __fastcall object_projectile_base_0063d3f0(void*self,void*,U animation,U value){
    const U old=rd<H>(self,0x48); //0063D3F7: zero-extended16bit current animation.
    wr<U>(self,0x4c,value);
    if(animation!=old){
        if(kotor_fastcall_0063c380(self,nullptr,animation)){
            methods(self)->s0a0_add_fire_forget_animation(self,nullptr,animation,0);
        }else{
            methods(self)->s09c_set_animation(self,nullptr,animation,0,0);
        }
    }
}
}
