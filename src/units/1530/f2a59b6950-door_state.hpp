#pragma once
#include <cstdint>
namespace recovered_door_state{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void byte(U,U)=0;virtual void animation(U,U)=0;};void set(U,U,U,Ports&);}
