// Extracted verbatim from task-2/addkey-r3/addkey_bodies.cpp (sha256 f25163cc...): Build00410190 + Rebuild00410260 + their helpers only.
#include "15444fa152-addkey_bodies.h"
#include <cstring>
#include <new>

// Live-object domain with modern String unwind cleanup in Destroy.
// Historical exception runtime transport remains unclaimed. Byte view of the sole
// canonical key allocation, not a second nominal key or String owner.
namespace {
using W=menu_resources::Word;
static_assert(sizeof(void*)==4,"original x86 pointers");
unsigned char* at(void* p,W n){return static_cast<unsigned char*>(p)+n;}
W get(void* p,W n){W v;std::memcpy(&v,at(p,n),4);return v;}
unsigned short half(void* p,W n){unsigned short v;std::memcpy(&v,at(p,n),2);return v;}
void put(void* p,W n,W v){std::memcpy(at(p,n),&v,4);}
void* ptr(W v){return reinterpret_cast<void*>(v);}
W word(void* v){return reinterpret_cast<W>(v);}
void* field(void* p,W n){return ptr(get(p,n));}
menu_resources::String* name(void* p){return std::launder(reinterpret_cast<menu_resources::String*>(at(p,0x20)));}
void virtual0(void* p,W slot){
 using F=void(__thiscall*)(void*); F f;
 void* table=field(p,0);std::memcpy(&f,at(table,slot),4);f(p);
}
void query(void* entry,W* table,W* result){
 (void)unresolved_key_entry_00407230(rim_current_manager_007a39e8(),nullptr,
  reinterpret_cast<menu_resources::Ref*>(entry),half(entry,0x1a),table,result);
}
void flag_or_service(void* resource,W flags){
 if(half(resource,4)>0)put(resource,0x0c,get(resource,0x0c)|flags);
 else c2da_resource_serviced_00409b40(resource,nullptr,0);
}
}

namespace add_key_lower {
Word __fastcall Build00410190(void* self,void*,Word kind,String* input,Word id,Word last){
 if(input->BorrowedEquals005e5290(c2da_empty_0073d71c())!=0)return 0;
 name(self)->Assign005e5c50(input);
 if(kind-1u>3u)return 0;
 put(self,0x28,id);put(self,0x1c,kind);
 switch(kind){
 case 1:return add_key_body_ports::Build0040fb80(self,nullptr,last);
 case 2:return add_key_body_ports::Build0040f200(self,nullptr,last);
 case 3:return add_key_body_ports::Build0040f3c0(self,nullptr,last);
 default:return add_key_body_ports::Build0040f990(self,nullptr,last);
 }
}


void __fastcall Rebuild00410260(void* self,void*){
 // Original EBX-pushed frame: entry+10h, table+14h, index+18h.
 // Preserve adjacent query outputs and persistence over failed queries.
 struct Locals{W entry,table,index;} locals{};
 static_assert(sizeof(Locals)==12,"three original output words");
 W kind=get(self,0x1c);void* old=field(self,0x10);
 if(old==nullptr){
  if(kind==0)return;
  // Existing canonical zero-test support, no new 005E5390 body credit.
  if(K22_RawNotEqual(name(self),c2da_empty_0073d71c())==0)return;
  W id=get(self,0x28);kind=get(self,0x1c);
  (void)Build00410190(self,nullptr,kind,name(self),id,1);return;
 }
 W count=get(self,0x0c);
 if(kind==4 && field(self,0x30)!=nullptr)virtual0(field(field(self,0x30),0),0x0c);
 W id=get(self,0x28);kind=get(self,0x1c);
 put(self,0x0c,0);put(self,0x10,0);
 (void)Build00410190(self,nullptr,kind,name(self),id,0);
 put(self,4,1);
 for(W i=0;i<count;++i){
  void* entry=at(old,i*28u);
  if(field(entry,0x10)==nullptr)continue;
  if(add_key_body_ports::Find0040d490(self,nullptr,&locals.index,
       reinterpret_cast<menu_resources::Ref*>(entry),half(entry,0x1a))!=0){
   W offset=locals.index*28u;
   put(at(field(self,0x10),offset),0x10,get(entry,0x10));
   void* target=at(field(self,0x10),offset);
   put(field(target,0x10),8,get(target,0x14));
   put(entry,0x10,0);
   target=at(field(self,0x10),offset);
   put(field(target,0x10),0x14,word(target));
  }else{
   query(entry,&locals.table,&locals.entry);
   void* resource=field(entry,0x10);
   if(locals.entry==0){
    if(half(resource,4)>0)put(resource,0x0c,get(resource,0x0c)|0x200u);
    else{
     c2da_resource_serviced_00409b40(resource,nullptr,0);
     put(field(entry,0x10),0x14,0);
     put(field(entry,0x10),8,0xffffffffu);
    }
   }else{
    put(ptr(locals.entry),0x10,word(resource));
    put(field(ptr(locals.entry),0x10),8,get(ptr(locals.entry),0x14));
    put(entry,0x10,0);
    put(field(ptr(locals.entry),0x10),0x14,locals.entry);
    flag_or_service(field(ptr(locals.entry),0x10),0x100u);
   }
  }
 }
 live_free(old);
 for(W i=0;i<get(self,0x0c);++i){
  W offset=i*28u;void* entry=at(field(self,0x10),offset);
  if(field(entry,0x10)!=nullptr)continue;
  query(entry,&locals.table,&locals.entry);
  if(locals.entry==0)continue;
  void* resource=field(ptr(locals.entry),0x10);
  if(resource==nullptr || get(ptr(locals.table),0x28)>=get(self,0x28))continue;
  flag_or_service(resource,0x100u);
  put(at(field(self,0x10),offset),0x10,get(ptr(locals.entry),0x10));
  entry=at(field(self,0x10),offset);
  put(field(entry,0x10),8,get(entry,0x14));
  put(ptr(locals.entry),0x10,0);
  entry=at(field(self,0x10),offset);
  put(field(entry,0x10),0x14,word(entry));
 }
 put(self,4,0);
}
}
