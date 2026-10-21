#include "22521de4edda2e7f_item_owner.hpp"
namespace recovered_item_owner {
static U event(Ports&p,bool signal){U a=p.allocate(0x34);return a?p.event_construct(a,signal):0;}
static void acquired(U self,U owner,U module,Ports&p){U e=event(p,false);p.half(e,19);p.event_object(e,0,p.read(self+0x14));p.event_object(e,1,owner);p.enqueue(p.read(self+0x14),module,e);}
static void notify(U self,U target,U kind,Ports&p){U e=event(p,true);p.event_object(e,0,p.read(self+0x14));p.event_int(e,0,1);p.signal(target,kind,e);}
void set(U self,U next,U mode,U announce,U previous,Ports&p){
 U oldCreature=p.invalid(),newCreature=p.invalid(),oldPlace=p.invalid(),newPlace=p.invalid();
 U module=p.read(p.module()+0x20);U old=p.read(self+0x268);
 if(old==next){acquired(self,next,module,p);return;}
 bool overridden=false;if(previous!=p.invalid()){overridden=old!=previous;p.write(self+0x268,previous);}
 old=p.read(self+0x268);
 if(old!=p.invalid()){U o=p.lookup(old);if(o){U type=p.byte(o+8);if(type==5)oldCreature=p.read(self+0x268);else if(type==6){U item=p.cast_item(o);U owner=p.lookup(p.read(item+0x268));if(owner&&p.byte(owner+8)==5)oldCreature=p.read(item+0x268);}else if(type==9)oldPlace=p.read(self+0x268);}}
 if(next!=p.invalid()){U o=p.lookup(next);if(o){U type=p.byte(o+8);if(type==5)newCreature=next;else if(type==6){U item=p.cast_item(o);U owner=p.lookup(p.read(item+0x268));if(owner&&p.byte(owner+8)==5)newCreature=p.read(item+0x268);}else if(type==9)newPlace=next;}}
 if(!overridden&&oldCreature!=newCreature){
  if(oldCreature!=p.invalid()){if(mode==1){U e=event(p,false);p.half(e,20);p.event_object(e,0,p.read(self+0x14));p.enqueue(p.read(self+0x14),module,e);}U c=p.creature(oldCreature);if(c&&announce==1)notify(self,c,0x33,p);}
  if(newCreature!=p.invalid()){U o=p.lookup(newCreature);if(o){U c=p.cast_creature(o);if(p.read(p.read(c+0xa74)+0x6c)){p.plot(self,1);p.write(self+0x288,p.read(self+0x288)|0x10);}}if(announce==1){U c=p.creature(newCreature);if(c)notify(self,c,0x32,p);}}
 }
 if(newCreature!=p.invalid()&&mode==1)acquired(self,newCreature,module,p);
 if(oldPlace!=p.invalid()){U o=p.placeable(oldPlace);if(o&&!p.read(o+0x334)){U source=p.invalid();if(newCreature!=p.invalid())source=newCreature;else if(newPlace!=p.invalid())source=newPlace;if(source!=p.invalid()&&mode==1){U e=event(p,false);p.half(e,27);p.event_int(e,0,1);p.event_object(e,0,p.read(self+0x14));p.enqueue(source,oldPlace,e);}}}
 if(newPlace!=p.invalid()){U o=p.placeable(newPlace);if(o&&!p.read(o+0x334)){U source=p.invalid();if(oldCreature!=p.invalid())source=oldCreature;else if(oldPlace!=p.invalid())source=oldPlace;if(source!=p.invalid()&&mode==1){U e=event(p,false);p.half(e,27);p.event_int(e,0,0);p.event_object(e,0,p.read(self+0x14));p.enqueue(source,newPlace,e);}}}
 p.write(self+0x268,next);
}
}
