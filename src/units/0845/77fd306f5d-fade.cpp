#include "943bb97e6a-fade.h"
#include <cstring>



namespace {
using U=std::uint32_t;
void* pointer(void* p,unsigned n){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
U milliseconds(){
    using namespace kotor_menu_10_19;
    auto timer=static_cast<TimerDependency*>(pointer(dependencies::base_007a39e0,4));
    return static_cast<U>(dependencies::unsigned_divide_006fb030(timer->high_resolution_005e9260(),1000));
}
}
namespace sound_fade29 {
void __fastcall Begin005d5b90(void* outer,void*,U duration){
    if(void* p=pointer(outer,0))sound_fade29_required::begin_internal_005dc480(p,nullptr,duration);
}
}
namespace table29_required {
void __fastcall gui_fade_005f1b20(menu_normal_40_49::MusicInternal*,void*,void* sound,U duration){






    const U start=milliseconds();



    sound_fade29::Begin005d5b90(sound,nullptr,duration);

    if(duration){
        U elapsed;
        do {



            sound_fade29_required::update_005d5dd0(table29_canonical_required::sound_007a39ec,nullptr);

            elapsed=milliseconds()-start;



        }while(elapsed<duration);

    }
}
}
