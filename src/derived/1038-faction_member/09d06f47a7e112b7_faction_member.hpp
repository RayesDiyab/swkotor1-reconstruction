#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct FactionMemberPorts{virtual ~FactionMemberPorts()=default;virtual Word creature(Word,Word)=0;virtual Word current(Word)=0;virtual void remove(Word,Word)=0;virtual void notification(Word,Word)=0;virtual void append(Word,Word)=0;virtual Word message(Word)=0;virtual Word player(Word,Word)=0;virtual void send(Word,Word,Word,Word,Word,Word)=0;};
void faction_member_005bfa70(Memory&,FactionMemberPorts&,Word,Word,Word);
}
