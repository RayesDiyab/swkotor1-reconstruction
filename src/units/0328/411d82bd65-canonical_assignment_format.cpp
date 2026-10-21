#include "7fa9bc840c-warning_runtime.h"
#include "77e8b6221f-menu_targets.h"
#include "61ae7fa68b-canonical_formatter_owner.h"
#include "c46300269d-allocator_live.h"
namespace kotor_reconstruction {
using Word = kotor_menu20::Word;
using kotor_menu20::bindings;
// Existing task19 algorithms; only receiver/signature projection changes below.
static Word Length(const char* p){Word n=0;while(*p++){++n;}return n;}
StartupString* StartupString::Assign005e5c50(StartupString* rhs){

 if(this==rhs)return this;

 char* old=data;
 if(old){char* source=rhs->data;if(!source||Length(source)+1u>capacity){capacity=0;live_free(old);data=nullptr;}}
 char* source=rhs->data;
 if(source){Word n=Length(source);if(static_cast<std::int32_t>(n)>0){
  if(!data){capacity=n+1u;data=static_cast<char*>(live_operator_new(n+1u));}
  const char* input=rhs->data;char* output=data;char c;do{c=*input++;*output++=c;}while(c);return this;
 }}
 old=data;if(old){live_free(old);data=nullptr;capacity=0;}return this;
}
void __cdecl StartupFormat005e5680(StartupString* destination,const char* format,...){
 kotor_canonical_string_format::FormatterUse retained_use;
 const auto& formatter_bindings=retained_use.get();
 va_list args;va_start(args,format);char* buffer=*formatter_bindings.format_buffer_008328a8;
 if(!buffer&&*formatter_bindings.format_capacity_008328ac==0){*formatter_bindings.format_capacity_008328ac=1024;buffer=static_cast<char*>(formatter_bindings.allocate_006fa7e6(1024));*formatter_bindings.format_buffer_008328a8=buffer;}
 std::int32_t result=formatter_bindings.vsnprintf_006fd5a8(buffer,*formatter_bindings.format_capacity_008328ac,format,args);
 if(result<-1){va_end(args);return;}
 while(result<0){



  *formatter_bindings.format_capacity_008328ac+=1024;

  char* old=*formatter_bindings.format_buffer_008328a8;if(old)formatter_bindings.free_006fa390(old);
  buffer=static_cast<char*>(formatter_bindings.allocate_006fa7e6(*formatter_bindings.format_capacity_008328ac));*formatter_bindings.format_buffer_008328a8=buffer;
  result=formatter_bindings.vsnprintf_006fd5a8(buffer,*formatter_bindings.format_capacity_008328ac,format,args);
 }
 Word size=static_cast<Word>(result)+1u;char* old=destination->data;
 if(old&&size>destination->capacity){destination->capacity=0;formatter_bindings.free_006fa390(old);destination->data=nullptr;}
 if(!destination->data){destination->capacity=size;destination->data=static_cast<char*>(formatter_bindings.allocate_006fa7e6(size));}
 formatter_bindings.strncpy_006fab80(destination->data,*formatter_bindings.format_buffer_008328a8,static_cast<Word>(result));destination->data[result]=0;va_end(args);
}
// Transport-only carrier: the actual member above remains the sole algorithm.
StartupString* __fastcall StartupAssign005e5c50(StartupString* self, void*, StartupString* rhs) {
    return self->Assign005e5c50(rhs);
}
} // namespace kotor_reconstruction
