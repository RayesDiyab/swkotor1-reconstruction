#pragma once
namespace recovered_creature_inventory_get{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U server()=0;virtual U client()=0;virtual U party(U)=0;virtual U shared(U)=0;virtual U module(U)=0;virtual U member(U,U)=0;virtual U gui(U)=0;virtual U gui_state(U,U)=0;};U get(U,U,Ports&);}
