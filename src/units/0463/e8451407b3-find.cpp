#include "15444fa152-addkey_bodies.h"
#include "873954807b-providers.h"
#include <cstring>
namespace {
using W=menu_resources::Word;
W load(void* self,unsigned offset){W result;std::memcpy(&result,static_cast<unsigned char*>(self)+offset,4);return result;}
unsigned char* entries(void* self){return reinterpret_cast<unsigned char*>(load(self,0x10));}
}
namespace add_key_body_ports {
// Sole new body: complete0040D490..0040D4F3. Existing Ref equality is reused.
// Opaque receiver is the canonical key allocation; entries contain live Refs.
Word __fastcall Find0040d490(void* self,void*,Word* output,menu_resources::Ref* resource,Word type){
 if(entries(self)==nullptr)return 0;
 for(Word index=0;index<load(self,0x0c);++index){
  unsigned char* entry=entries(self)+index*28u;
  unsigned short entry_type;std::memcpy(&entry_type,entry+0x1a,2);
  if(entry_type==static_cast<unsigned short>(type) &&
     resource_equal_004060b0(reinterpret_cast<menu_resources::Ref*>(entry),nullptr,resource)!=0){
   *output=index;
   return 1;
  }
 }
 return 0;
}
}
