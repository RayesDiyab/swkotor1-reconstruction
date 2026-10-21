#include "595a1d1682c02e75_item_property_query.hpp"
namespace kotor::server_mainloop {
Word item_property_query_005539c0(Memory&m,Word self,Word output,Word type,Word subtype){
 subtype&=65535;type&=65535;
 for(unsigned list=0;list<2;++list){
  Word count=m.read(self+0x248+list*4),index=0,offset=0;
  if(static_cast<int>(count)<=0)continue;
  auto at=[&](){return static_cast<int>(offset)<0||static_cast<int>(index)>=static_cast<int>(count)?0:m.read(self+0x250+list*4)+offset;};
  do {
   Word flag=m.read8(at()+0x11);
   if(flag==255||(m.read(self+0x294)&(Word(1)<<(flag&31)))){
    if(m.read16(at())==type&&(!subtype||m.read16(at()+2)==subtype)){
     Word result=static_cast<int>(index)<0||static_cast<int>(index)>=static_cast<int>(count)?0:m.read(self+0x250+list*4)+index*28;
     m.write(output,result);return 1;
    }
   }
   ++index;offset+=28;
  }while(static_cast<int>(index)<static_cast<int>(count));
 }
 return 0;
}
}
