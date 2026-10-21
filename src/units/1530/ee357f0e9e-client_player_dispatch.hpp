#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ClientDispatchPorts{virtual ~ClientDispatchPorts()=default;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual void initialize(Word,Word,Word,Word,Word)=0;virtual Word incomplete(Word,Word)=0;virtual Word remaining(Word,Word)=0;virtual void format_type(Word,Word,Word)=0;virtual void label(Word,Word)=0;virtual Word handler(Word,Word,Word)=0;virtual Word cstr(Word)=0;virtual void format_error(Word,Word,Word,Word)=0;};
Word client_player_dispatch_0066a640(Memory&,ClientDispatchPorts&,Word,Word,Word,Word);
}
