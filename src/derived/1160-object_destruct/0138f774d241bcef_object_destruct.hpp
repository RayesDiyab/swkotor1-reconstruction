#pragma once
#include <cstdint>
namespace recovered_object_destruct{using U=std::uint32_t;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U table(U)=0;virtual void virtual_destroy(U,U)=0;virtual void destroy(U,U)=0;virtual void release(U)=0;virtual U app()=0;virtual U objects(U)=0;virtual void remove(U,U)=0;virtual U pop(U)=0;};void destroy(U,Ports&);}
