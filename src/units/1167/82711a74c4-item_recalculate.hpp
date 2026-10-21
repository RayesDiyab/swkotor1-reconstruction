#pragma once
#include <cstdint>
namespace recovered_item_recalculate{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U word(U)=0;virtual U byte(U)=0;virtual void put_byte(U,U)=0;virtual U message()=0;virtual U player(U)=0;virtual void notify(U,U,U,U,U,U)=0;};U charges(U,U,Ports&);void recalculate(U,U,Ports&);}
