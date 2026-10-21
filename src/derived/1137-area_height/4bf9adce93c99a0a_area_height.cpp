#include "0ad07ddbcade454e_area_height.hpp"
namespace kotor::server_mainloop {
Word area_height_bits_004bc380(Memory&m,AreaHeightPorts&p,Word area,Word point,Word flag,Word out,Word s){
 m.write(s+4,0);Word room=p.find(area,point,s+0xc);if(out)m.write(out,room);if(!room)return m.read(s+4);
 Word x=m.read(point),y=m.read(point+4),z=m.read(point+8);if(flag)return p.ground(room,m.read(s+0xc),x,y,z);return p.alternate(room,x,y,z,0);
}
}
