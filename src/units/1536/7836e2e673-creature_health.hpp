#pragma once
#include <cstdint>
namespace recovered_creature_health{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U half(U)=0;virtual U party()=0;virtual U slot(U,U,U,bool)=0;virtual U ratio(U,U)=0;};U level(U,U,Ports&);U level_info(U,U,Ports&);U has_feat(U,U,Ports&);U current(U,U,Ports&);U maximum(U,U,Ports&);U low(U,Ports&);U base_dead(U,Ports&);}
