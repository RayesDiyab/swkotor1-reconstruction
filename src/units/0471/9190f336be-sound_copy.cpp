#include "1420d47ecc-fetch_value_providers.h"
#include <cstring>
extern "C" tlk_values::Ref* __fastcall SoundAssign00406170(tlk_values::Ref* self,void*,tlk_values::Ref* out,const tlk_values::Ref* input) {
 // Four ordered DWORD transfers preserve original self/input alias effects.
 for(unsigned off=0;off<16;off+=4){std::uint32_t word;std::memcpy(&word,input->bytes+off,4);std::memcpy(self->bytes+off,&word,4);}
 unsigned count=0;
 while(count<16){
  const unsigned char byte=self->bytes[count];
  if(!byte){for(unsigned off=count+1;off<16;++off)self->bytes[off]=0;break;}



  const int character=byte;

  self->bytes[count]=static_cast<unsigned char>(gui_reference::reference_providers().lowercase_006fb766(character));++count;
 }
 for(unsigned off=0;off<16;off+=4){std::uint32_t word;std::memcpy(&word,self->bytes+off,4);std::memcpy(out->bytes+off,&word,4);}
 return out;
}
