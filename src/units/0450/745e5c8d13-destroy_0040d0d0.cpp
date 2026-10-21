// Extracted verbatim from task-2/addkey-r3/addkey_bodies.cpp (sha256 f25163cc...): Destroy0040d0d0 + its helpers only.
#include "8df1242c60-add_key.h"
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
void put(void* p,W n,W v){std::memcpy(at(p,n),&v,4);}
void* ptr(W v){return reinterpret_cast<void*>(v);}
void* field(void* p,W n){return ptr(get(p,n));}
menu_resources::String* name(void* p){return std::launder(reinterpret_cast<menu_resources::String*>(at(p,0x20)));}
void virtual0(void* p,W slot){
 using F=void(__thiscall*)(void*); F f;
 void* table=field(p,0);std::memcpy(&f,at(table,slot),4);f(p);
}
void deleting(void* p){
 using F=void(__thiscall*)(void*,W);F f;
 void* table=field(p,0);std::memcpy(&f,table,4);f(p,1);
}
}

namespace add_key_lower {
void __fastcall Destroy0040d0d0(void* self,void*){
 // Original state0 map760A2C calls funclet714890 -> String destructor5E5C20.
 // Normal destruction also occurs once after the final zero stores.
 struct Cleanup {String* value;~Cleanup(){value->~StartupString();}} cleanup{name(self)};
 if(field(self,0x30)!=nullptr){
  for(W i=0;i<get(self,0x2c);++i){
   void* object=field(field(self,0x30),i*4);
   if(object!=nullptr){
    virtual0(object,0x40);
    object=field(field(self,0x30),i*4);
    if(object!=nullptr)deleting(object);
   }
  }
  live_free(field(self,0x30));
 }
 if(field(self,0x10)!=nullptr)live_free(field(self,0x10));
 put(self,4,1);put(self,0,0);put(self,0x0c,0);put(self,0x10,0);
 put(self,0x14,0);put(self,0x18,0xffffffffu);put(self,0x1c,0);
 (void)GuiAssign005e5140(name(self),nullptr,c2da_empty_0073d71c());
 put(self,0x2c,0);put(self,0x30,0);
}
}
