#pragma once
#include <cstdint>
namespace recovered_placeable_light{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual void string(U)=0;virtual U table()=0;virtual U query(U,U,U,U)=0;virtual void destroy(U)=0;};U get(U,U,Ports&);void set(U,U,U,Ports&);}
