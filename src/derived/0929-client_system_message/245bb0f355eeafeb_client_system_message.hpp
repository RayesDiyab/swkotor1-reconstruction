#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ClientSystemPorts{virtual ~ClientSystemPorts()=default;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual Word counted(Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual Word parse(Word,Word,Word,Word,Word,Word)=0;virtual Word equal(Word,Word)=0;virtual Word info(Word)=0;virtual void module_name(Word,Word)=0;virtual Word cstr(Word)=0;virtual void log0(Word)=0;virtual void log1(Word,Word)=0;virtual void log3(Word,Word,Word,Word)=0;virtual void format2(Word,Word,Word,Word)=0;virtual void send(Word,Word)=0;virtual Word option(Word,Word,Word)=0;virtual Word integer(Word)=0;virtual Word character(Word,Word)=0;virtual Word left(Word,Word,Word)=0;virtual Word length(Word)=0;};
Word client_system_00675710(Memory&,ClientSystemPorts&,Word,Word,Word,Word);
}
