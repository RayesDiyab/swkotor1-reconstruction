// lane-resource-lowers: 005E6670 -> 005E7A40 and 005E6450 (see base_extension_type.h).
// Evidence: evidence/thunks6660.txt, evidence/gettype7a40.txt, evidence/cmpnc6450.txt.
// Mirrors the M40 receiver projection of main-provider-joins/base_extension.cpp
// (c2da_base_005e6660 -> 005E7A00) for the sibling 005E6670 -> 005E7A40.
#include "989c9a724f-base_extension_type.h"
#include "a4a3a777a0-baseinternal_storage.h"
#include <cstddef>
#include <type_traits>
namespace {
using Base=base_ini_reconstruction::Base;
using Internal=base_ini_reconstruction::BaseInternal;
using String=kotor_reconstruction::StartupString;
static_assert(offsetof(Base,internal)==0x14,"005E6670 mov ecx,[ecx+14h]");
static_assert(sizeof(Internal)==12&&offsetof(Internal,extension_ids)==4&&offsetof(Internal,extension_strings)==8,"005E7A40 reads [edi],[edi+4],[edi+8]");
// strlen as the inline loops at 005E645E-005E646A / 005E648C-005E6498.
std::int32_t length(const char* text){std::int32_t n=0;while(text[n]!='\0')++n;return n;}
}
extern "C" std::int32_t __fastcall exostring_compare_no_case_005e6450(String* self,void*,String* other){
 const char* const right=other->data;                    // 005E6458 edi=[other]
 const std::int32_t right_length=right?length(right):0;   // 005E645A-005E6472
 const char* const left=self->data;                       // 005E6474 esi=[ecx]
 if(!right)return left?0:1;                               // 005E6476/005E6478
 if(!left)return 0;                                       // 005E6488
 if(right_length!=length(left))return 0;                  // 005E649A
 for(std::int32_t i=0;i<right_length;++i){                // 005E64A0 jle / 005E64DF jl (signed)
  const signed char a=static_cast<signed char>(right[i]); // 005E64A8 al=[edx] (other)
  const signed char c=static_cast<signed char>(left[i]);  // 005E64AA cl=[esi+edx] (self)
  if(a==c)continue;                                       // 005E64AF
  if(a>='A'&&a<='Z'){                                     // 005E64B1/005E64B5 signed byte range
   if(static_cast<std::int32_t>(a)+0x20!=static_cast<std::int32_t>(c))return 0; // 005E64B9-005E64C4
  }else if(c>='A'&&c<='Z'){                               // 005E64C6/005E64CB
   if(static_cast<std::int32_t>(c)+0x20!=static_cast<std::int32_t>(a))return 0; // 005E64D0-005E64DB
  }else return 0;                                         // 005E64C9/005E64CE -> 005E64EF
 }
 return 1;                                                // 005E64E6
}
extern "C" std::uint16_t __fastcall c2da_base_005e6670(void* receiver,void*,String* extension){
 auto* const internal=static_cast<Internal*>(static_cast<Base*>(receiver)->internal); // 005E6670
 // 005E7A45-005E7A66: the count, the String array and (on a hit) the ID array are
 // re-read from the live Internal on every iteration; the count test is signed.
 for(std::int32_t i=0;i<static_cast<std::int32_t>(internal->extension_count);++i){
  String* const entry=base_resource_table::LiveStringElement(internal->extension_strings,static_cast<std::uint32_t>(i));
  if(exostring_compare_no_case_005e6450(entry,nullptr,extension)!=0)
   return internal->extension_ids[i];                     // 005E7A78-005E7A7B
 }
 return internal->extension_ids[87];                      // 005E7A68 word [ids+0AEh]
}
