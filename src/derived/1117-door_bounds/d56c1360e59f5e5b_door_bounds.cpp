#include "59f5017c5df1a036_door_bounds.hpp"
namespace kotor::server_mainloop {
static void vector_copy(Memory&m,Word dst,Word src){for(Word i=0;i<12;i+=4)m.write(dst+i,m.read(src+i));}
Word door_bounds_00589980(Memory&m,DoorBoundsPorts&p,Word self,Word x,Word y,Word z,Word outMin,Word outMax,Word s){
 Word index=m.read(0x74c394),end=m.read(0x74c39c);for(Word i=0;i<24;i+=4)m.write(s+0x14+i,0);m.write(s+0x10,1);
 if(static_cast<int>(index)>static_cast<int>(end))return 1;
 do{
  Word position=m.read(self+0x32c+index*4)+0x20;m.write(position,x);m.write(position+4,y);m.write(position+8,z);
  Word q=m.read(self+0x32c+index*4)+0x38;Word q0=m.read(self+0x3b0),q1=m.read(self+0x3b4),q2=m.read(self+0x3b8),q3=m.read(self+0x3bc);m.write(q,q0);m.write(q+4,q1);m.write(q+8,q2);m.write(q+12,q3);
  Word result=p.bounds(m.read(self+0x32c+index*4),s+0x14,s+0x20);m.write(s+0x10,m.read(s+0x10)&result);
  if(m.read(0x832858))p.debug(self,index,s);
  if(index==m.read(0x74c394)){vector_copy(m,outMin,s+0x14);vector_copy(m,outMax,s+0x20);}
  else{for(Word i=0;i<12;i+=4)if(p.less(m.read(s+0x14+i),m.read(outMin+i)))m.write(outMin+i,m.read(s+0x14+i));for(Word i=0;i<12;i+=4)if(p.greater(m.read(s+0x20+i),m.read(outMax+i)))m.write(outMax+i,m.read(s+0x20+i));}
  end=m.read(0x74c39c);++index;
 }while(static_cast<int>(index)<=static_cast<int>(end));return m.read(s+0x10);
}
Word mesh_vertex_bounds_00596d10(Memory&m,MeshBoundsPorts&p,Word self,Word outMin,Word outMax,Word s){
 if(!m.read(self+0x54))return 0;
 Word initialized=0,index=0,offset=0;m.write(s+0x14,0);
 if(static_cast<int>(m.read(self+0x50))<=0)return 0;
 do{
  Word transformed=p.transform(self,s+0x24,m.read(self+0x54)+offset);
  Word x=m.read(transformed),y=m.read(transformed+4),z=m.read(transformed+8);m.write(s+0x18,x);m.write(s+0x1c,y);m.write(s+0x20,z);
  if(!initialized){m.write(outMax,x);m.write(outMax+4,y);m.write(outMax+8,z);m.write(outMin,x);m.write(outMin+4,y);m.write(outMin+8,z);initialized=1;}
  else{for(Word i=0;i<12;i+=4){if(p.greater(m.read(s+0x18+i),m.read(outMax+i)))m.write(outMax+i,m.read(s+0x18+i));if(p.less(m.read(s+0x18+i),m.read(outMin+i)))m.write(outMin+i,m.read(s+0x18+i));}}
  ++index;offset+=12;m.write(s+0x14,index);
 }while(static_cast<int>(index)<static_cast<int>(m.read(self+0x50)));return initialized;
}
}
