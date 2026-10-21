#include "aa94b4717b86e266_effect_creator.hpp"
namespace kotor::server_mainloop {
void effect_creator_00503a00(Memory&m,EffectCreatorPorts&p,Word self,Word id){Word object=p.object(m.read(m.read(0x7a39fc)+8),id);m.write(self+0x18,id);if(!object||!p.call(object,0x10))return;Word base=p.call(object,0x10);m.write(self+0x1c,p.call(base,0xb8));if(p.call(object,0x50)){p.call(object,0x50);Word trap=p.call(object,0x50);m.write(self+0x18,m.read(trap+0x248));}}
}
