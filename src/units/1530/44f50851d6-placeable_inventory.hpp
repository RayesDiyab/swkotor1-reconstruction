#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct PlaceableInventoryPorts{virtual ~PlaceableInventoryPorts()=default;virtual Word objects(Word)=0;virtual Word lookup(Word,Word,Word)=0;virtual Word byte(Word)=0;virtual Word as_creature(Word)=0;virtual Word as_placeable(Word)=0;virtual Word as_item(Word)=0;virtual void remove_creature(Word,Word,Word,Word,Word)=0;virtual void remove_placeable(Word,Word)=0;virtual Word base_item(Word,Word)=0;virtual void remove_inventory(Word,Word)=0;virtual void clear_item(Word)=0;virtual void insert(Word,Word,Word,Word,Word)=0;virtual void owner(Word,Word,Word,Word,Word)=0;};
Word placeable_inventory_00584b10(Memory&,PlaceableInventoryPorts&,Word,Word,Word,Word,Word);
}
