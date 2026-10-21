#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct SoundLoadPorts{virtual ~SoundLoadPorts()=default;virtual void ref_construct(Word)=0;virtual Word string(Word,Word,Word,Word,Word,Word)=0;virtual void tag(Word,Word)=0;virtual void string_destroy(Word)=0;virtual Word byte_read(Word,Word,Word,Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word floating(Word,Word,Word,Word,Word)=0;virtual Word byte(Word)=0;virtual void byte_write(Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual void ref_empty(Word)=0;virtual Word ref_read(Word,Word,Word,Word,Word,Word)=0;virtual void ref_assign(Word,Word,Word)=0;virtual void grow(Word,Word)=0;};
Word sound_load_005c9040(Memory&,SoundLoadPorts&,Word self,Word gff,Word root,Word scratch);
}
