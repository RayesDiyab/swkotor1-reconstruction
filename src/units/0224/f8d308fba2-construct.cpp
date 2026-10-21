#include "1adde386db-resources.h"
#include <new>
namespace menu_resources {
Manager* Manager::construct_00409bf0(){
 for(unsigned offset=0x10;offset<=0x24;offset+=4)unresolved_keylist_ctor_00406cf0(bytes+offset,nullptr);
 unresolved_critical_ctor_005e9290(bytes+0x40,nullptr);
 ::new(bytes+0x4c) String();
 unresolved_gff_hash_init_00410520();
 word(0x28,0);word(0x30,0);bytes[0x34]=0;
 MemoryStatus memory;unresolved_global_memory_status_0073d1c0(&memory);
 const Word total=memory.words[2];word(0,total);



 const Word half=(total>>1)|(total&0x80000000u);int32_t signed_half;memcpy(&signed_half,&half,4);
 const Word limit=signed_half>0x1000000?half:0x1000000;

 word(4,limit);word(8,limit);
 bytes[0x44]=0;bytes[0x5c]=0;word(0x48,0);
 Word parameter=0;
 const Word handle=unresolved_create_thread_0073d1f0(nullptr,0x10000,&unresolved_async_rim_thread_00409b90,&parameter,4,reinterpret_cast<Word*>(bytes+0x3c));
 word(0x38,handle);return this;
}
}
