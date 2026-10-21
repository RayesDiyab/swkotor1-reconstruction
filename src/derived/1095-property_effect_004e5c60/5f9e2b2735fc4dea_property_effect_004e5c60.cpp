#include "4900f0505da19c22_property_effect_004e5c60.hpp"
namespace kotor::server_mainloop {
Word property_effect_004e5c60(Memory&m,PropertyEffectPorts&p,Word item,Word property,Word creature,Word slot,Word flag){
 Word allocation=p.allocate(0x8c),effect=allocation?p.construct(allocation,1):0,amount=m.read16(property+6);if(!amount)return 0;
 Word type=m.read16(0x7466b0),flags=m.read16(effect+0xa);m.write16(effect+8,type);m.write16(effect+0xa,(flags&0xfff8)|m.read16(0x7473bc));
 p.creator(effect,m.read(item+0x14));p.integer(effect,0,amount);
 Word shift=m.read16(property)==11?m.read16(property+2):m.read8(property+9);p.integer(effect,1,1u<<(shift&31));
 Word cell=0;switch(slot){case 16:cell=0x7468e4;break;case 32:cell=0x7468e8;break;case 0x4000:cell=0x7468ec;break;case 0x8000:cell=0x7468f0;break;case 0x10000:cell=0x7468f4;break;case 8:cell=0x7468f8;break;default:break;}if(cell)p.integer(effect,5,m.read(cell));
 p.integer(effect,2,m.read8(m.read(0x7a3a28)+0xaa));
 Word kind=m.read16(property);if(kind==12){Word subtype=m.read16(property+2);if(subtype==1)p.integer(effect,3,1);else if(subtype==2)p.integer(effect,4,2);else if(subtype==3)p.integer(effect,4,3);}else if(kind==13)p.integer(effect,2,m.read16(property+2));
 Word id=m.read(item+0xc),base=p.base(m.read(m.read(0x7a3a28)+0x34),id);
 if(m.read8(base+8)==3){allocation=p.allocate(0x8c);Word second=allocation?p.construct(allocation,0):0;p.copy(second,effect,0);p.integer(second,5,m.read(0x7468e8));p.apply(creature,second,flag,0);}
 p.apply(creature,effect,flag,0);return 0;
}
}
