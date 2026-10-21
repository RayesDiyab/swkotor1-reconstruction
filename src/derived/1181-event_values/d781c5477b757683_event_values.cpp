#include "29ddd480658a8dfe_event_values.hpp"
namespace recovered_event_values{
U integer(U s,U i,Ports&p){if(static_cast<std::int32_t>(i)>=static_cast<std::int32_t>(p.read(s+8)))return 0;return p.read(p.read(s+4)+i*4);}
U real(U s,U i,Ports&p){U bits=0;if(static_cast<std::int32_t>(i)<static_cast<std::int32_t>(p.read(s+0x14)))bits=p.read(p.read(s+0x10)+i*4);return p.spill(bits);}
U object(U s,U i,Ports&p){if(static_cast<std::int32_t>(i)>=static_cast<std::int32_t>(p.read(s+0x2c)))return 0x7f000000;return p.read(p.read(s+0x28)+i*4);}
}
