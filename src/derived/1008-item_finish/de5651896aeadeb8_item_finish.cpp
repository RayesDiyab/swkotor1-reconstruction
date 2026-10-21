#include "7f2445d7b0f50359_item_finish.hpp"
namespace recovered_item_finish{
void uses(U self,U only_unset,Ports&p){U count=p.read(self+0x248),index=0,offset=0;while(static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count)){if(static_cast<std::int32_t>(offset)>=0&&static_cast<std::int32_t>(index)<static_cast<std::int32_t>(count)){U entry=p.read(self+0x250)+offset;if(entry&&p.word(entry)==10&&(!only_unset||p.byte(entry+16)==255)){U cost=p.word(entry+6);if(cost>=8&&cost<=12)p.put_byte(entry+16,cost-7);p.write(entry+12,1);}}count=p.read(self+0x248);++index;offset+=28;}}
void finish(U self,Ports&p){uses(self,1,p);p.recalculate(self);if(!p.read(self+0x248)&&!p.read(self+0x24c))p.write(self+0x288,p.read(self+0x288)|1);U value=0;if(p.byte(p.rule(self)+0x18)==1)value=p.byte(p.rule(self)+0x8a);p.write(self+0x244,value);U id=p.read(self+0x14),tag=self+0x28;U mod=p.module();p.register_tag(mod,tag,id);p.write(self+0x290,p.byte(p.rule(self)+0x89));}
}
