#include "0f939505b7-voice_start.h"
#include <cstring>



namespace {
using U=std::uint32_t;
void* ptr(void* p,unsigned n){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
U word(void* p,unsigned n){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void put(void* p,unsigned n,U v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
void setptr(void* p,unsigned n,void* v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
U different_from_zero(void* p,unsigned n) {
    const U zero=0;U result;auto at=static_cast<unsigned char*>(p)+n;
    __asm {
        mov ecx,at
        fld dword ptr [ecx]
        fcomp dword ptr zero
        fnstsw ax
        test ah,44h
        setp al
        movzx eax,al
        mov result,eax
    }
    return result;
}
U load_store_float(void* p,unsigned n) {
    U result;auto at=static_cast<unsigned char*>(p)+n;
    __asm {
        mov ecx,at
        fld dword ptr [ecx]
        fstp dword ptr result
    }
    return result;
}
U reverse_z(U value) {
    const U negative_one=0xbf800000;U result;
    __asm {
        fld dword ptr value
        fmul dword ptr negative_one
        fstp dword ptr result
    }
    return result;
}
U milliseconds() {
    using namespace kotor_menu_10_19;
    auto timer=static_cast<TimerDependency*>(ptr(dependencies::base_007a39e0,4));
    return static_cast<U>(dependencies::unsigned_divide_006fb030(timer->high_resolution_005e9260(),1000));
}
}
namespace sound_lower29_required {
void __fastcall start3d_005d71c0(void* system,void*,void* actor,void* voice) {
    const U looping=word(actor,0x24)==0;setptr(voice,0,actor);
    U position[3]={word(actor,0x78),word(actor,0x7c),word(actor,0x80)};

    const U distance1=word(actor,0x6c),distance2=word(actor,0x70);

    setptr(actor,0x3c,voice);
    void* resource=ptr(actor,8);
    using Data=void*(__fastcall*)(void*,void*);
    auto data=reinterpret_cast<Data>(ptr(ptr(resource,0),0x14))(resource,nullptr);
    AIL_set_3D_sample_file(ptr(voice,4),data);
    AIL_set_3D_sample_loop_count(ptr(voice,4),looping);
    sound_voice_required::actor_volume_005db800(actor,nullptr,static_cast<unsigned char*>(actor)[0x85],0,0x3f800000);



    AIL_set_3D_sample_distances(ptr(voice,4),distance1,distance2);

    AIL_set_3D_sample_offset(ptr(voice,4),word(actor,0x88));
    AIL_set_3D_position(ptr(voice,4),position[0],position[1],reverse_z(position[2]));
    U rate=different_from_zero(actor,0x74)?word(actor,0x54):different_from_zero(actor,0x68)?word(actor,0x50):word(actor,0x48);
    AIL_set_3D_sample_playback_rate(ptr(voice,4),static_cast<std::int32_t>(rate));
    U effect=0;
    if(word(system,4) && AIL_3D_room_type(ptr(system,0xf8))!=0 && AIL_3D_room_type(ptr(system,0xf8))!=-1)effect=load_store_float(system,0x68);
    AIL_set_3D_sample_effects_level(ptr(voice,4),effect);
    position[0]=word(actor,0x78);position[1]=word(actor,0x7c);position[2]=word(actor,0x80);
    U listener[3]={word(system,0x98),word(system,0x9c),word(system,0xa0)};
    U start=milliseconds(),spatial=0;
    if(sound_voice_required::spatial_query_005d6ef0(system,nullptr,position,listener,&start,&spatial)) {
        put(actor,0x8c,listener[0]);put(actor,0x90,listener[1]);put(actor,0x94,listener[2]);put(actor,0x98,start);put(actor,0x9c,spatial);
    } else {
        float value=sound_voice_required::spatial_value_0050ccf0(position[0],position[1],position[2],listener[0],listener[1],listener[2]);
        std::memcpy(static_cast<unsigned char*>(actor)+0x9c,&value,4);
        put(actor,0x8c,listener[0]);put(actor,0x90,listener[1]);put(actor,0x94,listener[2]);put(actor,0x98,start);
        const U count=word(system,0xc8)+1;put(system,0xc8,count);



        if(count>word(system,0xb8))put(system,0xb8,count);

    }
    sound_voice_required::actor_obstruction_005daeb0(actor,nullptr);



    const U elapsed=milliseconds()-start;

    put(system,0xc4,word(system,0xc4)+elapsed);
    AIL_resume_3D_sample(ptr(voice,4));
}
}
