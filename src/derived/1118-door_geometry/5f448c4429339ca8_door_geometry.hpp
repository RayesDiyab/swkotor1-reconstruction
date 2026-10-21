#pragma once
#include <cstdint>
namespace recovered_door_geometry{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U geometry(U,U,U,U,U)=0;};U calculate(U,U,U,U,U,U,U,U,Ports&);}
