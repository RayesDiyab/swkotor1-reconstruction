#include "fb3ffdb0a0986859_creature_add_feat.hpp"
namespace kotor::server_mainloop {
void creature_add_feat_005aa810(Memory&m,CreatureAddFeatPorts&p,Word self,Word id){Word info=p.feat_info(m.read(0x7a3a28),id);if(!info)return;p.add_unique(self,id);if(!m.read8(info+0x33))return;Word count=m.read(self+0x10),i=0;while(static_cast<int>(i&65535)<static_cast<int>(count)){Word entry=m.read(m.read(self+0x0c)+(i&65535)*4);if(m.read16(entry)==(id&65535))return;++i;}Word entry=p.allocate(4);m.write16(entry,id);p.byte_write(entry+2,0);Word list=self+0x0c,capacity=m.read(list+8),live=m.read(list+4);if(live==capacity)p.grow(list,capacity?capacity*2:16);live=m.read(list+4);m.write(m.read(list)+live*4,entry);m.write(list+4,m.read(list+4)+1);}
}
