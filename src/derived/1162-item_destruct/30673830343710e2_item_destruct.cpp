#include "a631a6c367649e0a_item_destruct.hpp"
namespace kotor::server_mainloop {
void item_destruct_0055ec70(Memory&m,ItemDestructPorts&p,Word self,Word s){Word object=self+0x10;p.tables(self);Word id=m.read(self+0x14);Word manager=m.read(0x7a39fc);Word client=m.read(manager+4);Word key=p.client_id(client,id);manager=m.read(0x7a39fc);client=m.read(manager+4);Word item=p.client_item(client,key);if(item){manager=m.read(0x7a39fc);client=m.read(manager+4);Word objects=p.client_objects(client);if((p.remove_client_object(objects,key,s)&0xff)==0&&m.read(s))p.destroy(item,1);}
 manager=m.read(0x7a39fc);Word server=m.read(manager+8);Word module=p.module(server);if(module){id=m.read(self+0x14);p.remove_tag(module,self+0x28,id);}
 Word inventory=m.read(self+0x26c);if(inventory){p.release(m.read(inventory+0xc));m.write(inventory+0xc,0);p.release(inventory);m.write(self+0x26c,0);}Word a=m.read(self+0x250);if(a){p.release(a);m.write(self+0x250,0);}a=m.read(self+0x254);if(a){p.release(a);m.write(self+0x254,0);}if(p.area(object))p.detach(self);
 manager=m.read(0x7a39fc);server=m.read(manager+8);Word master=p.master(server);p.remove_ai(master,object);p.localized_destroy(self+0x280);p.localized_destroy(self+0x278);p.localized_destroy(self+0x270);p.object_destroy(object);p.base_destroy(self);
}
}
