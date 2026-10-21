// Original005FB960 announcement conditions and005FB490 queue mutation, full data-dependent paths.
#include "32a11cddfd-area_vfx_native.hpp"
using namespace kotor_w3;
using namespace area_vfx_native;
extern "C" {
extern void* kotor_cell_sound_007a39ec;
I32 __fastcall kotor_fastcall_00414110(void*,void*,void*,void*,I32*);
I32 __fastcall kotor_fastcall_00413fa0(void*,void*,void*,void*,float*);
I32 __fastcall c2da_current_get_float_00413430(void*,void*,I32,void*,float*);
void __fastcall area_vfx_listener_position_005d63c0(void*,void*,Vector3*);
void __fastcall gui_load_event_list_allocate_004186e0(void*,void*,I32);
U __fastcall area_vfx_queue_announce_005fb490(void*,void*,I32);
}
namespace {
bool in_range(const Vector3* listener,const Vector3* position,const float* radius) {
    unsigned short status;
    __asm {
        mov ecx,listener
        mov edx,position
        fld dword ptr [ecx+8]
        fsub dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fsub dword ptr [edx+4]
        fld dword ptr [ecx]
        fsub dword ptr [edx]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        mov ecx,radius
        fld dword ptr [ecx]
        fmul dword ptr [ecx]
        fcompp
        fstp st(0)
        fnstsw ax
        fstp st(0)
        mov status,ax
        fstp st(0)
    }
    // TEST AH,5 / JNP exit: equality and unordered both enter the queue, exactly as the original.
    const U bits=status&0x0500u;
    return bits==0 || bits==0x0500u;
}
}
extern "C" void __fastcall area_vfx_internal_announce_005fb960(void* internal,void*,U effect,U kind,const Vector3* position) {
    LocalString row;
    const U8 type=U8(kind);
    void* table;
    if(type==1) {
        table=rp(rp(kotor_cell_rules_007a3a28,0xb8),0x88);
        { LocalString temporary(effect);async_assign_string_005e5c50(&row.value,nullptr,&temporary.value); }
    } else if(type==0) table=rp(rp(kotor_cell_rules_007a3a28,0xb8),0x58);
    else return;
    if(table==nullptr)return;
    I32 pattern;
    I32 found;
    { LocalString column("RumblePattern");
      found=type==1?kotor_fastcall_00414110(table,nullptr,&row.value,&column.value,&pattern):
          c2da_current_get_int_00413660(table,nullptr,effect,&column.value,reinterpret_cast<U*>(&pattern)); }
    if(found==0)return;
    void* player=client_get_player_creature_005ed540(client_app(),nullptr);
    if(player==nullptr)return;
    float radius;
    { LocalString column("RumbleCutOff");
      found=type==1?kotor_fastcall_00413fa0(table,nullptr,&row.value,&column.value,&radius):
          c2da_current_get_float_00413430(table,nullptr,I32(effect),&column.value,&radius); }
    if(found==0)radius=10.0f;
    Vector3 listener{0.0f,0.0f,0.0f};
    void* gui=frame_R4_005ed690(client_app(),nullptr);
    if(rp(gui,0xb4)!=nullptr)area_vfx_listener_position_005d63c0(kotor_cell_sound_007a39ec,nullptr,&listener);
    else std::memcpy(&listener,at(player,0x24),12);
    if(in_range(&listener,position,&radius))area_vfx_queue_announce_005fb490(internal,nullptr,pattern);
}
extern "C" U __fastcall area_vfx_queue_announce_005fb490(void* internal,void*,I32 pattern) {
    if(pattern<0 || pattern>=ld<I32>(internal,0x344))return 0;
    const U pattern_offset=U(pattern)*36u;
    const U unique=ld<U>(rp(internal,0x340),pattern_offset+0x20);
    const I32 old_count=ld<I32>(internal,0x350);
    if(unique!=0 && old_count>0) {
        for(I32 index=0;index<old_count;++index)
            if(ld<I32>(rp(internal,0x34c),U(index)*12u)==pattern)return 0;
    }
    const U new_count=U(old_count)+1u;
    void* list=at(internal,0x34c);
    if(new_count==0) {
        void* data=rp(list,0);
        if(data!=nullptr) { live_free(data);st<U>(list,0,0);st<U>(list,8,0); }
    } else gui_load_event_list_allocate_004186e0(list,nullptr,I32(new_count));
    st<U>(list,4,new_count);
    const U offset=U(old_count)*12u;
    st<I32>(rp(list,0),offset,pattern);
    st<U>(rp(list,0),offset+4,0);st<U>(rp(list,0),offset+8,1);
    return 1;
}
extern "C" void __fastcall area_vfx_listener_position_005d63c0(void* sound,void*,Vector3* out) {
    void* internal=rp(sound,0);
    if(internal!=nullptr)std::memcpy(out,at(internal,0x98),12);
}
