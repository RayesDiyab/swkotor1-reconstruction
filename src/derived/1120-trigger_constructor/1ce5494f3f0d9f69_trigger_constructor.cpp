#include "141b27c886d8bd4d_trigger_constructor.hpp"
#include <initializer_list>
namespace recovered_trigger_constructor{U construct(U s,U id,U scratch,Ports&p){
 p.base(s,7,id,0);p.write(s,p.table());p.localized(s+0x228);p.string(s+0x230);p.string(s+0x238);
 for(U i=0;i<7;++i){p.string(s+0x244+i*8);}p.string(s+0x27c);
 p.write(s+0x2ac,0);p.write(s+0x2b0,0);p.write(s+0x2a8,0);p.localized(s+0x30c);
 U ai=p.ai();p.register_ai(ai,s,0);p.write(s+0xd4,0x2711);
 p.construct_string(scratch,"PrimaryRange");U rules=p.rules(),manager=p.read(rules+0xb8),table=p.read(manager+0xc4);U okay=p.table_float(table,17,scratch,0x7a206c);p.destroy_string(scratch);if(!okay)p.write(0x7a206c,0x3f800000);
 p.literal(s+0x230,"");
 for(U o:{0x284u,0x288u,0x28cu,0x290u,0x294u,0x298u,0x2f8u})p.write(s+o,0);
 for(U o:{0x29cu,0x2a0u,0x2a4u})p.write(s+o,0x7f000000);
 p.write(s+0x2b4,0);p.write(s+0x2bc,0);p.write(s+0x2c0,1);p.write(s+0x2c4,1);p.write(s+0x2c8,0);p.literal(s+0x27c,"");
 p.write(s+0x2d0,1);p.write(s+0x2cc,0);p.byte(s+0x2d4,255);p.write(s+0x2b8,0);p.write(s+0x2d8,0);p.write(s+0x2dc,0);
 for(U i=0;i<7;++i)p.literal(s+0x244+i*8,"default");
 for(U o:{0x2e8u,0x2ecu,0x2f0u,0x2f4u}){p.write(s+o,0);}p.byte(s+0x2fc,0);p.write(s+0x300,0);p.write(s+0x304,0);p.write(s+0x2e4,0x7f000000);p.write(s+0x2e0,0x3dcccccd);p.half(s+0x308,0);p.half(s+0x30a,0);p.write(s+0x314,0);p.state(s,0x279f);return s;
}}
