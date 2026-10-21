#include "b22a0349fb60fb33_area_creature_collision_scan.hpp"
namespace kotor::server_mainloop {
Word area_collision_scan_004bc790(Memory&m,AreaCollisionScanPorts&p,Word area,Word from,Word output,Word s){
 for(;;){
  Word id=m.read(m.read(area+0x190)+m.read(s+0x20)*4);Word server=m.read(m.read(0x7a39fc)+8);Word object=p.creature(server,id);m.write(s+0x24,object);
  if(object&&id!=m.read(s+0x4c)&&id!=m.read(s+0x50)&&id!=m.read(s+0x54)&&!p.state(object)&&!p.incapacitated(m.read(s+0x24))){
   if(!m.read(s+0xa4)||!p.reaction(m.read(s+0x58),m.read(s+0x24))){
    Word route=p.geometry_route(m.read(s+0x24),s);
    if(route){Word result=p.contact(area,m.read(s+0x9c),s+0x60,id,from,m.read(s+0x10),m.read(s+0xa0),m.read(s+0xac),output);
     if(!result){if(route==2){Word hit=m.read(s+0xb0);p.notify(area,m.read(s+0x24)+0x90,m.read(s+0x10),from,m.read(s+0x98),hit);if(output)m.write(output,id);hit=m.read(s+0xb0);if(hit)m.write(hit+0x40,id);}return 0;}
    }
   }
  }
  Word index=m.read(s+0x20)+1,end=m.read(s+0x44);m.write(s+0x20,index);if(static_cast<int>(index)>static_cast<int>(end))break;
 }
 if(output)m.write(output,0x7f000000);
 return 1;
}
}
