#include "c1c89069a31e8b0a_area_load.hpp"
namespace kotor::server_mainloop {
Word area_load_0050e190(Memory&m,AreaLoadPorts&p,Word self,Word saved,Word scratch){
 Word demanded=p.demand_00409b10(m.read(self+0x108));if(!demanded)return demanded;
 p.root_00411240(m.read(self+0x108),scratch+4);
 p.properties_00508c50(self,scratch+4);p.tiles_005073d0(self,scratch+4);
 p.objects_0050dd80(self,saved);p.scripts_00508400(self,scratch+4);
 p.release_00409b80(m.read(self+0x108));
 Word id=m.read(self+0x120),server=m.read(m.read(0x7a39fc)+8);
 Word module=p.module_004ae6b0(server);p.register_tag_004c7de0(module,self+0x158,id);
 Word height=m.read(self+12),width=m.read(self+8);
 Word product=width*height;
 p.byte(self+0x223,p.numeric_tail(product,m.read(0x73f400))&255);return 1;
}
}
