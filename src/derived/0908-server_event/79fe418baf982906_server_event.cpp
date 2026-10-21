#include "f2d7eade096d02cc_server_event.hpp"
namespace kotor::server_mainloop {
void server_event_004b78f0(Memory&m,ServerEventPorts&p,Word self,Word id,Word kind,Word unused,Word scratch){
 (void)unused; auto s=[&](Word n){return scratch+n;};
 auto list=[&](Word field){return m.read(m.read(self+field));};
 auto record=[&](){return p.connection(m.read(self+0x10040),id);};
 auto log=[&](Word name){p.cstr(s(0x18));p.cstr(name);};
 p.empty(s(0x18));p.timestamp(m.read(m.read(0x7a39e0)+8),s(0x18));
 if(kind==1){
  p.copy(s(0x0c),record()+4);p.cstr(s(0x0c));
  if(m.read(record()+0x24)==1){Word object=p.allocate(12);if(object)object=p.player(0x5802f0,object,id);m.write(object+8,0);p.add_head(list(0x10068),object);log(s(0x0c));}
  if(m.read(record()+0x1c)==1){Word object=p.allocate(0x84);if(object)object=p.player(0x562c70,object,id);m.write(object+8,0);p.add_head(list(0x10064),object);log(s(0x0c));}
  p.joined(m.read(self+0x10010),m.read(0x74c534));p.destroy(s(0x0c));
 }else if(kind==0){
  p.copy(s(0x20),record()+4);m.write(s(0x14),0);
  if(m.read(record()+0x24)==1){
   m.write(s(0x0c),m.read(list(0x10068)));
   while(m.read(s(0x0c))){Word object=p.at(list(0x10068),m.read(s(0x0c)));
    if(m.read(object+4)==id){p.remove(list(0x10068),m.read(s(0x0c)));p.deleting_destroy(object,1);break;}
    p.next(list(0x10068),s(0x0c));
   }log(s(0x20));
  }
  if(m.read(record()+0x1c)==1){
   m.write(s(0x0c),m.read(list(0x10064)));
   while(m.read(s(0x0c))){Word object=p.at(list(0x10064),m.read(s(0x0c)));
    if(m.read(object+4)==id){
     if(m.read(self+0x100f8)==m.read(s(0x0c)))p.next(list(0x10064),self+0x100f8);
     p.remove(list(0x10064),m.read(s(0x0c)));p.left(m.read(self+0x10010),m.read(0x74c52c),object);
     m.write(s(0x14),m.read(object+0x3c));p.deleting_destroy(object,1);break;
    }p.next(list(0x10064),s(0x0c));
   }
   p.client_left(p.application_client(m.read(0x7a39fc),1),id);log(s(0x20));
   if(m.read(s(0x14)))p.shutdown(self,1,0);
  }
  Word count=m.read(self+0x10020),matches=0;
  if(static_cast<std::int32_t>(count)>0){Word base=m.read(self+0x1001c);for(Word i=0;i<count;++i)if(m.read(base+4*i)==id)++matches;
   if(matches){p.remove_id(self+0x1001c,id);if(!m.read(self+0x10020)){m.write(self+0x10028,1);m.write(self+0x1002c,0);}}
  }p.destroy(s(0x20));
 }p.destroy(s(0x18));
}
}
