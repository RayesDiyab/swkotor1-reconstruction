#include "b1198be130-startup_string.h"
#include "d979173f70-resources.h"
#include "2054790c93-m15_l0.h"
#include "75ca7482d5-m15e.h"
#include "7390f42639-object_projectile.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using H=std::uint16_t;
using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace kotor_scene3d { struct Vector; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern void* kotor_cell_sound_007a39ec;
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*,void*,String*);
U __cdecl aurora_vector_equal_004aa980(const void*,const void*);
double __cdecl vector_magnitude_004ab100(const kotor_scene3d::Vector*);
void __cdecl genuine_x87_ftol_006fae8c();
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
}
namespace {
struct VectorWords { U x,y,z; };
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using Actor=KotorM15ObjectMethods_007516c8;
using GetView=decltype(Actor::s00c_as_creature_or_self);
using Delete=decltype(Actor::s000_scalar_deleting_destructor);
using Load=decltype(Actor::s078_load_model);
using Position=decltype(Actor::s08c_set_position);
using Orientation=decltype(Actor::s088_set_orientation);
using Scene=decltype(Actor::s094_attach_to_scene);
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void* view(U id){
    void*object=invoke(kotor_fastcall_005ed580,client(),nullptr,id);
    return object?slot<GetView>(object,0x0c)(object,nullptr):nullptr;
}
// Original state1 cleans the allocation only if the projectile constructor throws.
struct ConstructionStorage {
    void* memory;bool active=true;
    ~ConstructionStorage(){if(active)invoke(live_free,memory);}
};
void initial_position(void*from,VectorWords*out){
    const U four=0x40800000u,one=0x3f800000u;
    U scaled_x,scaled_y;
    __asm {
        mov esi,from
        mov edi,out
        fld dword ptr [esi+38h]
        fmul dword ptr four
        fld dword ptr [esi+34h]
        fmul dword ptr four
        fstp dword ptr scaled_y
        fld dword ptr [esi+30h]
        fmul dword ptr four
        fstp dword ptr scaled_x
        fadd dword ptr [esi+2ch]
        fld dword ptr scaled_y
        fadd dword ptr [esi+28h]
        fld dword ptr scaled_x
        fadd dword ptr [esi+24h]
        fstp dword ptr [edi]
        fstp dword ptr [edi+4]
        fadd dword ptr one
        fstp dword ptr [edi+8]
    }
}
void difference(void*to,const void*from_position,VectorWords*out){
    __asm {
        mov esi,to
        mov edi,from_position
        mov ecx,out
        fld dword ptr [esi+2ch]
        fsub dword ptr [edi+8]
        fld dword ptr [esi+28h]
        fsub dword ptr [edi+4]
        fld dword ptr [esi+24h]
        fsub dword ptr [edi]
        fstp dword ptr [ecx]
        fstp dword ptr [ecx+4]
        fstp dword ptr [ecx+8]
    }
}
U duration(const VectorWords*delta){
    const U three=0x40400000u,two=0x40000000u,thousand=0x447a0000u;
    U result;
    // Consume actual provider ST0 without a C++ return spill. Its own FP
    // fidelity remains a separate canonical-provider gate.
    __asm {
        push delta
        call vector_magnitude_004ab100
        fld st(0)
        add esp,4
        fldln2
        fxch st(1)
        fyl2x
        fmul dword ptr three
        fadd dword ptr two
        fdivr st(1)
        fmul dword ptr thousand
        call genuine_x87_ftol_006fae8c
        fstp st(0)
        mov result,eax
    }
    return result;
}
}
extern "C" void __fastcall object_vfx_006a2af0(void*self,void*,U kind){
    String name;
    void*to=view(rd<U>(self,0x40));
    void*from=view(rd<U>(self,0x44));
    if(!to||!from)return;
    void*projectile;
    {
        void*allocation=invoke(live_operator_new,0x1c8u);
        ConstructionStorage cleanup{allocation};
        projectile=allocation?invoke(object_projectile_006d57b0,allocation,nullptr):nullptr;
        cleanup.active=false;
    }
    if(static_cast<H>(kind)==1100)(void)invoke(GuiAssign005e5140,&name,nullptr,"vpr_kngpow");
    menu_resources::Ref ref;
    (void)invoke(async_ref_construct_string_00406d60,&ref,nullptr,&name);
    // Original unconditionally dereferences the constructed pointer here,
    // including its allocation-null path; no replacement success is added.
    if(!slot<Load>(projectile,0x78)(projectile,nullptr,static_cast<void*>(&ref),0xffu,0u)){
        (void)slot<Delete>(projectile,0)(projectile,nullptr,1u);
        return;
    }
    VectorWords position;initial_position(from,&position);
    slot<Position>(projectile,0x8c)(projectile,nullptr,reinterpret_cast<const float*>(&position));
    slot<Orientation>(projectile,0x88)(projectile,nullptr,reinterpret_cast<const float*>(static_cast<B*>(from)+0x30));
    void*module=invoke(kotor_fastcall_005ed530,client(),nullptr);
    if(module){
        void*area=ptr(module,0x48);
        if(area)(void)slot<Scene>(projectile,0x94)(projectile,nullptr,ptr(area,0x184));
    }
    (void)invoke(object_projectile_006d4250,projectile,nullptr,1u);
    invoke(object_projectile_006d41b0,projectile,nullptr,0x2af8u);
    const void*direction=static_cast<B*>(self)+0x34;
    const VectorWords zero{0,0,0};
    if(invoke(aurora_vector_equal_004aa980,direction,&zero)){
        wr<VectorWords>(projectile,0x114,rd<VectorWords>(to,0x24));
        String hook("");
        invoke(object_projectile_006d42d0,projectile,nullptr,rd<U>(to,4),&hook);
    }else wr<VectorWords>(projectile,0x114,rd<VectorWords>(direction,0));
    if(static_cast<H>(kind)==1100){
        const void*from_position=static_cast<B*>(from)+0x24;
        VectorWords delta;difference(to,from_position,&delta);
        (void)invoke(object_projectile_006d2c30,projectile,nullptr,duration(&delta));
        wr<B>(projectile,0x1a1,2);
        invoke(object_projectile_006d6ee0,projectile,nullptr,4u);
        menu_resources::Ref sound_ref;
        sound_ref.construct_00406d80("c_cow_atk1");
        const KotorM15VectorWords sound_position=rd<KotorM15VectorWords>(from_position,0);
        (void)invoke(kotor_m15_sound_play_3d_one_shot_005d5e10,kotor_cell_sound_007a39ec,nullptr,
            static_cast<void*>(&sound_ref),sound_position,1.5f,13u,0u,0u,0.0f,0.0f);
    }
}
