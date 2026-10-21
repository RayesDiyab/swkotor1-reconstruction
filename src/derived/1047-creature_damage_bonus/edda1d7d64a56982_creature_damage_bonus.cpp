#include "946dd0b8b4376609_creature_damage_bonus.hpp"
namespace kotor::server_mainloop {
namespace {
Word ability(Memory&m,Word self){return static_cast<Word>(static_cast<int>(static_cast<signed char>(m.read8(self+0xea))));}
Word base(Memory&m,CreatureDamagePorts&p,Word item){Word type=m.read(item+0xc);return p.base(m.read(m.read(0x7a3a28)+0x34),type);}
Word specialization(Memory&m,CreatureDamagePorts&p,Word self,Word item){
 if(item&&p.rules(item)){Word data=p.rules(item),f=m.read16(data+0xb0);if(f&&p.feat(self,f))return 2;}
 return 0;
}
}
Word creature_melee_damage_005a7a20(Memory&m,CreatureDamagePorts&p,Word self,Word offhand){
 Word item=p.equipped(m.read(m.read(self+0x24)+0xa2c),offhand?0x20:0x10);
 if(offhand&&item&&m.read8(base(m,p,item)+8)!=3)item=0;
 if(item&&m.read8(base(m,p,item)+0x1a)>0)return 0;
 Word value;
 if(offhand){value=ability(m,self);if(static_cast<int>(value)>0)value=p.positive_scale(value,0x73e9ac);}
 else if(item){
  Word data=base(m,p,item),size=m.read8(data+0x1b),owner=m.read(self+0x24);
  if(size-m.read(owner+0x4f8)==1&&m.read8(data+8)!=3){value=ability(m,self);if(static_cast<int>(value)>0)value=p.positive_scale(value,0x73f414);}
  else value=ability(m,self);
 }else value=ability(m,self);
 return value+specialization(m,p,self,item);
}
Word creature_ranged_damage_005a7db0(Memory&m,CreatureDamagePorts&p,Word self,Word scratch){
 Word item=p.equipped(m.read(m.read(self+0x24)+0xa2c),0x10),value=0;
 if(item){
  if(m.read8(base(m,p,item)+0x1a)==0)return 0;
  if(m.read8(base(m,p,item)+8)==7)value=ability(m,self);
  else{
   Word a=ability(m,self);if(static_cast<int>(a)<0)value=a;
   if(p.property(item,scratch,30,0)){
    Word property=m.read(scratch),live=ability(m,self),limit=m.read16(property+6);
    value=static_cast<int>(live)<static_cast<int>(limit)?live&65535:limit;
   }
  }
 }
 return value+specialization(m,p,self,item);
}
}
