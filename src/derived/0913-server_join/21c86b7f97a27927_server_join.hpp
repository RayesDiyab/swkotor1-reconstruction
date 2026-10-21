#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ServerJoinPorts{virtual ~ServerJoinPorts()=default;virtual void empty(Word)=0;virtual void format(Word,Word,Word,Word)=0;virtual Word info(Word,Word)=0;virtual Word concat(Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;virtual Word length(Word)=0;virtual Word cstr(Word)=0;virtual Word allocate(Word)=0;virtual void write_byte(Word,Word)=0;virtual Word network(Word)=0;virtual Word send(Word,Word,Word,Word,Word)=0;virtual void free(Word)=0;};
Word status_packet_00579890(Memory&,ServerJoinPorts&,Word,Word,Word);
void joined_00579a80(Memory&,ServerJoinPorts&,Word,Word,Word);
}
