#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleAllocation {virtual ~ModuleAllocation()=default;virtual Word allocate(Word)=0;};
Word module_list_004c4e20(Memory&,ModuleAllocation&,Word);
Word module_flags_0059afe0(Memory&,Word);
Word module_map_00578be0(Memory&,Word);
}
