#include "2fd0d74c8d8b75b7_creature_stats_load.hpp"
namespace kotor::server_mainloop {
Word creature_stats_identity_005afce0(Memory&m,CreatureStatsPorts&p,Word self,Word gff,Word root,Word out,Word s){
 (void)p.loc_construct(s+0x4c);Word fallback=p.loc_construct(s+0x3c);
 Word value=p.loc_read(gff,s+0x28,root,0x749394,s+0x10,fallback);p.loc_assign(s+0x4c,value);p.loc_destroy(s+0x28);p.loc_destroy(s+0x3c);p.loc_assign(self+0x34,s+0x4c);
 fallback=p.loc_construct(s+0x3c);value=p.loc_read(gff,s+0x28,root,0x749388,s+0x10,fallback);p.loc_assign(s+0x4c,value);p.loc_destroy(s+0x28);p.loc_destroy(s+0x3c);p.loc_assign(self+0x3c,s+0x4c);
 value=p.loc_read(gff,s+0x28,root,0x7488c0,s+0x10,self+0x58);p.loc_assign(self+0x58,value);p.loc_destroy(s+0x28);
 Word flag=p.byte_read(gff,root,0x74af60,s+0x10,m.read(self+0x6c))&255;Word tag=m.read(self+0x24)+0x18;m.write(self+0x6c,flag);
 value=p.string_read(gff,s+0x28,root,0x747a28,s+0x10,tag);p.set_tag(m.read(self+0x24),value);p.string_destroy(s+0x28);
 Word a=m.read(self+0x44),b=m.read(self+0x48);m.write(s+0x28,a);Word c=m.read(self+0x4c);m.write(s+0x2c,b);Word d=m.read(self+0x50);m.write(s+0x30,c);m.write(s+0x34,d);
 value=p.ref_read(gff,s+0x3c,root,0x7496b8,s+0x10,s+0x28);p.ref_assign(self+0x44,s+0x100,value);
 m.write(self+0x54,p.byte_read(gff,root,0x74af50,s+0x10,m.read(self+0x54))&255);Word integer=p.integer_read(gff,root,0x7488f4,s+0x10,m.read(self+0x60));Word low=m.read8(self+0x64);m.write(self+0x60,integer);p.byte_write(s+0x14,low);
 flag=p.byte_read(gff,root,0x74af48,s+0x10,m.read(s+0x14))&255;if(flag>4)flag=4;low=m.read8(self+0x88);p.byte_write(self+0x64,flag);p.byte_write(s+0x14,low);p.byte_write(self+0x88,p.byte_read(gff,root,0x74af38,s+0x10,m.read(s+0x14)));
 Word race=p.byte_read(gff,root,0x74af30,s+0x10,m.read16(self+0xdc))&255;Word max=m.read8(m.read(0x7a3a28)+0xaa);if(race>=max){p.loc_destroy(s+0x4c);return 0x5f4;}m.write16(self+0xdc,race);
 return p.continuation_005affbe(self,gff,root,out,s);
}
}
