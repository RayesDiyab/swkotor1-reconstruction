#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsTailPorts {virtual ~CreatureStatsTailPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word structure(Word,Word,Word,Word)=0;virtual Word dword_read(Word,Word,Word,Word,Word)=0;virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual Word integer_read(Word,Word,Word,Word,Word)=0;virtual void perception(Word,Word,Word,Word,Word,Word)=0;virtual void repository(Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;virtual Word skill(Word,Word,Word,Word)=0;virtual void movement_rate(Word,Word)=0;virtual void grow(Word,Word)=0;virtual void combat(Word,Word,Word)=0;virtual void loc_destroy(Word)=0;};
Word creature_stats_tail_005b1791(Memory&,CreatureStatsTailPorts&,Word,Word,Word,Word,Word);
}
