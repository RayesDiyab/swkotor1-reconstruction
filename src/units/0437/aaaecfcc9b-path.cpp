#include "968af5075b-path.h"
#include "722469ff1f-native_strings.h"
#include <new>
namespace path_resolution {
namespace {
Base* actual_base(){return static_cast<Base*>(*bindings->base_007a39e0);}
alias_path::AliasListView* actual_aliases(){return static_cast<alias_path::AliasListView*>(actual_base()->aliases);}
String* actual_extension(std::uint16_t type){return c2da_base_005e6660(actual_base(),nullptr,type);}
}

extern "C" std::int32_t __fastcall find_005e5f20(String* self,void*,Word character,std::int32_t start){
 const char* text=self->data;if(!text||start<0)return -1;
 for(std::int32_t i=0;i<start;++i){if(!*text)return -1;++text;}
 while(*text){if(static_cast<unsigned char>(*text)==static_cast<unsigned char>(character))return start;++text;++start;}
 return -1;
}
String* __fastcall resolve_005e68a0(alias_path::AliasListView* self,void*,String* out,String* name,Word type){
 auto* captured=*reinterpret_cast<alias_path::InternalView* volatile*>(self);
 return resolve_internal_005eb6b0(captured,nullptr,out,name,type);
}
String* __fastcall resolve_internal_005eb6b0(alias_path::InternalView*,void*,String* out,String* name,Word type){
 String temporary;const std::int32_t colon=find_005e5f20(name,nullptr,':',0);
 if(colon<=0)return ::new(out) String(bindings->empty_0073d71c);
 String* alias=actual_aliases()->GetAliasPath(name,colon);
 if(alias->BorrowedLength005e5790()==0)return ::new(out) String(bindings->empty_0073d71c);
 std::int32_t skip=colon;

 if(WindowCStr005e5670(name,nullptr)[skip+1]=='\\')++skip;

 String* extension=actual_extension(static_cast<std::uint16_t>(type));
 if(extension->BorrowedLength005e5790()!=0){
  const char* ext=WindowCStr005e5670(actual_extension(static_cast<std::uint16_t>(type)),nullptr);
  const char* tail=WindowCStr005e5670(name,nullptr)+skip+1;
  const char* prefix=WindowCStr005e5670(actual_aliases()->GetAliasPath(name,colon),nullptr);



  bindings->format_005e5680(&temporary,bindings->with_extension_0074e470,prefix,tail,ext);

 }else{
  const char* tail=WindowCStr005e5670(name,nullptr)+skip+1;
  const char* prefix=WindowCStr005e5670(actual_aliases()->GetAliasPath(name,colon),nullptr);
  bindings->format_005e5680(&temporary,bindings->without_extension_00740ccc,prefix,tail);
 }
 return ::new(out) String(temporary); // Existing M39 C++ copy lifetime owner; no preclear.
}
}
