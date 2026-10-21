#pragma once
#include <cstdint>
namespace recovered_item_equip_restrictions{using U=std::uint32_t;struct Ports{virtual ~Ports()=default;virtual U read(U)=0;virtual U byte(U)=0;virtual U half(U)=0;virtual U has(U,U,U)=0;virtual U feat(U,U)=0;virtual U class_id(U,U)=0;virtual U feat_uses(U,U)=0;virtual U find(U,U,U)=0;virtual U rule(U)=0;};U alignment(U,U,Ports&);U classes(U,U,Ports&);U race(U,U,Ports&);U feats(U,U,Ports&);}
