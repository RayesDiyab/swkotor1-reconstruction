#pragma once
#include <cstdint>
namespace recovered_item_properties{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void half(U,U)=0;virtual void byte(U,U)=0;virtual U list(U,U,U,const char*)=0;virtual U count(U,U)=0;virtual void element(U,U,U,U)=0;virtual U valid(U,U)=0;virtual U scalar(U,U,U,const char*,U,U)=0;virtual void release(U)=0;virtual U allocate(U)=0;virtual void continuation(U,U,U)=0;};bool special(U);void load(U,U,U,U,Ports&);}
