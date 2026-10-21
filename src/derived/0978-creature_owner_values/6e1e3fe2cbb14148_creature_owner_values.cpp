#include "95a7a067badf8d2f_creature_owner_values.hpp"
namespace kotor::server_mainloop {
Word creature_owner_value_004edd60(Memory&m,CreatureOwnerValuePorts&p,Word self){if(m.read(self+0xa88))return m.read(p.party(m.read(m.read(0x7a39fc)+8))+0xfc);return m.read(self+0x9d0);}
void creature_owner_set_004edd90(Memory&m,CreatureOwnerValuePorts&p,Word self,Word value){if(static_cast<int>(value)>999999999)value=999999999;if(m.read(self+0xa88))m.write(p.party(m.read(m.read(0x7a39fc)+8))+0xfc,value);else m.write(self+0x9d0,value);}
void creature_experience_005af480(Memory&m,CreatureOwnerValuePorts&p,Word self,Word value,Word notify,Word scratch){
 Word id=m.read(m.read(self+0x24)+4);Word player=p.player(m.read(m.read(0x7a39fc)+8),id);(void)p.message(m.read(m.read(0x7a39fc)+8));Word delta=value-m.read(self+0x68);p.empty(scratch+0x18);
 if(value<m.read(self+0x68))p.decreased_experience(self,value,scratch);
 else{m.write(self+0x68,value);if(notify)p.notify_experience(self,value,delta,player);}
 p.destroy(scratch+0x18);
}
}
