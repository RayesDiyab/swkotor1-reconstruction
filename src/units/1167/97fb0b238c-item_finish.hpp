#pragma once
#include <cstdint>
namespace recovered_item_finish{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U word(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual void put_byte(U,U)=0;virtual void recalculate(U)=0;virtual U rule(U)=0;virtual U module()=0;virtual void register_tag(U,U,U)=0;};void uses(U,U,Ports&);void finish(U,Ports&);}
