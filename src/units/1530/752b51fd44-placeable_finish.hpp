#pragma once
#include <cstdint>
namespace recovered_placeable_finish{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U byte(U)=0;virtual U half(U)=0;virtual void write(U,U)=0;virtual void write_half(U,U)=0;virtual U allocate(U)=0;virtual U construct(U,U)=0;virtual U constant(U)=0;virtual void integer(U,U,U)=0;virtual void apply(U,U,U,U)=0;};void finish(U,Ports&);}
