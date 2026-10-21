#include "30b63a61bb-files.h"
namespace tlk_file {
extern "C" Word __fastcall file_opened_005e6a10(File* self,void*){



 return self->internal->stream!=nullptr;

}
extern "C" void __fastcall file_destroy_005e69e0(File* self,void*){
 file_internal::Internal* captured=self->internal;
 if(captured){



  bindings->destroy_internal_005eb900(captured,nullptr);
  bindings->free_006fa390(captured);
  self->internal=nullptr;
 }
}
}
