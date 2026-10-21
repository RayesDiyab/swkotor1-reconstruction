#include "f64bf1b85585dc82_creature_hitpoints_set.hpp"
namespace kotor::server_mainloop {void creature_hitpoints_set_004cc280(Memory&m,Word self,Word value){Word minimum=m.read(self+0x200);m.write(self+0xdc,value);if(minimum&&static_cast<int>(value)<1)m.write(self+0xdc,1);}}
