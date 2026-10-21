#include "96d8876d797f2827_client_begin_leaves.hpp"
#include <cstring>
namespace recovered_client_begin {
String*name_005ed490(void*self,String*out,Ports&p){p.copy_construct(out,(char*)p.pointer(self,4)+0x13c);return out;}
void*message_005ed6f0(void*self,Ports&p){return p.pointer(p.pointer(self,4),0x14c);}
void temp_005edaa0(void*self,const String*s,Ports&p){void*inner=p.pointer(self,4);p.assign((char*)inner+0x294,s);unsigned one=1;std::memcpy((char*)inner+0x290,&one,4);}
void screen_005edd30(void*self,const String*s,Ports&p){p.assign((char*)p.pointer(self,4)+0x304,s);}
unsigned begin_006775e0(Ports&p){auto*data=static_cast<unsigned char*>(p.allocate(3));if(!data)return 0;data[0]=0x70;data[1]=1;data[2]=0;void*client=p.pointer(p.manager(),4);void*network=p.network(client);unsigned result=p.send(network,0xfffffffd,data,3,0);p.free(data);return result;}
}
