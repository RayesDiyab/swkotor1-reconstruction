#pragma once
#include <cstdint>
namespace recovered_party_health{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U client()=0;virtual U party(U)=0;virtual U character(U,U)=0;virtual U client_id(U,U)=0;virtual U current(U)=0;virtual U base_dead(U)=0;virtual U lookup(U,U)=0;virtual U renderer(U)=0;virtual void attach(U,U)=0;};U dead(U,Ports&);U incapacitated(U,Ports&);U render(U,Ports&);}
