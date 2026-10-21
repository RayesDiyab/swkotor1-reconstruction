#include "005e504f13d9cc3b_room_ground_ray.hpp"
namespace kotor::server_mainloop {
Word mesh_ground_ray_00581530(Memory&m,RoomGroundRayPorts&p,Word mesh,Word from,Word to,Word hit,Word s){
 if(!m.read(mesh+0xb0))return 0;
 m.write(hit,m.read(mesh+0xe0));Word result=p.node(mesh,m.read(mesh+0xd4),from,to,hit);
 if(m.read(hit+0xc)==0xffffffff){m.write(s,0x3f800000);m.write(s+4,0x3f800000);m.write(s+8,0);p.normalize(s+0xc,s);p.scale(s+0xc,0x3a83126f);p.add(from,s+0xc);p.add(to,s+0xc);result=p.node(mesh,m.read(mesh+0xd4),from,to,hit);}
 if(static_cast<int>(result)>0){Word x=m.read(to),y=m.read(to+4);m.write(hit+0x10,x);Word z=m.read(to+8);m.write(hit+0x14,y);m.write(hit+8,1);m.write(hit+0x18,z);}
 return m.read(hit+8);
}
Word room_ground_ray_00579560(Memory&m,RoomGroundRayPorts&p,Word room,Word from,Word to,Word hit,Word s){
 if(!mesh_ground_ray_00581530(m,p,m.read(room+0x3c),from,to,hit,s))return 0;
 Word mesh=m.read(room+0x3c);p.world_point(hit+0x10,room+4,mesh+0x20);return 1;
}
}
