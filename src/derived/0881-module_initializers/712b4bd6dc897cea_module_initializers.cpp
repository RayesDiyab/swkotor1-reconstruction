#include "d7e1c5d3e7335676_module_initializers.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word module_list_004c4e20(Memory&m,ModuleAllocation&p,Word self){const Word value=p.allocate(12);if(value){m.write(value+4,0);m.write(value,0);m.write(value+8,0);}m.write(self,value);return self;}
Word module_flags_0059afe0(Memory&m,Word self){for(Word x:{0u,4u,8u,12u,16u})m.write(self+x,0);return self;}
Word module_map_00578be0(Memory&m,Word self){for(Word x:{0u,0x18u,0x1cu,0x20u,0x24u,4u,8u,12u})m.write(self+x,0);for(Word i=0;i<3;i++){m.write(self+0x28+i*4,0xffffffff);m.write(self+0x34+i*4,0xffffffff);m.write(self+0x40+i*4,0);}return self;}
}
