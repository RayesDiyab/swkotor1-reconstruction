#pragma once
#include <cstdint>
namespace recovered_module_event{
using U=std::uint32_t;
struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U read16(U)=0;virtual void write(U,U)=0;virtual void byte(U,U)=0;virtual U integer(U,U)=0;virtual U object(U,U)=0;virtual U real_bits(U,U)=0;virtual void saved_script(U vm,U data,U id,U flag)=0;virtual void script(U vm,U name,U id,U flag)=0;virtual U objects(U)=0;virtual void lookup(U,U,U)=0;virtual U creature(U)=0;virtual void remove(U,U)=0;virtual void destroy(U)=0;virtual void release(U)=0;};
void dispatch(U self,U kind,U source,U data,U day,U tick,U scratch,Ports&);
}
