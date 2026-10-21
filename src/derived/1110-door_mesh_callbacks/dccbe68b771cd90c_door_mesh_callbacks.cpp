#include "427364b564824a88_door_mesh_callbacks.hpp"
namespace kotor::server_mainloop {
void door_mesh_reference_005ce8b0(Memory&m,DoorMeshCallbackPorts&p,Word self,Word out,Word s){
 p.empty(s+0xc);p.empty(s+4);p.ref_string(self+0xc,s+0xc);
 Word suffix=m.read(self+0x88);Word base=p.text(s+0xc);p.format(s+4,base,suffix);
 p.assign_string(out,s+0x10,s+4);p.destroy(s+4);p.destroy(s+0xc);
}
Word door_mesh_resource_vector_005ce010(Memory&m,Word resource,Word out,Word index){
 Word h=m.read(resource+0x34);Word source=h+(index*3u+9u)*4u;
 for(Word i=0;i<12;i+=4)m.write(out+i,h?m.read(source+i):0);
 return out;
}
Word door_mesh_binary_005ce770(Memory&m,DoorMeshCallbackPorts&p,Word self,Word resource,Word s){
 (void)room_mesh_binary_base_00597120(m,self,resource);
 for(Word index=0;index<2;++index){
  Word h=m.read(resource+0x34);Word source=h+0x24+index*12;
  if(!h){m.write(s+0x14,0);m.write(s+0x18,0);m.write(s+0x1c,0);}
  Word x=h?m.read(source):0,y=h?m.read(source+4):0,z=h?m.read(source+8):0;
  m.write(s+8,x);bool zeroX=p.ordered_zero(x);m.write(s+0x10,z);m.write(s+0xc,y);
  if(!zeroX||!p.ordered_zero(m.read(s+0xc))||!p.ordered_zero(m.read(s+0x10))){
   Word v=door_mesh_resource_vector_005ce010(m,resource,s+0x14,index);
   Word dst=self+0x70+index*12;
   for(Word i=0;i<12;i+=4)m.write(dst+i,m.read(v+i));
  }
 }
 return 1;
}
}
