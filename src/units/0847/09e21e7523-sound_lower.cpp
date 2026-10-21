#include "590b90d687-sound_lower.h"
#include <cstring>



namespace {
using U=std::uint32_t;
void* ptr(void* p,unsigned n){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
U word(void* p,unsigned n){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void put(void* p,unsigned n,U v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
U byte(void* p,unsigned n){return static_cast<unsigned char*>(p)[n];}
void* voice(void* p,unsigned n){void* v=ptr(p,n);return v && ptr(v,4)?v:nullptr;}
}
namespace sound_lower29 {
U __fastcall ActorPlaying005db0e0(void* p,void*) {
    if(!word(ptr(p,0x44),8))return 0;

    if(void* v=voice(p,0x3c))return AIL_3D_sample_status(ptr(v,4))==4;

    if(void* v=voice(p,0x38))return AIL_sample_status(ptr(v,4))==4;
    return 0;
}
}
namespace sound_pause29_required {
// These three existing R4 transport returns are ignored by all admitted
// callers. Zero is a carrier convenience, not an original EAX-value claim.
U __fastcall stream_resume_005dcdb0(void* p,void*) {
    if(!word(ptr(p,0x20),8))return 0;
    void* handle=ptr(p,0);if(!handle)return 0;
    U status=AIL_stream_status(handle);



    if(status==4)return 0;

    sound_lower29_required::volume_005dc930(p,nullptr,byte(p,0x46),0,0x3f800000u);



    AIL_set_stream_loop_count(ptr(p,0),word(p,4)==0?1:0);

    AIL_set_stream_position(ptr(p,0),static_cast<std::int32_t>(word(p,0x40)));
    AIL_pause_stream(ptr(p,0),0);
    put(p,0x40,0);return 0;
}
U __fastcall actor_pause_005db150(void* p,void*) {
    if(!word(ptr(p,0x44),8) || !sound_lower29::ActorPlaying005db0e0(p,nullptr))return 0;
    if(void* v=voice(p,0x3c)) {
        put(p,0x88,AIL_3D_sample_offset(ptr(v,4)));
        AIL_stop_3D_sample(ptr(ptr(p,0x3c),4));return 0;
    }
    if(void* v=voice(p,0x38)) {
        put(p,0x88,AIL_sample_position(ptr(v,4)));
        AIL_stop_sample(ptr(ptr(p,0x38),4));
    }
    return 0;
}
U __fastcall actor_resume_005db1c0(void* p,void*) {
    void* system=ptr(p,0x44);if(!word(system,8))return 0;



    if(void* v=voice(p,0x3c))sound_lower29_required::start3d_005d71c0(system,nullptr,p,v);
    else if(void* v2=voice(p,0x38))sound_lower29_required::start2d_005d7480(system,nullptr,p,v2);

    put(p,0x88,0);

    return 0;
}
}
namespace table29_required {
void __fastcall gui_resume_005d5b30(void* outer,void*) {
    if(void* p=ptr(outer,0))sound_pause29_required::stream_resume_005dcdb0(p,nullptr);
}
}
