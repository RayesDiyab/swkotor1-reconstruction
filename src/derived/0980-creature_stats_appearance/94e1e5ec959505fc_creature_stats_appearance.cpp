#include "dd38a48845890fda_creature_stats_appearance.hpp"
namespace kotor::server_mainloop {
static void copy_ref(Memory&m,Word from,Word to){Word a=m.read(from),b=m.read(from+4);m.write(to,a);Word c=m.read(from+8);m.write(to+4,b);Word d=m.read(from+12);m.write(to+8,c);m.write(to+12,d);}
Word creature_stats_appearance_005b03b4(Memory&m,CreatureStatsAppearancePorts&p,Word self,Word gff,Word root,Word out,Word s){Word value=p.short_read(gff,root,0x74937c,s+0x10,0xffff);Word owner=m.read(self+0x24),callback=m.read(m.read(owner)+0xd4);bool fallback=(value&65535)>=0xfffe;p.portrait(callback,owner,value);
 if(fallback){Word ref=self+0x16e;copy_ref(m,ref,s+0x28);Word result=p.ref_read(gff,s+0x100,root,0x749370,s+0x10,s+0x28);copy_ref(m,result,s+0x3c);p.ref_assign(ref,s+0x9c,s+0x3c);copy_ref(m,ref,s+0x28);p.ref_text(s+0x28);}
 value=p.byte_read(gff,root,0x74a49c,s+0x10,m.read16(self+0x17e))&255;if(value>100)value=100;p.byte_write(s+0x14,m.read8(self+0x182));m.write16(self+0x17e,value);
 const Word labels[]={0x74aecc,0x74aec0,0x74aeb0,0x74aea0};for(unsigned i=0;i<4;++i){value=p.byte_read(gff,root,labels[i],s+0x10,m.read(s+0x14));if(i<3)p.byte_write(s+0x14,m.read8(self+0x183+i));Word next= i==3?m.read8(self+0x188):0;p.byte_write(self+0x182+i,value);if(i==3)value=p.integer_read(gff,root,0x74ae94,s+0x10,next);}
 p.byte_write(self+0x188,value);value=p.short_read(gff,root,0x74ae84,s+0x10,m.read16(self+0x186));p.byte_write(s+0x14,m.read8(self+0x189));m.write16(self+0x186,value);
 value=p.byte_read(gff,root,0x74ae74,s+0x10,m.read(s+0x14))&255;p.byte_write(self+0x189,value);if(!value)value=1;p.byte_write(s+0x14,m.read8(self+0x18a));p.byte_write(self+0x189,value);p.byte_write(self+0x18a,p.byte_read(gff,root,0x74ae64,s+0x10,m.read(s+0x14)));
 value=p.byte_read(gff,root,0x74ae54,s+0x10,m.read(self+0x18c))&255;Word prior=m.read(self+0x78);m.write(self+0x18c,value);p.byte_write(self+0x190,0);p.byte_write(self+0x191,0);value=p.short_read(gff,root,0x74ae48,s+0x10,prior)&65535;prior=m.read(self+0x84);m.write(self+0x78,value);m.write(s+0x14,prior);value=p.float_bits(gff,root,0x74ae38,s+0x10,prior);Word next=m.read16(self+0x13a);m.write(self+0x84,value);value=p.integer_read(gff,root,0x74ae30,s+0x10,next);owner=m.read(self+0x24);m.write16(self+0x13a,value);p.byte_write(s+0x14,m.read8(owner+0x9da));p.byte_write(owner+0x9da,p.byte_read(gff,root,0x7494c8,s+0x10,m.read(s+0x14)));
 return p.continuation_005b06b8(self,gff,root,out,s);
}
}
