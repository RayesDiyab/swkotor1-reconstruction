#include "ac3f7f4ee9-e990.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using Ref=menu_resources::Ref;
W load(void*p,W n){W v;std::memcpy(&v,static_cast<unsigned char*>(p)+n,4);return v;}
void store(void*p,W n,W v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
void* entry(void*self,W offset){return reinterpret_cast<void*>(load(self,16)+offset);}
Ref* ref(void*p){return std::launder(reinterpret_cast<Ref*>(p));}
unsigned short type(void*p){unsigned short v;std::memcpy(&v,static_cast<unsigned char*>(p)+26,2);return v;}
void duplicate(Ref*value){
 // Reuse the already-owned CopyToString algorithm; no00405FB0 body added.
 static const char prefix[]="CExoKeyTable::AddKey: Duplicate Resource ";
 static const char suffix[]=" - Ignoring Duplicate";
 static_assert(sizeof(prefix)==42&&sizeof(suffix)==22,"authenticated literal spans");
 volatile char message[100];
 for(unsigned i=0;i<42;++i)message[i]=prefix[i];
 for(unsigned i=42;i<100;++i)message[i]=0;
 kotor_reconstruction::CResRef snapshot;std::memcpy(snapshot.bytes,value,16);
 char text[17];snapshot.CopyToString(text);
 unsigned at=41,i=0;do{message[at+i]=text[i];}while(text[i++]!=0);
 at+=i-1;for(i=0;i<22;++i)message[at+i]=suffix[i];
}
}
namespace e990_private {
void* __fastcall InsertCore(void*self,void*,Ref*value,W incoming_type,W id,W){
 for(;;){
  // Hash precedes the empty-Ref gate, including the zero-capacity fault case.
  W start=key_table_hash_0040d500(self,nullptr,value,incoming_type);
  W index=start;
  if(c2da_equal_text_004060d0(value,nullptr,""))return nullptr;
  bool restart=false;
  for(;;){
   W offset=index*28u;
   if(c2da_equal_text_004060d0(ref(entry(self,offset)),nullptr,""))break;
   if(resource_equal_004060b0(ref(entry(self,offset)),nullptr,value)&&type(entry(self,offset))==static_cast<unsigned short>(incoming_type))break;
   W counter=load(self,20)+1u;W capacity=load(self,12);++index;store(self,20,counter);
   if(index>=capacity)index=0;
   if(index==start){restart=true;break;}
  }
  if(restart)continue;
  W offset=index*28u;
  void* selected=entry(self,offset);
  //00406060 is the exact canonical inverse of the reused text comparator.
  if(load(selected,20)!=0xffffffffu&&!c2da_equal_text_004060d0(ref(selected),nullptr,"")){
   duplicate(ref(entry(self,offset)));return nullptr;
  }
  void* first=entry(self,offset);W table_id=load(self,40);store(first,20,id);
  void* current=entry(self,offset);
  W composed=(load(current,20)&0xfff03fffu)|((table_id&0x0fffffffu)<<14);
  store(current,20,composed);
  Ref discarded;(void)e990_ports::Assign00406120(ref(entry(self,offset)),nullptr,&discarded,value);
  current=entry(self,offset);unsigned short low=static_cast<unsigned short>(incoming_type);std::memcpy(static_cast<unsigned char*>(current)+26,&low,2);
  store(self,20,load(self,20)+1u);
  return entry(self,offset);
 }
}
}
namespace f200_ports {
// Existing callers consume void. Preserve that symbol while retaining the
// original physical pointer/zero EAX result and exact RET16 machine contract.
__declspec(naked) void __fastcall Insert0040e990(void*,void*,Ref*,Word,Word,Word){
 using e990_private::InsertCore;
 __asm { jmp InsertCore }
}
}

