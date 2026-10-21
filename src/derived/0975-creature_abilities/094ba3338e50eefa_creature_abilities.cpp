#include "5c0e895d350ac438_creature_abilities.hpp"
namespace kotor::server_mainloop {
Word creature_ability(Memory&m,CreatureAbilityPorts&p,Word self,Word kind,Word scratch){
 const Word fields[]={0xe9,0xeb,0xed,0xef,0xf1,0xf3},racial[]={0x18,0x19,0x1d,0x1a,0x1c,0x1b};
 auto effects=[&](){Word owner=m.read(self+0x24);m.write(scratch,0);return p.effects(owner,4,0,scratch,0,0,0,0,255,kind,0);};
 Word effect=effects();Word race=m.read16(self+0xdc),rules=m.read(0x7a3a28),table=m.read(rules+0xb0);Word bonus=static_cast<Word>(static_cast<signed char>(m.read8(table+race*52+racial[kind])));Word value=bonus+static_cast<Word>(static_cast<signed char>(effect))+m.read8(self+fields[kind]);
 if(static_cast<int>(value)<=3)return 3;
 effect=effects();rules=m.read(0x7a3a28);table=m.read(rules+0xb0);race=m.read16(self+0xdc);bonus=static_cast<Word>(static_cast<signed char>(m.read8(table+race*52+racial[kind])));Word base=m.read8(self+fields[kind]);return bonus+static_cast<Word>(static_cast<signed char>(effect))+base;
}
void creature_recalculate_005a6d80(Memory&m,CreatureAbilityPorts&p,Word self,Word scratch){
 Word count=m.read8(self+0x89),index=0;p.byte_write(scratch+0x0c,0);if(!count)return;
 do{
  Word cls=index<count?m.read8(self+index*40+0xa7):255;
  if(p.force_class(cls)){
   Word arg=m.read(scratch+0x0c),offset=index*40;Word prior=m.read8(self+offset+0xa6);p.byte_write(scratch+0x0b,prior);Word next=p.slots(self,arg,0)&255;Word before=m.read8(scratch+0x0b),delta=(next-before)&255;p.byte_write(self+offset+0xa6,next);
   if(static_cast<signed char>(delta)<0){Word old=m.read8(self+offset+0xa5);if(static_cast<int>(static_cast<signed char>(delta))<static_cast<int>(old))p.byte_write(self+offset+0xa5,old+delta);else p.byte_write(self+offset+0xa5,0);}
  }
  count=m.read8(self+0x89);index=(index+1)&255;p.byte_write(scratch+0x0c,index);
 }while(index<count);
}
}
