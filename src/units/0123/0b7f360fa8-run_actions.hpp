#pragma once
#include "b748b65732-w1_ports.hpp"
// 0057F4A0 CSWSObject::RunActions (W1-owned runner; W5 owns the per-type action bodies).
namespace w1::run_actions {
// thiscall ret 10h: (worldDay, worldMs, hiResStart lo, hiResStart hi). `frame` = 0x38 bytes (frame+N == [esp+N]).
U run_actions_0057f4a0(U object, U day, U ms, U start_lo, U start_hi, U frame, Ports& p);
}
