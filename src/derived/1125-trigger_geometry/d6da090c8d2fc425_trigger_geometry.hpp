#pragma once
#include <cstdint>
namespace recovered_trigger_geometry {
using U=std::uint32_t;
struct Ports {virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void free(U)=0;virtual U allocate(U)=0;virtual U count(U,U)=0;virtual U element(U,U,U,U)=0;virtual U type(U,U)=0;virtual U floating(U,U,U,U)=0;virtual void translate(U,U,U,U,U)=0;};
void load(U self,U gff,U list,U elementScratch,U statusScratch,Ports&);
}
