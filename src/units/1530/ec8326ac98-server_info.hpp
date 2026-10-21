#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ServerInfoPorts{virtual ~ServerInfoPorts()=default;virtual void empty(Word)=0;virtual void integer(Word,Word)=0;virtual void literal(Word,Word)=0;virtual Word concat(Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;virtual Word at(Word,Word)=0;virtual void next(Word,Word)=0;virtual Word network(Word)=0;virtual Word connection(Word,Word)=0;virtual Word cstr(Word)=0;virtual void assign_text(Word,Word)=0;virtual void copy(Word,Word)=0;};
Word server_info_004b41c0(Memory&,ServerInfoPorts&,Word,Word,Word);
}
