#include "dabc512f02f5a62e_layout_resource.hpp"
namespace kotor::server_mainloop {
Word layout_resource_005df3a0(Memory&m,LayoutResourcePorts&p,Word self){p.base(self);m.write(self+0x28,0);m.write(self+0x2c,0);m.write(self+0x30,0);m.write(self,p.vtable());return self;}
Word layout_resource_freed_00710e60(Memory&m,Word self){m.write(self+0x28,0);m.write(self+0x2c,0);m.write(self+0x30,0);return 1;}
Word layout_resource_serviced_005d4c60(Memory&m,Word self){Word data=m.read(self+0x10);if(!data)return 0;if(m.read(self+0x28))return 1;Word size=m.read(self+0x18);m.write(self+0x30,data);m.write(self+0x2c,size);m.write(self+0x28,1);return 1;}
}
