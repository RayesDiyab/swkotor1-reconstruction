#include "e4a00e6aa4af0405_object_position.hpp"
namespace kotor::server_mainloop {
void object_orientation_004cd170(Memory&m,Word self,Word v){m.write(self+0x9c,m.read(v));m.write(self+0xa0,m.read(v+4));m.write(self+0xa4,m.read(v+8));m.write(self+0x1fc,m.read(self+0x1fc)|4);}
void object_position_004cd1a0(Memory&m,ObjectPositionPorts&p,Word self,Word v,Word update,Word unused,Word suppress,Word s){
 (void)unused;if(p.equal(self+0x90,v))return;if(!suppress)m.write(self+0x1fc,m.read(self+0x1fc)|2);
 m.write(s,0);Word array=p.object_array(m.read(m.read(0x7a39fc)+8));p.lookup(array,m.read(self+0x8c),s);Word area=0;if(m.read(s))area=p.virtual_call(m.read(s),0x28);
 m.write(self+0x90,m.read(v));m.write(self+0x94,m.read(v+4));m.write(self+0x98,m.read(v+8));
 if(update){if(p.virtual_call(self,0x30))p.virtual_call(self,0x30);if(area)p.update_area(area,self);}
}
}
