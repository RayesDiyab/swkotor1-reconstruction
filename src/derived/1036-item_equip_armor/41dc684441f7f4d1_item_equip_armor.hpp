#pragma once
namespace recovered_item_equip_armor {using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual void write_byte(U,U)=0;virtual U equipment(U,U)=0;virtual U rule(U)=0;};void update(U,Ports&);}
