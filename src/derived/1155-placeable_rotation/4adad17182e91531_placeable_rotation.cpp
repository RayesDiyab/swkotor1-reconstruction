#include "5d9e820488d3b36f_placeable_rotation.hpp"
namespace recovered_placeable_rotation{void rotate(U s,U q,U scratch,Ports&p){for(U i=0;i<16;i+=4)p.write(s+0x428+i,p.read(q+i));p.derive(q,scratch);U out=p.normalize(scratch+12,scratch);p.write(scratch,p.read(out));p.write(scratch+4,p.read(out+4));p.write(scratch+8,p.read(out+8));p.orientation(s,scratch);}}
