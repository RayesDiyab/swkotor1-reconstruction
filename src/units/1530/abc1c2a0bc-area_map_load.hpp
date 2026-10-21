#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop{struct AreaMapLoadPorts{virtual ~AreaMapLoadPorts()=default;virtual Word structure(Word,Word,Word,Word)=0;virtual Word present(Word,Word,Word,Word)=0;};Word area_map_load_00505da0(Memory&,AreaMapLoadPorts&,Word,Word,Word,Word);}
