#pragma once
#include <cstdint>
namespace recovered_sound_constructor{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual void base(U,U,U,U)=0;virtual U table()=0;};U construct(U,U,Ports&);}
