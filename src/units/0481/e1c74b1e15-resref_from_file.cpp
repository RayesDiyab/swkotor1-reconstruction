// lane-resource-lowers: f200_ports::Type00406650 and f200_ports::Ref004065a0
// (task-2 f200-r1/f200.h:13-14). Evidence: evidence/type00406650.txt,
// evidence/ref004065a0.txt; CC-01/CC-02 evidence packets (task-2
// master-catalog-delta-r1/proposed-workspaces) agree with this reading.
#include "06b965e51a-fb80.h"                 // fb80_ports::Find005e5f20 (CC-03 forward), SubString005e6270
#include "9d8f75a410-async_load.h"           // existing async_ref_assign_string_004061f0 (004061F0)
#include "989c9a724f-base_extension_type.h"  // c2da_base_005e6670 (005E6670)
#include <new>
#include <type_traits>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;using Ref=menu_resources::Ref;
static_assert(std::is_same<async_load::Ref,Ref>::value&&std::is_same<async_load::String,S>::value,"one Ref/String epoch");
// Raw storage for a SubString output: SubString005e6270 begins the String
// lifetime itself (source_substring.h). The guard destroys it only once begun.
struct FreshString {
 alignas(S) unsigned char bytes[sizeof(S)];bool live=false;
 S* output(){return reinterpret_cast<S*>(bytes);}
 ~FreshString(){if(live)std::launder(output())->~StartupString();}
};
}
namespace f200_ports {
// 00406650: ECX/EDX never read (ECX overwritten at 00406669); one stack String*,
// RET 4. Only AX is produced by the original; this port returns it zero-extended
// (inferred typed contract: every consumer reads 16 bits, see CC-01 HANDOFF).
Word __fastcall Type00406650(menu_resources::Manager*,void*,String* filename){
 S extension;                                                         // 0040666D 005B3190, state 0
 const std::int32_t dot=fb80_ports::Find005e5f20(filename,nullptr,'.',0); // 00406676-00406684
 if(dot<0)return 0xffffu;                                             // 0040668B jge; 00406699 dtor; or ax,0FFFFh
 {
  FreshString piece;                                                  // [esp+0Ch] T
  S* const result=startup_rand::SubString005e6270(filename,nullptr,piece.output(),dot+1,3); // 004066B4-004066BF
  piece.live=true;                                                    // state 1 (004066C9)
  extension.Assign005e5c50(result);                                   // 004066CE
 }                                                                    // 004066DC T destroyed, state 0
 const std::uint16_t type=c2da_base_005e6670(key_debug_root_007a39e0(),nullptr,&extension); // 004066E1-004066EC
 return type;                                                         // 004066FF dtor L; 00406708 mov ax,si
}
// 004065A0: ECX/EDX never read; stack Ref* out, String* name; RET 8; no result.
void __fastcall Ref004065a0(menu_resources::Manager*,void*,Ref* out,String* name){
 const std::int32_t dot=fb80_ports::Find005e5f20(name,nullptr,'.',0);   // 004065BD-004065C3
 if(dot<=0){                                                          // 004065CA jg (signed)
  Ref discarded;                                                      // hidden 16-byte return slot
  (void)unresolved_ref_assign_00406290(out,nullptr,&discarded,c2da_empty_0073d71c()); // 004065CC-004065DA push 73D71C
  return;
 }
 const std::int32_t count=dot>16?16:dot;                              // 004065F1-004065F6 (signed jle)
 FreshString stem;                                                    // [esp+4]
 S* const result=startup_rand::SubString005e6270(name,nullptr,stem.output(),0,count); // 004065FB-00406605
 stem.live=true;                                                      // state 0 (00406614)
 Ref discarded;                                                       // [esp+14h] hidden return slot
 (void)async_ref_assign_string_004061f0(out,nullptr,&discarded,result); // 0040660A-0040661C
}                                                                     // 0040662D temporary destroyed
}
