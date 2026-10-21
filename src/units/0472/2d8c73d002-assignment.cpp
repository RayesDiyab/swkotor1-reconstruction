#include "896b5ec2dc-assignment.h"
#include <cstring>
namespace resource_ref_assignment {
inline U32 WordAt(const void* p,unsigned n){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+n,4);return v;}
inline void StoreWord(void* p,unsigned n,U32 v){std::memcpy(static_cast<U8*>(p)+n,&v,4);}
Ref* __fastcall assign_00406120(Ref* self,void*,Ref* out,const Ref* rhs) {
 for(unsigned i=0;i<16;++i) {
  const U8 value=rhs->bytes[i];
  self->bytes[i]=static_cast<U8>(bindings.lower_006fb766(static_cast<unsigned>(value)));
 }
 for(unsigned offset=0;offset<16;offset+=4) {const U32 word=WordAt(self,offset);StoreWord(out,offset,word);}
 return out;
}
}
