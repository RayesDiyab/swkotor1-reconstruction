#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaSafePositionPorts{virtual ~AreaSafePositionPorts()=default;virtual Word room(Word,Word,Word)=0;virtual Word height_bits(Word,Word,Word,Word,Word)=0;virtual Word valid(Word,Word,Word)=0;virtual Word search_004be93d(Word,Word,Word,Word,Word,Word,Word)=0;};
Word area_safe_position_004be860(Memory&,AreaSafePositionPorts&,Word,Word,Word,Word,Word,Word,Word);
}
