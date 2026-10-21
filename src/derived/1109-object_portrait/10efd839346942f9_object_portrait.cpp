#include "2380a02c58326c3b_object_portrait.hpp"
namespace kotor::server_mainloop {
void object_portrait_004cad20(Memory&m,ObjectPortraitPorts&p,Word self,Word value,Word s){
 p.literal(s+0x20,0x745e4c);Word id=value&65535;
 if(id>=0xfffe){p.ref_literal(s+0x10,0x73d71c);p.set_ref(m.read(m.read(self)+0xcc),self,s+0x10);}
 else{p.empty(s+8);p.literal(s+0x10,0x745e40);Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0x38);(void)p.table_string(table,id,s+0x10,s+8);p.destroy(s+0x10);Word out=p.concat(s+0x20,s+0x10,s+8);p.assign(s+8,out);p.destroy(s+0x10);Word text=p.text(s+8);p.ref_literal(s+0x10,text);p.set_ref(m.read(m.read(self)+0xcc),self,s+0x10);p.destroy(s+8);}
 m.write16(self+0x30,id);p.destroy(s+0x20);
}
void object_portrait_ref_004d01f0(CreaturePortraitRefPorts&p,Word self,Word ref,Word scratch){p.ref_assign(self+0x20,scratch,ref);}
}
