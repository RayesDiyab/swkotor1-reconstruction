#include "b94f801e15fab830_options_leaf.hpp"
#include <cstring>
#include <initializer_list>
namespace recovered_options_leaf {
namespace{void word(void*p,unsigned o,unsigned v){std::memcpy((char*)p+o,&v,4);}}
void* construct_00520980(void*self,void*(*base)(void*),const void* vtable){base(self);word(self,0,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(vtable)));return self;}
void* construct_0052b8e0(void*self,Ports&p){
 for(unsigned o:{0x18U,0x10U,0x14U,0xcU,8U,4U,0U})word(self,o,0);
 void*items=p.allocate(12);if(items){word(items,4,0);word(items,8,0);word(items,0,0);}
 word(self,0x1c,0);word(self,0x20,0);word(self,0x24,static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(items)));return self;
}
}
