#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaHeightPorts{virtual~AreaHeightPorts()=default;virtual Word find(Word,Word,Word)=0;virtual Word ground(Word,Word,Word,Word,Word)=0;virtual Word alternate(Word,Word,Word,Word,Word)=0;};
Word area_height_bits_004bc380(Memory&,AreaHeightPorts&,Word area,Word point,Word heightFlag,Word roomOut,Word scratch);
}
