#pragma once
namespace recovered_stats_effect_query { using U=unsigned; struct Ports {virtual ~Ports()=default;virtual U read(U)=0;virtual U half(U)=0;virtual U rules_race()=0;virtual U integer(U,U)=0;}; U query(U self,U kind,U other,Ports&); }
