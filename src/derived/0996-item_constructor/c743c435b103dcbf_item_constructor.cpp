#include "89fdce75558a467c_item_constructor.hpp"
namespace recovered_item_constructor{
U base(U s,Ports&p){p.write(s,p.table(0x74b2a0));p.write(s+12,30);p.write(s+4,0);p.half(s+8,0);return s;}
U construct(U s,U id,Ports&p){base(s,p);p.object(s+16,p.cell(0x746635)&255,id,0);p.write(s,p.table(0x748da0));p.write(s+16,p.table(0x748cc0));for(U o=0x270;o<=0x280;o+=8)p.localized(s+o);U flags=p.read(s+0x288)&~2u;p.write(s+0x26c,0);p.write(s+0x288,flags);U ai=p.ai();p.register_ai(ai,s+16,0);p.write(s+0xe4,10001);p.write(s+0x268,p.cell(0x746644));for(U o=0x248;o<=0x254;o+=4)p.write(s+o,0);p.write(s+0x260,0);p.byte(s+0x238,0);p.write(s+0x258,50);p.write(s+0x239,0);p.write(s+0x23d,0);flags=p.read(s+0x288)&0xfffffe1a;p.write(s+0x244,0);flags|=0x18;p.write(s+0x294,0);p.byte(s+0x265,0);p.byte(s+0x266,0);p.write(s+0x28c,1);p.byte(s+0x264,1);p.write(s+0x288,flags);return s;}
}
