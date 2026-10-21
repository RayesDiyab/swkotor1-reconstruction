#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RoomHeightPorts{virtual ~RoomHeightPorts()=default;virtual Word plane(Word,Word,Word)=0;};
Word room_height_bits_00579470(Memory&,RoomHeightPorts&,Word,Word,Word,Word,Word,Word);
}
