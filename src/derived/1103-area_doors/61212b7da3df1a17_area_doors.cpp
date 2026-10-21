#include "176b426607a2d99b_area_doors.hpp"
namespace kotor::server_mainloop {
Word area_doors_0050a0e0(Memory&m,AreaDoorPorts&p,Word area,Word gff,Word root,Word saved,Word templates,Word s){
 const Word list=s+0x30,element=s+0x58,status=s+0x54;
 if(!p.list(gff,list,root)||!p.count(gff,list))return 1;
 Word index=0;do{
  if(p.element(gff,element,list,index)&&p.type(gff,element)==8){
   Word id=p.dword(gff,element,status);Word allocation=p.allocate(0x3d0);Word door=allocation?p.construct(allocation,id):0;
   if(door){if(!p.load(door,gff,element,templates))p.destroy(door);else{
    Word angle=p.floating(gff,element,0x747a8c,status);m.write(s+0x5c,angle);
    m.write(s+0x14,0);m.write(s+0x18,0);m.write(s+0x1c,0x3f800000);
    p.axis_angle(s+0x20,0,0,0x3f800000,angle);
    m.write(door+0x2b4,m.read(s+0x5c));
    Word q0=m.read(s+0x20),q1=m.read(s+0x24),q2=m.read(s+0x28),q3=m.read(s+0x2c);p.rotation(door,q0,q1,q2,q3);
    if(saved)p.common(door,gff,element);
    Word z=p.floating(gff,element,0x747a88,status);Word y=p.floating(gff,element,0x74699c,status);Word x=p.floating(gff,element,0x7476e8,status);p.place(door,area,x,y,z,saved==0);
    if(m.read(door+0x2e8))p.append(area+0x12c,m.read(door+4));
   }}
  }
  ++index;
 }while(index<p.count(gff,list));return 1;
}
}
