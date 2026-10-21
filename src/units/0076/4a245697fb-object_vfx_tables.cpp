// Original named-row / numeric-column C2DA getters. Reuses canonical string,
// row-index and parsing providers; no new table representation or ownership.
#include "1d00acc516-startup_string.h"
#include "aa9a4a1b31-menu_targets.h"
#include <cstdint>
#include <cstring>
#include <new>
using U=std::uint32_t;using I=std::int32_t;using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace options_video {struct String;}
namespace client_movies {I __fastcall C2daGetRowIndex_004138b0(void*,void*,options_video::String*);}
namespace directory_list {char __fastcall At005e5600(String*,void*,I);}
namespace kotor_sound_path {float __fastcall StringAsFloat005e5650(String*,void*);}
extern "C" {
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
char* __fastcall WindowCStr005e5670(String*,void*);
int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
String* cell(void*self,U row,U col){return std::launder(reinterpret_cast<String*>(static_cast<B*>(rd<void*>(ptr(self,0x38),row*4))+col*8));}
String* fallback(void*self){return reinterpret_cast<String*>(static_cast<B*>(self)+0x1c);}
I as_int(String*s){return std::launder(reinterpret_cast<kotor_menu20::String*>(s))->as_int_005e5630();}
void as_float_store(String*s,float*out){
 auto f=&kotor_sound_path::StringAsFloat005e5650;
 __asm {
  mov ecx,s
  xor edx,edx
  call f
  mov eax,out
  fstp dword ptr [eax]
 }
}
I row_index(void*self,String*row){return invoke(client_movies::C2daGetRowIndex_004138b0,self,nullptr,reinterpret_cast<options_video::String*>(row));}
bool invalid(void*self,I row,I col){return row==-1||col<0||col>=rd<I>(self,0x28);}
U packed_offset(void*self,U row,U col){return rd<std::uint16_t>(ptr(self,0x44),(rd<U>(self,0x28)*row+col)*2);}
const char* packed(void*self,U off){return static_cast<const char*>(ptr(self,0x40))+off;}
}
extern "C" {
I __fastcall object_vfx_table_00414260(void*self,void*,String*label,I column,I*out){
 const I row=row_index(self,label);
 if(invalid(self,row,column)){*out=as_int(fallback(self));return 0;}
 const U r=static_cast<U>(row),c=static_cast<U>(column);
 if(rd<U>(self,0x3c)){
  U offset=packed_offset(self,r,c);*out=0;
  invoke(kotor_sscanf_006fae08,packed(self,offset),"%i",out);
  return packed(self,offset)[0]!=0?1:0; // reload +40 after scanner
 }
 if(cell(self,r,c)->BorrowedEquals005e5290("")){*out=0;return 0;}
 if(cell(self,r,c)->BorrowedLength005e5790()>2&&
    invoke(WindowCStr005e5670,cell(self,r,c),nullptr)[0]=='0'&&
    (invoke(WindowCStr005e5670,cell(self,r,c),nullptr)[1]=='x'||
     invoke(WindowCStr005e5670,cell(self,r,c),nullptr)[1]=='X')){
  // Original ignores conversion status and leaves the output untouched on
  // failed hex scanning; there is intentionally no initialization here.
  invoke(kotor_sscanf_006fae08,invoke(WindowCStr005e5670,cell(self,r,c),nullptr),"%x",out);return 1;
 }
 *out=as_int(cell(self,r,c));return 1;
}
I __fastcall object_vfx_table_00414080(void*self,void*,String*label,I column,float*out){
 const I row=row_index(self,label);
 if(invalid(self,row,column)){as_float_store(fallback(self),out);return 0;}
 const U r=static_cast<U>(row),c=static_cast<U>(column);
 if(rd<U>(self,0x3c)){
  U offset=packed_offset(self,r,c);U zero=0;std::memcpy(out,&zero,4);
  invoke(kotor_sscanf_006fae08,packed(self,offset),"%f",out);return 1;
 }
 as_float_store(cell(self,r,c),out);return 1;
}
I __fastcall object_vfx_table_00413ec0(void*self,void*,String*label,I column,String*out){
 const I row=row_index(self,label);
 if(invalid(self,row,column)){out->Assign005e5c50(fallback(self));return 0;}
 const U r=static_cast<U>(row),c=static_cast<U>(column);
 if(rd<U>(self,0x3c)){
  U offset=packed_offset(self,r,c);String*assigned=invoke(GuiAssign005e5140,out,nullptr,packed(self,offset));
  return invoke(directory_list::At005e5600,assigned,nullptr,0)!=0?1:0;
 }
 if(cell(self,r,c)->BorrowedEquals005e5290("")){invoke(GuiAssign005e5140,out,nullptr,"");return 0;}
 out->Assign005e5c50(cell(self,r,c));return out->BorrowedLength005e5790()>0?1:0;
}
}
