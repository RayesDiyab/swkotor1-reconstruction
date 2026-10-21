#include "01bf36d500b9fd11_creature_inventory_get.hpp"
namespace recovered_creature_inventory_get{
U get(U self,U use_shared,Ports&p){
 if(use_shared){
  if(p.read(self+0xa88)){U party=p.party(p.server());return p.shared(party);}
  U module=p.module(p.server());
  if(p.byte(module+0x104)&1){
   U id=p.read(self+4);U party=p.party(p.server());
   if(p.member(party,id)){U gui=p.gui(p.client());if(p.gui_state(gui,0)){U again=p.party(p.server());return p.shared(again);}}
  }
 }
 return p.read(self+0xa30);
}
}
