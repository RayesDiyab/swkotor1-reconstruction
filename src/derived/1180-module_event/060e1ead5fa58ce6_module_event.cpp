#include "fc349e12159f3e9d_module_event.hpp"
namespace recovered_module_event{
void dispatch(U s,U kind,U source,U data,U day,U tick,U q,Ports&p){
 if(kind==1){if(data){U id=p.read(s+0x20);p.saved_script(p.read(0x7a3a00),data,id,1);}return;}
 if(kind!=10)return;
 U name=0,id=0,v=0;
 switch(p.read16(data)){
 case 0:id=p.read(s+0x20);p.write(s+0x128,day);p.write(s+0x12c,tick);name=s+0xb0;break;
 case 11:v=p.integer(data,0);p.write(s+0x148,v);id=p.read(s+0x20);name=s+0xb8;break;
 case 14:v=p.object(data,0);id=p.read(s+0x20);p.write(s+0x14c,v);name=s+0xd0;break;
 case 15:v=p.object(data,0);id=p.read(s+0x20);p.write(s+0x150,v);name=s+0xd8;break;
 case 16:id=p.read(s+0x20);name=s+0xc8;break;
 case 17:id=p.read(s+0x20);name=s+0xc0;break;
 case 19:v=p.object(data,1);p.write(s+0x158,v);v=p.object(data,0);id=p.read(s+0x20);p.write(s+0x154,v);name=s+0xe8;break;
 case 20:v=p.object(data,0);id=p.read(s+0x20);p.write(s+0x15c,v);p.write(s+0x160,source);name=s+0xf0;break;
 case 18:
  v=p.object(data,0);p.write(s+0x178,v);v=p.object(data,1);p.write(s+0x17c,v);v=p.object(data,2);p.write(s+0x180,v);v=p.object(data,3);p.write(s+0x184,v);
  v=p.real_bits(data,2);p.write(q+0x18,v);v=p.real_bits(data,1);p.write(q+0x14,v);v=p.real_bits(data,0);p.write(q,v);
  {U y=p.read(q+0x14),z=p.read(q+0x18);p.write(q+4,y);U x=p.read(q);p.write(q+8,z);y=p.read(q+4);p.write(s+0x188,x);z=p.read(q+8);p.write(s+0x18c,y);id=p.read(s+0x20);p.write(s+0x190,z);}name=s+0xe0;break;
 case 10:id=p.read(s+0x20);p.write(s+0x16c,source);name=s+0xf8;break;
 case 32:id=p.read(s+0x20);p.write(s+0x170,source);name=s+0x100;break;
 case 33:id=p.read(s+0x20);p.write(s+0x174,source);name=s+0x108;break;
 case 35:p.write(s+0x164,source);v=p.integer(data,0);id=p.read(s+0x20);p.byte(s+0x168,v);name=s+0x110;break;
 case 37:id=p.read(s+0x20);p.write(s+0x194,source);name=s+0x118;break;
 case 36:{U app=p.read(0x7a39fc);p.write(q+0x18,0);U array=p.objects(p.read(app+8));p.lookup(array,source,q+0x18);U obj=p.read(q+0x18);if(obj&&p.creature(obj)){app=p.read(0x7a39fc);array=p.objects(p.read(app+8));p.remove(array,source);}}break;
 case 38:v=p.object(data,0);id=p.read(s+0x20);p.write(s+0x19c,v);name=s+0x120;break;
 default:break;
 }
 if(name)p.script(p.read(0x7a3a00),name,id,1);
 p.destroy(data);p.release(data);
}
}
