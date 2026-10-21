#include "9689cedea68231da_stats_attack_bonus.hpp"
#include <cstdint>
namespace recovered_stats_attack_bonus{namespace{
U equip(U self,U slot,Ports&p){return p.equipment(p.read(p.read(self+0x24)+0xa2c),slot);}U signed_byte(U x){return static_cast<U>(static_cast<std::int32_t>(static_cast<std::int8_t>(x)));}
U dex(U self,Ports&p){U incapacitated=p.incapacitated(p.read(self+0x24));U value=signed_byte(p.byte(self+0xec));if(incapacitated&&static_cast<std::int32_t>(value)>=0)return 0;return value;}
U focus(U self,U item,Ports&p){if(item&&p.item_rule(item)){U id=p.half(p.item_rule(item)+0xb2);if(id&&p.feat(self,id))return 1;}return 0;}
U dueling(U self,Ports&p){U right=equip(self,16,p);U left=equip(self,32,p);if(right&&!left){U rule=p.item_rule(right);if(rule){U kind=p.byte(rule+8);if(kind==2||kind==4){if(p.feat(self,0x73))return 3;if(p.feat(self,0x72))return 2;if(p.feat(self,0x71))return 1;}}}return 0;}
U offhand_penalty(U self,Ports&p){if(p.feat(self,0x55))return 0xfffffffe;if(p.feat(self,9))return 0xfffffffc;if(p.feat(self,3))return 0xfffffffa;return 0xfffffff6;}
}
U melee(U self,U offhand,U include,U no_penalty,U opposite,Ports&p){U item=equip(self,offhand&&!opposite?32:16,p);if(item&&p.byte(p.base_rule(p.read(item+12))+0x1a)>0)return 0;U result=0;if(include==1){result=p.effect_attack(p.read(self+0x24),offhand);result+=p.base_attack(self);}if(p.finesse(self,item))result+=dex(self,p);else result+=signed_byte(p.byte(self+0xea));result+=focus(self,item,p);
 if(!no_penalty){U other=equip(self,opposite?16:32,p);U penalty=0;if(!offhand){if(other&&p.byte(p.base_rule(p.read(other+12))+9)){penalty=0xfffffffau;U size=p.weapon_size(p.read(self+0x24),other);if(size==0xffffffffu||p.byte(p.base_rule(p.read(other+12))+8)==3)penalty=0xfffffffcu;if(p.feat(self,0x55))penalty+=4;else if(p.feat(self,9))penalty+=2;}}else if(other)penalty=offhand_penalty(self,p);result+=penalty;}
 return result+dueling(self,p);
}
U ranged(U self,U offhand,U include,U,Ports&p){U item=equip(self,16,p);if(item&&p.byte(p.base_rule(p.read(item+12))+0x1a)==0)return 0;U result=0;if(include==1){result=p.effect_attack(p.read(self+0x24),0);result+=p.base_attack(self);}U left=equip(self,32,p);U penalty=0;if(!offhand){if(left&&p.byte(p.base_rule(p.read(left+12))+9)){penalty=0xfffffffau;U size=p.weapon_size(p.read(self+0x24),item);if(static_cast<std::int32_t>(size)<=-1||p.byte(p.base_rule(p.read(item+12))+8)==3)penalty=0xfffffffcu;if(p.feat(self,0x55))penalty+=4;else if(p.feat(self,9))penalty+=2;}}else if(item)penalty=offhand_penalty(self,p);result+=penalty;result+=dex(self,p);result+=focus(self,item,p);return result+dueling(self,p);}
}
