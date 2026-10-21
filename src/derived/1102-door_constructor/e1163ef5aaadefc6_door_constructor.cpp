#include "4d69ba7f5bf89b33_door_constructor.hpp"
#include <initializer_list>
namespace recovered_door_constructor{
U construct(U s,U id,Ports&p){
 p.base(s,10,id,0);p.write(s,p.table());
 for(U i=0;i<15;++i)p.string(s+0x228+i*8);
 p.ref(s+0x2a2);p.string(s+0x2d0);
 for(U o:{0x2e0u,0x2e4u,0x2dcu,0x338u,0x33cu,0x340u,0x344u,0x348u,0x34cu})p.write(s+o,0);
 for(U i=0;i<4;++i)for(U j=0;j<3;++j)p.write(s+0x350+i*12+j*4,0);
 p.string(s+0x388);p.string(s+0x390);p.localized(s+0x39c);p.localized(s+0x3a4);
 p.write(s+0x3bc,0);p.write(s+0x3b8,0);p.write(s+0x3b4,0);p.write(s+0x3b0,0x3f800000);p.localized(s+0x3c8);
 U ai=p.ai();p.register_ai(ai,s,0);p.write(s+0xd4,0x2726);
 for(U o:{0x31cu,0x320u,0x314u,0x318u,0x328u,0x324u})p.write(s+o,0x7f000000);
 for(U i=0;i<15;++i)p.literal(s+0x228+i*8,"default");
 p.byte(s+0x2a0,0);p.byte(s+0x2a1,0);p.write(s+0x2f0,0);p.byte(s+0x2ec,0);p.byte(s+0x2ed,0);p.write(s+0x2e8,0);p.write(s+0x2f4,0);p.byte(s+0x2f8,255);
 p.write(s+0x300,1);p.write(s+0x2fc,1);p.write(s+0x304,1);p.write(s+0x2b4,0);p.write(s+0x2b8,0);
 p.byte(s+0x2bc,0);p.byte(s+0x2bd,0);p.byte(s+0x2be,0);p.literal(s+0x2d0,"");p.write(s+0x2d8,0);
 for(U o:{0x2bfu,0x2c0u,0x2c1u,0x2c2u})p.byte(s+o,0);
 p.write(s+0x2c4,0);p.write(s+0x2c8,0);p.byte(s+0x2cc,0);p.byte(s+0x2cd,0);p.write(s+0x308,1);p.write(s+0x30c,0);p.write(s+0x310,0);p.byte(s+0x384,0);p.write(s+0x398,0x7f000000);
 for(U cell:{0x74c394u,0x74c398u,0x74c39cu})p.write(s+0x32c+p.cell(cell)*4,0);
 p.write(s+0x380,0);p.half(s+0x3ac,0);p.write(s+0x3c0,0);p.write(s+0x3c4,0);return s;
}
}
