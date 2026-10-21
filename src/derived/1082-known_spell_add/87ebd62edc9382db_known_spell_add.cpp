#include "866872626c054eaf_known_spell_add.hpp"
namespace recovered_known_spell_add{
void add(U self,U selector,U spell,Ports&p){
 selector&=255;if(selector>=1)return;
 U list=self+selector*12,count=p.read(list+4);
 if(static_cast<int>(count)>0){U data=p.read(list);for(U i=0;static_cast<int>(i)<static_cast<int>(count);++i,data+=4)if(p.read(data)==spell&&i!=0xffffffffu)return;}
 U capacity=p.read(list+8);if(p.read(list+4)==capacity)p.grow(list,capacity?capacity*2:16);
 U index=p.read(list+4),data=p.read(list);p.write(data+index*4,spell);p.write(list+4,p.read(list+4)+1);
}
}
