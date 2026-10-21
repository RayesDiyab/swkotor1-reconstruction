#include "14a315b30fefeafa_door_placement.hpp"
namespace recovered_door_placement {
void remove(U self,U s,Ports&p){
 if(p.area(self)){
  U id=p.read(self+4);U a=p.area(self);p.detach(a,id);
  id=p.read(self+4);a=p.area(self);U count=p.read(a+0x130),matches=0;
  if(static_cast<std::int32_t>(count)>0){U data=p.read(a+0x12c);do{if(p.read(data)==id)++matches;data+=4;}while(--count);}
  if(matches==1){id=p.read(self+4);a=p.area(self);p.erase_id(a+0x12c,id);}
  id=p.read(self+4);a=p.area(self);p.remove_bounds(a,id);
 }
 p.set_area(self,0);p.write(s,0);p.write(s+4,0);p.write(s+8,0);p.position(self,s,1,1,0);
}
void place(U self,U a,U x,U y,U z,U flag,U s,Ports&p){
 remove(self,s,p);if(!a)return;
 p.set_area(self,a);p.write(s,x);p.write(s+4,y);p.write(s+8,z);p.position(self,s,0,1,0);
 p.insert(a,p.read(self+4),flag);
 U capacity=p.read(a+0x1a8),count=p.read(a+0x1a4),id=p.read(self+4),list=a+0x1a0;
 if(count==capacity)p.grow(list,capacity?capacity+capacity:16);
 count=p.read(list+4);U data=p.read(list);p.write(data+4*count,id);count=p.read(list+4);p.write(list+4,count+1);
 if(!(self+0x32c))return;
 Vector direction;direction.x=p.read(self+0x9c);direction.y=p.read(self+0xa0);direction.z=p.read(self+0xa4);
 Vector position;position.x=p.read(self+0x90);position.y=p.read(self+0x94);position.z=p.read(self+0x98);
 if(!p.bounds(self,position,direction,self+0x338,self+0x344))return;
 id=p.read(self+4);p.add_bounds(a,id,self+0x338,self+0x344);
 direction.x=p.read(self+0x9c);direction.y=p.read(self+0xa0);direction.z=p.read(self+0xa4);
 position.x=p.read(self+0x90);position.y=p.read(self+0x94);position.z=p.read(self+0x98);
 p.geometry(self,position,direction,self+0x350,self+0x35c,self+0x368,self+0x374);
 p.write(self+0x380,4);
}
}
