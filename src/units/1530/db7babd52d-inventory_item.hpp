#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct InventoryItemPorts{virtual ~InventoryItemPorts()=default;virtual Word item(Word,Word)=0;};
Word inventory_item_005560b0(Memory&,InventoryItemPorts&,Word,Word);
}
