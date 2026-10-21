#include "cead353c48ad3510_object_render_link.hpp"
namespace kotor::server_mainloop{
Word object_render_link_004cc2b0(Memory&m,ObjectRenderLinkPorts&p,Word self){
 if(!m.read(self+0x224)){Word id=m.read(self+4),client=m.read(m.read(0x7a39fc)+4);id=p.client_id(client,id);client=m.read(m.read(0x7a39fc)+4);Word o=p.client_object(client,id);if(o){Word r=p.virtual_render(o);m.write(self+0x224,r);if(r)p.back_reference(r,self);}}
 return m.read(self+0x224);
}
Word object_dirty_004cc220(Memory&m,Word self,Word bit){return (m.read(self+0x1fc+(bit>>5)*4)&(Word{1}<<(bit&31)))?1:0;}
void object_clear_dirty_004cc250(Memory&m,Word self,Word bit){Word a=self+0x1fc+(bit>>5)*4;m.write(a,m.read(a)&~(Word{1}<<(bit&31)));}
}
