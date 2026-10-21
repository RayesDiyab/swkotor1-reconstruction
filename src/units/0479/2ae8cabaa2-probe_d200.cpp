// lane-resource-lowers: fb80_ports::Probe0040d200 (Ghidra: CExoKeyTable::LocateBifFile),
// original 0040D200-0040D2DD. Declared by task-2 fb80-r1/fb80.h:11:
//   W __fastcall Probe0040d200(void*,void*,S*)  -- ECX receiver, EDX unused, one stack DWORD, RET 4.
// Observed facts (evidence/probe_d200.txt, evidence/eh_d200.txt):
//  * ECX (receiver) is never read: 0040D246 overwrites ECX with the stack argument
//    before any use; EDX is never read. RET 4 at 0040D2B7 and 0040D2DD.
//  * 0040D21C push 4; call 006FA7E6 (operator new, M40 live_operator_new) -> raw.
//    EH state 0 (0040D232) covers raw: funclet 007148B0 frees [ebp-18h]=raw via 006FA390.
//  * raw==0 (0040D236 je 0040D26E): file=0, the mode temporary is never built.
//  * 0040D238 push 73D5B0 ("rb"); 005E5A90 String(const char*) builds the mode temporary;
//    bl=1 marks it live; state 1 (0040D25C); funclet 007148BB destroys it only if bit0 set.
//  * 0040D24F push 270Eh (BIF type); 0040D265 call 005E68D0(raw, name, 0x270E, &mode).
//  * 0040D270-0040D281: state -1, then the live mode temporary is destroyed (005E5C20).
//  * file==0 -> return 0 (0040D2A6). Otherwise 005E6A10 opened test; both outcomes run
//    005E69E0 then 006FA390(file) (operator delete -> free); return 0 / 1 (0040D2CD).
#include "06b965e51a-fb80.h"
#include <new>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;
void* attempt(S* name){
 void* raw=live_operator_new(4);
 if(!raw)return nullptr;
 alignas(S) unsigned char storage[sizeof(S)];S* mode;
 try{mode=::new(storage)S("rb");}catch(...){live_free(raw);throw;}          // state 0 unwind
 void* file;
 try{file=f3c0_ports::Construct005e68d0(raw,nullptr,name,0x270eu,mode);}
 catch(...){mode->~StartupString();live_free(raw);throw;}                   // state 1 -> 0 unwind
 mode->~StartupString();                                                     // 0040D281 after state -1
 return file;
}
}
namespace fb80_ports {
W __fastcall Probe0040d200(void*,void*,S* name){
 void* file=attempt(name);
 if(!file)return 0;
 const W opened=f3c0_ports::Opened005e6a10(file,nullptr);
 f3c0_ports::Destroy005e69e0(file,nullptr);
 live_free(file);
 return opened?1u:0u;
}
}
