#include "2f2450fa0cec297e_effect_collection_both.hpp"
namespace recovered_effect_collection{
U collect(U self,U out,U kind,U list,Ports&p){
 p.write(list+4,0);p.write(list+8,0);p.write(list,0);p.write(list+12,0);kind&=255;
 if(kind==1||kind==2){U count=p.read(self+0x128);if(static_cast<int>(count)>0){U index=p.half(p.read(self+0xa74)+(kind==1?0x130:0x136));if(static_cast<int>(index)<static_cast<int>(count)){
  U low=kind==1?10:13,high=kind==1?11:14,fields=kind==1?4:5;
  do{
   U effect=p.read(p.read(self+0x124)+index*4);if(p.half(effect+8)>high)break;
   U type=p.half(effect+8);if(type==low||type==high){U pos=0,existing=0;bool matched=false;
    while(static_cast<int>(pos)<static_cast<int>(p.read(list+4))){
     existing=p.read(p.read(list)+pos*4);
     bool same=p.half(existing+8)==p.half(effect+8);
     for(U field=1;same&&field<=fields;++field){U a=p.integer(existing,field),b=p.integer(effect,field);same=a==b;}
     if(same){U a=p.read(existing+0x1c);if(a==0xffffffffu)matched=true;else{U b=p.read(effect+0x1c);matched=b==0xffffffffu||a==b;}}
     if(matched)break;
     ++pos;
    }
    if(matched){bool stronger;if(kind==1){U a=p.integer(existing,0),b=p.integer(effect,0);stronger=static_cast<int>(b)>static_cast<int>(a);}else stronger=p.rank(effect,existing,list+12,list+16);if(stronger)p.write(p.read(list)+pos*4,effect);}
    else{U capacity=p.read(list+8);if(p.read(list+4)==capacity)p.grow(list,capacity?capacity*2:16);U n=p.read(list+4),data=p.read(list);p.write(data+n*4,effect);p.write(list+4,n+1);}
   }
   ++index;
  }while(static_cast<int>(index)<static_cast<int>(p.read(self+0x128)));
 }}}
 p.copy(out,list);p.release(p.read(list));return out;
}
}
