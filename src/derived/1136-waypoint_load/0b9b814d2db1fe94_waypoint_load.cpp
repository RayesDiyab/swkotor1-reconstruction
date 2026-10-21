#include "980344118a740873_waypoint_load.hpp"
namespace kotor::server_mainloop {
Word waypoint_load_005c7f30(Memory&m,WaypointLoadPorts&p,Word self,Word g,Word root,Word s){
 m.write(s+0x4c,0);m.write(s+0x50,0);m.write(s+0x54,0);m.write(s+0x40,0);m.write(s+0x44,0);m.write(s+0x48,0);p.localized_construct(s+0x28);
 p.string_empty(s+0x1c);Word r=p.string_read(g,s+0x14,root,0x747a28,s+0x10,s+0x1c);p.tag(self,r);p.string_destroy(s+0x14);p.string_destroy(s+0x1c);
 r=p.localized_construct(s+0x14);r=p.localized_read(g,s+0x1c,root,0x748fec,s+0x24,r);p.localized_assign(self+0x238,r);p.localized_destroy(s+0x1c);p.localized_destroy(s+0x14);
 m.write(s+0x4c,p.floating(g,root,0x745d80,s+0x10,0));m.write(s+0x50,p.floating(g,root,0x745d74,s+0x10,0));m.write(s+0x54,p.floating(g,root,0x745d68,s+0x10,0));
 m.write(s+0x40,p.floating(g,root,0x745d38,s+0x10,0));m.write(s+0x44,p.floating(g,root,0x745d48,s+0x10,0));m.write(s+0x48,p.floating(g,root,0x745d58,s+0x10,0));p.normalize(s+0x40);p.position(self,s+0x4c);p.orientation(self,s+0x40);
 Word value=p.byte(g,root,0x74c078,s+0x10,0)&255;Word status=m.read(s+0x10);m.write(s+0x14,value);
 if(status&&value){m.write(s+0x1c,p.byte(g,root,0x74c068,s+0x10,0)&255);r=p.localized_construct(s+0x38);r=p.localized_read(g,s+0x30,root,0x74c060,s+0x10,r);p.localized_assign(s+0x28,r);p.localized_destroy(s+0x30);p.localized_destroy(s+0x38);
  if(m.read(s+0x10)){Word enabled=m.read(s+0x1c),note=m.read(s+0x14);m.write(self+0x22c,enabled);m.write(self+0x228,note);p.localized_assign(self+0x230,s+0x28);}
 }
 Word id=m.read(self+4);Word server=m.read(m.read(0x7a39fc)+8);p.register_tag(p.module(server),self+0x18,id);p.localized_destroy(s+0x28);return 1;
}
}
