#include "c7b70ae275b3ab15_creature_template_finish.hpp"
namespace kotor::server_mainloop {
void creature_template_finish_004f1c40(Memory&m,CreatureTemplateFinishPorts&p,Word self,Word s){
 Word ai=p.ai(m.read(m.read(0x7a39fc)+8));if(m.read(m.read(self+0xa74)+0x6c)==1){p.priority(ai,self,4);Word mask=1;for(unsigned i=0;i<18;++i,mask+=mask){Word item=p.equipped(m.read(self+0xa2c),mask);if(item)m.write(item+0x288,m.read(item+0x288)|8);}Word index=0;Word inventory=p.inventory(self,1);if(m.read(inventory+0x10)){do{Word item=p.item(p.inventory(self,1),index);m.write(item+0x288,m.read(item+0x288)|8);++index;inventory=p.inventory(self,1);}while(index<m.read(inventory+0x10));}}
 Word id=m.read(self+4),tag=self+0x18;Word module=p.module(m.read(m.read(0x7a39fc)+8));p.tag(module,tag,id);
 m.write(self+0x9d4,m.read(m.read(self+0xa74)+0x6c));Word manager=p.factions(m.read(m.read(0x7a39fc)+8));
 if(m.read(self+0x9d4)==1){if(manager){Word faction=m.read(manager+0x18);if(faction)p.member(faction,m.read(self+4),0);}m.write(m.read(self+0xa74)+0x7c,0);}else{Word stats=m.read(self+0xa74),old=m.read(stats+0x78);m.write(stats+0x78,0xffffffff);Word faction=p.faction(manager,old);if(faction){p.member(faction,m.read(self+4),0);m.write(m.read(self+0xa74)+0x7c,old);}else{p.empty(s+0x14);Word text=p.text(tag);p.format(s+0x14,0x746fa0,text,old);faction=p.faction(manager,1);p.member(faction,m.read(self+4),0);m.write(m.read(self+0xa74)+0x7c,1);p.destroy(s+0x14);}}
 p.refresh(self);m.write(self+0x1ec,1);Word effect=p.allocate(0x8c);if(effect)effect=p.effect_construct(effect,1);
 Word bits=m.read16(effect+0xa);m.write16(effect+8,m.read16(0x74678c));m.write16(effect+0xa,(bits&0xfff8)|m.read16(0x7473c8));p.creator(effect,m.read(self+4));p.effect_integer(effect,0,m.read16(m.read(self+0xa74)+0xdc));p.apply(self,effect,0,0);
 m.write(self+0x1ec,0);if(static_cast<std::int16_t>(p.hp(self,0))<1){p.state(self,0x2718);m.write(self+0xe8,0);if(!m.read(self+0x9d4)||static_cast<std::int16_t>(p.hp(self,0))<=-10)m.write16(self+0x9f0,0);}if(m.read(self+0x9d4)==1)m.write(self+0xf0,1);p.finish(self,0);
}
}
