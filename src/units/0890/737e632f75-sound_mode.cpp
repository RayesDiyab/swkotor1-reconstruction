#include "9e9fec4007-sound_mode.h"
#include <cstring>



namespace {
using U=std::uint32_t;
U word(void* p,unsigned offset){U v;std::memcpy(&v,static_cast<unsigned char*>(p)+offset,4);return v;}
void put(void* p,unsigned offset,U v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,4);}
void* pointer(void* p){void* v;std::memcpy(&v,p,4);return v;}
U top(void* p,U depth){return word(p,0x118u+depth*4u);}
void set_mode(void* p,U mode) {
    using namespace sound_mode29;
    using namespace sound_mode29_required;
    for(;;) {
        if(mode==*word_0074c600) {




            PauseAll005d82c0(p,nullptr,1);
            put(p,0x144,1);

            return;
        }
        if(mode==*word_0074c604) {
            put(p,0x144,0);
            ResumeAll005d83f0(p,nullptr);
            U depth=word(p,0x140);
            mode=top(p,depth); // Original reads this before testing depth zero.
            if(depth==0)return;
            put(p,0x140,depth-1u);
            continue;
        }



        if(mode!=*word_0074c5ec && mode==top(p,word(p,0x140)))return;

        U depth=word(p,0x140);
        if(top(p,depth)==*word_0074c5f0)Unmute005d6a50(p,nullptr);

        depth=word(p,0x140);

        U old=top(p,depth);
        if(old==*word_0074c5f4 || old==*word_0074c5f8 || old==*word_0074c5fc)ResumeAll005d83f0(p,nullptr);
        if(mode==*word_0074c5ec) {
            depth=word(p,0x140)-1u;
            put(p,0x140,depth);
            if(depth&0x80000000u)put(p,0x140,0); // x86 jns after decrement.
            mode=top(p,word(p,0x140));
        } else {
            depth=word(p,0x140)+1u;
            put(p,0x140,depth);
            if((depth&0x80000000u)==0 && depth>9u)put(p,0x140,9); // Signed jle.



            put(p,0x118u+word(p,0x140)*4u,mode);

        }
        if(mode==*word_0074c5f0)Mute005d6a10(p,nullptr);
        if(mode==*word_0074c5f4 || mode==*word_0074c5f8 || mode==*word_0074c5fc)PauseAll005d82c0(p,nullptr,0);
        return;
    }
}
}
namespace table29_required {
void __fastcall gui_sound_mode_005d5e80(void* outer,void*,std::uint32_t mode) {
    void* internal=pointer(outer);
    if(internal)set_mode(internal,mode);
}
}
