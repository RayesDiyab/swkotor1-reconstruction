#include "4dec0c961ea46a76_creature_template_final.hpp"
namespace kotor::server_mainloop {
void creature_template_final_004ed850(Memory&m,CreatureTemplateFinalPorts&p,Word self,Word movement,Word s){
 auto table=[&](){return m.read(m.read(m.read(0x7a3a28)+0xb8)+0xc);};
 p.literal(s+0x20,0x746cd4);Word t=table(),row=m.read16(self+0xa60);p.named_integer(t,row,s+0x20,s+8);p.destroy(s+0x20);m.write(self+0x4f0,m.read(s+8));
 p.literal(s+0x20,0x746ccc);t=table();row=m.read16(self+0xa60);p.named_integer(t,row,s+0x20,s+8);p.destroy(s+0x20);m.write(self+0x4ec,m.read(s+8));
 Word rules=m.read(0x7a3a28),column=m.read(0x7a20d4);t=m.read(m.read(rules+0xb8)+0xc);row=m.read16(self+0xa60);p.integer(t,row,column,s+0x10);m.write(self+0xac4,m.read(s+0x10));
 rules=m.read(0x7a3a28);column=m.read(0x7a2124);t=m.read(m.read(rules+0xb8)+0xc);row=m.read16(self+0xa60);p.integer(t,row,column,s+0x14);m.write(self+0x4f8,m.read(s+0x14));
 p.empty(s+0x18);t=table();column=m.read(0x7a2110);row=m.read16(self+0xa60);p.string(t,row,column,s+0x18);p.assign(self+0x4c8,s+0x18);
 p.literal(s+0x20,0x746cc0);t=table();row=m.read16(self+0xa60);Word status=p.named_float(t,row,s+0x20,s+0xc);p.destroy(s+0x20);m.write(self+0x500,status==1?m.read(s+0xc):0x3f800000);
 if(movement)p.movement(m.read(self+0xa74),7);
 p.literal(s+0x28,0x746cb0);t=table();row=m.read16(self+0xa60);p.named_float(t,row,s+0x28,s+0xc);p.destroy(s+0x28);m.write(self+0x4fc,m.read(s+0xc));m.write(self+0x344,1);p.destroy(s+0x18);
}
}
