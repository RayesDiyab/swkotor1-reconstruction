#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaUpdatePorts{virtual~AreaUpdatePorts()=default;virtual Word objects(Word)=0;virtual Word lookup(Word,Word,Word)=0;virtual Word compare_status(Word,Word)=0;};
Word area_update_00506fe0(Memory&,AreaUpdatePorts&,Word area,Word object,Word scratch);
}
