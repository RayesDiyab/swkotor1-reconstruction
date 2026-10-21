#include "897ca13cc0-internal.h"
namespace file_internal {
extern "C" Word __fastcall exofile_read_005e6960(File* self,void*,void* out,Word size,Word count){
 Word bytes=internal_read_005eba40(self->internal,nullptr,out,size,count);



 return bytes;

}
extern "C" Word __fastcall exofile_seek_beginning_005e6ad0(File* self,void*){
 auto* p=self->internal;
 if(!p->stream)return 0;
 const int result=file_ops->seek(p->stream,0,0);



 return result==0;

}
extern "C" void __fastcall internal_destroy_005eb900(Internal* self,void*){
 if(self->stream){file_ops->close(self->stream);self->stream=nullptr;}



 self->mode.~String();self->name.~String();

}
extern "C" Word __fastcall internal_read_005eba40(Internal* self,void*,void* out,Word size,Word count){
 if(!self->stream)return 0;
 const long start=file_ops->tell(self->stream);Word bytes=file_ops->read(out,size,count,self->stream)*size;const Word requested=size*count;
 Word retries=0;bool retried=false;auto* log=engine->retry_log_008328d8;auto* target=log;
 while(bytes!=requested&&file_ops->error(self->stream)&&retries<10){
  retried=true;engine->format_005e5680(target,engine->retry_format,engine->text_005e5670(&self->name,nullptr),engine->text_005e5670(log,nullptr),retries,requested,bytes);
  file_ops->delay(500);
  if(self->stream)file_ops->close(self->stream);
  else engine->format_005e5680(target,engine->null_format,engine->text_005e5670(log,nullptr),retries);
  self->stream=file_ops->open(engine->text_005e5670(&self->name,nullptr),engine->text_005e5670(&self->mode,nullptr));
  if(self->stream){file_ops->seek(self->stream,start,0);bytes=file_ops->read(out,size,count,self->stream)*size;}
  else engine->format_005e5680(target,engine->reopen_format,engine->text_005e5670(log,nullptr),retries);
  ++retries;
 }
 if(bytes!=requested)engine->format_005e5680(target,engine->short_format,engine->text_005e5670(log,nullptr));
 else if(retried)engine->format_005e5680(target,engine->success_format,engine->text_005e5670(log,nullptr));
 return bytes;
}
}
