#pragma once
namespace recovered_stats_attack_helpers{using U=unsigned;struct Ports{virtual~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U restricted(U)=0;virtual U rule(U)=0;};U finesse(U,U,Ports&);U weapon_size(U,U,Ports&);}
