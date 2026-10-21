#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct DoorFieldPorts{virtual ~DoorFieldPorts()=default;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word byte(Word,Word,Word,Word,Word)=0;virtual Word short_value(Word,Word,Word,Word,Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual void write_byte(Word,Word)=0;virtual void state(Word,Word)=0;virtual Word current_hp(Word)=0;virtual void hitpoints(Word,Word)=0;virtual Word continuation_0058a424(Word,Word,Word,Word,Word)=0;};
Word door_fields_0058a1f0(Memory&,DoorFieldPorts&,Word,Word,Word,Word,Word);
}
