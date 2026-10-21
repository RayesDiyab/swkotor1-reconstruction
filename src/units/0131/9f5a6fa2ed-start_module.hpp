#pragma once
#include "93d73ae698-w1_ports.hpp"
// Tier 9: 004BA920 CServerExoAppInternal::StartNewModule(CExoString* module name), thiscall ret 4. Frame 0x2C.
namespace w1::start_module {
U start_new_module_004ba920(U internal, U name, U frame, Ports& p);
U get_module_004b14f0(U internal, U scratch, Ports& p);   // CServerExoAppInternal::GetModule (cached), ret
}
