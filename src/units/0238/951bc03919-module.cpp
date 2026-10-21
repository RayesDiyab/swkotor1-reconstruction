#include "71840a883d-normal.h"
#include "b593572262-m50_vtables.h"   // lane-m50-vtables W-4: typed CSWCModule_vtable 00751848
using namespace menu_normal_retained_40_49;
Module* __thiscall Module::construct_00643f40(){
 providers.game_object_004d7d60(this,nullptr,*providers.type_00746632,0x7f000000);
 // 00643F7E mov [esi],00751848h: the typed CSWCModule table (lane-m50-vtables W-4), not the unmapped original word.
 put(0,kotor_vtable_word(&kotor_vtable_cswcmodule_00751848));providers.string_construct_005b3190(bytes+0x10,nullptr);
 providers.ref_default_00405ed0(bytes+0x18,nullptr);providers.ref_default_00405ed0(bytes+0x28,nullptr);
 providers.string_construct_005b3190(bytes+0x38,nullptr);
 for(unsigned o=0x4c;o<=0x6c;o+=4)put(o,0);put(0x118,0);
 providers.string_construct_005b3190(bytes+0x130,nullptr);
 for(unsigned o=0x4c;o<=0x68;o+=4)put(o,0);
 put(0x40,0);put(0x44,0);put(0x48,0);put(0x0c,0xffffffff);put(0x80,0);put(0x84,0);put(0x88,0);put(0x6c,0x3f800000);
 put(0x70,*providers.word_00746644);put(0x74,0);put(0x78,0);put(0x7c,0);put8(0x90,0);put(0x8c,0);put(0x94,0);put(0x98,0);put(0x9c,0);
 providers.assign_005e5140(bytes+0x10,nullptr,providers.empty_0073d71c);
 const U32 word=*providers.word_0074410c;
 for(unsigned o:{0xa8u,0xacu,0xecu,0xf8u,0xfcu})put(o,1);
 for(unsigned o:{0xb0u,0xb8u,0xbcu,0xc8u,0xccu,0xd0u,0xd8u,0xdcu,0xe0u,0xe4u,0xf0u,0xf4u,0x100u,0x104u,0x108u,0x10cu,0x12cu})put(o,0);
 put8(0xb4,0);put8(0xd4,0);put(0xa4,word);put8(0xa0,5);



 put8(0xd5,0xff);

 return this;
}
