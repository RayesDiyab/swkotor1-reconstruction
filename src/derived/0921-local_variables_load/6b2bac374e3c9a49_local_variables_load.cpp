#include "5d5f5869fedd1d6d_local_variables_load.hpp"
namespace kotor::server_mainloop {
void local_variables_load_0059aa80(Memory&m,LocalVariablesLoadPorts&p,Word self,Word gff,Word root,Word scratch){auto s=[&](Word n){return scratch+n;};m.write(s(0x14),self);p.location_construct(s(0x68));p.empty(s(0x24));p.empty(s(0x2c));
 if(p.list(gff,s(0x54),root,0x74a11c)){m.write(s(0x18),0);if(p.count(gff,s(0x54))>0){Word index=0;do{
  p.element(gff,s(0x0c),s(0x54),index);p.literal(s(0x34),0x73d71c);Word name=p.string(gff,s(0x44),s(0x0c),0x745eec,s(0x10),s(0x34));p.assign(s(0x24),name);p.destroy(s(0x44));p.destroy(s(0x34));
  Word type=p.dword(gff,s(0x0c),0x745e38,s(0x10),0);
  switch(type){case 1:{Word value=p.integer(gff,s(0x0c),0x744bfc,s(0x10),0);p.set_integer(self,s(0x24),value,1);break;}
  case 2:{Word bits=p.float_bits(gff,s(0x0c),0x744bfc,s(0x10),0);m.write(s(0x1c),bits);Word entry=p.obtain(self,s(0x24),2,1);m.write(entry+12,m.read(s(0x1c)));break;}
  case 3:{p.literal(s(0x3c),0x73d71c);Word value=p.string(gff,s(0x4c),s(0x0c),0x744bfc,s(0x10),s(0x3c));p.assign(s(0x2c),value);p.destroy(s(0x4c));p.destroy(s(0x3c));Word entry=p.obtain(self,s(0x24),3,1);p.assign(m.read(entry+12),s(0x2c));break;}
  case 4:{Word value=p.dword(gff,s(0x0c),0x744bfc,s(0x10),0);Word entry=p.obtain(self,s(0x24),4,1);m.write(entry+12,value);break;}
  case 5:{p.structure(gff,s(0x20),s(0x0c),0x744bfc);p.location_load(s(0x68),gff,s(0x20));Word a=m.read(s(0x68)),b=m.read(s(0x6c)),c=m.read(s(0x70)),d=m.read(s(0x74)),e=m.read(s(0x78)),f=m.read(s(0x7c));p.set_location(m.read(s(0x14)),s(0x24),a,b,c,d,e,f);self=m.read(s(0x14));break;}
  default:break;}
  index=m.read(s(0x18))+1;m.write(s(0x18),index);
 }while(index<p.count(gff,s(0x54)));}}
 p.destroy(s(0x2c));p.destroy(s(0x24));p.location_destroy(s(0x68));
}
}
