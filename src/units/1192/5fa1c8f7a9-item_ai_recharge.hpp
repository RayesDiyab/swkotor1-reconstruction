#pragma once
#include <cstdint>
namespace recovered_item_ai_recharge {
using U=std::uint32_t;
struct Ports {
 virtual ~Ports()=default;
 virtual U read(U)=0;
 virtual U read16(U)=0;
 virtual U read8(U)=0;
 virtual void write(U,U)=0;
 virtual U has(U,U,U)=0;
 virtual U timer(U)=0;
 virtual void time(U,U,U)=0;
 virtual void difference(U,U,U,U,U,U,U)=0;
};
void update(U secondary,U scratch,Ports&);
U difference(U timer,U day1,U tick1,U day2,U tick2,U outDay,U outTick,Ports&);
}
