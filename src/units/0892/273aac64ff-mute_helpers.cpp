#include "d3e359b3b1-mute_helpers.h"
#include "89b65663c9-startup_views.h"
#include <cstring>



namespace {
using U=std::uint32_t;
using Wide=kotor_menu_10_19::Wide;
U word(void* p,unsigned offset){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+offset,4);return v;}
void put(void* p,unsigned offset,U v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,4);}
void* pointer(void* p,unsigned offset){void* v;std::memcpy(&v,static_cast<unsigned char*>(p)+offset,4);return v;}
Wide clock_value() {
    void* base=kotor_menu_10_19::dependencies::base_007a39e0;
    auto timer=static_cast<kotor_menu_10_19::TimerDependency*>(pointer(base,4));
    return timer->high_resolution_005e9260();
}
U result(void* p,Wide stamp) {



    const Wide product=static_cast<Wide>(static_cast<U>(stamp))*0x10624dd3u;

    put(p,0x50,static_cast<U>(product>>38));



    return static_cast<U>(product);

}
}
namespace sound_mode29 {
std::uint32_t __fastcall Mute005d6a10(void* p,void*) {
    U flag=word(p,0x0c);
    if(flag!=0) {



        return flag;

    }



    put(p,0x5c,0);put(p,0x0c,1);put(p,0x54,500);

    const Wide stamp=clock_value();

    return result(p,stamp);
}
std::uint32_t __fastcall Unmute005d6a50(void* p,void*) {
    if(word(p,0x0c)==0) {



        return 0;

    }



    put(p,0x54,0);put(p,0x0c,0);put(p,0x5c,500);

    const Wide stamp=clock_value();

    return result(p,stamp);
}
}
