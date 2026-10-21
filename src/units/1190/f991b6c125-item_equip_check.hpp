#pragma once
#include <cstdint>
namespace recovered_item_equip_check{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U level(U)=0;virtual U item_level(U)=0;virtual U options()=0;virtual void signal(U,U)=0;virtual U predicate(U,U,U,U)=0;virtual U choose(U,U,U,U,U,bool)=0;virtual U rule(U)=0;};U check(U,U,U,U,U,U,Ports&);}
