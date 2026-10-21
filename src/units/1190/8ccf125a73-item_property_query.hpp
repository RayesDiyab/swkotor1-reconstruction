#pragma once
#include <cstdint>
namespace recovered_item_property_query{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U half(U)=0;};U has(U,U,U,Ports&);}
