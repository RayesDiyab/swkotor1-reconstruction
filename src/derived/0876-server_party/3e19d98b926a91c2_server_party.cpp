#include "41107dc43edc9ed0_server_party.hpp"
namespace kotor::server_mainloop {
static VectorWords vector_at(Memory& m,Word a){return {m.read(a),m.read(a+4),m.read(a+8)};}
void party_positions_004b4e80(Memory& m,PartyPositionPorts& p,Word self){
 const Word party=p.party_005ed8b0(m.read(m.read(0x007a39fc)+4));
 const Word module=p.module_004b14f0(self);
 if(!party||!module)return;
 const Word map=m.read(module+0x218);
 if(!map)return;
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(m.read(party));++i){
  const Word character=p.character_006346c0(party,i);
  if(!character)continue;
  p.reveal_005792d0(map,vector_at(m,character+0x24),1);
  p.map_position_005790c0(map,i,vector_at(m,character+0x24));
  p.map_orientation_00579130(map,i,vector_at(m,character+0x30));
 }
}
}
