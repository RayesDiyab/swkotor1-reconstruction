#pragma once
#include <cstdint>
namespace recovered_item_equip_slots{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual U item(U)=0;virtual U required_feats(U,U,U)=0;virtual void signal(U,U)=0;virtual U rule(U)=0;};U slot(U,U);U equipped(U,U,Ports&);U other(U,U,U,U,U,Ports&);U weapon(U,U,U,U,U,Ports&);}
