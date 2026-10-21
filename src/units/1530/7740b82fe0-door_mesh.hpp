#pragma once
#include <cstdint>
namespace recovered_door_mesh {using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual void base(U,U,U)=0;virtual U table()=0;virtual void write(U,U)=0;virtual void half(U,U)=0;virtual void byte(U,U)=0;virtual U constant(U)=0;};U construct(U,Ports&);void state(U,U,Ports&);}
