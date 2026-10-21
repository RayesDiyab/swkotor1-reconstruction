#pragma once
namespace recovered_effect_collection{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U half(U)=0;virtual void write(U,U)=0;virtual void release(U)=0;virtual void required(U)=0;virtual U integer(U,U)=0;virtual void grow(U,U)=0;virtual void copy(U,U)=0;virtual bool rank(U,U,U,U)=0;};U collect(U self,U out,U kind,U scratch,Ports&);}
