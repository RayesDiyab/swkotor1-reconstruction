#include "ddd019821573a6bd_area_creatures.hpp"
namespace kotor::server_mainloop {
Word area_creatures_00504a70(Memory&m,AreaCreaturePorts&p,Word self,Word gff,Word root,Word saved,Word templates,Word s){Word list=s+0x5c,status=s+0x80,element=s+0x84;
 if(!p.list(gff,list,root,0x7458dc))return 1;
 if(!p.count(gff,list))return 1;
 Word index=0;
 do{
  if(p.element(gff,element,list,index)&&p.type(gff,element)==4){
   Word creature=0,loaded=0;
   if(templates){Word allocation=p.allocate(0xac8);creature=allocation?p.construct(allocation,0x7f000000,0):0;if(creature){p.ref_literal(s+0x3c,0x73d71c);p.gff_ref(gff,s+0x4c,element,0x747494,status,s+0x3c);if(m.read(status))loaded=p.load_template(creature,s+0x4c,1);}}
   else{Word id=p.gff_dword(gff,element,0x744c24,status,0x7f000000);Word allocation=p.allocate(0xac8);creature=allocation?p.construct(allocation,id,0):0;if(creature)loaded=p.load_gff(creature,gff,element,saved);}
   if(creature){if(!loaded)p.destroy(creature,1);else{
    for(Word offset=0x18;offset<=0x2c;offset+=4)m.write(s+offset,0);
    m.write(s+0x18,p.gff_float(gff,element,0x745d80,status,0));m.write(s+0x1c,p.gff_float(gff,element,0x745d74,status,0));m.write(s+0x20,p.gff_float(gff,element,0x745d68,status,0));
    if(p.safe_position(self,s+0x18,0x41a00000,m.read(creature+0x340),1,s+0x24)){Word x=m.read(s+0x24),y=m.read(s+0x28),z=m.read(s+0x2c);m.write(s+0x18,x);m.write(s+0x1c,y);m.write(s+0x20,z);}
    Word z=m.read(s+0x20),y=m.read(s+0x1c),x=m.read(s+0x18);p.place(creature,self,x,y,z,saved,0);
    m.write(s+0x14,p.gff_float(gff,element,0x745d58,status,0));m.write(s+0x10,p.gff_float(gff,element,0x745d48,status,0));m.write(s+0x30,p.gff_float(gff,element,0x745d38,status,0));Word oy=m.read(s+0x10),ox=m.read(s+0x14);m.write(s+0x34,oy);m.write(s+0x38,ox);p.orient(creature,s+0x30);
   }}
  }
  ++index;
 }while(index<p.count(gff,list));return 1;
}
}
