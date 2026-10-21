// Six complete FreeLook entries and the unique original 0075167C table.
#include "7d18ec89ea-freelook.h"
#include "f58c269829-scene3d.h"
#include "289d120a9a-construct3d.h"
#include "ac3ecc8ec9-camera_behavior_base.h"
using U=unsigned;
static_assert(sizeof(U)==4 && sizeof(void*)==4,"original x86 layout");
namespace C3=kotor_scene3d_construct;
using Vec=kotor_scene3d::Vector;
#include "0cdbed471d-w4_providers.hpp"
struct W4RK4State;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void __fastcall kotor_fastcall_006383c0(void*,void*); // sole client-owned Style
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void*,void*);
// Actual canonical math types; the yaw callee is consumed directly on ST0 below.
double __cdecl quaternion_yaw_004a9f40(const kotor_scene3d::Quaternion*);
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion*,float,float,float);
void __fastcall kotor_fastcall_005ee170(void*,void*,unsigned);
void* __fastcall kotor_fastcall_006d0b20(void*,void*);
W4RK4State* __fastcall kotor_fastcall_006d1190(void*,void*,W4RK4State*,W4Vector,float,unsigned,unsigned,unsigned,unsigned);
void* __fastcall safe_pointer_assign_00447fa0(void*,void*,void*);
void __fastcall exo_array_list_remove_00445080(kotor_scene3d::PointerList*,void*,void*);
}
namespace w4cam {
extern const C3::BehaviorBaseSlots camera_free_look_vtable_0075167c;
const C3::BehaviorBaseSlots camera_free_look_vtable_0075167c={
    kotor_fastcall_0063bad0,behavior_void_return_004015a0,kotor_fastcall_00639d00,
    behavior_void_return_4_0060e760,kotor_fastcall_0063a6b0,behavior_null_return_0063e7f0,
    behavior_return_zero_0063e7f0,behavior_return_zero_0063e7f0
};
}
namespace {
U load(void* p,U o){return *reinterpret_cast<volatile U*>(static_cast<unsigned char*>(p)+o);}
void store(void* p,U o,U v){*reinterpret_cast<volatile U*>(static_cast<unsigned char*>(p)+o)=v;}
void* address(void* p,U o){return static_cast<unsigned char*>(p)+o;}
void* pointer(void* p,U o){return reinterpret_cast<void*>(load(p,o));}
U word(const void* p){return reinterpret_cast<U>(p);}
void* client(){return pointer(kotor_menu_window_cells::manager_007a39fc,4);}
template<class F> F vslot(void* p,U o){return reinterpret_cast<F>(*reinterpret_cast<void**>(address(pointer(p,0),o)));}
struct BaseCleanup {
    void* self; bool* armed;
    ~BaseCleanup() noexcept(false){if(*armed) kotor_fastcall_006db4e0(self,nullptr);}
};
struct SafeCleanup {
    void* self; bool* armed;
    ~SafeCleanup() noexcept(false){if(*armed) aurora_texture_safe_pointer_destroy_00422380(address(self,0x14),nullptr);}
};
void init_fields(void* self){
    store(self,0,word(&w4cam::camera_free_look_vtable_0075167c));
    store(self,0x14,0);
    store(self,0x48,0);store(self,0x44,0);store(self,0x40,0);store(self,0x3c,0x3f800000);
    store(self,0x4c,0);store(self,0x50,0);store(self,0x54,0);store(self,0x58,0);
    store(self,0x5c,0);store(self,0x60,0);store(self,0x64,0);store(self,0x68,0);store(self,0x6c,0);
}
const float zero=0.f,two=2.f,minus180=-180.f,plus180=180.f,plus90=90.f;
const char hook[]="FreeLookHook",camera_hook[]="CameraHook",root[]="root";
// This region consumes/pops its original FLD-zero in both branches, never returns float.
bool direct_turn(void* self,U dt){
    U result;
    __asm {
        mov esi, self
        fld dword ptr zero
        fcom dword ptr [esi+0x38]
        fnstsw ax
        test ah, 0x44
        jnp inertia
        fld dword ptr dt
        fmul dword ptr [esi+0x2c]
        fmul dword ptr [esi+0x38]
        fadd dword ptr [esi+0x20]
        fstp dword ptr [esi+0x20]
        // Original zero retained on ST0 through these four vectors.
        fst dword ptr [esi+0xb0]
        fst dword ptr [esi+0xb4]
        fst dword ptr [esi+0xb8]
        fst dword ptr [esi+0xa4]
        fst dword ptr [esi+0xa8]
        fst dword ptr [esi+0xac]
        fst dword ptr [esi+0x98]
        fst dword ptr [esi+0x9c]
        fst dword ptr [esi+0xa0]
        fst dword ptr [esi+0x8c]
        fst dword ptr [esi+0x90]
        fstp dword ptr [esi+0x94]
        mov result, 1
        jmp done
    inertia:
        fstp st(0)
        mov result, 0
    done:
    }
    return result!=0;
}
// Exact native@44/RET36 stack. Forward-declared canonical W4 types avoid copying their definitions.
void integrate(void* self,U dt){
    U out[6];
    __asm {
        mov esi, self
        push 0
        push 0
        push 0
        push 1
        push dt
        push 0
        push dword ptr [esi+0x24]
        push 0
        lea eax, out
        push eax
        lea ecx, [esi+0x7c]
        call kotor_fastcall_006d1190
        fld dword ptr [eax+0x10]
        fadd dword ptr [esi+0x20]
        mov dword ptr [esi+0x24], 0
        fstp dword ptr [esi+0x20]
    }
}
// Original one-step wrap and asymmetric FCOM masks, then clamp; unspilled value feeds pitch.
void orientation(void* self,U* q,kotor_scene3d::Quaternion* out){
    U adjustment;
    __asm {
        mov esi, self
        fld dword ptr [esi+0x20]
        mov ecx, 360
        fcomp dword ptr minus180
        mov adjustment, ecx
        fnstsw ax
        test ah, 5
        jnp add_wrap
        mov adjustment, 0
    add_wrap:
        fild dword ptr adjustment
        mov adjustment, ecx
        fadd dword ptr [esi+0x20]
        fcom dword ptr plus180
        fst dword ptr [esi+0x20]
        fnstsw ax
        test ah, 0x41
        je subtract_wrap
        mov adjustment, 0
    subtract_wrap:
        fisub dword ptr adjustment
        fst dword ptr [esi+0x20]
        fld dword ptr [esi+0x34]
        fchs
        fstp dword ptr adjustment
        fcom dword ptr adjustment
        fnstsw ax
        test ah, 5
        jp clamp_high
        fstp st(0)
        fld dword ptr adjustment
    clamp_high:
        fcom dword ptr [esi+0x30]
        fst dword ptr [esi+0x20]
        fnstsw ax
        test ah, 0x41
        jne selected
        fstp st(0)
        fld dword ptr [esi+0x30]
    selected:
        fst dword ptr [esi+0x20]
        fadd dword ptr [esi+0x74]
        sub esp, 12
        mov dword ptr [esp+8], 0
        fadd dword ptr plus90
        fstp dword ptr [esp+4]
        push q
        call quaternion_yaw_004a9f40
        add esp, 4
        fadd dword ptr [esi+0x70]
        fstp dword ptr [esp]
        push out
        call yaw_pitch_roll_004acac0
        add esp, 16
    }
}
void add_root_height(U* pos){__asm {
    mov eax, pos
    fld dword ptr [eax+8]
    fadd dword ptr two
    fstp dword ptr [eax+8]
}}
}
extern "C" void* __fastcall kotor_fastcall_0063a5d0(void* self,void*,void* owner,void* part,U effect){
    kotor_fastcall_0049f110(self,nullptr,owner);
    bool base_armed=true;BaseCleanup base_cleanup{self,&base_armed};
    init_fields(self);
    bool safe_armed=true;SafeCleanup safe_cleanup{self,&safe_armed};
    kotor_fastcall_006d0b20(address(self,0x7c),nullptr);
    store(self,0x18,word(owner));
    behavior_add_dependency_0049f130(self,nullptr,part);
    safe_pointer_assign_00447fa0(address(self,0x14),nullptr,part);
    store(self,0x38,0);store(self,0x24,0);store(self,0x20,0);store(self,0x70,0);store(self,0x74,0);
    kotor_fastcall_006383c0(self,nullptr);
    store(self,0x78,effect);kotor_fastcall_005edf10(client(),nullptr,static_cast<int>(effect));
    store(self,0xbc,0x43480000);store(self,0xc0,0x43fa0000);
    safe_armed=false;base_armed=false;return self;
}
extern "C" void* __fastcall kotor_fastcall_0063baf0(void* self,void*,void* settings){
    kotor_fastcall_0049f110(self,nullptr,pointer(settings,8));
    bool base_armed=true;BaseCleanup base_cleanup{self,&base_armed};
    init_fields(self);
    bool safe_armed=true;SafeCleanup safe_cleanup{self,&safe_armed};
    kotor_fastcall_006d0b20(address(self,0x7c),nullptr);
    safe_pointer_assign_00447fa0(address(self,0x14),nullptr,pointer(settings,0xc));
    store(self,0x18,load(settings,8));store(self,0x20,0);store(self,0x38,0);store(self,0x24,0);
    kotor_fastcall_006383c0(self,nullptr);
    const U effect=load(settings,0x1c);store(self,0x78,effect);
    kotor_fastcall_005edf10(client(),nullptr,static_cast<int>(effect));
    store(self,0xbc,0x43480000);store(self,0xc0,0x43fa0000);
    safe_armed=false;base_armed=false;return self;
}
extern "C" void __fastcall kotor_fastcall_0063a6c0(void* self,void*){
    store(self,0,word(&w4cam::camera_free_look_vtable_0075167c));
    bool base_armed=true,safe_armed=true;
    BaseCleanup base_cleanup{self,&base_armed};SafeCleanup safe_cleanup{self,&safe_armed};
    kotor_fastcall_005edf20(client(),nullptr);
    void* target=pointer(self,0x14);
    if(target!=nullptr) exo_array_list_remove_00445080(static_cast<kotor_scene3d::PointerList*>(address(target,4)),nullptr,address(self,0x14));
    safe_armed=false;base_armed=false;
    behavior_destruct_0049fa70(self,nullptr);
}
extern "C" void* __fastcall kotor_fastcall_0063bad0(void* self,void*,U flags){
    kotor_fastcall_0063a6c0(self,nullptr);if(flags&1) live_free(self);return self;
}
extern "C" U __fastcall kotor_fastcall_0063a6b0(void*,void*){return 0x106e;}
extern "C" U __fastcall kotor_fastcall_00639d00(void* self,void*,U dt){
    if(behavior_control_0049f270(self,nullptr,dt)==0) return 1;
    kotor_fastcall_005ee170(client(),nullptr,0x20);
    U pos[3]={0,0,0},q[4]={0x3f800000,0,0,0};
    using PartPosition=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U);
    // 00639D6E/00639D82/00639DA0: the original reloads [esi+14h] and reads its vtable unchecked (a null part faults there).
    auto get_part=[&](const char* name){
        auto* part=static_cast<aurora_model::Gob*>(pointer(self,0x14));
        return vslot<PartPosition>(part,0x98)(part,nullptr,word(name),word(pos),word(q));
    };
    if(get_part(hook)==0 && get_part(camera_hook)==0){get_part(root);add_root_height(pos);}
    auto* owner=static_cast<aurora_model::Gob*>(pointer(self,0x10));
    // 00639DCC: owner vtable read unchecked, as the original.
    Vec position,position_out;std::memcpy(&position,pos,12);
    vslot<Vec*(__fastcall*)(aurora_model::Gob*,void*,Vec*,Vec)>(owner,0x5c)(owner,nullptr,&position_out,position);
    if(!direct_turn(self,dt)) integrate(self,dt);
    kotor_scene3d::Quaternion result;
    orientation(self,q,&result);
    // Original loads the owner's slot, then reloads owner ECX after filling the argument stack.
    owner=static_cast<aurora_model::Gob*>(pointer(self,0x10));
    // 00639F8B: owner vtable read unchecked, as the original.
    using SetOrientation=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U,U);
    SetOrientation set=vslot<SetOrientation>(owner,0x60);
    U words[4],out[4];std::memcpy(words,&result,16);
    set(static_cast<aurora_model::Gob*>(pointer(self,0x10)),nullptr,word(out),words[0],words[1],words[2],words[3]);
    return 1;
}
