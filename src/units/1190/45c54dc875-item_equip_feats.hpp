#pragma once
#include <cstdint>
namespace recovered_item_equip_feats{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U half(U)=0;virtual U rule(U)=0;virtual U feat(U,U)=0;};U rule_feat(U,U,Ports&);U check(U,U,U,Ports&);}
