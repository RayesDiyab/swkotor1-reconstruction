#include "9ebd48b774213157_room_height.hpp"
namespace kotor::server_mainloop {
Word room_height_bits_00579470(Memory&m,RoomHeightPorts&p,Word room,Word index,Word x,Word y,Word,Word scratch){
 Word mesh=m.read(room+0x3c);if(!mesh)return 0;
 m.write(scratch,x);m.write(scratch+4,y);
 Word distances=m.read(mesh+0x6c),normals=m.read(mesh+0x68);
 m.write(scratch+8,0xc1100000);Word d=m.read(distances+index*4);
 return p.plane(normals+(index+index*2)*4,d,scratch);
}
}
