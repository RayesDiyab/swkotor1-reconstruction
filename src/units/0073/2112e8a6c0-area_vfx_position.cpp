// Full original00690B00. All arithmetic at original x87 widths; real scene/model providers retained.
#include "32a11cddfd-area_vfx_native.hpp"
using namespace kotor_w3;
struct AreaVfxQuaternion { float w,x,y,z; };
extern "C" {
AreaVfxQuaternion* __cdecl rotation_arc_004ab630(AreaVfxQuaternion*,Vector3,Vector3);
double __cdecl quaternion_yaw_004a9f40(const AreaVfxQuaternion*);
AreaVfxQuaternion* __cdecl yaw_pitch_roll_004acac0(AreaVfxQuaternion*,float,float,float);
AreaVfxQuaternion* __cdecl quaternion_multiply_004a9ac0(AreaVfxQuaternion*,const AreaVfxQuaternion*,const AreaVfxQuaternion*);
void* __fastcall aurora_hit_info_construct_004578f0(void*,void*,U,U,U);
void __fastcall chit_info_destruct_00456b30(void*,void*);
void* __fastcall area_vfx_hitcheck_list_005ed8c0(void*,void*);
}
namespace {
bool player_direction(const Vector3* player,const Vector3* position,Vector3* out) {
    const float threshold=fbits(0x3727c5ac);
    unsigned short status;
    __asm {
        mov ecx,player
        mov edx,position
        mov esi,out
        fld dword ptr [ecx+8]
        fsub dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fsub dword ptr [edx+4]
        fld dword ptr [ecx]
        fsub dword ptr [edx]
        fstp dword ptr [esi]
        fst dword ptr [esi+4]
        fld st(1)
        fstp dword ptr [esi+8]
        fld dword ptr [esi]
        fmul dword ptr [esi]
        fld st(1)
        fmul st(2)
        faddp st(1),st(0)
        fld st(2)
        fmul st(3)
        faddp st(1),st(0)
        fcomp dword ptr threshold
        fnstsw ax
        fstp st(0)
        mov status,ax
        fstp st(0)
    }
    return (status&0x4100u)==0;
}
float yaw_store(const AreaVfxQuaternion* rotation) {
    float result;
    __asm {
        push rotation
        call quaternion_yaw_004a9f40
        add esp,4
        fstp dword ptr result
    }
    return result;
}
struct HitLocal {
    unsigned char bytes[0x64];
    HitLocal() { aurora_hit_info_construct_004578f0(bytes,nullptr,0,0xbfffffffu,0); }
    ~HitLocal() noexcept(false) { chit_info_destruct_00456b30(bytes,nullptr); }
};
}
extern "C" void* __fastcall area_vfx_hitcheck_list_005ed8c0(void* app,void*) { return rp(rp(app,4),0xa4); }
extern "C" void __fastcall area_vfx_set_position_00690b00(void* self,void*,const Vector3* position) {
    void* sound=rp(self,0xfc);
    if(sound!=nullptr)kotor_m15_sound_source_set_position_005d59e0(sound,nullptr,position,1.5f);
    area_vfx_base_set_position_0063f340(self,nullptr,position);
    AreaVfxQuaternion rotation{1.0f,0.0f,0.0f,0.0f};
    const U effect=ld<U>(self,0x108);
    if(effect==39 || effect==40) {
        if(client_get_player_creature_005ed540(client_app(),nullptr)!=nullptr) {
            void* player=client_get_player_creature_005ed540(client_app(),nullptr);
            Vector3 player_position,direction;
            std::memcpy(&player_position,at(player,0x24),12);
            if(player_direction(&player_position,position,&direction)) {
                Vector3 normalized;
                const Vector3* n=vector_normalized_004ab180(&normalized,&direction);
                const Vector3 forward{0.0f,1.0f,0.0f};
                AreaVfxQuaternion arc;
                rotation=*rotation_arc_004ab630(&arc,forward,*n);
                AreaVfxQuaternion yaw;
                rotation=*yaw_pitch_roll_004acac0(&yaw,yaw_store(&rotation),0.0f,0.0f);
            }
        }
        vslot<U(__fastcall*)(void*,void*,U)>(self,0xa8)(self,nullptr,3);
    }
    if(ld<U>(self,0x120)!=0) {
        void* module=kotor_fastcall_005ed530(client_app(),nullptr);
        void* scene=rp(rp(module,0x48),0x184);
        if(scene!=nullptr) {
            HitLocal hit;
            Vector3 hit_position{0.0f,0.0f,0.0f};
            st<U>(hit.bytes,0,1);
            st<void*>(hit.bytes,4,area_vfx_hitcheck_list_005ed8c0(client_app(),nullptr));
            Vector3 start,end;
            std::memcpy(&start,position,12);std::memcpy(&end,position,12);
            const float height=10.0f;
            __asm {
                mov eax,position
                fld dword ptr [eax+8]
                fadd dword ptr height
                fstp dword ptr start.z
                fld dword ptr [eax+8]
                fsub dword ptr height
                fstp dword ptr end.z
            }
            if(vslot<U(__fastcall*)(void*,void*,Vector3,Vector3,Vector3*,void*)>(scene,0x38)(scene,nullptr,start,end,&hit_position,hit.bytes)!=0) {
                Vector3 normal;std::memcpy(&normal,at(hit.bytes,0x40),12);
                const Vector3 up{0.0f,0.0f,1.0f};
                AreaVfxQuaternion arc,combined;
                const AreaVfxQuaternion* a=rotation_arc_004ab630(&arc,up,normal);
                rotation=*quaternion_multiply_004a9ac0(&combined,a,&rotation);
            }
        }
    }
    void* model=rp(self,0x68);
    if(model!=nullptr)vslot<U(__fastcall*)(void*,void*,const AreaVfxQuaternion*)>(model,0x60)(model,nullptr,&rotation);
}
