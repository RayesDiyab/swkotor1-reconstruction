#pragma once
#include <cstdint>
namespace recovered_item_template{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U allocate(U)=0;virtual U construct(U,U,const char*,U)=0;virtual U read(U)=0;virtual void destroy(U)=0;virtual void root(U,U)=0;virtual void fields(U,U,U)=0;};U load(U,U,U,Ports&);}
