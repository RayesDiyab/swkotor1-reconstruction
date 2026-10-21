#include "21c86b7f97a27927_server_join.hpp"
namespace kotor::server_mainloop {
Word status_packet_00579890(Memory&m,ServerJoinPorts&p,Word self,Word recipient,Word text){(void)self;Word allocation_size=p.length(text)+1,buffer=p.allocate(allocation_size);if(!buffer)return 0; // Original failure EAX is the null allocation.
 p.write_byte(buffer,0x53);Word length=p.length(text),source=p.cstr(text),i=0;
 for(;i<(length&~3u);i+=4){m.write(buffer+1+i,m.read(source+i));}for(;i<length;++i)p.write_byte(buffer+1+i,m.read8(source+i));
 Word network=p.network(m.read(m.read(0x7a39fc)+8));Word result=p.send(network,recipient,buffer,allocation_size,0);p.free(buffer);return result;
}
void joined_00579a80(Memory&m,ServerJoinPorts&p,Word self,Word recipient,Word scratch){auto s=[&](Word n){return scratch+n;};p.empty(s(4));Word part=m.read(0x7a1f3c),domain=m.read(0x7a1f14);p.format(s(4),0x7493bc,domain,part);Word info=p.info(m.read(m.read(0x7a39fc)+8),s(20));Word combined=p.concat(s(4),s(12),info);p.assign(s(4),combined);p.destroy(s(12));p.destroy(s(20));(void)status_packet_00579890(m,p,self,recipient,s(4));p.destroy(s(4));}
}
