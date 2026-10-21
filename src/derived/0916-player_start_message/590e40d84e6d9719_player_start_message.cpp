#include "5207e04035e1514d_player_start_message.hpp"
namespace kotor::server_mainloop {
Word player_status_00568310(Memory&m,ModuleMessagePorts&p,Word self,Word recipient){Word outer=m.read(m.read(0x7a39fc)+8);const auto state=static_cast<std::int16_t>(m.read16(m.read(outer+4)+0x10008));if(state<0||state>2)return 0;
 Word data=p.allocate(3);m.write(self+0x4c,0xffffffff);if(!data)return 0;
 if(state==1){p.write_byte(data+1,1);p.write_byte(data+2,1);}else{p.write_byte(data+2,state==0?2:3);p.write_byte(data+1,1);}p.write_byte(data,0x50);
 Word server=m.read(m.read(0x7a39fc)+8);Word net=p.network_getter(server);Word result=p.send_005d4cb0(net,recipient,data,3,0);p.free(data);return result;
}
Word player_start_message_00524c40(Memory&m,ModuleMessagePorts&p,Word self,Word player,Word subtype){Word end=m.read(self+0x30);if(m.read(self+0x2c)<end)return 0;if(static_cast<unsigned char>(subtype))return 0;return player_status_00568310(m,p,self,m.read(player+4));}
}
