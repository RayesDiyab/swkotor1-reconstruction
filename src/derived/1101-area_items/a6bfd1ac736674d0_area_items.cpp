#include "a1ee378b98ac1d13_area_items.hpp"
namespace kotor::server_mainloop {
Word area_items_00504de0(Memory&m,AreaItemPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 const Word list=s+0x44,element=s+0x6c,status=s+0x68;
 if(!p.list(gff,list,root)||!p.count(gff,list))return 1;
 Word index=0;do{
  if(p.element(gff,element,list,index)&&p.type(gff,element)==0){
   Word id=p.dword(gff,element,s+0x14);Word allocation=p.allocate(0x29c);Word item=allocation?p.construct(allocation,id):0;
   if(item){Word loaded=0;if(templates){p.ref_empty(s+0x24);p.ref_read(gff,s+0x34,element,status,s+0x24);if(m.read(status))loaded=p.load_template(item,s+0x34);}else loaded=p.load_gff(item,gff,element);
    if(!loaded)p.destroy(item);else{
     m.write(s+0x18,0);m.write(s+0x1c,0);m.write(s+0x20,0);
     if(saved)p.common(item+0x10,gff,element);
     Word z=p.floating(gff,element,0x745d68,status);Word y=p.floating(gff,element,0x745d74,status);Word x=p.floating(gff,element,0x745d80,status);
     p.place(item,area,x,y,z,saved==0);
     m.write(s+0x18,p.floating(gff,element,0x745d38,status));m.write(s+0x1c,p.floating(gff,element,0x745d48,status));m.write(s+0x20,p.floating(gff,element,0x745d58,status));p.orient(item+0x10,s+0x18);
    }
   }
  }
  ++index;
 }while(index<p.count(gff,list));return 1;
}
}
