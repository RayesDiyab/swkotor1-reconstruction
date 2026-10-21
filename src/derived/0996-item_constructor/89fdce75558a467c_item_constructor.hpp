#pragma once
#include <cstdint>
namespace recovered_item_constructor{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual void half(U,U)=0;virtual U table(U)=0;virtual U cell(U)=0;virtual void object(U,U,U,U)=0;virtual void localized(U)=0;virtual U ai()=0;virtual void register_ai(U,U,U)=0;};U base(U,Ports&);U construct(U,U,Ports&);}
