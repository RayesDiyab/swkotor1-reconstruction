#pragma once
namespace recovered_placeable_mesh{using U=unsigned;struct Ports{virtual~Ports()=default;virtual void base(U,U,U)=0;virtual void write(U,U)=0;virtual U table()=0;};U construct(U,Ports&);}
