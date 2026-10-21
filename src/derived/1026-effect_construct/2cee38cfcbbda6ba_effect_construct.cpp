#include "1a569dafa81b778a_effect_construct.hpp"
namespace kotor::server_mainloop {
Word effect_construct_00503e40(Memory&m,EffectConstructPorts&p,Word self,Word mode){
 for(Word i=0;i<6;++i)p.empty(self+0x48+i*8);
 if(mode==1){m.write(self,m.read(0x7a1b40));m.write(self+4,m.read(0x7a1b44));Word low=m.read(0x7a1b40),high=m.read(0x7a1b44);Word next=low+1;m.write(0x7a1b40,next);m.write(0x7a1b44,high+(next<low));}else{m.write(self,0);m.write(self+4,0);}
 m.write(self+0x20,mode);m.write(self+0x88,0);m.write16(self+8,0);m.write16(self+0xa,0);m.write(self+0xc,0);m.write(self+0x10,0);m.write(self+0x14,0);m.write(self+0x18,0x7f000000);m.write(self+0x28,0);m.write(self+0x2c,0);m.write(self+0x24,0xffffffff);m.write(self+0x1c,0xffffffff);m.write(self+0x34,0);m.write(self+0x30,8);
 Word data=p.allocate(32),bytes=m.read(self+0x30)*4;m.write(self+0x34,data);p.zero(data,bytes);for(Word i=0;i<4;++i)m.write(self+0x38+i*4,0);for(Word i=0;i<6;++i)p.literal(self+0x48+i*8,0x73d71c);for(Word i=0;i<4;++i)m.write(self+0x78+i*4,0x7f000000);return self;
}
void effect_integer_005036a0(Memory&m,Word self,Word index,Word value){Word data=m.read(self+0x34);m.write(data+index*4,value);}
}
