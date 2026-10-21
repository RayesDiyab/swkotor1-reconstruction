#include "51238f3085773c7b_area_sounds.hpp"
namespace kotor::server_mainloop {
Word area_sounds_00505560(Memory&m,AreaSoundPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 if(!p.list(gff,s+0x48,root,0x7474f8))return 1;
 Word count=p.count(gff,s+0x48);m.write(s+0x18,count);m.write(s+0x10,0);if(!count)return 1;
 Word index=0;
 do{if(p.element(gff,s+0x70,s+0x48,index)&&p.type(gff,s+0x70)==6){
  Word id=p.dword(gff,s+0x70,0x744c24,s+0x6c,0x7f000000);Word storage=p.allocate(0x280);m.write(s+0x74,storage);Word object=storage?p.construct(storage,id):0;bool loaded=false;
  if(templates){p.ref_empty(s+0x28);p.ref_read(gff,s+0x38,s+0x70,0x747494,s+0x6c,s+0x28);if(m.read(s+0x6c)&&p.template_load(object,s+0x38)){
   p.byte(object+0x24c,p.dword(gff,s+0x70,0x7474e8,s+0x6c,0));
   Word z=p.floating(gff,s+0x70,0x745d68,s+0x6c,0);m.write(s+0x14,z);Word y=p.floating(gff,s+0x70,0x745d74,s+0x6c,0);m.write(s+0x74,y);Word x=p.floating(gff,s+0x70,0x745d80,s+0x6c,0);m.write(s+0x1c,x);m.write(s+0x24,m.read(s+0x14));m.write(s+0x20,m.read(s+0x74));p.position(object,s+0x1c,1,1,0);p.place(object,area,saved==0);loaded=true;
  }}else if(p.load(object,gff,s+0x70)){p.place(object,area,saved==0);loaded=true;}
  if(!loaded&&object)p.destroy(object);
 }
 index=m.read(s+0x10)+1;m.write(s+0x10,index);
 }while(index<m.read(s+0x18));return 1;
}
}
