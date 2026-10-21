#include "1dd73dfb34bbc266_area_triggers.hpp"
namespace kotor::server_mainloop {
static void trigger_position(Memory&m,AreaTriggerPorts&p,Word gff,Word element,Word s){m.write(s+0x18,p.floating(gff,element,0x745d80,s+0x14));m.write(s+0x1c,p.floating(gff,element,0x745d74,s+0x14));m.write(s+0x20,p.floating(gff,element,0x745d68,s+0x14));}
Word area_triggers_0050a350(Memory&m,AreaTriggerPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 const Word list=s+0x5c,element=s+0x10;m.write(s+0x24,area);
 if(!p.list(gff,list,root,0x74768c))return 1;
 Word index=0;m.write(s+0x28,0);if(!p.count(gff,list))return 1;
 do{if(p.element(gff,element,list,index)&&p.type(gff,element)==1){Word id=p.dword(gff,element,s+0x58);Word storage=p.allocate(0x318);Word trigger=storage?p.construct(storage,id):0;
 if(trigger){bool loaded=false;if(templates){p.ref_empty(s+0x70);p.ref_read(gff,s+0x90,element,0x747494,s+0x14,s+0x70);if(m.read(s+0x14)&&p.load_template(trigger,s+0x90)){
  loaded=true;p.string_zero(s+0x2c);p.ref_empty(s+0xa0);p.ref_read(gff,s+0x80,element,0x747ac8,s+0x14,s+0xa0);p.ref_string(s+0x80,s+0x2c);
  Word out=p.localized(gff,s+0x50,element,0x747ab0,s+0x14,trigger+0x30c);p.localized_assign(trigger+0x30c,out);p.localized_destroy(s+0x50);
  p.string_empty(s+0x34);out=p.string_read(gff,s+0x48,element,0x747aa4,s+0x14,s+0x34);p.assign(trigger+0x230,out);p.string_destroy(s+0x48);p.string_destroy(s+0x34);p.assign(trigger+0x238,s+0x2c);p.byte_write(trigger+0x240,p.byte_read(gff,element,0x747a94,s+0x14));
  trigger_position(m,p,gff,element,s);m.write(s+0x44,m.read(s+0x20));Word x=m.read(s+0x18),y=m.read(s+0x1c);m.write(s+0x3c,x);m.write(s+0x40,y);p.position(trigger,s+0x3c,1,1,0);
  if(p.list(gff,s+0xb0,element,0x7474bc))p.geometry(trigger,gff,s+0xb0);
  p.string_destroy(s+0x2c);
 }}else{loaded=p.load_gff(trigger,gff,element)!=0;if(loaded)trigger_position(m,p,gff,element,s);}
 if(!loaded)p.destroy(trigger);else{if(saved)p.common(trigger,gff,element);Word z=m.read(s+0x20),y=m.read(s+0x1c),x=m.read(s+0x18);p.place(trigger,m.read(s+0x24),x,y,z,saved==0);if(m.read(trigger+0x2bc))p.append(m.read(s+0x24)+0x12c,m.read(trigger+4));}
 }}index=m.read(s+0x28)+1;m.write(s+0x28,index);
 }while(index<p.count(gff,list));return 1;
}
}
