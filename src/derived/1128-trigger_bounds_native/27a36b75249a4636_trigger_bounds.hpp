#pragma once
#include <cstdint>
namespace recovered_trigger_bounds{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual bool equal(U,U)=0;virtual bool less(U,U)=0;virtual bool greater(U,U)=0;};void bounds(U,U,U,U,U,Ports&);}
