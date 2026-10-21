#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleMessagePorts {virtual ~ModuleMessagePorts()=default;virtual Word allocate(Word)=0;virtual void free(Word)=0;virtual void write_byte(Word,Word)=0;virtual Word network_getter(Word)=0;virtual Word send_005d4cb0(Word,Word,Word,Word,Word)=0;};
void message_grow_004d6260(Memory&,ModuleMessagePorts&,Word,Word);
void message_begin_004d68a0(Memory&,ModuleMessagePorts&,Word,Word,Word,Word);
void message_word_004d6340(Memory&,ModuleMessagePorts&,Word,Word,Word);
Word message_finish_004d6440(Memory&,Word,Word,Word);
Word message_send_00567e60(Memory&,ModuleMessagePorts&,Word,Word,Word,Word,Word,Word);
void message_player_left_005682b0(Memory&,ModuleMessagePorts&,Word,Word,Word,Word);
void message_stall_0056c8b0(Memory&,ModuleMessagePorts&,Word,Word,Word);
void message_date_0056c910(Memory&,ModuleMessagePorts&,Word,Word,Word,Word);
}
