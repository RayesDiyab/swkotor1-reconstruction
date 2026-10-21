#include "bc84e41e97b142a1_item_weight.hpp"
namespace kotor::server_mainloop {
Word item_weight_005562a0(Memory&m,ItemWeightPorts&p,Word self){Word inventory=m.read(self+0x26c);if(inventory){Word total=inventory_weight_00556100(m,p,inventory);return total+m.read(self+0x290);}Word count=m.read(self+0x28c),weight=m.read(self+0x290);return static_cast<int>(count)>1?weight*count:weight;}
Word inventory_weight_00556100(Memory&m,ItemWeightPorts&p,Word self){Word count=m.read(self+0x10),total=0,index=0;if(static_cast<int>(count)<=0)return 0;do{if(static_cast<int>(index)>=0&&static_cast<int>(index)<static_cast<int>(count)){Word app=m.read(0x7a39fc),data=m.read(self+0xc),server=m.read(app+8),id=m.read(data+index*4);Word item=p.item(server,id);if(item)total+=item_weight_005562a0(m,p,item);}count=m.read(self+0x10);++index;}while(static_cast<int>(index)<static_cast<int>(count));return total;}
}
