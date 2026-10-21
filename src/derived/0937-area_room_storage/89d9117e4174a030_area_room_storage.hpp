#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaRoomStoragePorts {virtual ~AreaRoomStoragePorts()=default;virtual Word allocate(Word)=0;virtual void free(Word)=0;virtual void construct_string(Word)=0;virtual void destroy_string(Word)=0;};
void area_rooms_allocate_00506910(Memory&,AreaRoomStoragePorts&,Word,Word);
}
