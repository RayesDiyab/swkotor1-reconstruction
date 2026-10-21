#pragma once
#include "fea6560366-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaRegionPorts{virtual ~AreaRegionPorts()=default;virtual Word detailed(Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;};
Word area_region_005086d0(Memory&,AreaRegionPorts&,Word,Word,Word,Word,Word,Word,Word,Word,Word,Word);
}
