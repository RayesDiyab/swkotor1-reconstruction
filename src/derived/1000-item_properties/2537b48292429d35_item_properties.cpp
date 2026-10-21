#include "c6dee77bf0c504ed_item_properties.hpp"
namespace recovered_item_properties{
bool special(U v){v&=65535;return v==10||v==37||v==46||v==53;}
void load(U self,U g,U root,U s,Ports&p){U list=s,element=s+20,status=s+24;auto value=[&](U type,const char*n,U def){return p.scalar(type,g,element,n,status,def);};
 if(p.list(g,list,root,"PropertiesList")==1&&p.count(g,list)>0){U total=p.count(g,list),selected=0;
  for(U i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(total);++i){p.element(g,element,list,i);if(p.valid(g,element)){U prop=value(2,"PropertyName",0);if(p.read(status)&&special(prop))++selected;}}
  for(U group=0;group<2;++group){U countoff=group?0x24c:0x248,ptroff=group?0x254:0x250;U count=group?total-p.read(self+0x248):selected;U old=p.read(self+ptroff);p.write(self+countoff,count);if(old)p.release(old);p.write(self+ptroff,count?p.allocate(p.read(self+countoff)*28):0);
   U output=0,offset=0;auto address=[&](){return static_cast<std::int32_t>(offset)>=0&&static_cast<std::int32_t>(output)<static_cast<std::int32_t>(p.read(self+countoff))?p.read(self+ptroff)+offset:0;};
   for(U i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(total);++i){p.element(g,element,list,i);if(!p.valid(g,element))continue;U prop=value(2,"PropertyName",0);if(special(prop)!=(group==0))continue;
    if(p.read(status))p.half(address(),prop);
    U v=value(2,"Subtype",0);if(p.read(status))p.half(address()+2,v);
    v=value(0,"CostTable",0);if(p.read(status))p.byte(address()+4,v);
    v=value(2,"CostValue",0);if(p.read(status))p.half(address()+6,v);
    v=value(0,"Param1",0);if(p.read(status))p.byte(address()+8,v);
    v=value(0,"Param1Value",0);if(p.read(status))p.byte(address()+9,v);
    v=value(0,"ChanceAppear",0);if(p.read(status))p.byte(address()+10,v);
    v=value(0,"Useable",group?0:1);p.write(address()+12,v&255);
    v=value(0,"UsesPerDay",255);p.byte(address()+16,v);
    v=value(0,"UpgradeType",255);U target=address();++output;p.byte(target+17,v);offset+=28;
   }
  }
 }
 p.continuation(self,g,root);
}}
