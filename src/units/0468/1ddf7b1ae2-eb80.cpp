#include "68d27d7f1e-eb80.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using Ref=menu_resources::Ref;
void store(void*p,W offset,W value){std::memcpy(static_cast<unsigned char*>(p)+offset,&value,4);}
}
namespace f200_ports {
void* __fastcall Allocate0040eb80(void*,void*,Word capacity){
 void* allocation=live_operator_new(eb80_private::AllocationBytes(capacity));
 const Word base=reinterpret_cast<Word>(allocation);
 const bool construct=eb80_private::FirstPass(capacity);
 if(allocation&&construct){
  // Actual canonical Ref subobjects only; no invented28-byte entry class.
  for(Word i=0;i<capacity;++i){
   void* entry=reinterpret_cast<void*>(base+i*28u);
   ::new(entry)menu_resources::Ref{}; // original00405ED0 zero stores
   store(entry,16,0);store(entry,20,0); // trailing four bytes remain untouched
  }
 }
 // Original leaves allocation cleanup state before this second pass.
 // Null allocation is NOT a general return: only entry address0 is skipped.
 for(Word i=0;i<capacity;++i){
  void* entry=reinterpret_cast<void*>(base+i*28u);
  if(entry){
   // The high-capacity path skipped zero construction. Start the actual
   // implicit-lifetime Ref without stores before the genuine assignment.
   if(!construct)::new(entry)menu_resources::Ref;
   auto* ref=std::launder(reinterpret_cast<menu_resources::Ref*>(entry));
   menu_resources::Ref discarded;
   (void)unresolved_ref_assign_00406290(ref,nullptr,&discarded,"");
   store(entry,16,0);store(entry,20,0xffffffffu);
   const unsigned short zero=0,invalid=0xffff;
   std::memcpy(static_cast<unsigned char*>(entry)+24,&zero,2);
   std::memcpy(static_cast<unsigned char*>(entry)+26,&invalid,2);
  }
 }
 return allocation;
}
}
