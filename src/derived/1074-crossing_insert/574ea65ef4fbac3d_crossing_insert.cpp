#include "9ca911027ddfdc58_crossing_insert.hpp"
namespace recovered_crossing_insert{
void vector_insert(U self,U x,U y,U z,U index,Ports&p){U oldCount=p.read(self+4),capacity=p.read(self+8);if(oldCount==capacity)p.vector_grow(self,capacity?capacity*2:16);U count=p.read(self+4),target=p.read(self)+count*12;p.write(target,x);p.write(target+4,y);p.write(target+8,z);p.write(self+4,p.read(self+4)+1);if(static_cast<int>(oldCount)>static_cast<int>(index)){U offset=oldCount*12,left=oldCount-index;do{U dst=p.read(self)+offset;p.write(dst,p.read(dst-12));p.write(dst+4,p.read(dst-8));p.write(dst+8,p.read(dst-4));offset-=12;}while(--left);}target=p.read(self)+index*12;p.write(target,x);p.write(target+4,y);p.write(target+8,z);}
void insert(U self,U,U direction,U point,U countOut,U edge,Ports&p){U count=p.read(self+0xc0),index=0;bool located=false;
 if(static_cast<int>(count)>=4){U dot=p.projection(point,direction),array=p.read(self+0xbc);U last=3;do{for(U j=0;j<4;++j){if(p.greater(array+(index+j)*12,direction,dot,j<2?0:1)){index+=j;located=true;break;}}if(located)break;index+=4;last+=4;}while(static_cast<int>(last)<static_cast<int>(count));}
 if(!located&&static_cast<int>(index)<static_cast<int>(count)){U dot=p.projection(point,direction),array=p.read(self+0xbc)+index*12;while(static_cast<int>(index)<static_cast<int>(count)){if(p.greater(array,direction,dot,5))break;++index;array+=12;}}
 U x=p.read(point),y=p.read(point+4),z=p.read(point+8);p.vector_insert(self+0xbc,x,y,z,index);
 U oldCount=p.read(self+0xcc),capacity=p.read(self+0xd0);if(oldCount==capacity){U newCapacity=capacity?capacity*2:16,old=p.read(self+0xc8);p.write(self+0xd0,newCapacity);U allocated=p.allocate(newCapacity*4);p.write(self+0xc8,allocated);for(U i=0;static_cast<int>(i)<static_cast<int>(p.read(self+0xcc));++i)p.write(p.read(self+0xc8)+i*4,p.read(old+i*4));if(old)p.release(old);}
 p.write(p.read(self+0xc8)+p.read(self+0xcc)*4,edge);p.write(self+0xcc,p.read(self+0xcc)+1);
 for(U i=oldCount;static_cast<int>(i)>static_cast<int>(index);--i){U array=p.read(self+0xc8);p.write(array+i*4,p.read(array+i*4-4));}p.write(p.read(self+0xc8)+index*4,edge);p.write(countOut,p.read(countOut)+1);
}
}
