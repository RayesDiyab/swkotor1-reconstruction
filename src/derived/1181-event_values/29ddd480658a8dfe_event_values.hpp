#pragma once
#include <cstdint>
namespace recovered_event_values{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U spill(U)=0;};U integer(U,U,Ports&);U real(U,U,Ports&);U object(U,U,Ports&);}
