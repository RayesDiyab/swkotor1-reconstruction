#include "4f9579524e34813f_area_placeables.hpp"
namespace kotor::server_mainloop {
Word area_placeables_0050a7b0(Memory&m,AreaPlaceablePorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 m.write(s+0xc,area);if(!p.list(gff,s+0x60,root,0x747698))return 1;m.write(s+0x10,0);if(!p.count(gff,s+0x60))return 1;Word index=0;
 do{if(p.element(gff,s+0x88,s+0x60,index)&&p.type(gff,s+0x88)==9){
  Word id=p.dword(gff,s+0x88,0x744c24,s+0x84,0x7f000000);Word storage=p.allocate(0x454);m.write(s+0x8c,storage);Word object=storage?p.construct(storage,id):0;
  if(object){Word loaded=0;if(templates){p.ref_empty(s+0x30);p.ref_read(gff,s+0x40,s+0x88,0x747494,s+0x84,s+0x30);if(m.read(s+0x84))loaded=p.template_load(object,s+0x40,1);}else loaded=p.load(object,gff,s+0x88,0);
   if(!loaded)p.destroy(object);else{
    Word bearing=p.floating(gff,s+0x88,0x747a8c,s+0x84,0);m.write(s+0x8c,bearing);m.write(object+0x25c,bearing);m.write(s+0x18,0);m.write(s+0x1c,0);m.write(s+0x20,0x3f800000);p.axis_angle(s+0x50,0,0,0x3f800000,bearing);p.rotation(object,s+0x50);
    if(saved)p.common(object,gff,s+0x88);
    m.write(s+0x14,p.floating(gff,s+0x88,0x747a88,s+0x84,0));m.write(s+0x8c,p.floating(gff,s+0x88,0x74699c,s+0x84,0));m.write(s+0x24,p.floating(gff,s+0x88,0x7476e8,s+0x84,0));Word y=m.read(s+0x8c),z=m.read(s+0x14);m.write(s+0x28,y);m.write(s+0x2c,z);p.place(object,m.read(s+0xc),s+0x24,saved);
    if(m.read(object+0x278))p.append(m.read(s+0xc)+0x12c,m.read(object+4));
   }
  }
 }
 index=m.read(s+0x10)+1;m.write(s+0x10,index);
 }while(index<p.count(gff,s+0x60));return 1;
}
}
