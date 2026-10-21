#include "ae645d2d5c731fa9_creature_forcepoints.hpp"
#include "9ae7aaa6bd4b3aa1_creature_class_leaves.hpp"
namespace kotor::server_mainloop {
Word creature_has_effect_004f7880(Memory&m,Word self,Word id){Word count=m.read(self+0x128);if(static_cast<int>(count)>0){Word cursor=m.read(self+0x124);for(Word i=0;static_cast<int>(i)<static_cast<int>(count);++i,cursor+=4)if(m.read16(m.read(cursor)+8)==id)return 1;}return 0;}
Word creature_force_class_present_005a6060(Memory&m,Word self){Word count=m.read8(self+0x89);if(count){Word first=m.read8(0x74b31c);for(Word i=0;i<count;++i){Word cls=i<count?m.read8(self+(i&255)*40+0xa7):255;if(cls==first||cls==m.read8(0x74b31d)||cls==m.read8(0x74b31e))return 1;}}return 0;}
Word creature_forcepoints_004fd490(Memory&m,CreatureForcepointPorts&p,Word self){
 auto stats=[&](){return m.read(self+0xa74);};auto sx8=[](Word a){return static_cast<Word>(static_cast<signed char>(a));};
 Word st=stats();if(m.read16(st+0xdc)==m.read16(0x74bf10))return 0;
 auto bonus=[&](){Word receiver=stats(),count=m.read8(receiver+0x89),i=0;if(count)do{Word cls=p.class_id(receiver,i)&255;if(cls==m.read8(0x74b31d)&&p.effect(self,m.read(0x74682c)))return Word(50);receiver=stats();count=m.read8(receiver+0x89);i=(i+1)&255;}while(i<count);return Word(0);};
 if(m.read(st+0x6c)&&m.read(p.party(m.read(m.read(0x7a39fc)+8))+0xf0)==0xffffffff){
  st=stats();Word cls=p.class_id(st,(m.read8(st+0x89)-1)&255)&255;if(!creature_force_class_005be4a0(cls))return 0;
  Word total=0,i=0;if(p.level(stats(),0)&255)do{Word info=p.level_info(stats(),i),base=m.read8(info+0x2d);if(base){st=stats();Word a=base+sx8(m.read8(st+0xf4))+sx8(m.read8(st+0xf2));if(static_cast<int>(a)<1)a=1;total+=a;}++i;}while(static_cast<int>(i)<static_cast<int>(p.level(stats(),0)&255));
  if(p.feat(stats(),0x74)){total+=40;}return (total+bonus())&65535;
 }
 st=stats();Word total=m.read16(st+0x122),cached=m.read8(st+0xf2);Word level=p.level(st,0)&255;st=stats();Word next=m.read8(st+0xf4);total+=level*sx8(cached);total+=(p.level(st,0)&255)*sx8(next);
 if(p.feat(stats(),0x74)){total+=40;}total+=bonus();if(!creature_force_class_present_005a6060(m,stats()))return 0;
 level=p.level(stats(),0)&255;if(static_cast<short>(level)>static_cast<short>(total))return p.level(stats(),0)&255;return total&65535;
}
}
