#pragma once
#include <cstdint>
namespace recovered_trigger_creator{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U server()=0;virtual U player_id(U)=0;virtual U party(U)=0;};U member(U,U,Ports&);void set(U,U,Ports&);}
