#pragma once
#include <cstdint>
namespace recovered_creature_priority{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write(U,U)=0;virtual U player(U)=0;virtual U client()=0;virtual U info(U)=0;virtual U party(U)=0;virtual U character(U,U)=0;virtual U client_id(U,U)=0;virtual U dead(U)=0;virtual U incapacitated(U)=0;virtual U render(U)=0;virtual U model(U)=0;virtual U animation(U,U)=0;virtual void dirty(U,U)=0;};void set(U,U,Ports&);}
