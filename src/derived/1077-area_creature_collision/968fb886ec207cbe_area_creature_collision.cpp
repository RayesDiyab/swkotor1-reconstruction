#include "7d19257a25780478_area_creature_collision.hpp"
namespace kotor::server_mainloop {
Word area_creature_collision_004bc590(Memory&m,AreaCreatureCollisionPorts&p,Word area,Word from,Word to,Word shape,Word flag4,Word flag5,Word output,Word flag7,Word hit,Word s){
 if(p.equal(to,from))return 1;
 if(!shape)return 0;
 m.write(s+0x94,from);m.write(s+0x98,to);m.write(s+0x9c,shape);m.write(s+0xa0,flag4);m.write(s+0xa4,flag5);m.write(s+0xa8,output);m.write(s+0xac,flag7);m.write(s+0xb0,hit);
 Word radius=p.radius(m.read(shape+8)),id=m.read(shape+0x2c),server=m.read(m.read(0x7a39fc)+8);m.write(s+0x4c,id);m.write(s+0x5c,radius);Word owner=p.creature(server,id);m.write(s+0x58,owner);if(!owner)m.write(s+0xa4,0);
 m.write(s+0x54,m.read(shape+0x34));m.write(s+0x50,m.read(shape+0x30));p.geometry(from,to,s);
 Word high=p.expand(m.read(s+0x1c),true);
 Word x=m.read(from),y=m.read(from+4),z=m.read(from+8);m.write(s+0x6c,x);m.write(s+0x70,y);m.write(s+0x74,z);x=m.read(to);y=m.read(to+4);z=m.read(to+8);m.write(s+0x78,x);m.write(s+0x7c,y);m.write(s+0x80,z);
 Word last=m.read(area+0x194)-1;m.write(s+0x44,last);if(p.index(area,s+0x28,high)){last=m.read(s+0x28);m.write(s+0x44,last);}
 Word low=p.expand(m.read(s+0x18),false);m.write(s+0x20,0);if(p.index(area,s+0x28,low))m.write(s+0x20,m.read(s+0x28));
 output=m.read(s+0xa8);if(static_cast<int>(m.read(s+0x20))<=static_cast<int>(last))return p.nonempty_004bc790(area,from,to,last,output,s);
 if(output)m.write(output,0x7f000000);
 return 1;
}
}
