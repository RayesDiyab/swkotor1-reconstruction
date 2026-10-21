#pragma once
#include <cstdint>
namespace recovered_camera_records{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U float_spill(U)=0;};U count(U,U,Ports&);void insert(U,U,U,U,U,U,U,U,U,Ports&);}
