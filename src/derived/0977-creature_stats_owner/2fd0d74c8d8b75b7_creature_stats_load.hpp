#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureStatsPorts{virtual ~CreatureStatsPorts()=default;virtual Word loc_construct(Word)=0;virtual Word loc_read(Word,Word,Word,Word,Word,Word)=0;virtual void loc_assign(Word,Word)=0;virtual void loc_destroy(Word)=0;virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void set_tag(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word ref_read(Word,Word,Word,Word,Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual Word integer_read(Word,Word,Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;virtual Word continuation_005affbe(Word,Word,Word,Word,Word)=0;};
// Draft coherent identity/race prefix. A mandatory continuation prevents claiming full stats loading.
Word creature_stats_identity_005afce0(Memory&,CreatureStatsPorts&,Word,Word,Word,Word,Word);
}
