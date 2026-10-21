#include "9ff48c00c85bc9a5_stats_effect_conversion_both.hpp"
namespace recovered_stats_effect_conversion{
void convert(U self,U list,Ports&p){
 U count=p.read(list+4);for(U i=0;static_cast<int>(i)<static_cast<int>(count);++i){U effect=p.read(p.read(list)+i*4);
  if(p.integer(effect,1)!=1&&p.integer(effect,1)!=2&&p.integer(effect,1)!=7&&p.integer(effect,1)!=0)continue;
  U memory=p.allocate(5),record=memory?p.modifier(memory):0;
  U type=p.half(effect+8),amount=p.integer(effect,0);p.put_byte(record,type==10?amount:0u-amount);
  if(p.integer(effect,1)==1)p.put_byte(record+2,1);
  else if(p.integer(effect,1)==2)p.put_byte(record+2,2);
  else if(p.integer(effect,1)==0)p.put_byte(record+2,6);
  else if(p.integer(effect,1)==7)p.put_byte(record+2,7);
  U race=p.rules_race();if(p.integer(effect,2)!=race)p.put_byte(record+4,p.integer(effect,2));
  if(p.integer(effect,4))p.put_byte(record+3,p.integer(effect,4));
  U target=p.read(self+0x118)+0x14,capacity=p.read(target+8),n=p.read(target+4);
  if(n==capacity)p.grow(target,capacity?capacity*2:16);
  n=p.read(target+4);U data=p.read(target);p.write(data+n*4,record);p.write(target+4,p.read(target+4)+1);
 }
}
}

namespace recovered_stats_effect_conversion{
void convert_second(U self,U list,Ports&p){
 U count=p.read(list+4);for(U i=0;static_cast<int>(i)<static_cast<int>(count);++i){U effect=p.read(p.read(list)+i*4);
  if(p.integer(effect,5)!=1&&p.integer(effect,5)!=2&&p.integer(effect,5)!=0&&p.integer(effect,5)!=7)continue;
  U memory=p.allocate(5),record=memory?p.modifier(memory):0;
  U type=p.half(effect+8),amount=p.integer(effect,0);p.put_byte(record,type==13?amount:0u-amount);
  U damage=p.integer(effect,1)&65535;
  if(damage<=7)p.put_byte(record+1,damage==2?1:damage==4?2:damage==1?0:14);
  else p.put_byte(record+1,p.log_round(damage));
  if(p.integer(effect,5)==1)p.put_byte(record+2,1);
  else if(p.integer(effect,5)==2)p.put_byte(record+2,2);
  else if(p.integer(effect,5)==0)p.put_byte(record+2,6);
  else if(p.integer(effect,5)==7)p.put_byte(record+2,7);
  U race=p.rules_race();if(p.integer(effect,2)!=race)p.put_byte(record+4,p.integer(effect,2));
  if(p.integer(effect,4))p.put_byte(record+3,p.integer(effect,4));
  U target=p.read(self+0x118)+0x20,capacity=p.read(target+8);
  if(p.read(target+4)==capacity)p.grow(target,capacity?capacity*2:16);
  U n=p.read(target+4),data=p.read(target);p.write(data+n*4,record);p.write(target+4,p.read(target+4)+1);
 }
}
}
