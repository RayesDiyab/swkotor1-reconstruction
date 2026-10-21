#include "ab1972c6c60c1833_template_inventory.hpp"
namespace recovered_template_inventory {
void load(U self,U gff,U root,U restore_ids,U owner_mode,U templates,U s,Ports&p){
 constexpr U invalid=0x7f000000;U id=invalid;
 if(!p.read(self+0x4f8)){p.default_inventory(self,s+0x28);p.write(self+0x4f8,p.read(s+0x28));}
 U party=p.party();U party_flag=party&&p.read(party+0x110)?1u:0u;p.write(s+0x28,party_flag);p.write(self+0xab8,0);
 for(U group=0;group<2;++group){
  if(p.list(gff,s+0x70,root,group?"ItemList":"Equip_ItemList")!=1)continue;
  U index=0;if(!group)p.write(s+0x24,0);
  U count=p.count(gff,s+0x70);if(!count)continue;
  do{
   if(!group)index=p.read(s+0x24);
   p.element(gff,s+0x14,s+0x70,index);
   if(restore_ids)id=p.object_id(gff,s+0x14,s+0x18);
   bool existing=p.find(id)!=0;
   if(existing){U item=p.item(id);p.write(s+0x10,item);if(p.read(item+0x268)!=p.read(self+4))goto next;}
   else{
    U allocation=p.allocate(0x29c);U item=allocation?p.construct(allocation,id):0;p.write(s+0x10,item);if(!item)goto next;
    bool loaded;
    if(templates){U def=s+(group?0x50:0x30),out=def+0x10;p.empty_ref(def);p.ref(gff,out,s+0x14,group?"InventoryRes":"EquippedRes",s+0x18,def);loaded=p.read(s+0x18)&&p.load_template(p.read(s+0x10),out);if(loaded){U bit=p.dropable(gff,s+0x14,s+0x18)&1;U current=p.read(s+0x10);p.write(current+0x288,(p.read(current+0x288)&~8u)|(bit<<3));}}
    else loaded=p.load_saved(item,gff,s+0x14)!=0;
    if(!loaded){if(p.read(s+0x10))p.destroy(p.read(s+0x10));p.write(s+0x10,0);goto next;}
    if(!p.read(s+0x10))goto next;
   }
   if(!group){
    p.write(s+0x1c,p.type(gff,s+0x14));p.owner(p.read(s+0x10),p.read(self+4),owner_mode,0,invalid);
    if(p.can_equip(self,p.read(s+0x10),s+0x1c)&0xff)p.equip(self,p.read(s+0x1c),p.read(s+0x10));
    else{if(!existing&&p.read(s+0x1c)==2)p.write(self+0xab8,1);U container=p.inventory(self,existing?1:p.read(s+0x28),s+0x10,1,0,existing?1:0);p.finish(container);}
   }else{
    U container=p.inventory(self,1,s+0x10,0,0,1);U result=p.finish(container);
    if(!existing&&result&&p.read(s+0x10))p.owner(p.read(s+0x10),p.read(self+4),owner_mode,0,invalid);
   }
 next:
   if(!group)index=p.read(s+0x24);
   ++index;if(!group)p.write(s+0x24,index);
   count=p.count(gff,s+0x70);if(!group)index=p.read(s+0x24);
  }while(index<count);
 }
}
}
