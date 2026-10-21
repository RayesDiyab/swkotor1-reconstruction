#include "6e97814237522ee2_door_finish.hpp"
namespace kotor::server_mainloop {
void door_finish_00589d40(Memory&m,DoorFinishPorts&p,Word self,Word s){
 if(m.read8(self+0x2c2)){
  m.write(self+0x1ec,1);Word storage=p.allocate(0x8c);Word effect=storage?p.construct(storage,1):0;
  Word kind=m.read16(0x746684);Word flags=m.read16(effect+0xa);m.write16(effect+8,kind);
  flags=(flags&0xfff8)|m.read16(0x7473c4);m.write(effect+0x20,0);m.write16(effect+0xa,flags);
  p.integer(effect,0,m.read(0x746890));p.integer(effect,1,m.read8(self+0x2c2));
  p.apply(self,effect,0,0);m.write(self+0x1ec,0);
 }
 if(!m.read(self+0x308)){p.state_00589600(self,3,1);m.write(self+0xf8,1);}
 if(m.read(self+0x3c0)){
  p.empty(s+8);p.literal(s+0x10,0x7498a0);
  Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x28);
  Word ok=p.table_string(table,m.read8(self+0x2a1),s+0x10,s+8);p.destroy(s+0x10);
  if(!ok)p.assign_literal(s+8,0x73d71c);
  if(p.compare(s+8,0x749898))p.animation(self,0x27a9);
  p.destroy(s+8);
 }
}
}
