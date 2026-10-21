#include "6add3cafee-size_seek.h"
#include <cstring>
namespace file_internal {
extern "C" Word __fastcall exofile_size_005e6950(File* self,void*) {
 return internal_size_005eb990(self->internal,nullptr);
}
extern "C" Word __fastcall internal_size_005eb990(Internal* self,void*) {
 if(self->stream){
  file_ops->flush(self->stream);
  const long saved=file_ops->tell(self->stream);
  if(saved>=0){
   file_ops->seek(self->stream,0,2);
   const long size=file_ops->tell(self->stream);

   file_ops->seek(self->stream,saved,0);

   return static_cast<Word>(size);
  }
 }



 return 0xffffffffU;

}
extern "C" Word __fastcall exofile_seek_005e6aa0(File* self,void*,Word offset_bits,Word origin_bits) {
 auto* inner=self->internal;
 if(!inner->stream)return 0;
 static_assert(sizeof(long)==sizeof(Word)&&sizeof(int)==sizeof(Word),"32-bit CRT arguments");
 long offset;int origin;
 std::memcpy(&offset,&offset_bits,sizeof offset);std::memcpy(&origin,&origin_bits,sizeof origin);
 const int result=file_ops->seek(inner->stream,offset,origin);



 return result==0;

}
}
