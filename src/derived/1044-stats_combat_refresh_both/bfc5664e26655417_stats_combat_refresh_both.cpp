#include "241a5cd4db9461aa_stats_combat_refresh_both.hpp"
#include <cstdint>
namespace recovered_stats_combat_refresh{
// Original normal flow with explicitly unresolved nonempty effect-conversion blocks.
void refresh(U self,U scratch,Ports&p){
 U list=scratch,temp=scratch+12;p.write(list+4,0);p.write(list+8,0);p.write(list,0);
 U old=p.read(self+0x118);if(old){p.combat_destroy(old);p.release(old);U m=p.allocate(0x48);U value=m?p.combat_construct(m):0;p.write(self+0x118,value);}
 U returned=p.collect(p.read(self+0x24),1,temp);p.assign_array(list,returned);p.release(p.read(temp));
 if(static_cast<std::int32_t>(p.read(list+4))>0)p.convert_kind1(self,list);
 returned=p.collect(p.read(self+0x24),2,temp);p.assign_array(list,returned);p.release(p.read(temp));
 if(static_cast<std::int32_t>(p.read(list+4))>0)p.convert_kind2(self,list);
 p.put_byte(p.read(self+0x118),1);
 U right=p.equipment(p.read(p.read(self+0x24)+0xa2c),16);
 U left=p.equipment(p.read(p.read(self+0x24)+0xa2c),32);
 if(left&&p.byte(p.rule(p.read(left+12))+9))p.write(p.read(self+0x118)+12,1);
 else if(right&&p.byte(p.rule(p.read(right+12))+8)!=3)p.write(p.read(self+0x118)+12,0);
 U left_id=left?p.read(left+20):0x7f000000;p.write(p.read(self+0x118)+0x30,left_id);
 U damage;
 if(right&&p.byte(p.rule(p.read(right+12))+0x1a)>0){U attack=p.ranged_attack(self);p.put_byte(p.read(self+0x118)+1,attack);damage=p.ranged_damage(self);}
 else{U attack=p.melee_attack(self,0);p.put_byte(p.read(self+0x118)+1,attack);damage=p.melee_damage(self,0);}
 U info=p.read(self+0x118);p.put_byte(info+2,damage);U right_id=right?p.read(right+20):0x7f000000;p.write(p.read(self+0x118)+0x2c,right_id);
 if(p.read(p.read(self+0x118)+12)==1){U attack=p.melee_attack(self,1);p.put_byte(p.read(self+0x118)+3,attack);U d=p.melee_damage(self,1);p.put_byte(p.read(self+0x118)+4,d);}
 U delta=(p.byte(self+0x11f)-p.byte(self+0x120))&255;p.put_byte(p.read(self+0x118)+5,(delta&128)?0:delta);
 U x=p.byte(self+0x16d),y=p.byte(self+0x16c);p.put_byte(p.read(self+0x118)+7,0u-(x+y));
 U owner=p.read(self+0x24);(void)p.read(owner+0x4f8);(void)p.constant_zero(owner);p.put_byte(p.read(self+0x118)+8,1);
 owner=p.read(self+0x24);U style=p.read(owner+0x4f8);U zero=p.constant_zero(owner);U attacks=zero==1?(static_cast<std::int32_t>(style)>2?6:4):(static_cast<std::int32_t>(style)<=2?2:1);p.put_byte(p.read(self+0x118)+9,attacks);
 U item=p.equipment(p.read(p.read(self+0x24)+0xa2c),16);U multiplier=item?p.byte(p.rule(p.read(item+12))+0x1f):2;p.put_byte(p.read(self+0x118)+0xb,multiplier);
 U critical=p.threat(self,0);p.put_byte(p.read(self+0x118)+0xa,21-critical);
 item=p.equipment(p.read(p.read(self+0x24)+0xa2c),16);multiplier=2;
 if(item){if(p.byte(p.rule(p.read(item+12))+8)!=3)item=p.equipment(p.read(p.read(self+0x24)+0xa2c),32);if(item)multiplier=p.byte(p.rule(p.read(item+12))+0x1f);}
 p.put_byte(p.read(self+0x118)+0x11,multiplier);critical=p.threat(self,1);p.put_byte(p.read(self+0x118)+0x10,21-critical);p.release(p.read(list));
}
}
