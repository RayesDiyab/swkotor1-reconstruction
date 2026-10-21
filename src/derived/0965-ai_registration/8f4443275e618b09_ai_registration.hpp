#pragma once
#include <cstdint>
namespace recovered_ai_registration{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void grow(U,U)=0;virtual void remove(U,U)=0;};void append_unique(U,U,Ports&);U add(U,U,U,Ports&);}
