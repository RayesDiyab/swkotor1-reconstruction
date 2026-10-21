// LightsaberThrow: complete original 006A30E0 CFG and three bounded leaves.
#include "0cf01a61b8-object_vfx_lightsaber.h"
#include "b01329e984-object_projectile.h"
#include "5f9fa33b46-object_vfx_initializer.h"
#include "97b49d46ad-startup_string.h"
#include "4c4573e9c4-m15e.h"
#include "9295cae9de-resources.h"
#include "488aeb4007-scene3d.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using Vector=kotor_scene3d::Vector;
using Quaternion=kotor_scene3d::Quaternion;
using String=kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,U);
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*,void*,U);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*,void*) noexcept(false);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*,void*);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion*,void*,Vector,float);
void __cdecl genuine_x87_ftol_006fae8c();
}
namespace {
template<class T>T read(const void*p,U o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof(v));return v;}
template<class T>void write(void*p,U o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return read<void*>(p,o);}
template<class F>F slot(void*p,U o){return read<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
U word(const void*p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
using Get=decltype(KotorM15ObjectMethods_007516c8::s00c_as_creature_or_self);
using Load=decltype(KotorM15ObjectMethods_007516c8::s078_load_model);
using Model=decltype(KotorM15ObjectMethods_007516c8::s098_get_model);
using SetVector=decltype(KotorM15ObjectMethods_007516c8::s088_set_orientation);
using Attach=decltype(KotorM15ObjectMethods_007516c8::s094_attach_to_scene);
using GobGetPart=decltype(aurora_model::GobVTable::s098_gob_get_part_position);
using GobDisable=decltype(aurora_model::GobVTable::s0e0_gob_disable_force_animations);
struct ConstructorAllocation{
    void* allocation;
    ~ConstructorAllocation(){if(allocation)live_free(allocation);}
    void release(){allocation=nullptr;}
};
// Original addresses are provenance only; these bits and strings are copied
// from independently pinned original .rdata, never dereferenced as VAs.
const U default_rate_bits=0x42140000; // 0074B2E8 FLD32
#include <kotor_generated/vfx/lightsaber_literals.inc> // 0074F334
 // 00743318
const U duration_scale_bits=0x44fa0000; // 007469E4 FMUL32
float bits(U u){float f;std::memcpy(&f,&u,4);return f;}
// Raw ST0 -> original FSTP32. The callback's nominal double return must not
// introduce a C++ double local or a 64-bit spill.
void store_rate(void* creature,float* destination){
    auto callback=object_vfx_006109b0;
    __asm {
        mov ecx,creature
        xor edx,edx
        call callback
        mov eax,destination
        fstp dword ptr[eax]
    }
}
U duration(const Vector* difference,const float* rate){
    auto magnitude=vector_magnitude_004ab100;
    auto convert=genuine_x87_ftol_006fae8c;
    U result;
    __asm {
        push difference
        call magnitude
        mov eax,rate
        fdiv dword ptr[eax]
        add esp,4
        fmul dword ptr duration_scale_bits
        call convert
        mov result,eax
    }
    return result;
}
}
extern "C" {
U __fastcall object_vfx_0060ed80(void*self,void*,U id){write(self,0x3c8,id);return id;}
// MOV order matters when the input aliases projectile storage.
U __fastcall object_vfx_0064b4d0(void*self,void*,const ObjectVfxPosition* source){
    write(self,0x114,read<U>(source,0));
    write(self,0x118,read<U>(source,4));
    U z=read<U>(source,8);write(self,0x11c,z);return z;
}
// Naked only to preserve the original FLD32/raw ST0 return, including its
// repeated getter calls and original zero-base +AB4 fault path. No EH frame.
__declspec(naked) double __fastcall object_vfx_006109b0(void*,void*){
    __asm {
        push esi
        mov esi,ecx
        call kotor_m15a_object_get_server_object_0063d4b0
        test eax,eax
        je rate_default
        mov ecx,esi
        call kotor_m15a_object_get_server_object_0063d4b0
        mov edx,dword ptr[eax]
        mov ecx,eax
        call dword ptr[edx+30h]
        test eax,eax
        je rate_default
        mov ecx,esi
        call kotor_m15a_object_get_server_object_0063d4b0
        test eax,eax
        je rate_null
        mov ecx,esi
        call kotor_m15a_object_get_server_object_0063d4b0
        mov edx,dword ptr[eax]
        mov ecx,eax
        call dword ptr[edx+30h]
        fld dword ptr[eax+0ab4h]
        pop esi
        ret
rate_null:
        xor eax,eax
        fld dword ptr[eax+0ab4h]
        pop esi
        ret
rate_default:
        fld dword ptr default_rate_bits
        pop esi
        ret
    }
}
void __fastcall object_vfx_006a30e0(void*self,void*,U){
    void* object=invoke(kotor_fastcall_005ed580,client(),nullptr,read<U>(self,0x40));
    void* source=object?slot<Get>(object,0x0c)(object,nullptr):nullptr;
    object=invoke(kotor_fastcall_005ed580,client(),nullptr,read<U>(self,0x44));
    void* target=object?slot<Get>(object,0x0c)(object,nullptr):nullptr;
    object=invoke(kotor_fastcall_005ed580,client(),nullptr,read<U>(self,0x48));
    void* actor=object?slot<Get>(object,0x0c)(object,nullptr):nullptr;
    float rate=bits(0x42140000);
    if(actor && slot<Get>(actor,0x2c)(actor,nullptr))
        store_rate(slot<Get>(actor,0x2c)(actor,nullptr),&rate);
    if(!source || !target || !actor || !slot<Get>(actor,0x2c)(actor,nullptr))return;
    void* creature=slot<Get>(actor,0x2c)(actor,nullptr);
    void* item_client=client(); // original caches client before equipped-item call
    U item_id=invoke(kotor_m15a_creature_get_equipped_item_id_0060ca80,creature,nullptr,U(0x10));
    void* item=invoke(kotor_m15_client_get_item_by_id_005ed5a0,item_client,nullptr,item_id);
    if(!item)return;
    void* allocation=invoke(live_operator_new,U(0x1c8));
    void* projectile=nullptr;
    {
        ConstructorAllocation guard{allocation};
        if(allocation)projectile=invoke(object_projectile_006d57b0,allocation,nullptr);
        guard.release(); // original state -1 precedes first post-constructor call
    }
    invoke(object_projectile_006d41b0,projectile,nullptr,U(0x31));
    write(projectile,0x1c4,read<U>(actor,4));
    const char* model_name=static_cast<const char*>(invoke(kotor_m15e_slot00c_self_00641db0,static_cast<unsigned char*>(item)+0x128,nullptr));
    menu_resources::Ref model_ref;
    model_ref.construct_00406d80(model_name);
    (void)slot<Load>(projectile,0x78)(projectile,nullptr,&model_ref,0xff,0);
    auto* model=static_cast<aurora_model::Gob*>(slot<Model>(projectile,0x98)(projectile,nullptr,0xff,1));
    (void)slot<GobDisable>(model,0xe0)(model,nullptr);
    const bool same_id=read<U>(target,4)==read<U>(actor,4);
    Vector position{0.0f,0.0f,0.0f};
    Quaternion part_rotation{1.0f,0.0f,0.0f,0.0f};
    void* part_owner=same_id?creature:target;
    model=static_cast<aurora_model::Gob*>(slot<Model>(part_owner,0x98)(part_owner,nullptr,0xff,1));
    (void)slot<GobGetPart>(model,0x98)(model,nullptr,word(same_id?hand_name:target_name),word(&position),word(&part_rotation));
    slot<SetVector>(projectile,0x8c)(projectile,nullptr,&position.x);
    invoke(object_vfx_0060ed80,creature,nullptr,read<U>(projectile,4));
    Vector target_direction=read<Vector>(target,0x30);
    Vector up{0.0f,0.0f,1.0f};
    Vector cross;
    vector_cross_004ab210(&cross,&target_direction,&up);
    Quaternion orientation;
    quaternion_construct_004ac960(&orientation,nullptr,cross,bits(0x3f9c61aa));
    void* orientation_table=ptr(projectile,0); // cache table, not its slot entry
    Vector rotated;
    Vector* rotated_result=quaternion_rotate_004a9bb0(&rotated,&orientation,&target_direction);
    read<SetVector>(orientation_table,0x88)(projectile,nullptr,&rotated_result->x);
    auto* source_origin=reinterpret_cast<const ObjectVfxPosition*>(static_cast<unsigned char*>(source)+0x24);
    invoke(object_vfx_0064b4d0,projectile,nullptr,source_origin);
    {
        String name(actor==source?hand_name:"");
        U source_id=read<U>(source,4);
        invoke(object_projectile_006d42d0,projectile,nullptr,source_id,&name);
    }
    Vector difference;
    float* difference_result=kotor_cdecl_00423bc0(&difference.x,reinterpret_cast<const float*>(source_origin),reinterpret_cast<const float*>(static_cast<unsigned char*>(target)+0x24));
    U elapsed=duration(reinterpret_cast<const Vector*>(difference_result),&rate);
    (void)invoke(object_projectile_006d2c30,projectile,nullptr,elapsed);
    (void)invoke(object_projectile_006d4250,projectile,nullptr,U(3));
    invoke(object_projectile_006d6ee0,projectile,nullptr,U(1));
    write<std::uint8_t>(projectile,0x158,0);
    void* module=invoke(kotor_fastcall_005ed530,client(),nullptr);
    if(module){void* area=ptr(module,0x48);if(area)(void)slot<Attach>(projectile,0x94)(projectile,nullptr,ptr(area,0x184));}
}
}
