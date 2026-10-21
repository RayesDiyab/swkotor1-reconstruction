#include "a5d4f4a90d11dbe2_effect_handler_004d9170.hpp"
namespace kotor::server_mainloop {
Word effect_handler_004d9170(Memory&m,EffectHandlerPorts&p,Word target,Word effect,Word){
 if(!p.creature(target)||!effect)return 0;
 if(static_cast<int>(p.integer(effect,0))<1||m.read(target+0xf8))return 1;
 Word id=m.read(effect+0x18),creator=p.lookup(m.read(m.read(0x7a39fc)+8),id);
 Word creature=p.creature(target);if(p.protected_from(m.read(creature+0xa74),20,creator))return 1;
 m.write(p.creature(target)+0x344,1);return 0;
}
}
