#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaRegionPorts{virtual ~AreaRegionPorts()=default;virtual Word detailed(Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;};
Word area_region_005086d0(Memory&,AreaRegionPorts&,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word);
}
