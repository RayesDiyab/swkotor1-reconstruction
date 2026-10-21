#include "c1e3f9bcd477335b_item_stack_compatible.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word item_stack_property(Memory&m,Word self,Word index,Word group){Word offset=group?0x24c:0x248;if(static_cast<int>(index)<0||static_cast<int>(index)>=static_cast<int>(m.read(self+offset)))return 0;return m.read(self+offset+8)+index*28;}
Word item_stack_compatible_00553cf0(Memory&m,ItemStackCompatiblePorts&p,Word self,Word other){
 for(Word offset:{0x294u,0x248u,0x24cu,0x108u,0x258u,0xcu})if(m.read(self+offset)!=m.read(other+offset))return 0;
 if((m.read(other+0x288)^m.read(self+0x288))&0x20)return 0;
 for(Word offset=0x264;offset<=0x266;++offset)if(m.read8(self+offset)!=m.read8(other+offset))return 0;
 if(p.string_different(self+0x28,other+0x28)||p.localized_different(self+0x280,other+0x280))return 0;
 for(unsigned pass=0;pass<3;++pass){Word offset=pass?0x24c:0x248,index=0;if(static_cast<int>(m.read(self+offset))<=0)continue;
  do{Word a=p.property(pass?0x553990:0x553960,self,index),b=p.property(pass?0x553990:0x553960,other,index);
   if(m.read16(a)!=m.read16(b)||m.read16(a+2)!=m.read16(b+2)||m.read8(a+4)!=m.read8(b+4)||m.read16(a+6)!=m.read16(b+6)||m.read8(a+8)!=m.read8(b+8)||m.read8(a+9)!=m.read8(b+9)||m.read8(a+0x10)!=m.read8(b+0x10)||m.read8(a+0x11)!=m.read8(b+0x11))return 0;
   ++index;
  }while(static_cast<int>(index)<static_cast<int>(m.read(self+offset)));
 }
 return 1;
}
}
