#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsSavePorts {virtual ~CreatureStatsSavePorts()=default;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;virtual void level_history_005b0d49(Word,Word,Word,Word)=0;virtual Word continuation_005b1254(Word,Word,Word,Word,Word)=0;};
Word creature_stats_saves_005b0cdb(Memory&,CreatureStatsSavePorts&,Word,Word,Word,Word,Word);
}
