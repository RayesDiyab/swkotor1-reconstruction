#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsClassPorts{virtual ~CreatureStatsClassPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word integer_read(Word,Word,Word,Word,Word)=0;virtual Word short_read(Word,Word,Word,Word,Word)=0;virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;virtual void set_class(Word,Word,Word)=0;virtual Word get_class(Word,Word)=0;virtual Word force_class(Word)=0;virtual void loc_destroy(Word)=0;virtual Word continuation_005b0cdb(Word,Word,Word,Word,Word)=0;};
Word creature_stats_classes_005b098b(Memory&,CreatureStatsClassPorts&,Word,Word,Word,Word,Word);
}
