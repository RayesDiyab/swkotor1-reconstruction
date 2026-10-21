#include "5017a73cfbb16784_net_connect.hpp"
#include <cstring>
namespace recovered_net_connect {
namespace {unsigned rd(void*p,unsigned o){unsigned x;std::memcpy(&x,(char*)p+o,4);return x;}void wr(void*p,unsigned o,unsigned x){std::memcpy((char*)p+o,&x,4);}}
unsigned start_005d5040(void*self,unsigned unused,const String*name,const String*password,unsigned unused2,unsigned mode,Ports&p){(void)unused;(void)unused2;if(rd(self,0x20060)==1)return 0;wr(self,0x20060,0);unsigned size=p.length(name)+9;auto*packet=static_cast<unsigned char*>(p.allocate(size));packet[0]='B';packet[1]='N';packet[2]='C';packet[3]='S';packet[4]='U';packet[5]=0;packet[6]=0;packet[7]=static_cast<unsigned char>(mode);packet[8]=static_cast<unsigned char>(p.length(name));unsigned length=p.length(name);const char*text=p.text(name);std::memcpy(packet+9,text,length);p.send(self,0,packet,size,0);wr(self,0x20060,1);p.assign((char*)self+0x20064,password);wr(self,0x2006c,mode);wr(self,0x20070,0);wr(self,0x20074,0);wr(self,0x20078,0);p.free(packet);return 1;}
}
