#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct BuiltinVariablesPorts{virtual ~BuiltinVariablesPorts()=default;virtual Word structure(Word,Word,Word,Word)=0;virtual Word list(Word,Word,Word,Word)=0;virtual Word count(Word,Word)=0;virtual Word element(Word,Word,Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word byte(Word,Word,Word,Word,Word)=0;virtual void write_byte(Word,Word)=0;};
void builtin_variables_load_0059b0f0(Memory&,BuiltinVariablesPorts&,Word,Word,Word,Word);
}
