#include "0147cb20a5891e8c_client_loadbar.hpp"
namespace kotor::server_mainloop {
Word message_read_word_004d6780(Memory&m,Word self,Word){Word position=m.read(self+0x30),end=position+4,size=m.read(self+0x2c);if(end>size){m.write(self+0x30,end);return 0;}Word value=m.read(m.read(self+0x28)+position);m.write(self+0x30,end);return value;}
void begin_loading_004b13e0(Memory&m,Word self,Word kind){m.write(self,1);m.write(self+4,kind);m.write(self+0x10,1);m.write(self+0x38,0);}
Word client_loadbar_0064edd0(Memory&m,LoadbarPorts&p,Word self,Word subtype,Word scratch){if(p.overrun(self,1))return 0;Word kind=subtype&255;if(kind<1||kind>3)return 0;Word first=p.read_word(self,32),second=0;if(kind>1)second=p.read_word(self,kind==2?32:3);if(p.overrun(self,1)||p.underrun(self,1))return 0;Word app=m.read(0x7a39fc),outer=m.read(app+4),inner=m.read(outer+4);
if(kind==3){if(m.read(inner+0x240)==1){m.write(inner+0x240,0);return 1;}m.write(m.read(outer+4)+0x264,second);if(m.read(m.read(0x7a39fc)+8))return 1;if(first==2){p.empty_literal(scratch+0x10);p.event(m.read(m.read(0x7a39fc)+4),0xe2,0,scratch+0x10);p.destroy(scratch+0x10);}m.write(m.read(m.read(0x7a39fc)+0x14)+0x14,1);return 1;}
if(kind==2){if(m.read(inner+0x240)==1||m.read(app+8))return 1;m.write(m.read(app+0x14)+8,first);if(second!=0xffffffff)m.write(m.read(m.read(0x7a39fc)+0x14)+12,second);return 1;}
if(m.read(inner+0x188)&&first==2){m.write(m.read(outer+4)+0x240,1);return 1;}if(m.read(app+8))return 1;if(first==2){p.empty_literal(scratch+0x18);p.event(m.read(m.read(0x7a39fc)+4),0xe1,0,scratch+0x18);p.destroy(scratch+0x18);app=m.read(0x7a39fc);}p.begin(m.read(app+0x14),first);m.write(m.read(m.read(0x7a39fc)+0x14)+8,0);m.write(m.read(m.read(0x7a39fc)+0x14)+12,0);return 1;}
}
