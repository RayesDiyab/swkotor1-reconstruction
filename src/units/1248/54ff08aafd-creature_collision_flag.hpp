#pragma once
namespace recovered_creature_collision_flag{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U disabled()=0;virtual int relation(U,U,U)=0;virtual U server()=0;virtual U selected_id(U)=0;virtual U creature(U,U)=0;};U query(U,U,Ports&);}
