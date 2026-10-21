#pragma once
namespace recovered_effect_copy{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U half(U)=0;virtual void write(U,U)=0;virtual void put_half(U,U)=0;virtual U allocate(U)=0;virtual void release(U)=0;virtual U construct(U,U)=0;virtual void zero(U,U)=0;virtual void string_assign(U,U)=0;};void resize(U,U,Ports&);void copy(U,U,U,Ports&);}
