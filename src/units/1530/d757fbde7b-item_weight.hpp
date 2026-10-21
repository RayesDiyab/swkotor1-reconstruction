#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {struct ItemWeightPorts{virtual ~ItemWeightPorts()=default;virtual Word item(Word,Word)=0;};Word item_weight_005562a0(Memory&,ItemWeightPorts&,Word);Word inventory_weight_00556100(Memory&,ItemWeightPorts&,Word);}
