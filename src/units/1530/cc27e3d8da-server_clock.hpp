#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ClockPorts {virtual ~ClockPorts()=default;virtual void world_timer_reset_004adc70(Word)=0;virtual Word low_time_005e9250(Word)=0;virtual Wide divide_006fb030(Wide,Wide)=0;virtual void notify_timeout_0056b2b0(Word,Word,Word)=0;};
Word update_004b1ee0(Memory&,ClockPorts&,Word);
Word tail_004b1e10(Memory&,Word,Wide);
Word timeout_004b4ab0(Memory&,ClockPorts&,Word,Wide);
Word load_mode_005edfb0(Memory&,Word);
}
