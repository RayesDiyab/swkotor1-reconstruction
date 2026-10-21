#include "c835db58e25eae4e_area_constructor.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word area_constructor_0050cf80(Memory&m,AreaConstructorPorts&p,Word self,Word ref,Word arg2,Word id,Word scratch){
 auto z=[&](Word off){m.write(self+off,0);};
 p.base_0058f6b0(self);
 m.write(self+0x100,p.vtable(0x74767c));p.ref_00405ed0(self+0x10c);
 z(0x108);p.assign_00406290(self+0x10c,scratch+0x1c,p.literal(0x73d71c));z(0x104);
 p.object_004d7d60(self+0x11c,m.read8(0x746633),id);
 m.write(self,p.vtable(0x747bfc));m.write(self+0x100,p.vtable(0x747bf8));m.write(self+0x11c,p.vtable(0x747b88));
 z(0x130);z(0x134);z(0x12c);p.small_00506d90(self+0x144);
 p.empty_005b3190(self+0x148);p.localized_005e9e10(self+0x150);p.empty_005b3190(self+0x158);
 p.strings_006fb55f(self+0x164,8,4,0x5b3190,0x5e5c20);
 for(Word off:{0x194u,0x198u,0x190u,0x1a4u,0x1a8u,0x1a0u,0x1e8u,0x1ecu,0x1e4u,0x1f0u})z(off);
 p.location_0059afe0(self+0x1f4);
 for(Word off:{0x274u,0x278u,0x270u,0x280u,0x284u,0x27cu,0x28cu,0x290u,0x288u,0x298u,0x29cu,0x294u,0x2a4u,0x2a8u,0x2a0u})z(off);
 m.write(self+0x160,p.allocate(12));z(4);z(0x184);z(12);z(8);p.byte(self+0x223,0);
 for(Word off:{0x138u,0x128u,0x188u,0x18cu,0x13cu,0x140u,0x1b4u,0x250u})z(off);
 p.resource_00506c30(self+0x100,ref,arg2);
 Word allocated=p.allocate(0x278);m.write(self+0x1b0,allocated?p.collision_005d0ce0(allocated):0);
 p.initialize_0058f800(self);
 Word server=m.read(m.read(0x7a39fc)+8);
 if(id==0x7f000000){Word objects=p.objects_004aed70(server);p.register_004d80e0(objects,self+0x120,self+0x11c,0);}
 else {Word objectid=m.read(self+0x120);Word objects=p.objects_004aed70(server);p.insert_004d7e90(objects,objectid,self+0x11c);}
 z(0x1e0);allocated=p.allocate(0x30);m.write(self+0x208,allocated?p.effects_005c95a0(allocated,m.read(self+0x120)):0);
 server=m.read(m.read(0x7a39fc)+8);Word timer=p.timer_004aede0(server);p.time_004ade40(timer,scratch+0x10,scratch+0x14);
 Word first=m.read(scratch+0x10),second=m.read(scratch+0x14);m.write(self+0x218,first);m.write(self+0x21c,second);
 p.byte(self+0x20c,0);z(0x210);z(0x214);p.byte(self+0x221,0);
 for(Word off:{0x224u,0x2acu,0x2b0u,0x238u,0x23cu,0x240u,0x244u,0x1acu})z(off);
 m.write16(self+0x228,0);
 for(Word off:{0x22cu,0x230u,0x25cu,0x260u,0x264u,0x268u,0x26cu,0x2b4u,0x2b8u,0x2bcu,0x2c0u,0x2c4u})z(off);
 p.byte(self+0x2c8,0);p.byte(self+0x2c9,0);z(0x2d0);p.byte(self+0x220,0xff);return self;
}
}
