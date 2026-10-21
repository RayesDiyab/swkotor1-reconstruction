#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaSoundPorts{virtual ~AreaSoundPorts()=default;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word type(Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual Word construct(Word,Word)=0;virtual void ref_empty(Word)=0;virtual void ref_read(Word,Word,Word,Word,Word,Word)=0;virtual Word template_load(Word,Word)=0;virtual Word load(Word,Word,Word)=0;virtual void byte(Word,Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;virtual void place(Word,Word,Word)=0;virtual void destroy(Word)=0;};
Word area_sounds_00505560(Memory&,AreaSoundPorts&,Word,Word,Word,Word,Word,Word);
}
