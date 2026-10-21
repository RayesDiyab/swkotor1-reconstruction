#include "ab74cbbb05e21229_trigger_placement.hpp"
namespace kotor::server_mainloop {
void trigger_detach_0058ee30(Memory&m,TriggerPlacementPorts&p,Word self,Word s){
 if(p.area(self)){Word id=m.read(self+4);Word area=p.area(self);p.remove(area,id);area=p.area(self);id=m.read(self+4);p.erase(area+0x1a0,id);
 id=m.read(self+4);area=p.area(self);Word count=m.read(area+0x130),matches=0;
 if(static_cast<std::int32_t>(count)>0){Word cursor=m.read(area+0x12c);do{if(m.read(cursor)==id)++matches;cursor+=4;--count;}while(count);if(matches==1){id=m.read(self+4);area=p.area(self);p.erase(area+0x12c,id);}}
 }
 p.set_area(self,0);m.write(s+4,0);m.write(s+8,0);m.write(s+12,0);p.position(self,s+4,1,1,0);
}
void trigger_placement_0058f030(Memory&m,TriggerPlacementPorts&p,Word self,Word area,Word x,Word y,Word z,Word flag,Word s){
 // Stack argument slots are also the four final bounds outputs in the original.
 m.write(s+0x30,area);m.write(s+0x34,x);m.write(s+0x38,y);m.write(s+0x3c,z);m.write(s+0x40,flag);
 trigger_detach_0058ee30(m,p,self,s+0x44);area=m.read(s+0x30);if(!area)return;
 p.string_zero(s+0xc);p.set_area(self,area);x=m.read(s+0x34);y=m.read(s+0x38);z=m.read(s+0x3c);m.write(s+0x14,x);m.write(s+0x18,y);m.write(s+0x1c,z);p.position(self,s+0x14,0,1,0);
 flag=m.read(s+0x40);Word id=m.read(self+4);area=p.area(self);p.insert(area,id,flag);area=p.area(self);Word count=m.read(area+0x1a4);id=m.read(self+4);Word array=area+0x1a0;Word capacity=m.read(array+8);if(count==capacity)p.grow(array,capacity?capacity*2:16);
 Word index=m.read(array+4),data=m.read(array);m.write(data+index*4,id);m.write(array+4,m.read(array+4)+1);p.bounds(self,s+0x3c,s+0x38,s+0x34,s+0x30);p.string_destroy(s+0xc);
}
}
