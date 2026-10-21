#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleDatePorts {virtual ~ModuleDatePorts()=default;virtual void write_byte(Word,Word)=0;virtual void update_004adbd0(Word)=0;};
void server_date_004b2320(Memory&,ModuleDatePorts&,Word,Word,Word,Word,Word,Word,Word,Word);
Word timer_milliseconds_004adcf0(Memory&,Word,Word,Word,Word,Word);
void timer_offset_004adde0(Memory&,ModuleDatePorts&,Word,Word,Word,Word);
void timer_date_004ae3e0(Memory&,ModuleDatePorts&,Word,Word,Word,Word,Word,Word,Word,Word,Word);
}
