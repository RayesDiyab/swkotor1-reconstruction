#include "b164542a0d5e41bf_area_find_room.hpp"
namespace kotor::server_mainloop {
Word area_find_room_004bb600(Memory&m,AreaFindRoomPorts&p,Word area,Word input,Word output,Word s){
 Word z=m.read(input+8),y=m.read(input+4);m.write(s+0x24,p.offset_z(z,true));z=m.read(input+8);Word x=m.read(input);m.write(s+0x1c,x);m.write(s+0x10,x);m.write(s+0x18,p.offset_z(z,false));m.write(s+0x20,y);m.write(s+0x14,y);
 p.hit_construct(s+0x28,0xffffffff);if(output)m.write(output,0xffffffff);
 Word count=m.read(area+0x22c),index=0,offset=0;if(static_cast<int>(count)<=0)return 0;
 Word cached_y=m.read(s+0x20);
 do{
  Word xlow=m.read(s+0x10),ylow=m.read(s+0x14),zlow=m.read(s+0x18),xhigh=m.read(s+0x1c),zhigh=m.read(s+0x24);Word room=m.read(area+0x230)+offset;
  if(p.ray(room,xhigh,cached_y,zhigh,xlow,ylow,zlow,s+0x28)){
   Word result=m.read(area+0x230)+index*0x4c;if(output)m.write(output,m.read(s+0x34));return result;
  }
  count=m.read(area+0x22c);++index;offset+=0x4c;
 }while(static_cast<int>(index)<static_cast<int>(count));return 0;
}
}
