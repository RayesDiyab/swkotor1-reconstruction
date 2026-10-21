#include "5d10545fb55ed0f3_creature_effect_skill.hpp"
namespace kotor::server_mainloop {
Word creature_effect_skill_004f3fe0(Memory&m,CreatureEffectSkillPorts&p,Word self,Word out,Word skill){
 m.write(out,0);Word round=m.read(self+0x9c8);Word attack=m.read8(round+0x96c);Word record=round+4+attack*0x14c;(void)m.read8(record+0x3a);
 for(Word offset=0x5f8;offset<0x688;offset+=4){m.write(self+offset-0x90,0xffffffff);m.write(self+offset,0xffffffff);m.write(self+offset+0x90,0xffffffff);m.write(self+offset+0x120,0xffffffff);m.write(self+offset+0x1b0,0x7f000000);m.write(self+offset+0x240,0x7f000000);}
 if(static_cast<int>(m.read(self+0x128))>0)return p.nonempty(self,out,skill);
 // Original case5 sums the freshly reset positive/negative arrays, ignoringFFFFFFFF.
 Word positive=0,negative=0;for(Word i=0;i<36;++i){Word a=m.read(self+0x568+i*4);if(a!=0xffffffff)positive+=a;Word b=m.read(self+0x5f8+i*4);if(b!=0xffffffff)negative+=b;}
 if(static_cast<int>(positive)>30)positive=30;
 if(static_cast<int>(negative)>30)negative=30;
 return positive-negative;
}
}
