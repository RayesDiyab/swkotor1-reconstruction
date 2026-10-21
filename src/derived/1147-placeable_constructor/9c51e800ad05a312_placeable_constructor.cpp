#include "4e08414ef1bc4d4c_placeable_constructor.hpp"
#include <initializer_list>
namespace recovered_placeable_constructor{
U construct(U s,U id,U scratch,Ports&p){p.base(s,9,id,0);p.write(s,p.table());p.localized(s+0x228);p.localized(s+0x234);p.ref(s+0x240);p.string(s+0x264);for(U i=0;i<16;++i)p.string(s+0x294+i*8);p.write(s+0x31c,0);p.write(s+0x320,0);p.write(s+0x318,0);p.ref(s+0x378);p.mesh(s+0x3a0);p.write(s+0x434,0);p.write(s+0x430,0);p.write(s+0x42c,0);p.write(s+0x428,0x3f800000);p.register_ai(p.ai(),s,0);
 for(U off:{0x32cu,0x338u,0x348u,0x34cu})p.write(s+off,0);
 for(U off:{0x360u,0x364u,0x368u,0x35cu,0x350u,0x354u,0x358u})p.write(s+off,0x7f000000);
 U child=p.allocate(24);p.write(s+0x36c,child?p.inventory(child,p.read(s+4),0):0);p.write(s+0x284,1);p.write(s+0x280,1);p.write(s+0x288,1);p.write(s+0x28c,0);p.byte(s+0x274,0);p.byte(s+0x290,255);p.write(s+0x254,0);p.write(s+0x334,0);p.half(s+0x370,0);p.half(s+0x372,0);U invalid=p.invalid();p.write(s+0x374,invalid);p.write(s+0x344,0xffffffff);p.empty_ref(s+0x378,scratch);p.write(s+0x388,0xffffffff);p.write(s+0x25c,0);p.write(s+0x38c,1);p.write(s+0x390,0);p.write(s+0x39c,0);if(id==0x7f000000)p.write(s+0x39c,1);p.write(s+0x448,0x7f000000);p.write(s+0x444,1);p.byte(s+0x394,0);
 for(U off:{0x438u,0x43cu,0x33cu,0x440u,0x44cu,0x450u})p.write(s+off,0);
 return s;}
}
