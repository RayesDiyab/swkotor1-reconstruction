#include "c48c0aa5a96c08f2_room_mesh_binary.hpp"
namespace kotor::server_mainloop {
static Word scalar(Memory&m,Word resource,Word offset){Word h=m.read(resource+0x34);return h?m.read(h+offset):0;}
static Word pointer(Memory&m,Word resource,Word offset){Word h=m.read(resource+0x34);if(!h)return 0;Word relative=m.read(h+offset);return relative+m.read(resource+0x30);}
static void vector(Memory&m,Word resource,Word offset,Word destination){Word h=m.read(resource+0x34);for(Word i=0;i<12;i+=4)m.write(destination+i,h?m.read(h+offset+i):0);}
Word room_mesh_binary_base_00597120(Memory&m,Word self,Word resource){m.write(self+0x4c,1);m.write(self+4,scalar(m,resource,8));vector(m,resource,0xc,self+0x70);vector(m,resource,0x18,self+0x7c);m.write(self+0x50,scalar(m,resource,0x48));vector(m,resource,0x3c,self+0x2c);m.write(self+0x54,pointer(m,resource,0x4c));m.write(self+0x58,scalar(m,resource,0x50));m.write(self+0x5c,scalar(m,resource,0x70));m.write(self+0x60,pointer(m,resource,0x54));m.write(self+0x64,pointer(m,resource,0x58));m.write(self+0x68,pointer(m,resource,0x5c));Word h=m.read(resource+0x34);Word final=0;if(h){Word base=m.read(resource+0x30);final=m.read(h+0x60)+base;}m.write(self+0x6c,final);return 1;}
Word room_mesh_binary_005807c0(Memory&m,Word self,Word resource){if(!room_mesh_binary_base_00597120(m,self,resource))return 0;m.write(self+0x88,pointer(m,resource,0x74));m.write(self+0xd4,scalar(m,resource,0x6c));
 const Word offsets[]={0x78,0x64,0x80},lists[]={0x8c,0xac,0x9c};
 for(unsigned i=0;i<3;++i){Word h=m.read(resource+0x34);Word count=h?m.read(h+offsets[i]):0;Word data=h?m.read(h+offsets[i]+4)+m.read(resource+0x30):0;Word list=self+lists[i];if(!m.read(list+4)){m.write(list,data);m.write(list+4,count);m.write(list+8,count);m.write(list+12,1);}}
 return 1;}
}
