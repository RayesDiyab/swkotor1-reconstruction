#include "4a59aea4a96a362f_area_content.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word area_layout_005073d0(Memory&m,AreaContentPorts&p,Word self,Word){
 m.write(self+8,m.read(0x7a1b48));m.write(self+12,m.read(0x7a1b4c));
 Word allocated=p.allocate(0x60);Word layout=allocated?p.layout_construct_005de770(allocated):0;
 Word result=p.layout_load_005df140(layout,self+0x10c);
 if(result){m.write(self+0x22c,p.layout_count_005a4c50(layout));m.write(self+0x230,p.layout_extract_00504870(layout));}
 p.layout_release_005de450(layout);if(layout)p.destroy(layout,1);return result;
}
Word area_objects_0050dd80(Memory&m,AreaContentPorts&p,Word self,Word saved,Word scratch){
 Word ref=self+0x10c;
 if(!p.exists_00408bc0(m.read(0x7a39e8),ref,0x7e7,0))return 0;
 Word allocated=p.allocate(0xa0);Word resource=allocated?p.gff_construct_00410630(allocated,0x7e7,0x747b70,ref):0;
 if(!m.read(resource+0x94)){p.destroy(resource,1);p.release_00409b80(m.read(self+0x108));return 0;}
 Word root=scratch+0xc,status=scratch+0x10;p.root_00411240(resource,root);
 if(saved){p.variables_0059aa80(self+0x1e4,resource,root);p.effects_0059b0f0(self+0x1f4,resource,root);
 p.byte(self+0x20c,p.gff_byte_00411a60(resource,root,0x747b60,status,0)&255);
 m.write(self+0x210,p.gff_byte_00411a60(resource,root,0x747b50,status,0)&255);
 if(m.read8(self+4)&1){p.byte(self+0x20c,255);m.write(self+0x210,0);}}
 Word options=p.gff_byte_00411a60(resource,root,0x747c18,status,0)&255;
 for(Word address:{0x504a70u,0x504de0u,0x50a0e0u,0x50a350u,0x505060u,0x505360u,0x505560u,0x50a7b0u,0x5057a0u,0x505af0u})p.group(address,self,resource,root,saved,options);
 p.doors_extra_00507490(self,resource,root);p.extra_00505da0(self,resource,root);p.extra_00505eb0(self,resource,root);p.destroy(resource,1);return 1;
}
}
