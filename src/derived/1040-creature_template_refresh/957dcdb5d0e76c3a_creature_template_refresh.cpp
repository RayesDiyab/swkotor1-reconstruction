#include "989c734697fa1772_creature_template_refresh.hpp"
namespace kotor::server_mainloop {
void creature_template_refresh_004ed6e0(Memory&m,CreatureTemplateRefreshPorts&p,Word self,Word s){
 auto table=[&](){return m.read(m.read(m.read(0x7a3a28)+0xb8)+0xc);};m.write(s,0x3f19999a);
 Word t=table(),column=m.read(0x7a2118),row=m.read16(self+0xa60);Word status=p.table_float(t,row,column,s);Word collision=m.read(self+0x340);m.write(collision+4,status==1?m.read(s):0x3f19999a);p.collision_refresh(collision);
 t=table();column=m.read(0x7a20d0);row=m.read16(self+0xa60);p.table_float(t,row,column,s);collision=m.read(self+0x340);m.write(collision+8,m.read(s));
 t=table();column=m.read(0x7a20cc);row=m.read16(self+0xa60);status=p.table_float(t,row,column,s);collision=m.read(self+0x340);if(status==1)m.write(collision+0x10,m.read(s));else p.float_copy(collision+0x10,collision+8);
 t=table();column=m.read(0x7a2108);row=m.read16(self+0xa60);status=p.table_float(t,row,column,s);collision=m.read(self+0x340);m.write(collision+0x18,status==1?m.read(s):0x3f000000);
 t=table();column=m.read(0x7a2104);row=m.read16(self+0xa60);status=p.table_float(t,row,column,s);collision=m.read(self+0x340);m.write(collision+0x14,status==1?m.read(s):0x3f800000);
}}
