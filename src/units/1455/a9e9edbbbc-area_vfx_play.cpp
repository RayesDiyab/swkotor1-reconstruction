// Original00690510: impact/cessation one-shot and duration looping sound, real providers only.
#include "32a11cddfd-area_vfx_native.hpp"
#include "bd9f6ce7e8-m15e_animbase.h"
using namespace kotor_w3;
using namespace area_vfx_native;
extern "C" {
extern void* kotor_cell_sound_007a39ec;
U __fastcall kotor_m15_c2da_get_string_entry_00413de0(void*,void*,void*,void*,void*);
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void*,void*,U);
void __fastcall kotor_fastcall_005d5930(void*,void*);
struct KotorM15VectorWords { float x,y,z; };
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void*,void*,void*,KotorM15VectorWords,float,U,U,U,float,float);
// Original initialized dword3F4CCCCD, only original00690562 reader; owner census has no existing provider.
volatile U area_vfx_scale_007a2674=0x3f4ccccd;
}
namespace {
// The canonical +054 ABI carries rate bits as a DWORD. No floating conversion.
__declspec(noinline) void animation_play_raw(void* animation,U mode,U rate_bits,U looping) {
    const auto target=vslot<decltype(KotorM15eAnimBaseVtbl::s054)>(animation,0x54);
    target(animation,nullptr,mode,rate_bits,looping,0);
}
U animation_duration(void* animation,U mode) {
    const auto target=vslot<float(__fastcall*)(void*,void*,U)>(animation,0x38);
    U result;
    __asm {
        mov ecx,animation
        xor edx,edx
        push mode
        call target
        call genuine_x87_ftol_006fae8c
        mov result,eax
    }
    return result;
}
}
extern "C" void __fastcall area_vfx_play_00690510(void* self,void*,U mode) {
    const U effect=ld<U>(self,0x108);
    st<U>(self,0x104,0);st<U16>(self,0x10c,U16(mode));
    LocalString row(effect);
    const U rate_bits=ld<U>(self,0x108)==40?area_vfx_scale_007a2674:0x3f800000u;
    if(U16(mode)==1 || U16(mode)==3) {
        const U animation_mode=U16(mode)==1?1u:3u;
        LocalString sound;
        U found;
        { LocalString column(animation_mode==1?"SoundImpact":"SoundCessation");
          found=kotor_m15_c2da_get_string_entry_00413de0(visual_table(),nullptr,&row.value,&column.value,&sound.value); }
        if(found==0)c2da_assign_text_005e5140(&sound.value,nullptr,"");
        ResRef reference;
        async_ref_construct_string_00406d60(&reference,nullptr,&sound.value);
        KotorM15VectorWords position;std::memcpy(&position,at(self,0x24),12);
        kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec,nullptr,&reference,position,1.5f,13,0,0,0.0f,0.0f);
        if(animation_mode==3) {
            void* source=rp(self,0xfc);
            if(source!=nullptr) {
                kotor_m15_sound_source_stop_005d5a20(source,nullptr);
                source=rp(self,0xfc);if(source!=nullptr)delete_object(source);
                st<U>(self,0xfc,0);
            }
        }
        void* animation=rp(self,0x68);
        if(animation!=nullptr) {
            st<U>(self,0x104,animation_duration(animation,animation_mode));
            const U looping=ld<U>(self,0x12c)!=0?0u:1u;
            animation=rp(self,0x68);
            animation_play_raw(animation,animation_mode,rate_bits,looping);
        }
        return;
    }
    LocalString sound;
    U found;
    { LocalString column("SoundDuration");
      found=kotor_m15_c2da_get_string_entry_00413de0(visual_table(),nullptr,&row.value,&column.value,&sound.value); }
    if(found==0)c2da_assign_text_005e5140(&sound.value,nullptr,"");
    void* allocation=live_operator_new(8);
    void* source=nullptr;
    try {
        if(allocation!=nullptr) {
            ResRef reference;
            async_ref_construct_string_00406d60(&reference,nullptr,&sound.value);
            source=kotor_sound_rev3::SourceConstruct005d60e0(allocation,nullptr,&reference);
        }
    } catch(...) { live_free(allocation);throw; }
    st<void*>(self,0xfc,source);
    if(source!=nullptr) {
        kotor_m15_sound_source_set_3d_005d5910(source,nullptr,1);
        kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self,0xfc),nullptr,13);
        kotor_m15_sound_source_set_looping_005d59d0(rp(self,0xfc),nullptr,1);
        kotor_fastcall_005d5930(rp(self,0xfc),nullptr);
    }
    void* animation=rp(self,0x68);
    if(animation!=nullptr)animation_play_raw(animation,mode,rate_bits,0);
}
