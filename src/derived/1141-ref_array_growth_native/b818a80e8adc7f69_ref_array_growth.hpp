#pragma once
#include <cstdint>
namespace recovered_ref_array_growth{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U allocate(U)=0;virtual void release(U)=0;virtual void construct(U)=0;virtual void assign(U,U,U)=0;};void grow(U,U,U,Ports&);}
