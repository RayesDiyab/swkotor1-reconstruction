#include "dd80e0b2e910b2a5_packet_dispatch.hpp"
#include <cstring>
namespace recovered_server_packets {
namespace{unsigned r(void*p,unsigned o){unsigned v;std::memcpy(&v,static_cast<unsigned char*>(p)+o,4);return v;}}
unsigned dispatch_004b15d0(void*self,std::uint32_t id,void*packet,std::uint32_t length,std::uint32_t flags,Ports&p){
 if(flags!=0)return 0;
 if(*static_cast<unsigned char*>(packet)==0x73&&r(p.connection(p.pointer(self,0x10040),id),0x24)==1)p.dispatch_00528380(p.pointer(self,0x10010),id,packet,length);
 if(*static_cast<unsigned char*>(packet)==0x70&&r(p.connection(p.pointer(self,0x10040),id),0x1c)==1)p.dispatch_00527b20(p.pointer(self,0x10010),id,packet,length);
 return 1;
}
}
