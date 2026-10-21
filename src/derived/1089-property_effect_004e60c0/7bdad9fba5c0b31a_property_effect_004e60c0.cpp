#include "c654ee0ec6b28046_property_effect_004e60c0.hpp"
namespace kotor::server_mainloop {
Word property_cost_table_005c49a0(Memory&m,Word self,Word index){index&=255;if(index>=m.read8(self+8))return 0;Word tables=m.read(self);return tables?m.read(tables+index*4):0;}
Word property_effect_004e60c0(Memory&m,PropertyEffectPorts&p,Word item,Word property,Word creature,Word slot,Word flag,Word scratch){
 Word allocation=p.allocate(0x8c),effect=allocation?p.construct(allocation,1):0;
 p.string(scratch+0xc,0x744bfc);
 Word row=m.read16(property+6),table=p.table(m.read(m.read(0x7a3a28)+0xb8),20);
 p.integer_read(table,row,scratch+0xc,scratch+8);p.destroy(scratch+0xc);
 Word value=0-m.read(scratch+8);m.write(scratch+8,value);if(!value)return 0;
 Word type=m.read16(0x7466a8),flags=m.read16(effect+0xa);m.write16(effect+8,type);m.write16(effect+0xa,(flags&0xfff8)|m.read16(0x7473bc));
 p.creator(effect,m.read(item+0x14));p.integer(effect,0,m.read(scratch+8));p.integer(effect,2,m.read8(m.read(0x7a3a28)+0xaa));
 Word cell=0;switch(slot){case 16:cell=0x7468c8;break;case 32:cell=0x7468cc;break;case 0x4000:cell=0x7468d0;break;case 0x8000:cell=0x7468d4;break;case 0x10000:cell=0x7468d8;break;case 8:cell=0x7468dc;break;default:break;}if(cell)p.integer(effect,1,m.read(cell));
 Word id=m.read(item+0xc),base=p.base(m.read(m.read(0x7a3a28)+0x34),id);
 if(m.read8(base+8)==3){allocation=p.allocate(0x8c);Word second=allocation?p.construct(allocation,0):0;p.copy(second,effect,0);p.integer(second,1,m.read(0x7468cc));p.apply(creature,second,flag,0);}
 p.apply(creature,effect,flag,0);return 0;
}
}
