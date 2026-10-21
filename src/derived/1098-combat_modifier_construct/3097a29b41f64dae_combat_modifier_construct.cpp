#include "f2d2fb40ed686c31_combat_modifier_construct.hpp"
namespace kotor::server_mainloop {
Word combat_modifier_construct_0054f260(Memory&m,CombatModifierPorts&p,Word self){
 p.byte(self,0);p.byte(self+1,0);p.byte(self+2,0);p.byte(self+3,0);
 p.byte(self+4,m.read8(m.read(0x7a3a28)+0xaa));return self;
}
}
