#pragma once
#include <cstdint>
namespace recovered_vm_stack_view{using U=std::uint32_t;using I=std::int32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual std::uint8_t byte(U)=0;virtual void destroy_string(U)=0;virtual void release(U)=0;virtual void destroy_engine(U implementer,I type,U payload)=0;};U delete_script(U descriptor,Ports&);void clear(U stack,Ports&);void shrink(U stack,I requested,Ports&);}
