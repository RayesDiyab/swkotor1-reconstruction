#include "4a4e550950219e3c_creature_stats_owner.hpp"
namespace kotor::server_mainloop {
Word creature_stats_owner_005b024c(Memory&m,CreatureStatsOwnerPorts&p,Word self,Word gff,Word root,Word out,Word s,Word owner){
 m.write16(owner+0x9d8,p.short_read(gff,root,0x74aef4,s+0x10,m.read16(owner+0x9d8)));
 owner=m.read(self+0x24);if(!m.read(owner+0xa88)){Word fallback=p.owner_value(owner);p.owner_set(owner,p.dword_read(gff,root,0x748248,s+0x10,fallback));}
 Word value=p.byte_read(gff,root,0x745368,s+0x10,m.read(m.read(self+0x24)+0xf8))&255;if(!m.read(s+0x10))value=p.byte_read(gff,root,0x748f64,s+0x10,m.read(m.read(self+0x24)+0xf8))&255;m.write(m.read(self+0x24)+0xf8,value);
 value=p.byte_read(gff,root,0x7497c0,s+0x10,m.read(m.read(self+0x24)+0x200))&255;m.write(m.read(self+0x24)+0x200,value);
 value=p.byte_read(gff,root,0x7497b0,s+0x10,m.read(m.read(self+0x24)+0x218))&255;m.write(m.read(self+0x24)+0x218,value);
 owner=m.read(self+0x24);Word fallback=(owner&0xffffff00)|(m.read(owner+0x21c)==0?1:0);value=p.byte_read(gff,root,0x74aee4,s+0x10,fallback)&255;m.write(m.read(self+0x24)+0x21c,value?0:1);
 value=p.byte_read(gff,root,0x74aed8,s+0x10,m.read(m.read(self+0x24)+0x4f4))&255;m.write(m.read(self+0x24)+0x4f4,value);
 Word experience=p.dword_read(gff,root,0x745378,s+0x10,m.read(self+0x68));p.set_experience(self,experience,0);
 return p.continuation_005b03b4(self,gff,root,out,s);
}
}
