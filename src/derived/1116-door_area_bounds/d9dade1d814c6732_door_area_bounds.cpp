#include "5eff727c0375edf3_door_area_bounds.hpp"
namespace recovered_door_area_bounds{
U append(U self,U id,U lo,U hi,Ports&p){
 U capacity=p.read(self+0x44);
 if(p.read(self+0x48)==capacity){
  U next=capacity+capacity;if(static_cast<std::int32_t>(next)<=8)next=8;
  U vectors=p.allocate(next*24);
  if(vectors)for(U i=0;i<next*2;++i){p.write(vectors+i*12,0);p.write(vectors+i*12+4,0);p.write(vectors+i*12+8,0);}
  U ids=p.allocate(next*4),old=p.read(self+0x4c);
  if(old){U count=p.read(self+0x48)*6;for(U i=0;i<count;++i)p.write(vectors+4*i,p.read(old+4*i));p.release(p.read(self+0x4c));}
  old=p.read(self+0x50);if(old){U count=p.read(self+0x48);for(U i=0;i<count;++i)p.write(ids+4*i,p.read(old+4*i));p.release(p.read(self+0x50));}
  p.write(self+0x44,next);p.write(self+0x4c,vectors);p.write(self+0x50,ids);
 }
 U count=p.read(self+0x48),data=p.read(self+0x4c),out=data+count*24;
 for(U i=0;i<12;i+=4)p.write(out+i,p.read(lo+i));
 count=p.read(self+0x48);data=p.read(self+0x4c);out=data+count*24+12;
 for(U i=0;i<12;i+=4)p.write(out+i,p.read(hi+i));
 count=p.read(self+0x48);data=p.read(self+0x50);p.write(data+count*4,id);
 count=p.read(self+0x48)+1;p.write(self+0x48,count);return count;
}}
