#include "cff42c8da36b1322_mesh_resource.hpp"
namespace recovered_mesh_resource {
U construct(U s,Ports&p){p.base_construct(s);p.write(s,p.table());p.write(s+0x28,0);return s;}
U service(U s,Ports&p){U data=p.read(s+0x10);if(!data)return 0;if(p.read(s+0x28))return 1;U size=p.read(s+0x18);p.write(s+0x2c,size);p.write(s+0x30,data);p.write(s+0x34,data);if(!p.header(data))p.write(s+0x34,0);p.write(s+0x28,1);return 1;}
U freed(U s,Ports&p){p.write(s+0x28,0);p.write(s+0x2c,0);p.write(s+0x30,0);p.write(s+0x34,0);return 1;}
U binary(U s,Ports&p){return p.header(p.read(s+0x30))?1:0;}
U destroy(U s,U flags,Ports&p){p.write(s,p.table());p.base_destroy(s);if(flags&1)p.release(s);return s;}
}
