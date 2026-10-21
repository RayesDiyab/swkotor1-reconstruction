#include "7bc6168ef05aea73_room_walkmesh.hpp"
namespace kotor::server_mainloop {
Word room_mesh_base_00596b80(Memory&m,RoomWalkmeshPorts&p,Word self,Word type,Word alternate){
 m.write(self,p.vtable(0x74a068));p.ref_default(self+0xc);
 const Word first[]={0x20,0x24,0x28,0x2c,0x30,0x34,0x44,0x40,0x3c};for(Word o:first)m.write(self+o,0);m.write(self+0x38,0x3f800000);
 for(Word o=0x70;o<0x88;o+=4)m.write(self+o,0);
 const Word last[]={0x4c,4,0x1c,0x50,0x54,0x58,0x5c,0x60,0x64,0x68,0x6c};for(Word o:last)m.write(self+o,0);m.write16(self+0x48,alternate);m.write16(self+0x4a,type);return self;
}
Word room_mesh_construct_00582ae0(Memory&m,RoomWalkmeshPorts&p,Word self,Word s){
 room_mesh_base_00596b80(m,p,self,0x7d2,0x7e0);m.write(self,p.vtable(0x74944c));
 const Word initial[]={0x90,0x94,0x8c,0x98,0xa0,0xa4,0x9c,0xa8,0xb0,0xb4,0xac,0xb8,0xc0,0xc4,0xbc,0xcc,0xd0,0xc8,0x88};for(Word o:initial)m.write(self+o,0);
 m.write(s+0x3c,0);m.write(s+0x40,0);m.write(s+0x38,0);m.write(s+0x44,0);p.list_copy(self+0xac,s+0x38);m.write(self+0xb8,0);
 for(Word o=0xd8;o<=0xe4;o+=4)m.write(self+o,0);
 for(Word row=0;row<32;++row){
  p.empty(s+0x38);p.literal(s+0x18,0x749490);
  p.table_string(m.read(m.read(m.read(0x7a3a28)+0xb8)+0x14),row,s+0x18,s+0x38);p.destroy(s+0x18);
  const Word labels[]={0x749484,0x747670,0x749478};
  for(Word field=0;field<3;++field){Word temp=s+0x20+field*8;p.literal(temp,labels[field]);p.table_integer(m.read(m.read(m.read(0x7a3a28)+0xb8)+0x14),row,temp,s+0x10);p.destroy(temp);if(m.read(s+0x10)==1)m.write(self+0xd8+field*4,m.read(self+0xd8+field*4)+(Word(1)<<row));}
  m.write(self+0xe4,m.read(self+0xe4)+(Word(1)<<row));p.destroy(s+0x38);
 }
 return self;
}
}
