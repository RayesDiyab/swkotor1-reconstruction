#include "7bad5dc2a97d683e_creature_placement.hpp"
namespace recovered_creature_placement {
void place(U self,U target,U x,U y,U z,U saved,U finalFlag,U scratch,Ports&p){
 U outer=p.server(),id=p.read(self+4),player=p.player(outer,id);
 if(p.area(self)!=target||saved==1)p.remove(self,0);
 U ai=p.ai(p.server());U objects=p.objects(p.server());
 for(U index=0;static_cast<int>(index)<static_cast<int>(p.read(self+0x328));++index){
  U objectId=p.read(p.read(self+0x324)+index*4);
  U status=p.lookup(objects,objectId,scratch);
  if((status&255)==(p.success()&255)){U object=p.read(scratch);if(p.cast(object)){U body=p.cast(p.read(scratch));p.associate(body,target,x,y,z,1);}}
 }
 if(!target)return;
 if(p.area(self)!=target&&p.read(target+0x120)!=p.read(self+0x310)&&player){
  U message=p.message(p.server());if(message){
   p.write(self+0x310,p.read(target+0x120));p.write(scratch+4,x);p.write(scratch+8,y);p.write(scratch+12,z);
   p.write(self+0x314,p.read(scratch+4));p.write(self+0x318,p.read(scratch+8));p.write(self+0x31c,p.read(scratch+12));
   p.write(self+0x320,0);p.transfer(message,player,target,x,y,z,self+0x9c,0);p.set_area(self,0);
   p.write(scratch+4,x);p.write(scratch+8,y);p.write(scratch+12,z);p.position(self,scratch+4,0,1,0);return;
  }
 }
 p.write(scratch+4,x);p.write(scratch+8,y);p.write(scratch+12,z);p.position(self,scratch+4,1,1,0);
 if(p.area(self)!=target||saved==1)p.insert(target,p.read(self+4),saved==0?1:0);
 p.set_area(self,target);
 if(p.read(self+0x78)==0&&static_cast<int>(p.read(target+0x128))>0&&ai)p.priority(ai,self,1);
 if(p.read(self+0x78)==1&&p.read(target+0x128)==0&&ai)p.priority(ai,self,0);
 if(finalFlag){U server=p.server(),areaId=p.read(target+0x120);U mode=p.mode(server);p.notify(self,self+0x90,areaId,mode==0?1:0);}
}
}
