// CSWCItem / CSWItem: original GOG K1 x86 behavior, including original field reloads.
#include "7a6ead434c-w3_item.h"
#include "bc22045a27-w3_cells.h"
#include "46aa47f6e9-w3_item_contracts.h"
#include "cca80f7c35-m15e.h"
#include "cc436f6baa-m15e_animbase.h"
#include "47ac9dbbbc-scene3d.h"
#include <cstring>
#include <cstddef>
using U=std::uint32_t;
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern volatile U scene3d_hit_trimeshes_007fbf4c;
extern const void* const recovered_sw_item_methods_0074b2a0[1];
void __cdecl live_free(void*);
menu_continuation::WorldTimer* __fastcall frame_GetWorldTimer_005ed4f0(void*,void*);
void __fastcall frame_GetWorldTime_004ade40(menu_continuation::WorldTimer*,void*,U*,U*);
U __fastcall recovered_world_time_difference_004ae460(void*,void*,U,U,U,U,U*,U*);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
unsigned char __fastcall recovered_object_array_remove_004d81a0(void*,void*,U,void**);
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void*,void*);
U __fastcall kotor_m15e_client_ai_list_remove_object_00604810(void*,void*,void*);
U __fastcall kotor_m15e_client_ai_master_remove_object_00604a30(void*,void*,void*);
U __fastcall kotor_fastcall_0063df60(void*,void*);
void __fastcall area_vfx_base_set_position_0063f340(void*,void*,const kotor_w3::Vector3*);
double __fastcall w5_server_compute_height_004aea50(void*,void*,void*,w5::Vector*,U);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,std::int32_t);
float __cdecl kotor_m15a_yaw_004aa0f0(const void*);
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion*,float,float,float);
}
namespace {
template<class T>T load(const void* p,std::ptrdiff_t o){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,sizeof v);return v;}
template<class T>void store(void* p,std::ptrdiff_t o,T v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,sizeof v);}
void* at(void* p,std::ptrdiff_t o){return reinterpret_cast<void*>(reinterpret_cast<U>(p)+static_cast<U>(o));}
void* ptr(const void* p,std::ptrdiff_t o){return load<void*>(p,o);}
template<class F>F slot(void* p,U o){return reinterpret_cast<F>(ptr(ptr(p,0),o));}
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
using Delete=void*(__fastcall*)(void*,void*,U);
void copy3(void* to,const void* from){store<U>(to,0,load<U>(from,0));store<U>(to,4,load<U>(from,4));store<U>(to,8,load<U>(from,8));}
void copy4(void* to,const void* from){store<U>(to,0,load<U>(from,0));store<U>(to,4,load<U>(from,4));store<U>(to,8,load<U>(from,8));store<U>(to,12,load<U>(from,12));}
const float animation_finished=-1.0f;
const float gap=0.1f;
const U threshold_bits=0x3727C5ACu;
}
extern "C" {
void* __fastcall recovered_sw_item_scalar_destructor_005b4770(void* self,void*,U flags) noexcept(false){
    store<const void*>(self,0,recovered_sw_item_methods_0074b2a0);
    if(flags&1u)live_free(self);
    return self;
}
void __fastcall recovered_client_item_ai_update_00646390(void* self,void*) noexcept(false){
    U day,time,day_difference,time_difference;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,&day,&time);
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client(),nullptr),nullptr,
        day,time,load<U>(self,0x3C),load<U>(self,0x40),&day_difference,&time_difference);
    store<U>(self,0x44,time_difference);
    const auto* table=load<const KotorM15ObjectMethods_007516c8*>(self,0);
    table->s0e8_update_pop_up_bubble(self,nullptr);
    if(kotor_fastcall_0063df60(self,nullptr)!=0){
        void* const object=reinterpret_cast<U>(self)==0x10u?nullptr:self;
        kotor_m15e_client_ai_list_remove_object_00604810(at(kotor_m15e_client_get_client_ai_master_005ed6b0(client(),nullptr),4),nullptr,object);
        void* removed;
        const auto result=recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client(),nullptr),nullptr,load<U>(self,4),&removed);
        if(result==0 && removed!=nullptr){
            void* const full=at(self,-0x10);
            if(full!=nullptr)slot<Delete>(full,0)(full,nullptr,1);
            return;
        }
    }
    void* shadow=ptr(self,0x184);
    if(shadow!=nullptr){
        float animation_time=0.0f;
        load<const aurora_model::GobVTable*>(shadow,0)->s01c_gob_get_animation(
            static_cast<aurora_model::Gob*>(shadow),nullptr,reinterpret_cast<U>("impact"),reinterpret_cast<U>(&animation_time),0);
        unsigned char equal;
        __asm {fld dword ptr animation_time
            fcomp dword ptr animation_finished
            fnstsw ax
            test ah,44h
            setnp equal}
        if(equal){
            shadow=ptr(self,0x184);
            load<const aurora_model::GobVTable*>(shadow,0)->s050_gob_attach_to_object(shadow,nullptr,nullptr,"",0);
            shadow=ptr(self,0x184);
            if(shadow!=nullptr)load<const aurora_model::GobVTable*>(shadow,0)->s000_gob_destruct_gob(static_cast<aurora_model::Gob*>(shadow),nullptr,1);
            store<void*>(self,0x184,nullptr);
            void* const object=reinterpret_cast<U>(self)==0x10u?nullptr:self;
            kotor_m15e_client_ai_master_remove_object_00604a30(kotor_m15e_client_get_client_ai_master_005ed6b0(client(),nullptr),nullptr,object);
        }
    }
    store<U>(self,0x3C,day);store<U>(self,0x40,time);
}
void __fastcall recovered_client_item_align_model_00646a30(void* self,void*,const void* normal) noexcept(false){
    Quaternion q{1.0f,0.0f,0.0f,0.0f};
    float roll=0.0f,pitch=0.0f;
    void* descriptor=kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(kotor_cell_rules_007a3a28,0x34),nullptr,load<std::int32_t>(self,0xC));
    if(load<unsigned char>(descriptor,0x88)==1)roll=90.0f;
    else{
        descriptor=kotor_m15b_base_item_array_get_base_item_005b31a0(ptr(kotor_cell_rules_007a3a28,0x34),nullptr,load<std::int32_t>(self,0xC));
        if(load<unsigned char>(descriptor,0x88)==2)pitch=90.0f;
    }
    const void* direction=at(self,0x40);float yaw;
    __asm{push direction
        call kotor_m15a_yaw_004aa0f0
        fstp dword ptr yaw
        add esp,4}
    Quaternion temporary;
    copy4(&q,yaw_pitch_roll_004acac0(&temporary,yaw,pitch,roll));
    if(recovered_client_item_ground_align_00833a98!=0){
        Vector target;copy3(&target,normal);
        Quaternion arc,product;
        const auto* const rotation=rotation_arc_004ab630(&arc,Vector{0.0f,0.0f,1.0f},target);
        copy4(&q,quaternion_multiply_004a9ac0(&product,rotation,&q));
    }
    void* const model=ptr(self,0x78);
    if(model!=nullptr)load<const KotorM15eAnimBaseVtbl*>(model,0)->s060(model,nullptr,reinterpret_cast<const float*>(&q));
}
void __fastcall recovered_client_item_set_orientation_00646dd0(void* self,void*,const float* direction) noexcept(false){
    copy3(at(self,0x30),direction);
    Vector normal{0.0f,0.0f,1.0f};
    if(recovered_client_item_ground_align_00833a98!=0 && ptr(self,0x20)!=nullptr){
        w5::Vector position;copy3(&position,at(self,0x24));store<U>(&position,8,0);
        void* area=ptr(self,0x20);
        if(area!=nullptr){
            void* const server=ptr(kotor_menu_window_cells::manager_007a39fc,8);
            void* const area_arg=at(area,0x10C);w5::Vector* const position_arg=&position;
            __asm{push 1
                push position_arg
                push area_arg
                xor edx,edx
                mov ecx,server
                call w5_server_compute_height_004aea50
                fstp st(0)}
        }
    }
    recovered_client_item_align_model_00646a30(at(self,-0x10),nullptr,&normal);
}
void __fastcall recovered_client_item_set_position_00647030(void* self,void*,const float* input) noexcept(false){
    w5::Vector position;
    const U x=load<U>(input,0),y=load<U>(input,4);
    volatile U discarded_z=load<U>(input,8);(void)discarded_z;
    store<U>(&position,0,x);store<U>(&position,4,y);store<U>(&position,8,0);
    Vector normal{0.0f,0.0f,1.0f};
    void* const full=at(self,-0x10);
    void* volatile* const manager_cell=&kotor_menu_window_cells::manager_007a39fc;
    w5::Vector* const position_arg=&position;
    U mode=recovered_client_item_ground_align_00833a98;
    // Keep zero/height in ST0 through the optional flag restoration and FADD.
    __asm{fldz
        cmp mode,0
        jne direct_height
        mov eax,scene3d_hit_trimeshes_007fbf4c
        mov mode,eax
        mov scene3d_hit_trimeshes_007fbf4c,1
        mov eax,full
        mov eax,[eax+30h]
        test eax,eax
        jz restore_flag
        fstp st(0)
        mov ecx,manager_cell
        mov ecx,[ecx]
        mov ecx,[ecx+8]
        add eax,10Ch
        push 1
        push position_arg
        push eax
        xor edx,edx
        call w5_server_compute_height_004aea50
    restore_flag:
        mov eax,mode
        mov scene3d_hit_trimeshes_007fbf4c,eax
        jmp add_gap
    direct_height:
        mov eax,full
        mov eax,[eax+30h]
        test eax,eax
        jz add_gap
        fstp st(0)
        mov ecx,manager_cell
        mov ecx,[ecx]
        mov ecx,[ecx+8]
        add eax,10Ch
        push 1
        push position_arg
        push eax
        xor edx,edx
        call w5_server_compute_height_004aea50
    add_gap:
        fadd dword ptr gap
        mov eax,position_arg
        fstp dword ptr[eax+8]}
    area_vfx_base_set_position_0063f340(self,nullptr,reinterpret_cast<const kotor_w3::Vector3*>(&position));
    if(recovered_client_item_ground_align_00833a98!=0){
        unsigned char should_align;
        __asm{mov ecx,self
            fld dword ptr[ecx+38h]
            fmul dword ptr[ecx+38h]
            fld dword ptr[ecx+34h]
            fmul dword ptr[ecx+34h]
            faddp st(1),st(0)
            fld dword ptr[ecx+30h]
            fmul dword ptr[ecx+30h]
            faddp st(1),st(0)
            fcomp dword ptr threshold_bits
            fnstsw ax
            test ah,41h
            sete should_align}
        if(should_align)recovered_client_item_align_model_00646a30(full,nullptr,&normal);
    }
}
}
