#pragma once
namespace recovered_area_insert_index{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual void write(U,U)=0;virtual U objects()=0;virtual U lookup(U,U,U)=0;virtual U success()=0;virtual bool below_or_unordered(U,U)=0;};U find(U,U,U,U,Ports&);}
