#include "4a60ade13551e4ad_server_state.hpp"
namespace kotor::server_mainloop {
void update_state_004b6950(Memory& m,StatePorts& p,Word self,Word flags,Word scratch){
 const Wide now=p.high_time_005e9260(m.read(m.read(0x7a39e0)+4));
 m.write(scratch,m.read(m.read(m.read(self+0x10064))));
 while(m.read(scratch)){
  const Word player=p.list_at_005e9b60(m.read(m.read(self+0x10064)),m.read(scratch));
  p.update_player_004b3ec0(self,player,flags,now);
  p.list_next_005e9b90(m.read(m.read(self+0x10064)),scratch);
 }
}
}
