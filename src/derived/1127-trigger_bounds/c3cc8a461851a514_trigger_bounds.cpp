#include "27a36b75249a4636_trigger_bounds.hpp"
namespace recovered_trigger_bounds{
void bounds(U s,U x0,U y0,U x1,U y1,Ports&p){
 if(p.equal(p.read(s+0x2e8),p.read(s+0x2ec))&&p.equal(p.read(s+0x2f0),p.read(s+0x2f4))){
 U data=p.read(s+0x288);p.write(s+0x2e8,p.read(data));p.write(s+0x2f0,p.read(data+4));p.write(s+0x2ec,p.read(data));p.write(s+0x2f4,p.read(data+4));
 U count=p.read(s+0x284);for(U i=1,a=data+12;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i,a+=12){
 if(p.less(p.read(a),p.read(s+0x2e8)))p.write(s+0x2e8,p.read(a));
 if(p.less(p.read(a+4),p.read(s+0x2f0)))p.write(s+0x2f0,p.read(a+4));
 if(p.greater(p.read(a),p.read(s+0x2ec)))p.write(s+0x2ec,p.read(a));
 if(p.greater(p.read(a+4),p.read(s+0x2f4)))p.write(s+0x2f4,p.read(a+4));
 count=p.read(s+0x284);
 }
 }
 p.write(x0,p.read(s+0x2e8));p.write(y0,p.read(s+0x2f0));p.write(x1,p.read(s+0x2ec));p.write(y1,p.read(s+0x2f4));
}}
