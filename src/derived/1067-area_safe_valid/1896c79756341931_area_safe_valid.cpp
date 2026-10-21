#include "46a90adb20010236_area_safe_valid.hpp"
namespace kotor::server_mainloop {
Word area_safe_valid_004be5e0(Memory&m,AreaSafeValidPorts&p,Word area,Word input,Word shape,Word s){
 m.write(s+0x54,input);m.write(s+0x58,shape);m.write(area+0x1ac,shape);m.write(s+0x14,m.read(shape+4));m.write(s+0x10,0);p.bounds(input,shape,s);
 Word room=p.room(area,s+0x38,0);if(!room)return 0;
 Word x=m.read(s+0x38),y=m.read(s+0x3c),z=m.read(s+0x40);
 for(;;){
  Word mx=m.read(s+0x2c),my=m.read(s+0x30),mz=m.read(s+0x34);
  m.write(s+0x44,0);m.write(s+0x48,0);m.write(s+0x4c,0);m.write(s+0x20,0);m.write(s+0x24,0);m.write(s+0x28,0);
  p.mesh(m.read(room+0x3c),x,y,z,mx,my,mz,s+0x44,s+0x20,s+0x1c);
  if(!p.sweep(room,s+0x44,s+0x20,s+0x54,s+0x18,s+0x14,0))return 0;
  if(p.near(m.read(s+0x2c),m.read(s+0x30),m.read(s+0x20),m.read(s+0x24)))break;
  Word next=p.adjacent(room,m.read(s+0x1c));if(next==0xffffffff)return 0;
  room=m.read(area+0x230)+next*0x4c;if(m.read(room+0x38))return 0;
  if(p.equal(s+0x44,s+0x20))m.write(s+0x10,1);
  Word stop=m.read(s+0x10);x=m.read(s+0x20);y=m.read(s+0x24);z=m.read(s+0x28);m.write(s+0x38,x);m.write(s+0x3c,y);m.write(s+0x40,z);if(stop)break;
 }
 shape=m.read(s+0x58);Word extent=m.read(shape+0xbc),id=m.read(s+0x14),position=m.read(s+0x54);
 if(!p.region(area,s+0x38,s+0x2c,position,m.read(s+0x10),id,extent,0,0))return 0;
 return p.collision(area,s+0x38,s+0x2c,shape,1,0,0,0,0)!=0?1u:0u;
}
}
