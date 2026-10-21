#pragma once
#include "d7e1c5d3e7335676_module_initializers.hpp"
namespace kotor::server_mainloop {
// Return value models the original AL status. Upper EAX bits are unspecified.
Word object_register_004d80e0(Memory&,ModuleAllocation&,Word,Word,Word,Word);
}
