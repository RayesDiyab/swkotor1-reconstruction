#pragma once
#include "e713912a1f-module_initializers.hpp"
namespace kotor::server_mainloop {
// Return value models the original AL status. Upper EAX bits are unspecified.
Word object_register_004d80e0(Memory&,ModuleAllocation&,Word,Word,Word,Word);
}
