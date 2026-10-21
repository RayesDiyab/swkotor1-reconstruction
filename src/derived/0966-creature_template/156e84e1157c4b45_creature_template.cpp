#include "219e50023d62f88f_creature_template.hpp"
namespace kotor::server_mainloop {
Word creature_template_005026d0(Memory&m,CreatureTemplatePorts&p,Word self,Word reference,Word flags,Word s){
 Word allocation=p.allocate(0xa0);Word resource=allocation?p.gff_construct(allocation,0x7eb,0x747368,reference):0;
 if(!m.read(resource+0x94)){p.destroy(resource,1);p.empty(s+0x14);Word text=p.ref_text(reference);p.format(s+0x14,0x747340,text);allocation=p.allocate(0xa0);resource=0;if(allocation){p.ref_literal(s+0x34,0x747334);resource=p.gff_construct(allocation,0x7eb,0x747368,s+0x34);}if(!m.read(resource+0x94)){p.destroy(resource,1);p.string_destroy(s+0x14);return 0;}p.string_destroy(s+0x14);}
 Word root=s+0xc,status=s+0x10;p.root(resource,root);if(p.stats_header(m.read(self+0xa74),resource,root,self+0xa50)){p.destroy(resource,1);return 0;}
 p.appearance(self,resource,root);p.fields(self,resource,root,0,1,flags);p.stats(m.read(self+0xa74),resource,root);
 Word flag=p.byte(resource,root,0x747260,status,0)&255;m.write(self+0xa48,flag);if(flag)m.write16(self+0xa4c,p.short_value(resource,root,0x747250,status,0));
 Word stealth=p.byte(resource,root,0x7472d0,status,0)&255;
 if(!(m.read8(self+0xa00)&1)){Word bits=m.read(self+0x9fc),changed;if(stealth==1){changed=(~bits)&stealth;bits|=stealth;}else{changed=bits&1;bits&=~Word(1);}m.write(self+0x9fc,bits);if(changed){if(stealth==1&&!(bits&12))p.stealth(self,stealth,0);else if(stealth==0)p.stealth(self,0,0);else m.write(self+0x9fc,m.read(self+0x9fc)&~Word(1));}}
 p.object_fields(self,resource,root);for(Word o=0x28;o<=0x30;o+=4)m.write(s+o,0);for(Word o=0x1c;o<=0x24;o+=4)m.write(s+o,0);
 const Word positionLabels[]={0x745d80,0x745d74,0x745d68},orientationLabels[]={0x745d38,0x745d48,0x745d58};
 for(unsigned i=0;i<3;++i)m.write(s+0x28+i*4,p.float_bits(resource,root,positionLabels[i],status,0));
 for(unsigned i=0;i<3;++i)m.write(s+0x1c+i*4,p.float_bits(resource,root,orientationLabels[i],status,0));
 m.write(self+0x22c,p.integer(resource,root,0x7472e8,status,0));p.position(self,s+0x28,1,1,0);
 if(p.orientation_nonzero(m.read(s+0x1c),m.read(s+0x20),m.read(s+0x24)))p.orientation(self,s+0x1c);
 p.finish(self);p.destroy(resource,1);return 1;
}
}
