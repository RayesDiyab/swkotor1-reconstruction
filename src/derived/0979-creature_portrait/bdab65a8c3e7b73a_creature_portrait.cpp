#include "d0c22ee5c6928509_creature_portrait.hpp"
namespace kotor::server_mainloop {
void creature_portrait_004ed4c0(Memory&m,CreaturePortraitPorts&p,Word self,Word value,Word s){
 p.literal(s+0x20,0x745e4c);Word id=value&65535;
 if(id>=0xfffe){p.ref_literal(s+0x10,0x73d71c);p.set_ref(m.read(m.read(self)+0xcc),self,s+0x10);}
 else{p.empty(s+8);p.literal(s+0x10,0x745e40);Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x38);(void)p.table_string(table,id,s+0x10,s+8);p.destroy(s+0x10);p.assign(s+8,s+8);Word text=p.text(s+8);p.ref_literal(s+0x10,text);p.set_ref(m.read(m.read(self)+0xcc),self,s+0x10);p.destroy(s+8);}
 m.write16(self+0x30,id);p.destroy(s+0x20);
}
}

namespace kotor::server_mainloop {
void creature_portrait_ref_004f83c0(Memory&m,CreaturePortraitRefPorts&p,Word self,Word ref,Word s){Word stats=m.read(self+0xa74);if(stats){m.write(s,m.read(ref));m.write(s+4,m.read(ref+4));Word a=m.read(ref+8),b=m.read(ref+12);m.write(s+8,a);m.write(s+12,b);p.ref_assign(stats+0x16e,s+0x10,s);}}
}
