#include "2a0468f943a5761e_room_mesh_load.hpp"
namespace kotor::server_mainloop {
Word room_mesh_load_00596670(Memory&m,RoomMeshLoadPorts&p,Word self,Word flags,Word s){Word result=1,resource=0;
 p.ref_default(s+0x10);p.reference(m.read(m.read(self)+0xc),self,s+0x10);
 bool first=p.exists(m.read(0x7a39e8),s+0x10,m.read16(self+0x48))!=0;
 if(!first){p.reference(m.read(m.read(self)+0x10),self,s+0x10);if(!p.exists(m.read(0x7a39e8),s+0x10,m.read16(self+0x4a)))return result;}
 Word offset=first?0x48:0x4a;resource=p.lookup(m.read(0x7a39e8),s+0x10,m.read16(self+offset));
 if(!resource){Word allocation=p.allocate(first?0x38:0x34);resource=allocation?p.construct(first?0x5ceab0:0x5cea50,allocation):0;p.insert(m.read(0x7a39e8),s+0x10,m.read16(self+offset),resource);}
 if(!p.demand(resource))return result;
 if(first&&p.binary(resource)){
  if(flags){p.release(resource);p.dump(resource,1);return 0;}
  Word callback=m.read(m.read(self)+0x24);m.write(self+0x4c,1);result=p.parse_binary(callback,self,resource);m.write(self+0x1c,resource);return result;
 }
 Word table=m.read(self);m.write(self+0x4c,0);Word size=p.size(resource),data=p.data(resource);result=p.parse_text(m.read(table+0x28),self,data,size);p.release(resource);p.dump(resource,1);return result;
}
}
