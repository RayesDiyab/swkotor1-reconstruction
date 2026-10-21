#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AdminStatusPorts{virtual ~AdminStatusPorts()=default;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual Word state(Word)=0;virtual Word name(Word,Word)=0;virtual void assign(Word,Word)=0;virtual Word cstr(Word)=0;virtual void format2(Word,Word,Word,Word)=0;virtual void format3(Word,Word,Word,Word,Word)=0;virtual void send(Word,Word,Word)=0;};
void admin_status_00579920(Memory&,AdminStatusPorts&,Word,Word,Word);
}
