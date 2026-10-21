#include "a3254fefbb419fe5_door_load.hpp"
namespace kotor::server_mainloop {
Word door_load_0058c5f0(Memory&m,DoorLoadPorts&p,Word self,Word gff,Word root,Word templates,Word s){Word result=1,status=s+0x78;
 if(templates){p.ref_empty(s+0x20);p.ref_read(gff,s+0x30,root,0x747494,status,s+0x20);if(!m.read(status)||!p.load_template(self,s+0x30))return 0;
  Word out=p.localized(gff,s+0x18,root,0x747ab0,status,self+0x3c8);p.localized_assign(self+0x3c8,out);p.localized_destroy(s+0x18);
  p.string_empty(s+0x18);out=p.string_read(gff,s+0x20,root,0x747aa4,status,s+0x18);p.assign(self+0x388,out);p.destroy(s+0x20);p.destroy(s+0x18);
  p.byte_write(self+0x384,p.byte_read(gff,root,0x747a94,status));p.string_zero(s+0x10);p.ref_empty(s+0x40);p.ref_read(gff,s+0x50,root,0x747ac8,status,s+0x40);p.ref_string(s+0x50,s+0x10);p.assign(self+0x390,s+0x10);p.destroy(s+0x10);
 }else result=p.fields(self,gff,root,0);
 if(result){Word id=m.read(self+4);Word server=m.read(m.read(0x7a39fc)+8);Word module=p.module(server);p.tag(module,self+0x18,id);}return result;
}
}
