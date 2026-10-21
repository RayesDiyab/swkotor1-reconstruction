#include "e6848e0e12af41d3_effect_copy.hpp"
namespace recovered_effect_copy{
void resize(U self,U count,Ports&p){p.write(self+0x30,count);U old=p.read(self+0x34);if(old){p.release(old);p.write(self+0x34,0);}U bytes=p.read(self+0x30)*4;U data=p.allocate(bytes);U zeroBytes=p.read(self+0x30)*4;p.write(self+0x34,data);p.zero(data,zeroBytes);}
void copy(U self,U source,U skip,Ports&p){
 if(!source)return;
 U identity=p.read(self);identity|=p.read(self+4);if(!identity){p.write(self,p.read(source));p.write(self+4,p.read(source+4));}
 p.put_half(self+8,p.half(source+8));p.put_half(self+0xa,p.half(source+0xa));
 const U prefix[]={0x20,0xc,0x10,0x14,0x18,0x1c,0x24};for(U offset:prefix)p.write(self+offset,p.read(source+offset));
 const U children[]={0x28,0x2c};for(U offset:children)if(p.read(source+offset)){U child=p.allocate(0x8c);if(child)child=p.construct(child,0);p.write(self+offset,child);U original=p.read(source+offset);copy(child,original,0,p);}
 resize(self,p.read(source+0x30),p);if(skip)return;
 for(U i=0;static_cast<int>(i)<static_cast<int>(p.read(self+0x30));++i){U value=p.read(p.read(source+0x34)+i*4);U data=p.read(self+0x34);p.write(data+i*4,value);}
 const U suffix[]={0x38,0x3c,0x40,0x44,0x78,0x7c,0x80,0x84};for(U offset:suffix)p.write(self+offset,p.read(source+offset));
 for(U i=0;i<6;++i)p.string_assign(self+0x48+i*8,source+0x48+i*8);
}
}
