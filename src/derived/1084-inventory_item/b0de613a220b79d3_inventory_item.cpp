#include "9c94c3152200c103_inventory_item.hpp"
namespace kotor::server_mainloop {
Word inventory_item_005560b0(Memory&m,InventoryItemPorts&p,Word self,Word index){
 if(static_cast<int>(index)<0||static_cast<int>(index)>=static_cast<int>(m.read(self+0x10)))return 0;
 Word app=m.read(0x7a39fc),array=m.read(self+0xc),server=m.read(app+8),id=m.read(array+index*4);
 return p.item(server,id);
}
}
