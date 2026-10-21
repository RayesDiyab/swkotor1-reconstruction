#pragma once
#include <cstdint>
namespace recovered_item_identity{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void release(U)=0;virtual U rule(U)=0;virtual U allocate(U)=0;};void reset(U,U,Ports&);}
