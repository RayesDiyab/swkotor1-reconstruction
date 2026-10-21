// Original instruction contracts: ../original-0060*.asm / original-0061*.asm.
// Missing table entry bodies only. Existing allocators, predicates, portrait,
// vector normalization and highlight providers retain their sole ownership.
#include "1380d00cbd-creature_slots_0832.h"
#include "547b570114-m15e.h"
#include "916798d1e4-resources.h"
#include "62420f93e8-allocator_live.h"
#include "177aadaf45-scene3d.h"
#include <cstring>

struct W4Vector;
namespace kotor_w3 { struct Vector3; }
extern "C" {
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void*,void*);
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void*,void*,std::uint32_t);
std::uint32_t __fastcall kotor_fastcall_0063c380(void*,void*,std::uint32_t);
void __fastcall kotor_fastcall_00611850(void*,void*,W4Vector*);
kotor_scene3d::Vector* __cdecl normalize_004ab180(kotor_scene3d::Vector*,const kotor_scene3d::Vector*);
void __fastcall kotor_fastcall_004015a0(void*,void*);
menu_resources::Ref* __fastcall kotor_m15e_portrait_get_portrait_00678ef0(void*,void*,menu_resources::Ref*,std::uint32_t);
std::uint32_t __fastcall area_vfx_base_hilite_0063cb00(void*,void*,const kotor_w3::Vector3*,std::uint32_t,std::uint32_t,std::uint32_t);
std::uint32_t __fastcall area_vfx_base_unhilite_0063cb30(void*,void*,std::uint32_t);
}

namespace {
using U=std::uint32_t;
using B=unsigned char;
template<class T> T load(const void* object,U offset) {
    T result; std::memcpy(&result,static_cast<const B*>(object)+offset,sizeof result); return result;
}
template<class T> void store(void* object,U offset,T value) {
    std::memcpy(static_cast<B*>(object)+offset,&value,sizeof value);
}
const KotorM15ObjectMethods_007516c8* methods(void* self) {
    return load<const KotorM15ObjectMethods_007516c8*>(self,0);
}
template<class F> F slot(void* self,U offset) {
    void* const* const table=load<void* const*>(self,0);
    return reinterpret_cast<F>(table[offset/4]);
}
}

