#include "c85869a80dc91691_module_gff.hpp"
namespace kotor::server_mainloop {
Word gff_default_004105a0(Memory& m,ModuleGffPorts& p,Word self){
 p.base_00406460(self);
 m.write(self+0x38,0xfd4);m.write(self+0x3c,0xfd4);
 for(Word off=0x40;off<=0x78;off+=4)m.write(self+off,0);
 p.write_byte(self+0x8c,0);m.write(self+0x94,0);m.write(self+0x98,0);m.write(self+0x9c,0);
 m.write(self,p.vtable(0x73e2d0));
 m.write(self+0x28,0x6e);m.write(self+0x2c,0x286);m.write(self+0x30,0x62);m.write(self+0x34,0x72c);
 m.write(self+0x8d,0);return self;
}
Word module_gff_004c30c0(Memory& m,ModuleGffPorts& p,Word self){
 gff_default_004105a0(m,p,self);m.write(self,p.vtable(0x745810));
 // Original copies four bytes "IFO " to unaligned format field.
 m.write(self+0x8d,0x204f4649);return self;
}
}
