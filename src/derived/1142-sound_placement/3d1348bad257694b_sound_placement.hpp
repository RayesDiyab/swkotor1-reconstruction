#pragma once
#include <cstdint>
namespace recovered_sound_placement{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U area(U)=0;virtual void remove(U,U)=0;virtual void set_area(U,U)=0;virtual void insert(U,U,U)=0;virtual U server()=0;virtual U module(U)=0;virtual void tag(U,U,U)=0;};void place(U,U,U,Ports&);}
