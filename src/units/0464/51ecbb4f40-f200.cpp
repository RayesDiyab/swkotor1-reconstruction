#include "98ca969b25-f200.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;
W load(void* p,unsigned offset){W v;std::memcpy(&v,static_cast<unsigned char*>(p)+offset,4);return v;}
void store(void* p,unsigned offset,W v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,4);}
void __fastcall DestroyElement(S* value,void*){value->~StartupString();}
// Local ownership only. Actual canonical String objects must be live in the
// returned allocation; its original cookie contains the destruction count.
struct EnumerationOwner {
 f200_ports::Enumeration value{};
 ~EnumerationOwner(){
  S* elements=value.entries;
  if(elements){
   unsigned char* allocation=reinterpret_cast<unsigned char*>(elements)-4;
   W cookie;std::memcpy(&cookie,allocation,4);
   c2da_destroy_array_006fa852(elements,8,cookie,&DestroyElement);
   live_free(allocation);
  }
  value.entries=nullptr;
 }
};
}
namespace add_key_body_ports {
Word __fastcall Build0040f200(void* self,void*,Word last){
 // Original local construction/unwind order: String, enumeration, trivial Ref.
 S filename;
 EnumerationOwner names;
 menu_resources::Ref reference{}; // proven00405ED0 four-zero-DWORD support
 auto* key_name=std::launder(reinterpret_cast<S*>(static_cast<unsigned char*>(self)+0x20));
 f200_ports::Enumerate005e6640(key_debug_root_007a39e0(),nullptr,
     &names.value,key_name,0xffffu,0,0);
 const W count=names.value.count;
 const W slots=count*2u+1u;
 store(self,0x0c,slots);
 void* entries=f200_ports::Allocate0040eb80(self,nullptr,slots);
 store(self,0x10,reinterpret_cast<W>(entries));
 if(entries==nullptr)return 0;
 // Original JLE/JL interprets captured count as signed; arithmetic above wraps.
 for(W i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i){
  filename.Assign005e5c50(names.value.entries+i);
  W type=f200_ports::Type00406650(rim_current_manager_007a39e8(),nullptr,&filename);
  if(static_cast<unsigned short>(type)!=0xffffu){
   f200_ports::Ref004065a0(rim_current_manager_007a39e8(),nullptr,&reference,&filename);
   // Existing equality complement preserves consumed00406060 zero test;
   // it creates no second comparison algorithm or new address-credit claim.
   if(c2da_equal_text_004060d0(&reference,nullptr,c2da_empty_0073d71c())==0){
    W id=(load(self,0x28)|0xfffffc00u)<<20;
    f200_ports::Insert0040e990(self,nullptr,&reference,type,id,last);
   }
  }
 }
 return 1;
}
}
