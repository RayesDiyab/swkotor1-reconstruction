#include "f9391255a7e2a2b8_server_info.hpp"
namespace kotor::server_mainloop {
Word server_info_004b41c0(Memory&m,ServerInfoPorts&p,Word self,Word out,Word scratch){auto s=[&](Word n){return scratch+n;};auto list=[&](Word field){return m.read(m.read(self+field));};
 m.write(s(12),0);p.empty(s(16));p.empty(s(24));
 p.integer(s(40),m.read(list(0x10068)+8));p.literal(s(32),0x745464);
 Word a=p.concat(s(16),s(56),s(32));a=p.concat(a,s(48),s(40));p.assign(s(16),a);
 p.destroy(s(48));p.destroy(s(56));p.destroy(s(32));p.destroy(s(40));
 m.write(s(8),m.read(list(0x10068)));
 while(m.read(s(8))){Word entry=p.at(list(0x10068),m.read(s(8))),id=m.read(entry+4);Word net=p.network(m.read(m.read(0x7a39fc)+8));Word record=p.connection(net,id);p.assign_text(s(24),p.cstr(record+4));
  p.literal(s(40),0x73d5cc);a=p.concat(s(16),s(48),s(40));a=p.concat(a,s(56),s(24));p.assign(s(16),a);p.destroy(s(56));p.destroy(s(48));p.destroy(s(40));
  p.literal(s(32),0x745454);a=p.concat(s(16),s(64),s(32));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(32));p.next(list(0x10068),s(8));
 }
 p.literal(s(40),0x73d5cc);a=p.concat(s(16),s(64),s(40));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(40));
 p.integer(s(32),m.read(list(0x10064)+8));p.literal(s(40),0x745448);a=p.concat(s(16),s(56),s(40));a=p.concat(a,s(64),s(32));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(56));p.destroy(s(40));p.destroy(s(32));
 m.write(s(8),m.read(list(0x10064)));
 while(m.read(s(8))){Word entry=p.at(list(0x10064),m.read(s(8))),id=m.read(entry+4);Word net=p.network(m.read(m.read(0x7a39fc)+8));Word record=p.connection(net,id);p.assign_text(s(24),p.cstr(record+4));
  p.literal(s(40),0x73d5cc);a=p.concat(s(16),s(56),s(40));a=p.concat(a,s(64),s(24));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(56));p.destroy(s(40));
  p.literal(s(32),0x74543c);a=p.concat(s(16),s(48),s(32));p.assign(s(16),a);p.destroy(s(48));p.destroy(s(32));p.next(list(0x10064),s(8));
 }
 p.literal(s(40),0x73d5cc);a=p.concat(s(16),s(64),s(40));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(40));
 Word first=list(0x10068),second=list(0x10064);Word total=m.read(first+8)+m.read(second+8);p.integer(s(32),total);p.literal(s(40),0x745434);a=p.concat(s(16),s(56),s(40));a=p.concat(a,s(64),s(32));p.assign(s(16),a);p.destroy(s(64));p.destroy(s(56));p.destroy(s(40));p.destroy(s(32));
 p.copy(out,s(16));m.write(s(12),1);p.destroy(s(24));p.destroy(s(16));return out;
}
}
