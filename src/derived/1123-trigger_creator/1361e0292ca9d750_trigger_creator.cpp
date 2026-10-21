#include "b541422f1757d20e_trigger_creator.hpp"
namespace recovered_trigger_creator{
U member(U self,U id,Ports&p){U index=0;while(index<9){if(p.read(self+12+index*4)==id)break;++index;}if(index==9)return 0;U count=p.read(self);if(static_cast<std::int32_t>(count)<=0)return 0;for(U i=0;i<count;++i)if(p.read(self+4+i*4)==index)return 1;return 0;}
void set(U self,U id,Ports&p){p.write(self+0x2e4,id);if(id==0x7f000000){p.write(self+0x314,0);return;}U server=p.server();if(id==p.player_id(server)){p.write(self+0x314,1);return;}server=p.server();U party=p.party(server);p.write(self+0x314,member(party,id,p)?1:0);}
}
