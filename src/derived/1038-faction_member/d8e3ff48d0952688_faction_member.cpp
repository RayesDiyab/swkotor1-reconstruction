#include "09d06f47a7e112b7_faction_member.hpp"
namespace kotor::server_mainloop {
void faction_member_005bfa70(Memory&m,FactionMemberPorts&p,Word self,Word id,Word skip){Word creature=p.creature(m.read(m.read(0x7a39fc)+8),id);if(!creature)return;Word old=p.current(creature),count=m.read(self+4),data=m.read(self);for(Word i=0;static_cast<int>(i)<static_cast<int>(count);++i)if(m.read(data+i*4)==id)return;if(old)p.remove(old,id);if(!skip)p.notification(self,id);p.append(self,id);Word stats=m.read(creature+0xa74);m.write(stats+0x78,m.read(self+0xc));Word message=p.message(m.read(m.read(0x7a39fc)+8));Word player=p.player(m.read(m.read(0x7a39fc)+8),id);if(player){Word data2=m.read(self),count2=m.read(self+4),playerId=m.read(player+4);p.send(message,playerId,count2,data2,1,id);}}
}
