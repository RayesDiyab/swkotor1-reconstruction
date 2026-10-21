#include "d6da090c8d2fc425_trigger_geometry.hpp"
namespace recovered_trigger_geometry {
void load(U s,U g,U l,U e,U status,Ports&p){
 U n=p.count(g,l);p.write(s+0x284,n);p.write(s+0x294,n);
 U old=p.read(s+0x288);if(old){p.free(old);p.write(s+0x288,0);}old=p.read(s+0x298);if(old){p.free(old);p.write(s+0x298,0);}
 n=p.read(s+0x284);U data=p.allocate(n*12);if(data&&static_cast<std::int32_t>(n-1)>=0){for(U i=0;i<n;++i){p.write(data+i*12,0);p.write(data+i*12+4,0);p.write(data+i*12+8,0);}}
 U bytes=p.read(s+0x284)*4;p.write(s+0x288,data);p.write(s+0x298,p.allocate(bytes));
 U bound=p.read(s+0x284);for(U i=0,offset=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(bound);++i,offset+=12){
  if(p.element(g,e,l,i)&&p.type(g,e)==3){U x=p.floating(g,e,0x7499e4,status);U y=p.floating(g,e,0x7499dc,status);U z=p.floating(g,e,0x7499d4,status);p.translate(s,offset,x,y,z);p.write(p.read(s+0x298)+i*4,i);}bound=p.read(s+0x284);
 }
}
}
