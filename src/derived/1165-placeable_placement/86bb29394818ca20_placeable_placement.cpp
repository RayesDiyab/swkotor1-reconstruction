#include "cb1525bca24e646e_placeable_placement.hpp"
namespace kotor::server_mainloop {
void placeable_detach_00584950(Memory&m,PlaceablePlacementPorts&p,Word self,Word s){if(p.area(self)){if(static_cast<std::int32_t>(m.read(self+0x344))>=0){Word id=m.read(self+4);Word area=p.area(self);p.remove_bounds(area,id);m.write(self+0x344,0xffffffff);}Word id=m.read(self+4);Word area=p.area(self);p.remove_object(area,id);}p.set_area(self,0);m.write(s+4,0);m.write(s+8,0);m.write(s+0xc,0);p.position(self,s+4,1,1,0);}
void placeable_placement_00587170(Memory&m,PlaceablePlacementPorts&p,Word self,Word area,Word position,Word,Word s){placeable_detach_00584950(m,p,self,s+0x80);if(!area)return;p.set_area(self,area);p.position(self,position,0,1,0);p.string_zero(s+0x14);m.write(s+0x74,area);
 auto table=[&](){Word rules=m.read(0x7a3a28);return m.read(m.read(rules+0xb8)+0x2c);};
 p.string_construct(s+0x1c,0x749820);Word row=p.byte(self+0x230);p.table_integer(table(),row,s+0x1c,s+0x74);p.string_destroy(s+0x1c);m.write(self+0x33c,m.read(s+0x74)==1);
 p.string_construct(s+0x1c,0x747f78);row=p.byte(self+0x230);p.table_integer(table(),row,s+0x1c,s+0x10);p.string_destroy(s+0x1c);m.write(self+0x340,m.read(s+0x10)==1);
 p.string_construct(s+0x1c,0x749814);row=p.byte(self+0x230);p.table_string(table(),row,s+0x1c,s+0x14);p.string_destroy(s+0x1c);p.ref_from_string(s+0x44,s+0x14);p.ref_assign(self+0x3ac,s+0x34,s+0x44);
 Word mesh=self+0x3a0;if(p.mesh_load(mesh,0)){
  Word x=m.read(self+0x90);m.write(self+0x3c0,x);Word y=m.read(self+0x94),z=m.read(self+0x98);m.write(self+0x3c4,y);m.write(self+0x3c8,z);
  m.write(s+0x34,0);m.write(s+0x38,0);m.write(s+0x3c,0);m.write(s+0x28,0);m.write(s+0x2c,0);m.write(s+0x30,0);m.write(s+0x1c,0);m.write(s+0x20,0);m.write(s+0x24,0x3f800000);
  Word angle=p.yaw_scaled(self+0x9c);Word q=p.axis_angle(s+0x54,0,0,m.read(s+0x24),angle);m.write(self+0x3d8,m.read(q));m.write(self+0x3dc,m.read(q+4));m.write(self+0x3e0,m.read(q+8));m.write(self+0x3e4,m.read(q+0xc));p.bounds(mesh,s+0x34,s+0x28);
  if(m.read(0x832854))p.debug(mesh,0,0xffff0000,0xff00ff00);
  Word id=m.read(self+4);m.write(self+0x344,p.register_bounds(area,id,s+0x34,s+0x28));
 }
 Word id=m.read(self+4);p.insert(p.area(self),id,1);p.string_destroy(s+0x14);
}
}