extern "C" {
// 0060B900: two independent DWORD stores, RET8.
void __fastcall creature_reset_update_times_0060b900(void* self,void*,U day,U tick) {
    store(self,0x3c,day);store(self,0x40,tick);
}
// 0060C190/1A0/1C0: current pointer and WORD-count boundaries; no null repair.
void* __fastcall creature_current_waypoint_0060c190(void* self,void*) {
    return load<void*>(self,0x1c0);
}
void* __fastcall creature_end_waypoint_0060c1a0(void* self,void*) {
    const auto count=load<std::uint16_t>(self,0x1ec);
    if(count==0)return nullptr;
    const U base=load<U>(self,0x1bc);
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(base+static_cast<U>(count)*12u-12u));
}
void* __fastcall creature_start_waypoint_0060c1c0(void* self,void*) {
    return load<std::uint16_t>(self,0x1ec)==0?nullptr:load<void*>(self,0x1bc);
}
U __fastcall creature_next_waypoint_0060c4a0(void* self,void*) {
    const auto count=load<std::uint16_t>(self,0x1ec);
    if(count==0)return 0;
    const U current=load<U>(self,0x1c0);
    if(current==load<U>(self,0x1bc)+static_cast<U>(count)*12u-12u)return 0;
    store(self,0x1c0,current+12u);return 1;
}
// 0060E770/7A0 have the same original branch/argument semantics as the
// already selected base0063CB00/30; reuse those bodies rather than copy them.
U __fastcall creature_hilite_0060e770(void* self,void*,U color,U channel,U unused,U flag) {
    return area_vfx_base_hilite_0063cb00(self,nullptr,
        reinterpret_cast<const kotor_w3::Vector3*>(static_cast<std::uintptr_t>(color)),channel,unused,flag);
}
U __fastcall creature_unhilite_0060e7a0(void* self,void*,U channel) {
    return area_vfx_base_unhilite_0063cb30(self,nullptr,channel);
}
// 00617030 is the raw portrait wrapper, not the alignment-aware00616FF0 slot.
void* __fastcall creature_portrait_ref_00617030(void* self,void*,void* result,U variant) {
    auto* const out=static_cast<menu_resources::Ref*>(result);
    void* const stats=load<void*>(self,0x2f8);
    if(stats!=nullptr)kotor_m15e_portrait_get_portrait_00678ef0(static_cast<B*>(stats)+0x70,nullptr,out,variant);
    else out->construct_00406d80("");
    return out;
}
std::uint16_t __fastcall creature_portrait_id_00617070(void* self,void*) {
    const void* const stats=load<void*>(self,0x2f8);
    return stats?load<std::uint16_t>(stats,0x11c):static_cast<std::uint16_t>(0xffffu);
}
U __fastcall creature_is_hostile_006170c0(void* self,void*) {
    return load<std::uint8_t>(self,0x2dc)==2?1u:0u;
}
void* __fastcall object_scalar_destroy_0063f5c0(void* self,void*,U flags) {
    kotor_m15e_object_destruct_0063e800(self,nullptr);
    if((flags&1u)!=0)live_free(self);
    return self;
}
// 0060C8E0: reload +68 after the virtual release; callback may replace it.
// EAX is unspecified on the null path; its actual caller contract is void.
void __fastcall creature_unload_model_0060c8e0(void* self,void*) {
    kotor_fastcall_004015a0(self,nullptr);
    void* animation=load<void*>(self,0x68);
    if(animation==nullptr)return;
    slot<U(__fastcall*)(void*,void*)>(animation,0x78)(animation,nullptr);
    animation=load<void*>(self,0x68);
    if(animation!=nullptr)
        slot<void*(__fastcall*)(void*,void*,U)>(animation,0)(animation,nullptr,1);
    store<void*>(self,0x68,nullptr);
}
U __fastcall creature_bark_string_0060f510(void* self,void*,U a,U b,U sound,U d) {
    methods(self)->s128_lip_sync(self,nullptr,sound);
    return methods(self)->s124_bark_bubble(self,nullptr,a,b,sound,d);
}
void __fastcall creature_sync_animation_0060c100(void* self,void*,U animation,U tick) {
    store(self,0x4c,tick);
    switch(animation) {
    case 0x2719:case 0x277d:case 0x271c:case 0x271b:
    case 0x271e:case 0x2716:case 0x2718:case 0x27ac:return;
    default:break;
    }
    if(animation==static_cast<U>(kotor_m15a_object_get_current_animation_0063c060(self,nullptr)))return;
    if(kotor_fastcall_0063c380(self,nullptr,animation)!=0)
        methods(self)->s0a0_add_fire_forget_animation(self,nullptr,animation,0);
    else methods(self)->s09c_set_animation(self,nullptr,animation,0,0);
}
void __fastcall creature_sync_orientation_00611ec0(void* self,void*,const float* orientation) {
    kotor_scene3d::Vector normalized;
    const auto* input=reinterpret_cast<const kotor_scene3d::Vector*>(orientation);
    const auto* first=normalize_004ab180(&normalized,input);
    std::memcpy(static_cast<B*>(self)+0x1d0,first,12);
    const auto* second=normalize_004ab180(&normalized,input); // original repeats the call
    std::memcpy(static_cast<B*>(self)+0x1c4,second,12);
    const U animation=kotor_m15a_object_get_current_animation_0063c060(self,nullptr);
    const U offset=kotor_m15a_object_animation_stationary_0063c170(self,nullptr,animation)!=0?0x1d0u:0x1c4u;
    kotor_fastcall_00611850(self,nullptr,reinterpret_cast<W4Vector*>(static_cast<B*>(self)+offset));
}
}
