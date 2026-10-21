#include "3429db64394739bd_item_equip_check.hpp"
namespace recovered_item_equip_check{U check(U self,U item,U slot,U mode,U bypass,U messages,Ports&p){
 if(mode==1){U stats=p.read(self+0xa74);if(p.read(stats+0x6c)==1&&!bypass){U level=p.level(stats)&255;U need=p.item_level(item)&255;if(level<need&&p.read(p.options()+0xc8)){if(messages==1)p.signal(self,0x62);return 0;}}}
 if(p.byte(item+0x288)&0x40)return 0;
 if(!p.predicate(0x510e30,self,item,p.read(slot)))return 0;
 const U funcs[]={0x5145a0,0x518210,0x5182f0,0x514650};const U notices[]={0xcf,0xd0,0xd1,0x6b};
 for(U i=0;i<4;++i)if(!p.predicate(funcs[i],self,item,0)){if(mode==1&&messages==1)p.signal(self,notices[i]);return 0;}
 U mask=p.read(slot);U result=p.choose(self,item,slot,mode,messages,mask==16||mask==32)&255;
 U rule=p.rule(p.read(item+12));if(!rule)return 0;if(!(p.read(rule+4)&p.read(slot)))return 0;return result;
}}
