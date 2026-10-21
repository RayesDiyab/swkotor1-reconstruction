#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct DoorLoadPorts{virtual ~DoorLoadPorts()=default;virtual void ref_empty(Word)=0;virtual void ref_read(Word,Word,Word,Word,Word,Word)=0;virtual Word load_template(Word,Word)=0;virtual Word localized(Word,Word,Word,Word,Word,Word)=0;virtual void localized_assign(Word,Word)=0;virtual void localized_destroy(Word)=0;virtual void string_empty(Word)=0;virtual Word string_read(Word,Word,Word,Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;virtual Word byte_read(Word,Word,Word,Word)=0;virtual void byte_write(Word,Word)=0;virtual void string_zero(Word)=0;virtual void ref_string(Word,Word)=0;virtual Word fields(Word,Word,Word,Word)=0;virtual Word module(Word)=0;virtual void tag(Word,Word,Word)=0;};
Word door_load_0058c5f0(Memory&,DoorLoadPorts&,Word,Word,Word,Word,Word);
}
