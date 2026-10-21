#include "11ccf1ea44605740_module_constructor.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word module_constructor_004c84a0(Memory& m,ModuleConstructorPorts& p,Word self,Word name,Word event_arg,Word unused,Word scratch){
 (void)unused;const auto s=[&](Word x){return scratch+x;};
 const auto server=[&](){return m.read(m.read(0x7a39fc)+8);};
 m.write(self,p.vtable(0x7458d4));p.list_00405ed0(self+0xc);m.write(self+8,0);p.list_insert_00406290(self+0xc,s(0x10),0x73d71c);m.write(self+4,0);
 p.object_004d7d60(self+0x1c,m.read8(0x746632),0x7f000000);
 m.write(self,p.vtable(0x745960));m.write(self+0x1c,p.vtable(0x7458f0));
 p.table_004c4e20(self+0x28);p.table_004c4e20(self+0x2c);p.list_00405ed0(self+0x30);p.table_004c4e20(self+0x48);p.linked_005e9e10(self+0x4c);p.empty_005b3190(self+0x5c);p.linked_005e9e10(self+0x64);p.list_00405ed0(self+0x6c);p.list_00405ed0(self+0x7c);
 for(Word x:{0x90u,0x94u,0x8cu,0x98u})m.write(self+x,0);
 p.location_0059afe0(self+0x9c);p.strings_006fb55f(self+0xb0,8,15,0x5b3190,0x5e5c20);
 for(Word x:{0x134u,0x138u,0x130u,0x140u,0x144u,0x13cu,0x188u,0x18cu,0x190u,0x1d0u,0x1d4u,0x1ccu,0x1e0u,0x1e4u,0x1dcu})m.write(self+x,0);
 p.empty_005b3190(self+0x1f8);p.empty_005b3190(self+0x204);p.empty_005b3190(s(0x24));
 m.write(self+0x54,p.allocate_006fa7e6(0x28));const Word list=p.allocate_006fa7e6(0x24);if(list)p.list_00405ed0(list);m.write(self+0x58,list);
 const Word raw_map=p.allocate_006fa7e6(0x4c);const Word map=raw_map?p.map_00578be0(raw_map):0;m.write(self+0x218,map);m.write(self+0x128,0);m.write(self+0x12c,0);
 const Word prefix=p.literal_005e5a90(s(0x2c),0x73d8f8);const Word path=p.concat_005e5d10(prefix,s(0x10),name);p.assign_005e5c50(s(0x24),path);p.destroy_005e5c20(s(0x10));p.destroy_005e5c20(s(0x2c));p.assign_005e5c50(self+0x5c,s(0x24));
 m.write(self+0x44,1);m.write(self+0x148,0);
 for(Word x:{0x14cu,0x150u,0x154u,0x158u,0x16cu,0x15cu,0x160u})m.write(self+x,m.read(0x746644));
 m.write(self+0x164,0x7f000000);p.write_byte(self+0x168,0);
 for(Word x:{0x178u,0x17cu,0x180u,0x184u,0x194u,0x198u,0x19cu})m.write(self+x,0x7f000000);
 m.write(self+0x170,m.read(0x746644));m.write(self+0x174,m.read(0x746644));m.write(self+0x214,0);m.write(self+0x200,0);p.set_literal_005e5140(self+0x204,0x73d71c);m.write(self+0x1c8,0);p.set_name_004c4150(self,self+0x5c);
 p.write_byte(self+0x210,2);p.write_byte(self+0x1a0,5);p.write_byte(self+0x1a1,0);p.write_byte(self+0x1a2,0);p.write_byte(self+0x1a3,1);p.write_byte(self+0x1a4,1);p.write_byte(self+0x1a5,0);p.write_byte(self+0x1a6,10);
 m.write(self+0x1a8,m.read(0x74410c));m.write(self+0x1ac,1);m.write(self+0x1b0,1);m.write(self+0x1b4,0);m.write(self+0x1b8,0);p.write_byte(self+0x1bc,0);m.write(self+0x1d8,1);
 for(Word x:{0x1e8u,0x1ecu,0x1f0u,0x1f4u})m.write(self+x,0);
 p.resref_00406d80(s(0x2c),0x73d71c);p.event_004c4cc0(self,s(0x2c),0);p.resref_00406d80(s(0x2c),0x745d30);p.event_004c4cc0(self,s(0x2c),event_arg);
 p.register_004d80e0(p.array_004aed70(server()),self+0x20,self+0x1c,0);
 p.time_004ade40(p.timer_004aede0(server()),self+0x1c0,self+0x1c4);
 p.set_literal_005e5140(self+0x1f8,0x73d71c);m.write(self+0x20c,0xffffffff);m.write(self+0x220,0);m.write(self+0x21c,0x7f000000);p.destroy_005e5c20(s(0x24));return self;
}
}
