#include "5f8243f73d3d8b97_creature_skill.hpp"
namespace kotor::server_mainloop {
Word creature_skill_005aa570(Memory&m,CreatureSkillPorts&p,Word self,Word raw,Word target,Word baseOnly){Word rules=m.read(0x7a3a28),id=raw&255;if(id>=m.read8(rules+0xab))return 0;Word value=static_cast<Word>(static_cast<signed char>(m.read8(m.read(self+0x168)+id)));
 auto sx=[&](Word off){return static_cast<Word>(static_cast<signed char>(m.read8(self+off)));};
 if(!baseOnly){Word offset=id*32,table=m.read(rules+0xb4);if(!(m.read8(table+offset+0x1c)&2)&&!value)return 0;
  value+=p.effects(m.read(self+0x24),target,raw);rules=m.read(0x7a3a28);table=m.read(rules+0xb4);Word ability=m.read(table+offset+0x18);
  if(ability==m.read(0x746838)){value+=sx(0xea);if(p.penalty(m.read(self+0x24)))value-=4;}
  else if(ability==m.read(0x74683c)){Word modifier;if(p.armor(m.read(self+0x24))){modifier=sx(0xec);if(static_cast<int>(modifier)>=0)modifier=0;}else modifier=sx(0xec);value+=modifier;if(p.penalty(m.read(self+0x24)))value-=4;}
  else if(ability==m.read(0x746840))value+=sx(0xee);else if(ability==m.read(0x746844))value+=sx(0xf0);else if(ability==m.read(0x746848))value+=sx(0xf2);else if(ability==m.read(0x74684c))value+=sx(0xf4);
  table=m.read(m.read(0x7a3a28)+0xb4);if(m.read8(table+offset+0x1c)&4)value+=static_cast<Word>(static_cast<signed char>((m.read8(self+0x16d)+m.read8(self+0x16c))&255));
  Word area=p.area(m.read(m.read(0x7a39fc)+8),m.read(m.read(self+0x24)+0x54));
  if(id<=7){Word high,medium,low;if(id==1||id==2){high=0x76;medium=0x75;low=7;}else if(id==3||id==4||id==7){high=0x7a;medium=0x79;low=10;}else{high=0x78;medium=0x77;low=12;}
   if(p.feat(self,high))value+=3;else if(p.feat(self,medium))value+=2;else if(p.feat(self,low))++value;if(id==0&&area)value+=m.read(area+0x43c);
  }
 }
 if(static_cast<int>(value)>=127){return 127;}if(static_cast<int>(value)<=-127)return static_cast<Word>(-127);return value;
}
}
