#include "f4f2f2f883-reference.h"
#include <cstring>
extern "C" menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref* self,void*,menu_resources::Ref* out,const char* input){
    const std::uint32_t zero=0;
    for(unsigned off=0;off<16;off+=4)std::memcpy(self->bytes+off,&zero,4);
    if(input){
        unsigned count=0;
        while(count<16){
            const auto value=static_cast<signed char>(static_cast<unsigned char>(input[count]));
            if(!value)break;
            const int lowered=gui_reference::reference_providers().lowercase_006fb766(static_cast<int>(value));
            self->bytes[count]=static_cast<unsigned char>(lowered);++count;
        }
    }
    for(unsigned off=0;off<16;off+=4){std::uint32_t word;std::memcpy(&word,self->bytes+off,4);std::memcpy(out->bytes+off,&word,4);}



    return out;

}
