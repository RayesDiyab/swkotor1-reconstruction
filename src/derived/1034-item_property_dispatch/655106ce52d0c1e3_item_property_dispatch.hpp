#pragma once
#include <cstdint>
namespace recovered_item_property_dispatch{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U half(U)=0;virtual U invoke(U,U,U,U,U,U,U)=0;};U apply(U,U,U,U,U,U,Ports&);}
