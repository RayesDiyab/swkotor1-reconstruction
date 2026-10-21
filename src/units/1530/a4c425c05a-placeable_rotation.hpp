#pragma once
#include <cstdint>
namespace recovered_placeable_rotation{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void derive(U,U)=0;virtual U normalize(U,U)=0;virtual void orientation(U,U)=0;};void rotate(U,U,U,Ports&);}
