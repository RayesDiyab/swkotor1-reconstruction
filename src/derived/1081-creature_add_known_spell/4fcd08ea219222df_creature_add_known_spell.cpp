#include "a21fd89cb58524bd_creature_add_known_spell.hpp"
namespace kotor::server_mainloop {
void creature_add_known_spell_005aa9b0(Memory&m,AddKnownSpellPorts&p,Word self,Word index,Word spell){
 index&=255;if(index>=m.read8(self+0x89))return;
 if(!p.spell(m.read(m.read(0x7a3a28)+0x8c),spell))return;
 if(m.read(self+0x6c)){Word cls=index<m.read8(self+0x89)?m.read8(self+index*40+0xa7):255;if(!p.force_class(cls))return;}
 p.add(self+index*40+0x8c,0,spell);
}
}
