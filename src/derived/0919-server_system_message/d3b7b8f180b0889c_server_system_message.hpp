#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ServerSystemPorts{virtual ~ServerSystemPorts()=default;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual Word length(Word)=0;virtual Word compare(Word,Word,Word)=0;virtual Word network(Word)=0;virtual Word connection(Word,Word)=0;virtual Word counted(Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void status_text(Word,Word,Word)=0;virtual void status(Word,Word)=0;virtual void modules(Word,Word)=0;virtual void players(Word,Word)=0;virtual Word player(Word,Word,Word)=0;virtual Word player_object(Word)=0;virtual void load_module(Word,Word,Word)=0;virtual void run_module(Word)=0;virtual Word save_tail(Word,Word,Word,Word,Word)=0;virtual Word remaining_tail(Word,Word,Word,Word,Word)=0;};
Word system_token_00528130(Memory&,ServerSystemPorts&,Word,Word,Word,Word);
Word system_argument_00528090(Memory&,ServerSystemPorts&,Word,Word,Word,Word,Word);
// Exact entry/status/module-load/run branches. Save and remaining admin/config tails are explicit providers.
Word server_system_00528380(Memory&,ServerSystemPorts&,Word,Word,Word,Word,Word);
}
