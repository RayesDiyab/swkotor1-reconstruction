// CSWSMessage::SendServerToPlayerParty_List 00567F90..0056802C, RET14h.
// Reuses the selected W2 message writers, finalizer and send provider.
#include <cstdint>
#include <cstring>
extern "C" {
void __fastcall kotor_fastcall_004d68a0(void*,void*,unsigned,unsigned,unsigned);
void __fastcall kotor_fastcall_004d6340(void*,void*,unsigned,unsigned);
void __fastcall kotor_fastcall_00578570(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_004d6440(void*,void*,void**,unsigned*);
unsigned __fastcall kotor_fastcall_00567e60(void*,void*,unsigned,unsigned,unsigned,void*,unsigned);
}
namespace {
using U=std::uint32_t;
static_assert(sizeof(void*)==4,"original x86 pointer width");
U read_entry(const void* list,U index){U value;std::memcpy(&value,static_cast<const unsigned char*>(list)+index*4,4);return value;}
}
extern "C" unsigned __fastcall recovered_party_list_00567f90(void* self,void*,unsigned player,unsigned count,const void* list,unsigned subtype,unsigned excluded){
 U filtered=0;
 if(static_cast<std::int32_t>(count)>0){
  // 00567FA6..FAA: [list+i*4] read unchecked; a null list with count>0 faults as in the original.
  for(U i=0;i<count;++i)if(read_entry(list,i)!=excluded)++filtered;
 }
 kotor_fastcall_004d68a0(self,nullptr,0x80,0xffffffff,1);
 kotor_fastcall_004d6340(self,nullptr,filtered,0x20);
 if(static_cast<std::int32_t>(count)>0){
  for(U i=0;i<count;++i){U id=read_entry(list,i);if(id!=excluded)kotor_fastcall_00578570(self,nullptr,id);}
 }
 void* data;unsigned size;
 if(!kotor_fastcall_004d6440(self,nullptr,&data,&size))return 0;
 return kotor_fastcall_00567e60(self,nullptr,player,0x0e,subtype,data,size);
}
