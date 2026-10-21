#include "5fa1c8f7a9-item_ai_recharge.hpp"
namespace recovered_item_ai_recharge {
U difference(U self,U d1,U t1,U d2,U t2,U od,U ot,Ports&p){
 U bound=p.read(self+0x3c);
 if(t1<bound&&t2<bound&&(d1<d2||(d1==d2&&t1<t2)))return 0xfffffffeu;
 p.write(od,d1-d2);
 U delta=t1-t2;
 p.write(ot,delta);
 if(delta>=p.read(self+0x3c)){
  p.write(od,p.read(od)-1);
  bound=p.read(self+0x3c);
  p.write(ot,p.read(ot)+bound);
 }
 return 0;
}
void update(U self,U q,Ports&p){
 p.write(q+0x14,self);
 if(!p.has(self-0x10,10,0))return;
 U app=p.read(0x7a39fc),server=p.read(app+8);
 U clock=p.timer(server);
 p.time(clock,q+0x10,q+0xc);
 U count=p.read(self+0x238),index=0;
 p.write(q+8,0);
 if(static_cast<std::int32_t>(count)<=0)return;
 U offset=0;
 do{
  if(static_cast<std::int32_t>(offset)>=0){
   count=p.read(self+0x238);
   if(static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count)){
    U entry=p.read(self+0x240)+offset;
    if(entry&&p.read16(entry)==10){
     U subtype=p.read16(entry+6);
     if(subtype>=14&&subtype<=18&&!p.read8(entry+0xc)){
      U previous_tick=p.read(entry+0x18),previous_day=p.read(entry+0x14);
      app=p.read(0x7a39fc);server=p.read(app+8);
      U day=p.read(q+0x10),tick=p.read(q+0xc);
      clock=p.timer(server);
      p.difference(clock,day,tick,previous_day,previous_tick,q+0x18,q+4);
      if(p.read(q+4)>=(subtype-13)*60000)p.write(entry+0xc,1);
     }
    }
   }
  }
  index=p.read(q+8);
  self=p.read(q+0x14);
  count=p.read(self+0x238);
  ++index;offset+=0x1c;p.write(q+8,index);
 }while(static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count));
}
}
