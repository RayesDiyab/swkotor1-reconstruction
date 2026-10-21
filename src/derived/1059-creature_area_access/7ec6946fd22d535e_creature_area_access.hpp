#pragma once
namespace recovered_creature_area_access{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U invalid()=0;};void set(U,U,Ports&);}
