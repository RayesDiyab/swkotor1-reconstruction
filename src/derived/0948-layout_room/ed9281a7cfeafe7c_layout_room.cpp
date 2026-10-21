#include "ad8b317e488f5b0e_layout_room.hpp"
namespace kotor::server_mainloop {
Word layout_room_construct_005793b0(Memory&m,LayoutRoomPorts&p,Word self){p.base_construct(self);m.write(self,p.vtable());Word allocation=p.allocate(0xe8);m.write(self+0x3c,allocation?p.mesh_construct(allocation):0);return self;}
void layout_room_destroy_005794b0(Memory&m,LayoutRoomPorts&p,Word self){m.write(self,p.vtable());Word mesh=m.read(self+0x3c);if(mesh){p.mesh_destroy(mesh);p.free(mesh);}m.write(self+0x3c,0);p.base_destroy(self);}
Word layout_room_load_00579520(Memory&m,LayoutRoomPorts&p,Word self,Word flags,Word scratch){if(m.read(self+0x38))return 0;p.reference_assign(m.read(self+0x3c)+0xc,scratch+4,self+0x20);return p.mesh_load(m.read(self+0x3c),flags);}
void layout_room_bounds_005796b0(Memory&m,LayoutRoomPorts&p,Word self){if(!m.read(self+0x38))p.mesh_position(m.read(self+0x3c),self+4);}
Word layout_room_overlap_005796d0(Memory&m,LayoutRoomPorts&p,Word self,Word other,Word tolerance,Word scratch){if(m.read(self+0x38)||m.read(other+0x38))return 0;Word x=m.read(other+4),mesh=m.read(other+0x3c);m.write(scratch+4,x);Word y=m.read(other+8),z=m.read(other+12);m.write(scratch+12,z);Word own=m.read(self+0x3c);m.write(scratch+8,y);return p.overlap(own,mesh,tolerance,self+4,scratch+4);}
Word layout_room_connect_00579740(Memory&m,LayoutRoomPorts&p,Word self,Word other,Word ownIndex,Word otherIndex,Word scratch){if(m.read(self+0x38)||m.read(other+0x38))return 0;Word x=m.read(other+4),mesh=m.read(other+0x3c);m.write(scratch+4,x);Word y=m.read(other+8),z=m.read(other+12);m.write(scratch+12,z);Word own=m.read(self+0x3c);m.write(scratch+8,y);return p.connect(own,mesh,ownIndex,otherIndex,self+4,scratch+4);}
void layout_room_finish_00579730(Memory&m,LayoutRoomPorts&p,Word self){if(!m.read(self+0x38))p.finish(m.read(self+0x3c));}
}
