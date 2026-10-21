#include "6c137a7caef4f99a_stats_effect_query.hpp"
namespace recovered_stats_effect_query {
U query(U self,U kind,U other,Ports&p){
 U race,alignment;
 if(other){U stats=p.read(other+0xa74);race=p.half(stats+0xdc);int value=static_cast<short>(p.half(stats+0x17e));alignment=value<=40?3u:value>=60?2u:1u;}
 else {race=p.rules_race();alignment=0xffffffffu;}
 U index=p.half(self+0x15e),owner=p.read(self+0x24),list=owner+0x124;
 if(static_cast<int>(index)>=static_cast<int>(p.read(list+4)))return 0;
 do {
  U effect=p.read(p.read(list)+index*4);
  if(p.half(effect+8)>22)return 0;
  U a=p.integer(effect,0),b=p.integer(effect,1),c=p.integer(effect,2);
  if(p.half(effect+8)==22&&(a==0||a==(kind&255))){
   U wildcard=p.rules_race();if((b==wildcard||b==race)&&(c==0||c==alignment))return 1;
  }
  ++index;
 }while(static_cast<int>(index)<static_cast<int>(p.read(list+4)));
 return 0;
}
}
