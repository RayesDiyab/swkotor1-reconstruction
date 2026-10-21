#pragma once
namespace recovered_known_spell_add{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual void grow(U,U)=0;};void add(U,U,U,Ports&);}
