#pragma once
#include <cstdint>
namespace recovered_server_stats{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U read8(U)=0;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual void half(U,U)=0;virtual void localized(U)=0;virtual void ref(U)=0;virtual void string(U)=0;virtual U allocate(U)=0;virtual U skills()=0;virtual void clear(U,U)=0;virtual void grow(U,U)=0;virtual void ref_empty(U,U)=0;virtual void literal_empty(U)=0;virtual U combat(U)=0;virtual U race_default()=0;};U class_info(U,Ports&);U level_info(U,Ports&);U construct(U,U,U,Ports&);}
