#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LocalVariablesLoadPorts{virtual ~LocalVariablesLoadPorts()=default;virtual void location_construct(Word)=0;virtual void location_destroy(Word)=0;virtual void empty(Word)=0;virtual void literal(Word,Word)=0;virtual void destroy(Word)=0;virtual void assign(Word,Word)=0;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual void element(Word,Word,Word,Word)=0;virtual Word string(Word,Word,Word,Word,Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word integer(Word,Word,Word,Word,Word)=0;virtual Word float_bits(Word,Word,Word,Word,Word)=0;virtual void structure(Word,Word,Word,Word)=0;virtual void location_load(Word,Word,Word)=0;virtual Word obtain(Word,Word,Word,Word)=0;virtual void set_integer(Word,Word,Word,Word)=0;virtual void set_location(Word,Word,Word,Word,Word,Word,Word,Word)=0;};
void local_variables_load_0059aa80(Memory&,LocalVariablesLoadPorts&,Word,Word,Word,Word);
}
