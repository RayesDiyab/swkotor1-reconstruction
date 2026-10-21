#include "e5e3f8176ad0270d_creature_effect_added.hpp"
namespace kotor::server_mainloop {
void creature_effect_added_004ffaf0(Memory&m,CreatureEffectAddedPorts&p,Word self,Word effect){
 if(!effect)return;
 Word type=m.read16(effect+8);
 if(type!=m.read(0x74670c)&&type!=m.read(0x746710))return;
 p.ability_continuation_004ffb38(self,effect);
}
}
