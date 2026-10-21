#include "cd5676f4ba-f990.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;using Ref=menu_resources::Ref;
W load(void* p,W n){W v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void store(void* p,W n,W v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
void* ptr(W v){return reinterpret_cast<void*>(v);}W word(const void*p){return reinterpret_cast<W>(p);}
void* field(void*p,W n){return ptr(load(p,n));}
S* name(void*p,W n){return std::launder(reinterpret_cast<S*>(static_cast<unsigned char*>(p)+n));}
W open(void* p){using F=W(__thiscall*)(void*);F f;std::memcpy(&f,static_cast<unsigned char*>(field(p,0))+0x2c,4);return f(p);}
void erase(void* p){using F=void(__thiscall*)(void*,W);F f;std::memcpy(&f,field(p,0),4);f(p,1);}
void discard(void* self){
 store(self,0,0);
 void* object=field(field(self,0x30),0);
 if(object)erase(object);
 live_free(field(self,0x30)); // deleting callback may replace the array
 store(self,0x30,0);
}
W slots(W count){
 // Verified immutable binary64 literals0073E288/0073E290; no data-address credit.
 static const double scale=1.2,unsigned_bias=4294967296.0;
 W result;
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
struct Record {Ref name;W type,id,offset,length;};
static_assert(sizeof(Record)==32 && offsetof(Record,type)==16 && offsetof(Record,id)==20,"private copied32-byte record, canonical Ref first");
}
namespace add_key_body_ports {
Word __fastcall Build0040f990(void* self,void*,Word last){
 Ref reference{};
 store(self,0x2c,1);
 void* array=live_operator_new(4);
 store(self,0x30,word(array));
 if(!array)return 0;
 void* object=live_operator_new(0x34);
 if(object){
  // The original construction unwind only frees this raw52-byte allocation.
  // Begin only the actual String subobject lifetime, never an invented class.
  try{
   store(object,0,word(f990_ports::InitialTable0073e128()));
   ::new(static_cast<unsigned char*>(object)+4)S;
   store(object,0x2c,0);
   store(object,0,word(f990_ports::FinalTable0073e240()));
   store(object,0x14,0);
   const unsigned short zero=0;std::memcpy(static_cast<unsigned char*>(object)+0x0c,&zero,2);
   store(object,0x10,0);store(object,0x1c,0);store(object,0x24,0);store(object,0x30,0);
  }catch(...){live_free(object);throw;}
 }
 store(field(self,0x30),0,word(object));
 object=field(field(self,0x30),0);
 if(!object){live_free(field(self,0x30));store(self,0x30,0);return 0;}
 name(object,4)->Assign005e5c50(name(self,0x20));
 object=field(field(self,0x30),0);
 if(open(object)==0)return 0; // deliberately retain published object/array
 void* image=field(field(field(self,0x30),0),0x30);
 W count=load(image,0x0c);
 if(count==0){discard(self);return 0;}
 W capacity=slots(count);
 store(self,0x0c,capacity);
 void* entries=f200_ports::Allocate0040eb80(self,nullptr,capacity);
 store(self,0x10,word(entries));
 if(!entries){discard(self);return 0;}
 // Loop bound reloads the original image; each record comes from current image.
 for(W index=0;index<load(image,0x0c);++index){
  void* current=field(field(field(self,0x30),0),0x30);
  void* source=ptr(word(current)+load(current,0x10)+index*32u);
  Record record;std::memcpy(&record,source,32);
  Ref discarded;
  (void)f990_ports::RefCopy00406170(&reference,nullptr,&discarded,&record.name);
  W id=(((load(self,0x28)&0x3ffu)|0x400u)<<20)|(record.id&0xfffffu);
  record.id=id;
  f200_ports::Insert0040e990(self,nullptr,&reference,record.type,id,last);
 }
 return 1;
}
}
