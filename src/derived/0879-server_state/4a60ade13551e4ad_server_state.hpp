#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct StatePorts {virtual ~StatePorts()=default;virtual Wide high_time_005e9260(Word)=0;virtual Word list_at_005e9b60(Word,Word)=0;virtual void list_next_005e9b90(Word,Word)=0;virtual void update_player_004b3ec0(Word,Word,Word,Wide)=0;};
void update_state_004b6950(Memory&,StatePorts&,Word,Word,Word);
}
