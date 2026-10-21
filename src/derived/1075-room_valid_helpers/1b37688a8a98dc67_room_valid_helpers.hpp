#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomValidHelperPorts{virtual ~RoomValidHelperPorts()=default;virtual Word collision(Word,Word,Word,Word,Word,Word,Word)=0;};
Word room_sweep_00579680(Memory&,RoomValidHelperPorts&,Word,Word,Word,Word,Word,Word,Word);
Word room_adjacent_00579420(Memory&,Word,Word);
}
