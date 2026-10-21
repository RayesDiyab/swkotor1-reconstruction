#include "8c1839a37fa5d4b7_vm_construct.hpp"
#include <cstring>
#include <initializer_list>
namespace recovered_vm_construct {
namespace {void word(void*p,unsigned o,unsigned v){std::memcpy((char*)p+o,&v,4);}void put(void*p,unsigned o,const void*v){word(p,o,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(v)));}void* get(void*p,unsigned o){unsigned v;std::memcpy(&v,(char*)p+o,4);return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v));}}
void* frame_005d1740(void*self,Ports&p){p.string_default((char*)self+0x18);p.string_empty((char*)self+0x18);for(unsigned o:{0U,4U,8U,12U,16U,20U,32U,36U})word(self,o,0);return self;}
void* internal_005d4900(void*self,Ports&p){
 put(self,0,p.base_table());p.reference_default((char*)self+0xc);word(self,8,0);alignas(4) unsigned char temporary[16];p.reference_empty((char*)self+0xc,temporary);word(self,4,0);put(self,0,p.derived_table());
 for(unsigned n=0;n<8;++n)frame_005d1740((char*)self+0x2c+n*0x28,p);
 for(unsigned o:{0x1acU,0x1b0U,0x1b4U,0x1b8U,0x1bcU})word(self,o,0);
 word(self,0x28,0xffffffff);word(self,0x3d4,0);
 // Constructor-only specialization of005D2140: both owned pointers were just
 // zeroed, so no cleanup/free branch can run. This is not a general VM reset.
 word(self,0x1b0,0);word(self,0x1ac,0);word(self,0x1b4,0);
 put(self,0x1c0,self);return self;
}
void* outer_005d0f40(void*self,Ports&p){void*s=p.allocate(0x3d8);void*i=s?internal_005d4900(s,p):nullptr;put(self,0,i);return self;}
void set_implementer_005d1a40(void*self,void* implementer,Ports&p){put(self,0x3d4,implementer);if(implementer)p.initialize_implementer(implementer);}
void outer_set_005d0ff0(void*self,void* implementer,Ports&p){set_implementer_005d1a40(get(self,0),implementer,p);}
}
