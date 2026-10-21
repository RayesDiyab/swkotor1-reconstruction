#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct RunPorts{virtual ~RunPorts()=default;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual void player_status(Word,Word)=0;virtual void admin_status(Word,Word)=0;virtual Word at(Word,Word)=0;virtual void next(Word,Word)=0;virtual void login_player(Word,Word)=0;virtual Word module(Word)=0;virtual void select_player(Word,Word)=0;virtual void party_ready(Word)=0;virtual Word messages(Word)=0;virtual void begin_game(Word,Word)=0;};
Word server_run_004b6270(Memory&,RunPorts&,Word,Word);
}
