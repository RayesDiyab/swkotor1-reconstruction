#pragma once
#include <cstdint>
namespace recovered_area_properties_load{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U byte(U)=0;virtual void byte(U,U)=0;virtual U structure(U,U,U,U)=0;virtual U scalar(U,U,U,U,U,U)=0;virtual U party()=0;virtual void restrict_party(U,U)=0;virtual U child(U,U,U)=0;};U load(U,U,U,U,Ports&);}
