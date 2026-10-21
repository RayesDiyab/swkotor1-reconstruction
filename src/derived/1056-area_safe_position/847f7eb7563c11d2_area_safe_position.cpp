#include "eaf732ddeba257bd_area_safe_position.hpp"
namespace kotor::server_mainloop {
Word area_safe_position_004be860(Memory&m,AreaSafePositionPorts&p,Word area,Word input,Word range,Word shape,Word mode,Word output,Word s){
 m.write(output,m.read(input));m.write(output+4,m.read(input+4));m.write(output+8,m.read(input+8));
 m.write(area+0x1ac,shape);m.write(s+0x18,m.read(shape+4));
 Word x=m.read(input),y=m.read(input+4),z=m.read(input+8);m.write(s+0x54,x);m.write(s+0x58,y);m.write(s+0x5c,z);m.write(s+0x84,0);
 Word room=p.room(area,s+0x54,s+0x7c);
 if(room){x=m.read(s+0x54);y=m.read(s+0x58);z=m.read(s+0x5c);Word index=m.read(s+0x7c);m.write(s+0x5c,p.height_bits(room,index,x,y,z));}
 else m.write(s+0x5c,m.read(s+0x84));
 if(p.valid(area,s+0x54,shape)==1){x=m.read(s+0x54);y=m.read(s+0x58);z=m.read(s+0x5c);m.write(output,x);m.write(output+4,y);m.write(output+8,z);return 1;}
 return p.search_004be93d(area,input,range,shape,mode,output,s);
}
}
