#pragma once
#include <cstdint>
namespace recovered_minimap {
using U=std::uint32_t;using I=std::int32_t;
struct Ports{virtual ~Ports()=default;virtual U read(U,U)=0;virtual void write(U,U,U)=0;virtual I height(I)=0;virtual U reciprocal(I)=0;virtual U allocate(U)=0;virtual void clear(U,U)=0;virtual void mapping(U,const U*)=0;};
// Twelve original stack words. Floats remain raw IEEE bits at this boundary.
void setup(U self,const U* args,Ports&);
}
