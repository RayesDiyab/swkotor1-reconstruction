#include "7a3b6895e03fa3ac_inventory_insert.hpp"
namespace recovered_inventory_insert{
U add(U self,U itemOut,U combine,U notify,U mark,U scratch,Ports&p){
 U owner=p.object(p.server(),p.read(self));
 auto consume=[&](){U item=p.read(itemOut);if(item)p.destroy(item);p.write(itemOut,0);};
 if(owner){
  if(p.cast(owner,0x30)){
   U item=p.read(itemOut);U rule=p.rules(item);U kind=p.byte(rule+0xac);
   item=p.read(itemOut);
   if(kind==23){U amount=p.read(item+0x28c);U creature=p.cast(owner,0x30);p.gold(creature,amount,1);consume();return 1;}
   rule=p.rules(item);if(p.byte(rule+0xac)==42){item=p.read(itemOut);U creature=p.cast(owner,0x30);p.special(creature,item);consume();return 1;}
  }else if(p.cast(owner,0x38)){
   U container=p.cast(owner,0x38);U rule=p.base_rule(p.read(container+0xc));
   if(p.read(rule+0x70)==1){U id=p.read(container+0x268);if(id!=0x7f000000u){U creature=p.creature(p.server(),id);if(creature){U rule2=p.rules(p.read(itemOut));bool converted=false;if(p.byte(rule2+0xac)==23){U amount=p.read(p.read(itemOut)+0x28c);p.gold(creature,amount,1);converted=true;}else{rule2=p.rules(p.read(itemOut));if(p.byte(rule2+0xac)==42){p.special(creature,p.read(itemOut));converted=true;}}if(converted){consume();return 1;}}}}
  }
 }
 U position=0,found=0,talk=p.talk(p.client()),name=scratch,otherName=scratch+8;
 p.empty(name);p.empty(otherName);
 if(p.byte(self+8)&1)p.text(p.read(itemOut)+0x280,talk,name,0);
 auto done=[&](){p.destroy_string(otherName);p.destroy_string(name);return 1u;};
 if(combine==1){
  for(U i=0;static_cast<int>(i)<static_cast<int>(p.read(self+0x10));++i){
   U id=p.read(p.read(self+0xc)+i*4),existing=p.item(p.server(),id);
   if(!existing)continue;
   if((p.byte(self+8)&1)&&!found){p.text(existing+0x280,talk,otherName,0);U a=p.read(p.read(itemOut)+0xc),b=p.read(existing+0xc);if(a<b||(a==b&&p.less(name,otherName))){position=i;found=1;}}
   U current=p.read(itemOut);
   if(p.read(existing+0xc)!=p.read(current+0xc)||p.byte(existing+0x264)!=p.byte(current+0x264)||p.merge(existing,current)!=1)continue;
   if(owner){U type=p.byte(owner+8),id=0;bool relevant=false;if(type==5){id=p.read(self);relevant=true;}else if(type==6){id=p.read(p.cast(owner,0x38)+0x268);relevant=true;}if(relevant){U player=p.player(p.server(),id);if(player){U itemID=p.read(p.read(itemOut)+0x14);U message=p.message(p.server());p.notify(message,player,itemID);}}}
   current=p.read(itemOut);if(current)p.destroy(current);p.write(itemOut,existing);p.owner(existing,p.read(existing+0x268),notify,notify,0x7f000000u);
   if(mark){U finalItem=p.read(itemOut);p.write(finalItem+0x288,p.read(finalItem+0x288)|0x80);p.write(self+4,p.read(self+4)+1);}
   return done();
  }
 }else if(p.byte(self+8)&1){
  for(U i=0;static_cast<int>(i)<static_cast<int>(p.read(self+0x10));++i){U id=p.read(p.read(self+0xc)+i*4),existing=p.item(p.server(),id);if(existing&&(p.byte(self+8)&1)){p.text(existing+0x280,talk,otherName,0);if(p.less(name,otherName)){position=i;found=1;break;}}}
 }
 U current=p.read(itemOut),temporary=p.allocate(64);p.write(temporary,current);
 if(p.byte(self+8)&1){
  U player=p.local_player(p.server(),0,1);if(player){U id=p.read(player+0x38);p.owner(p.read(itemOut),id,notify,notify,0x7f000000u);}
  if(found){U id=p.read(p.read(itemOut)+0x14);p.insert(self+0xc,id,position);}
  else{U id=p.read(p.read(itemOut)+0x14),capacity=p.read(self+0x14),list=self+0xc;if(p.read(list+4)==capacity)p.grow(list,capacity?capacity*2:16);U count=p.read(list+4),data=p.read(list);p.write(data+count*4,id);p.write(list+4,p.read(list+4)+1);}
 }else{U id=p.read(p.read(itemOut)+0x14);p.insert(self+0xc,id,0);}
 if(mark){U item=p.read(itemOut);p.write(item+0x288,p.read(item+0x288)|0x80);}
 if(p.byte(p.read(itemOut)+0x288)&0x80)p.write(self+4,p.read(self+4)+1);
 p.release(temporary);return done();
}
}
