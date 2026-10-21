#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ServerEventPorts {
 virtual ~ServerEventPorts()=default;
 virtual void empty(Word)=0; virtual void timestamp(Word,Word)=0;
 virtual Word connection(Word,Word)=0; virtual void copy(Word,Word)=0;
 virtual void cstr(Word)=0; virtual void destroy(Word)=0;
 virtual Word allocate(Word)=0; virtual Word player(Word,Word,Word)=0;
 virtual void add_head(Word,Word)=0; virtual Word at(Word,Word)=0;
 virtual void next(Word,Word)=0; virtual void remove(Word,Word)=0;
 virtual void deleting_destroy(Word,Word)=0;
 virtual void joined(Word,Word)=0; virtual void left(Word,Word,Word)=0;
 virtual Word application_client(Word,Word)=0; virtual void client_left(Word,Word)=0;
 virtual void shutdown(Word,Word,Word)=0; virtual void remove_id(Word,Word)=0;
};
// Original three stack arguments, RET 0xC. Third argument is unused.
void server_event_004b78f0(Memory&,ServerEventPorts&,Word,Word,Word,Word,Word);
}
