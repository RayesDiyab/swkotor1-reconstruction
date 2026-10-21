#include "2b338cca142b4bd4_area_waypoints.hpp"
namespace kotor::server_mainloop {
Word area_waypoints_00505360(Memory&m,AreaWaypointPorts&p,Word area,Word gff,Word root,Word saved,Word,Word s){
 if(!p.list(gff,s+0x20,root,0x7474d8))return 1;
 Word index=0;m.write(s+0x10,0);if(!p.count(gff,s+0x20))return 1;
 do{
  if(p.element(gff,s+0x48,s+0x20,index)&&p.type(gff,s+0x48)==5){
   Word id=p.dword(gff,s+0x48,0x744c24,s+0x44,0x7f000000),storage=p.allocate(0x240);m.write(s+0x4c,storage);Word object=storage?p.construct(storage,id):0;
   if(object){if(!p.load(object,gff,s+0x48))p.destroy(object);else{
    if(saved)p.common(object,gff,s+0x48);
    Word flag=saved==0;Word z=p.floating(gff,s+0x48,0x745d68,s+0x44,0);Word y=p.floating(gff,s+0x48,0x745d74,s+0x44,0);Word x=p.floating(gff,s+0x48,0x745d80,s+0x44,0);p.place(object,area,x,y,z,flag);
    x=m.read(object+0x90);y=m.read(object+0x94);z=m.read(object+0x98);m.write(s+0x14,x);m.write(s+0x18,y);m.write(s+0x1c,z);m.write(s+0x1c,p.height(area,s+0x14,1,0));p.position(object,s+0x14,1,1,0);
   }}
  }
  index=m.read(s+0x10)+1;m.write(s+0x10,index);
 }while(index<p.count(gff,s+0x20));return 1;
}
}
