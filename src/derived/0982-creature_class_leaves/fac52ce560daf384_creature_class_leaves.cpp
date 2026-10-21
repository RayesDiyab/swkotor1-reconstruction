#include "9ae7aaa6bd4b3aa1_creature_class_leaves.hpp"
namespace kotor::server_mainloop {
void creature_set_class_005a5420(Memory&m,CreatureClassLeafPorts&p,Word self,Word index,Word value){index&=255;if(index<m.read8(self+0x89))p.byte_write(self+index*40+0xa7,value);}
Word creature_get_class_005a4e90(Memory&m,Word self,Word index){index&=255;return index<m.read8(self+0x89)?m.read8(self+index*40+0xa7):255;}
Word creature_force_class_005be4a0(Word value){return value==3||value==4||value==5;}
}
