#include "48d4671e3a-f3c0.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;using Ref=menu_resources::Ref;
W load(const void*p,W n){W v;std::memcpy(&v,static_cast<const unsigned char*>(p)+n,4);return v;}
void store(void*p,W n,W v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
W word(const void*p){return reinterpret_cast<W>(p);}void* ptr(W v){return reinterpret_cast<void*>(v);}
void* field(void*p,W n){return ptr(load(p,n));}
S* name(void*p,W n){return std::launder(reinterpret_cast<S*>(static_cast<unsigned char*>(p)+n));}
void close(void*p){f3c0_ports::Destroy005e69e0(p,nullptr);live_free(p);}
void* attempt(void*self,W kind){
 void* raw=live_operator_new(4);if(!raw)return nullptr;
 alignas(S) unsigned char storage[sizeof(S)];S* mode;
 try{mode=::new(storage)S("rb");}catch(...){live_free(raw);throw;}
 void* file;
 try{file=f3c0_ports::Construct005e68d0(raw,nullptr,name(self,0x20),kind,mode);}
 catch(...){mode->~StartupString();live_free(raw);throw;}
 // Original returns to state1 before this normal temporary destruction.
 mode->~StartupString();return file;
}
W slots(W count){
 static const double scale=1.2,unsigned_bias=4294967296.0;W result;
 __asm {
  fild dword ptr count
  test count,80000000h
  jz nonnegative
  fadd qword ptr unsigned_bias
 nonnegative:
  fmul qword ptr scale
  call genuine_x87_ftol_006fae8c
  mov result,eax
 }
 return result;
}
struct Record{Ref name;W id,type;};
static_assert(sizeof(Record)==24&&offsetof(Record,id)==16&&offsetof(Record,type)==20,"private record24");
}
namespace add_key_body_ports {
Word __fastcall Build0040f3c0(void*self,void*,Word last){
 S first,second;Ref reference{};
 const W kinds[]={0x80e,0x7db,0x809,0x270d,0x80d};
 const W categories[]={4,0,1,2,3};
 void* file=nullptr;W category=0,saved=0;
 for(W i=0;i<5;++i){
  file=attempt(self,kinds[i]);if(i<4)saved=word(file);
  if(!file){store(self,0,0);return 0;}
  if(f3c0_ports::Opened005e6a10(file,nullptr)){category=categories[i];break;}
  close(file);if(i==4){store(self,0,0);return 0;}
 }
 saved=(saved&0xffffff00u)|category;
 unsigned char header[160];
 (void)f3c0_ports::Read005e6960(file,nullptr,header,160,1);
 W magic=category==2?0x20465245u:category==3?0x204b4148u:0x20444f4du;
 if(load(header,0)!=magic||load(header,4)!=0x302e3156u||load(header,0x10)==0){store(self,0,0);close(file);return 0;}
 W capacity=slots(load(header,0x10));store(self,0x0c,capacity);
 void* entries=f200_ports::Allocate0040eb80(self,nullptr,capacity);store(self,0x10,word(entries));
 if(!entries){store(self,0,0);close(file);return 0;}
 (void)f3c0_ports::Seek005e6aa0(file,nullptr,load(header,0x18),0);
 Record record;
 for(W index=0;index<load(header,0x10);++index){
  (void)f3c0_ports::Read005e6960(file,nullptr,&record,24,1);
  Ref discarded;(void)f990_ports::RefCopy00406170(&reference,nullptr,&discarded,&record.name);
  W id=(((load(self,0x28)&0x3ffu)|0xfffff800u)<<20)|(record.id&0xfffffu);record.id=id;
  f200_ports::Insert0040e990(self,nullptr,&reference,record.type,id,last);
 }
 close(file);store(self,0x2c,1);
 void* array=live_operator_new(4);store(self,0x30,word(array));if(!array)return 0;
 void* object=live_operator_new(68);
 if(object){void* raw=object;try{object=f3c0_ports::Archive0040ef90(raw,nullptr);}catch(...){live_free(raw);throw;}}
 store(field(self,0x30),0,word(object));object=field(field(self,0x30),0);
 if(!object){live_free(field(self,0x30));store(self,0x30,0);return 0;}
 name(object,4)->Assign005e5c50(name(self,0x20));object=field(field(self,0x30),0);
 using Call=void(__thiscall*)(void*,W);Call call;std::memcpy(&call,static_cast<unsigned char*>(field(object,0))+0x3c,4);call(object,saved);
 return 1;
}
}
