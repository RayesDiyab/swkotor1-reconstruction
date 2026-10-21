#pragma once
#include "d14b72eb41-model_family_b.h"
#include "450483e25e-model_inherited_b.h"
#include "628decf93e-m15e_animbase_internal.h"

namespace model_family_b_detail {
using namespace kotor_m15e_ab;
struct Name { char text[20]; };
inline void copy_forward(void* to,const void* from,U bytes) {
    U offset=0;
    for(U count=bytes>>2;count!=0;--count,offset+=4) st<U>(to,offset,ld<U>(from,offset));
    for(U count=bytes&3u;count!=0;--count,++offset) st<U8>(to,offset,ld<U8>(from,offset));
}
inline void save_ref(void* target,U source) {
    Ref scratch;
    c2da_copy_00406120(static_cast<Ref*>(target),nullptr,&scratch,reinterpret_cast<Ref*>(source));
}
inline void* head_tw(void* self) { return reinterpret_cast<void*>(wd(self)+ld<U>(pv(self,-0x50),4)-0x50u); }
struct StringSlot {
    alignas(StartupString) unsigned char raw[sizeof(StartupString)];
    bool live=false;
    StartupString* get() {return reinterpret_cast<StartupString*>(raw);}
    ~StringSlot() {if(live)get()->~StartupString();}
};
}

extern "C" {
kotor_m15e::U __fastcall recovered_anim_base_set_env_map_0069d7b0(void*,void*,kotor_m15e::U,kotor_m15e::U,void*);
kotor_m15e::U __fastcall recovered_anim_base_internal_0069e8d0(void*,void*,kotor_m15e::U,kotor_m15e::U,kotor_m15e::U,kotor_m15e::U,kotor_m15e::U);
void __fastcall recovered_anim_base_turn_off_shadows_0069dc30(void*,void*) noexcept(false);
kotor_m15e::U __fastcall model_disable_bumped_out_shell_0069ddf0(void*,void*) noexcept(false);
kotor_m15e::U __fastcall model_set_motion_blur_0069de10(void*,void*,kotor_m15e::U) noexcept(false);
}
