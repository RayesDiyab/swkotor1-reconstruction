#include "d46fd7a8ce-voice_start.h"
#include <cstring>



namespace {
using U=std::uint32_t;
void* ptr(void* p,unsigned n){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
U word(void* p,unsigned n){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void setptr(void* p,unsigned n,void* v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}

U different_from_zero(void* p,unsigned n) {
    // Exact original x87 relation, including unordered and masked exceptions.
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

}
namespace sound_lower29_required {
void __fastcall start2d_005d7480(void* system,void*,void* actor,void* voice) {

    const U looping=word(actor,0x24)==0;

    setptr(actor,0x38,voice);setptr(voice,0,actor);
    AIL_init_sample(ptr(voice,4));
    void* resource=ptr(actor,8);
    // Vtable slot +0x14 consumes only ECX and returns one pointer in EAX.
    // The push 0 before this call belongs to AIL_set_sample_file's third arg.
    using Data=void*(__fastcall*)(void*,void*);
    auto data=reinterpret_cast<Data>(ptr(ptr(resource,0),0x14))(resource,nullptr);
    AIL_set_sample_file(ptr(voice,4),data,0);



    AIL_set_sample_loop_count(ptr(voice,4),looping);

    AIL_set_sample_position(ptr(voice,4),word(actor,0x88));
    U rate;



    if(different_from_zero(actor,0x74))rate=word(actor,0x54);
    else if(different_from_zero(actor,0x68))rate=word(actor,0x50);
    else rate=word(actor,0x48);

    AIL_set_sample_playback_rate(ptr(voice,4),static_cast<std::int32_t>(rate));



    const U scale=load_store_float(system,0x14);

    const U volume=static_cast<unsigned char*>(actor)[0x85];
    sound_voice_required::actor_volume_005db800(actor,nullptr,volume,0,scale);
    AIL_resume_sample(ptr(voice,4));
}
}
