#include "73b810dc0bb09fca_item_stack_merge.hpp"
namespace kotor::server_mainloop {
Word item_stack_merge_00553f90(Memory&m,ItemStackMergePorts&p,Word self,Word other){
 if(!p.compatible(self,other))return 0;
 Word id=m.read(self+0xc),base=p.base(m.read(m.read(0x7a3a28)+0x34),id);
 Word otherCount=m.read(other+0x28c),selfCount=m.read(self+0x28c),sum=otherCount+selfCount,maximum=m.read16(base+0x24);
 if(static_cast<int>(sum)>static_cast<int>(maximum)){m.write(self+0x28c,maximum);Word delta=selfCount-m.read16(base+0x24);m.write(other+0x28c,m.read(other+0x28c)+delta);return 0;}
 m.write(self+0x28c,sum);return 1;
}
}
